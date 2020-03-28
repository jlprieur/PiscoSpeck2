/**************************************************************************
* Routines to access to FITS files
* Defined in asp2_3dfits_rd_utils.cpp
*
* JLP
* Version 06/11/2017
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include "asp2_typedef.h"  // AWORD, BYTE

int open_input_byte_fits_cube(char *filename, int *nx, int *ny,
                              int *nz_total, double *bscale, double *bzero,
                              char *comments, char *errmess,
                              FILE **fp_FITS_cube);
int open_input_fits_cube1(char *filename, int *nx, int *ny, int *nz_total,
                          int *bitpix, double *bscale, double *bzero,
                          char *comments, char *errmess, FILE **fp_FITS_cube);
int read_byte_from_input_fits_cube(FILE *fp_FITS_cube, BYTE *image,
                                   int nx, int ny, int nz_cube, int bitpix,
                                   double bscale, double bzero,
                                   int *current_file_nz,
                                   int total_file_nz, int *all_read);
int read_aword_from_input_fits_cube(FILE *fp_FITS_cube, AWORD *image,
                                    int nx, int ny, int nz_cube, int bitpix,
                                    double bscale, double bzero,
                                    int *current_file_nz,
                                    int total_file_nz, int *all_read);
int read_float_from_input_fits_cube(FILE *fp_FITS_cube, float *image,
                                    int nx, int ny, int nz_cube, int bitpix,
                                    double bscale, double bzero,
                                    int *current_file_nz,
                                    int total_file_nz, int *all_read);
