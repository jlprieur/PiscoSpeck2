/******************************************************************************
* Set of routines to read/write 3D data from/to FITS format files
*
* JLP Version
* Version 02/01/2015
******************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "jlp_time0.h"
#include "jlp_rw_fits0.h"
#include "asp2_3D_outfits.h"

/*************************************************************************
* Copy current 3D temporary file to a final file on disk
*
* INPUT:
*  bitpix: number of bits per pixel
*************************************************************************/
int Copy_outFITS_3DFile_to_3DFITSFile(JLP_3D_outFITSFile *outFITSFile,
                                   char *generic_name, char *directory,
                                   char *comments, DESCRIPTORS descrip,
                                   PROCESS_SETTINGS Pset,
                                   int bitpix, char *errmess)
{
FILE *fp_in, *fp_out;
AWORD *u_work;
BYTE *b_work;
int status, nval, nval_to_read, nx1, ny1, nz1_total, n_planes;
long size_of_header1;
char filename[256];

// Only 8 or 16 bits per pixel:
if(bitpix != 8 && bitpix != 16) {
  wxMessageBox(wxT("Error:only 8 or 16 bit/pixels allowed here!"),
               wxT("CopyAndSaveFrames_to_3DFITSFile"), wxICON_ERROR);
  return(-3);
  }

// Error if file is closed:
if(outFITSFile == NULL) {
  sprintf(errmess,"CopyAndSaveFrames_to_3DFITSFile/Error: input file pointer is NULL!");
  return(-1);
  }

fp_in = outFITSFile->fp();
nx1 = outFITSFile->Nx();
ny1 = outFITSFile->Ny();
nz1_total = outFITSFile->Nz_Current();

// Go to beginning of file, and skip the header:
size_of_header1 = outFITSFile->SizeOfHeader();
status = fseek(fp_in, size_of_header1, SEEK_SET);
if(status) {
  sprintf(errmess,"CopyAndSaveFrames_to_3DFITSFile/fseek/Error: size=%ld status=%d",
          size_of_header1, status);
  return(-2);
  }

// Generate filename:
if(!strcmp(directory, ".") || directory[0] == '\0') {
 sprintf(filename, "%s_cube.fits", generic_name);
} else {
#ifdef LINUX
 sprintf(filename, "%s/%s_cube.fits", directory, generic_name);
#else
 sprintf(filename, "%s\\%s_cube.fits", directory, generic_name);
#endif
}

// Open new file:
fp_out = fopen(filename,"wb");
if(fp_out == NULL) {
  sprintf(errmess,"CopyAndSaveFrames_to_3DFITSFile/Error: can't open new file >%s< !",filename);
  fclose(fp_in);
  return(-3);
  }

// Write the header to the new file:
status = write_header_output_fits_cube(fp_out, nx1, ny1, nz1_total, bitpix,
                                       comments, descrip, Pset,
                                       errmess);
if(status) {
  wxMessageBox(wxT("write_header_output_fits_cube/Error"),
               wxT("CopyAndSaveFrames_to_3DFITSFile"), wxICON_ERROR);
  fclose(fp_in);
  fclose(fp_out);
  return(-4);
  }

 nval_to_read = nx1 * ny1;

// Copy next data:
// Case when bitpix == 8:
if(bitpix == 8) {

 b_work = new BYTE[nval_to_read];

// Copy all data:
n_planes = 0;
while(!feof(fp_in)) {
 nval = fread(b_work, sizeof(BYTE), nval_to_read, fp_in);
 fwrite(b_work, sizeof(BYTE), nval, fp_out);
 n_planes++;
 if(nval < nval_to_read) break;
 }

delete[] b_work;

// Copy next data:
// Case when bitpix == 16:
} else if(bitpix == 16) {
 u_work = new AWORD[nval_to_read];

// Copy all data:
n_planes = 0;
while(!feof(fp_in)) {
 nval = fread(u_work, sizeof(AWORD), nval_to_read, fp_in);
 fwrite(u_work, sizeof(AWORD), nval, fp_out);
 n_planes++;
 if(nval < nval_to_read) break;
 }
delete[] u_work;
}

if(n_planes < nz1_total) {
  sprintf(errmess,"CopyAndSaveFrames_to_3DFITSFile/Error: size=%ld nx_ny=%d nval=%d n_planes=%d < nz1_total=%d!",
          size_of_header1, nval_to_read, nval, n_planes, nz1_total);
  status = -5;
  }

fseek(fp_in, 0L, SEEK_END);

fclose(fp_out);
return(status);
}
/*************************************************************************
* Copy current 3D temporary file to a individual 2D FITS files on disk
*************************************************************************/
int Copy_outFITS_3DFile_to_2DFITSFiles(JLP_3D_outFITSFile *outFITSFile,
                                   char *generic_name, char *directory,
                                   char *comments, DESCRIPTORS descrip,
                                   PROCESS_SETTINGS Pset,
                                   int bitpix, char *errmess)
{
FILE *fp_out, *fp_in;
AWORD *u_work;
BYTE *b_work;
double bzero, bscale;
int status, nval, nval_to_read, nx1, ny1, nz1_total, byte_writ;
register int kk, i;
long size_of_header1;
char filename[256], ffname[256], buf[80];
bool all_read = false;

// Only 8 or 16 bits per pixel:
if(bitpix != 8 && bitpix != 16) {
  sprintf(errmess, "CopyAndSaveFrames_to_2DFITSFiles/Error: bitpix=%d (!= 8 or 16)!",
  bitpix);
  return(-3);
  }

// Error if file is closed:
if(outFITSFile == NULL) {
  sprintf(errmess,"CopyAndSaveFrames_to_2DFITSFiles/Error: input file pointer is NULL!");
  return(-1);
  }

fp_in = outFITSFile->fp();
nx1 = outFITSFile->Nx();
ny1 = outFITSFile->Ny();
nz1_total = outFITSFile->Nz_Current();

// Go to beginning of file, and skip the header:
size_of_header1 = outFITSFile->SizeOfHeader();
status = fseek(fp_in, size_of_header1, SEEK_SET);
if(status) {
  sprintf(errmess,"CopyAndSaveFrames_to_2DFITSFiles/fseek/Error: size=%ld status=%d",
          size_of_header1, status);
  return(-2);
  }

/* Define bzero and bscale
*/
bzero = 0.;
bscale = 1.;

nval_to_read = nx1 * ny1;
b_work = NULL;
u_work = NULL;
if(bitpix == 8) {
 b_work = new BYTE[nval_to_read];
} else if(bitpix == 16) {
 u_work = new AWORD[nval_to_read];
}

// Generate filename:
if(!strcmp(directory, ".") || directory[0] == '\0') {
 sprintf(filename, "%s", generic_name);
} else {
#ifdef LINUX
 sprintf(filename, "%s/%s", directory, generic_name);
#else
 sprintf(filename, "%s\\%s", directory, generic_name);
#endif
}

// Open new file:
all_read = false;
for(kk = 0; kk < nz1_total && !all_read && !feof(fp_in); kk++) {

sprintf(ffname, "%s%04d.fits", filename, kk);
fp_out = fopen(ffname,"wb");
if(fp_out == NULL) {
  sprintf(errmess,"CopyAndSaveFrames_to_2DFITSFiles/Error: can't open new file >%s< !",
          ffname);
  fclose(fp_in);
  return(-3);
  }

// Write the header to the new file:
status = fits_writhead(fp_out, nx1, ny1, 0, bscale, bzero, bitpix,
                       comments, descrip, Pset);

if(status) {
  sprintf(errmess, "CopyAndSaveFrames_to_2DFITSFiles/Error in fits_writhead status=%d",
          status);
  fclose(fp_in);
  fclose(fp_out);
  return(-4);
  }

// Copy next data:
// Case when bitpix == 8:
if(bitpix == 8) {

 nval = fread(b_work, sizeof(BYTE), nval_to_read, fp_in);
 fwrite(b_work, sizeof(BYTE), nval, fp_out);
 byte_writ = nval;
 if(nval < nval_to_read) {
    all_read = true;
    break;
    }

// Case when bitpix == 16:
} else if(bitpix == 16) {

 nval = fread(u_work, sizeof(AWORD), nval_to_read, fp_in);
 fwrite(u_work, sizeof(AWORD), nval, fp_out);
 byte_writ = nval * 2;
 if(nval < nval_to_read) {
    all_read = true;
    break;
    }
}

// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (i = 0; i < 80; i++) buf[i] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf, sizeof(char), 1, fp_out);
  byte_writ++;
  }

fclose(fp_out);
}  // EOF loop on kk

if(kk < nz1_total) {
  sprintf(errmess,"CopyAndSaveFrames_to_2DFITSFiles/Error: nx_ny=%d nval=%d n_planes=%d < nz1_total=%d!",
          nval_to_read, nval, kk, nz1_total);
  status = -5;
  }

fseek(fp_in, 0L, SEEK_END);

if(b_work != NULL) delete[] b_work;
if(u_work != NULL) delete[] u_work;

return(status);
}
/******************************************************************************
* Open a FITS 3D cube for output
******************************************************************************/
int open_output_fits_cube(char *filename, FILE **fp_FITS_cube, char *errmess)
{
/* Open output FITS file
* JLP2009: I add + to allow re-reading it !
*/
*fp_FITS_cube = fopen(filename,"wb+");
if (*fp_FITS_cube == NULL)
        {
        sprintf(errmess,"can't create file >%s< !",filename);
        return(-1);
        }
return(0);
}
/******************************************************************************
* Write the header of a FITS 3D cube
*
* bitpix = 8, 16 or 32;
* Here 8 bits
* and BYTE images
*
* INPUT:
*  total_file_nz: total number of images contained in this file
*
* OUTPUT:
*  fp_FITS_cube: pointer to the FITS cube file
******************************************************************************/
int write_header_output_fits_cube(FILE *fp_FITS_cube, int nx, int ny,
                                 int total_file_nz, int bitpix,
                                 char *comments, DESCRIPTORS descrip,
                                 PROCESS_SETTINGS Pset,
                                 char *errmess)
{
int     status;
double  bscale, bzero;

/* Define bzero and bscale
*/
bzero = 0.;
bscale = 1.;

status = fits_writhead(fp_FITS_cube,nx,ny,total_file_nz,bscale,bzero,bitpix,
                       comments, descrip, Pset);

if(status)
        {
        fprintf(stderr,"write_header_output_fits_cube/Error writing FITS header to file \n");
        sprintf(errmess,"fits_writhead/Error writing FITS header: status=%d",
                status);
        fclose(fp_FITS_cube);
        return(-1);
        }

return(0);
}
/******************************************************************************
* Write byte data cube to a FITS 3D file
*
* bitpix = 8, 16 or 32;
* Here 8 bits
* and BYTE images
*
* INPUT:
*  image: 3D data to be written to output 3D FITS file
*  fp_FITS_cube: pointer to the 3D FITS file
*  nz_cube: number of frames contained in 3D input image
*
* INPUT/OUTPUT:
*  current_file_nz
******************************************************************************/
int WriteByteCube_to_output_fits_cube(FILE *fp_FITS_cube, BYTE *image,
                                      int nx, int ny, int nz_cube,
                                      int *current_file_nz, int total_file_nz,
                                      int *ifull)
{
register int i;
int nz_max, nz_allowed, itotal;

// Maximum space left, in order to fill the file:
nz_max = total_file_nz - (*current_file_nz);
if(nz_max < 0) nz_max = 0;
if(nz_max > nz_cube) {
 nz_allowed = nz_cube;
 *ifull = 0;
 } else {
 nz_allowed = nz_max;
 *ifull = 1;
 }

// Total size of input image:
itotal = nx * ny * nz_allowed;
for (i = 0; i < itotal; i++) putc(image[i], fp_FITS_cube);
*current_file_nz += nz_allowed;

return(0);
}
/******************************************************************************
* Write aword data to a byte FITS 3D cube
*
* bitpix = 8, 16 or 32;
* Here 8 bits
* and BYTE images
*
* INPUT:
*  image: 3D data cube to be written to output 3D FITS file
*  fp_FITS_cube: pointer to the 3D FITS file
*  nz_cube: number of frames contained in 3D input image
*
* INPUT/OUTPUT:
*  current_file_nz
******************************************************************************/
int WriteAwordCube_to_output_fits_cube(FILE *fp_FITS_cube, AWORD *image,
                                       int nx, int ny, int nz_cube,
                                       int *current_file_nz, int total_file_nz,
                                       int *ifull)
{
register int i;
int nz_max, nz_allowed, itotal;
UI2 buf2;

// Read "asp2_computer.h" and initialize conversion rules
cvinit();

// Maximum space left, in order to fill the file:
nz_max = total_file_nz - (*current_file_nz);
if(nz_max < 0) nz_max = 0;
if(nz_max > nz_cube) {
 nz_allowed = nz_cube;
 *ifull = 0;
 } else {
 nz_allowed = nz_max;
 *ifull = 1;
 }

// Total size of input image:
itotal = nx * ny * nz_allowed;

for (i = 0; i < itotal; i++) {
// Copy input image value to AWORD buffer:
    buf2.i = image[i];
// Swap bytes if needed:
    cui2(&buf2, 1, 0);
    putc(buf2.c[0],fp_FITS_cube);
    putc(buf2.c[1],fp_FITS_cube);
    }

*current_file_nz += nz_allowed;

return(0);
}
/******************************************************************************
* Close byte FITS 3D cube
*
* bitpix = 8, 16 or 32;
* Here 8 bits (i.e. BYTE) images
*
* INPUT:
*  fp_FITS_cube: pointer to the FITS cube file
*  nx, ny: size of 2D elementary frames
*  current_file_nz: actual number of frames written to 3D FITS file
*               (may be different from naxis3 that was written into the header)
******************************************************************************/
int close_output_byte_fits_cube(FILE *fp_FITS_cube, int nx, int ny,
                                int current_file_nz)
{
char    buf[8];
int     c, byte_writ;

byte_writ = nx * ny * current_file_nz;

// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (c=0; c<8; c++) buf[c] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),1,fp_FITS_cube);
  byte_writ ++;
  }

fclose(fp_FITS_cube);

return(0);
}
/******************************************************************************
* Close AWORD FITS 3D cube
*
* bitpix = 8, 16 or 32;
* Here 16 bits (i.e. AWORD) images
*
* INPUT:
*  fp_FITS_cube: pointer to the FITS cube file
*  nx, ny: size of 2D elementary frames
*  current_file_nz: actual number of frames written to 3D FITS file
*               (may be different from naxis3 that was written into the header)
******************************************************************************/
int close_output_aword_fits_cube(FILE *fp_FITS_cube, int nx, int ny,
                                 int current_file_nz)
{
char    buf[8];
int     c, byte_writ;

byte_writ = 2 * nx * ny * current_file_nz;

// Fill last 2880-byte block with blanks
byte_writ = byte_writ % 2880;
for (c=0; c<8; c++) buf[c] = ' ';
while (byte_writ != 2880)
  {
  fwrite(buf,sizeof(char),1,fp_FITS_cube);
  byte_writ ++;
  }

fclose(fp_FITS_cube);

return(0);
}
