/**************************************************************************
* Routines to read a series of 2D FITS files
* and definition of JLP_2D_inFITSFile
* ** FileSpeck2 project **
*
* JLP
* Version 28/01/2015
**************************************************************************/
#ifndef _asp2_2Dfits_h    /* Sentry */
#define _asp2_2Dfits_h

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <stdio.h>
#include <string.h>
#include "asp2_typedef.h"       // BYTE

/***************************************************************************/
// In asp2_2DFits.cpp
int open_input_2DFitsFiles(char *GenericName, char *FileDirectory,
                           wxString **FileName, int *nx, int *ny, int *nz_total,
                           char *errmess);
int read_byte_cube_from_2DFitsFiles(wxString *FileName, BYTE *image,
                                   int nx, int ny, int nz_cube,
                                   int *current_file_nz,
                                   int total_file_nz, int *all_read);
int read_aword_cube_from_2DFitsFiles(wxString *FileName, AWORD *image,
                                   int nx, int ny, int nz_cube,
                                   int *current_file_nz,
                                   int total_file_nz, int *all_read);
int read_float_cube_from_2DFitsFiles(wxString *FileName, float *image,
                                   int nx, int ny, int nz_cube,
                                   int *current_file_nz,
                                   int total_file_nz, int *all_read);

/************************************************************************
* Define a new class to enable opening a series of input FITS files and reading
* them sequentially
*************************************************************************/
class JLP_2D_inFITSFile {
  public:
/**************************************************************************
* Creator:
**************************************************************************/
   JLP_2D_inFITSFile(char *generic_name, char *directory, int *nx, int *ny,
                     int *nz_total, char *errmess)
   {
    int status = 0;
    char error_msg[512];
    nx1 = ny1 = 0;
    current_file_nz1 = 0;
    is_all_read = 0;
    FileName1 = NULL;
    status = Open_2DFile(generic_name, directory, nx, ny, nz_total, errmess);
    if(status) Close_2DFile();

    return;
   }
/**************************************************************************
* Routine to open the series of 2D FITS files
**************************************************************************/
   int Open_2DFile(char *generic_name, char *directory, int *nx, int *ny,
                   int *nz_total, char *errmess)
   {
    int status = 0;

// Cleanup old file list if necessary:
    if(FileName1 != NULL) {
       delete FileName1;
       FileName1 = NULL;
       }

// Load a new file list and read nx, ny, nz_total:
    status = open_input_2DFitsFiles(generic_name, directory, &FileName1,
                                    nx, ny, nz_total, errmess);
    if(status) {
      strcpy(GenericName1, "");
      strcpy(FileDirectory1, "");
      nx1 = 0; ny1 = 0;
      total_file_nz1 = 0;
      wxMessageBox(wxString(errmess), wxT("open_input_2DFitsFiles/Error"),
                   wxICON_ERROR);
      } else {
      strcpy(GenericName1, generic_name);
      strcpy(FileDirectory1, directory);
      nx1 = *nx; ny1 = *ny;
      total_file_nz1 = *nz_total;
      }
     return status;
   }
/**************************************************************************
* Routine that reads BYTE data from the 2D FITS files
* (and stop when all files are read)
**************************************************************************/
    int Read_byte_cube_from_2DFitsFiles(BYTE *image, int nx, int ny, int nz_cube,
                                        char *error_messg) {
      int status;
      if(FileName1 == NULL) {
        sprintf(error_messg, "Read_byte_cube_from_2DFitsFiles/Error: 2D FITS files not open\n");
        return(-1);
        }
      if(nx != nx1 || ny != ny1) {
        sprintf(error_messg,
        "Read_byte_cube_from_2DFitsFiles/Error: incompatible size: nx=%d nx1=%d ny=%d ny1=%d\n",
        nx, nx1, ny, ny1);
        return(-2);
        }
        status = read_byte_cube_from_2DFitsFiles(FileName1, image, nx, ny,
                                                 nz_cube, &current_file_nz1,
                                                 total_file_nz1, &is_all_read);
        if(is_all_read) status = 10;

      return(status);
      }
/**************************************************************************
* Routine that reads AWORD data from 2D FITS files
* (and closes it when it is completely read)
**************************************************************************/
    int Read_aword_cube_from_2DFitsFiles(AWORD *image, int nx, int ny, int nz_cube,
                                         char *err_messg) {
      int status;
      if(FileName1 == NULL) {
        sprintf(err_messg, "Read_byte_cube_from_2DFitsFiles/Error: 2D FITS files not open\n");
        return(-1);
        }
      if(nx != nx1 || ny != ny1) {
        sprintf(err_messg,
        "Read_aword_cube_from_2DFitsFiles/Error: incompatible size: nx=%d nx1=%d ny=%d ny1=%d\n",
        nx, nx1, ny, ny1);
        return(-2);
        }
        status = read_aword_cube_from_2DFitsFiles(FileName1, image, nx, ny,
                                                  nz_cube, &current_file_nz1,
                                                  total_file_nz1, &is_all_read);
        if(is_all_read) status = 10;

      return(status);
      }
/**************************************************************************
* Routine that reads float data from the 3D FITS file
* (and closes it when it is completely read)
**************************************************************************/
    int Read_float_cube_from_2DFitsFiles(float *image, int nx, int ny, int nz_cube,
                                     char *err_messg) {
      int status;
      if(FileName1 == NULL) {
        sprintf(err_messg, "Read_byte_cube_from_2DFitsFiles/Error: 2D FITS files not open\n");
        return(-1);
        }
      if(nx != nx1 || ny != ny1) {
        sprintf(err_messg,
        "Read_float_cube_from_2DFitsFiles/Error: incompatible size: nx=%d nx1=%d ny=%d ny1=%d\n",
        nx, nx1, ny, ny1);
        return(-2);
        }
        status = read_float_cube_from_2DFitsFiles(FileName1, image, nx, ny,
                                                  nz_cube, &current_file_nz1,
                                                  total_file_nz1, &is_all_read);
        if(is_all_read) status = 10;
        
      return(status);
      }
/**************************************************************************
* Routine to close the file list:
**************************************************************************/
    void Close_2DFile() {
      if(FileName1 != NULL) {
        delete[] FileName1;
        FileName1 = NULL;
        current_file_nz1 = 0;
        total_file_nz1 = 0;
        }
     }
/**************************************************************************
* Routine to rewind the file list:
**************************************************************************/
    void Rewind_2DFile() {
      current_file_nz1 = 0;
      return;
     }
/**************************************************************************
* Accessors (JLP_2D_inFITSFile):
**************************************************************************/
// Accessor to know if file is open:
   int IsClosed(){
      if(FileName1 == NULL) return 1;
       else return 0;
      }
   int IsOpen(){return (1 - IsClosed());}
   int IsAllRead(){return(is_all_read);}
   int Nx(){return(nx1);}
   int Ny(){return(ny1);}
   int Nz_Current(){return(current_file_nz1);}
   int Nz_Total(){return(total_file_nz1);}
/**************************************************************************
* Destructor:
**************************************************************************/
    ~JLP_2D_inFITSFile() { Close_2DFile();}
  private:
    int nx1, ny1, current_file_nz1, total_file_nz1, is_all_read;
    char GenericName1[128], FileDirectory1[128], error_messg1[128];
    wxString *FileName1;
};
#endif  /* EOF sentry */
