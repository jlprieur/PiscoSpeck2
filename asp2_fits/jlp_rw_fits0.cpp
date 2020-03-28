/******************************************************************************
* jlp_rw_fits0.cpp
* Set of routines to read/write in FITS format with 8-bits integers
* initially for integer 2-D images from T. Oosterloo
* but deeply modified to 3-D and floating-point images
*
* JLP Version
* Version 07/08/2015
******************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "jlp_rw_fits0.h"
#include "jlp_time0.h"

// Defined here:
static int fits_readpxl_int(FILE *fp, int bitpix, long *pixval);
static int fits_writpxl_int(FILE *fp, int bitpix, long pixval);

// Define output format by default as float:
#define FITS_FLOAT

/******************************************************************************
* Read float 2D image
******************************************************************************/
int readimag_float_fits(char *filename, float **image1,
                        int *nx, int *ny, char *comments, char *errmess)
{
register int     i;
int     imax, bitpix, ok, nz, status;
FILE*   fp;
double  bscale, bzero;
long val;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* read FITS file
*/
fp = fopen(filename,"rb");
if (fp == NULL)
        {
        sprintf(errmess,"can't open file >%s< !",filename);
        return(-1);
        }

ok = fits_readhead(fp,nx,ny,&nz,&bscale,&bzero,&bitpix);
if (ok == -2)
        {
        sprintf(errmess,"file is not in simple format, can't read!");
        fclose(fp);
        return(-2);
        }
if (*nx == 1 || *ny == 1)
        {
        sprintf(errmess,"file is not 2D: nx=%d ny=%d!", *nx, *ny);
        fclose(fp);
        return(-3);
        }
if (nz > 1)
        {
        sprintf(errmess,"file has 3 axes, can't read here!");
        fclose(fp);
        return(-3);
        }
if (ok == -4)
        {
        sprintf(errmess, "bitpix=%d bits per pixel is not 8,\
 16, 32 or -32, can't read!", bitpix);
        fclose(fp);
        return(-4);
        }

imax = (*nx) * (*ny);

*image1 = new float[imax];

if (bitpix == -32)
  {
    status = read_float_data_from_FITS_file(fp, *image1, imax, errmess);
    if(status) {
      fclose(fp);
      return(-1);
      }
  }
else
  {
  for (i = 0; i < imax; i++)
    {
    ok = fits_readpxl_int(fp, bitpix, &val);
    if (ok < 0)
         {
         sprintf(errmess,"I/O error, can't read!");
         fclose(fp);
         return(-1);
         }
    (*image1)[i] = (float)val * bscale + bzero;
    }
  }

fclose(fp);

return(0);
}
/***************************************************************************
*  Read data fromFITS file when bitpix == -32
***************************************************************************/
int read_float_data_from_FITS_file(FILE *fp, float *image, int npixels,
                                   char *errmess)
{
int byte_to_read, nval, i;
float *fwork;

// Read "asp2_computer.h" and initialize conversion rules
  cvinit();

  byte_to_read = 4 * npixels;
  fwork = new float[npixels];
  nval = fread(fwork, sizeof(char), byte_to_read, fp);
  if(nval != byte_to_read) {
    sprintf(errmess, "bitpix=-32; Error, could only read %d out of %d pixels (feof=%d)!",
            nval/4, npixels, feof(fp));
    delete[] fwork;
    return(-1);
    }

// Conversion from FITS format to computer format
// (in cvr4, no difference between "to" and "from")
  cvr4(fwork, npixels);
  for (i = 0; i < npixels; i++) image[i] = fwork[i];
  delete[] fwork;
return(0);
}
/******************************************************************************
* Read a byte 2D image from a byte FITS file
******************************************************************************/
int readimag_byte_fits(char *filename, BYTE **image,
                       int *nx, int *ny, char *comments, char *errmess)
{
register int     i;
FILE*   fp;
int     imax, bitpix, ok, nz;
double bscale, bzero;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* read FITS file
*/
fp = fopen(filename,"rb");
if (fp == NULL)
        {
        sprintf(errmess,"can't open file >%s< !",filename);
        return(-1);
        }

ok = fits_readhead(fp,nx,ny,&nz,&bscale,&bzero,&bitpix);
if (ok == -2)
        {
        sprintf(errmess,"file is not in simple format, can't read!");
        fclose(fp);
        return(-2);
        }
if (nz > 1)
        {
        sprintf(errmess,"file has 3 axes, can't read here!");
        fclose(fp);
        return(-3);
        }
if (bitpix != 8)
        {
        sprintf(errmess,"bits per pixel is not 8, can't read here!");
        fclose(fp);
        return(-4);
        }

imax = (*nx) * (*ny);
*image = new BYTE [imax];

for (i = 0; i < imax; i++)
  {
  *image[i] = (BYTE)getc(fp);
  }

fclose(fp);

return(0);
}
/******************************************************************************
* Read AWORD 2D image from a FITS file
******************************************************************************/
int readimag_aword_fits(char *filename, AWORD **image,
                        int *nx, int *ny, char *comments, char *errmess)
{
register int i;
FILE*   fp;
float *flt_image;
int     imax, bitpix, ok, nz, status;
UI2 buf2;
double bscale, bzero;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* read FITS file
*/
fp = fopen(filename,"rb");
if (fp == NULL)
        {
        sprintf(errmess,"can't open file >%s< !",filename);
        return(-1);
        }

ok = fits_readhead(fp, nx, ny, &nz, &bscale, &bzero, &bitpix);
if (ok == -2)
        {
        sprintf(errmess,"file is not in simple format, can't read!");
        fclose(fp);
        return(-2);
        }
if (nz > 1)
        {
        sprintf(errmess,"file has 3 axes, can't read here!");
        fclose(fp);
        return(-3);
        }
if ((bitpix != 8) && (bitpix != 16) && (bitpix != -32))
        {
        sprintf(errmess,"bitpix=%d : bits per pixel is not 8, 16 or -32,\
 can't read here!", bitpix);
        fclose(fp);
        return(-4);
        }

imax = (*nx) * (*ny);
*image = new AWORD[imax];

// If bitpix = 8:
if(bitpix == 8) {
  for (i = 0; i < imax; i++) *image[i] = (AWORD)getc(fp);
} else if (bitpix == 16) {
// If bitpix = 16:
  for (i = 0; i < imax; i++) {
    buf2.c[0] = getc(fp);
    buf2.c[1] = getc(fp);
// Swap bytes if needed:
    cui2(&buf2, 1, 0);
// Copy AWORD value to image array:
    (*image)[i] = buf2.i;
  }
// bitpix = -32:
} else {
    flt_image = new float[imax];
    status = read_float_data_from_FITS_file(fp, flt_image, imax, errmess);
    if(status) {
      fclose(fp);
      delete[] flt_image;
      return(-1);
    }
// normalized conversion is not necessary for Andor...
//  float_to_aword_normalized_conversion(flt_image, *image, imax);
// So I simply convert it to AWORD with "cast"
  for (i = 0; i < imax; i++) {
    *image[i] = (AWORD)flt_image[i];
    }

  delete[] flt_image;
}

fclose(fp);

return(0);
}
/******************************************************************************
* Read data contained in byte FITS 3D cube
******************************************************************************/
int read_smallcube_byte_fits(char *filename, BYTE **image,
                             int *nx, int *ny, int *nz, char *comments,
                             char *errmess)
{
register int     i;
FILE*   fp;
int     imax, bitpix, ok;
double bscale, bzero;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* read FITS file
*/
fp = fopen(filename,"rb");
if (fp == NULL)
        {
        sprintf(errmess,"can't open file >%s< !",filename);
        return(-1);
        }

ok = fits_readhead(fp,nx,ny,nz,&bscale,&bzero,&bitpix);
if (ok == -2)
        {
        sprintf(errmess,"file is not in simple format, can't read!");
        fclose(fp);
        return(-2);
        }
if (*nz < 1)
        {
        sprintf(errmess,"file has less than 3 axes, can't read here!");
        fclose(fp);
        return(-3);
        }
if (bitpix != 8)
        {
        sprintf(errmess,"bits per pixel is not 8, can't read here!");
        fclose(fp);
        return(-4);
        }

imax = (*nx) * (*ny) * (*nz);
if(imax > 1000000 || *nz > 500)
        {
        sprintf(errmess,"Cube is too large: nx=%d ny=%d nz=%d (nz_max = 500)",
                *nx, *ny, *nz);
        fclose(fp);
        return(-5);
        }

*image = new BYTE [imax];

for (i = 0; i < imax; i++)
  {
  (*image)[i] = (BYTE)getc(fp);
  }

fclose(fp);

return(0);
}
/******************************************************************************
* Scale float image for good AWORD conversion ???
******************************************************************************/
int float_to_aword_normalized_conversion(float *f_image, AWORD *u_image, int npixels)
{
float min1, max1, ww;
register int i;

  min1 = f_image[0];
  max1 = f_image[0];
  for (i = 0; i < npixels; i++) {
   min1 = MINI(min1, f_image[i]);
   max1 = MAXI(min1, f_image[i]);
   }
  if(max1 == min1) max1 = min1 + 1.;

// 65535 = 2^{16} - 1
  for (i = 0; i < npixels; i++) {
    ww = 65535. * (f_image[i] - min1) / (max1 - min1);
    u_image[i] = (AWORD)ww;
    }

return(0);
}
/******************************************************************************
* Scale float image for good BYTE conversion
******************************************************************************/
int float_to_byte_normalized_conversion(float *f_image, BYTE *u_image, int npixels)
{
float min1, max1, ww;
register int i;

  min1 = f_image[0];
  max1 = f_image[0];
  for (i = 0; i < npixels; i++) {
   min1 = MINI(min1, f_image[i]);
   max1 = MAXI(min1, f_image[i]);
   }
  if(max1 == min1) max1 = min1 + 1.;

// 255 = 2^{8} - 1
  for (i = 0; i < npixels; i++) {
    ww = 65535. * (f_image[i] - min1) / (max1 - min1);
    u_image[i] = (BYTE)ww;
    }

return(0);
}
/******************************************************************************
* Read data contained in BYTE or AWORD FITS 3D cube
******************************************************************************/
int read_smallcube_fits1(char *filename, AWORD **image,
                         int *nx, int *ny, int *nz, char *comments,
                         char *errmess)
{
register int     i;
FILE*   fp;
int     imax, bitpix, ok, status;
double bscale, bzero;
float *flt_image;
UI2 buf2;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* read FITS file
*/
fp = fopen(filename,"rb");
if (fp == NULL)
        {
        sprintf(errmess,"can't open file >%s< !",filename);
        return(-1);
        }

ok = fits_readhead(fp,nx,ny,nz,&bscale,&bzero,&bitpix);
if (ok == -2)
        {
        sprintf(errmess,"file is not in simple format, can't read!");
        fclose(fp);
        return(-2);
        }
if (*nz < 1)
        {
        sprintf(errmess,"file has less than 3 axes, can't read here!");
        fclose(fp);
        return(-3);
        }
if ((bitpix != 8) && (bitpix != 16))
        {
        sprintf(errmess,"bitpix=%d bits per pixel is not 8 or 16,\
 can't read here!", bitpix);
        fclose(fp);
        return(-4);
        }

imax = (*nx) * (*ny) * (*nz);
if(imax > 1000000 || *nz > 500)
        {
        sprintf(errmess,"Cube is too large: nx=%d ny=%d nz=%d (nz_max = 500)",
                *nx, *ny, *nz);
        fclose(fp);
        return(-5);
        }

*image = new AWORD [imax];

// If bitpix = 8:
if(bitpix == 8) {
  for (i = 0; i < imax; i++) *image[i] = (AWORD)getc(fp);
} else if (bitpix == 16) {
// If bitpix = 16:
  for (i = 0; i < imax; i++) {
    buf2.c[0] = getc(fp);
    buf2.c[1] = getc(fp);
// Swap bytes if needed:
    cui2(&buf2, 1, 0);
// Copy AWORD value to image array:
    (*image)[i] = buf2.i;
  }
// bitpix = -32:
} else {
    flt_image = new float[imax];
    status = read_float_data_from_FITS_file(fp, flt_image, imax, errmess);
    if(status) {
      fclose(fp);
      delete[] flt_image;
      return(-1);
    }
// normalized conversion is not necessary for Andor...
//  float_to_aword_normalized_conversion(flt_image, *image, imax);
// So I simply convert it to float with "cast"
  for (i = 0; i < imax; i++) {
    (*image)[i] = (AWORD)flt_image[i];
    }

  delete[] flt_image;
}

fclose(fp);

return(0);
}
/******************************************************************************
* Read FITS header
******************************************************************************/
int fits_readhead(FILE *fp, int *naxis1, int *naxis2, int *naxis3,
                  double *bscale, double *bzero, int *bitpix)
{
char   simple, buf[81];
int    naxis, byte_read = 0, seen_end = 0;

*bscale = 0.;
*bzero = 0.;

*naxis3 = 1;
while (!seen_end)
  {
  if (fread(buf,sizeof(char),80,fp) == 0) return(-1);
  buf[80] = '\0';
  byte_read += 80;

  if (strncmp(buf,"SIMPLE  ",8) == 0) sscanf(buf,"SIMPLE  = %c",&simple);
  else if (strncmp(buf,"NAXIS   ",8) == 0) sscanf(buf,"NAXIS   = %d",&naxis);
  else if (strncmp(buf,"NAXIS1  ",8) == 0) sscanf(buf,"NAXIS1  = %d",naxis1);
  else if (strncmp(buf,"NAXIS2  ",8) == 0) sscanf(buf,"NAXIS2  = %d",naxis2);
  else if (strncmp(buf,"NAXIS3  ",8) == 0) sscanf(buf,"NAXIS3  = %d",naxis3);
  else if (strncmp(buf,"BSCALE  ",8) == 0) sscanf(buf,"BSCALE  = %lg",bscale);
  else if (strncmp(buf,"BZERO   ",8) == 0) sscanf(buf,"BZERO   = %lg",bzero);
  else if (strncmp(buf,"BITPIX  ",8) == 0) sscanf(buf,"BITPIX  = %d",bitpix);
  else if (strncmp(buf,"END",3) == 0) seen_end = 1;
  }

if((*bscale == 0.) && (*bzero == 0.)) {
    *bscale = 1.;
    *bzero = 0.;
  }

byte_read = byte_read % 2880;
while (byte_read != 2880) byte_read += fread(buf,sizeof(char),80,fp);

if ((simple != 't') && (simple != 'T')) return(-2);
if (naxis < 1 || naxis > 3) return(-3);

// Check if can be read here:
if ((*bitpix != 8) && (*bitpix != 16) && (*bitpix != 32) && (*bitpix != -32)) return(-4);
return(0);
}

////////////////////////////////////////////////////////////////////////
/// JLP November 2007
////////////////////////////////////////////////////////////////////////
typedef union
{
unsigned char c;
UINT1 s;
}
VINT1;
/******************************************************************************
* Read integer pixel  (WARNING: should call cvinit() first!)
*
* INPUT:
*  fp: File pointer
*  bitpix: number of bits per pixel
*
* OUTPUT:
*  pixval: long integer
******************************************************************************/
static int fits_readpxl_int(FILE *fp, int bitpix, long *pixval)
{
VINT1 val;
VI2 buf2[1];
VI4 buf4[1];

switch (bitpix)
  {
  case 8:
    val.c = (signed char)getc(fp);
    *pixval = (long)val.s;
    break;

  case 16:
    buf2[0].c[0] = getc(fp);
    buf2[0].c[1] = getc(fp);
// Swap bytes if needed:
    cvi2(buf2, 1, 0);
    *pixval = buf2[0].i;
    break;

  case 32:
    buf4[0].c[0] = getc(fp);
    buf4[0].c[1] = getc(fp);
    buf4[0].c[2] = getc(fp);
    buf4[0].c[3] = getc(fp);
    cvi4(buf4, 1, 0);
    *pixval = buf4[0].i;
    break;

  default:
    *pixval = 0;
    return(-1);
  }

return(0);
}
/******************************************************************
* Write data from byte 2D image to FITS file
* bitpix = 8, 16 or 32;
* Here output to 8 bits
*******************************************************************/
int writeimag_byte_fits(char *filename, BYTE *image, int nx, int ny,
                        char *comments, DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess)
{
register int i;
FILE*   fp;
char    buf[81];
int     c, byte_writ, imax, bitpix;
double  bscale, bzero;

/* Define bzero and bscale
*/
bzero = 0;
bscale = 1.;

/* write FITS file
*/
fp = fopen(filename,"wb");
if (fp == NULL)
        {
        sprintf(errmess,"can't create file >%s< !",filename);
        return(-1);
        }
bitpix = 8;
fits_writhead(fp,nx,ny,0,bscale,bzero,bitpix,comments,descrip, Pset);
byte_writ = 0;
imax = nx * ny;
for (i = 0; i < imax; i++)
  {
  putc(image[i],fp);
  byte_writ ++;
  }
// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (c=0; c<80; c++) buf[c] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),1,fp);
  byte_writ ++;
  }
fclose(fp);

return(0);
}
/******************************************************************
* Write data from AWORD 2D image to FITS file
* bitpix = 8, 16 or 32;
* Here output to 16 bits
*******************************************************************/
int writeimag_aword_fits(char *filename, AWORD *image, int nx, int ny,
                        char *comments, DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess)
{
register int i;
FILE*   fp;
char    buf[81];
int     c, byte_writ, imax, bitpix;
double  bscale, bzero;
UI2 buf2;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* Define bzero and bscale
*/
bzero = 0;
bscale = 1.;

/* write FITS file
*/
fp = fopen(filename,"wb");
if (fp == NULL)
        {
        sprintf(errmess,"can't create file >%s< !",filename);
        return(-1);
        }
bitpix = 16;
fits_writhead(fp,nx,ny,0,bscale,bzero,bitpix,comments,descrip, Pset);
byte_writ = 0;
imax = nx * ny;
for (i = 0; i < imax; i++)
  {
// Copy input image value to AWORD buffer:
    buf2.i = image[i];
// Swap bytes if needed:
    cui2(&buf2, 1, 0);
    putc(buf2.c[0],fp);
    byte_writ ++;
    putc(buf2.c[1],fp);
    byte_writ ++;
  }
// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (c=0; c<80; c++) buf[c] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),1,fp);
  byte_writ ++;
  }
fclose(fp);

return(0);
}
/******************************************************************************
* Write 3D BYTE data to FITS 3D cube
* bitpix = 8, 16 or 32;
* Here 8 bits and BYTE images
******************************************************************************/
int write_smallcube_byte_fits(char *filename, BYTE *image,
                        int nx, int ny, int nz, char *comments,
                        DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess)
{
register int     i;
FILE*   fp;
char    buf[81];
int     c, byte_writ, imax, bitpix;
double  bscale, bzero;

/* Define bzero and bscale
*/
bzero = 0.;
bscale = 1.;

/* write FITS file
*/
fp = fopen(filename,"wb");
if (fp == NULL)
        {
        sprintf(errmess,"can't create file >%s< !",filename);
        return(-1);
        }
bitpix = 8;
fits_writhead(fp,nx,ny,nz,bscale,bzero,bitpix,comments,descrip, Pset);

byte_writ = 0;
imax = nx * ny * nz;
for (i = 0; i < imax; i++)
  {
  putc(image[i],fp);
  byte_writ ++;
  }
// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (c=0; c<80; c++) buf[c] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),1,fp);
  byte_writ ++;
  }
fclose(fp);

return(0);
}
/******************************************************************************
* Write 3D AWORD data to FITS 3D cube
* bitpix = 8, 16 or 32;
* Here 16 bits and AWORD images
******************************************************************************/
int write_smallcube_aword_fits(char *filename, AWORD *image,
                        int nx, int ny, int nz, char *comments,
                        DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess)
{
register int     i;
FILE*   fp;
char    buf[81];
int     c, byte_writ, imax, bitpix;
double  bscale, bzero;
UI2 buf2;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* Define bzero and bscale
*/
bzero = 0.;
bscale = 1.;

/* write FITS file
*/
fp = fopen(filename,"wb");
if (fp == NULL)
        {
        sprintf(errmess,"can't create file >%s< !",filename);
        return(-1);
        }
bitpix = 16;
fits_writhead(fp,nx,ny,nz,bscale,bzero,bitpix,comments,descrip,
              Pset);

byte_writ = 0;
imax = nx * ny * nz;
for (i = 0; i < imax; i++)
  {
// Copy input image value to AWORD buffer:
    buf2.i = image[i];
// Swap bytes if needed:
    cui2(&buf2, 1, 0);
    putc(buf2.c[0],fp);
    byte_writ ++;
    putc(buf2.c[1],fp);
    byte_writ ++;
  }
// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (c=0; c<80; c++) buf[c] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),1,fp);
  byte_writ ++;
  }
fclose(fp);

return(0);
}
/****************************************************************
* Write float image to 2D FITS file
* bitpix = 8, 16 or 32;
* Here conversion of float images to long int (32 bits)
* NOT FULLY TESTED YET
*****************************************************************/
int writeimag_float_fits(char *filename, float *image, int nx, int ny,
                         char *comments, DESCRIPTORS descrip,
                         PROCESS_SETTINGS Pset, char *errmess)
{
register int     i;
FILE*   fp;
char    buf[81];
int     c, byte_writ, imax, bitpix;
double  bscale, bzero;
float   mini, maxi, *fwork;
#ifndef FITS_FLOAT
int ok;
#endif

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* compute bzero and bscale
*/
mini = maxi = image[0];
imax = nx * ny;
for (i = 0; i < imax; i++)
  {
  if (image[i] < mini) mini = image[i];
  if (image[i] > maxi) maxi = image[i];
  }

#ifdef FITS_FLOAT
  bitpix = -32;
  bscale = 1.;
  bzero = 0.;
#else
bitpix = 32;
bzero  = 0.5*(maxi+mini);
bscale = 2.0/(maxi-mini)*(pow(2.0,bitpix-1)-1.0);
for (i = 0; i < imax; i++)
  {
  image[i] -= bzero;
  image[i] *= bscale;
  }
bscale = 1.0/bscale;
#endif

/* write FITS file
*/
fp = fopen(filename,"wb");
if (fp == NULL)
        {
        sprintf(errmess,"can't create file >%s< !",filename);
        return(-1);
        }
fits_writhead(fp,nx,ny,0,bscale,bzero,bitpix,comments,descrip,
              Pset);
#ifdef FITS_FLOAT
  cvinit();
  byte_writ = 4 * imax;
  fwork = new float[imax];
  for (i = 0; i < imax; i++) fwork[i] = image[i];
// Conversion from computer format to FITS format
// (in cvr4, no difference between "to" and "from")
  cvr4(fwork, imax);
  fwrite(fwork, sizeof(char), byte_writ, fp);
  delete[] fwork;
#else
byte_writ = 0;
for (i = 0; i < imax; i++)
  {
  val = image[i];
  ok = fits_writpxl_int(fp,bitpix,val);
  if (ok < 0)
           {
           sprintf(errmess,"I/O error, can't write! (val=%f, i=%d)",val,i);
           return(-1);
           }
  byte_writ ++;
  }
#endif
// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (c=0; c<80; c++) buf[c] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),1,fp);
  byte_writ ++;
  }
fclose(fp);

return(0);
}
/****************************************************************
* Write double image to 2D FITS file
* bitpix = 8, 16 or 32;
* Here conversion of double images to long int (32 bits)
*****************************************************************/
int writeimag_double_fits(char *filename, double *image, int nx, int ny,
                         char *comments, DESCRIPTORS descrip,
                         PROCESS_SETTINGS Pset, char *errmess)
{
register int     i;
FILE*   fp;
char    buf[121];
int     c, byte_writ, imax, bitpix;
double  bscale, bzero;
float   mini, maxi, *fwork;
#ifndef FITS_FLOAT
int ok;
long val;
#endif

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

/* compute bzero and bscale
*/
mini = maxi = image[0];
imax = nx * ny;
for (i = 0; i < imax; i++)
  {
  if (image[i] < mini) mini = image[i];
  if (image[i] > maxi) maxi = image[i];
  }

#ifdef FITS_FLOAT
  bitpix = -32;
  bscale = 1.;
  bzero = 0.;
#else
  bitpix = 32;
  bzero  = mini;
  bscale = (maxi-mini)/(pow(2.0,bitpix-1)-1.0);
#endif

/* write FITS file
*/
fp = fopen(filename,"wb");
if (fp == NULL)
        {
        sprintf(errmess,"can't create file >%s< !",filename);
        return(-1);
        }
fits_writhead(fp,nx,ny,0,bscale,bzero,bitpix,comments,descrip, Pset);

#ifdef FITS_FLOAT
  cvinit();
  byte_writ = 4 * imax;
  fwork = new float[imax];
  for (i = 0; i < imax; i++) fwork[i] = image[i];
  cvr4(fwork, imax);
  fwrite(fwork, sizeof(char), byte_writ, fp);
  delete[] fwork;
#else
  byte_writ = 0;
  for (i = 0; i < imax; i++)
   {
   val = (image[i] - bzero) / bscale;
   ok = fits_writpxl_int(fp, bitpix, val);
   if (ok < 0)
           {
           sprintf(errmess,"I/O error, can't write! (val=%f, i=%d)",val,i);
           return(-1);
           }
   byte_writ += ok;
   }
#endif

// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (c=0; c<80; c++) buf[c] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),1,fp);
  byte_writ += 1;
  }
fclose(fp);

return(0);
}

/******************************************************************************
* Write FITS header
*
*
******************************************************************************/
int fits_writhead(FILE *fp, int naxis1, int naxis2, int naxis3,
                  double bscale, double bzero, int bitpix,
                  char* comments, DESCRIPTORS descrip,
                  PROCESS_SETTINGS Pset)
{
char buf[80], *pc, date[80];
int naxis, byte_writ = 0, istat;
register int i;

fprintf(fp,"%-80.80s","SIMPLE  =                    T /Standard FITS format");
byte_writ += 80;

strcpy(buf,"BITPIX  =                      /Bits per pixel");
sprintf(&buf[10],"%20d",bitpix); buf[30] = ' ';
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;

naxis = 1;
if(naxis2 > 1) naxis = 2;
if(naxis3 > 1) naxis = 3;

strcpy(buf,"NAXIS   =                      /Number of axes");
sprintf(&buf[10],"%20d",naxis); buf[30] = ' ';
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;

strcpy(buf,"NAXIS1  =                      /Number of columns");
sprintf(&buf[10],"%20d",naxis1); buf[30] = ' ';
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;

if(naxis >= 2)
{
strcpy(buf,"NAXIS2  =                      /Number of rows");
sprintf(&buf[10],"%20d",naxis2); buf[30] = ' ';
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;
}

if(naxis >= 3)
{
strcpy(buf,"NAXIS3  =                      /Number of planes");
sprintf(&buf[10],"%20d",naxis3); buf[30] = ' ';
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;
}

strcpy(buf,"BSCALE  =                      /REAL = TAPE*BSCALE + BZERO");
sprintf(&buf[10],"%20.10E",bscale); buf[30] = ' ';
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;

strcpy(buf,"BZERO   =                      /REAL = TAPE*BSCALE + BZERO");
sprintf(&buf[10],"%20.10E",bzero); buf[30] = ' ';
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;

// FITS convention:
// ' in column 11, string and ended by '  but not before column 20:
pc = comments;
while(*pc && (*pc != '\n') && (*pc != '\r') && (*pc != '\f')) pc++;
*pc ='\0';
// Do not use COMMENT, since other syntax for FITS format (without "=")
sprintf(buf,"OBJECT  = \'%20.60s\' //", comments);
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;

// Write descriptors:
 for(i = 0; i < descrip.ndescr; i++) {
// Remove possible end of line:
// pc = descrip;
// while(*pc && (*pc != '\n') && (*pc != '\r') && (*pc != '\f')) pc++;
// *pc ='\0';
 sprintf(buf,"%8.8s= \'%20.60s\' //", (descrip.descr[i]).keyword,
         (descrip.descr[i]).cvalue);
 fprintf(fp,"%-80.80s",buf);
 byte_writ += 80;
 }

if(Pset.Selection_maxval)
 {
 sprintf(buf,"MEANCUTS= \'%.1f %.1f\' //", Pset.MaxValSelect.LowCut,
         Pset.MaxValSelect.HighCut);
 fprintf(fp,"%-80.80s",buf);
 byte_writ += 80;
 }

if(Pset.Selection_fwhm)
 {
 sprintf(buf,"FWHMCUTS= \' Low=%.1f High=%.1f\' //", Pset.FwhmSelect.LowCut,
         Pset.FwhmSelect.HighCut);
 fprintf(fp,"%-80.80s",buf);
 byte_writ += 80;
 }

if(*Pset.MaxValSelect.Stats) {
 sprintf(buf,"MAXVALSTAT= \'%.60s\' //",Pset.MaxValSelect.Stats);
 fprintf(fp,"%-80.80s",buf);
 byte_writ += 80;
 }

if(*Pset.FwhmSelect.Stats) {
 sprintf(buf,"FWHMSTAT= \'%.60s\' //",Pset.FwhmSelect.Stats);
 fprintf(fp,"%-80.80s",buf);
 byte_writ += 80;
 }

 sprintf(buf,"KK_CROSS=  %d              /DELAY FOR CROSS-CORRELATION",
         Pset.KK_CrossCorr);
 fprintf(fp,"%-80.80s",buf);
 byte_writ += 80;


// Defined in asp2_utils.cpp
// int jlp_time_microsec(char *full_date, int *istat);
jlp_time_microsec(date,&istat);
date[60] = '\0';
sprintf(buf,"DATE    = \'%.60s\' //", date);
fprintf(fp,"%-80.80s",buf);
byte_writ += 80;

strcpy(buf, "END");
fprintf(fp,"%-80.80s", buf);
byte_writ += 80;

for (i=0; i<80; i++) buf[i] = ' ';
byte_writ = byte_writ % 2880;
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),80,fp);
  byte_writ += 80;
  }

return(0);
}

typedef union
{
unsigned char i1[4];
long int i4;
}
VINT4;
/******************************************************************************
* IBM/PC's swap like a Vax
* LSB, MSB
* 12 => 21 for FITS
* 1234 => 4321 for FITS
* idem for float values
*******************************************************************************/
static int fits_writpxl_int(FILE *fp, int bitpix, long pixval)
{
signed char   ch1;
unsigned char ch2;
VINT4 val;
int  ret;

val.i4 = 0;

switch (bitpix)
  {
  case 8:
    ret = 1;
    val.i4 = pixval;
    ch1 = val.i1[0];       putc(ch1,fp);
    break;

  case 16:
    val.i4 = pixval;
    ret = 2;
    ch1 = val.i1[1];  putc(ch1,fp);
    ch2 = val.i1[0];  putc(ch2,fp);
    break;

  case 32:
    val.i4 = pixval;
    ret = 4;
    ch1 = val.i1[3];
    putc(ch1,fp);
    putc(val.i1[2],fp);
    putc(val.i1[1],fp);
    putc(val.i1[0],fp);
/*
    v.i1[0] = val.i1[3];
    v.i1[1] = val.i1[2];
    v.i1[2] = val.i1[1];
    v.i1[3] = val.i1[0];
    fwrite(&v.i4,sizeof(long),1,fp);
*/
    break;

/* Not used, not tested:
  case -32:
    val.f4 = pixval;
    ret = 4;
    ch1 = val.i1[3];
    putc(ch1,fp);
    putc(val.i1[2],fp);
    putc(val.i1[1],fp);
    putc(val.i1[0],fp);
    break;
*/

  default:
    return(-1);
  }

return(ret);
}


