/**************************************************************************
* Routines to access to FITS files
* Defined in asp2_2
dfits_utils.cpp
*
* JLP
* Version 06/11/2017
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include "asp2_typedef.h"   // AWORD, BYTE

int  JLP_D_WRITEIMAG0(double* image, int nx1, int ny1, char* filename,
                      char* comments);
int  JLP_D_WRITEIMAG(double* image, int nx, int ny,
                     char* filename, char* comments, DESCRIPTORS descrip,
                     PROCESS_SETTINGS Pset);
int  JLP_F_WRITEIMAG(float* image, int nx, int ny,
                     char* filename, char* comments, DESCRIPTORS descrip,
                     PROCESS_SETTINGS Pset);
int ext_and_dir_from_filename(char *file_ext, char *directory);
