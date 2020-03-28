/****************************************************************
*  correct_bisp_auto1
*  (derived from hege's method of fitting a model to the photon response...)
*
*  Contains: photon_corr_auto1()
*  called by correct_bisp.c
*
*  Fits the photon response to the power spectrum
*
* JLP
* Version 10-09-2008
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include "jlp_numeric.h"  // JLP_MEDIAN, SQUARE, etc
#include "asp2_covermas.h"
#include "asp2_photon_corr.h"

#include "asp2_frame.h"

#define DEBUG

/* Contained here and defined in invbisp.h:
int photon_corr_auto1(double *bispp, double *modsq, double *phot_modsq,
                          int nx, int ny, int bisp_dim, double *xphot,
                          int nbeta, int ngamma)
*/
/*
static int compute_nphotons_test(double *modsq, double *bispp, int ngamma,
                                 int nx, int ny, double a1, double *xphot);
*/
static int compute_k_factor(double modsq_0, double phot_modsq_0,
                            double a1, double k_range, double *k_factor);

/****************************************************************
* Photon noise correction found by JLP in May2008
*
*
* Photon noise correction (cf JOSA 2, 14, Wirnitzer):
* (When spectrum normalized to one in the center)
* <i(u)>**2 = E(D...)/N**2 - 1/N
*
* <i(3)(u,v)> = E(D(3)(u,v)/N**3 - E(D(2)(u)/N**2)/N - E(D(2)(v)... +2/N**3)
*
* INPUT:
* modsq[]: mean normalized squared modulus (with zero frequency at nx/2,ny/2)
* phot_modsq[]: power spectrum of photon response (with 0 freq. at nx/2,ny/2)
* bispp[]: bispectrum (sorted in a "standard" way, and thus different
*         from what is needed by photon_corr...)
*
* OUTPUT:
* xphot: mean photon flux per frame
*****************************************************************/
int photon_corr_auto1(double *bispp, double *modsq, double *phot_modsq,
                      int nx, int ny, int bisp_dim, double *xphot,
                      int nbeta, int ngamma, char *err_msg)
{
double *xmodsq, *phot_re, a1, epsilon=1.e-12;
double k_range, k_factor, k_correction, rmin, rmax;
double work1;
int status, k1, k2, k3, iix, iiy, nb, ixc, iyc, ng;
register int i;
wxString buffer;

ixc = nx/2;
iyc = ny/2;

/* Old and DEBUG version: I used the value entered by the user...
k_factor = *modsq;
*/

/* JLP96:
* Number of photons:  xphot**2 + xphot - modsq_0 = 0
* (for photon-counting devices!)
* Delta = b**2 - 4 a*c = 1 + 4 * modsq_0
* Solutions: (- b +/- sqrt(Delta))/2a
*  i.e.,     (-1 + sqrt(1 + 4 * modsq_0) )/2
*/
  work1 = (-1. + sqrt((double)(1.
       + 4. * modsq[ixc + (iyc * nx)]/phot_modsq[ixc + (iyc * nx)])))/2.;


/* New version with compute_nphotons (in "photon_model.c"):
* Computes a1 = xphot / k_factor^2, were xphot is the number of photons per frame,
* by fitting a constant to the ratio modsq/phot_modsq
* for the high frequencies
*
*/

/* JLP SEP2008 for the ICCD in Merate 128x128 pixels
 rmin = 0.1;
 rmax = 0.3;
*/
rmin = 0.1;
rmax = 0.3;
compute_nphotons(modsq, phot_modsq, &a1, nx, ny, nx, rmin, rmax);
/* JLP SEP2008 for the ICCD in Merate 128x128 pixels
k_range = 10.;
*/
k_range = 10.;

/* Compute k_factor from modsq_0 and a1: */
status = compute_k_factor(modsq[ixc + iyc * nx],
                          phot_modsq[ixc + iyc * nx], a1, k_range, &k_factor);
if(status) {
   sprintf(err_msg, "Error in compute_k_factor: could not find a solution. I set it to one");
   k_factor = 1;
  }

/* N = a1 * k^2 */
*xphot = a1 * SQUARE(k_factor);

#ifdef DEBUG
buffer.Printf("Estimates of xphot (from modsq[0 freq] work1=%f) a1=%f  xphot=%f (xfact=%f)\n",
              work1, a1, *xphot, k_factor);
wxMessageBox(buffer, wxT("photon_corr_auto"), wxICON_INFORMATION);
#endif

/* Correction of the experimental power spectrum: */
for(i = 0; i < nx * ny; i++){
    modsq[i] = (modsq[i] / (a1 * phot_modsq[i] + epsilon)) - 1.;
/* Multiplication with N of the corrected power spectrum, to make it
compatible with the bispectrum (for bisp_to_image...): */
    modsq[i] *= *xphot;
    }

/* Allocation of memory for xmodsq, modsq on the spectral list: */
xmodsq = new double[nbeta + 1];

phot_re = new double[nbeta + 1];

/* IXY(1,NB) and IXY(2,NB) are the X,Y coordinates of the
* element NB of the spectral list.
* As they (i.e. IXY) can be negative, and that zero frequency is at (0,0),
* we do the following transformation:
*/
  for(nb = 0; nb <= nbeta; nb++)
     {
/* Get coordinates (i,j) from nb index: */
       COVER_IXY(&iix, &iiy, &nb);
/* OLD VERSION (assuming that zero freq. is at (0,0):
       iix = ((iix + nx) % nx);
       iiy = ((iiy + ny) % ny);
*/
/* NEW VERSION (since that zero freq. has been shifted to (nx/2ny/2):
*/
       iix += nx/2;
       iiy += ny/2;
/* The photon response is assumed to be symmetric and real, hence its FT is real: */
       phot_re[nb] = sqrt(phot_modsq[iix + iiy * nx]);
/* xmodsq is used to store the corrected square modulus */
       xmodsq[nb] = modsq[iix + iiy * nx];
#ifdef DEBUG
       if(nb < 4) {
         printf(" nb = %d iix, iiy: %d %d \n",nb,iix,iiy);
         printf(" xmodsq=%f phot_modsq=%f\n", xmodsq[nb], phot_modsq[nb]);
       }
#endif
     }

/****************************************************************/
/* Phase factor of the bispectrum (with bispectral list):
* and correction from photon noise effects:
*/

/* Division by the real and imaginary parts of the bispectrum
* by the photon response:
* (from 2000 onwards...)
*/
for(ng = 0; ng < ngamma; ng++) {
     cover_klm0(&k1,1,ng);
     cover_klm0(&k2,2,ng);
     cover_klm0(&k3,3,ng);
     work1 = phot_re[k1] * phot_re[k2] * phot_re[k3] + epsilon;
     bispp[ng] /= work1;
     bispp[ng + bisp_dim] /= work1;
  }

k_correction = 1. / (k_factor * k_factor * k_factor);
for(ng = 0; ng < ngamma; ng++)
 {
     cover_klm0(&k1,1,ng);
     cover_klm0(&k2,2,ng);
     cover_klm0(&k3,3,ng);

/* Photon noise correction
     work1 = (xmodsq[k1] + xmodsq[k2] + xmodsq[k3] - 2 * *xphot);
     if xmodsq is not corrected
     or
     work1 = (xmodsq[k1] + xmodsq[k2] + xmodsq[k3] + *xphot);
     if xmodsd is corrected (i.e. xmodsq := xmodsq - xphot)
*/
     work1 = (xmodsq[k1] + xmodsq[k2] + xmodsq[k3]  + *xphot) * k_correction;
#ifdef DEBUG
     if(ng < 4) {
        printf(" ng = %d k, l, m: %d %d %d \n",ng,k1,k2,k3);
        printf(" xmodsq1, xmodsq2, xmodsq3 %f %f %f (k_corr=%f)\n",
                xmodsq[k1], xmodsq[k2], xmodsq[k3], k_correction);
        printf(" ng= %d bispp=%f correction= %f (xphot=%f)\n",
               ng, bispp[ng], work1, *xphot);
     }
#endif
     bispp[ng] -= work1;
  }

delete phot_re;
delete xmodsq;
return(0);
}
/**********************************************************************
*
* Solving equation a1^3 k^4 - k bisp_0 + 3 mod2_0 - 2 a1 = 0
*
* INPUT:
*  k_range: maximum value of the interval [0, k_range] where
*           to look for the solution
**********************************************************************/
static int compute_k_factor(double modsq_0, double phot_modsq_0,
                            double a1, double k_range, double *k_factor)
{
double mod2_0, bisp_0, kf, cte, work, old_value;
int status = -1, nsteps;
register int i;

*k_factor = 1.;

mod2_0 = modsq_0 / phot_modsq_0;
bisp_0 = pow(mod2_0, 1.5);

#ifdef DEBUG
printf("modsq_0=%f phot_modsq_0=%f\n", modsq_0, phot_modsq_0);
printf(" mod2_0=%f bisp_0=%f a1=%f \n", mod2_0, bisp_0, a1);
printf(" bisp_0^{1/3}=%f mod_0^{1/2}=%f \n", pow(bisp_0, 0.333), sqrt(mod2_0));
#endif

cte = 3 * mod2_0 - 2. * a1;
work = bisp_0 - cte;
#ifdef DEBUG
printf("cte = %f    bisp - cte = %f\n", cte, work);
#endif

work /= (a1 * a1 * a1);
#ifdef DEBUG
printf(" bisp_prime = %f\n", work);
#endif

/* From 0 to 10, function is positive very close to zero
* then it is negative and positive again (this is the value we are looking for) */
old_value = +1;
nsteps = 1000.;
  for(i = 0; i < nsteps; i++){
    kf = i * k_range /(double)nsteps;
    work = a1 * a1 * a1 * kf * kf * kf *kf - kf * bisp_0 + cte;
#ifdef DEBUG
    if(i < nsteps) printf(" %d work=%f k_f=%f \n", i, work, kf);
#endif
    if(old_value < 0. && work > 0.) {
     printf("Good value of k_f is k_factor = %f\n", kf);
     *k_factor = kf;
     status = 0;
     break;
     }
    old_value = work;
   }

if(status) fprintf(stderr,"compute_k_factor/Error, cannot find solution...\n");

return(status);
}
/**************************************************************************
* Fit a model of the background
* Contribute to determining the number of photons per frame,
* by fitting a constant a1 to the ratio modsq/phot_modsq
* for the high frequencies
*
* INPUT:
* modsq: power spectrum (with zero frequency at nx/2 ny/2)
* phot_modsq: power spectrum of photon response
* rmin, rmax: relative radii limits for fitting photon reponse
*             (as a fraction of nx)
*
**************************************************************************/
int compute_nphotons(double *modsq, double *phot_modsq, double *a1,
                     int nx, int ny, int idim, double rmin, double rmax)
{
double work, epsilon = 1.e-10, sigma, *tmp, mean, median;
double sum, sumsq;
double radsq_min, radsq_max, radsq;
int jj, ixc, iyc, npts;
register int i, j;
wxString buffer;

/* Default value: */
*a1 = 1.;

ixc = nx/2; iyc = ny/2;

tmp = new double[nx * ny];

/* Central pixels are "polluted" by the power spectrum of the object,
* so I neutralize them for the fit:
*
* For ads11454 with 200 ph/frame (Paper VI)
rmin = 0.4;
rmax = 100;
radsq_min = SQUARE(0.4 * (double)nx);
radsq_max = 100;
* For ads11454 with 20 ph/frame
rmin = 0.1;
rmax = 0.3;
radsq_min = SQUARE(0.1 * (double)nx);
radsq_max = SQUARE(0.3 * (double)nx);
*/
radsq_min = SQUARE(rmin * (double)nx);
radsq_max = SQUARE(rmax * (double)nx);

sum = 0.;
sumsq = 0.;
npts = 0;
for(j = 0; j < ny; j++) {
  jj = j * idim;
  for(i = 0; i < nx; i++) {
   radsq = SQUARE((j-iyc)/1.2) + SQUARE(i - ixc);
   if(radsq > radsq_min && radsq < radsq_max){
     work = modsq[i + jj] / (phot_modsq[i + jj] + epsilon);
     sum += work;
     sumsq += SQUARE(work);
     tmp[npts] = work;
     npts++;
     }
  }
 }

if(npts <= 5) {
  fprintf(stderr, "rad_min,max=%.3f,%.3f\n", radsq_min, radsq_max);
  fprintf(stderr, "compute_nphotons/Error: two few points: npts=%d\n", npts);
  free(tmp);
  return(-1);
  }

 mean = sum/(double)npts;
 sigma = sqrt(sumsq / (double)npts - SQUARE(mean));
// Defined in jlp_numeric.h:
// int JLP_MEDIAN_DBLE(double *data, int npts, double *value);
 JLP_MEDIAN_DBLE(tmp, npts, &median);

#ifdef DEBUG
buffer.Printf(wxT("compute_nphotons/ mean=%f sigma=%f median=%f npts=%d\n"),
        mean, sigma, median, npts);
wxMessageBox(buffer, wxT("photon_corr_auto"), wxICON_INFORMATION);
#endif

*a1 = median;

delete tmp;

return(0);
}
