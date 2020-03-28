/***************************************************************************
* asp2_fits_utils.cpp
*
* JLP
* Version 17/08/2015
***************************************************************************/
#include <stdio.h>
#include <math.h>

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "asp2_3D_outfits.h"
#include "asp2_rw_config_files.h"   // Init_PSET()

/**************************************************************************
* Creator:
**************************************************************************/
JLP_3D_outFITSFile::JLP_3D_outFITSFile(char *filename, int nx, int ny, int nz_maxi,
                                      char *comments, DESCRIPTORS descrip,
                                      int bitpix, char *errmess) {
    int status;
    PROCESS_SETTINGS Pset2;

// Initialize Pset2 to dummy values:
    Init_PSET(Pset2);

    current_file_nz1 = 0;
    size_of_header1 = 0;
    nx1 = nx; ny1 = ny;
    bitpix1 = bitpix;
    nz1_maxi = nz_maxi;
    strcpy(filename1, filename);
    is_full = 0;
    status = open_output_fits_cube(filename, &fp_FITSFile1, errmess);
    if(status) fp_FITSFile1 = NULL;
    status = write_header_output_fits_cube(fp_FITSFile1, nx1, ny1, nz1_maxi,
                                           bitpix1, comments, descrip,
                                           Pset2, errmess);
// Get size of header by inquiring the current position in the file:
    size_of_header1 = ftell(fp_FITSFile1);

    if(status) fp_FITSFile1 = NULL;
    }
/**************************************************************************
* Routine to write BYTE data cube to the output file
* (and close it when it is full)
**************************************************************************/
int JLP_3D_outFITSFile::WriteByteCube_to_3D_cube(BYTE *image, int nx, int ny, int nz_cube,
                           char *err_messg)
{
 int ifull, status;
 if(fp_FITSFile1 == NULL) {
      sprintf(err_messg, "WriteByteCube_to_3D_cube/Error: FITS file is not open!\n");
      return(-1);
      }
 if(bitpix1 != 8) {
      sprintf(err_messg, "WriteByteCube_to_3D_cube/Error: bitpix != 8!\n");
      return(-1);
      }
 if(is_full) {
      sprintf(err_messg, "WriteByteCube_to_3D_cube/Error: FITS file is full!\n");
      return(1);
      }
 if(nx != nx1 || ny != ny1) {
      sprintf(err_messg,
      "WriteByteCube_to_3D_cube/Error: incompatible size: nx=%d nx1=%d ny=%d ny1=%d\n",
      nx, nx1, ny, ny1);
      return(-2);
      }
 status = WriteByteCube_to_output_fits_cube(fp_FITSFile1, image, nx, ny,
                                            nz_cube, &current_file_nz1,
                                            nz1_maxi, &ifull);
 if(ifull) is_full = 1;

 return(status);
}
