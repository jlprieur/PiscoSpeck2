/**************************************************************************
* Routines to access to FITS files
* Defined in asp2_3dfits_wr_utils.cpp
*
* JLP
* Version 06/11/2017
**************************************************************************/
#include <stdio.h>
#include <string.h>

class JLP_3D_outFITSFile;

int Copy_outFITS_3DFile_to_2DFITSFiles(JLP_3D_outFITSFile *outFITSFile,
                                   char *generic_name, char *directory,
                                   char *comments, DESCRIPTORS descrip,
                                   PROCESS_SETTINGS Pset,
                                   int bitpix, char *errmess);

int Copy_outFITS_3DFile_to_3DFITSFile(JLP_3D_outFITSFile *outFITSFile,
                                   char *generic_name, char *directory,
                                   char *comments, DESCRIPTORS descrip,
                                   PROCESS_SETTINGS Pset,
                                   int bitpix, char *errmess);

int open_output_fits_cube(char *filename, FILE **fp_FITS_cube, char *errmess);
int write_header_output_fits_cube(FILE *fp_FITS_cube, int nx, int ny,
                                 int total_file_nz, int bitpix,
                                 char *comments, DESCRIPTORS descrip,
                                 PROCESS_SETTINGS Pset, char *errmess);
int WriteByteCube_to_output_fits_cube(FILE *fp_FITS_cube, BYTE *image,
                                   int nx, int ny, int nz_cube, int *current_file_nz,
                                   int nz_maxi, int *ifull);
int WriteAwordCube_to_output_fits_cube(FILE *fp_FITS_cube, AWORD *image,
                                   int nx, int ny, int nz_cube, int *current_file_nz,
                                   int total_file_nz, int *ifull);
int close_output_byte_fits_cube(FILE *fp_FITS_cube, int nx, int ny,
                                int current_file_nz);
