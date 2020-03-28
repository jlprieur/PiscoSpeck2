/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* asp2_decod_process_cube.cpp
* JLP_Decode Class
*
    int ProcessCube(int iCube);
    int ProcessCube_OnlyNumeri(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCube_OnlyInteg(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeAutoc(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeBisp(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeBispAutoc(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeStatistics(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeSpectro(AWORD* CubeImage, int nx, int ny, int nz_cube);

* To compute autocorrelation, power spectrum and bispectrum of elementary frames
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

#include "jlp_wx_ipanel_filters.h" // subtract_back_model ...
#include "asp2_covermas.h"
#include "jlp_decode.h"

/*
#define DEBUG_2
*/

/*****************************************************************************
//  Called by decode_thread  and DC_Process_inFITS_3DFile()
//
// ProcessingMode = 0 : no processing (only numerisation)
// ProcessingMode = 1 : Only integration
// ProcessingMode = 2 : autoc - crossc, FFT modulus, Long integration
// ProcessingMode = 3 : autoc - crossc, FFT modulus, Long integration, Quadrant
// ProcessingMode = 4 : Bispectrum
// ProcessingMode = 5 : Bispectrum, autoc and quadrant
// ProcessingMode = 6 : Spectroscopy
// ProcessingMode = 7 : SCIDAR Log
// ProcessingMode = 8 : SCIDAR Lin
// ProcessingMode = 9 : Statistics
*****************************************************************************/
int JLP_Decode::ProcessCube(int iCube)
{
register int i;
#ifdef DEBUG
char ss[80];
sprintf(ss, "Start processing cube #%d (ngoodframes=%d) Select=%d %d",
        iCube, ngood_frames, Pset1.Selection_fwhm, Pset1.Selection_maxval);
wxMessageBox(wxString(ss), wxT("DC_ProcessCube"), wxICON_INFORMATION | wxOK);
#endif


// Erase Cube of Good Frames:
nz1_CubeOfGoodFrames = 0;
for(i = 0; i < nx1 * ny1 * nz1_cube; i++) CubeOfGoodFrames[i] = 0;

switch (Pset1.ProcessingMode)
   {
// ProcessingMode = 0 : no processing (only numerisation)
   case 0:
     ProcessCube_OnlyNumeri(CubeImage[iCube], nx1, ny1, nz1_cube);
     break;
// ProcessingMode = 1 : Only integration
   case 1:
     ProcessCube_OnlyInteg(CubeImage[iCube], nx1, ny1, nz1_cube);
     break;
// ProcessingMode = 2 : autoc - crossc, FFT modulus, Long integration
// ProcessingMode = 3 : autoc - crossc, FFT modulus, Long integration, Quadrant
   case 2:
   case 3:
     ProcessCubeAutoc(CubeImage[iCube], nx1, ny1, nz1_cube);
     if(Pset1.SubtractUnresolvedAutoc && (UnresolvedAutocFrame != NULL)) {
        subtract_back_model(autocc_centered_v, UnresolvedAutocFrame,
                            autocc_flattened, nx1, ny1, nx1);
       }
     break;
// ProcessingMode = 4 : Bispectrum
   case 4:
     ProcessCubeBisp(CubeImage[iCube], nx1, ny1, nz1_cube);
     break;
// ProcessingMode = 5 : Bispectrum and autoc
   case 5:
     ProcessCubeBispAutoc(CubeImage[iCube], nx1, ny1, nz1_cube);
     break;
// ProcessingMode = 6 : Spectroscopy
   case 6:
     ProcessCubeSpectro(CubeImage[iCube], nx1, ny1, nz1_cube);
     break;
// ProcessingMode = 7 : Scidar Log
   case 7:
     ProcessCubeScidarLog(CubeImage[iCube], nx1, ny1, nz1_cube);
     break;
// ProcessingMode = 8 : Scidar Lin
   case 8:
     ProcessCubeScidarLin(CubeImage[iCube], nx1, ny1, nz1_cube);
     break;
// ProcessingMode = 9 : Statistics
   case 9:
     ProcessCubeStatistics(CubeImage[iCube], nx1, ny1, nz1_cube);
     break;
   default:
     break;
   }

if(Pset1.SaveFramesToFitsFile != 0)
        SaveCubeToFITSFile(CubeOfGoodFrames, nx1, ny1, nz1_CubeOfGoodFrames);

#ifdef DEBUG
if(ngood_frames <= 60)
sprintf(ss, "End of processing cube #%d ngood_frames=%d nbad=%d",
        iCube, ngood_frames, nbad_frames);
wxMessageBox(wxString(ss), wxT("DC_ProcessCube"), wxICON_INFORMATION | wxOK);
#endif

return (0);
}
/**************************************************************
* Called by ProcessCube()
* ProcessingMode = 0 : No processing, only numerisation
**************************************************************/
int JLP_Decode::ProcessCube_OnlyNumeri(AWORD* CubeImage, int nx, int ny,
                                       int nz_cube)
{
int nx_ny;
register int i;

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

ngood_frames += nz_cube;

// For visualisation (save only one frame):
nx_ny = nx * ny;
for(i = 0; i < nx_ny; i++) ShortExp_v[i] = (double)CubeImage[i + (nz_cube -1) * nx * ny];

return(0);
}
/**************************************************************
* ProcessingMode = 1 : Only integration
* Only integrate the images
* Test in 2007: no extra time spent for doing this processing
* compared to doing nothing (2700 frames/min or 45 frames/sec in Toulouse)
**************************************************************/
int JLP_Decode::ProcessCube_OnlyInteg(AWORD* CubeImage, int nx, int ny,
                                      int nz_cube)
{
int good_frame, nx_ny, iz_nx_ny;
register int i, iz;
#ifdef DEBUG_2
double sum0, sum1;
char ss[80];
char filename[64], comments[80];
#endif

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

nx_ny = nx * ny;

/* Preprocess the elementary frames : */
for(iz = 0; iz < nz_cube; iz++) {
   PreProcessFrame(CubeImage, cplx_data, dble_data, iz, &good_frame);

// Compute FFT only when Selection with FWHM is required:
   if(Pset1.ProcessingMode != 0 && good_frame && Pset1.Selection_fwhm) {
// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
    fftw_fast(cplx_data, ny, nx, 1);
    SelectFrame_with_fwhm(cplx_data, nx, ny, &good_frame);
    }

    iz_nx_ny = iz * nx * ny;

/* Save data to CubeGoodFrames: */
    SaveToCubeOfGoodFrames(&CubeImage[iz_nx_ny], nx, ny, good_frame);

// Integration:
  if(good_frame) {
    ngood_frames++;

/* Long integration : */
    for(i = 0; i < nx_ny; i++) long_int[i] += dble_data[i];

/* Output of the first image as a diagnostic: */
#ifdef DEBUG_2
  if(ngood_frames == 1.)
   {
    strcpy(filename,"first_image.fits");
    strcpy(comments,"   ");
    JLP_D_WRITEIMAG0(dble_data, nx, ny, filename, comments);
    wxMessageBox(wxT("First image"), wxT("DC_ProcessCube_OnlyInteg"),
                 wxICON_INFORMATION | wxOK);
   }
#endif

/* Lucky imaging */
   if(Pset1.LuckyImaging) BuildUpLuckyImage(dble_data, cplx_data, nx, ny,
                                           ngood_frames);

/* Direct Vector */
   if(Pset1.DirectVector) BuildUpDirectVector(dble_data, nx, ny);

#ifdef DEBUG_2
   if(ngood_frames == 100) {
    sum0 = 0.;
    for(i = 0; i < nx_ny; i++) sum0 += dble_data[i];
    sum0 /= nx_ny;
    sum1 = 0.;
    for(i = 0; i < nx_ny; i++) sum1 += long_int[i];
    sum1 /= nx_ny;
    sprintf(ss, "DEBUG_2/ ngood_frames = %d mean_frame=%f mean_long=%f",
            ngood_frames, sum0, sum1);
    wxMessageBox(wxString(ss), wxT("DC_OnlyInteg/DEBUG_2"),
                 wxICON_INFORMATION | wxOK);
    }
#endif
    }
  else nbad_frames++;

// For visualisation (save only one frame):
  if(iz == 0) {
   for(i = 0; i < nx_ny; i++) {
    ShortExp_v[i] = dble_data[i];
    }
   }
  }  /* EOF loop on iz */

return(0);
}
/**************************************************************
* ProcessCubeAutoc for:
* ProcessingMode = 2 : autoc - crossc, FFT modulus, long integration
* ProcessingMode = 3 : autoc - crossc, FFT modulus, long integration, quadrant
*
* Compute autocorrelation - crosscorrelation
* (Warning: if Pset1.ProcessingMode = 5, the autocorrelation is
*   computed by ProcessCubeBispAutoc)
**************************************************************/
int JLP_Decode::ProcessCubeAutoc(AWORD* CubeImage, int nx, int ny, int nz_cube)
{
double ww;
double cre, cim, cre_sq, cim_sq;
int good_frame, k_data_old;
long int iz_nx_ny, nx_ny;
register int i, iz;

nx_ny = nx * ny;

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

/* Initialize the index of FT in data_old FIFO (First_in/First_out) array: */
k_data_old = 0;


/***************************************************************/
/* Main loop on the frames: */
for(iz = 0; iz < nz_cube; iz++)
  {
  PreProcessFrame(CubeImage, cplx_data, dble_data, iz, &good_frame);

// Compute FFT and, when required, apply FWHM selection:
  if(good_frame) {
// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
    fftw_fast(cplx_data, ny, nx, 1);
    SelectFrame_with_fwhm(cplx_data, nx, ny, &good_frame);
    }

  iz_nx_ny = iz * nx * ny;

/* Save data to CubeGoodFrames: */
  SaveToCubeOfGoodFrames(&CubeImage[iz_nx_ny], nx, ny, good_frame);

  if(good_frame) {
   ngood_frames++;

/* Long integration : */
   for(i = 0; i < nx_ny; i++) long_int[i] += dble_data[i];

/* Lucky imaging */
   if(Pset1.LuckyImaging) BuildUpLuckyImage(dble_data, cplx_data, nx, ny,
                                           ngood_frames);

/* Direct Vector */
   if(Pset1.DirectVector) BuildUpDirectVector(dble_data, nx, ny);


// Auto-correlation and quadrant:
   if(Pset1.ProcessingMode == 3) {
     for(i = 0; i < nx_ny; i++) {
      ww = CubeImage[i + iz_nx_ny];
      c_re(cplx_data_sq[i]) = ww * ww;
      c_im(cplx_data_sq[i]) = 0.;
      }
// Fourrier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
     fftw_fast(cplx_data_sq, ny, nx, 1);

// JLP2007: attempt to improve speed:
     for(i = 0; i < nx_ny; i++) {
      cre = c_re(cplx_data[i]);
      cim = c_im(cplx_data[i]);
      cre_sq = c_re(cplx_data_sq[i]);
      cim_sq = c_im(cplx_data_sq[i]);
      Quadrant_Re[i] += (cre * cre_sq + cim * cim_sq);
      Quadrant_Im[i] += (cre * cim_sq - cre_sq * cim);
      }
    } /* EOF case of Pset1.ProcessingMode == 3 */

// Compute and integrate the power spectrum:
    update_modsq(cplx_data, modsq, nx, ny);

// Compute and integrate the cross-spectrum:
    if(iz >= DATA_KK-1)
       update_cross_spectrum(cplx_data, data_old_Re, data_old_Im, CrossSpec_Re,
                             CrossSpec_Im, nx, ny, DATA_KK, k_data_old);

// Prepare next step:
     rotate_data(cplx_data, data_old_Re, data_old_Im, nx, ny, DATA_KK, &k_data_old);

/****************** End of loop with ifil (all input files processed) ****/
   } // End of good frame
   else nbad_frames++;

// For visualisation (save only one frame):
  if(iz == 0) {
   for(i = 0; i < nx_ny; i++) {
    ShortExp_v[i] = dble_data[i];
    }
   }
}  // End of loop on nz_cube frames

/************************************************************************/
// Just for display: (The time spent here is negligible, tested in 2007)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the autocorrelation (FFT-1 of the power-spectrum):
  for(i = 0; i < nx_ny; i++) {
      c_re(cplx_data[i]) = modsq[i];
      c_im(cplx_data[i]) = 0;
      }

// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny, nx, -1);
   for(i = 0; i < nx_ny; i++) autocc[i] = c_re(cplx_data[i]);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Compute the crosscorrelation (FFT-1 from the cross-spectrum):
// Transfer to data array, using working window:
// and resetting the imaginary part for the FFT:

  for(i = 0; i < nx_ny; i++) {
      c_re(cplx_data[i]) = CrossSpec_Re[i];
      c_im(cplx_data[i]) = CrossSpec_Im[i];
      }

// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny, nx, -1);

// Autocorrelation - Crosscorrelation
   for(i = 0; i < nx_ny; i++) autocc[i] -= c_re(cplx_data[i]);

// Centered autocc and modsq used for visualisation:
   RECENT_FFT_DOUBLE(autocc, autocc_centered_v, &nx, &ny, &nx);
   RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx, &ny, &nx);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the quadrant:
if(Pset1.ProcessingMode == 3)
   {
     for(i = 0; i < nx_ny; i++)
       {
// By setting the real part to zero, get f(x)-f(-x):
//        c_re(data[i]) = c_re(data_quad[i]);
        c_re(cplx_data[0]) = 0.;
        c_im(cplx_data[i]) = Quadrant_Im[i];
       }

// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny, nx, -1);
   for(i = 0; i < nx_ny; i++) quadr_centered_v[i] = c_re(cplx_data[i]);
   RECENT_FFT_DOUBLE(quadr_centered_v, quadr_centered_v, &nx, &ny, &nx);
   }

   return(0);
 }
/**************************************************************
* ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
* Compute bispectrum
***************************************************************/
int JLP_Decode::ProcessCubeBisp(AWORD* CubeImage, int nx, int ny, int nz_cube)
{
long int iz_nx_ny, nx_ny;
int good_frame;
/* Images nx*ny, irmax=25, ngmax=187566 : */
register int i, iz;
char err_message[128];
#ifdef DEBUG_2
char filename[64], comments[80];
#endif

nx_ny = nx * ny;

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

/***************************************************************/
/* Main loop on the frames: */
for(iz = 0; iz < nz_cube; iz++)
  {
  PreProcessFrame(CubeImage, cplx_data, dble_data, iz, &good_frame);

// Compute FFT and, when required, apply FWHM selection:
  if(good_frame) {
// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
    fftw_fast(cplx_data, ny, nx, 1);
    SelectFrame_with_fwhm(cplx_data, nx, ny, &good_frame);
    }

    iz_nx_ny = iz * nx * ny;

/* Save data to CubeGoodFrames: */
    SaveToCubeOfGoodFrames(&CubeImage[iz_nx_ny], nx, ny, good_frame);

  if(good_frame) {
    ngood_frames++;

/* Long integration : */
    for(i = 0; i < nx_ny; i++) long_int[i] += dble_data[i];

/* Output of the first image as a diagnostic: */
#ifdef DEBUG_2
  if(ngood_frames == 1.)
   {
    strcpy(filename,"first_image.fits");
    strcpy(comments,"   ");
    JLP_D_WRITEIMAG0(dble_data, nx, ny, filename, comments);
    wxMessageBox(wxT("First image"), wxT("DC_ProcessCube_Bisp"),
                 wxICON_INFORMATION | wxOK);
   }
#endif

/* Processing the data now:  bispec3 is without photon bias correction */
/* bispec4 in "jlp_cover_mask.c" */
    bispec3_fftw(cplx_data, modsq, snrm, nx, ny, bispp, Pset1.ir_bisp, nbeta,
                 ngamma, err_message);
// EOF "good_frame" case
    } else nbad_frames++;

// For visualisation (save only one frame):
  if(iz == 0) {
   for(i = 0; i < nx_ny; i++) {
    ShortExp_v[i] = dble_data[i];
    }
   }
/****************** End of loop with iz ****/
   }

// Centered modsq used for visualisation:
   RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx, &ny, &nx);

   return(0);
 }
/**************************************************************
* ProcessingMode = 5 : Bispectrum, autoc, quadrant, FFT modulus, long integration
* Compute bispectrum and autoc
***************************************************************/
int JLP_Decode::ProcessCubeBispAutoc(AWORD* CubeImage, int nx, int ny, int nz_cube)
{
double ww;
double cre, cim, cre_sq, cim_sq;
int good_frame, k_data_old;
long int iz_nx_ny, nx_ny;
char err_message[128];
register int i, iz;

nx_ny = nx * ny;

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

/* Initialize the index of FT in data_old FIFO (First_in/First_out) array: */
k_data_old = 0;

/***************************************************************/
/* Main loop on the frames: */
for(iz = 0; iz < nz_cube; iz++)
  {
  PreProcessFrame(CubeImage, cplx_data, dble_data, iz, &good_frame);

// Compute FFT and, when required, apply FWHM selection:
  if(good_frame) {
// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
    fftw_fast(cplx_data, ny, nx, 1);
    SelectFrame_with_fwhm(cplx_data, nx, ny, &good_frame);
    }

  iz_nx_ny = iz * nx * ny;

/* Save data to CubeGoodFrames: */
  SaveToCubeOfGoodFrames(&CubeImage[iz_nx_ny], nx, ny, good_frame);

  if(good_frame) {
   ngood_frames++;

/* Long integration : */
   for(i = 0; i < nx_ny; i++) long_int[i] += dble_data[i];

/* Lucky imaging */
   if(Pset1.LuckyImaging) BuildUpLuckyImage(dble_data, cplx_data, nx, ny,
                                           ngood_frames);

/* Direct Vector */
   if(Pset1.DirectVector) BuildUpDirectVector(dble_data, nx, ny);

// Auto-correlation and quadrant:
   for(i = 0; i < nx_ny; i++) {
      ww = dble_data[i];
      c_re(cplx_data_sq[i]) = ww * ww;
      c_im(cplx_data_sq[i]) = 0.;
      }

// Fourrier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
     fftw_fast(cplx_data_sq, ny, nx, 1);

// JLP2007: attempt to improve speed:
     for(i = 0; i < nx_ny; i++) {
      cre = c_re(cplx_data[i]);
      cim = c_im(cplx_data[i]);
      cre_sq = c_re(cplx_data_sq[i]);
      cim_sq = c_im(cplx_data_sq[i]);
      Quadrant_Re[i] += (cre * cre_sq + cim * cim_sq);
      Quadrant_Im[i] += (cre * cim_sq - cre_sq * cim);
      }

/* Computing the bispectrum without photon bias correction */
/* bispec3_fftw in "jlp_cover_mask.c" */
    bispec3_fftw(cplx_data, modsq, snrm, nx, ny, bispp, Pset1.ir_bisp, nbeta,
                 ngamma, err_message);

// Compute and integrate the cross-spectrum:
    if(iz >= DATA_KK-1)
       update_cross_spectrum(cplx_data, data_old_Re, data_old_Im, CrossSpec_Re,
                             CrossSpec_Im, nx, ny, DATA_KK, k_data_old);

// Prepare next step:
     rotate_data(cplx_data, data_old_Re, data_old_Im, nx, ny, DATA_KK, &k_data_old);

/****************** End of loop with ifil (all input files processed) ****/
   } // End of good frame
   else nbad_frames++;

// For visualisation (save only one frame):
  if(iz == 0) {
   for(i = 0; i < nx_ny; i++) {
    ShortExp_v[i] = dble_data[i];
    }
   }

}  // End of loop on nz_cube frames

// Just for display: (The time spent here is negligible, tested in 2007)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the autocorrelation (FFT-1 from the cross-spectrum):
  for(i = 0; i < nx_ny; i++) {
      c_re(cplx_data[i]) = modsq[i];
      c_im(cplx_data[i]) = 0;
      }
// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny, nx, -1);
   for(i = 0; i < nx_ny; i++) autocc[i] = c_re(cplx_data[i]);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the crosscorrelation (FFT-1 from the cross-spectrum):
// Transfer to data array, using working window:
// and resetting the imaginary part for the FFT:
  for(i = 0; i < nx_ny; i++) {
      c_re(cplx_data[i]) = CrossSpec_Re[i];
      c_im(cplx_data[i]) = CrossSpec_Im[i];
      }
// JLP99: warning: NY,NX!!!!
   fftw_fast(cplx_data, ny, nx, -1);
// Autocorrelation - Crosscorrelation
   for(i = 0; i < nx_ny; i++) autocc[i] -= c_re(cplx_data[i]);

// Centered autocc and modsq used for visualisation:
   RECENT_FFT_DOUBLE(autocc, autocc_centered_v, &nx, &ny, &nx);
   RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx, &ny, &nx);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Computing the quadrant:
 for(i = 0; i < nx_ny; i++) {
// By setting the real part to zero, get f(x)-f(-x):
//        c_re(data[i]) = c_re(data_quad[i]);
   c_re(cplx_data[0]) = 0.;
   c_im(cplx_data[i]) = Quadrant_Im[i];
 }

// JLP99: warning: NY,NX!!!!
 fftw_fast(cplx_data, ny, nx, -1);
 for(i = 0; i < nx_ny; i++) quadr_centered_v[i] = c_re(cplx_data[i]);
 RECENT_FFT_DOUBLE(quadr_centered_v, quadr_centered_v, &nx, &ny, &nx);

return(0);
}
/**************************************************************
* ProcessingMode = 9 : statistics
* Compute Statistics on the images (for further image selection)
*
**************************************************************/
int JLP_Decode::ProcessCubeStatistics(AWORD* CubeImage, int nx, int ny,
                                      int nz_cube)
{
long int iz_nx_ny, nx_ny;
int good_frame;
register int i, iz;

nx_ny = nx * ny;

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

/***************************************************************/
/* Main loop on the frames: */
for(iz = 0; iz < nz_cube; iz++) {
  PreProcessFrame(CubeImage, cplx_data, dble_data, iz, &good_frame);

// Compute FFT and, when required, apply FWHM selection:
   if(good_frame) {
// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
     fftw_fast(cplx_data, ny, nx, 1);
     SelectFrame_with_fwhm(cplx_data, nx, ny, &good_frame);
    }

   iz_nx_ny = iz * nx * ny;

/* Save data to CubeGoodFrames: */
   SaveToCubeOfGoodFrames(&CubeImage[iz_nx_ny], nx, ny, good_frame);

   if(good_frame) {
     ngood_frames++;
   } else {
     nbad_frames++;
   }

  }

/* "Long" integration (only the last one): */
  iz_nx_ny = (nz_cube-1) * nx_ny;
  for(i = 0; i < nx_ny; i++) long_int[i] = dble_data[i];

/* Modsq (only the last one)*/
  for(i = 0; i < nx_ny; i++)
     modsq[i] = c_re(cplx_data[i]) * c_re(cplx_data[i])
               + c_im(cplx_data[i]) * c_im(cplx_data[i]);

// Centered modsq used for visualisation:
  RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx, &ny, &nx);

// For visualisation (save only one frame):
   for(i = 0; i < nx_ny; i++) {
    ShortExp_v[i] = dble_data[i];
    }

return(0);
}
/**************************************************************
* ProcessingMode = 6 : spectroscopy
* Compute 1-D bispectrum for spectroscopic mode
***************************************************************/
int JLP_Decode::ProcessCubeSpectro(AWORD* CubeImage, int nx, int ny,
                                   int nz_cube)
{
long int nx_ny_num, nx_ny;
/* Images nx*ny, irmax=25, ngmax=187566 : */
register int i, ix, iy, iz;
#ifdef DEBUG_2
char filename[64], comments[80];
#endif

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

/***************************************************************/
/* Main loop on the frames: */
nx_ny = nx * ny;
for(iz = 0; iz < nz_cube; iz++)
  {
  ngood_frames++;

// Offset and Flat Field correction of current elementary frame:
  OffsetAndFlatFieldCorrection(&CubeImage[iz * Pset1.nx_num * Pset1.ny_num],
                               dble_data, Pset1.nx_num,Pset1.ny_num);

/* Rotation so that the slit is parallel to Oy
   and the dispersion parallel to Ox (hence ir < ny) */
/* After rotation the spectrum is horizontal, and the slit vertical: */
   nx_ny_num = Pset1.nx_num * Pset1.ny_num;
     for(i = 0; i < nx_ny_num; i++)
       work1[i] = dble_data[i];

// For numerisation, larger size:
    rotate_spectro_ima(work1, real_part, Pset1.nx_num, Pset1.ny_num, nx, ny);

/* Long integration : */
  for(i = 0; i < nx_ny; i++) long_int[i] += real_part[i];

/* Output of the first image as a diagnostic: */
#ifdef DEBUG_2
  if(ngood_frames == 1.)
   {
    strcpy(filename,"first_image.fits");
    strcpy(comments,"   ");
    JLP_D_WRITEIMAG0(long_int, nx, ny, filename, comments);
    wxMessageBox(wxT("First image"), wxT("DC_ProcessCube_Spectro"),
                 wxICON_INFORMATION | wxOK);
   }
#endif

// Transfer to cplx_data array column by column
// and resetting the imaginary part for the FFT:
  for(ix = 0; ix < nx; ix++)
  {
     for(iy = 0; iy < ny; iy++)
       {
       c_re(cplx_data[iy]) = real_part[ix + iy * nx];
       c_im(cplx_data[iy]) = 0.;
       }

// Fourrier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
     fftw_fast(cplx_data, 1, ny, 1);

// Transfer back to storage arrays:
     for(iy = 0; iy < ny; iy++)
       {
       real_part[ix + iy * nx] = c_re(cplx_data[iy]);
       imag_part[ix + iy * nx] = c_im(cplx_data[iy]);
       }
// End of loop on ix:
  }

/* Processing the data now:  without photon bias correction */
 bispec_1D_Y(real_part, imag_part, modsq, snrm, nx, ny, bispp, Pset1.ir_bisp,
             nbeta, ngamma);
/* Debug...
#ifdef TTR
  for(i = 0; i < nx_ny; i++)
     modsq[i] += real_part[i] * real_part[i] + imag_part[i] * imag_part[i];
#endif
*/

/****************** End of loop with ifil (all input files processed) ****/
   }

// Centered modsq used for visualisation:
   RECENT_FFT_1D_Y(modsq, modsq_centered_v, &nx, &ny, &nx);

// For visualisation (save only one frame):
   for(i = 0; i < nx_ny; i++) {
     ShortExp_v[i] = dble_data[i];
     }

return(0);
}
