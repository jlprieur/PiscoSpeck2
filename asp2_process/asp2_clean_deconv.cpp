/*********************************************************************
* clean_deconv.cpp
* set of routines used for implementing the CLEAN deconvolution
* or independant program used to remove artefacts from bispectrum restored
* images...
*
* October 2008: I multiply both the spectrum of the input image
*               and that of the PSF by the uv_mask
*               in order to make them compatible (since
*               the PSF entered by the user is not the good one...)
*
* JLP
* Previous version: 16/10/2008
* Version 10/08/2015
*********************************************************************/
#include <stdio.h>
#include <math.h>
#include "jlp_rw_fits0.h"          // writeimag_double_fits...
#include "asp2_rw_config_files.h"  // Init_PSET()
#include "jlp_numeric.h"           // For SQUARE, FFTW, RECENT_FFT ...
#include "asp2_photon_corr.h"  /* prototype definition of "clean_deconvolution"
                                (to check compatibility) */

/* Prototype defined in asp2_photon_corr.h
int clean_deconvolution(double *in_map, double *cleaned_map,
                        double *modsq_of_unresolved, int nx, int ny,
                        double omega, double noise, int ir,
                        int *n_iterations);
*/
// Prototypes of functions defined and used here:
int clean_setup(double *dirty_map, double *dirty_beam, double *clean_map,
                double *clean_beam, double *in_map, double *modsq_of_unresolved,
                int nx, int ny, int ir, double clean_beam_fwhm);
int clean_save_to_file(double *dirty_map, double *dirty_beam,
                       double *clean_map, double *clean_beam,
                       double omega, double noise, int n_iterations,
                       double clean_beam_fwhm, char *in_name,
                       char *out_prefix, int nx, int ny);
int clean_deconv(double *cleaned_map, double *dirty_map, double *dirty_beam,
                 double *clean_map, double *clean_beam, int nx, int ny, double omega,
                 double noise, double *max_dirty_map, int *n_iterations);
int compute_uv_mask(double *uv_mask, int ir, int nx, int ny);
int dirty_beam_from_unres_modsq(double *dirty_beam, double *modsq_of_unresolved,
                                double *uv_mask, int nx, int ny);
int gaussian_clean_beam(double *clean_beam, int nx, int ny, double fwhm);
static int max_residual(double *dirty_map, int nx, int ny, double *max_val,
                         int *i_max, int *j_max);
static int filter_with_uv_mask(double *in_array, double *out_array,
                               double *uv_mask, int nx, int ny);

/******************************************************************************
* clean_deconvolution
*
* omega = gain of the clean loops\n");
* noise = final level of the residuals as a fraction of the maximum of the input image\n");
* clean_beam_fwhm = fwhm of clean beam (in pixels)\n");
* uv_mask: mask to be applied to Fourier domain
******************************************************************************/
int clean_deconvolution(double *in_map, double *cleaned_map,
                        double *modsq_of_unresolved, int nx, int ny,
                        double omega, double noise, int ir,
                        int *n_iterations)
{
double *dirty_map, *dirty_beam, *clean_map, *clean_beam;
double max_dirty_map, clean_beam_fwhm;

dirty_map = new double[nx * ny];
clean_map = new double[nx * ny];
dirty_beam = new double[nx * ny];
clean_beam = new double[nx * ny];

// FWHM of clean beam is set to 3 pixels when ir =30
// (i.e. width of the central peak of the dirty beam):
clean_beam_fwhm = 3.0 * ((double)ir/30.);
clean_setup(dirty_map, dirty_beam, clean_map, clean_beam, in_map,
             modsq_of_unresolved, nx, ny, ir, clean_beam_fwhm);

clean_deconv(cleaned_map, dirty_map, dirty_beam, clean_map, clean_beam, nx, ny,
              omega, noise, &max_dirty_map, n_iterations);

/*
  if(!status) clean_save_to_file(dirty_map, dirty_beam, clean_map, clean_beam,
                                 omega, noise, n_iterations, clean_beam_fwhm,
                                 in_name, out_prefix, nx, ny);
*/

delete clean_beam;
delete dirty_beam;
delete clean_map;
delete dirty_map;
return(0);
}
/*************************************************************************
* Setup the arrays necessary for CLEAN deconvolution
*
* INPUT:
*  in_map: map to be deconvolved
*  modsq_of_unresolved: power spectrum of unresolved star  (centered on the frame)
*  clean_beam_fwhm: Full Width at Half Maximum of the Gaussian to be used
*                   for the clean beam
*  nx = number of lines
*  ny = number of pixels per line
*  ir = radius of uv coverage
*
* OUTPUT:
*  dirty_map: dirty map (copy of input image)
*  clean_map: clean map (filled with zeroes)
*  dirty_beam: dirty beam (centered on 0,0)
*  clean_beam: clean beam (centered on 0,0)
*
*************************************************************************/
int clean_setup(double *dirty_map, double *dirty_beam, double *clean_map,
                double *clean_beam, double *in_map, double *modsq_of_unresolved,
                int nx, int ny, int ir, double clean_beam_fwhm)
{
double *uv_mask;
register int i;

uv_mask = new double[nx * ny];

// Create uv mask (from file or set it to unity):
compute_uv_mask(uv_mask, ir, nx, ny);

/* Transfer input image to residuals */
/* I also multiply both the spectrum of the input image
*             and that of the PSF by the uv_mask
*             in order to make them compatible (since
*             the PSF entered by the user is not the good one...)
*/
filter_with_uv_mask(in_map, dirty_map, uv_mask, nx, ny);

/* Square modulus of the transfer function (centered in the frame)
* Compute dirty beam from PSF:
* Compute dirty beam from modsq of transfer function: */
dirty_beam_from_unres_modsq(dirty_beam, modsq_of_unresolved, uv_mask, nx, ny);

/* Compute clean beam: gaussian of FWHM equal to clean_beam_fwhm: */
gaussian_clean_beam(clean_beam, nx, ny, clean_beam_fwhm);

/* Clean map initialized to zero: */
for(i = 0; i < nx * ny; i++) clean_map[i] = 0.;

delete uv_mask;
return(0);
}
/********************************************************************
* Save results to FITS files
*
********************************************************************/
int clean_save_to_file(double *dirty_map, double *dirty_beam,
                       double *clean_map, double *clean_beam,
                       double omega, double noise, int n_iterations,
                       double clean_beam_fwhm, char *in_name,
                       char *out_prefix, int nx, int ny)
{
char out_name[80], comments[120], err_message[60];
DESCRIPTORS descrip;
PROCESS_SETTINGS Pset;
register int i;

// Set descriptors to zero:
descrip.ndescr = 0;

// Initialize Pset to defaults values:
Init_PSET(Pset);

/* Clean map is saved to file: */
sprintf(out_name,"%s_cm",out_prefix);
sprintf(comments,"Clean map of %.20s (omega=%5.3f noise=%g it=%d)", in_name,
        omega, noise, n_iterations);
/*
int writeimag_double_fits(char *filename, double *image, int nx, int ny,
                        char *comments, DESCRIPTORS descrip, 
                        PROCESS_SETTINGS Pset, char *errmess);
*/
writeimag_double_fits(out_name, clean_map, nx, ny, comments, descrip, 
                      Pset, err_message);

/* Clean map + dirty map (final image) is also saved to file: */
for(i = 0; i < nx * ny; i++) clean_map[i] += dirty_map[i];
sprintf(out_name,"%s_cleaned",out_prefix);
sprintf(comments,"Clean + dirty map of %.20s (omega=%5.3f noise=%g it=%d)", in_name,
        omega, noise, n_iterations);
writeimag_double_fits(out_name, clean_map, nx, ny, comments, descrip,
                      Pset, err_message);
/* Dirty map is saved to file: */
sprintf(out_name,"%s_dm",out_prefix);
sprintf(comments,"Dirty map of %.20s (omega=%5.3f noise=%g it=%d)",
        in_name, omega, noise, n_iterations);
writeimag_double_fits(out_name, dirty_map, nx, ny, comments, descrip, 
                      Pset, err_message);

/* Dirty beam is saved to file: */
sprintf(out_name,"%s_db",out_prefix);
sprintf(comments,"Dirty beam of %.30s",in_name);
writeimag_double_fits(out_name, dirty_beam, nx, ny, comments, descrip,
                      Pset, err_message);

/* Clean beam is saved to file: */
sprintf(out_name,"%s_cb",out_prefix);
sprintf(comments,"Gaussian Clean beam with fwhm=%.3f pixels", clean_beam_fwhm);
writeimag_double_fits(out_name, clean_beam, nx, ny, comments, descrip,
                      Pset, err_message);

return(0);
}
/***********************************************************************
* Compute clean beam: gaussian of FWHM equal to clean_beam_fwhm:
*
* INPUT:
* nx, ny: size of input/output arrays
* fwhm: Full Width at Half Maximum of the Gaussian
*
* OUTPUT:
* clean_beam: dirty beam (centered on 0,0, with a dynamical range of 1/EPSILON)
***********************************************************************/
int gaussian_clean_beam(double *clean_beam, int nx, int ny, double fwhm)
{
int nx2, ny2;
double EPSILON = 1.e-3, ww, sigma2;
register int i, j;

/* FWHM of a Gaussian of sigma=1 is 2*x with:
* exp(- x^2 / sigma^2) = 0.5:
* Hence x^2 = 0.693 and 2*x = 1.665
*/
sigma2 = SQUARE(fwhm / 1.665);
nx2 = nx/2;
ny2 = ny/2;
for(j = 0; j < ny; j++) {
  for(i = 0; i < nx; i++) {
   ww = SQUARE(i - nx2) + SQUARE(j - ny2);
   ww = exp(- ww / sigma2);
/* Limit the dynamical range to 1/EPSILON */
   if(ww > EPSILON) clean_beam[i + j * nx] = ww;
   else clean_beam[i + j * nx] = 0.;
  }
}
return(0);
}
/***********************************************************************
* Compute dirty beam from modsq of transfer function
*
* I multiply both the spectrum of the input image
* and the transfer function by the uv_mask
* in order to make them compatible (since
* the transfer function entered by the user is not the good one...)
*
* INPUT:
* transf_fct: transfer function (centered on 0,0)
* uv_mask: mask to be applied on the Fourier domain
* nx, ny: size of input/output arrays
*
* OUTPUT:
* dirty_beam: dirty beam (centered on 0,0)
***********************************************************************/
int dirty_beam_from_unres_modsq(double *dirty_beam, double *modsq_of_unresolved,
                                double *uv_mask, int nx, int ny)
{
FFTW_COMPLEX *work;
double *re, ww;
register int i;

for(i = 0; i < nx * ny; i++) dirty_beam[i] = 0.;

re = new double[nx * ny];
work = new FFTW_COMPLEX[nx * ny];

/* Multiplication of the transfer function with the uv_mask: */
for(i = 0; i < nx * ny; i++) {
   ww = modsq_of_unresolved[i];
   if(ww > 0.) {
     re[i] = sqrt(ww) * uv_mask[i];
     } else {
     re[i] = 0.;
     }
   }
RECENT_FFT_DOUBLE(re, re, &nx, &ny, &nx);

  for(i = 0; i < nx * ny; i++) {
    c_re(work[i]) = re[i];
    c_im(work[i]) = 0.;
    }

// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
  fftw_fast(work, ny, nx, 1);

  for(i = 0; i < nx * ny; i++) re[i] = c_re(work[i]);

/* Normalisation to one: */
ww = re[0];
if(ww == 0.) {
   fprintf(stderr, "dirty_beam_from_transfer/Error re[0) is null \n");
   return(-1);
   }
for(i = 0; i < nx * ny; i++) re[i] /= ww;

RECENT_FFT_DOUBLE(re, re, &nx, &ny, &nx);

/* Assuming that transfer function is real and symmetric
* its Fourier transform is real */
for(i = 0; i < nx * ny; i++) dirty_beam[i] = re[i];

delete re;
delete work;
return(0);
}
/*********************************************************************
* clean_deconv
* Clean algorithm used for deconvolution.
* Derived from a routine of Eric ANTERRIEU, Lib_an11.c Version July 1990
*
* INPUT:  
*  dirty_map[nx*ny] = dirty map 
*  dirty_beam[nx*ny] = dirty beam
*  clean_beam[nx*ny] = clean beam
*  nx = number of lines
*  ny = number of pixels per line
*  omega = loop gain (percentage of the peak to be removed at each iteration)
*  noise = noise (percentage of the maximum value of the dirty map)
*
* OUTPUT: 
*  cleaned_map[nx*ny] = clean map + dirty_map (final image)
*  clean_map[nx*ny] = clean map
*  dirty_map[nx*ny] = residual map
*  max_dirty_map : maximum of the dirty map (residuals)
*  n_iterations = number of iterations
*
***************************************************************/
int clean_deconv(double *cleaned_map, double *dirty_map, double *dirty_beam,
                 double *clean_map, double *clean_beam, int nx, int ny, double omega,
                 double noise, double *max_dirty_map, int *n_iterations)
{
int    nx2, ny2, i_max, j_max;
double  max_val, threshold;
register  int  i, j, ii, jj;

nx2 = nx/2;  ny2 = ny/2;

for(i = 0; i < nx * ny; i++) clean_map [i] = 0.0;

/* Look for the maximum of the map: */
max_residual(dirty_map, nx, ny, &max_val, &i_max, &j_max);

if(max_val <=0) {
  fprintf(stderr,
          "clean_deconv/error: maximum of residuals is negative: max=%f\n",
          max_val);
  *max_dirty_map = max_val;
  return(-1);
  }

*n_iterations = 0;
threshold = noise * max_val;
printf("clean_deconv: omega=%f threshold=%f\n", omega, threshold);
while (max_val >= threshold) {
  (*n_iterations)++;
  ii = i_max-nx2;
  jj = j_max-ny2;
/* DEBUG:
printf("clean_deconv: max_val=%f omega=%f threshold=%f\n", max_val, omega, threshold);
*/
/* Removes max_val * omega * clean_beam  centered at (i_max, j_max)
* from the dirty map (residuals) and update the clean map: */
  for(i = 0; i < nx; i++) {
    for(j = 0; j < ny; j++) {
      if ( ((i-ii) >= 0) && ((j-jj) >= 0)
        && ((i-ii) < nx) && ((j-jj) < ny) ) {
        clean_map[i + j * nx] += (max_val * omega
                                  * clean_beam[(i-ii) + (j - jj) * nx]);
        dirty_map[i + j * nx] -= (max_val * omega
                                  * dirty_beam[(i-ii) + (j - jj) * nx]);
        }
      }
    }
  max_residual(dirty_map, nx, ny, &max_val, &i_max, &j_max);
} /* End of while loop */

*max_dirty_map = max_val;

// Final image (cleaned_map) is the sum of the clean map plus the residuals:
for(i = 0; i < nx * ny; i++) cleaned_map [i] = clean_map[i] + dirty_map[i];

return(0);
}
/**********************************************************************
* Look for the maximum value in the dirty map array
*
* INPUT:
*  dirty_map: input array
*  nx, ny: size of input array
*
* OUTPUT:
*  max_val: maximum value found in the input array
*  i_max, j_max: x, y location of the maximum 
**********************************************************************/
static int max_residual(double *dirty_map, int nx, int ny, double *max_val,
                         int *i_max, int *j_max)
{
register int i, j;

*max_val = dirty_map[0];
*i_max = 0;
*j_max = 0;
  for(i = 0; i < nx; i++) {
    for(j = 0; j < ny; j++) {
      if (dirty_map[i + j * nx] > (*max_val)) {
        *max_val = dirty_map[i + j * nx];
        *i_max = i;
        *j_max = j;
        }
      }
    }
return(0);
}
/***********************************************************************
* Create a uv mask: disk of radius ir, centered on (nx/2, ny/2)
*
***********************************************************************/
int compute_uv_mask(double *uv_mask, int ir, int nx, int ny)
{
int nx2, ny2, irad2, ir2;
register int i, j;

nx2 = nx / 2;
ny2 = ny / 2;
ir2 = SQUARE(ir);
for(j = 0; j < ny; j++) {
  for(i = 0; i < nx; i++) {
    irad2 = SQUARE(i - nx2) + SQUARE(j - ny2);
    if(irad2 < ir2)
      uv_mask[i + j * nx] = 1.;
    else
      uv_mask[i + j * nx] = 0.;
  }
}

return(0);
}
/****************************************************************************
*
* INPUT:
* in_array: input array to be filtered
* uv_mask: mask in Fourier domain
*
* OUTPUT:
* out_array: array filtered in Fourier domain with uv_mask
****************************************************************************/
static int filter_with_uv_mask(double *in_array, double *out_array,
                               double *uv_mask, int nx, int ny)
{
FFTW_COMPLEX *work;
double *re, *im;
register int i;

re = new double[nx * ny];
im = new double[nx * ny];
work = new FFTW_COMPLEX[nx * ny];

/* Copy input image to real part: */
  for(i = 0; i < nx * ny; i++) re[i] = in_array[i];

/* Shift image to avoid artefacts due to non-continuities at the edges */
  RECENT_FFT_DOUBLE(re, re, &nx, &ny, &nx);

  for(i = 0; i < nx * ny; i++) {
    c_re(work[i]) = re[i];
    c_im(work[i]) = 0.;
    }

// Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
  fftw_fast(work, ny, nx, 1);

  for(i = 0; i < nx * ny; i++) {
    re[i] = c_re(work[i]);
    im[i] = c_im(work[i]);
    }

  RECENT_FFT_DOUBLE(re, re, &nx, &ny, &nx);
  RECENT_FFT_DOUBLE(im, im, &nx, &ny, &nx);

/* Multiplication of the transfer function with the uv_mask: */
  for(i = 0; i < nx * ny; i++) re[i] *= uv_mask[i];
  for(i = 0; i < nx * ny; i++) im[i] *= uv_mask[i];

  RECENT_FFT_DOUBLE(re, re, &nx, &ny, &nx);
  RECENT_FFT_DOUBLE(im, im, &nx, &ny, &nx);

  for(i = 0; i < nx * ny; i++) {
    c_re(work[i]) = re[i];
    c_im(work[i]) = im[i];
    }

// Inverse Fourier Transform: (size=2-D, 1=direct FFT)
// JLP99: warning: NY,NX!!!!
  fftw_fast(work, ny, nx, -1);

  for(i = 0; i < nx * ny; i++) {
    re[i] = c_re(work[i]);
    }
  RECENT_FFT_DOUBLE(re, re, &nx, &ny, &nx);

/* Copy real part to output image: */
  for(i = 0; i < nx * ny; i++) out_array[i] = re[i];

delete re;
delete im;
delete work;

return(0);
}
