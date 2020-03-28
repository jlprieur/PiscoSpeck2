/********************************************************************
* Set of routines for uv-coverage, spectral and bispectral lists:
*   
* Contains:
* int COVERA_MASK(mask,nx_mask,ny_mask,ir,max_nclosure,nbeta,ngamma)
* static int couv_mask(mask,nxm,nym,ir,nbeta,ngamma,nbc_dim,nbc_offset)
* static int affect_mask(mask,nxm,nym,isx,isy,irs,nb,ng,
*                       nbc_dim,nbc_offset,max_nclosure,dimension_only)
* int bisp2D_to_2D_image(double* out_image, double* modsq,
*                       int nx, int ny, double* bisp, int nx_bisp,
*                       int nbeta, int ngamma, int ShouldNormalize, int iopt);
* int bisp2D_to_2D_image(out_image,modsq,nx,ny,bisp,nx_bisp,nbeta,ngamma,iopt)
*                        nbeta,ngamma,line_to_output,iopt)
* int bispec3(re,im,modsq,snrm,nx,ny,bispp,ir,nbeta,ngamma)
* int bispec3_fourn(data,modsq,snrm,nx,ny,bispp,ir,nbeta,ngamma)
* int photon_corr(bispp,modsq,snrm,nx,ny,xphot,nbeta,ngamma,photon_correction)
* int photon_corr_mask(bispp,modsq,nx,ny,bisp_dim,xphot,nbeta,ngamma)
* int rearrange_mask(bispp,ngamma,ydim1)
* int compute_uv_coverage(char* fmask_name, int ir, int* nbeta,
*                         int* ngamma, int max_nclosure)
*
* *** 1D routines:
* int COVERA_MASK_1D(mask,nx_mask,ir,max_nclosure,nbeta,ngamma)
* static int couv_mask_1D(mask,nxm,ir,nbeta,ngamma,
* int photon_corr_1D(bispp,modsq,nx,ny,bisp_dim,nbeta,ngamma)
* int bispec_1D_Y(re,im,modsq,snrm,nx,ny,bispp,ir,nbeta,ngamma)
* static int affect_mask_1D(mask,nxm,isx,nb,ng,max_nclosure,dimension_only)
* int compute_uv_coverage_1D(char* fmask_name, int ir, int* nbeta,
*                            int* ngamma, int max_nclosure)
*
*
* FORTRAN interface:
* int COVER_NGT(ngt_val,index)
* int COVER_IXY(ixy1_val,ixy2_val,index)
* int COVER_IXY_1D(ixy1_val,index)
* int COVER_NBCOUV(nb_val,index1,index2,ir_max)
* int COVER_KLM(klm_val,klm_index,ng_index)
*
* Changes:
* Dec 94: double precision arrays for modsq, re, im, and bisp1, ...)
*
* JLP
* Version 02-08-2010
*******************************************************************/
#include <stdio.h>
#include <math.h>
#include "jlp_numeric.h"  // fftw
#include "asp2_covermas.h"

//#define DEBUG

static int couv_mask(double* mask, int nxm, int nym, int* ir, int* nbeta,
                     int* ngamma, int nbc_dim, int nbc_offset,
                     int max_nclosure, int dimension_only);
static int affect_mask(double* mask, int nxm, int nym, int isx, int isy,
                       int irs, int* nb, int* ng,
                       int nbc_dim, int nbc_offset, int max_nclosure,
                       int dimension_only);
static int affect_mask_1D(double* mask, int nxm, int isx, int* nb, int* ng,
                       int max_nclosure, int dimension_only);
static int couv_mask_1D(double* mask, int nxm, int* ir, int* nbeta,
                     int* ngamma, int max_nclosure, int dimension_only);

/* For ir = 25, nbmax = 980  and ngmax = 187566
*  For ir = 30, nbmax = 1410 and ngmax = 388400
*  For ir = 35, nbmax = 1926 and ngmax = 724838
*  For ir = 40, nbmax = 2512 and ngmax = 1233164
*/
/*
#define IRMAX 25
#define NBMAX 980

#define IRMAX 30
#define NBMAX 1410

#define IRMAX 35
#define NBMAX 1926

#define IRMAX 40
#define NBMAX 2512
*/

#define IRMAX 50
#define NBMAX 3922

static int *nbcouv, *ixy_1, *ixy_2, *ngt, *klm;
static double xr[NBMAX], xi[NBMAX];

/********************************************************************
* Subroutine COVERA_MASK
* To compute the frequencies of the uv coverage,
* and the elements of the A matrix.
* (To solve eventually the equation A*X=Y,
*   i.e., to invert the bispectral relations)
*
* Compute u-v coverage with the mask and within a disk of radius ir.
*
* Input:
*  ir: maximum radius of the uv-coverage
*  max_nclosure: maximum number of closure relations
*  mask[nx * ny]: frequency mask ( >=1. if frequency accessible, 0. otherwise)
*
* Output:
*  NBETA: Number of elements of the spectral list (Number of columns of A)
*  NGAMMA: Number of elements of the bispectral list (Number of rows of A)
*
*********************************************************************/
int COVERA_MASK(double* mask, int* nx_mask, int* ny_mask, int* ir,
                int* max_nclosure, int* nbeta, int* ngamma, char *err_message)
{
/* max_nclosure: max number of closure relations per spectral term */
/* Limitation of the maximum number of closure relations
* (to simulate Knox-Thompson and/or accelerate computations...) */
int isize, nxm, nym, ir_max, nbc_dim, nbc_offset;
register int i;

/* JLP 94
 ir_max = IRMAX;
*/
ir_max = *ir;
if(ir_max > IRMAX)
   {
    sprintf(err_message,"Covera_mask/Fatal error: ir max = %d",IRMAX);
    return(-1);
   }
nbc_dim = 2 * ir_max + 1;
nbc_offset = ir_max + 1;

nxm = *nx_mask;
nym = *ny_mask;
/* Check first that mask is correct: */
if(nxm < 2 * (*ir) || nym < 2 * (*ir))
  {
   sprintf(err_message,"Covera_mask/Fatal error: mask is too small !\
 nxm = %d nym = %d ir = %d",nxm,nym,*ir);
   return(-1);
  }

/* nbcouv( x from -IRMAX to IRMAX,  y from 0 to IRMAX ) */
/* JLP94 */
isize = (ir_max + 1) * nbc_dim;
nbcouv = new int[isize];
/* ixy( 1 for x; 2 for y,  nb from 0 to NBMAX ) */
isize = (NBMAX + 1);
ixy_1 = new int[isize];
ixy_2 = new int[isize];
ngt = new int[isize];
/*JLP 94 */
for(i = 0; i < nbc_dim * (ir_max + 1); i++) nbcouv[i] = 0;

/* Computing the A matrix as generated by the uv-coverage
   (defined only by IR for a full pupil) */

/* First call to compute bispectrum size NGMAX = ngamma: */
couv_mask(mask,nxm,nym,ir,nbeta,ngamma,nbc_dim,nbc_offset,*max_nclosure,1);

/* Allocation of memory for bispectrum: */
isize = 3 * (*ngamma);
klm = new int[isize];

/* Second call to compute spectral and bispectral lists: */
couv_mask(mask,nxm,nym,ir,nbeta,ngamma,nbc_dim,nbc_offset,*max_nclosure,0);

#ifdef DEBUG
sprintf(err_message," IR=%d NBETA (spectral list)=%d NGAMMA (bispec. list)=%d",
        *ir, *nbeta, *ngamma);
#endif

/* Computing the number of closure relations for some values of NB: */
/*
{ int k, iw1;
for (int k = 2; k < 5 ; k++) {
   i = 3 + (*nbeta * k) /5;
   iw1 = sqrt((double)(ixy_1[i] * ixy_1[i] + ixy_2[i] * ixy_2[i]));
   printf(" NB = %d irad=%d Closure relations: %d \n",
             i,iw1,ngt[i] - ngt[i - 1]);
  }
}
*/
 
/* Free memory: */
/* JLP96: don't do that, since nbcouv is not accessible otherwise... */
/*
delete[] nbcouv;
*/

return(0);
}
/*******************************************************************
* couv_mask defines the uv-coverage and the A matrix:
* Input:
* mask[nx * ny]: frequency mask ( >=1. if frequency accessible, 0. otherwise)
* IR: maximum radius of the uv-coverage
* dimension_only: flag set to one if only bispectral list dimension is required
*
* Output:
* NBETA, NGAMMA: number of elements of the spectral and bispectral lists
*
* In common blocks (output): the uv-coverage is accessible from 2 sides:
*
* NBCOUV(I,J): uv-coverage (i.e. number of the spectral list for
*              the pixel(I,J) of the spectrum (I=0,J=0 for null frequency)
*
* IXY(1,I) and IXY(2,I) coordinates of the pixel number I in the spectral list
*              (this allows another entry for the uv-coverage)
*
*************************************************************************/
static int couv_mask(double* mask, int nxm, int nym, int* ir, int* nbeta,
                     int* ngamma, int nbc_dim, int nbc_offset, 
                     int max_nclosure, int dimension_only)
{
/* 
* nb: beta index, i.e. spectral list index
* ng: gamma index, i.e. bispectral list index
*/
int ir2max, nb, ng;
int i, i2, j2, ii2, irs, icent;
register int j, ir2;
 
/* Coordinate of center of the mask: */
icent = nxm/2 + nxm * nym/2;
 
/* Easy cases: */
/* First spectral value at (0,0) */
 nb = 0;
 i = 0; j = 0;
 if(mask[ i + icent + nxm * j] > 0.)
   {
   ixy_1[nb] = i; ixy_2[nb] = j;
   nbcouv[i + nbc_offset + nbc_dim * j] = nb;
   }
 
/* Second spectral value at (1,0), nb=1 */
 i = 1; j = 0;
 if(mask[ i + icent + nxm * j] > 0.)
   {
   nb++;
   ixy_1[nb] = i; ixy_2[nb] = j;
   nbcouv[i + nbc_offset + nbc_dim * j] = nb;
   }

/* Third spectral value at (0,1), nb=2 */
 i = 0; j = 1;
 if(mask[ i + icent + nxm * j] > 0.)
   {
   nb++;
   ixy_1[nb] = i; ixy_2[nb] = j;
   nbcouv[i + nbc_offset + nbc_dim * j] = nb;
   }

/* Reseting the total number of elements the  bispectral list */
 ng = 0;
 
/* Main loop: work with successive iterations on circles with
* increasing radii.
* Squared radius: IR2 = 2, 3, ... ,IR2MAX
*/
ir2max = *ir * *ir;
for ( ir2 = 2; ir2 <= ir2max; ir2++) 
  {
/* Searching for the couples (I,J) such as: I**2 + J**2 = IR2 with I>=J */
    for ( j = 0; j <= *ir; j++) 
      {
       j2 = j*j; 
       i2 = ir2 - j2;
       if ( i2 < j2) break;
         i = (int)sqrt((double)i2);
         ii2 = i * i;
/* Selecting the points defined by each couple (i,j)
   such that (i*i + j*j= ir2): */
         if(ii2 == i2)
            {
              irs = (int)sqrt((double)ir2);
               affect_mask(mask,nxm,nym,i,j,irs,&nb,&ng,
                           nbc_dim,nbc_offset,max_nclosure,dimension_only);
/* Now use the symmetry relations: */
              if( i != j) 
                affect_mask(mask,nxm,nym,j,i,irs,&nb,&ng,
                           nbc_dim,nbc_offset,max_nclosure,dimension_only);
              if( j != 0) 
                affect_mask(mask,nxm,nym,-j,i,irs,&nb,&ng,
                           nbc_dim,nbc_offset,max_nclosure,dimension_only);
              if( i != j && j != 0) 
                affect_mask(mask,nxm,nym,-i,j,irs,&nb,&ng,
                           nbc_dim,nbc_offset,max_nclosure,dimension_only);
            }
      }
  }

/* NBETA: Total number of the spectral list (Number of columns of the X matrix)
* NGAMMA: Total number of the bispectral list (Number of rows of the X matrix)
* (Remember, we have to solve    A*X = Y) */
  *nbeta = nb;
  *ngamma = ng;

  return(0);
}
/*******************************************************************
* affect_mask:
* Gives a structure to the S group of the A matrix
* Input:
* ISX, ISY : coordinates
* IRS: radius
*
* Output:
* NB: index in the spectral list
* NG: index in the bispectral list
********************************************************************/
static int affect_mask(double* mask, int nxm, int nym, int isx, int isy,
                       int irs, int *nb, int *ng,
                       int nbc_dim, int nbc_offset, int max_nclosure,
                       int dimension_only)
{ 
int nbs, nbk, nbr, itx, ity, icent;
register int nbq, iklm;

/* Coordinate of center of the mask: */
icent = nxm/2 + nxm * nym/2; 
 
/* First condition: input point has to be accessible */
  if(mask[isx + icent + isy * nxm] <= 0.) return(-1);

/* If so, record the new point of the uv coverage (spectral list): */
 (*nb)++;
 ixy_1[*nb] = isx; ixy_2[*nb] = isy;
 nbcouv[isx + nbc_offset + nbc_dim * isy] = *nb;

/* Searching for the couples associated with the point NBS=NB
* Generating the bispectral list and building the rows of the A matrix:
*/
 nbs = *nb;
 
/* Loop on all the possible points (Q): */
 iklm = 3 * (*ng);
 for( nbq = 1; nbq < nbs; nbq++)
  { 
/* JLP 94: add an exit test when maximum number of closure relations
has been found (to simulate Knox-Thompson and/or accelerate computations...) */ 
   if((*ng - ngt[(*nb) -1]) == max_nclosure) break;

/* Coordinates of the vector T = S - Q */
   itx = isx - ixy_1[nbq];
   ity = isy - ixy_2[nbq];
 
/* Work within the circle of radius IRS, so we can a priori reject
* the points outside the window [-IRS,+IRS]:
*/
   if(itx >= -irs && itx <= irs && ity >= -irs && ity <= irs)
      {
       if(ity > 0 || ( ity == 0 && itx >= 0))
          {
/* Case number 1 (which could be : k=t, l=q, k+l=s) */

/* It should be accessible too: */
            if(mask[itx + icent + ity * nxm] > 0.)
            {
              nbk = nbcouv[ itx + nbc_offset + ity * nbc_dim];
 
/* We select this couple (U,V) if the vector NBK is in [0,NBQ] */
              if( nbk != 0 && nbk <= nbq)
                 {
/* Add new k,l,m coordinates if more than dimension is wanted */
                 if(!dimension_only)
                   {
                   klm[iklm] = nbk; 
                   iklm++;
                   klm[iklm] = nbq; 
                   iklm++;
                   klm[iklm] = nbs;
                   iklm++;
                   }
                 (*ng)++;
                 }
            }
          }
      else
          {
/* Case number 2 (which could be : r=-t, s=s, r+s=m=q) */
/* r should be accessible too: */
            if(mask[-itx + icent - ity * nxm] > 0.)
            {
             nbr = nbcouv[ -itx + nbc_offset - ity * nbc_dim];
/* We select this couple (R,S) if the vector NBR is in [0,NBS] */
              if( nbr != 0 && nbr <= nbs)
                  {
/* Add new k,l,m coordinates if more than dimension is wanted */
                  if(!dimension_only)
                     {
                     klm[iklm] = nbr;
                     iklm++;
                     klm[iklm] = nbs;
                     iklm++;
                     klm[iklm] = nbq;
                     iklm++;
                     }
                  (*ng)++;
                  }
            }
/* Nota: we can only have L=NBS or M=NBS (never K=NBS) */
          }
      } 

/* End of loop on nbq */
}

/* NGT(NB) is the number of the last U,V couple of the group NB=NBS=S: */
   ngt[nbs] = *ng;

 return(0);
}
#ifdef TO_BE_DELETED
/*******************************************************************
* select_fwhm
* to select frames of good quality
*******************************************************************/
int select_fhwm(double* re, double* im, int* nx, int* ny,
                double* work_array, double max_radius)
{
int status;
status = 0;
return(status);
}
#endif
/*******************************************************************
* bispec3
* Same as bispec1, but newer version.
* Sum of full bispectrum terms (amplitude and phase)
* Reads a spectrum from an real image (from observations)
*
* Computes the bispectrum and spectrum lists from RE and IM
* already computed before calling this routine.
*
* Integrates the squared modulus of the spectrum and
* the phase factor of the bispectrum.
* Does not correct from photon noise effects.
*
* Input:
* RE(NX,NY) (please note that idim has disappeared...)
* IM(NX,NY)
*
* Output:
* MODSQ: Sum of the modulus squared
* YCE1: phase factor of the bispectrum (real and imaginary)
* YCE1(.,3): sum of square real parts of the bispectrum
* YCE1(.,4): sum of square imaginary parts of the bispectrum
********************************************************************/
int bispec3(double* re, double* im, double* modsq, double* snrm,
            int nx, int ny, double* bispp, int ir, int nbeta, int ngamma)
{
double w1;
int k1, k2, k3;
double wr1, wr2, wr3, wi1, wi2, wi3;
double aar, aai, work1, work2;
register int ng, i, nb, yce_ng, iklm;

#ifdef DEBUG
/* Check the FFT (zero frequency is at (0,0)): */
w1 = re[0]*re[0] + im[0]*im[0];
w1 = sqrt(w1);
printf(" xphot1 = %f \n",w1);
#endif

/* IXY(1,NB) and IXY(2,NB) are the X,Y coordinates of the
* element NB of the spectral list.
* As they (i.e. IXY) can be negative, and that zero frequency is at (0,0),
* we do the following transformation:
*/
  for(nb = 0; nb <= nbeta; nb++)
     {
       i = ((ixy_1[nb] + nx) % nx)
           + (nx * ((ixy_2[nb] + ny) % ny));
       xr[nb] = re[i];
       xi[nb] = im[i];
     }

/****************************************************************/
/* Phase factor of the bispectrum (with bispectral list):
* (No correction of photon noise)
*/
yce_ng = 0;
iklm = 0;
for(ng = 0; ng < ngamma; ng++)
 {
     k1 = klm[iklm]; iklm++;
     k2 = klm[iklm]; iklm++;
     k3 = klm[iklm]; iklm++;
     wr1 = xr[k1]; wr2 = xr[k2]; wr3 = xr[k3];
     wi1 = xi[k1]; wi2 = xi[k2]; wi3 = xi[k3];
/*  CC1=XC1*XC2*CONJG(XC3): AAR*WR3+AAI*WI3 */
     aar = wr1 * wr2 - wi1 * wi2;
     aai = wr2 * wi1 + wr1 * wi2;
     work1 = aar * wr3 + aai * wi3;
     work2 = aai * wr3 - aar * wi3;
     bispp[yce_ng] += work1;
     yce_ng++;
     bispp[yce_ng] += work2;

/* Estimation of the noise with the sum of squares: */
     yce_ng++;
     bispp[yce_ng] += work1*work1;
     yce_ng++;
     bispp[yce_ng] += work2*work2;
     yce_ng++;
  }

/****************************************************************/
/* Squared modulus for the output (not corrected for photon noise) */
    for( i = 0; i < nx * ny; i++)
       {
        w1 = re[i]*re[i] + im[i]*im[i];
        modsq[i] += w1;
        snrm[i] += (w1 * w1);
       }
 
 return(0);
}
/*******************************************************************
* bispec3_fftw
* Same as bispec3, but adapted to "FFTW" format
*
* Input:
* FFTW_COMPLEX data(NX*NY) 
*
* Output:
* MODSQ: Sum of the modulus squared
* YCE1: phase factor of the bispectrum (real and imaginary)
* YCE1(.,3): sum of square real parts of the bispectrum
* YCE1(.,4): sum of square imaginary parts of the bispectrum
********************************************************************/
int bispec3_fftw(FFTW_COMPLEX *data, double* modsq, double* snrm,
                 int nx, int ny, double* bispp, int ir,
                 int nbeta, int ngamma, char *err_message)
{
double w1;
int k1, k2, k3;
double wr1, wr2, wr3, wi1, wi2, wi3;
double aar, aai, work1, work2;
register int ng, i, nb, yce_ng, iklm;

#ifdef DEBUG
/* Check the FFT (zero frequency is at (0,0)): */
w1 = c_re(data[0])*c_re(data[0]) + c_im(data[0])*c_im(data[0]);
w1 = sqrt(w1);
printf(" xphot1 = %f \n",w1);
#endif

/* IXY(1,NB) and IXY(2,NB) are the X,Y coordinates of the
* element NB of the spectral list.
* As they (i.e. IXY) can be negative, and that zero frequency is at (0,0),
* we do the following transformation:
*/
  for(nb = 0; nb <= nbeta; nb++)
     {
       i = ((ixy_1[nb] + nx) % nx)
           + (nx * ((ixy_2[nb] + ny) % ny));
       xr[nb] = c_re(data[i]);
       xi[nb] = c_im(data[i]);
     }

/****************************************************************/
/* Phase factor of the bispectrum (with bispectral list):
* (No correction of photon noise)
*/
yce_ng = 0;
iklm = 0;
for(ng = 0; ng < ngamma; ng++)
 {
     k1 = klm[iklm]; iklm++;
     k2 = klm[iklm]; iklm++;
     k3 = klm[iklm]; iklm++;
     if(k3 < 0 || k3 > nbeta)
     {
     sprintf(err_message,"bisp3_fftw/iklm=%d, k3=%d, ng=%d ngamma=%d yce_ng=%d\n",
            iklm,k3,ng,ngamma,yce_ng);
     return(-1);
     }
     wr1 = xr[k1]; wr2 = xr[k2]; wr3 = xr[k3];
     wi1 = xi[k1]; wi2 = xi[k2]; wi3 = xi[k3];
/*  CC1=XC1*XC2*CONJG(XC3): AAR*WR3+AAI*WI3 */
     aar = wr1 * wr2 - wi1 * wi2;
     aai = wr2 * wi1 + wr1 * wi2;
     work1 = aar * wr3 + aai * wi3;
     work2 = aai * wr3 - aar * wi3;
     bispp[yce_ng] += work1;
     yce_ng++;
     bispp[yce_ng] += work2;

/* Estimation of the noise with the sum of squares: */
     yce_ng++;
     bispp[yce_ng] += SQUARE(work1);
     yce_ng++;
     bispp[yce_ng] += SQUARE(work2);
     yce_ng++;
  }

/****************************************************************/
/* Squared modulus for the output (not corrected for photon noise) */
    for( i = 0; i < nx * ny; i++)
       {
        w1 = SQUARE(c_re(data[i])) + SQUARE(c_im(data[i]));
        modsq[i] += w1;
        snrm[i] += (w1 * w1);
       }

 return(0);
}
/*******************************************************************
* bispec3_fourn
* Same as bispec3, but adapted to "fourn" format
*
* Input:
* DATA(2*NX*NY)
*
* Output:
* MODSQ: Sum of the modulus squared
* YCE1: phase factor of the bispectrum (real and imaginary)
* YCE1(.,3): sum of square real parts of the bispectrum
* YCE1(.,4): sum of square imaginary parts of the bispectrum
********************************************************************/
int bispec3_fourn(double* data, double* modsq, double* snrm,
            int nx, int ny, double* bispp, int ir, int nbeta, int ngamma)
{
double w1;
int k1, k2, k3;
double wr1, wr2, wr3, wi1, wi2, wi3;
double aar, aai, work1, work2;
register int ng, i, nb, yce_ng, iklm;

/* IXY(1,NB) and IXY(2,NB) are the X,Y coordinates of the
* element NB of the spectral list. 
* As they (i.e. IXY) can be negative, and that zero frequency is at (0,0),
* we do the following transformation:
*/
  for(nb = 0; nb <= nbeta; nb++)
     {
       i = ((ixy_1[nb] + nx) % nx) 
           + (nx * ((ixy_2[nb] + ny) % ny)); 
       xr[nb] = data[2*i +1];
       xi[nb] = data[2*i +2];
     }
 
/****************************************************************/
/* Phase factor of the bispectrum (with bispectral list):
* (No correction of photon noise)
*/
yce_ng = 0;
iklm = 0;
for(ng = 0; ng < ngamma; ng++)
 {
     k1 = klm[iklm]; iklm++;
     k2 = klm[iklm]; iklm++;
     k3 = klm[iklm]; iklm++;
     wr1 = xr[k1]; wr2 = xr[k2]; wr3 = xr[k3];
     wi1 = xi[k1]; wi2 = xi[k2]; wi3 = xi[k3];
/*  CC1=XC1*XC2*CONJG(XC3): AAR*WR3+AAI*WI3 */
     aar = wr1 * wr2 - wi1 * wi2;
     aai = wr2 * wi1 + wr1 * wi2;
     work1 = aar * wr3 + aai * wi3;
     work2 = aai * wr3 - aar * wi3;
     bispp[yce_ng] += work1;
     yce_ng++;
     bispp[yce_ng] += work2;

/* Estimation of the noise with the sum of squares: */
     yce_ng++;
     bispp[yce_ng] += work1*work1;
     yce_ng++;
     bispp[yce_ng] += work2*work2;
     yce_ng++;
  }
 
/****************************************************************/
/* Squared modulus for the output (not corrected for photon noise) */
    for( i = 0; i < nx * ny; i++)
       {
        w1 = data[2*i+1]*data[2*i+1] + data[2*i+2]*data[2*i+2];
        modsq[i] += w1;
        snrm[i] += (w1 * w1);
       }

 return(0);
}
/*******************************************************************
* bispec_1D_Y
* From bispec3, 1-D version for spectra
* Sum of full bispectrum terms (amplitude and phase)
* Reads a spectrum from a real image (from observations)
*
* Computes the bispectrum and spectrum lists from RE and IM
* already computed before calling this routine 
* (Fourier spectra along the columns).
*
* Integrates the squared modulus of the spectrum and
* the phase factor of the bispectrum. 
*
* Does not correct from photon noise effects. 
*
* Input:
* RE(NX,NY) (please note that idim has disappeared...)
* IM(NX,NY)
*
* Output:
* MODSQ: Sum of the modulus squared
* YCE1(.,0,iy) and YCE1(.,1,iy): phase factor of the bispectrum (real and imaginary)
* YCE1(.,2,iy): sum of square real parts of the bispectrum
* YCE1(.,3,iy): sum of square imaginary parts of the bispectrum
********************************************************************/
int bispec_1D_Y(double* re, double* im, double* modsq, double* snrm,
              int nx, int ny, double* bispp, int ir, int nbeta, int ngamma)
{
double w1;
int k1, k2, k3;
double wr1, wr2, wr3, wi1, wi2, wi3;
double aar, aai, work1, work2;
int iiy, ng, nb, ix_b, bisp_dim;
register int ix, i;

bisp_dim = ngamma * 4;

for(ix = 0; ix < nx; ix++)
  {
  ix_b = ix * bisp_dim;
  for(nb = 0; nb <= nbeta; nb++)
     {
/* Get coordinates (i,0) from nb index: */
       COVER_IXY_1D(&iiy,&nb);
/* IXY(1,NB) and IXY(2,NB) are the X,Y coordinates of the
* element NB of the spectral list.
* As they (i.e. IXY) can be negative, and that zero frequency is at (0,0),
* we do the following transformation:
*/
       iiy = ((iiy + ny) % ny);
       xr[nb] = re[ix + iiy*nx];
       xi[nb] = im[ix + iiy*nx];
     }

/****************************************************************/
/* Phase factor of the bispectrum (with bispectral list):
* (No correction of photon noise)
*/
  for(ng = 0; ng < ngamma; ng++)
    {
     cover_klm0(&k1,1,ng);
     cover_klm0(&k2,2,ng);
     cover_klm0(&k3,3,ng);
     wr1 = xr[k1]; wr2 = xr[k2]; wr3 = xr[k3];
     wi1 = xi[k1]; wi2 = xi[k2]; wi3 = xi[k3];
/*  CC1=XC1*XC2*CONJG(XC3): AAR*WR3+AAI*WI3 */
     aar = wr1 * wr2 - wi1 * wi2;
     aai = wr2 * wi1 + wr1 * wi2;
     work1 = aar * wr3 + aai * wi3;
     work2 = aai * wr3 - aar * wi3;
     bispp[ng + ix_b] += work1;
     bispp[ng + ngamma + ix_b] += work2;

/* Estimation of the noise with the sum of squares: */
     bispp[ng + 2*ngamma + ix_b] += work1*work1;
     bispp[ng + 3*ngamma + ix_b] += work2*work2;
  }

/****************************************************************/
/* Squared modulus for the output (not corrected for photon noise) */
    for( i = 0; i < ny; i++)
       {
        w1 = re[ix + i*nx]*re[ix + i*nx] + im[ix + i*nx]*im[ix + i*nx];
        modsq[ix + i*nx] += w1;
        snrm[ix + i*nx] += (w1 * w1);
       }
/* End of loop on ix: */
  }

 return(0);
}
/*********************************************************************
*
* jlp_normalize_fft_1D
*********************************************************************/
int jlp_normalize_fft_1D(double* bispp, double* modsq, double* snrm,
                         int* nx, int* ny, int* nbeta, int* ngamma)
{
double norm_fft, norm_fft2, norm_fft3, norm_fft4, norm_fft6;
int bisp_dim, iy_s, iy_b;
register int iy, i, ng;

norm_fft = sqrt((double)(*nx));
norm_fft2 = *nx;
norm_fft3 = norm_fft2 * norm_fft;
norm_fft4 = norm_fft2 * norm_fft2;
norm_fft6 = norm_fft3 * norm_fft3;

bisp_dim = *ngamma;

for(iy = 0; iy < *ny; iy++)
  {
    iy_s = iy * (*nx);
    iy_b = iy * bisp_dim * 4;
/* Normalizes FFT (for compatibility with FFT_2D instead of FFT_2D_FAST...*/
  for( i = 0; i < *nx; i++)
     {
     modsq[i + iy_s] *= norm_fft2;
     snrm[i + iy_s] *= norm_fft4;
     }

   for(ng = 0; ng < *ngamma; ng++)
     {
     bispp[ng + iy_b] *= norm_fft3;
     bispp[ng + bisp_dim + iy_b] *= norm_fft3;
     bispp[ng + 2*bisp_dim + iy_b] *= norm_fft6;
     bispp[ng + 3*bisp_dim + iy_b] *= norm_fft6;
     }
  }

return(0);
}
/*********************************************************************
*
* jlp_normalize_fft
*********************************************************************/
int jlp_normalize_fft(double* bispp, double* modsq, double* snrm,
                      int* nx, int* ny, int* nbeta, int* ngamma)
{
double norm_fft, norm_fft2, norm_fft3, norm_fft4, norm_fft6;
register int i, ng, yce_ng;

norm_fft = sqrt((double)(*nx * *ny));
norm_fft2 = *nx * *ny;
norm_fft3 = norm_fft2 * norm_fft;
norm_fft4 = norm_fft2 * norm_fft2;
norm_fft6 = norm_fft3 * norm_fft3;

/* Normalizes FFT (for compatibility with FFT_2D instead of FFT_2D_FAST...*/
  for( i = 0; i < *ny * *nx; i++)
     {
     modsq[i] *= norm_fft2;
     snrm[i] *= norm_fft4;
     }

   yce_ng = 0;
   for(ng = 0; ng < *ngamma; ng++)
     {
     bispp[yce_ng] *= norm_fft3;
     yce_ng++; 
     bispp[yce_ng] *= norm_fft3;
     yce_ng++; 
     bispp[yce_ng] *= norm_fft6;
     yce_ng++; 
     bispp[yce_ng] *= norm_fft6;
     yce_ng++; 
     }

return(0);
}
/****************************************************************
* Photon noise correction: (called by decode_car2 )
*
*  Input:
* modsq[]: mean normalized squared modulus
* xphot: mean photon flux per frame
*
* Photon noise correction (cf JOSA 2, 14, Wirnitzer):
* (When spectrum normalized to one in the center)
* <i(u)>**2 = E(D...)/N**2 - 1/N
*
* <i(3)(u,v)> = E(D(3)(u,v)/N**3 - E(D(2)(u)/N**2)/N - E(D(2)(v)... +2/N**3)
*
*****************************************************************/
int photon_corr(double* bispp, double* modsq, double* snrm,
                int* nx, int* ny, double* xphot, int* nbeta, int* ngamma,
                int* photon_correction)
{
double xphot2, xphot3, xphot4, xphot6;
double w2, work1;
int k1, k2, k3, iix, iiy;
register int i, nb, ng, yce_ng, iklm;

/* Normalizes FFT (for compatibility with FFT_2D instead of FFT_2D_FAST...*/
jlp_normalize_fft(bispp,modsq,snrm,nx,ny,nbeta,ngamma);

/*******************************************************/
/* Return if no correction is needed: */
if(!*photon_correction) return(0);

/*******************************************************/
/* First the bispectrum */

  xphot2 = *xphot * *xphot;
/* IXY(1,NB) and IXY(2,NB) are the X,Y coordinates of the
* element NB of the spectral list.
* As they (i.e. IXY) can be negative, and that zero frequency is at (0,0),
* we do the following transformation:
*/
  for(nb = 0; nb <= *nbeta; nb++)
     {
       iix = ((ixy_1[nb] + *nx) % *nx);
       iiy = ((ixy_2[nb] + *ny) % *ny);
/* Here xr is used to store the mean normalized square modulus: */
       xr[nb] = modsq[iix + iiy * *nx];
     }

/****************************************************************/
/* Phase factor of the bispectrum (with bispectral list):
* and correction from photon noise effects:
*/
 w2 = 2. *  *xphot;
 yce_ng = 0;
 iklm = 0;
for(ng = 0; ng < *ngamma; ng++)
 {
     k1 = klm[iklm]; iklm++;
     k2 = klm[iklm]; iklm++;
     k3 = klm[iklm]; iklm++;
/* Photon noise correction
*/
     work1 = - (xr[k1] + xr[k2] + xr[k3]) + w2;
     bispp[yce_ng] += work1;

/* Estimation of the noise with the sum of squares: */
     yce_ng++; yce_ng++;
     bispp[yce_ng] -= work1*work1;
     yce_ng++; yce_ng++;
  }

/*******************************************************/
/* Then correcting the squared modulus: */
  xphot4 = xphot2 * xphot2;
  for( i = 0; i < *ny * *nx; i++)
     {
/* Photon noise correction: (biased_sq = xphot + xphot_sq * unbiased_sq)*/
      modsq[i] -= *xphot;
/* Normalization for compatibility with previous computations: */
      modsq[i] /= xphot2;
      snrm[i] /= xphot4;
    }

/*******************************************************/
xphot3 = *xphot * *xphot * *xphot;
xphot6 = xphot3 * xphot3;
yce_ng = 0;
for(ng = 0; ng < *ngamma; ng++)
 {
     bispp[yce_ng] /= xphot3;
     yce_ng++;
     bispp[yce_ng] /= xphot3;
     yce_ng++;
     bispp[yce_ng] /= xphot6;
     yce_ng++;
     bispp[yce_ng] /= xphot6;
     yce_ng++;
 }

return(0);
}
/*******************************************************
* From sequential bispectral list to "2D" array (4 * ngamma)
*******************************************************/
int rearrange_mask(double* bispp, int ngamma)
{
int isize;
register int ng, yce_ng;
double *work;

isize = 4 * ngamma;
work = new double[isize];

/* Erases work array: */
for(ng = 0; ng < 4 * ngamma; ng++) work[ng] = 0.;

/* Transfer to temporary array: */
yce_ng = 0;
for(ng = 0; ng < ngamma; ng++)
 {
     work[ng] = bispp[yce_ng];
     yce_ng++;
     work[ng + ngamma] = bispp[yce_ng];
     yce_ng++;
     work[ng + 2 * ngamma] = bispp[yce_ng];
     yce_ng++;
     work[ng + 3 * ngamma] = bispp[yce_ng];
     yce_ng++;
 }

/* Transfer back to bispp: */
for(ng = 0; ng < 4 * ngamma; ng++) bispp[ng] = work[ng];

delete[] work;
return(0);
}
/**************************************************************** 
* Photon noise correction (Called by main program "phot_noise_mask.c"):
*
*  Input:
* modsq[]: mean normalized squared modulus
* xphot: mean photon flux per frame 
* bispp[]: bispectrum (sorted in a "standard" way, and thus different
*         from what is needed by photon_corr...)
*
* Photon noise correction (cf JOSA 2, 14, Wirnitzer):
* (When spectrum normalized to one in the center)
* <i(u)>**2 = E(D...)/N**2 - 1/N
*
* <i(3)(u,v)> = E(D(3)(u,v)/N**3 - E(D(2)(u)/N**2)/N - E(D(2)(v)... +2/N**3)
* fraction: percentage of the correction of modsq to be applied to bispectrum
*****************************************************************/
int photon_corr_mask(double* bispp, double* modsq, int* nx, int* ny,
                     int* bisp_dim, double* xphot, int* nbeta, int* ngamma,
                     double* fraction)
{
double w2, work1;
int k1, k2, k3, iix, iiy;
register int i, nb, ng, iklm;


/*******************************************************/
/* First the bispectrum */

//  xphot2 = *xphot * *xphot;
/* IXY(1,NB) and IXY(2,NB) are the X,Y coordinates of the
* element NB of the spectral list.
* As they (i.e. IXY) can be negative, and that zero frequency is at (0,0),
* we do the following transformation:
*/
  for(nb = 0; nb <= *nbeta; nb++)
     {
       iix = ((ixy_1[nb] + *nx) % *nx);
       iiy = ((ixy_2[nb] + *ny) % *ny);
/* Here xr is used to store the mean (not normalized) square modulus: */
       xr[nb] = modsq[iix + iiy * *nx];
     }

/* JLP96: */
/* Number of photons:  xphot**2 + xphot - modsq_0 = 0
* Delta = b**2 - 4 a*c = 1 + 4 * modsq_0
* Solutions: (- b +/- sqrt(Delta))/2a
*  i.e.,     (-1 + sqrt(1 + 4 * modsq_0) )/2
*/
//  work = (-1. + sqrt((double)(1. + 4. * xr[0])))/2.;
//  printf(" My estimate of xphot (from modsq[0]) is: %f, whereas yours is: %f \n",
//           work,*xphot);
/* End of JLP96. */
//  printf("Correcting bispectrum with fraction=%f of square modulus \n",*fraction);

/****************************************************************/
/* Phase factor of the bispectrum (with bispectral list):
* and correction from photon noise effects:
*/
 w2 = 2. *  *xphot;
 iklm = 0;
for(ng = 0; ng < *ngamma; ng++)
 {
     k1 = klm[iklm]; iklm++;
     k2 = klm[iklm]; iklm++;
     k3 = klm[iklm]; iklm++;
/* Photon noise correction
*/
     work1 = - (xr[k1] + xr[k2] + xr[k3]) * *fraction + w2;
     bispp[ng] += work1;
/* Phase factor: */
/* JLP96: Do not normalize it (to be able to visualize it with bisp_to_image)
     w1 = bispp[ng]*bispp[ng] + bispp[ng + *bisp_dim] * bispp[ng + *bisp_dim];
     if(w1 > 0)
       {
       w1 = sqrt(w1);
       bispp[ng] /= w1;
       bispp[ng + *bisp_dim] /= w1;
       }
*/

  }
/*******************************************************/
/* Then correcting the squared modulus: */
/* (Normalization to obtain unity in the center (zero frequency is at 0,0 here))
  w2 = modsq[0] - *xphot;
 */
  for( i = 0; i < *ny * *nx; i++)
     {
/* Photon noise correction: (biased_sq = xphot + xphot_sq * unbiased_sq)*/
      modsq[i] -= *xphot;
/* Normalization for compatibility with previous computations: */
/* and get xphot in the central frequency: (instead of xphot**2): */
/* JLP96: Do not normalize it (to be able to visualize it with bisp_to_image)
      modsq[i] /= w2;
*/
    }

return(0);
}
/****************************************************************
* Photon noise correction for slit spectra (Called by phot_noise_1D):
*
*  Input:
* modsq[]: mean (not normalized) squared modulus
* xphot: mean photon flux per frame 
* bispp[]: bispectrum (sorted in a "standard" way, and thus different
*         from what is needed by photon_corr...)
*
* Photon noise correction (cf JOSA 2, 14, Wirnitzer):
* (When spectrum normalized to one in the center)
* <i(u)>**2 = E(D...)/N**2 - 1/N
* <i(3)(u,v)> = E(D(3)(u,v)/N**3 - E(D(2)(u)/N**2)/N 
*               - E(D(2)(v)/N**2)/N -E(D(2)(-u-v)/N**2)/N +2/N**3)
* When it is not normalized:
* (cf Wirnitzer, JOSA A 1985, p16)
* Q3 = D3(u,v) - (D2(u) + D2(v) + D2(-u-v) - 2N)
* where D2= N + N**2 <|i(u)|**2>
*****************************************************************/
int photon_corr_1D(double* bispp, double* modsq, int* nx, int* ny,
                   int* bisp_dim, int* nbeta, int* ngamma)
/*
* WARNING: single precision !!!!! 
*/
{
double xphot, modsq_0;
double w2, work1;
int k1, k2, k3, iix, nb, ng, iy_b,iy_s;
register int iy, i;

/*******************************************************/
/* First the bispectrum */

for(iy = 0; iy < *ny; iy++)
  { 
/* Index of bispectrum list: */
   iy_b = iy * (*bisp_dim);
/* Index of spectrum line: */
   iy_s = iy * (*nx); 
/* Number of photons:  N**2 + N at zero frequency 
*  (i.e., modsq_0=N**2+N) */
   modsq_0 = modsq[0+iy_s];
/* Case of empty line: */
   if(modsq_0 < 0)
     {
     modsq[0+iy_s] = 0.;
/* Set modulus to zero and leave bispectrum unchanged */
     }
/* Case of non-empty line: */
   else
     {
/* Number of photons:  xphot**2 + xphot - modsq_0 = 0
* Delta = b**2 - 4 a*c = 1 + 4 * modsq_0
* Solutions: (- b +/- sqrt(Delta))/2a
*  i.e.,     (-1 + sqrt(1 + 4 * modsq_0) )/2
*/
     xphot = (-1. + sqrt((double)(1. + 4. * modsq_0)))/2.;
     for(nb = 0; nb <= *nbeta; nb++)
       {
/* Get coordinates (iix,iiy) from nb index: */
       COVER_IXY_1D(&iix,&nb);
/* IXY(1,NB) and IXY(2,NB) are the X,Y coordinates of the
* element NB of the spectral list. 
* As they (i.e. IXY) can be negative, and that zero frequency is at (0,0),
* we do the following transformation:
* (JLP96: transformation checked, OK)
*/
       iix = ((iix + *nx) % *nx); 
       xr[nb] = modsq[iix + iy_s];
       }

/****************************************************************/
/* Phase factor of the bispectrum (with bispectral list):
* and correction from photon noise effects:
*/
      w2 = 2. *  xphot;
      for(ng = 0; ng < *ngamma; ng++)
        {
        cover_klm0(&k1,1,ng);
        cover_klm0(&k2,2,ng);
        cover_klm0(&k3,3,ng);
/* Photon noise correction 
*/
        work1 = - (xr[k1] + xr[k2] + xr[k3]) + w2;
        bispp[ng + iy_b] += work1;
/* To get the phase factor, need normalization : */
/* Commented out:
        w1 = bispp[ng + iy_b]*bispp[ng + iy_b] 
         + bispp[ng + *bisp_dim + iy_b] * bispp[ng + *bisp_dim + iy_b];
*/
/* Normalization: (no longer used now, JLP95)*/
/* Commented out:
        if(w1 > 0) 
         {
         w1 = sqrt(w1);
         if(w1 < 1.e-10)w1=1.e-10;
         bispp[ng + iy_b] /= w1;
         bispp[ng + *bisp_dim + iy_b] /= w1;
         }
*/
/* End of case: modsq > 0 */
    }

/* End of loop on ng: */
  }

/*******************************************************/
/* Then correcting the squared modulus: */
/* (Normalization to obtain unity in the center (zero frequency is at 0,y here)) */
     for( i = 0; i < *nx; i++) 
      {
/* Photon noise correction: (biased_sq = xphot + xphot_sq * unbiased_sq)*/
      modsq[i + iy_s] -= xphot;
      }
/* End of loop on iy */
 }

return(0);
}
/***************************************************************
*
* Compute bispectrum slice:
* (Called by bisp_to_image.c)
*
* bisp(u,v) = bisp((u_1,u_2),(v_1,v_2))
* bisp(u,v) = spec(u) * spec(v) * spec(-u-v)
* bisp(u,v) = spec(u_1,u_2) * spec(v_1,v_2) * spec(-u_1-v_1,-u_2-v_2)
* Here we take u_2 = v_2 = 0
* bisp(u,v) = spec(u_1,0) * spec(v_1,0) * spec(-u_1-v_1,0)
* Hence we display the image bisp(u_1,0,v_1,0),
* which is a function of the two coordinates u_1 and v_1
*
* ShouldNormalize: flag set to 1 if modsq and bisp have to be normalized
* iopt: option set to 0 if real part of bispectrum has to be output,
*                     1 if imaginary part ...
*                     2 if SNR ...
*                     3 if phase
*                     4 if modulus i.e., sqrt(real^2 + imag^2)
****************************************************************/
int bisp2D_to_2D_image(double* out_image, double* modsq,
                       int nx, int ny, double* bisp, int nx_bisp,
                       int nbeta, int ngamma, int ShouldNormalize, int iopt,
                       char *err_message)
/* WARNING: Single precision for modsq and bisp arrays!!! */
{
double cos1, sin1, modulus;
double work, corr_factor, angle1;
register int i, ng, ix, iy;
int icent, kk, ll, mm, ix_cent, iy_cent;

/* If you want to check that all pixels are scanned in this routine:
replace 0 by any arbitrary number:  */
/* JLP2008: some pixels are not set because the image has even pixels
* and symmetry cannot reach them... */
for(i = 0; i < nx*ny; i++) out_image[i] = 0.;

  if(nx != ny){
  fprintf(stderr,
  "bisp2D_to_2D_image/Fatal error, the image should be square: nx=%d ny=%d!\n",
           nx, ny);
  return(-1);
  }

ix_cent = nx / 2;
iy_cent = ny / 2;
/* Coordinate of the center: */
icent = nx/2 + nx * ny/2;

/* Fill main array:
* iopt = 3, i.e., phase
* iopt = 4, i.e., modulus
*/
if(iopt == 3 || iopt == 4)
 {
  for (ng = 0; ng < ngamma; ng++)
    {
/* Spectral indices k, l, m, of the three components of bispectral term:
*/
     cover_klm0(&kk,1,ng);
     cover_klm0(&ll,2,ng);
     cover_klm0(&mm,3,ng);
/* Load Bisp[ u(kk,0), v(ll, 0) ]: */
     if(ixy_2[kk] == 0 && ixy_2[ll] == 0)
       {
         ix = ixy_1[kk];
         iy = ixy_1[ll];
         modulus = SQUARE(bisp[ng]) + SQUARE(bisp[ng+ngamma]);
         if(modulus == 0.)
            {
            angle1 = 0.;
            }
         else
            {
            modulus = sqrt(modulus);
            cos1 = bisp[ng] / modulus;
            sin1 = bisp[ng + ngamma] / modulus;
            angle1 = acos(cos1);
            if(sin1 < 0.) angle1 = -angle1;
            }
         if(iopt == 3)
            out_image[ix + icent + nx * iy] = angle1;
         else
            out_image[ix + icent + nx * iy] = modulus;
       }
    }
 }
/* Fill main array, general case: iopt=0 (real), 1(imag), 2(snr) */
else
 {
  for (ng = 0; ng < ngamma; ng++)
    {
/* Spectral indices k, l, m, of the three components of bispectral term:
*/
     cover_klm0(&kk,1,ng);
     cover_klm0(&ll,2,ng);
     cover_klm0(&mm,3,ng);
     if(ixy_2[kk] == 0 && ixy_2[ll] == 0)
       {
         ix = ixy_1[kk];
         iy = ixy_1[ll];
/* Load Bisp[ u(kk,0), v(0,ll) ]: */
         out_image[ix + icent + nx * iy] =
                              bisp[ng + ngamma*iopt];
       }
    }
/* end of iopt != 3 */
 }

/* Fill central line with
* modsq, for since bisp(u,0)=modsq(u) for iopt=0 (real part), iopt=2 (snr)
* or for iopt=4 (real^2 + imag^2)
* zero for iopt=1, (imaginary part) or iopt=3 (phase) */
  if(iopt == 0 || iopt == 2 || iopt == 4)
   {

/* X and Y axes correspond to the normalized power spectrum:
* (They are equal since B(u,v)=B(v,u) ...)
* X axis, i.e., v_2=0:
* bisp(u,v) = spec(u_1,0) * spec(0,0) * spec(-u_1,0) = |spec(u_1,0)|**2
* Y axis, i.e., u_2=0:
* bisp(u,v) = spec(0,0) * spec(v_1,0) * spec(-v_1,0) = |spec(v_1,0)|**2
*/

/* Upper part of Y axis:  (at the centre of out_image) */
/* JLP2008: I multiply with the value of the spectrum for zero frequency: */
  corr_factor = sqrt(modsq[icent]);
  for(ix = ix_cent; ix < nx; ix++)
    out_image[ix_cent + nx * ix] =
                     corr_factor * modsq[ix + nx * iy_cent];

  }
/* iopt = 1 (imaginary part) or iopt = 3 (phase) */
  else
  {
  for(ix = ix_cent; ix < nx; ix++) out_image[ix_cent + nx * ix] = 0.;
  }

/* Now the upper left triangle in the upper right quadrant is filled */
/* Fill remaining half (lower right triangle) of the upper right quadrant with
*  axial symmetry: bisp(u,v)=bisp(v,u) */
  for(ix = 0; ix < ix_cent; ix++)
    for(iy = 0; iy < ix; iy++)
       {
       out_image[ix + ix_cent + (iy + iy_cent) * nx] =
                  out_image[iy + ix_cent + (ix + iy_cent) * nx];
       }

/* Fill the lower left quadrant with central symmetry:
* bisp(u,v)=conjugate of bisp(-u,-v) (since image values are real values)*/
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < ix_cent; ix++)
       {
/* iopt=1 imag, iopt=3 phase: */
       if(iopt == 1 || iopt == 3)
       out_image[-ix + ix_cent + (-iy + iy_cent) * nx] =
                  -out_image[ix + ix_cent + (iy + iy_cent) * nx];
/* iopt=0 real, iopt=2 snrm, iopt=4 modulus: */
       else
       out_image[-ix + ix_cent + (-iy + iy_cent) * nx] =
                  out_image[ix + ix_cent + (iy + iy_cent) * nx];
       }

/* Fill half of the lower right quadrant with "special" symmetry:
* bisp(u,v)=s(u)s(v)s(-u-v)= s(u)s(-u-v)s(-u-(-u-v))=bisp(u,-u-v) */
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < ix_cent - iy; ix++)
       {
       out_image[ix + ix_cent + (-ix-iy + iy_cent) * nx] =
                  out_image[ix + ix_cent + (iy + iy_cent) * nx];
       }

/* Again, use bisp(-u,v)=conjugate of bisp(u,-v) (since image values
* are real values)
* to fill half of the upper left quadrant with central symmetry: */
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < iy; ix++)
       {
/* iopt=1 imag, iopt=3 phase: */
       if(iopt == 1 || iopt == 3)
       out_image[-ix + ix_cent + (iy + iy_cent) * nx] =
                  -out_image[ix + ix_cent + (-iy + iy_cent) * nx];
/* iopt=0 real, iopt=2 snrm, iopt=4 modulus: */
       else
       out_image[-ix + ix_cent + (iy + iy_cent) * nx] =
                  out_image[ix + ix_cent + (-iy + iy_cent) * nx];
       }

/* Fill remaining half of the lower right quadrant with axial symmetry:
* bisp(v,-u)=bisp(-u,v) */
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < iy; ix++)
       {
       out_image[iy + ix_cent + (-ix + iy_cent) * nx] =
                  out_image[-ix + ix_cent + (iy + iy_cent) * nx];
       }

/* Fill remaining half of the upper left quadrant with central symmetry:
* bisp(-v,u)=bisp(u,-v) */
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = iy; ix < ix_cent; ix++)
       {
       if(iopt == 1 || iopt == 3)
       out_image[-ix + ix_cent + (iy + iy_cent) * nx] =
                  -out_image[iy + ix_cent + (-ix + iy_cent) * nx];
       else
       out_image[-ix + ix_cent + (iy + iy_cent) * nx] =
                  out_image[iy + ix_cent + (-ix + iy_cent) * nx];
       }

/* iopt=0 real, iopt=2 snrm, iopt=4 modulus: */
work = out_image[icent];
if(ShouldNormalize && (iopt == 0 || iopt == 2 || iopt == 4) && work != 0.){
      sprintf(err_message,
              "Bisp_2D_to_2D_image/Normalizing output image with scale=%f\n",
              work);
       for(i = 0; i < nx*ny; i++) out_image[i] /= work;
      }

return(0);
}
/***********************************************************************
* Display bispectra for 1D-spectra (slit spectra)
* for a given line (line_to_output)
*
* iopt: option set to 0 if real part of bispectrum has to be output,
*                     1 if imaginary part ...
*                     2 if SNR ...
*                     3 if phase
***********************************************************************/
int bisp1D_to_2D_image(double* out_image, int* nx_out, int* ny_out,
                       double* modsq, int* nx_modsq, double* bisp, int* nx_bisp,
                       int* nbeta, int* ngamma, int* line_to_output,int* iopt)
// Single precision!!!: double *bisp, *modsq;
{
double corr_factor;
int ix, iy, iy_b, ix_cent, iy_cent, iix, k1, k2, ng;

/* JLP96: */
 if(*iopt == 3) {printf("bisp1D_to_2Dimage/error, option not yet available\n");
                return(-1);
               }

ix_cent = (*nx_out)/2;
iy_cent = (*ny_out)/2;
iy_b = (*line_to_output) * (*nx_bisp);

/* Display bispectrum of (u_1,0,v_1,0): */
  for(ng = 0; ng < *ngamma; ng++)
    {
/* Get nb spectral index (k1) for the first vector 
* of ng index bispectral list: */
     cover_klm0(&k1,1,ng);
/* Get coordinates (iix,0) from nb index: */
     COVER_IXY_1D(&iix,&k1);
     ix = ((iix + *nx_modsq) % *nx_modsq); 
/* Same for second vector : */
     cover_klm0(&k2,2,ng);
     COVER_IXY_1D(&iix,&k2);
     iy = ((iix + *nx_modsq) % *nx_modsq); 
/* I add (ix_cent,iy_cent) to get (0,0) bifrequency in the center: */
     out_image[ix + ix_cent + (iy + iy_cent) * (*nx_out)] = 
                  bisp[ng + (*iopt) * (*ngamma) + iy_b];
    }

/* Fill central line with modsq, since bisp(u,0)=modsq(u): 
* (except when iopt = 1, i.e., imaginary part is wanted) */
  if(*iopt != 1)
   {
/* For some unknown reason, modsq should be multiplied by 2 (symmetry ???): 
   and snrm by 4*/
   if(*iopt == 0)
     corr_factor = 2.;
   else
     corr_factor = 4.;
   for(iy = 0; iy < iy_cent; iy++)
     out_image[ix_cent + (iy + iy_cent) * (*nx_out)] = 
      corr_factor * modsq[(*nx_modsq)/2 + iy + (*line_to_output) * (*nx_modsq)];
   }
/* iopt = 1: imaginary part is wanted */
  else
   {
   for(iy = 0; iy < iy_cent; iy++) 
       out_image[ix_cent + (iy + iy_cent) * (*nx_out)] = 0.;
   }

/* Fill remaining half of the upper right quadrant with
*  axial symmetry: bisp(u,v)=bisp(v,u) */
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < iy; ix++)
       {
       out_image[iy + ix_cent + (ix + iy_cent) * (*nx_out)] = 
                  out_image[ix + ix_cent + (iy + iy_cent) * (*nx_out)];
       }

/* Fill the lower left quadrant with central symmetry: 
* bisp(u,v)=conjugate of bisp(-u,-v) (since image values are real values)*/
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < ix_cent; ix++)
       {
       if(*iopt == 1)
       out_image[-ix + ix_cent + (-iy + iy_cent) * (*nx_out)] = 
                  -out_image[ix + ix_cent + (iy + iy_cent) * (*nx_out)];
       else
       out_image[-ix + ix_cent + (-iy + iy_cent) * (*nx_out)] =
                  out_image[ix + ix_cent + (iy + iy_cent) * (*nx_out)];
       }

/* Fill half of the lower right quadrant with "special" symmetry: 
* bisp(u,v)=s(u)s(v)s(-u-v)= s(u)s(-u-v)s(-u-(-u-v))=bisp(u,-u-v) */
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < ix_cent - iy; ix++)
       {
       out_image[ix + ix_cent + (-ix-iy + iy_cent) * (*nx_out)] = 
                  out_image[ix + ix_cent + (iy + iy_cent) * (*nx_out)];
       }

/* Fill half of the upper left quadrant with central symmetry:
* bisp(-u,v)=conjugate of bisp(u,-v) (since image values are real values)*/
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < iy; ix++)
       {
       if(*iopt == 1)
       out_image[-ix + ix_cent + (iy + iy_cent) * (*nx_out)] = 
                  -out_image[ix + ix_cent + (-iy + iy_cent) * (*nx_out)];
       else
       out_image[-ix + ix_cent + (iy + iy_cent) * (*nx_out)] = 
                  out_image[ix + ix_cent + (-iy + iy_cent) * (*nx_out)];
       }

/* Fill remaining half of the lower right quadrant with axial symmetry: 
* bisp(v,-u)=bisp(-u,v) */
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = 0; ix < iy; ix++)
       {
       out_image[iy + ix_cent + (-ix + iy_cent) * (*nx_out)] = 
                  out_image[-ix + ix_cent + (iy + iy_cent) * (*nx_out)];
       }

/* Fill remaining half of the upper left quadrant with central symmetry:
* bisp(-v,u)=bisp(u,-v) */
  for(iy = 0; iy < iy_cent; iy++)
    for(ix = iy; ix < ix_cent; ix++)
       {
       if(*iopt == 1)
       out_image[-ix + ix_cent + (iy + iy_cent) * (*nx_out)] = 
                  -out_image[iy + ix_cent + (-ix + iy_cent) * (*nx_out)];
       else
       out_image[-ix + ix_cent + (iy + iy_cent) * (*nx_out)] =
                  out_image[iy + ix_cent + (-ix + iy_cent) * (*nx_out)];
       }

return(0);
}
/***************************************************************
*
* Interface to Fortran programs
* Output internal static arrays
*
****************************************************************/

/********************************************
* Fortran array NGT(NBMAX)
* Index: 1 to NBMAX (but c compatible on Feb 2nd 1994)
********************************************/
int COVER_NGT(int* ngt_val, int* index)
{
*ngt_val = ngt[*index];
return(0);
}
/********************************************
* Fortran array IXY(2,NULL:NBMAX)
* Second index: like c programs
********************************************/
int COVER_IXY(int* ixy1_val, int* ixy2_val, int* index)
{
*ixy1_val = ixy_1[*index];
*ixy2_val = ixy_2[*index];
return(0);
}
/********************************************
* Fortran array IXY(NULL:NBMAX)
* Second index: like c programs
********************************************/
int COVER_IXY_1D(int* ixy1_val, int* index)
{
*ixy1_val = ixy_1[*index];
/* JLP98: I set it to "identity", since array overflood somewhere...: */
/*  *ixy1_val = *index; */
/* end of JLP98 */
return(0);
}
/********************************************
* Fortran array NBCOUV(-IRMAX:IRMAX,NULL:IRMAX)
* Indices: (like c programs)
********************************************/
int COVER_NBCOUV(int* nb_val, int* index1, int* index2, int* ir_max)
{
int nbc_dim, nbc_offset;

nbc_dim = 2 * (*ir_max) + 1;
nbc_offset = (*ir_max) + 1;

*nb_val = nbcouv[(*index1) + nbc_offset + nbc_dim * (*index2)];
return(0);
}
/*******************************************
* Fortran array KLM(3,NGMAX)
* Input indices: 1 to 3 and 1 to NGMAX
* Output index: klm_val from 0 to NBMAX
*******************************************/
int COVER_KLM(int* klm_val, int* klm_index, int* ng_index)
{

*klm_val = klm[(*ng_index - 1) * 3 + (*klm_index - 1)];

return(0);
}
/********************************************************************
* Subroutine COVERA_MASK_1D
* To compute the frequencies of the uv coverage,
* and the elements of the A matrix.
* (To solve eventually the equation A*X=Y, 
*   i.e., to invert the bispectral relations)
*
* Compute u-v coverage with the mask and within a segment of length 2*ir.
*
* Input:
*  ir: maximum radius of the uv-coverage
*  max_nclosure: maximum number of closure relations
*  mask[nx]: frequency mask ( >=1. if frequency accessible, 0. otherwise)
*
* Output:
*  NBETA: Number of elements of the spectral list (Number of columns of A)
*  NGAMMA: Number of elements of the bispectral list (Number of rows of A)
*
*********************************************************************/
int COVERA_MASK_1D(double* mask, int* nx_mask, int* ir, int* max_nclosure,
                   int* nbeta, int* ngamma, char *err_message)
{
/* max_nclosure: max number of closure relations per spectral term */
/* Limitation of the maximum number of closure relations
* (to simulate Knox-Thompson and/or accelerate computations...) */
int isize, nxm, ir_max;
register int i;

/* JLP 94
 ir_max = IRMAX;
*/
ir_max = *ir;
if(ir_max > IRMAX)
   {sprintf(err_message,"Covera_mask_1D/Fatal error: ir max = %d",(int)IRMAX);
    return(-1);
   }

nxm = *nx_mask;
/* Check first that mask is correct: */
if(nxm < 2 * (*ir) )
  {
   sprintf(err_message,"Covera_mask_1D/Fatal error: mask is too small!\
 nxm = %d ir = %d \n",nxm,*ir);
   return(-1);
  }

/* nbcouv( x from 0 to IRMAX ) */
/* JLP94 */
isize = (ir_max + 1);
nbcouv = new int[isize];
/* ixy_1 for x;  nb from 0 to NBMAX ) */
isize = (NBMAX + 1);
ixy_1 = new int[isize];
ngt = new int[isize];
/*JLP 94 */
for(i = 0; i < (ir_max + 1); i++) nbcouv[i] = 0;

/* Computing the A matrix as generated by the uv-coverage
   (defined only by IR for a full pupil) */

/* First call to compute bispectrum size NGMAX = ngamma: */
couv_mask_1D(mask,nxm,ir,nbeta,ngamma,*max_nclosure,1);

/* Allocation of memory for bispectrum index: */
isize = 3 * (*ngamma);
klm = new int[isize];

/* Second call to compute spectral and bispectral lists: */
couv_mask_1D(mask,nxm,ir,nbeta,ngamma,*max_nclosure,0);

#ifdef DEBUG
sprintf(err_message," IR=%d NBETA (spectral list)=%d NGAMMA (bispec. list)=%d",
        *ir, *nbeta, *ngamma);
#endif

/* Computing the number of closure relations for some values of NB: */
/*
{int k, iw1;
for ( k = 2; k < 5 ; k ++) {
   i = 3 + *nbeta * k / 5;
   iw1 = sqrt((double)(ixy_1[i] * ixy_1[i]));
   printf(" NB = %d irad=%d Closure relations: %d \n",
             i,iw1,ngt[i] - ngt[i - 1]);
  }
}
*/
 
/* Free memory: */
/* JLP96: don't do that, since nbcouv is not accessible otherwise... */
/*
delete[] nbcouv;
*/

return(0);
}
/*******************************************************************
* couv_mask_1D defines the uv-coverage and the A matrix:
* Input:
* mask[nx]: frequency mask ( >=1. if frequency accessible, 0. otherwise) 
* IR: maximum radius of the uv-coverage
* dimension_only: flag set to one if only bispectral list dimension is required
*
* Output:
* NBETA, NGAMMA: number of elements of the spectral and bispectral lists
*
* In common blocks (output): the uv-coverage is accessible from 2 sides:
*
* NBCOUV(I,J): uv-coverage (i.e. number of the spectral list for
*              the pixel(I,J) of the spectrum (I=0,J=0 for null frequency)
*
* IXY(1,I) X-coordinate of the pixel number I in the spectral list
*              (this allows another entry for the uv-coverage)
*
*************************************************************************/
static int couv_mask_1D(double* mask, int nxm, int* ir, int* nbeta,
                     int* ngamma, int max_nclosure, int dimension_only)
{
/*
* nb: beta index, i.e. spectral list index
* ng: gamma index, i.e. bispectral list index
*/
int nb, ng, icent;
register int i;
 
/* Coordinate of center of the mask: */
icent = nxm/2; 
 
/* Easy cases: */
/* First spectral value at (0) */
 nb = 0;
 i = 0; 
 if(mask[i + icent] > 0.)
   {
   ixy_1[nb] = i; 
   nbcouv[i] = nb;
   }
 
/* Second spectral value at (1), nb=1 */
 i = 1;
 if(mask[i + icent] > 0.)
   {
   nb++;
   ixy_1[nb] = i;
   nbcouv[i] = nb;
   }

/* Reseting the total number of elements the  bispectral list */
 ng = 0;
 
/* Main loop:
*/
for ( i = 2; i <= *ir; i++)
  {
    affect_mask_1D(mask,nxm,i,&nb,&ng,max_nclosure,dimension_only);
  }

/* NBETA: Total number of the spectral list (Number of columns of the X matrix)
* NGAMMA: Total number of the bispectral list (Number of rows of the X matrix)
* (Remember, we have to solve    A*X = Y) */
  *nbeta = nb;
  *ngamma = ng;

  return(0);
}
/*******************************************************************
* affect_mask_1D:
* Gives a structure to the S group of the A matrix
* Input:
* ISX, ISY : coordinates
* IRS: radius
*
* Output:
* NB: index in the spectral list
* NG: index in the bispectral list
********************************************************************/
static int affect_mask_1D(double* mask, int nxm, int isx, int* nb, int* ng,
                       int max_nclosure, int dimension_only)
{ 
int nbs, nbk, nbr, itx, icent;
register int nbq, iklm;

/* Coordinate of center of the mask: */
icent = nxm/2;
 
/* First condition: input point has to be accessible */
  if(mask[isx + icent] <= 0.) return(-1);

/* If so, record the new point of the uv coverage (spectral list): */
 (*nb)++;
 ixy_1[*nb] = isx;
 nbcouv[isx] = *nb;

/* Searching for the couples associated with the point NBS=NB
* Generating the bispectral list and building the rows of the A matrix:
*/
 nbs = *nb;
 
/* Loop on all the possible points (Q): */
 iklm = 3 * (*ng);
 for( nbq = 1; nbq < nbs; nbq++)
  { 
/* JLP 94: add an exit test when maximum number of closure relations
has been found (to simulate Knox-Thompson and/or accelerate computations...) */
   if((*ng - ngt[(*nb) -1]) == max_nclosure) break;

/* Coordinates of the vector T = S - Q */
   itx = isx - ixy_1[nbq];

/* Work within the segment of length IRS, so we can a priori reject
* the points outside the window [-IRS,+IRS]:
*/
   if(itx >= -isx && itx <= isx)
      {
/* Case number 1 only for 1D case (which could be : k=t, l=q, k+l=s) */

/* t should be accessible too: */
            if(mask[itx + icent] > 0.)
            {
              nbk = nbcouv[itx];

/* We select this couple (U,V) if the vector NBK is in [0,NBQ] */
              if( nbk != 0 && nbk <= nbq)
                 {
/* Add new k,l,m coordinates if more than dimension is wanted */
                 if(!dimension_only)
                   {
                   klm[iklm] = nbk; 
                   iklm++;
                   klm[iklm] = nbq; 
                   iklm++;
                   klm[iklm] = nbs;
                   iklm++;
                   }
                 (*ng)++;
                 }
          }
      else
          { 
/* Case number 2 (which could be : r=-t, s=s, r+s=m=q) */
/* r should be accessible too: */
            if(mask[-itx + icent] > 0.)
            {
             nbr = nbcouv[-itx];
/* We select this couple (R,S) if the vector NBR is in [0,NBS] */
              if( nbr != 0 && nbr <= nbs)
                  {
/* Add new k,l,m coordinates if more than dimension is wanted */
                  if(!dimension_only)
                     {
                     klm[iklm] = nbr; 
                     iklm++;
                     klm[iklm] = nbs; 
                     iklm++;
                     klm[iklm] = nbq;
                     iklm++;
                     }
                  (*ng)++;
                  }
            }
/* Nota: we can only have L=NBS or M=NBS (never K=NBS) */
          }
      } 

/* End of loop on nbq */
}

/* NGT(NB) is the number of the last U,V couple of the group NB=NBS=S: */
   ngt[nbs] = *ng;

 return(0);
}
/***************************************************************
* Interface with COVER_KLM
* Designed for C-like arrays
* (COVER_KLM was designed for Fortran arrays)
*
****************************************************************/
int cover_klm0(int* k, int kk, int ng)
{
int kkk, nng;
kkk = kk;
nng = ng + 1;
COVER_KLM(k,&kkk,&nng);
return(0);
}
/***************************************************************
* Interface with COVER_KLM
* Designed for Fortran-like arrays
* (COVER_KLM was designed for Fortran arrays)
*
****************************************************************/
int cover_klm1(int* k, int kk, int ng)
{
int kkk, nng;
kkk = kk;
nng = ng;
COVER_KLM(k,&kkk,&nng);
return(0);
}
/***************************************************************
* Interface with COVER_NGT
* (COVER_NGT was designed for Fortran arrays)
*
****************************************************************/
int cover_ngt1(int* ng2, int nb)
{
int nbb;
nbb = nb;
COVER_NGT(ng2,&nbb);
return(0);
}
/****************************************************************
*  compute_uv_coverage
*****************************************************************/
int compute_uv_coverage(char* fmask_name, int ir, int* nbeta,
                        int* ngamma, int max_nclosure, char *err_message)
{
double *fmask;
int isize, nx_fmask, ny_fmask, status;
register int i;

  if(fmask_name[0] == '\0')
    {
/* Old call: (either in jlp_cover1.for or in jlp_cover2.c)
    COVERA(&ir,nbeta,ngamma);
*/
/* Full u-v coverage: */
//    printf(" Full u-v coverage.\n");
    nx_fmask = 2 * ir + 1; ny_fmask = nx_fmask;
    isize = nx_fmask * ny_fmask;
    fmask = new double[isize];
    for(i = 0; i < nx_fmask * ny_fmask; i++) fmask[i] = 1.;
    }
  else
/* Read Fourier mask file: */
    {
/*
    JLP_VM_READIMAG(&pntr_ima,&nx_fmask,&ny_fmask,fmask_name,fmask_comments);
    JLP_FROM_MADRID(&pntr_ima,&fmask);
    if( nx_fmask < (2 * ir + 1) || ny_fmask < (2 * ir + 1))
      {
      printf(" Fatal error/ Input mask is too small relative to ir \n");
      printf("     (ir = %d, nx_fmask = %d, ny_fmask = %d)\n",
              ir, nx_fmask, ny_fmask);
      return(-1);
      }
*/
    }

/* "Masked" u-v coverage: */
    status = COVERA_MASK(fmask,&nx_fmask,&ny_fmask,&ir,&max_nclosure,nbeta,ngamma,
                         err_message);
/* Free virtual memory space: */
    delete[] fmask;


return(status);
}
/****************************************************************
*  compute_uv_coverage_1D
*****************************************************************/
int compute_uv_coverage_1D(char* fmask_name, int ir, int* nbeta,
                           int* ngamma, int max_nclosure, char *err_message)
{
double *fmask;
int status, nx_fmask;
register int i;

  if(fmask_name[0] == '\0')
    {
/* Full u-v coverage: */
/*
    printf(" Full u-v coverage.\n");
*/
    nx_fmask = 2 * ir + 1;
    fmask = new double[nx_fmask];
    for(i = 0; i < nx_fmask; i++) fmask[i] = 1.;
    }
  else
/* Read Fourier mask file: */
    {
/*
    int ny_fmask;
    JLP_VM_READIMAG(&pntr_ima,&nx_fmask,&ny_fmask,fmask_name,fmask_comments);
    JLP_FROM_MADRID(&pntr_ima,&fmask);
    if( nx_fmask < (2 * ir + 1) || ny_fmask != 1)
      {
      printf(" Fatal error/ Input mask is too small relative to ir \n");
      printf("  or ny_mask != 1:   (ir = %d, nx_fmask = %d, ny_fmask = %d)\n",
              ir, nx_fmask, ny_fmask);
      return(-1);
      }
*/
    }

/* "Masked" u-v coverage: */
    status = COVERA_MASK_1D(fmask,&nx_fmask,&ir,&max_nclosure,nbeta,
                            ngamma,err_message);
/* Free virtual memory space: */
    delete[] fmask;

return(status);
}
/*******************************************************************
* prepare_output_bisp
* To compute the mean of input arrays and SNR values
********************************************************************/
int prepare_output_bisp(double* bispp, double* modsq, double* snrm,
                        int nx, int ny, double xframes,
                        int nbeta, int ngamma)
{
register int i;
int bisp_dim;
double w1, w2, w3;

   bisp_dim=ngamma;

if(xframes <= 0.) {
 fprintf(stderr,"prepare_output_bisp/Error: xframes=%d\n", (int)xframes);
 return(-1);
 }

/* Compute the mean: */
  for(i = 0; i < nx * ny; i++) {
     modsq[i] /= xframes;
     snrm[i] /= xframes;
        }
  for(i=0; i< 4 * ngamma; i++) bispp[i] /= xframes;
/****************************************************************/
/* SNR of modsq. First step: sigma**2 computation
* (Keep sigma**2, for consistency with normalisation in photon_corr)
*  AFTER MEAN COMPUTATION */
   for(i = 0; i < nx * ny; i++) {
        snrm[i] = snrm[i] - SQUARE(modsq[i]);
        }

// New arrangement of bispp from bispectral list to 2D array*/
   rearrange_mask(bispp, ngamma);

/****************************************************************/
// SNR of modsq. Second step: modsq[i]/sqrt(variance)
   for(i = 0; i < nx * ny; i++)
      {
      if(snrm[i] <= 1.e-4) snrm[i] = 1.e-4;
      snrm[i] = 1. / sqrt(snrm[i]);
      }

/***************** Bispectrum: **********************************/
   for(i = 0; i < ngamma; i++) {
/* First computing the variance of real and imaginary parts: */
        w1 =  bispp[2*bisp_dim + i] - SQUARE(bispp[i]);
        w2 =  bispp[3*bisp_dim + i] - SQUARE(bispp[bisp_dim + i]);
/* Then the sigma (sum of real and imag variances): */
        w1 = w1 + w2;
        if(w1 < 1.e-10) w1 = 1.e-10;
        w1 = sqrt(w1);
/* Phase factor of the bispectrum */
        w3 = SQUARE(bispp[i]) + SQUARE(bispp[bisp_dim + i]);
        w3 = sqrt(w3);
/* SNR of bispectrum in 3rd line: */
        bispp[2*bisp_dim + i] = w3/w1;
        }

return(0);
}
/*******************************************************************
* prepare_output_bisp_1D
* double precision version
* No photon correction
********************************************************************/
int prepare_output_bisp_1D(double* bispp, double* modsq, double* snrm,
                           int nx, int ny, double xframes,
                           int nbeta, int ngamma)
{
register int i, ix;
int bisp_dim, ix_b;
double w1, w2, w3;

/* Compute the mean: */
  for(i = 0; i < nx * ny; i++) {
     modsq[i] /= xframes;
     snrm[i] /= xframes;
        }
  for(i=0; i< 4 * ngamma * nx; i++) bispp[i] /= xframes;
/****************************************************************/
/* SNR of modsq. First step: sigma**2 computation
* (Keep sigma**2, for consistency with normalisation in photon_corr)
*/
   for(i = 0; i < nx * ny; i++) {
        snrm[i] = snrm[i] - modsq[i]*modsq[i];
        }

/****************************************************************/
/* SNR of modsq. Second step: modsq[i]/sqrt(variance) */
   for(i = 0; i < nx * ny; i++)
      {
      if(snrm[i] <= 1.e-4) snrm[i] = 1.e-4;
      snrm[i] = 1. / sqrt(snrm[i]);
      }

bisp_dim = ngamma;
/***************** Bispectrum: **********************************/
for(ix = 0; ix < nx; ix++)
   {
   ix_b = ix * ngamma * 4;
   for(i=0; i<ngamma; i++)
       {
/* First computing the variance: */
        w1 =  bispp[2*bisp_dim + i + ix_b]
               - bispp[i + ix_b]*bispp[i + ix_b];
        w2 =  bispp[3*bisp_dim + i + ix_b]
                - bispp[bisp_dim + i + ix_b]*bispp[bisp_dim + i + ix_b];
/* Then the sigma=sqrt(real**2 + imag**2): */
        w1 = w1 + w2;
        if(w1 < 1.e-10) w1 = 1.e-10;
        w1 = sqrt(w1);
/* Modulus of the bispectrum */
        w3 = bispp[i + ix_b]*bispp[i + ix_b] 
              + bispp[bisp_dim + i + ix_b]*bispp[bisp_dim + i + ix_b];
        w3 = sqrt(w3);
/* Normalization of real and imaginary parts (not done any longer)
        bispp[i + ix_b]=bispp[i + ix_b]/w3;
        bispp[bisp_dim + i + ix_b] = bispp[bisp_dim + i + ix_b]/w3;
*/
/* SNR of bispectrum in 3rd line: */
        bispp[2*bisp_dim + i + ix_b] = w3/w1;
        }
     }

return(0);
}
/***********************************************************
* output_bisp      
* Prepare output of bispectrum
*
* Contraction from 4 to 3 parameters
***********************************************************/
int output_bisp(double* bispp, double* bisp1, int ngamma)
{
register int ng, k;

   for(ng = 0; ng < ngamma; ng++)
      {
      for(k = 0; k < 3; k++)
        {
        bisp1[ng + k*ngamma] = bispp[ng + k*ngamma];
        }
      }
return(0);
}
/***********************************************************
* output_bisp_1D
* Prepare output of bispectrum
*
* Contraction from 4 to 3 parameters
***********************************************************/
int output_bisp_1D(double* bispp, double* bisp1, int ngamma, int nx)
{
register int ix, ng, k;
int bdim, bdim1;

bdim = ngamma * 4;
bdim1 = ngamma * 3;

for(ix = 0; ix < nx; ix++)
   {
   for(ng = 0; ng < ngamma; ng++)
      {
      for(k = 0; k < 3; k++)
        {
        bisp1[ng + k*ngamma + ix*bdim1] = bispp[ng + k*ngamma + ix*bdim];
        }
      }
   }
return(0);
}
