/**************************************************************************
* jlp_rw_fits0.h
*
* Routines to access to FITS files
* and definition of JLP_3D_inFITSFile, JLP_3D_outFITSFile
* ** fileSpeck2 project **
*
* JLP
* Version 03/01/2011
**************************************************************************/
#ifndef _jlp_rw_fits0_h    /* Sentry */
#define _jlp_rw_fits0_h
#include <stdio.h>
#include <string.h>
#include "asp2_typedef.h" //PROCESS_SETTINGS definition

/***************************************************************************/
// Read/write fits images (rwfits.cpp):
int readimag_float_fits(char *filename, float **image, int *nx, int *ny,
                        char *comments, char *errmess);
int readimag_byte_fits(char *filename, BYTE **image, int *nx, int *ny,
                        char *comments, char *errmess);
int readimag_aword_fits(char *filename, AWORD **image,
                        int *nx, int *ny, char *comments, char *errmess);
int read_smallcube_byte_fits(char *filename, BYTE **image,
                             int *nx, int *ny, int *nz,
                             char *comments, char *errmess);
int read_smallcube_fits1(char *filename, AWORD **image,
                         int *nx, int *ny, int *nz, char *comments,
                         char *errmess);
int writeimag_byte_fits(char *filename, BYTE *image, int nx, int ny,
                        char *comments, DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess);
int writeimag_aword_fits(char *filename, AWORD *image, int nx, int ny,
                        char *comments, DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess);
int write_smallcube_byte_fits(char *filename, BYTE *image,
                        int nx, int ny, int nz,
                        char *comments, DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess);
int write_smallcube_aword_fits(char *filename, AWORD *image,
                        int nx, int ny, int nz, char *comments,
                        DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess);
int writeimag_float_fits(char *filename, float *image, int nx, int ny,
                        char *comments, DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess);
int writeimag_double_fits(char *filename, double *image, int nx, int ny,
                        char *comments, DESCRIPTORS descrip,
                        PROCESS_SETTINGS Pset, char *errmess);
int read_from_input_byte_fits_cube(FILE *fp_FITS_cube, BYTE *image,
                                   int nx, int ny, int nz, int *current_file_nz,
                                   int total_file_nz, int *all_read);
int write_header_output_byte_fits_cube(FILE *fp_FITS_cube, int nx, int ny,
                              int nz_maxi, char *comments, DESCRIPTORS descrip,
                              PROCESS_SETTINGS Pset,
                              char *errmess);
int open_output_byte_fits_cube(char *filename, FILE **fp_FITS_cube,
                               char *errmess);
int write_to_output_byte_fits_cube(FILE *fp_FITS_cube, BYTE *image,
                                   int nx, int ny, int nz, int *current_file_nz,
                                   int nz_maxi, int *ifull);
int close_output_byte_fits_cube(FILE *fp_FITS_cube, int nx, int ny,
                                int current_file_nz);
int fits_readhead(FILE *fp, int *naxis1, int *naxis2, int *naxis3,
                  double *bscale, double *bzero, int *bitpix);
int fits_writhead(FILE *fp, int naxis1, int naxis2, int naxis3,
                  double bscale, double bzero, int bitpix, char *comments,
                  DESCRIPTORS descrip, PROCESS_SETTINGS Pset);
int read_float_data_from_FITS_file(FILE *fp, float *image, int npixels,
                                   char *errmess);
int float_to_aword_normalized_conversion(float *f_image, AWORD *u_image, int npixels);
int float_to_byte_normalized_conversion(float *f_image, BYTE *u_image, int npixels);

// In jlp_fits0_cvb.cpp
int cvinit();
int cui2(UI2 *pbuf, int no, int to);
int cvi2(VI2 *pbuf, int no, int to);
int cvi4(VI4 *pbuf, int no, int to);
int cvr4(float *array, int nb);

#endif  /* EOF sentry */
