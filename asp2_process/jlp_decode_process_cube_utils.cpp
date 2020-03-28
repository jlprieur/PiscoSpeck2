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
    int ProcessCubeScidarLog(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeScidarLin(AWORD* CubeImage, int nx, int ny, int nz_cube);

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
* Version 06-09-2015
---------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include "jlp_decode.h"

/* Cannot be used in debug mode
#define DEBUG
*/
//*************************************************************
// Rotation of the input image of "slangle"
//
// spangle: angle of the spectrum relative to OX
// slangle: angle of the slit relative to OX: here that is only this one
//          which is taken into account
//*************************************************************
int JLP_Decode::rotate_spectro_ima(double *in_1, double *out_2,
                                   int nx0, int ny0, int nx2, int ny2)
{
/* Rotation matrix: */
float c1, c2, c3, c4, c5, c6;
float back1, back2, ix1, iy1, dx, dy, sum, wt, wtsum;
double rot_angle;
int ixcent, iycent;
register int i1, j1, i2, j2;
#ifdef DEBUG
wxString buffer;
#endif

/* Conversion to radians: */
  rot_angle = (Pset1.slangle - 90.) * (PI/180.);
/* ANNA: New vectors in old coordinates:  */
  c2 = cos(rot_angle);  c3 = -sin(rot_angle);
  c5 = sin(rot_angle); c6 = cos(rot_angle);
/* Compute c1 and c4 to maintain center in the middle
* Matrix applied to output image, should keep the middle in the middle
* of the input image: */
  ixcent = nx2/2; iycent = ny2/2;
  c1 = (float)(nx0/2) - (float) ixcent * c2 - (float)iycent * c3;
  c4 = (float)(ny0/2) - (float) ixcent * c5 - (float)iycent * c6;
// Rotation matrix: c1 c2 c3
//                  c4 c5 c6
// x1 = c1 + c2 x2 + c3 y2
// y1 = c4 + c5 x2 + c6 y2

#ifdef DEBUG
  buffer.Printf(wxT("slangle=%f  rot_angle=%f"), Pset1.slangle, rot_angle);
  wxMessageBox(buffer, wxT("rotate_spectro_ima"), wxICON_INFORMATION | wxOK);
#endif

// Find the background value with two squares in the bottom left/right of
// the image:
back1 = 0.;
for(j1 = 2; j1 < 12; j1++)
  for(i1 = 2; i1 < 12; i1++) back1 += in_1[i1+j1*nx0];
back2 = 0.;
for(j1 = 2; j1 < 12; j1++)
  for(i1 = nx0-12; i1 < nx0-2; i1++) back2 += in_1[i1 + j1*nx0];
// Take the minimum value:
if(back2 < back1) back1 = back2;
back1 /= 100.;

// Fill the output image with the background value, since some pixels remain
// without any affectation
  for(i2 = 0; i2 < nx2 * ny2; i2++) out_2[i2] = back1;

// Scan the output image (location i2,j2) and calculate the
// transformed position (i1,j1) in the input image:
for(j2 = 0; j2 < ny2; j2++)
  {
  for(i2 = 0; i2 < nx2; i2++)
    {
// Compute the corresponding position in the input image:
    ix1 = c1 + c2 * (float)i2 + c3 * (float)j2;
    iy1 = c4 + c5 * (float)i2 + c6 * (float)j2;
    i1 = (int) ix1;
    j1 = (int) iy1;
// Linear interpolation with the four adjacent pixels
    if(i1 >= 0 && i1 < nx0-1 && j1 >= 0 && j1 < ny0-1)
       {
       dx = ix1 - i1;
       dy = iy1 - j1;
       sum = 0.;
       wtsum = 0.;
// Add the values of the four corners, weighted by (1 - distance):
// First corner: (0,0)
       wt = (1 - dx) * (1 - dy);
       sum += wt * in_1[i1 + j1 * nx0];
       wtsum += wt;
// Second corner: (1,0)
       wt = dx * (1 - dy);
       sum += wt * in_1[(i1+1) + j1 * nx0];
       wtsum += wt;
// Third corner: (0,1)
       wt = (1 - dx) * dy;
       sum += wt * in_1[i1 + (j1+1) * nx0];
       wtsum += wt;
// Fourth corner: (1,1)
       wt = dx * dy;
       sum += wt * in_1[(i1+1) + (j1+1) * nx0];
       wtsum += wt;
// Final value:
       if(wtsum != 0.) out_2[i2 + j2 * nx2] = sum / wtsum;
       }
    }
  }

return(0);
}
/***************************************************************************
* Compute the cross-spectrum
*
* data_old_Re, data_old_Im: Its size is kk times that of "data"
****************************************************************************/
int JLP_Decode::update_cross_spectrum(FFTW_COMPLEX *data, double *data_old_Re[DATA_KK],
                                  double *data_old_Im[DATA_KK],
                                  double *CrossSpec_Re, double *CrossSpec_Im,
                                  int nx, int ny, int kk, int k_data_old)
{
double cre, cim, cre_old, cim_old;
long int nx_ny;
int k_oldest;
register int i;
nx_ny = nx * ny;

/* Index of FT of oldest images: */
k_oldest = (k_data_old + kk - 1) % kk;

// Computation of the cross-spectrum:
for(i = 0; i < nx_ny; i++)
    {
    cre = c_re(data[i]);
    cim = c_im(data[i]);
    cre_old = data_old_Re[k_oldest][i];
    cim_old = data_old_Im[k_oldest][i];
// Computation of the mean cross-spectrum:
    CrossSpec_Re[i] += (cre * cre_old + cim * cim_old);
    CrossSpec_Im[i] += (cim * cre_old - cre * cim_old);
/* Before 2007:
    c_re(CrossSpec[i]) += c_re(data[i]) * c_re(data_old[i])
                      + c_im(data[i]) * c_im(data_old[i]);
    c_im(CrossSpec[i]) += c_im(data[i]) * c_re(data_old[i])
                      - c_re(data[i]) * c_im(data_old[i]);
*/
    }

return(0);
}
/***************************************************************************
* Compute the cross-spectrum
*
* data_old_Re, data_old_Im: Its size is kk times that of "data"
***************************************************************************/
int JLP_Decode::update_modsq(FFTW_COMPLEX *data, double* modsq, int nx, int ny)
{
register int i;
double cre, cim;

// Computation of the square modulus:
for(i = 0; i < nx*ny; i++) {
    cre = c_re(data[i]);
    cim = c_im(data[i]);

// Computation of the mean square modulus:
    modsq[i] += (cre * cre + cim * cim);
    }

return(0);
}
/***************************************************************************
*
* data_old: Its size is kk times that of "data"
***************************************************************************/
int JLP_Decode::cross_spectrum_fourn(double *data, double* data_old[DATA_KK],
                                 double* CrossSpec_Re, double* CrossSpec_Im,
                                 double* modsq, int nx, int ny, int kk,
                                 int k_data_old)
{
long int nx_ny, i_oldest, k_oldest;
double dre, dim, dre_old, dim_old;
register int i, two_i;
nx_ny = nx * ny;

/* Index of FT of oldest image: */
k_oldest = (k_data_old + kk - 1) % kk;
i_oldest = k_oldest * nx_ny;

// Computation of the mean cross-spectrum:
for(i = 0; i < nx_ny; i++)
    {
    two_i = 2 * i;
    dre = data[two_i + 1];
    dim = data[two_i + 2];
    dre_old = data_old[i_oldest][two_i + 1];
    dim_old = data_old[i_oldest][two_i + 2];
    CrossSpec_Re[i] += dre * dre_old + dim * dim_old;
    CrossSpec_Im[i] += dim * dre_old - dre * dim_old;
/*
    CrossSpec_Re[i] += data[2*i+1] * data_old[2*i+1 + i_data_old]
                      + data[2*i+2] * data_old[2*i+2 + i_data_old];
    CrossSpec_Im[i] += data[2*i+2] * data_old[2*i+1 + i_data_old]
                      - data[2*i+1] * data_old[2*i+2 + i_data_old];
*/
// Computation of the mean square modulus:
    modsq[i] += dre * dre + dim * dim;
/*
    modsq[i] += data[2*i+1] * data[2*i+1] + data[2*i+2] * data[2*i+2];
*/
    }

return(0);
}
/**************************************************************************
* Update i_data_old and fill FT of new image into data_old array
*
* data_old: Its size is kk times that of "data"
*
* INPUT:
* data: FT data set to be inserted in data_old array
* kk: number of FT data sets contained in data_old array
*
* INPUT/OUTPUT:
* data_old: array containing kk FT data sets ("First_in" "First_out" FIFO stack)
* k_data_old: index of Fourier Transform of current image in data_old array
*             (between 0 and kk)
*
****************************************************************************/
int JLP_Decode::rotate_data(FFTW_COMPLEX *data, double *data_old_Re[DATA_KK],
                        double *data_old_Im[DATA_KK],
                        int nx, int ny, int kk, int *k_data_old)
{
long int nx_ny;
int k_new;
register int i;
nx_ny = nx * ny;

/* Update index of FT of new image in data_old array: */
*k_data_old = (*k_data_old + 1) % kk;
k_new = *k_data_old;

// Fill new entry:
   for(i = 0; i < nx_ny; i++) {
       data_old_Re[k_new][i] = c_re(data[i]);
       data_old_Im[k_new][i] = c_im(data[i]);
       }

return(0);
}
/**********************************************************************
*
* data_old: Its size is kk times that of "data"
***********************************************************************/
int JLP_Decode::rotate_data_fourn(double *data, double* data_old[DATA_KK],
                              int nx, int ny, int kk, int *k_data_old)
{
long int data_size;
int k_new;
register int i;
data_size = nx * ny * 2 + 2;

/* Update index of FT of new image in data_old array: */
*k_data_old = (*k_data_old + 1) % kk;
k_new = *k_data_old;

// Fill new entry:
   for(i = 0; i < data_size; i++) data_old[k_new][i] = data[i];

return(0);
}
/*****************************************************************************
* Compute statistics and output result to character strings
*
* OUTPUT:
* Pset1.FwhmSelect.Stats: character string with information about FWHM statistics
* Pset1.MaxValSelect.Stats: character string with information about Mean statistics
*****************************************************************************/
int JLP_Decode::ComputeStatistics()
{
double mean, sigma, rate;
int status;

Pset1.MaxValSelect.Stats[0] = '\0';
Pset1.FwhmSelect.Stats[0] = '\0';
status = -1;

// Statistics:
  if(Pset1.MaxValSelect.Nval > 0) {
    mean = Pset1.MaxValSelect.Sum / Pset1.MaxValSelect.Nval;
    sigma = (Pset1.MaxValSelect.Sumsq / Pset1.MaxValSelect.Nval)
                      - mean * mean;
// Max value selection is applied BEFORE fwhm, so different treatment here:
    if(Pset1.Selection_maxval && (Pset1.MaxValSelect.Nval > 0. || nbad_frames != 0))
      rate = 100. * Pset1.MaxValSelect.Nval / ((float)nbad_frames + Pset1.MaxValSelect.Nval);
    else rate = 100.;

    if(sigma > 0) sigma = sqrt(sigma);
    sprintf(Pset1.MaxValSelect.Stats, "MaxVal=%.2f+/-%.2f Min=%.2f Max=%.2f (n=%d, rate=%d%%)",
            mean, sigma, Pset1.MaxValSelect.MinVal,
            Pset1.MaxValSelect.MaxVal, (int)Pset1.MaxValSelect.Nval, (int)rate);
    status = 0;
    }
  if(Pset1.FwhmSelect.Nval > 0.) {
    mean = Pset1.FwhmSelect.Sum / Pset1.FwhmSelect.Nval;
    sigma = (Pset1.FwhmSelect.Sumsq / Pset1.FwhmSelect.Nval)
                      - mean * mean;
    if(Pset1.FwhmSelect.Nval > 0. || nbad_frames != 0)
      rate = 100. * Pset1.FwhmSelect.Nval / ((float)nbad_frames + Pset1.FwhmSelect.Nval);
    else rate = 100.;

    if(sigma > 0) sigma = sqrt(sigma);
    sprintf(Pset1.FwhmSelect.Stats, "FWHM=%.2f+/-%.2f Min=%.2f Max=%.2f (n=%d, rate=%d%%)",
            mean, sigma, Pset1.FwhmSelect.MinVal,
            Pset1.FwhmSelect.MaxVal, (int)Pset1.FwhmSelect.Nval, (int)rate);
    status = 0;
    } else {
    sprintf(Pset1.FwhmSelect.Stats, "FWHM selection nval = %d",
            (int)Pset1.FwhmSelect.Nval);
    status = -1;
    }

return(status);
}
/*********************************************************************
* Rotation of elementary images
* RotationMode: 0 if no rotation
*               1 if +90 degree rotation
*               2 if +180 degree rotation
*               3 if +180 degree rotation
*********************************************************************/
int JLP_Decode::RotateImage(AWORD *ima, int nx, int ny, int RotationMode)
{
AWORD *tmp;
register int i, j;

 if(RotationMode != 1 && RotationMode != 2 && RotationMode != 3) return(0);

 tmp = new AWORD[nx * ny];
 for (i = 0; i < nx * ny; i++) tmp[i] = ima[i];

// Rotation of +90 degrees:
 if(RotationMode == 1) {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[j + i * nx];
      }
    }
// Rotation of +180 degrees:
 } else if(RotationMode == 2) {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[(nx - 1 - i) + (ny - 1 - j) * nx];
      }
    }
// Rotation of +270 degrees:
 } else {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[(ny - 1 - j) + (nx - 1 - i) * nx];
      }
    }
 }

delete tmp;
return(0);
}
/*********************************************************************
* Mirroring of elementary images
* MirrorMode: 0 if no change
*             1 if vertical mirror
*             2 if horizontal mirror
*             3 if horiz. and vertical mirror
*********************************************************************/
int JLP_Decode::MirrorImage(AWORD *ima, int nx, int ny, int MirrorMode)
{
AWORD *tmp;
register int i, j;

 if(MirrorMode < 1 || MirrorMode > 3) return(0);

 tmp = new AWORD[nx * ny];
 for (i = 0; i < nx * ny; i++) tmp[i] = ima[i];

// Vertical mirror:
 if(MirrorMode == 1) {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[i + (ny - 1 - j) * nx];
      }
    }
// Horizontal mirror:
 } else if (MirrorMode == 2) {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[(nx - 1 - i) + j * nx];
      }
    }
// Horizontal and vertical mirror:
 } else {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[(nx - 1 - i) + (ny - 1 - j) * nx];
      }
    }
 }

delete tmp;

return(0);
}
/*************************************************************************
* Removing threshold and offset frame, correction of flat field
* Called by PreProcessFrame()
*
* Remove background and apply a threshold
***************************************************************************/
int JLP_Decode::OffsetAndFlatFieldCorrection(AWORD *ima, double *dble_ima,
                                             int nx, int ny)
{
double ww;
int isize;
register int i;

 if(OffsetFrame == NULL) Pset1.OffsetFrameCorrection = false;
 if(FlatFieldFrame == NULL) Pset1.FlatFieldCorrection = false;

 isize = nx * ny;

// Threshold correction:
    for (i = 0; i < isize; i++) {
     ww = ima[i] - Pset1.OffsetThreshold;
      if(ww < 0.) dble_ima[i] = 0.;
      else dble_ima[i] = ww;
      }

 if(Pset1.OffsetFrameCorrection) {
    for (i = 0; i < isize; i++) {
     ww = dble_ima[i] - OffsetFrame[i];
      if(ww < 0.) dble_ima[i] = 0.;
      else dble_ima[i] = ww;
     }
   }

// Flat Field correction:
  if(Pset1.FlatFieldCorrection) {
    for (i = 0; i < isize; i++) {
       if(FlatFieldFrame[i] > 0.) dble_ima[i] /= FlatFieldFrame[i];
       }
  }

 return(0);
}
/************************************************************
*
* Threshold operation for photo-counting mode
* and photo-center determination
* To simulate photon detection
*
************************************************************/
int JLP_Decode::seuillage_photon(AWORD* ima, int nx, int ny)
{
/* Good values (checked experimentally, JLP feb98) */
int threshold = 20, iside = 2;

/* iside=2, threshold=20, valeurs qui vont bien */
        int i,j,imax,jmax;
        int ii,jj;
        float pmax;

        for (i = iside-1; i < (ny-iside); i++)
         for (j = iside-1; j < nx-iside; j++)
          if(*(ima+i*nx+j) < threshold) *(ima+i*nx+j)=0;
/* Centrage */
          else
          {
/* Look for the maximum: */
                pmax=-1;
                for (ii=0; ii<=iside; ii++)
                for (jj=0; jj<=iside; jj++)
                if ( pmax < *(ima+(i+ii)*nx+j+jj) )
                {
                        pmax = *(ima+(i+ii)*nx+j+jj);
                        imax=i+ii;
                        jmax=j+jj;
                };

                for (ii=-1; ii<iside; ii++)     /* on fait le vide */
                for (jj=-1; jj<iside; jj++)
                *(ima+(imax+ii)*nx+jmax+jj)=0;

/* We put a photon at this location:
JLP: to allow for correct photon correction, I tried to set it to 1.0
--> worse!
*/
                *(ima+imax*nx+jmax)=pmax;
          }

/* gestion des bords */
    for (i=nx-iside; i<nx; i++) for (j=0; j<nx; j++) *(ima+i*nx+j)=0;
    for (i=0; i<nx; i++) for (j=nx-iside; j<nx; j++) *(ima+i*nx+j)=0;
    for (i=0; i<iside-1; i++) for (j=0; j<nx; j++)  *(ima+i*nx+j)=0;
    for (i=0; i<nx; i++) for (j=0; j<iside-1; j++)  *(ima+i*nx+j)=0;

return(0);
}
/***************************************************************************
* To compute the FWHM from the modsq
*
* f1: array of values f1(xx[i],yy[i])
* xx, yy: arrays of coordinates x, y
* npts: number of points
*
* Parameters:
* sigx, sigy, xc, yc, rho
*
****************************************************************************/
int JLP_Decode::fwhm_by_modsq(FFTW_COMPLEX *data, int nx, int ny, float *fwhm)
{
double *f2, *log_g;
double *modsq1, ww;
int npts, nx0, ny0, istart, jstart;
float aa, bb;
register int i, j, k;

// Works on a subwindow 20x20 pixels centred on the frames
nx0 = 20; ny0 = 20;
istart = nx1/2 - nx0/2;
jstart = ny1/2 - ny0/2;
f2 = new double[nx0 * ny0];
log_g = new double[nx0 * ny0];

modsq1 = new double[nx1 * ny1];
  for(i = 0; i < nx1 * ny1; i++)
     modsq1[i] = c_re(data[i]) * c_re(data[i])
               + c_im(data[i]) * c_im(data[i]);
RECENT_FFT_DOUBLE(modsq1, modsq1, &nx1, &ny1, &nx1);

k = 0;
for(j = 0; j < ny0; j++)
  for(i = 0; i < nx0; i++)
   {
   ww = modsq1[i + istart + (j + jstart) * nx1];
   if(ww > 0)
      {
      f2[k] = (i - nx1/2) * (i - nx1/2)
                   + (j - ny1/2) * (j - ny1/2);
      log_g[k] = log(ww);
      k++;
      }
   }
npts = k;

*fwhm = -1;

if(npts > 0)
  {
  jlp_fit_kolmogorof(f2, log_g, npts, &aa, &bb);
  bb = ABS(bb);
// JLP2016 with Andor: it is necessary to multiply by 0.1 to have good values
  if(bb > 0.) *fwhm = 0.1 *pow((double)(bb/6.88),-0.6)/15.;
  else *fwhm = 10.;
/*
  sprintf(buffer," Full Kolmogorov fit: aa=%f bb=%f fwhm~%f (10 mm)\n",
          aa,bb,*fwhm);
  wxMessageBox(wxString(buffer), wxT("ProcessCubeStatistics/fwhm"),
               wxICON_INFORMATION | wxOK);
*/
  }

delete[] modsq1;
delete[] f2;
delete[] log_g;

return(0);
}
/***************************************************************
* fit_kolmogorof
* To fit a Kolmogorof law to Log_intensity(|x|^2) as a function of
* in x^2 + y^2
*
*   g(f) = rho * exp [ -6.88 * (lambda * f / r_0)^5/3 ]
*
*   g(f) = rho * exp [ - b * f^5/3 ]    with b = 6.88 * (lambda / r_0)^5/3
*
*  Log_Intensity(x,y)
*    G(f) = a - b f^(5/3)    with f^2 = x^2 + y^2
*
*  where:
*  The problem is to find the coefficients (a, b)
*  which minimize the sums:
*   SUM on all the selected disk (x, y) of ( Log_intensity - G(x,y) ) **2
*
* The normal equation can be written as:
*
*    n a - b SUM f^5/3 = SUM Log_g
*    a SUM f^5/3 - b SUM f^10/3 = SUM f^5/3 Log_g
*
* Hence:
*    a = 1/n * (SUM Log_g + b SUM f^5/3)
*    b = (SUM f^5/3 Log_g  - 1/n SUM Log_g SUM f^5/3)
*          / ( 1/n * SUM f^5/3 * SUM f^5/3 - SUM f^10/3)
*
*   where n is the number of points (SUM 1)
*
*
* gg: array of values gg(xx[i],yy[i])
*
* ff2: arrays of x^2 + y^2
* npts: number of points
*
*****************************************************************/
int JLP_Decode::jlp_fit_kolmogorof(double *ff, double *log_g, int npts,
                       float *aa, float *bb)
{
/* log_g log of measured intensities */
double sum_g, sum_f1, sum_f2, sum_gf, w1;
register int k;

/* Compute the sums:  5/3 = 1.667 (but 5/6=0.833 since x^2+y^2)*/
/* Compute the sums:  10/3 = 0.833 (but 5/3=1.667 since x^2+y^2)*/
sum_f1 = 0.; sum_f2 = 0.;
sum_g = 0.; sum_gf = 0.;
for(k = 0; k < npts; k++)
  {
  w1 = pow(ff[k],0.833);
  sum_f1 += w1;
  sum_f2 += pow(ff[k],1.667);
  sum_g  += log_g[k];
  sum_gf += w1 * log_g[k];
  }

/* Result:
*    a = 1/n * (SUM Log_g + b SUM f^5/3)
*    b = (SUM f^5/3 Log_g  - 1/n SUM Log_g SUM f^5/3)
*          / ( 1/n * SUM f^5/3 * SUM f^5/3 - SUM f^10/3)
*/
  *bb = (sum_gf - sum_g * sum_f1 / npts)
           / (sum_f1 * sum_f1 / npts - sum_f2);
  *aa = (sum_g + (*bb) * sum_f1) / npts;

return(0);
}
/**********************************************************
* FWHM computation
* Eric Aristidi's fit (approximation only)
*
**********************************************************/
int JLP_Decode::fwhm_aris(FFTW_COMPLEX *data, int nx, int ny, float *fwhm)
{
int    f12_1, f12_2, dim, ixc, iyc;
float  pix,lambda,x,y1,y2,r01,r02,r0;
double *modsq1;
register int i;

dim = nx;
ixc = nx/2;
iyc = ny/2;

modsq1 = new double[nx * ny];
  for(i = 0; i < nx * ny; i++)
     modsq1[i] = c_re(data[i]) * c_re(data[i])
               + c_im(data[i]) * c_im(data[i]);
RECENT_FFT_DOUBLE(modsq1, modsq1, &nx, &ny, &nx);

 x=modsq1[ixc + iyc*dim];

 f12_1=4;
 y1=modsq1[(ixc+f12_1) + iyc * dim] + modsq1[ixc + (iyc+f12_1) * dim];
 y1/=2.;

 f12_2=3;
 y2=modsq1[(ixc+f12_2) + iyc * dim] + modsq1[ixc + (iyc+f12_2) * dim];
 y2/=2.;

//  Compute r0
//  ---------
 pix=0.0119/206265.;lambda=0.00000065;
 r01=pow(3.44/log(x/y1),0.6)*lambda*(float)f12_1/pix/dim;
 r02=pow(3.44/log(x/y2),0.6)*lambda*(float)f12_2/pix/dim;
     r0=(r02+r01)/2.;
// To make it compatible with Kolmogorof:
     *fwhm = 0.3 / r0;

delete[] modsq1;
return(0);
}
/***************************************************************************
* Check if fwhm selection criterium is satisfied,
* Compute statistics on the FWHM of the frames
*
* Structure used for frame selection (defined in vcr5mdic.h):
  typedef struct {
  float LowCut, HighCut
  float MinVal, MaxVal;
  double Sum, Sumsq;
  float Nval;
  char Stats[80];
  } MY_SELECT;
*
* INPUT:
* FWHM_COMPLEX *data: direct Fourier transform of elementary frame
* nx, ny
*
* OUTPUT:
* good_frame: set to 1 if OK, 0 otherwise
***************************************************************************/
int JLP_Decode::SelectFrame_with_fwhm(FFTW_COMPLEX *data, int nx, int ny,
                                      int *good_frame)
{
float fwhm;

*good_frame = 1;

// Check whether FWHM lies within the boundaries, if needed
// When statistics are required always go through this:
  if(Pset1.Selection_fwhm || Pset1.Statistics_is_wanted) {
    fwhm_by_modsq(data,nx,ny,&fwhm);
// fwhm_aris(data,nx,ny,&fwhm);
// Compute statistics with the good frames only:
    if(fwhm > 0){
      if(Pset1.Selection_fwhm &&
         (fwhm < Pset1.FwhmSelect.LowCut || fwhm > Pset1.FwhmSelect.HighCut)){
       *good_frame = 0;
       }
       else {
       Pset1.FwhmSelect.Nval++;
       Pset1.FwhmSelect.Sum += fwhm;
       Pset1.FwhmSelect.Sumsq += fwhm * fwhm;
       Pset1.FwhmSelect.MinVal = MINI(Pset1.FwhmSelect.MinVal, fwhm);
       Pset1.FwhmSelect.MaxVal = MAXI(Pset1.FwhmSelect.MaxVal, fwhm);
       }
      }
    else {
/* DEBUG :
    sprintf(buffer," Error fitting Kolmogorov law\n");
    wxMessageBox(wxString(buffer),
                 wxT("SelectFrame_with_fwhm"), wxICON_ERROR);
*/
      *good_frame = 1;    // By default, assume image is good...
      }
   }

return(0);
}
/***************************************************************************
* Check if MaxVal selection criterium is satisfied,
* Compute statistics on the maxima of the frames
*
* Structure used for frame selection (defined in vcr5mdic.h):
  typedef struct {
  float LowCut, HighCut
  float MinVal, MaxVal;
  double Sum, Sumsq;
  float Nval;
  char Stats[80];
  } MY_SELECT;
*
* INPUT:
* nx, ny
*
* OUTPUT:
* good_frame: set to 1 if OK, 0 otherwise
***************************************************************************/
int JLP_Decode::SelectFrame_with_MaxVal(double *frame, int nx, int ny,
                                        int *good_frame)
{
double max_value, *work;
int nx_ny, imax;
register int i, k;

*good_frame = 1;
nx_ny = nx * ny;

  if(Pset1.Selection_maxval || Pset1.Statistics_is_wanted) {
   work = new double[nx *ny];
   for(i = 0; i < nx_ny; i++) work[i] = frame[i];

// Work on 3 iterations to
// look for the 3rd maximum (to avoid noisy "hot points");
   for(k = 0; k < 3; k++) {
    max_value = 0.;
    imax = 0;
    for(i = 0; i < nx_ny; i++) {
       if(work[i] > max_value) {
        imax = i;
        max_value = work[i];
       }
     }
// Invalidate the maximum found in this loop:
    work[imax] = 0.;
    }
   delete[] work;

// Check whether maximum value lies between boundaries:
   if(Pset1.Selection_maxval &&
      (max_value < (AWORD)Pset1.MaxValSelect.LowCut
       || max_value > (AWORD)Pset1.MaxValSelect.HighCut)) {
     *good_frame = 0;
     }

// Statistics on the maximum value for the good frames:
   if(*good_frame || Pset1.Statistics_is_wanted) {
     Pset1.MaxValSelect.Nval++;
     Pset1.MaxValSelect.MinVal = MINI(Pset1.MaxValSelect.MinVal, (double)max_value);
     Pset1.MaxValSelect.MaxVal = MAXI(Pset1.MaxValSelect.MaxVal, (double)max_value);
     Pset1.MaxValSelect.Sum += (double)max_value;
     Pset1.MaxValSelect.Sumsq += (double)max_value * (double)max_value;
   }
  }

return(0);
}
/***************************************************************************
* Preprocess elementary frame
* - apply threshold and/or substract offset frame
* - check if selection mean criterium is satisfied
* - if OK, copy frame to data structure
*
* Structure used for frame selection (defined in vcr5mdic.h):
  typedef struct {
  float LowCut, HighCut
  float MinVal, MaxVal;
  double Sum, Sumsq;
  float Nval;
  char Stats[80];
  } MY_SELECT;
*
* INPUT:
* CubeImage
* nx, ny, iz
*
* OUTPUT:
* double *dble_frame  : corrected frame (double precision version)
* FFTW_COMPLEX *cplx_frame : corrected frame (complex version)
* good_frame: set to 1 if OK, 0 otherwise
***************************************************************************/
int JLP_Decode::PreProcessFrame(AWORD* CubeImage, FFTW_COMPLEX *cplx_frame,
                                double *dble_frame, int iz, int *good_frame)
{
long int iz_nx_ny, nx_ny;
register int i;

*good_frame = 1;
iz_nx_ny = iz * nx1 * ny1;
nx_ny = nx1 * ny1;

// Case when ANDOR camera is connected:
if(CamSet1.is_initialized == 1) {
// Rotation:
 RotateImage(&CubeImage[iz_nx_ny],nx1,ny1, CamSet1.RotationMode);
// Mirror:
 MirrorImage(&CubeImage[iz_nx_ny],nx1,ny1, CamSet1.MirrorMode);
 }

// Offset and Flat Field correction of elementary frame number iz:
   OffsetAndFlatFieldCorrection(&CubeImage[iz_nx_ny], dble_frame, nx1, ny1);

// Selection according to the maximum value:
   SelectFrame_with_MaxVal(dble_frame, nx1, ny1, good_frame);

if (*good_frame && Pset1.ProcessingMode != 0) {
// Transfer to data array, using working window:
// and resetting the imaginary part for the FFT:
  for(i = 0; i < nx_ny; i++) {
    c_re(cplx_data[i]) = dble_frame[i];
    c_im(cplx_data[i]) = 0.;
    }
 }

return(0);
}
//***********************************************************************
// Check if presence of horizontal stripes
// Other version that compares the mean of two successive lines (better)
//***********************************************************************
int JLP_Decode::Check_if_frame_is_good(AWORD *im1, int nx0, int ny0)
{
float v, mean, sigma, old_mean, old_sigma;
register int i, j;
int status;

status = 0;

old_mean = 0.; old_sigma = 0.;
for(j = 0; j < ny0; j++) {
 mean = 0.; sigma = 0.;
   for(i = 0; i < nx0; i++) {
     v = im1[i + j * nx0];
     mean += v;
     sigma += v * v;
    }
  mean /= nx0;
  sigma /= nx0;
  sigma = sigma - mean * mean;
  if(sigma > 0.) sigma = sqrt((double)sigma);
  else sigma = 0.;
// Criterium: bright line and really different from previous:
  if(j > 1 && mean > 100) {
   v = mean - old_mean;
   if (v < 0) v *= -1;
   if(v > 2. * (sigma + old_sigma)) {status = 1; break;}
   }
  old_mean = mean;
  old_sigma = sigma;
}

return status;
}
/************************************************************************
* Direct Vector (cf. Bagnuolo)
*************************************************************************/
int JLP_Decode::BuildUpDirectVector(double *inframe, int nx, int ny)
{
int ix0, iy0, ix1, iy1, ix01, iy01, ixc, iyc;
double DVA_threshold, *work;
register int i0, i1;

if((Pset1.DirectVector == false) || (DirectVector == 0)
   || (direct_vector == NULL)) return(-1);

ComputeThresholdForDVA(inframe, nx, ny, DVA_threshold);

work = new double[nx * ny];

for(i0 = 0; i0 < nx * ny; i0++) work[i0] = 0.;

for(i0 = 0; i0 < nx * ny; i0++) {
  if(inframe[i0] > DVA_threshold) work[i0] = inframe[i0];
  }

ixc = nx / 2;
iyc = ny / 2;
for(i0 = 0; i0 < nx * ny; i0++) {
  if(work[i0] > 0.) {
    iy0 = i0 / nx;
    ix0 = i0 - iy0 * nx;
    for(i1 = i0 + 1; i1 < nx * ny; i1++) {
      if(work[i1] > 0.) {
        iy1 = i1 / nx;
        ix1 = i1 - iy1 * nx;
        ix01 = ix1 - ix0;
        iy01 = iy1 - iy0;
        if(ABS(ix01) < ixc && ABS(iy01) < iyc) {
          if(work[i1] > work[i0])
            direct_vector[(ixc + ix01) + (iyc + iy01) * nx] += (work[i1] - work[i0]);
          else
            direct_vector[(ixc - ix01) + (iyc - iy01) * nx] += (work[i0] - work[i1]);
          }
        }
     }
  }
}

delete[] work;
return(0);
}
/************************************************************************
* Lucky Imaging
*
* INPUT:
*  inframe: elementary image
*  nx, ny: size of inframe
*  inFFT: FFT of inframe
*  iframe: index of inframe (1 if first image)
*************************************************************************/
int JLP_Decode::BuildUpLuckyImage(double *inframe, FFTW_COMPLEX *inFFT,
                                 int nx, int ny, const int iframe)
{
double *outframe;
int status;
register int i;
double x0, y0, delta_x, delta_y;

if((Pset1.LuckyImaging == false) || (LuckyImage == 0)
   || (lucky_image == NULL)) return(-1);

status = ComputeCenterOfImage(inframe, nx, ny, x0, y0);
if(status){
  wxMessageBox(wxT("Error computing center of frame"),
               wxT("BuildUpLuckyImage"), wxICON_ERROR);
  x0 = lucky_x0;
  y0 = lucky_y0;
 }

outframe = new double[nx *ny];

// Take the first frame as the reference
//if(iframe == 1) {
//  lucky_x0 = x0;
//  lucky_y0 = y0;
//}
  lucky_x0 = nx / 2.;
  lucky_y0 = ny / 2.;


// Shift all frames so that their centers will be the same as the first frame:
delta_x = lucky_x0 - x0;
delta_y = lucky_y0 - y0;

ShiftImage(inframe, inFFT, outframe, nx, ny, delta_x, delta_y);

for(i = 0; i < nx * ny; i++)
  lucky_image[i] += outframe[i];

delete outframe;

return(0);
}
