/**************************************************************************
* Routines to access to FITS files
* JLP_3D_outFITSFile class
*
* JLP
* Version 06/11/2017
**************************************************************************/
#ifndef _asp2_3Dfits_out_h    /* Sentry */
#define _asp2_3Dfits_out_h

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <stdio.h>
#include <string.h>
#include "asp2_typedef.h"
#include "asp2_rw_config_files.h" // Init_PSET
#include "asp2_3dfits_wr_utils.h"

/************************************************************************
* Define a new class to enable opening output FITS files and writing to
* them like with a SVHS tape
*************************************************************************/
class JLP_3D_outFITSFile {
  public:
/**************************************************************************
* Creator:
**************************************************************************/
    JLP_3D_outFITSFile(char *filename, int nx, int ny, int nz_maxi,
                       char *comments, DESCRIPTORS descrip,
                       int bitpix, char *errmess);

/**************************************************************************
* Routine to write BYTE data cube to the output file
* (and close it when it is full)
**************************************************************************/
    int WriteByteCube_to_3D_cube(BYTE *image, int nx, int ny, int nz_cube,
                           char *err_messg);
/**************************************************************************
* Routine to write AWORD data to the output file
* (and close it when it is full)
**************************************************************************/
    int WriteAwordCube_to_3D_cube(AWORD *image, int nx, int ny, int nz_cube,
                                  char *err_messg) {
      int ifull, status;
      if(fp_FITSFile1 == NULL) {
        sprintf(err_messg, "WriteAwordCube_to_3D_cube/Error: FITS file is not open!\n");
        return(-1);
        }
      if(bitpix1 != 16) {
        sprintf(err_messg, "WriteAwordCube_to_3D_cube/Error: bitpix != 16 !\n");
        return(-1);
        }
      if(is_full) {
        sprintf(err_messg, "WriteAwordCube_to_3D_cube/Error: FITS file is full!\n");
        return(1);
        }
      if(nx != nx1 || ny != ny1) {
        sprintf(err_messg,
        "WriteAwordCube_to_3D_cube/Error: incompatible size: nx=%d nx1=%d ny=%d ny1=%d\n",
        nx, nx1, ny, ny1);
        return(-2);
        }
      status = WriteAwordCube_to_output_fits_cube(fp_FITSFile1, image, nx, ny,
                                                  nz_cube, &current_file_nz1,
                                                  nz1_maxi, &ifull);
      if(ifull) {is_full = 1; status = 1;}
      return(status);
      }
/**************************************************************************
* Routine to close the file:
**************************************************************************/
    int Close_3D_outFITSFile() {
      if(fp_FITSFile1 != NULL) {
        close_output_byte_fits_cube(fp_FITSFile1, nx1, ny1, current_file_nz1);
        fp_FITSFile1 = NULL;
        }
    return(0);
    }
/**************************************************************************
* Correct the value of naxis3: set it to current_file_nz1
* before closing file   To be tested later...
**************************************************************************/
int CorrectNaxis3(int bitpix, char *comments, DESCRIPTORS descrip,
                  PROCESS_SETTINGS Pset, char *errmess)
{
int status;

// Error if file is closed:
    if(fp_FITSFile1 == NULL) return(-1);

// Go to beginning of file:
    fseek(fp_FITSFile1, 0L, SEEK_SET);

// Rewrite the header:
    status = write_header_output_fits_cube(fp_FITSFile1, nx1, ny1,
                                           current_file_nz1, bitpix, comments,
                                           descrip, Pset, errmess);

// Go to end of file:
    fseek(fp_FITSFile1, 0L, SEEK_END);

   return(status);
   }
/**************************************************************************
* Accessor to know if outFITSFile is open:
**************************************************************************/
   int IsClosed(){
      if(fp_FITSFile1 == NULL) return 1;
       else return 0;
      }
   int IsOpen(){return (1 - IsClosed());}
/**************************************************************************
* Accessors (JLP_3D_outFITSFile):
**************************************************************************/
   int IsFull(){return(is_full);}
   int Nx(){return(nx1);}
   int Ny(){return(ny1);}
   int Nz_Current(){return(current_file_nz1);}
   int Nz_Maxi(){return(nz1_maxi);}
   long SizeOfHeader(){return(size_of_header1);}
   FILE *fp(){return(fp_FITSFile1);}
/**************************************************************************
* Destructor:
**************************************************************************/
    ~JLP_3D_outFITSFile() {
       Close_3D_outFITSFile();
       }
  private:
    FILE *fp_FITSFile1;
    int nx1, ny1, current_file_nz1, nz1_maxi, is_full;
    int bitpix1;
    long size_of_header1;
    char filename1[128];
};

#endif  /* EOF sentry */
