/******************************************************************************
* Set of routines to read data from/to 3D FITS files
*
* JLP Version
* Version 06/11/2017
******************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "jlp_time0.h"
#include "asp2_3dfits_rd_utils.h"
#include "jlp_rw_fits0.h"

/******************************************************************************
* Open a byte FITS 3D cube
*
* bitpix = 8, 16 or 32;
* Here 8 bits (i.e BYTE) images
*
* INPUT:
*  filename
*
* OUTPUT:
*  nx, ny, nz_total, comments, err_mess
*  fp_FITS_cube: pointer to the FITS cube file
******************************************************************************/
int open_input_byte_fits_cube(char *filename, int *nx, int *ny,
                               int *nz_total,  double *bscale, double *bzero,
                               char *comments, char *errmess,
                               FILE **fp_FITS_cube)
{
int bitpix = 0, status;

status = open_input_fits_cube1(filename, nx, ny, nz_total, &bitpix,
                               bscale, bzero, comments, errmess, fp_FITS_cube);
if(!status) {
   if (bitpix != 8) {
        sprintf(errmess,"bitpix=%d : bits per pixel is not 8, can't read here!",
                bitpix);
        fclose(*fp_FITS_cube);
        return(-1);
        }
  }

return(status);
}
/******************************************************************************
* Open a AWORD or BYTE FITS 3D cube
*
* bitpix = 8, 16, 32 or -32;
* Here 8, 16 bits or float images
*
* INPUT:
*  filename
*
* OUTPUT:
*  nx, ny, nz_total, comments, err_mess
*  fp_FITS_cube: pointer to the FITS cube file
******************************************************************************/
int open_input_fits_cube1(char *filename, int *nx, int *ny, int *nz_total,
                          int *bitpix, double *bscale, double *bzero,
                          char *comments, char *errmess,
                          FILE **fp_FITS_cube)
{
int imax, ok;

/* read FITS file
*/
*fp_FITS_cube = fopen(filename,"rb");
if (*fp_FITS_cube == NULL)
        {
        sprintf(errmess,"can't open file >%s< !",filename);
        return(-1);
        }

ok = fits_readhead(*fp_FITS_cube, nx, ny, nz_total, bscale, bzero, bitpix);
if (ok == -2)
        {
        sprintf(errmess,"file is not in simple format, can't read!");
        fclose(*fp_FITS_cube);
        return(-1);
        }
if (*nz_total < 1)
        {
        sprintf(errmess,"file has less than 3 axes, can't read here!");
        fclose(*fp_FITS_cube);
        return(-1);
        }
if ((*bitpix != 8) && (*bitpix != 16) && (*bitpix != -32))
        {
        sprintf(errmess,"bitpix=%d : bits per pixel is neither 8, 16 nor -32 ! Can't read here!",
                *bitpix);
        fclose(*fp_FITS_cube);
        return(-1);
        }
imax = (*nx) * (*ny);
if(imax > 100000000)
        {
        sprintf(errmess,"Frames are too large: nx=%d ny=%d (max= 10000*10000)",
                *nx, *ny);
        fclose(*fp_FITS_cube);
        return(-1);
        }

return(0);
}
/******************************************************************************
* Read BYTE data from FITS 3D cube
*
* INPUT:
*  fp_FITS_cube: pointer to the FITS cube file
*  nz_cube: number of frames to be read from the 3D input image
*  bitpix: number of bits per pixel in input FITS file
*
* OUTPUT:
*  image: 3D data to be written to output file
*
* INPUT/OUTPUT:
*  current_file_nz
******************************************************************************/
int read_byte_from_input_fits_cube(FILE *fp_FITS_cube, BYTE *image,
                                   int nx, int ny, int nz_cube, int bitpix,
                                   double bscale, double bzero, int *current_file_nz,
                                   int total_file_nz, int *all_read)
{
int nz_max, imax, status;
register int i;
float fval, *flt_image;
char errmess[256];
VI2 buf2;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

nz_max = total_file_nz - *current_file_nz;
if(nz_max < nz_cube) {
 *all_read = 1;
 return(10);
 } else {
 *all_read = 0;
 }

imax = nx * ny * nz_cube;
// If bitpix = 8:
if(bitpix == 8) {
  for (i = 0; i < imax; i++) {
    fval = (float)getc(fp_FITS_cube);
    image[i] = (BYTE)(fval * bscale + bzero);
  }
} else if(bitpix == 16){
// If bitpix = 16:
  for (i = 0; i < imax; i++) {
    buf2.c[0] = getc(fp_FITS_cube);
    buf2.c[1] = getc(fp_FITS_cube);
// Swap bytes if needed:
    cvi2(&buf2, 1, 0);
// Copy AWORD value to image array:
    fval = (float)buf2.i;
    image[i] = (BYTE)(fval * bscale + bzero);
  }
// bitpix = -32:
} else {
    flt_image = new float[imax];
    status = read_float_data_from_FITS_file(fp_FITS_cube, flt_image,
                                            imax, errmess);
    if(status) {
      fclose(fp_FITS_cube);
      delete[] flt_image;
      return(-1);
    }
// normalized conversion is not necessary for Andor...
//  float_to_byte_normalized_conversion(flt_image, image, imax);
// So I simply convert it to float with "cast"
  for (i = 0; i < imax; i++) {
    image[i] = (BYTE)(flt_image[i] * bscale + bzero);
    }

  delete[] flt_image;
}

// Fill complement with zeroes:
for (i = imax; i < nx * ny * nz_cube; i++) image[i] = 0.;

*current_file_nz += nz_cube;
if(*current_file_nz == total_file_nz) {
 *all_read = 1;
}

return(0);
}
/******************************************************************************
* Read AWORD data from FITS 3D cube
*
* INPUT:
*  fp_FITS_cube: pointer to the FITS cube file
*  nz_cube: number of frames to be read from the 3D input image
*
* OUTPUT:
*  image: 3D data to be written to output file
*
* INPUT/OUTPUT:
*  current_file_nz
******************************************************************************/
int read_aword_from_input_fits_cube(FILE *fp_FITS_cube, AWORD *image,
                                   int nx, int ny, int nz_cube, int bitpix,
                                   double bscale, double bzero, int *current_file_nz,
                                   int total_file_nz, int *all_read)
{
int nz_max, imax, status;
register int i;
float fval, *flt_image;
char errmess[256];
UI2 buf2;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

nz_max = total_file_nz - *current_file_nz;
if(nz_max < nz_cube) {
 *all_read = 1;
 return(10);
 } else {
 *all_read = 0;
 }

imax = nx * ny * nz_cube;
// If bitpix = 8:
if(bitpix == 8) {
  for (i = 0; i < imax; i++) {
    fval = (float)getc(fp_FITS_cube);
    image[i] = (AWORD)(fval * bscale + bzero);
  }
} else if(bitpix == 16){
// If bitpix = 16:
  for (i = 0; i < imax; i++) {
    buf2.c[0] = getc(fp_FITS_cube);
    buf2.c[1] = getc(fp_FITS_cube);
// Swap bytes if needed:
    cui2(&buf2, 1, 0);
// Copy AWORD value to image array:
    fval = (float)buf2.i;
    image[i] = (AWORD)(fval * bscale + bzero);
  }
// bitpix = -32:
} else {
    flt_image = new float[imax];
    status = read_float_data_from_FITS_file(fp_FITS_cube, flt_image,
                                            imax, errmess);
    if(status) {
      fclose(fp_FITS_cube);
      delete[] flt_image;
      return(-1);
    }
// normalized conversion is not necessary for Andor...
//  float_to_aword_normalized_conversion(flt_image, image, imax);
// So I simply convert it to float with "cast"
  for (i = 0; i < imax; i++) {
    image[i] = (AWORD)(flt_image[i] * bscale + bzero);
    }
  delete[] flt_image;
}


// Fill complement with zeroes:
for (i = imax; i < nx * ny * nz_cube; i++) image[i] = 0.;

*current_file_nz += nz_cube;

if(*current_file_nz == total_file_nz) {
 *all_read = 1;
}

return(0);
}
/******************************************************************************
* Read float data from FITS 3D cube
*
* INPUT:
*  fp_FITS_cube: pointer to the FITS cube file
*  nz_cube: number of frames to be read from the 3D input image
*
* OUTPUT:
*  image: 3D data to be written to output file
*
* INPUT/OUTPUT:
*  current_file_nz
******************************************************************************/
int read_float_from_input_fits_cube(FILE *fp_FITS_cube, float *image,
                                   int nx, int ny, int nz_cube, int bitpix,
                                   double bscale, double bzero, int *current_file_nz,
                                   int total_file_nz, int *all_read)
{
int nz_max, imax, status;
float fval;
register int i;
char errmess[256];
VI2 buf2;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

nz_max = total_file_nz - *current_file_nz;
if(nz_max < nz_cube) {
 *all_read = 1;
 return(10);
 } else {
 *all_read = 0;
 }

imax = nx * ny * nz_cube;

// If bitpix = 8:
if(bitpix == 8) {
  for (i = 0; i < imax; i++) {
    fval = (float)getc(fp_FITS_cube);
    image[i] = fval * bscale + bzero;
  }
} else if(bitpix == 16){
// If bitpix = 16:
  for (i = 0; i < imax; i++) {
    buf2.c[0] = getc(fp_FITS_cube);
    buf2.c[1] = getc(fp_FITS_cube);
// Swap bytes if needed:
    cvi2(&buf2, 1, 0);
// Copy int value to image array:
    fval = (float)buf2.i;
    image[i] = fval * bscale + bzero;
  }
// bitpix = -32:
} else {
    status = read_float_data_from_FITS_file(fp_FITS_cube, image,
                                            imax, errmess);
    if(status) {
      fclose(fp_FITS_cube);
      return(-1);
    }
  for (i = 0; i < imax; i++) image[i] = image[i] * bscale + bzero;
}

// Fill complement with zeroes:
for (i = imax; i < nx * ny * nz_cube; i++) image[i] = 0.;

*current_file_nz += nz_cube;
if(*current_file_nz == total_file_nz) {
 *all_read = 1;
}

return(0);
}
