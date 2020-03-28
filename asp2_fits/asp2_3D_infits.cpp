/***************************************************************************
* asp2_3D_infits.cpp
*
* JLP
* Version 17/08/2015
***************************************************************************/
#include "asp2_3D_infits.h"

/**************************************************************************
* Creator:
* JLP2015: simplified version
**************************************************************************/
JLP_3D_inFITSFile::JLP_3D_inFITSFile(char *filename)
   {
    wxString error_msg;
    char comments[128], errmess[128];
    int nx, ny, nz_total, status;

    nx1 = ny1 = 0;
    current_file_nz1 = 0;
    fp_FITSFile1 = NULL;
    is_all_read = 0;

    status = Open_3DFile(filename, &nx, &ny, &nz_total, comments, errmess);
    if(nz_total < 5) {
       error_msg.Printf(wxT("Small size: nx=%d ny=%d nz_total=%d !"),
                       nx, ny, nz_total);
       wxMessageBox(error_msg, wxT("JLP_3D_inFITSFile/Error"), wxICON_ERROR);
       Close_3DFile();
      }

    return;
   }
/**************************************************************************
* Routine to open the 3D FITS file
**************************************************************************/
int JLP_3D_inFITSFile::Open_3DFile(char *filename, int *nx, int *ny, int *nz_total,
                    char *comments, char *errmess)
{
int status, bitpix;
double bscale, bzero;

current_file_nz1 = 0;
nx1 = 0;
ny1 = 0;
total_file_nz1 = 0;
bitpix1 = 0;
bscale1 = 0;
bzero1 = 0;
is_all_read = 0;

 status = open_input_fits_cube1(filename, nx, ny, nz_total, &bitpix,
                                &bscale, &bzero, comments,
                                errmess, &fp_FITSFile1);
 if(status) {
    fp_FITSFile1 = NULL;
    } else {
    strcpy(filename1, filename);
    strncpy(comments1, comments, 80);
    nx1 = *nx; ny1 = *ny;
    total_file_nz1 = *nz_total;
    bitpix1 = bitpix;
    bscale1 = bscale;
    bzero1 = bzero;
    }


return(status);
}
/**************************************************************************
* Routine that reads BYTE data from the 3D FITS file
* (and closes it when it is completely read)
**************************************************************************/
int JLP_3D_inFITSFile::Read_byte_cube_from_3DFitsFile(BYTE *image, int nx,
                                                      int ny, int nz_cube,
                                                      char *err_messg)
{
int status;
 if(fp_FITSFile1 == NULL) {
    sprintf(err_messg, "Read_byte_cube_from_3DFitsFile/Error: FITS file is not open\n");
    return(-1);
    }
 if(nx != nx1 || ny != ny1) {
    sprintf(err_messg,
    "Read_byte_cube_from_3DFitsFile/Error: incompatible size: nx=%d nx1=%d ny=%d ny1=%d\n",
    nx, nx1, ny, ny1);
    return(-2);
    }
 status = read_byte_from_input_fits_cube(fp_FITSFile1, image, nx, ny,
                                         nz_cube, bitpix1, bscale1, bzero1,
                                         &current_file_nz1, total_file_nz1,
                                         &is_all_read);
// Close file when it is all read:
    if(is_all_read) {
        current_file_nz1 = total_file_nz1;
        Close_3DFile();
        status = 10;
        }
return(status);
}
/**************************************************************************
* Routine that reads AWORD data from the 3D FITS file
* (and closes it when it is completely read)
**************************************************************************/
int JLP_3D_inFITSFile::Read_aword_cube_from_3DFitsFile(AWORD *image, int nx,
                                                       int ny, int nz_cube,
                                                       char *err_messg)
{
int status;
  if(fp_FITSFile1 == NULL) {
    sprintf(err_messg, "Read_aword_cube_from_3DFitsFile/Error: FITS file is not open\n");
    return(-1);
    }
  if(nx != nx1 || ny != ny1) {
    sprintf(err_messg,
    "Read_aword_cube_from_3DFitsFile/Error: incompatible size: nx=%d nx1=%d ny=%d ny1=%d\n",
    nx, nx1, ny, ny1);
    return(-2);
    }
  status = read_aword_from_input_fits_cube(fp_FITSFile1, image, nx, ny,
                                            nz_cube, bitpix1, bscale1, bzero1,
                                            &current_file_nz1,
                                            total_file_nz1, &is_all_read);
// Close file when it is all read:
    if(is_all_read) {
        current_file_nz1 = total_file_nz1;
        Close_3DFile();
        status = 10;
        }
return(status);
}
/**************************************************************************
* Routine that reads float data from the 3D FITS file
* (and closes it when it is completely read)
**************************************************************************/
int JLP_3D_inFITSFile::Read_float_cube_from_3DFitsFile(float *image,
                                                       int nx, int ny, int nz_cube,
                                                       char *err_messg)
{
int status;
  if(fp_FITSFile1 == NULL) {
    sprintf(err_messg, "Read_float_cube_from_3DFitsFile/Error: FITS file is not open\n");
    return(-1);
    }
  if(nx != nx1 || ny != ny1) {
    sprintf(err_messg,
        "Read_float_cube_from_3DFitsFile/Error: incompatible size: nx=%d nx1=%d ny=%d ny1=%d\n",
        nx, nx1, ny, ny1);
    return(-2);
    }
  status = read_float_from_input_fits_cube(fp_FITSFile1, image, nx, ny,
                                            nz_cube, bitpix1, bscale1, bzero1,
                                            &current_file_nz1, total_file_nz1,
                                            &is_all_read);
// Close file when it is all read:
  if(is_all_read) {
    current_file_nz1 = total_file_nz1;
    Close_3DFile();
    status = 10;
    }

return(status);
}
/**************************************************************************
* Routine to close the file:
**************************************************************************/
void JLP_3D_inFITSFile::Close_3DFile()
{
 if(fp_FITSFile1 != NULL) {
  fclose(fp_FITSFile1);
  fp_FITSFile1 = NULL;
 }
}
/**************************************************************************
* Routine to rewind the file:
**************************************************************************/
int JLP_3D_inFITSFile::Rewind_3DFile()
{
int status;
  Close_3DFile();
  status = Open_3DFile(filename1, &nx1, &ny1, &total_file_nz1,
                        comments1, error_messg1);
  if(status) {
    wxMessageBox(wxString(error_messg1), wxT("Rewind_3DFile/Error"),
                wxICON_ERROR);
    Close_3DFile();
  }

 return(status);
}
