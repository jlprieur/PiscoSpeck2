/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* asp2_decod_process2.cpp
* JLP_Decode Class
*
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

#include "jlp_itt1.h"              // Min_Max_Itt (in jlp_splot)
#include "jlp_numeric.h"           // recent_fft
#include "jlp_wx_ipanel_filters.h" // subtract_back_model ...

#include "asp2_defs.h"         // enum ChildIndex
#include "asp2_photon_corr.h"  // definition of clean_deconvolution, ...
#include "asp2_covermas.h"     // bisp2D_to_2D_Image
#include "jlp_decode.h"
#include "jlp_time0.h"         // jlp_italian_date() ...

// #define DEBUG
/*************************************************************************
* Compute autoc from the square modulus
* (not good, since there is a bright background)
*
* Input:
*  modsq
*
* Output:
*  out_autoc: autocorrelation
*************************************************************************/
int JLP_Decode::DC_AutocFromModsq(double *out_autoc, int nx, int ny)
{
register int i;
FFTW_COMPLEX *work;

if(ngood_frames == 0) return(-1);

work = new FFTW_COMPLEX[nx * ny];

// Computing the autocorrelation (FFT-1 from the cross-spectrum):
  for(i = 0; i < nx * ny; i++)
      {
      c_re(work[i]) = modsq[i]/ngood_frames;
      c_im(work[i]) = 0.;
      }

// JLP99: warning: NY,NX!!!!
  fftw_fast(work, ny, nx, -1);

// Prepare for display:
  for(i = 0; i < nx * ny; i++) out_autoc[i] = c_re(work[i]);
  RECENT_FFT_DOUBLE(out_autoc, out_autoc, &nx, &ny, &nx);

delete work;

return(0);
}
/*************************************************************************
* Restore an image from the bispectrum
*
* INPUT:
* Private parameters used here:
* nx, ny: size of images
* UnresolvedModsqFrame : modsq of unresolved star
* Phot_Modsq : modsq of the photon response of the detector
*
* OUTPUT:
* restored_image: restored (deconvolved) image
* xphot: mean nber of photons/elementary frame
*************************************************************************/
int JLP_Decode::DC_ImageFromBispectrum(double *xphot)
{
int status, n_iterations;
char fname0[128], comments0[128], err_messg0[128];
char msg1[120];
double *bispp1, *modsq1, *re1, *im1, *snrm1;
double omega, noise;
FFTW_COMPLEX *work;
register int i;
wxString buffer;

if((ngood_frames == 0) && (input_processed_frames == false)) {
  buffer.Printf(wxT("ngood_frames = %d"), ngood_frames);
  wxMessageBox(buffer, wxT("DC_ImageFromBispectrum"), wxOK | wxICON_ERROR);
  return(-1);
}

if(PhotModsqFrame == NULL)
{
   status = DC_LoadPhotModsqFrame(Pset1.PhotModsqFileName);
   if(status) {
    wxMessageBox(wxT("Error loading PhotModsqFrame"),
                 wxT("DC_ImageFromBispectrum"), wxOK | wxICON_ERROR);
    return(-5);
   }
}

if(RestoredImageFrame != NULL) delete[] RestoredImageFrame;
RestoredImageFrame = new double[nx1 * ny1];

// Copy to temporary array, since the bispectrum and power spectrum
// are both modified in "prepare_output_bisp":
  bispp1 = new double[ngamma * 4];
  for(i = 0; i < ngamma * 4; i++) bispp1[i] = bispp[i];
  modsq1 = new double[nx1 * ny1];

  re1 = new double [nx1 * ny1];
  for(i = 0; i < nx1 * ny1; i++) re1[i] = 0.;
  im1 = new double[nx1 * ny1];
  for(i = 0; i < nx1 * ny1; i++) im1[i] = 0.;
  work = new FFTW_COMPLEX[nx1 * ny1];
  for(i = 0; i < nx1 * ny1; i++) modsq1[i] = modsq[i];

// If bispectrum was not read from file, computes SNR, phase, and so on:
if( (ngood_frames > 1) && (input_processed_frames != true) ) {
  snrm1 = new double [nx1 * ny1];
// "prepare_output_bisp" is contained in "covermas.cpp"
// WARNING: "prepare_output_bisp" modify modsq, bispp, snrm, etc,
// when computing their average values
  prepare_output_bisp(bispp1, modsq1, snrm1, nx1, ny1, ngood_frames, nbeta,
                      ngamma);

// Recenter modsq1 !
  RECENT_FFT_DOUBLE(modsq1, modsq1, &nx1, &ny1, &nx1);
  delete[] snrm1;
  }

/* Photon noise bias correction:
int photon_corr_auto1(double *bispp, double *modsq, double *phot_modsq,
                          int nx, int ny, int bisp_dim, double *xphot,
                          int nbeta, int ngamma)
*/
   status = photon_corr_auto1(bispp1, modsq1, PhotModsqFrame, nx1, ny1, ngamma,
                              xphot, nbeta, ngamma, msg1);
  if(status) {
    wxMessageBox(wxString(msg1), wxT("Error in photon_corr_auto1"),
                 wxICON_ERROR);
/* DEBUG:
   sprintf(msg1," ir=%d nbeta=%d ngamma=%d nmax=%d status=%d nphot/frame = %.1f",
            ir, nbeta, ngamma, max_nclosure, status, *xphot);
    wxMessageBox(wxString(msg1), wxT("End of photon_corr_auto1"),
                 wxICON_INFORMATION | wxOK);
*/
    }

/* DEBUGGGG
for(i = 0; i < nx1 * ny1; i++) output[i] = modsq1[i];
*/

 if(!status){
// Bispectrum inversion and image restoration:
/*
 int inv_bispec2(double *re_out, double *im_out, double *bisp1, double *modsq1,
                int ir, int ngamma, int nbeta, int nclos_max, int nx1, int ny1);
*/
    status = inv_bispec2(re1, im1, bispp1, modsq1, Pset1.ir_bisp, ngamma, nbeta,
                         Pset1.nmax_bisp, nx1, ny1);
/* DEBUG:
    if(status) {
      sprintf(msg1," Status = %d", status);
      wxMessageBox(wxString(msg1), wxT("Error in inv_bispec2"),
                   wxICON_ERROR);
      }
*/
  }


/* DEBUGGGG
for(i = 0; i < nx1 * ny1; i++) output[i] = re1[i];
if(i > 2) return(0);
*/

if(!status) {

// To avoid spurious fringes in the final image, recenter re1 and im1:
  RECENT_FFT_DOUBLE(re1, re1, &nx1, &ny1, &nx1);
  RECENT_FFT_DOUBLE(im1, im1, &nx1, &ny1, &nx1);

  for(i = 0; i < nx1 * ny1; i++) {
    c_re(work[i]) = re1[i];
    c_im(work[i]) = im1[i];
    }

// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
  fftw_fast(work, ny1, nx1, -1);

  for(i = 0; i < nx1 * ny1; i++) RestoredImageFrame[i] = c_re(work[i]);
  RECENT_FFT_DOUBLE(RestoredImageFrame, RestoredImageFrame, &nx1, &ny1, &nx1);

// If unresolved modsq is available performs deconvolution:
   if(UnresolvedModsqFrame != NULL) {
/* in "clean_deconv.cpp:"
int clean_deconvolution(double *in_map, double *cleaned_map,
                        double *modsq_of_unresolved, int nx, int ny,
                        double omega, double noise, int ir,
                        int *n_iterations);
*/
       omega = 0.3;
       noise = 0.01;
       clean_deconvolution(RestoredImageFrame, RestoredImageFrame,
                           UnresolvedModsqFrame, nx1, ny1,
                           omega, noise, Pset1.ir_bisp, &n_iterations);
       }

// In case of failure, fills output array with zeroes:
} else {
 for(i = 0; i < nx1 * ny1; i++) RestoredImageFrame[i] = 0.;
}

delete[] bispp1;
delete[] modsq1;
delete[] re1;
delete[] im1;
delete work;

return status;
}
/*************************************************************************
* DC_BispectrumSlice
* Compute a slice of the real part of the bispectrum
*
*************************************************************************/
int JLP_Decode::DC_BispectrumSlice(double *output, int nx, int ny)
{
int status;
double *modsq1, *bispp1, *snrm1;
register int i;
char error_messg[128];

// Copy to temporary arrays, since "prepare_output"
// will modify those arays (when computing the mean of modsq1, bispp1, ...)
  bispp1 = new double[ngamma * 4];
  for(i = 0; i < ngamma * 4; i++) bispp1[i] = bispp[i];
  modsq1 = new double[nx * ny];
  for(i = 0; i < nx1 * ny1; i++) modsq1[i] = modsq[i];

// If bispectrum was not read from file, computes SNR, phase, and so on:
if(ngood_frames > 1) {
  snrm1 = new double [nx1 * ny1];

// WARNING: "prepare_output" modify modsq1, bispp1, snrm1
// when computing their average values
  prepare_output_bisp(bispp1, modsq1, snrm1, nx, ny, ngood_frames,
                       nbeta, ngamma);

// Recenter modsq1 !
  RECENT_FFT_DOUBLE(modsq1, modsq1, &nx1, &ny1, &nx1);
  delete[] snrm1;
  } else {
  for(i = 0; i < nx1 * ny1; i++) modsq1[i] = modsq_centered_v[i];
  }

/*
* ShouldNormalize: flag set to 1 if modsq and bisp have to be normalized
* iopt: option set to 0 if real part of bispectrum has to be output,
*                     1 if imaginary part ...
*                     2 if SNR ...
*                     3 if phase
*                     4 if modulus i.e., sqrt(real^2 + imag^2)
int bisp2D_to_2D_image(double* out_image, double* modsq,
                       int nx, int ny, double* bisp, int nx_bisp,
                       int nbeta, int ngamma, int ShouldNormalize, int iopt,
                       char *error_messg);
*/
status = bisp2D_to_2D_image(output, modsq1, nx1, ny1, bispp1, ngamma,
                            nbeta, ngamma, 0, 0, error_messg);

delete[] modsq1;
delete[] bispp1;

return status;
}
/*************************************************************
* Clipping of Flat field frame at 4 sigma
* to avoid introducing too much noise
**************************************************************/
int JLP_Decode::ClipFlatField()
{
double sum, sumsq, mean0, sigma0, mean, sigma;
double ww, nvalues, mini, maxi;
register int i, j;
char ss[128];

mini = -1.e+12;
maxi = +1.e+12;

// Performs 3 iterations to estimate the smallest standard deviation
// as possible:
for(j = 0; j < 3; j++) {
 sum = 0.; sumsq = 0.; nvalues = 0.;
  for(i = 0; i < nx1 * ny1; i++) {
   ww = FlatFieldFrame[i];
   if(ww < mini) {
   FlatFieldFrame[i] = mini;
   } else if (ww > maxi) {
   FlatFieldFrame[i] = maxi;
   } else {
   sum += ww;
   sumsq += ww * ww;
   nvalues++;
   }
  }

// Compute the mean and sigma:
  if(nvalues > 0) {
   mean = sum / nvalues;
   sigma = MAXI(1.e-09, (sumsq / nvalues) - (mean * mean));
   sigma = sqrt(sigma);
   }
  if(j == 0) {mean0 = mean; sigma0 = sigma;}
  mini = mean - 4. * sigma;
  maxi = mean + 4. * sigma;
}

  sprintf(ss, "Mean0=%.3f sigma0=%.3f; mean=%.3f sigma=%.3f  (%d bad values > 4 sigma)\n",
          mean0, sigma0, mean, sigma, (nx1 * ny1) - (int)nvalues);
  wxMessageBox(wxString(ss), wxT("Flat Field: statistics"),
               wxICON_INFORMATION | wxOK);

// Normalization by the mean:
  for(i = 0; i < nx1 * ny1; i++) FlatFieldFrame[i] /= mean;

 return(0);
}
/***************************************************************************
* Subtract a model built with the profile (bins of one pixel width)
* to the input image
*
* INPUT:
* ima0: pointer corresponding to  the input/output image
* pro0: input profile
* np0: number of points of the input profile
*
* NOW DEFINED IN jlp_wxplot !!!
***************************************************************************/
int OLD_SubtractProfile(double *ima0, const int nx0, const int ny0,
                    double *pro0, const int np0)
{
double *back_model;
// rad2 is forbidden with Windows !
double xc, yc, rad_sq;
int ipro;
register int i, j;

back_model = new double[nx0 * ny0];
for(i = 0; i < nx0 * ny0; i++) back_model[i] = 0.;

xc = 0.5 + (double)nx0 / 2.;
yc = 0.5 + (double)ny0 / 2.;
for(j = 0; j < ny0; j++) {
  for(i = 0; i < nx0; i++) {
   rad_sq = MAXI(1.E-9, SQUARE(i - xc) + SQUARE(j - yc));
   ipro = (int)sqrt(rad_sq);
   if(ipro < np0) back_model[i + j * nx0] = pro0[ipro];
  }
}

// int subtract_back_model(double *autoc_data, double *back_model,
//                         double *autoc_flattened, int nx, int ny, int idim)
subtract_back_model(ima0, back_model, ima0, nx0, ny0, nx0);

delete back_model;
return(0);
}
/***************************************************************************
* ComputeThresholdForDVA(used for Direct Vector Autocorrelation)
*
* INPUT:
* inframe: input image
* nx, ny: size of input image
*
* OUTPUT:
* DVA_threshold: threshold to be used for DVA (in order to keep
*                only a few points)
***************************************************************************/
int ComputeThresholdForDVA(double *inframe, const int nx, const int ny,
                           double &DVA_threshold)
{
int nbins;
double *histo_nb, *histo_val;
double npoints_mini, sum;
register int k;

// Compute the histogram:
nbins = 1000;
histo_nb = new double[nbins];
histo_val = new double[nbins];

ComputeHistogram(inframe, nx, ny, histo_nb, histo_val, nbins);

// Determine the threshold in order to keep 100 points or 5% of the points:
npoints_mini = MINI(100., 0.05 * nx * ny);
sum = 0.;
for(k = nbins - 1; k > 0; k--) {
  sum += histo_nb[k];
  if(sum >= npoints_mini) break;
  }

DVA_threshold = histo_val[k];

delete[] histo_nb;
delete[] histo_val;

return(0);
}
/***************************************************************************
* ComputeHistogram
* Called by ComputeThresholdForDVA(used for Direct Vector Autocorrelation)
*
* INPUT:
* inframe: input image
* nx, ny: size of input image
* nbins: number of bins
*
* OUTPUT:
* histo_nb: number of points in the bin
* histo_val: lowest value of the bin
***************************************************************************/
int ComputeHistogram(double *inframe, const int nx, const int ny,
                     double *histo_nb, double *histo_val, const int nbins)
{
double min_val, max_val, ww;
int bad_value = 12345;
register int i, k;

// Determine the minimum and maximum of an image that will be used for display
// (not taking into account the three pixels corresponding to the largest
// and smallest intensities)
Min_Max_itt(inframe, nx, ny, 0, 0, nx, ny, &min_val, &max_val, bad_value);

if(min_val == max_val) max_val += 1.;
ww = nbins  / (max_val - min_val);

// Initialize the histogram:
for(k = 0; k < nbins; k++) histo_nb[k] = 0.;
for(k = 0; k < nbins; k++) histo_val[k] = min_val + (double)k / ww;

for(i = 0; i < nx * ny; i++) {
 k = (int)((inframe[i] - min_val) * ww);
 k = MAXI(k, 0);
 k = MINI(k, nbins - 1);
 histo_nb[k] += 1.;
 }

return(0);
}
/***************************************************************************
* ShiftImage (used for Lucky Imaging)
*
* INPUT:
* inframe: input image
* inFFT: FFT of input frame
* nx, ny: size of input image
* x0, y0: shift vector to be applied to the image
*
* OUTPUT:
* outframe: output recentred image
***************************************************************************/
int ShiftImage(double *inframe, FFTW_COMPLEX *inFFT, double *outframe,
               const int nx, const int ny, const double x0, const double y0)
{
FFTW_COMPLEX *work;
double a, b, w, sin_w, cos_w;
int ii;
register int i, j;

work = new FFTW_COMPLEX[nx * ny];

// Multiplication with exp - 2 i PI (u x + v y)
// (a + i b)(cos w + i sin w) = (a cos w - b sin w) + i (a sin w + b cos w)
// To avoid artefacts, the slope 2 PI (u x + v y)
// should be continuous modulo 2 PI at the edges
// i.e.  2 * PI * x0 * nx / nx = 0 (modulo 2PI)
//  and  2 * PI * y0 * ny / ny = 0 (modulo 2PI)
// i.e. x0 and y0 should be integers
for(j = 0; j < ny; j++) {
  for(i = 0; i < nx; i++) {
    w = -2. * (double)PI * ( ((double)i * (int)(x0 + 0.5)) / (double)(nx)
                           + ((double)j * (int)(y0 + 0.5)) / (double)(ny));
    sin_w = sin(w);
    cos_w = cos(w);
    ii = i + j * nx;
    a = c_re(inFFT[ii]);
    b = c_im(inFFT[ii]);
    c_re(work[ii]) = a * cos_w - b * sin_w;
    c_im(work[ii]) = a * sin_w + b * cos_w;
  }
}

// JLP99: warning: NY,NX!!!!
fftw_fast(work, ny, nx, -1);

for(i = 0; i < nx * ny; i++) outframe[i] = c_re(work[i]);

delete work;

return(0);
}
/***************************************************************************
* ComputeCenterOfImage
* by computing the center of gravity of the pixels above a given threshold
*
* INPUT:
* frame: input image
* nx, ny: size of input image
*
* OUTPUT:
* x0, y0: center of image
***************************************************************************/
int ComputeCenterOfImage(double *frame, int nx, int ny, double &x0, double &y0)
{
int bad_value = 12345;
double min_val, max_val, thresh0, w0, ww;
register int i, j;

// Determine the minimum and maximum of an image that will be used for display
// (not taking into account the three pixels corresponding to the largest
// and smallest intensities)
Min_Max_itt(frame, nx, ny, 0, 0, nx, ny, &min_val, &max_val, bad_value);

// Threshold used to compute the center of image:
thresh0 = min_val + (max_val - min_val) * 0.90;

// Compute the center of gravity of the pixels above this threshold:
w0 = 0.;
x0 = 0.;
y0 = 0.;
for(j = 0; j < ny; j++) {
 for(i = 0; i < nx; i++) {
    ww = frame[i + j * nx];
    if(ww > thresh0) {
       w0 += ww;
       x0 += ww * float(i);
       y0 += ww * float(j);
      }
   }
 }
if(w0 == 0.) return(-1);


x0 /= w0;
y0 /= w0;

return(0);
}
