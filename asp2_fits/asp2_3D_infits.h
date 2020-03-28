/**************************************************************************
* Routines to access to FITS files
* JLP_3D_inFITSFile class
*
* JLP
* Version 06/11/2017
**************************************************************************/
#ifndef _asp2_3D_infits_h    /* Sentry */
#define _asp2_3D_infits_h

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <stdio.h>
#include <string.h>
#include "asp2_typedef.h"
#include "asp2_rw_config_files.h" // Init_PSET
#include "asp2_3dfits_rd_utils.h" // open_input_fits_cube1()

/************************************************************************
* Define a new class to enable opening input FITS files and reading to
* them like with a SVHS tape
*************************************************************************/
class JLP_3D_inFITSFile {
  public:
   JLP_3D_inFITSFile(char *filename);
// Destructor:
    ~JLP_3D_inFITSFile() { Close_3DFile();}

   int Open_3DFile(char *filename, int *nx, int *ny, int *nz_total,
                   char *comments, char *errmess);
   int Read_byte_cube_from_3DFitsFile(BYTE *image, int nx, int ny, int nz_cube,
                                      char *err_messg);
   int Read_aword_cube_from_3DFitsFile(AWORD *image, int nx, int ny, int nz_cube,
                                       char *err_messg);
   int Read_float_cube_from_3DFitsFile(float *image, int nx, int ny, int nz_cube,
                                       char *err_messg);
   void Close_3DFile();
   int Rewind_3DFile();

/**************************************************************************
* Accessors (JLP_3D_inFITSFile):
**************************************************************************/
// Accessor to know if file is open:
   bool IsClosed(){
      if(fp_FITSFile1 == NULL) return true;
       else return false;
      }
   bool IsOpen(){return (!IsClosed());}
   int IsAllRead(){return(is_all_read);}
   int Nx(){return(nx1);}
   int Ny(){return(ny1);}
   int Nz_Current(){return(current_file_nz1);}
   int Nz_Total(){return(total_file_nz1);}

  private:
    FILE *fp_FITSFile1;
    int nx1, ny1, current_file_nz1, total_file_nz1, is_all_read;
    int bitpix1;
    double bscale1, bzero1;
    char filename1[128], comments1[128], error_messg1[128];
};

#endif  /* EOF sentry */
