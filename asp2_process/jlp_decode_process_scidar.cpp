/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* jlp_decod_process_scidar.cpp
* JLP_Decode Class
*
    int ProcessCubeScidarLog(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeScidarLin(AWORD* CubeImage, int nx, int ny, int nz_cube);

*
* ProcessingMode = 0 : no processing (only numerisation)
* ProcessingMode = 1 : Only integration
* ProcessingMode = 2 : autoc - crossc, FFT modulus, long integration
* ProcessingMode = 3 : autoc - crossc, FFT modulus, long integration, quadrant
* ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
* ProcessingMode = 5 : Bispectrum, autoc, quadrant
* ProcessingMode = 6 : Spectroscopy
* ProcessingMode = 7 : SCIDAR Log
* ProcessingMode = 8 : SCIDAR Lin
* ProcessingMode = 9 : Statistics
*
* JLP
* Version 16-01-2016
---------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include "jlp_decode.h"

/* Cannot be used in debug mode
#define DEBUG
*/
static int Scidar_CreatePupilMask(double **pupil_mask0, int nx0, int ny0);

/**************************************************************************
* ProcessingMode = 7 : SCIDAR Log
*
* Compute auto-correlation and cross-correlation
* Auto-correlation of the logarithm
* Work out the scidar function of each sequence (nz_cube)
*
* Process CubeImage twice:
*  - First step: compute a mean image
*  - Second step: compute the auto-correlation of the fluctuations
****************************************************************************/
int JLP_Decode::ProcessCubeScidarLog(AWORD* CubeImage, int nx, int ny,
                                     int nz_cube)
{
double *tmp, *tmp2, *sumsq_img, *sum_img, *pupil_mask, *mean_image;
int *frame_is_good, good_frame;
float n_mean, w1, w2;
double ww;
register int i, j, iz, istep;
int nx2, ny2, k_data_old;
long int nx_ny, iz_nx_ny;
char err_message[128];

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

nx2 = 2 * nx;
ny2 = 2 * ny;
nx_ny = nx * ny;
/* Initialize index of FT data set in data_old FIFO (First_in First_out)
* array: */
k_data_old = 0;

// Array with good frames:
frame_is_good = new int[nz_cube];
for(iz = 0; iz < nz_cube; iz++) frame_is_good[iz] = 1;

tmp = new double[nx*ny];
tmp2 = new double[nx2*ny2];

// Create the pupil mask:
Scidar_CreatePupilMask(&pupil_mask, nx, ny);

/***************************************************************/
// First step: compute a mean image:
// Second step: compute the auto-correlation of the fluctuations
mean_image = new double[nx*ny];
for(i = 0; i < nx_ny; i++) mean_image[i] = 0.;
//for(istep = 0; istep < 2; istep++)
for(istep = 1; istep < 2; istep++)
{
  n_mean = 0.;
  fftw_shutdown();
  fftw_setup(Pset1.FFTW_directory, (ny+istep*ny), (nx+istep*nx), err_message);

/***************************************************************/
/* Main loop on the frames: */
  for(iz = 0; iz < nz_cube; iz++)
  {

/* Temporary array: */
  iz_nx_ny = iz * nx * ny;
  for(i = 0; i < nx_ny; i++) tmp[i] = (float)CubeImage[i + iz_nx_ny];

// First go, check whether mean level is between boundaries:
  if(istep == 0 && Pset1.Selection_maxval) {
    PreProcessFrame(CubeImage, cplx_data, dble_data, iz, &good_frame);
    frame_is_good[iz] = good_frame;

    iz_nx_ny = iz * nx * ny;

/* Save data to CubeGoodFrames: */
    SaveToCubeOfGoodFrames(&CubeImage[iz_nx_ny], nx, ny, good_frame);

// Computes FFT and check there are no bad horizontal stripes
// Transfer to cplx_data array, using working window:
// and resetting the imaginary part for the FFT:
   if(good_frame)
     {
     ngood_frames++;
// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
      fftw_fast(cplx_data, ny, nx, 1);

// If spikes in the middle, throws it away:
// Computation of the mean square modulus:
      for(i = 0; i < nx_ny; i++)
        tmp2[i] = c_re(cplx_data[i]) * c_re(cplx_data[i])
                 + c_im(cplx_data[i]) * c_im(cplx_data[i]);
// Criterium: examine central column of power spectrum
// (spurious horizontal lines produce a vertical brightness enhancement
// of the power spectrum)
      w1 = 0.;
      w2 = 0.;
      for(j = 0; j < ny/4; j++) {
        w1 += tmp2[(nx/2) + j*nx] + tmp2[(nx/2+1) + j*nx]
            + tmp2[(nx/2+2) + j*nx] + tmp2[(nx/2+3) + j*nx];
        w2 += tmp2[(nx/2+5) + j*nx] + tmp2[(nx/2+6) + j*nx]
            + tmp2[(nx/2+7) + j*nx] + tmp2[(nx/2+8) + j*nx];
       }
// 1.01 slightly too large
// 1.005 much too small
// 1.008 seems good:
       if(w2 < 1.008*w1) frame_is_good[iz] = 0;
// End of frame_is_good:
       }
// End of istep=0 && Selection_maxval
  }

// First go, compute mean value:
  if(frame_is_good[iz])
  {
// Take the logarithm of the image (offset equal to 12. here):
     for(i = 0; i < nx_ny; i++)
       {
       ww = (double)tmp[i] - 12.;
       if(ww < 0.1) ww = 0.1;
       if(ww > 88.) ww = 88.;
       tmp[i] = log10(ww);
       }

// Mask:
     for(i = 0; i < nx_ny; i++) tmp[i] *= pupil_mask[i];

// Mean image:
   if(istep == 0)
     {
     n_mean += 1.;
     for(i = 0; i < nx_ny; i++) mean_image[i] += tmp[i];
     }
   else
     {
     ngood_frames++;
// Long integration :
     for(i = 0; i < nx_ny; i++) long_int[i] += tmp[i];

// Subtraction of the mean image:
    for(i = 0; i < nx_ny; i++) tmp[i] -= mean_image[i];

// Scidar: Long integration of the fluctuations: (Scidar Log mode only)
     for(i = 0; i < nx_ny; i++) scidar_lf[i] += tmp[i];

// Re-scaling to larger array
     for(i = 0; i < nx2 * ny2; i++) tmp2[i]=0.;
     for(j=0; j < ny; j++)
       for(i=0; i < nx; i++)
         tmp2[(i + nx/2) + (j + ny/2) * nx2] = tmp[i + j * nx];

// Transfer to cplx_data array, using working window:
// and resetting the imaginary part for the FFT:
  for(i = 0; i < nx2 * ny2; i++)
      {
      c_re(cplx_data[i]) = tmp2[i];
      c_im(cplx_data[i]) = 0.;
      }

// Fourrier Transform: (size=2-D, 1=direct FFT)
 // JLP99: warning: NY,NX!!!!
 fftw_fast(cplx_data, ny2, nx2, 1);

// Compute and integrate the power spectrum:
 update_modsq(cplx_data, modsq, nx2, ny2);

// Compute and integrate the cross-spectrum:
#ifdef SCIDAR_CROSSC
   if(iz >= DATA_KK-1)
       update_cross_spectrum(cplx_data, data_old_Re, data_old_Im, CrossSpec_Re,
                             CrossSpec_Im, nx2, ny2, DATA_KK_SCIDAR, k_data_old);
//
// Prepare next step:
   rotate_data(cplx_data, data_old_Re, data_old_Im, nx2, ny2, DATA_KK_SCIDAR,
               &k_data_old);
#endif

// End of case: istep != 0:
  }

// End of case: mean value is correct:
  }
/****************** End of loop with iz (all input files processed) ****/
   }
// Mean image:
 if(istep == 0 && n_mean)
       for(i = 0; i < nx_ny; i++) mean_image[i] /= (double)n_mean;
/****************** End of loop with istep ****/
}

if(ngood_frames > 0)
{
// Just for display:
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the autocorrelation (FFT-1 from the cross-spectrum):
  for(i = 0; i < nx2 * ny2; i++)
      {
      c_re(cplx_data[i]) = modsq[i]/ngood_frames;
      c_im(cplx_data[i]) = 0;
      }
// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny2, nx2, -1);
   for(i = 0; i < nx2 * ny2; i++) tmp2[i] = c_re(cplx_data[i]);
   RECENT_FFT_DOUBLE(tmp2, tmp2, &nx2, &ny2, &nx2);

// Rescaling for display:
     for(j=0; j < ny; j++)
       for(i=0; i < nx; i++)
         autocc[i + j * nx] = tmp2[(i + nx/2) + (j + ny/2) * nx2];

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the crosscorrelation (FFT-1 from the cross-spectrum):
// Transfer to cplx_data array, using working window:
// and resetting the imaginary part for the FFT:
#ifdef SCIDAR_CROSSC
  for(i = 0; i < nx2 * ny2; i++)
      {
      c_re(cplx_data[i]) = CrossSpec_Re[i];
      c_im(cplx_data[i]) = CrossSpec_Im[i];
      }
// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny2, nx2, -1);
// Crosscorrelation
   for(i = 0; i < nx2 * ny2; i++) crossc[i] = c_re(cplx_data[i]);
   RECENT_FFT_DOUBLE(crossc, crossc, &nx2, &ny2, &nx2);
#endif

// Centered modsq used for visualisation:
   RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx2, &ny2, &nx2);

// For visualisation (save only one frame):
  for(i = 0; i < nx_ny; i++) {
   ShortExp_v[i] = dble_data[i];
   }

// End of ngood_frames > 0
}

   delete[] frame_is_good;
   delete[] mean_image;
   delete[] tmp2;
   delete[] tmp;
   delete[] pupil_mask;
   return(0);
 }
/*****************************************************************************
* ProcessingMode = 8 : SCIDAR Lin
* Compute autocorrelation and crosscorrelation
* Use "normal" auto-correlation
*****************************************************************************/
int JLP_Decode::ProcessCubeScidarLin(AWORD* CubeImage, int nx, int ny, int nz_cube)
{
double *tmp, *tmp2, *pupil_mask, back1, back2;
long int iz_nx_ny, nx_ny;
register int i, j, iz;
int nx2, ny2, good_frame, k_data_old;

nx2 = 2 * nx;
ny2 = 2 * ny;
nx_ny = nx * ny;

/* Initialize index of FT data set in data_old array */
k_data_old = 0;

// Create the pupil mask:
Scidar_CreatePupilMask(&pupil_mask, nx, ny);

tmp = new double[nx*ny];
tmp2 = new double[nx2*ny2];

// Setup all parameters and erase all the arrays
// (only the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

/***************************************************************/
/* Main loop on the frames: */
  for(iz = 0; iz < nz_cube; iz++)
  {
    PreProcessFrame(CubeImage, cplx_data, dble_data, iz, &good_frame);

    iz_nx_ny = iz * nx * ny;

/* Save data to CubeGoodFrames: */
    SaveToCubeOfGoodFrames(&CubeImage[iz_nx_ny], nx, ny, good_frame);

// Main processing:
  if(good_frame)
  {
   ngood_frames++;

/* Copy to temporary array: */
  for(i = 0; i < nx_ny; i++) tmp[i] = dble_data[i];

// Find the background value with two squares in the bottom left/right of
// the image:
   back1 = 0.;
   for(j = 2; j < 12; j++)
     for(i = 2; i < 12; i++) back1 += tmp[i+j*nx];
   back2 = 0.;
   for(j = 2; j < 12; j++)
     for(i = nx-12; i < nx-2; i++) back2 += tmp[i+j*nx];
// Subtract the minimum value:
   if(back2 < back1) back1 = back2;
   back1 /= 100.;
   for(i = 0; i < nx_ny; i++) tmp[i] -= back1;

// Multiplication with the pupil mask:
   for(i = 0; i < nx_ny; i++) tmp[i] *= pupil_mask[i];

/* Long integration : */
   for(i = 0; i < nx_ny; i++) long_int[i] += tmp[i];

   for(i = 0; i < nx2 * ny2; i++) tmp2[i]=0.;

   for(j = 0; j < ny; j++)
     for(i = 0; i < nx; i++)
       tmp2[(i + nx/2) + (j + ny/2) * nx2] = tmp[i + j * nx];

// Transfer to data array, using working window:
// and resetting the imaginary part for the FFT:
   for(i = 0; i < nx2 * ny2; i++)
     {
     c_re(cplx_data[i]) = tmp2[i];
     c_im(cplx_data[i]) = 0.;
     }

// Fourrier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny2, nx2, 1);

// Compute and integrate the power spectrum:
 update_modsq(cplx_data, modsq, nx2, ny2);

// Compute and integrate the cross-spectrum:
#ifdef SCIDAR_CROSSC
   if(iz >= DATA_KK-1)
       update_cross_spectrum(cplx_data, data_old_Re, data_old_Im, CrossSpec_Re,
                             CrossSpec_Im, nx2, ny2, DATA_KK_SCIDAR, k_data_old);
//
// Prepare next step:
   rotate_data(cplx_data, data_old_Re, data_old_Im, nx2, ny2, DATA_KK_SCIDAR,
               &k_data_old);
#endif
// End of case: good_frame == 1, i.e. mean value is correct
   } else nbad_frames++;
/****************** End of loop with ifil (all input files processed) ****/
}

if(ngood_frames > 0)
{
// Just for display:

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the autocorrelation (FFT-1 from the cross-spectrum):
  for(i = 0; i < nx2 * ny2; i++)
     {
     c_re(cplx_data[i]) = modsq[i]/ngood_frames;
     c_im(cplx_data[i]) = 0;
     }
// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny2, nx2, -1);
   for(i = 0; i < nx2 * ny2; i++) tmp2[i] = c_re(cplx_data[i]);
   RECENT_FFT_DOUBLE(tmp2, tmp2, &nx2, &ny2, &nx2);

// Rescaling for display:
   for(j = 0; j < ny; j++)
     for(i = 0; i < nx; i++)
        autocc[i + j * nx] = tmp2[(i + nx/2) + (j + ny/2) * nx2];

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the crosscorrelation (FFT-1 from the cross-spectrum):
// Transfer to cplx_data array, using working window:
// and resetting the imaginary part for the FFT:
#ifdef SCIDAR_CROSSC
   for(i = 0; i < nx2 * ny2; i++)
      {
      c_re(cplx_data[i]) = CrossSpec_Re[i];
      c_im(cplx_data[i]) = CrossSpec_Im[i];
      }
// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny2, nx2, -1);
// Crosscorrelation
   for(i = 0; i < nx2 * ny2; i++) crossc[i] = c_re(cplx_data[i]);
   RECENT_FFT_DOUBLE(crossc, crossc, &nx2, &ny2, &nx2);
#endif

// Centered modsq used for visualisation:
   RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx2, &ny2, &nx2);

// For visualisation (save only one frame):
  for(i = 0; i < nx_ny; i++) {
    ShortExp_v[i] = dble_data[i];
   }

// End of ngood_frames > 0
}

delete[] pupil_mask;
delete[] tmp2;
delete[] tmp;

return(0);
}
/************************************************************************************
* Pupil mask
*************************************************************************************/
static int Scidar_CreatePupilMask(double **pupil_mask0, int nx0, int ny0)
{
int i, j, ic, jc;
double x2, y2, ww;

// Pupil mask (settings for special set of parameters 256x256, etc):
// computed by Gerard Daigne (128x128 for the TBL oservations
*pupil_mask0 = new double[nx0*ny0];

  for(i = 0; i < nx0*ny0; i++) (*pupil_mask0)[i] = 0.;

  for(i = 0; i < nx0; i++)
    {
      for(j = 0; j < ny0; j++)
         {
// Ellipse centered in the middle, whose major axis is aligned with
// first bisectrix with a=128, b=112:
         ic = i - (nx0/2 - 5);
         jc = j - (ny0/2 + 5);
         x2 = (float)(ic + jc)/128.;
         y2 = (float)(ic - jc)/112.;
         ww = 0.5 * (x2 * x2 + y2 * y2);
         if(ww <= 1.) (*pupil_mask0)[i + j * nx0] = 1.;
// Hole of radius=48. (48*48=2304)
         ic = i - nx0/2;
         jc = j - ny0/2;
         ww = jc*jc + ic*ic;
         if(ww <= 2304.) (*pupil_mask0)[i + j * nx0] = 0.;
         }
    }

return(0);
}
