//---------------------------------------------------------------------------
#ifndef jlp_fits1_h
#define jlp_fits1_h

#include "asp2_typedef.h" // AWORD 

int readimag_aword_fits(char *filename, AWORD **image, int *nx0,
                        int *ny0, char *comments, char *errmess);
int readimag_float_fits(char *filename, float **flt_image, int *nx0,
                        int *ny0, char *comments, char *errmess);

#endif
//---------------------------------------------------------------------------
 
