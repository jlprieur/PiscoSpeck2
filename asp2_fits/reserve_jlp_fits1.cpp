/***************************************************************************
* jlp_fits1.cpp
*
* JLP
* Version 17/08/2015
***************************************************************************/
#include <stdio.h>
#include <math.h>

#include "jlp_fits1.h"
#include "jlp_fitsio.h"             // JLP_RDFITS_2D_dble, JLP_WRFITS_2D_dble
/*********************************************************************
* Interface with jlp_fitsio routines
* int JLP_RDFITS_2D_flt(float **array1, int *nx1, int *ny1, int *nz1,
*                       int iplane, char *infile, char *comments,
*                       char *err_mess);
*
*********************************************************************/
int readimag_aword_fits(char *filename, AWORD **image, int *nx0,
                        int *ny0, char *comments, char *errmess)
{
int status, nz0, iplane, i, npts, ivalue;
float **flt_image;

iplane = 0;
status = JLP_RDFITS_2D_flt(flt_image, nx0, ny0, &nz0, iplane,
                           filename, comments, errmess);
if(status == 0) {
  npts = (*nx0) * (*ny0);
  *image = new AWORD[npts];
  for(i = 0; i < npts; i++) {
// Compiler complains here, so I go in two steps:
    ivalue = (int)flt_image[i];
    (*image)[i] = ivalue;
    }
  }

return(status);
}
/*********************************************************************
* Interface with jlp_fitsio routines
* int JLP_RDFITS_2D_flt(float **array1, int *nx1, int *ny1, int *nz1,
*                       int iplane, char *infile, char *comments,
*                       char *err_mess);
*
*********************************************************************/
int readimag_float_fits(char *filename, float **flt_image, int *nx0,
                        int *ny0, char *comments, char *errmess)
{
int status, nz0, iplane = 0;

status = JLP_RDFITS_2D_flt(flt_image, nx0, ny0, &nz0, iplane,
                           filename, comments, errmess);

return(status);
}
