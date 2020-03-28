/****************************************************************
* inv_bispec2.c
* To compute the phase of the spectrum from the bispectrum,
* assuming a full pupil.
*
* Contains CREYCE_OBSERV, TRANS, RECURSIVE, EPLUS,
*          NOYAU, PROJ_EPLUS,
*          BISP_WEIGHT,BISP_WEIGHT2
*
* Syntax:
*
* Example1:
* RUNS INV_BISPEC 0 12,20,0.08,220,0.004,0.8 out_name real_fft ima_fft
*
*   with 0=simulation option
*   12=radius of uv cover., 20 = weights or phase error,
*   220=nbeta, 0.004=low_modulus, 0.8= max sigma
*   real_fft and ima_fft= FFT of image to be reconstructed
*
* Example2:
* RUNS INV_BISPEC 1 12,20,0.08,220,0.004,0.8 out_name modsq bisp1
*
*   with 1=real data,
*   12=radius of uv cover., 20 = weights or phase error,
*   220=nbeta, 0.004=low_modulus, 0.8 = max sigma
*   modsq=mean squared modulus of FFT
*   bisp1=mean bispectrum
*
* Example3:
* RUNS INV_BISPEC 2 12,20,0.08,220,0.004,0.8 out_name modsq real_fft,ima_fft bisp1
*
*   with 2=simulated data,
*   12=radius of uv cover., 20 = weights or phase error,
*   220=nbeta, 0.004=low_modulus, 0.8= max sigma
*   modsq=mean squared modulus of FFT (simulated)
*   real_fft and ima_fft= FFT of image to be reconstructed
*   bisp1=mean bispectrum (simulated)
*
* INPUT files (in the case of real data, i.e., observations):
*  modsq: square modulus
*  bisp: bispectrum
*
* INPUT files (in the case of simulations):
*  modsq: square modulus
*  bisp: bispectrum
*  real_fft: real part of the spectrum of the object
*  ima_fft: imaginary part of the spectrum of the object
*
* OUPUT files:
*  out_name.SNR
*  out_name.SIG
*
* JLP
* Version: 10/01/2015
* Previous version: 25-01-00
*          (with comments added in April 2008)
* From A. Lannes P.FOR (January 1988)
* From inv_bispec1.for (February 1994)
*****************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "asp2_covermas.h"
#include "asp2_photon_corr.h"  /* Where the prototype of inv_bispec2 is defined
                          (to check compatibility with this file) */

/* defined in jlp_numeric.h now:
#define ABS(a) ((a) < 0) ? (-(a)) : (a)
#define SQUARE(a) ((a)*(a))
#define MINI(a,b) ((a) < (b)) ? (a) : (b)
#define MAXI(a,b) ((a) < (b)) ? (b) : (a)
*/

#ifndef PI
#define PI 3.141592654
#endif
/* Don't forget the parenthesis... */
#define DEGRAD (PI/180.)
/* Minimum value of sigma (for the central frequencies): */
/* This implies that SNR max = 1 / MIN_SIGMA */
#define MIN_SIGMA 1.E-2

#define DEBUG

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

/* For ir=30 :
#define IRMAX 30
#define MIRMAX -IRMAX
#define NBMAX 1410
#define NGMAX 388400
#define IDIM 256
*/

/* For ir=50 :  (to be checked) (successfully tested with 50,100 on 21/01/00)*/
#define IRMAX 50
#define MIRMAX -IRMAX
#define NBMAX 3922
#define NGMAX 379368
#define IDIM 256

int CREYCE_OBSERV(double *yce_re, double *yce_im, double *xc_re, double *xc_im,
                  double *ro, int ir, double cte, int nbeta, int ngamma, int ngamma_max,
                  double lower_modulus, double *modsq1, double *bisp1,
                  double *weight, double *bmask, int nx1, int ny1);
int CREYCE_LOAD_BISP(double *yce_re, double *yce_im, double *bisp1,
                        int ngamma, int nx1);
int CREYCE_BISP_WEIGHTS(int ngamma, int ngamma_max, int ir,
                        double cte, double *bmask, double *ro, double *bisp1,
                        double *weight);
int RECURSIVE(double *sigm, int nbeta, int ngamma, double sigma_null,
              int ifermax, double *bmask, double *yce_re, double *yce_im,
              double *xc_re, double *xc_im, double *weight);
int BISP_WEIGHT11(int ngamma, int ir, double cte,
                  double *bmask, double *ro, double *weight);
int BISP_WEIGHT2(double *bisp1, int ngamma, int ngamma_max, double *weight);
int MODIF_WEIGHTS_SIGM2(double *sigm, int nbeta, int ngamma, double sig_max,
                        double lower_modulus, int ifermax, double *bmask,
                        double *weight);
int MODIF_WEIGHTS_SIGM1(double *sigm, int nbeta, int ngamma, double sig_max,
                        double lower_modulus, int ifermax,double *bmask,
                        double *weight);
int TRANS(int ir, int nbeta, double *bmask, double *xc_re, double *xc_im);
int COMPLEX_PROD2(double c1_re, double c1_im, double c2_re, double c2_im,
                  int sign1, int sign2, double *c12_re, double *c12_im);
int COMPLEX_PROD3(double c1_re, double c1_im, double c2_re, double c2_im,
                  double c3_re, double c3_im, int sign1, int sign2, int sign3,
                  double *c123_re, double *c123_im);
int ZERO_PHASE(double *xc_re, double *xc_im, int nbeta);
int NORMALIZE_L1(double *array, int npts, double *norm);
int NORMALIZE_L2(double *array, int npts, double *norm);
int SORTIE(double *re_out, double *im_out, int nx1, int ny1,
           double *xc_re, double *xc_im, double *ro, int nbeta, double *bmask);
int PROJ_EPLUS(int nbeta, int n1, int n2, double *xx,
               int idim_xx, double *vecxy);
int NOYAU(int nbeta, double *bmask, double *vecxy, int idim_xx);
int EPLUS(int nbeta, double *bmask, double *xc_re, double *xc_im,
          double *xx, int idim_xx, double *vecxy);
int jlp_atan2c(double *xxc, double cc_im, double cc_re);

static FILE *fp1;
/* xc: Spectrum (list): phase factor
* ro: Spectrum (list): modulus
* yce: Bispectrum (list)
* xcr: Reference spectrum (list), phase factor  (not known for real observ.)
   static double *mask, *xx, *vecxy;
   static double *weight, *re, *im, *ro;
   static int ifermax = 10000;
*/
/**************************************************************************
* INPUT:
* bisp1, modsq1
* nx1, ny1 : size of modsq1
* nbeta, ngamma, nclos_max
*
* OUTPUT:
* re_out, im_out: real and imaginary part of the restored image
***************************************************************************/
int inv_bispec2(double *re_out, double *im_out, double *bisp1, double *modsq1,
                int ir, int ngamma, int nbeta, int nclos_max, int nx1, int ny1)
{
 int status, ngamma_maxi, ifermax, idim_xx;
 double *xc_re, *xc_im, *yce_re, *yce_im, *vecxy, *xx, *weight;
 double *bmask, *sigm, *ro, sig_max = 0.8;
 double lower_modulus = 0., cte = 20., sigma_null = 1.0;
 char logfile_name[128];

// Assume that the dimension of bisp1 is (ngamma * 4):
 ngamma_maxi = ngamma;

/* ifermax: simply to give a limit to the number of closure relations to
* be taken into account (when this number would be really too big...)
* (Max. number of closure phase relations allowed for each pixel of
* the uv-coverage).
* IFERMAX is an internal upper limit to this routine, whereas NCLOSURE_MAX is set
* when computing the data.
* Now I set ifermax to nclos (Sept 2008):
*/
   ifermax = nclos_max;

/* Opening logfile: */
   strcpy(logfile_name,"inv_bispec2.log");
   if((fp1 = fopen(logfile_name,"w")) == NULL)
      {
       fprintf(stderr,"inv_bispec2/Fatal error opening logfile %s \n",logfile_name);
       return(-1);
      }

  fprintf(fp1," Subroutine inv_bispec2, version 10/09/2008\n");

/* cte = angular reference for bispectral perturbation simulations (degrees)
* Used for weight computation and weight selection...
* This reference is linked to the square norm of IR frequency
* Formerly: cte = 20.
*
*/
/* Output to logfile: */
  fprintf(fp1," Maximum number of closure relations allowed by the program: ifermax=%d \n",
          ifermax);
  fprintf(fp1,"ir: %d, cte: %6.2f, lower_modulus: %12.5e \n",
          ir, cte, lower_modulus);
  fprintf(fp1," Sig_max (for weights): %10.3e  Sig_null: %10.3e \n",
          sig_max,sigma_null);



  fprintf(fp1,
   " Angular constant for bispectral noise estimation: %8.3e (degrees)\n",cte);

/* First dimension of array xx  */
   idim_xx = nbeta + 1;

   xc_re = new double[nbeta + 1];
   xc_im = new double[nbeta + 1];
   bmask = new double[nbeta + 1];
   ro = new double[nbeta + 1];
   sigm = new double[nbeta + 1];

/* COMPLEX YCE(NGMAX) */
  yce_re = new double[(ngamma + 1) * 2];
  yce_im = new double[(ngamma + 1) * 2];

/* WEIGHT(NGMAX) */
   weight = new double[ngamma + 1];

/* vecxy: Kernel vectors
*  VECXY(NBMAX+1,2) */
   vecxy = new double[(nbeta + 1) * 2];
/* REAL X(NBMAX+1,4) */
   xx = new double[(nbeta + 1) * 4];

/* Creates YCE, bispectrum phasor, from bisp1
* and initial set of weights */
  CREYCE_OBSERV(yce_re, yce_im, xc_re, xc_im, ro, ir, cte, nbeta, ngamma,
                ngamma_maxi, lower_modulus, modsq1, bisp1, weight, bmask,
                nx1, ny1);

/******************************************************
* To initialize the solution,
* we solve the problem with the recursive method (Weigelt,...)
* The initial spectral phase is stored in XC(.)
*/
/* Compute the recursive solution and derives the standard
* deviation sigm of the spectral terms */
     RECURSIVE(sigm,nbeta,ngamma,sigma_null,ifermax,
               bmask,yce_re,yce_im,xc_re,xc_im,weight);

/* Projection of the initial solution onto E^+ */
    status = EPLUS(nbeta,bmask,xc_re,xc_im,xx,idim_xx,vecxy);

/* SORTIE DES PARTIES REELLES ET IMAGINAIRES DE XC
*/
   if(!status)
      SORTIE(re_out, im_out, nx1, ny1, xc_re, xc_im, ro, nbeta, bmask);

delete yce_re;
delete yce_im;
delete xc_re;
delete xc_im;
delete bmask;
delete ro;
delete sigm;
delete weight;
delete xx;
delete vecxy;

fclose(fp1);

return(status);
}
/*******************************************************************
* TRANS
* fait le calage en translation du facteur de phase spectral XC(.)
* En sortie, la phase de nb=1 est zero degre.
*
* A translation in the direct plane of (Dx, Dy) induces an increase of
* the phase of u_k by (2 pi (u_kx Dx + u_ky Dy)):
* TF[f(x - Dx)](u_kx, u_ky) =
*                  exp(-2 i pi (u_kx Dx + u_ky Dy)) * TF[f(x)](u_kx,u_ky)
*
*  XC(1+1) = exp i Beta(1)
*
* Array TX is defined as:
*  TX(1) = exp -i Beta(1)
*  TX(-1)= exp i Beta(1)
*  TX(2) = exp -i 2 Beta(1)
*   ....
*  TX(N) = exp -i N Beta(1)
*
* OUTPUT:
*  If coordinates of frequency #NB are: (IXY1,IXY2)
*  XC[NB] = XC[NB] * TX(IXY1) * TY(IXY2)
*  i.e.:
*  exp i Beta_new = exp i Beta_old * exp -i IXY1 Beta(1) * exp -i IXY2 Beta(2)
*******************************************************************/
int TRANS(int ir, int nbeta, double *bmask, double *xc_re, double *xc_im)
{
double tx_re[IRMAX+1], tx_im[IRMAX+1], ty_re[IRMAX+1], ty_im[IRMAX+1];
double cx_re, cx_im, cy_re, cy_im;
double ccx_re, ccx_im, ccy_re, ccy_im, work_re, work_im;
int iix, iiy, nb;
register int i;

    if(ir > IRMAX)
      {
        fprintf(fp1," TRANS/Fatal error, maximum IR= %d \n",IRMAX);
        fclose(fp1); return(-1);
      }

/* Take the conjugate values of xc[1], xc[2] as a starting point,
* to obtain a null phase  for xc[1] and xc[2]i at the end of this routine
* i.e., xc[1]=(1,0) and xc[2]=(1,0)
*/
/* cx=conjg(xc(1+1)) */
  cx_re = xc_re[1];
  cx_im = -xc_im[1];
/* cy=conjg(xc(2+1)) */
  cy_re = xc_re[2];
  cy_im = -xc_im[2];

/* Tableau de la forme lineaire concerne du noyau */
  ccx_re = 1.; ccx_im = 0.;
  ccy_re = 1.; ccy_im = 0.;
  tx_re[0] = 1.; tx_im[0] = 0.;
  ty_re[0] = 1.; ty_im[0] = 0.;

   for(i = 1; i <= ir; i++)
     {
/* CCX=CCX*CX  Complex multiplication. */
       COMPLEX_PROD2(ccx_re,ccx_im,cx_re,cx_im,1,1,&ccx_re,&ccx_im);
       tx_re[i] = ccx_re;
       tx_im[i] = ccx_im;

/* CCY=CCY*CY  Complex multiplication. */
       COMPLEX_PROD2(ccy_re,ccy_im,cy_re,cy_im,1,1,&ccy_re,&ccy_im);
       ty_re[i] = ccy_re;
       ty_im[i] = ccy_im;
     }

/* Calage en translation */
     for(nb = 0; nb <= nbeta; nb++)
       {
/* XC(NB) = XC(NB) * TX(IXY(1,NB)) * TY(IXY(2,NB)) */
       COVER_IXY(&iix,&iiy,&nb);
              if(iix >= 0)
                {
                  cx_re = tx_re[iix];
                  cx_im = tx_im[iix];
                }
              else
                {
                  cx_re = tx_re[-iix];
                  cx_im = -tx_im[-iix];
                }
            cy_re = ty_re[iiy];
            cy_im = ty_im[iiy];

/* XC[NB] = XC[NB] * CX * CY; Complex multiplication... */
/*
            work_re =   xc_re[nb] * cx_re * cy_re
                      - xc_im[nb] * cx_im * cy_re
                      - xc_re[nb] * cx_im * cy_im
                      - xc_im[nb] * cx_re * cy_im;
            work_im =   xc_re[nb] * cx_im * cy_re
                      + xc_im[nb] * cx_re * cy_re
                      + xc_re[nb] * cx_re * cy_im
                      - xc_im[nb] * cx_im * cy_im;
*/
            COMPLEX_PROD3(xc_re[nb],xc_im[nb],cx_re,cx_im,cy_re,cy_im,
                          1,1,1, &work_re, &work_im);
            xc_re[nb] = work_re * bmask[nb];
            xc_im[nb] = work_im * bmask[nb];
      }

 return(0);
}
/*******************************************************************
* CREYCE_LOAD_BISP
* Load input data from file to YCE array
* yce index will be like fortran arrays: between 1 and ngamma
*******************************************************************/
int CREYCE_LOAD_BISP(double *yce_re, double *yce_im, double *bisp1,
                     int ngamma, int ngamma_max)
{
double xr, xi, xm;
int ng, inull;


  inull = 0.;
  for(ng = 0; ng < ngamma; ng ++)
  {
      xr = bisp1[ng];
      xi = bisp1[ng + ngamma_max];
/* Modulus */
      xm = sqrt((double)(xr*xr + xi*xi));
/* Phasor: */
      if(xm == 0.)
         {
         yce_re[ng+1] = 1.;
         yce_im[ng+1] = 0.;
         inull++;
         }
      else
         {
         yce_re[ng+1] = xr / xm;
         yce_im[ng+1] = xi / xm;
         }
    }

  if(inull > 0)
    {
    fprintf(fp1," CREYCE_LOAD_BISP/Warning: null bispectrum for %d values \n",
           inull);
    }

/* Debug: */
  for(ng = 1; ng <= 5; ng ++)
    {
     fprintf(fp1," YCE(%d) = %12.5f %12.5f \n",ng,yce_re[ng],yce_im[ng]);
    }

return(0);
}
/**********************************************************************
* NORMALIZE_L1
* Norm 1 is the initial value of the sum of the absolute values of array.
* WARNING: it works with arrays from 1 to ngamma
*         Index from 1 to npts inclusive!!!
***********************************************************************/
int NORMALIZE_L1(double *array, int npts, double *norm)
{
double ssum;
register int i;

 ssum=0.;
 for(i = 1; i <= npts; i++)
  {
  if(array[i] > 0)
    ssum += array[i];
  else
    ssum -= array[i];
  }

/* Normalisation of the array:
*/
    if(ssum == 0.)
      {
        fprintf(fp1," NORMALIZE_L1/Fatal error: norm is null\n");
        fclose(fp1); return(-1);
      }

    for(i = 1; i <= npts; i++) array[i] /= ssum;

    *norm = ssum;

return(0);
}
/**********************************************************
* To normalize a vector with L2 norm
* Called to normalize the two vectors of the Kernel
* Norm 2 is the square root of the sum of the squared values of array.
* WARNING: index from 1 to npts inclusive!!!
**********************************************************/
int NORMALIZE_L2(double *array, int npts, double *norm)
{
double ssum;
register int i;

 ssum=0.;
 for(i = 1; i <= npts; i++)
    ssum += (array[i] * array[i]);


/* Normalisation of the array:
*/
  if(ssum == 0.)
    {
      fprintf(fp1," NORMALIZE_L2/Fatal error: norm is null\n");
      fclose(fp1); return(-1);
    }


 *norm = sqrt(ssum);
 for(i = 1; i <= npts; i++) array[i] /= *norm;

return(0);
}
/*********************************************************************
* Load the initial weights for computing the recursive solution
* Called by CREYCE_OBSERV
*
* OUTPUT:
* weight: weights to be used for computing the recursive solution
*********************************************************************/
int CREYCE_BISP_WEIGHTS(int ngamma, int ngamma_max, int ir,
                        double cte, double *bmask, double *ro, double *bisp1,
                        double *weight)
{
double xnorm;
int k, kk, ng, status = -1;

/* Computing the weights: */
        if(cte > 0)
          {
           BISP_WEIGHT11(ngamma,ir,cte,bmask,ro,weight);
           status = NORMALIZE_L1(weight,ngamma,&xnorm);
          }
        else if (cte < 0)
          {
/* With SNR stored in 3rd line of bispectrum: */
          BISP_WEIGHT2(bisp1,ngamma,ngamma_max,weight);
          status = NORMALIZE_L1(weight,ngamma,&xnorm);
          }
        else
          {
           fprintf(fp1," Weights set to unity, and then normalized \n");
           for(ng = 1; ng <= ngamma; ng++)
              {
               weight[ng]=1.;
               for(kk = 1; kk <= 3; kk++)
                  {
                  cover_klm1(&k,kk,ng);
                  if(bmask[k] == 0.) weight[ng]=0.;
                  }
              }
           status = NORMALIZE_L1(weight,ngamma,&xnorm);
          }
return(status);
}
/*****************************************************************
* BISP_WEIGHT11
* Computing the weights:
* Old version (before 25-07-91)
*
*****************************************************************/
int BISP_WEIGHT11(int ngamma, int ir, double cte,
                  double *bmask, double *ro, double *weight)
{
double work;
double cte1, sigb, sig2;
int ng, is2, k, l, m, kk, iix, iiy;

 cte1 = cte * DEGRAD / (double)(ir*ir);

    for(ng = 1; ng <= ngamma; ng++)
      {
       is2 = 0;
       cover_klm1(&k,1,ng);
       cover_klm1(&l,2,ng);
       cover_klm1(&m,3,ng);
       work = ro[k] * ro[l] * ro[m];
       work *= (bmask[k] * bmask[l] * bmask[m]);
       if(work == 0)
           weight[ng]=0.;
       else
           {
            for(kk = 1; kk <= 3; kk++)
             {
              cover_klm1(&k,kk,ng);
              COVER_IXY(&iix,&iiy,&k);
              is2 += iix*iix + iiy*iiy;
             }
/* sigb = ecart type (ou standard deviation) de gamma
*/
          sigb = cte1 * (double)is2;

/* Valeur moyenne du carre de [2 sin( (gamma - gamma barre)/2 ) ]
* cette valeur moyenne majore la precedente
*/
          work = -0.5*(sigb*sigb);
          sig2 = 1. - exp(work);

/* WEIGHT[NG] : (do not limit the weight values ...)
*/
         if(sig2 > 1.E-9)
              weight[ng] = 1./sig2;
         else
             {
             fprintf(fp1," BISP_WEIGHT11/Warning: Null sigma for NG = %d (sigb = %f)\n",ng,sigb);
             weight[ng] = 1.E+9;
             }
        }
    }

 fprintf(fp1," WEIGHT11/Old version.\n");

return(0);
}
/*****************************************************************
* BISP_WEIGHT2
* Computing the weights with SNR stored in the bispectrum file (line 3)
*
******************************************************************/
int BISP_WEIGHT2(double *bisp1, int ngamma, int ngamma_max, double *weight)
{
double sum;
double mean_snr;
int ng;

 sum = 0.;
 for(ng = 0; ng < ngamma; ng++) {
    weight[ng+1] = ABS(bisp1[ng + 2 * ngamma_max]);
    sum += weight[ng+1];
    }

/* Diagnostic: */
  mean_snr = sum/(double)ngamma;

  fprintf(fp1," BISP_WEIGHT2/SNR weight Initial sum: %12.5f Mean bisp. SNR: %12.5f \n",
           sum,mean_snr);
return(0);
}
/*******************************************************************
* CREYCE_OBSERV
* Reads a real spectrum (square modulus only) and bispectrum
* (Case of observations)
*
* INPUT:
* modsq1
* bisp1
*
* OUTPUT:
*  yce_re, yce_im: bispectrum phasor
*  bmask: mask applied to the spectral list to discard bad data
*******************************************************************/
int CREYCE_OBSERV(double *yce_re, double *yce_im, double *xc_re, double *xc_im,
                  double *ro, int ir, double cte, int nbeta, int ngamma, int ngamma_max,
                  double lower_modulus, double *modsq1, double *bisp1,
                  double *weight, double *bmask, int nx1, int ny1)
{
double *re, *im;
int ixc, iyc, nb, inull, iix, iiy, irem, ir2, irad2;
double work, small_sqmodulus;
register int i, j;

/* Allocation of memory: */
   re = new double[nx1 * ny1];
   im = new double[nx1 * ny1];
   ixc = nx1 / 2;
   iyc = ny1 / 2;

/* Look for the minimum value of the modulus: */
   small_sqmodulus = ABS(modsq1[0]);
   ir2 = ir * ir;
   for(j = 0; j < ny1; j++)
     for(i = 0; i < nx1; i++) {
     irad2 = SQUARE(i - ixc) + SQUARE(j - iyc);
     if(irad2 < ir2)
        small_sqmodulus = MINI(small_sqmodulus,ABS(modsq1[i + j * nx1]));
     }
   fprintf(fp1," Smallest positive value of square modulus: %12.5e\n",
            small_sqmodulus);

/* Initializing the real and imaginary part of the spectrum: */
  inull = 0;
  for(j = 0; j < ny1; j++)
    {
    for(i = 0; i < nx1; i++)
        {
           work = modsq1[i + j * nx1];
/* JLP2008: DO NOT DISCARD NEGATIVE MODULII ...
*/
           if(work < 0.) work = small_sqmodulus;

           re[i + j * nx1] = sqrt(work);
           im[i + j * nx1] = 0.;
        }
    }

/* Assume that zero frequency is at IXC,IYC:  */
   ixc = nx1 / 2;
   iyc = ny1 / 2;

/* Loading modulus and phasor to ro and xc */
     for(nb = 0; nb <= nbeta; nb++)
        {
/* Get coordinates (iix,iiy) from nb index: */
         COVER_IXY(&iix,&iiy,&nb);
/* Add (ixc,iyc) vector, since centered FFT... */
         iix += ixc;
         iiy += iyc;
/* Modulus (here it is the real part) : */
         ro[nb] = re[iix + iiy * nx1];
/* Output of some values of the modulus: */
#ifdef DEBUG
    if(nb < 5) fprintf(fp1," iix=%d iiy=%d RO(%d)=%f \n",iix, iiy, nb, ro[nb]);
#endif
/* Phase factor: */
         xc_re[nb] = 1.;
         xc_im[nb] = 0.;
        }

    if(inull > 0)
       {
       fprintf(fp1," CREYCE_OBSERV/Modulus null for %d values\n",inull);
       }


/* Truncation: */
    irem = 0;
/* Mask to discard some values of the spectral list: */
    for(nb = 0; nb <= nbeta; nb++)
    {
/* Check that RO[NB] greater than LOWER_MODULUS:
* Remember that LOWER_MODULUS can be negative... */
      if(ro[nb] < lower_modulus || ro[nb] < 0.)
        {
/*
         printf("CREYCE_OBSERV/Discarding value ro[%d]=%e (bmask)\n",
                nb, ro[nb]);
*/
         bmask[nb] = 0.;
         irem++;
        }
      else
        bmask[nb] = 1.;
   }

   fprintf(fp1," Number of terms of the spectral list after bmask selection: %d(%d removed) \n",
            nbeta-irem, irem);

/* Spectrum phasor blocked in translation: xc[.] */
   TRANS(ir, nbeta, bmask, xc_re, xc_im);

/***********************************************************************
* Bispectrum phasor: YCE
*/
/* Copy BISP array to YCE arrays: */
    CREYCE_LOAD_BISP(yce_re, yce_im, bisp1, ngamma, ngamma_max);

/* Compute weights: */
    CREYCE_BISP_WEIGHTS(ngamma, ngamma_max, ir, cte, bmask, ro, bisp1, weight);

delete re;
delete im;

return(0);
}
/*******************************************************************
* RECURSIVE: Initial solution of the phasor.
*                      YCE(.)  to  XC(.)
*
* exp(i*YCE[NG]) = exp (i * phase_K) * exp (i * phase_L) * exp (i * phase_M)
* with L=NB or M=NB
*
* INPUT:
*  yce_re, yce_im: bispectrum phasor
*  weight: weights used for computing the recursive solution
*
* OUTPUT:
*  xc_re[nb], xc_im[nb]: spectrum corresponding to the recursive solution
*                        (distributed along the spectral list)
*  sigm[nb]: standard deviation of the spectral terms obtained with
*            the recursive solution (distributed along the spectral list)
*******************************************************************/
int RECURSIVE(double *sigm, int nbeta, int ngamma, double sigma_null,
              int ifermax, double *bmask, double *yce_re, double *yce_im,
              double *xc_re, double *xc_im, double *weight)
{
double cc1_re, cc1_im;
/* Double precision is necessary for large sums !!!
*/
double sumxr1, sumxi1, sumsqr, sumsqi, sigr, sigi, sumweights, rnorm;
double xw;
int k, l, m, ng1, ng2, nb, ng, indx, nval, inull;

/* JLP96: */
FILE *fp2;
fp2 = fopen("recurs.dat","w");

xc_re[0] = 1.;
xc_im[0] = 0.;
xc_re[1] = 1.;
xc_im[1] = 0.;
xc_re[2] = 1.;
xc_im[2] = 0.;
/* In the central part of the spectrum, good SNR...
*/
sigm[0] = MIN_SIGMA;
sigm[1] = MIN_SIGMA;
sigm[2] = MIN_SIGMA;

ng1 = 1;
inull = 0;

for(nb = 3; nb <= nbeta; nb++)
 {
 sumxi1 = 0.;
 sumxr1 = 0.;
 sumsqr = 0.;
 sumsqi = 0.;
 sumweights = 0.;
/* JLP94: sigma has a meaning only if more than 2 values are involved in the
* mean...
*/
 nval = 0;
/* NG2=NGT(NB) */
 cover_ngt1(&ng2,nb);
 indx = 0;
/* For each nb index, the bispectral list is contained in the range ng1, ng2:
*/
 for(ng = ng1; ng <= ng2; ng++)
   {
   indx++;
   if(indx <= ifermax)
     {
/*  K=KLM(1,NG) L=KLM(2,NG) M=KLM(3,NG)
*/
      cover_klm1(&k,1,ng);
      cover_klm1(&l,2,ng);
      cover_klm1(&m,3,ng);
      xw = weight[ng];
      if(xw > 0.)
         {
          if(m == nb)
             {
/* Case 1 (m = nb): cc1=xc(k)*xc(l)*conjg(yce[ng]) */
             COMPLEX_PROD3(xc_re[k],xc_im[k],xc_re[l],xc_im[l],
                  yce_re[ng],yce_im[ng],1,1,-1,&cc1_re,&cc1_im);
             }
          else
             {
/* Case 2 (l = nb): cc1=conjg(xc(k))*xc(m)*yce[ng] */
             COMPLEX_PROD3(xc_re[k],xc_im[k],xc_re[m],xc_im[m],
                    yce_re[ng],yce_im[ng],-1,1,1,&cc1_re,&cc1_im);
             }
          nval++;
          sumsqr += (cc1_re * cc1_re * xw);
          sumsqi += (cc1_im * cc1_im * xw);
          sumxr1 += (cc1_re * xw);
          sumxi1 += (cc1_im * xw);
          sumweights += xw;
/* End of test on xw (weight) */
         }
/* End of test on ifermax  (max number of closure relations) */
       }
/* End of ng loop */
    }

/* Now since the phase of xc[nb] has been found, we normalize the phasor: */
    rnorm = sqrt(sumxr1 * sumxr1 + sumxi1 * sumxi1);
    if(rnorm > 0.)
       {
        xc_re[nb] = sumxr1/rnorm;
        xc_im[nb] = sumxi1/rnorm;

/* JLP94: sigma has a meaning only if more than 3 values are involved in the
* mean... */
        if(nval >= 3)
          {
/* SIGR**2 = Sum of (weight_i *(X_i - mean)**2) / Sum of (weight_i)
* or Sum of (weight_i X_i**2)/Sum of weight_i - mean**2
* Here the weighted mean of real(CC) is SUMXR1/SUMWEIGHTS:
* (Checked again that's OK in 1996)
*/
           sigr = sumsqr / sumweights - SQUARE(sumxr1 / sumweights);
           sigi = sumsqi / sumweights - SQUARE(sumxi1 / sumweights);
/* Note that double precision variables are needed for large sums !!!
*/
           if(sigr < 0. || sigi < 0.) {
/* Just in case of a (round-off??) problem:
             printf("\n\n");
             printf(" Round-off problem in RECURSIVE... for NB= %d \n",nb);
             fprintf(fp1," Round-off problem in RECURSIVE... for NB= %d \n",nb);
             printf(" sigi: %f sigr: %f \n",sigi,sigr);
             fprintf(fp1," sigi: %f sigr: %f \n",sigi,sigr);
             printf(" rnorm: %f sumweights: %f \n",rnorm,sumweights);
             printf(" sumsqr: %f sumsqi: %f \n",sumsqr,sumsqi);
             printf(" sumxr1: %f sumxi1: %f \n",sumxr1,sumxi1);
             printf(" ng1: %d ng2: %d \n",ng1,ng2);
             printf(" weight(ng1): %f  weight(ng2): %f\n",
                      weight[ng1],weight[ng2]);
             printf("\n\n");
*/
/* JLP97: I comment this out:
             fclose(fp1); exit(-1);
 and replace with:
*/
              sigm[nb] = 0.;
           } else {
              sigm[nb] = sqrt(sigi+sigr);
/* JLP96: */
              fprintf(fp2,"%d phase:|(%.2f,%.2f)|=%f sig:|%.3f,%.3f|=%f nval=%d\n",
                  nb,xc_re[nb],xc_im[nb],rnorm,sqrt(sigr),sqrt(sigi),sigm[nb],nval);
           }
/* End of sigr < 0 or sigi < 0 */
/* If less than 3 values were used for the mean computation:
* Assume that it is the same as for the previous spectral term
*/
       } else {
          fprintf(fp1,"JLPPP: less than 3 values for nb=%d (nval=%d)\n", nb, nval);
          sigm[nb] = sigm[nb-1];
       }
/* rnorm = 0, i.e., the recursive process has been interrupted: */
     } else {
       inull++;
/* We arbitrarily set the phase to zero: */
       xc_re[nb] = 1.;
       xc_im[nb] = 0.;
/* Maximum sigma is one, but I set it to SIGMA_NULL since we assume that
* phase is not important if modulus is too small
       printf("JLPPP: recursive process interrupt for nb=%d (nval=%d)\n",
               nb, nval);
       printf(" rnorm: %f sumweights: %f \n",rnorm,sumweights);
       printf(" sumsqr: %f sumsqi: %f \n",sumsqr,sumsqi);
       printf(" sumxr1: %f sumxi1: %f \n",sumxr1,sumxi1);
       printf(" ng1: %d ng2: %d  (ngamma=%d)\n", ng1, ng2, ngamma);
       printf(" weight(ng1): %f  weight(ng2): %f\n", weight[ng1],weight[ng2]);
       printf("\n\n");
*/
       sigm[nb] = sigma_null;
       sigm[nb] = 1.;
/* Allows breaks only if in the list of null modulus: */
       if(bmask[nb] != 0.) {
/* Then add this frequency to BMASK, since it won't be able to recover the
* phase of this frequency:
*/
           bmask[nb] = 0.;
           fprintf(fp1," RECURSIVE/Warning: too many null modulii: the phase of xc[%d] will remain undetermined.\n",nb);
#ifdef DEBUG
/* Print the values of the modulii that may be null: */
           fprintf(fp1," ng1=%d, ng2=%d sumxr1=%e sumxi1=%e rnorm=%e\n",
                       ng1,ng2,sumxr1,sumxi1,rnorm);
           for(ng = ng1; ng <= ng2; ng++) {
                cover_klm1(&k,1,ng);
                cover_klm1(&l,2,ng);
                cover_klm1(&m,3,ng);
                fprintf(fp1," ng=%d, k=%d, l=%d, m=%d \n",ng,k,l,m);
/*
                fprintf(fp1," ro[k]=%.4e, ro[l]=%.4e, ro[m]=%.4e weight[ng]=%.4e\n",
                   ro[k],ro[l],ro[m],weight[ng]);
*/
           }
#endif
        }
/* End of bmask != 0 */
     }
/* End of interruption of recursive process */
   ng1 = ng2+1;
/* End of nb loop */
   }

/* JLP96: */
  fclose(fp2);

/* Set the first 4 values to zero, to avoid problems... */
sigm[0] = MIN_SIGMA;
sigm[1] = MIN_SIGMA;
sigm[2] = MIN_SIGMA;
sigm[3] = MIN_SIGMA;
sigm[4] = MIN_SIGMA;

/* Applies a threshold on sigm, to avoid SIGM = 0,
* and problems when computing 1/SIGM in SNR map:
*/
for(nb = 5; nb <= nbeta; nb++) sigm[nb] = MAXI(MIN_SIGMA, sigm[nb]);

/* Diagnostic: */
 if(inull > 0)
   {
   fprintf(fp1," RECURSIVE/Warning: Null modulus (i.e., the spectral list has been reduced) for %d values \n",inull);
   fprintf(fp1," WARNING: In that case SIGMA was set to %f (and SNR to %f) \n",
           sigma_null, 1./sigma_null);
   }

return(0);
}
/**************************************************************
* COMPLEX_PROD2
* Complex product of two complex values
* INPUT:
* c1_re, c1_im, c2_re, c2_im
* sign1, sign2 ("sign" of imaginary part: 1 if simple product, -1 if conjugate)
* OUTPUT:
* (c1 * c2)_re
* (c1 * c2)_im
**************************************************************/
int COMPLEX_PROD2(double c1_re, double c1_im, double c2_re, double c2_im,
                  int sign1, int sign2, double *c12_re, double *c12_im)
{
/* The values are modified only in this routine (not outside) */
  c1_im *= (double)sign1;
  c2_im *= (double)sign2;

/* c12 = c1 * c2: Complex multiplication... */
 *c12_re =   c1_re * c2_re - c1_im * c2_im;
 *c12_im =   c1_re * c2_im + c1_im * c2_re;
return(0);
}
/**************************************************************
* COMPLEX_PROD3
* Complex product of three complex values
* INPUT:
* c1_re, c1_im, c2_re, c2_im, c3_re, c3_im
* sign1, sign2, sign3 ("sign" of imaginary part:
*                             1 if simple product, -1 if conjugate)
* OUTPUT:
* (c1 * c2 * c3)_re
* (c1 * c2 * c3)_im
**************************************************************/
int COMPLEX_PROD3(double c1_re, double c1_im, double c2_re, double c2_im,
                  double c3_re, double c3_im, int sign1, int sign2, int sign3,
                  double *c123_re, double *c123_im)
{
/* The values are modified only in this routine (not outside) */
  c1_im *= (double)sign1;
  c2_im *= (double)sign2;
  c3_im *= (double)sign3;

/* c123 = c1 * c2 * c3: Complex multiplication... */
 *c123_re =   c1_re * c2_re * c3_re
            - c1_im * c2_im * c3_re
            - c1_re * c2_im * c3_im
            - c1_im * c2_re * c3_im;
 *c123_im =   c1_re * c2_im * c3_re
            + c1_im * c2_re * c3_re
            + c1_re * c2_re * c3_im
            - c1_im * c2_im * c3_im;
return(0);
}
/*******************************************************************
* EPLUS projects ALPHA_0 onto E^+
* xx(idim_x,4)
* vecxy(idim_xx,2)
*******************************************************************/
int EPLUS(int nbeta, double *bmask, double *xc_re, double *xc_im,
          double *xx, int idim_xx, double *vecxy)
{
double cc_re, cc_im, xxc, alpha;
int nb, status;

/* Vectors of the base of the kernel */
  status = NOYAU(nbeta,bmask,vecxy,idim_xx);
  if(status) return(-1);

/* Alpha_0 en xx(.,1) */
 for(nb = 0; nb <= nbeta; nb++)
   {
    cc_re = xc_re[nb];
    cc_im = xc_im[nb];
/* X(NB+1,1) = BMASK[NB]*ATAN2( IMAG(CC), REAL(CC) ) */
    jlp_atan2c(&xxc,cc_im,cc_re);
    xx[nb] = bmask[nb] * xxc;
   }
/* Alpha_0^+  en xx(.,1) */
    PROJ_EPLUS(nbeta,1,1,xx,idim_xx,vecxy);

/* Corresponding initial phasor: */
 for(nb = 0; nb <= nbeta; nb++)
   {
/* ALPHA=X(NB+1,1) */
    alpha = xx[nb];
    xc_re[nb] = bmask[nb] * cos((double)alpha);
    xc_im[nb] = bmask[nb] * sin((double)alpha);
   }

return(0);
}
/*******************************************************************
* NOYAU:
* Define the vectors of the base of the kernel
*  vecxy(idim_xx,2)
*******************************************************************/
int NOYAU(int nbeta, double *bmask, double *vecxy, int idim_xx)
{
int nb, iix, iiy, status;
double xnorm, sum;

/* The base vectors are formed with the coordinates of the uv vectors.
*/
for(nb = 0; nb <= nbeta; nb++)
  {
/*  VECXY(NB,1)=BMASK(NB)*IXY(1,NB)
*  VECXY(NB,2)=BMASK(NB)*IXY(2,NB)
*/
    COVER_IXY(&iix,&iiy,&nb);
    vecxy[nb] = bmask[nb] * (double)iix;
    vecxy[nb + idim_xx] = bmask[nb] * (double)iiy;
  }

/* Normalization of the first vector: */
  status = NORMALIZE_L2(&vecxy[0],nbeta,&xnorm);
  if(status) return(-1);

/* Orthogonalization (Schmidt process): */
/* First compute scalar product: */
sum = 0.;
for(nb = 0; nb <= nbeta; nb++)
   sum += (vecxy[nb] * vecxy[nb + idim_xx]);

/* Then removes to the second vector its projection unto the first one: */
/* VECXY(NB+1,2)=VECXY(NB+1,2)-SUM*VECXY(NB+1,1) */
for(nb = 0; nb <= nbeta; nb++)
   vecxy[nb + idim_xx] = vecxy[nb + idim_xx] - sum * vecxy[nb];

/* Normalization of the second vector: */
  status = NORMALIZE_L2(&vecxy[idim_xx],nbeta,&xnorm);

return(status);
}
/*******************************************************************
* PROJ_EPLUS
* Projects X(.,N1) onto E^+ and stores result in X(.,N2)
* X(IDIM_X,4),VECXY(IDIM_X,2)
*******************************************************************/
int PROJ_EPLUS(int nbeta, int n1, int n2, double *xx,
               int idim_xx, double *vecxy)
{
double xx1, yy1;
int nb, nn1, nn2;

/* Scalar product of X(.,N1) with the base vectors of the kernel */
xx1 = 0.;
yy1 = 0.;
nn1 = n1 - 1;
nn2 = n2 - 1;

for(nb = 1; nb <= nbeta; nb++)
  {
   xx1 += xx[nb + nn1 * idim_xx] * vecxy[nb];
   yy1 += xx[nb + nn1 * idim_xx] * vecxy[nb + idim_xx];
  }

/*
 delta = sqrt(xx1 * xx1 + yy1 * yy1);
 printf("xx1 = %17.10e, yy1= %17.10e \n",xx1,yy1);
 printf("Distance to E+: DELTA = %17.10e \n",delta);
*/

/* Projection onto E^+ and stored in X(.,N2) */
for(nb = 1; nb <= nbeta; nb++)
   xx[nb + nn2 * idim_xx] = xx[nb + nn1 * idim_xx]
           - xx1 * vecxy[nb] - yy1 * vecxy[nb + idim_xx];

return(0);
}
/*******************************************************************
* SORTIE: write output files
*
* INPUT:
*  xc_re, xc_im: real and imaginary part (spectral list)
* OUTPUT:
* re_out, im_out
*******************************************************************/
int SORTIE(double *re_out, double *im_out, int nx1, int ny1,
           double *xc_re, double *xc_im, double *ro, int nbeta, double *bmask)
{
double cc_re, cc_im, xr, xi, xm;
int nb, ixc, iyc, iix, iiy, ix, iy;
register int i;

/* Assume that zero frequency is at IXC,IYC:  */
   ixc = nx1 / 2;
   iyc = ny1 / 2;

/* Initializing the real and imaginary parts: */
    for(i = 0; i < nx1 * ny1; i++) {
        re_out[i] = 0.;
        im_out[i] = 0.;
        }

/* Computing real and imaginary parts: */
for(nb = 0; nb <= nbeta; nb++)
   {
/* JLP94, try...
*  XM = BMASK[NB]*RO[NB]
*/
     xm = ro[nb];
     cc_re = xc_re[nb];
     cc_im = xc_im[nb];
     xr = xm * cc_re;
     xi = xm * cc_im;
     COVER_IXY(&ix,&iy,&nb);
     iix = ixc + ix;
     iiy = iyc + iy;
     re_out[iix + iiy * nx1] = xr;
     im_out[iix + iiy * nx1] = xi;
     iix = ixc - ix;
     iiy = iyc - iy;
     re_out[iix + iiy * nx1] = xr;
     im_out[iix + iiy * nx1] = -xi;
   }

return(0);
}
/***********************************************************
* jlp_atan2c
* C Version of Fortran ATAN2
* Arc Tangent (radians)
* Slightly different (leads to differences of 1/10000 in final images)
************************************************************/
int jlp_atan2c(double *xxc, double cc_im, double cc_re)
{
*xxc = atan(cc_im / cc_re);
/* To be compatible with fortran ATAN2(cc_im,cc_re): */
 if(cc_re < 0)
    {
    *xxc += PI;
    if(*xxc > PI) *xxc -= 2. * PI;
    }
return(0);
}
/**********************************************************************/
