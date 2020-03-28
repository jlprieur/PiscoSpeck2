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

#include "asp2_2dfits_utils.h"
#include "asp2_rw_config_files.h"   // Init_PSET()
#include "jlp_rw_fits0.h"           // readimag_....

//---------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int  JLP_D_WRITEIMAG0(double* image, int nx1, int ny1, char* filename,
                      char* comments)
{
char err_message[60];
DESCRIPTORS descrip;
PROCESS_SETTINGS Pset;
int status;

descrip.ndescr = 0;
Init_PSET(Pset);

status = writeimag_double_fits(filename, image, nx1, ny1, comments,
                                descrip, Pset, err_message);
if(status) wxMessageBox(wxString(err_message), wxT("JLP_D_WRITEIMAG0/Error"),
                        wxICON_ERROR);

return (status);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int  JLP_D_WRITEIMAG(double* image, int nx1, int ny1, char* filename,
                     char* comments, DESCRIPTORS descrip,
                     PROCESS_SETTINGS Pset)
{
char err_message[60];
int status;

status = writeimag_double_fits(filename, image, nx1, ny1, comments,
                                descrip, Pset, err_message);
if(status) wxMessageBox(wxString(err_message), wxT("JLP_D_WRITEIMAG/Error"),
                        wxICON_ERROR);

return (status);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int  JLP_F_WRITEIMAG(float* image, int nx1, int ny1, char* filename,
                     char* comments, DESCRIPTORS descrip,
                     PROCESS_SETTINGS Pset)
{
char err_message[60];
int status;

status = writeimag_float_fits(filename, image, nx1, ny1, comments,
                                descrip, Pset, err_message);
if(status) wxMessageBox(wxString(err_message), wxT("JLP_F_WRITEIMAG/Error"),
                        wxICON_ERROR);

return (status);
}
/*******************************************************************
*
* INPUT:
* filename, with the directory
* OUTPUT:
* file_ext and directory (separated)
*******************************************************************/
int ext_and_dir_from_filename(char *filename, char *file_ext, char *directory)
{
char *pc, buffer[256];
register int i;
int ilast;
// Do not save any file if generic name "file_ext" is empty
  if(filename[0] == '\0') return(-1);

/*********************************************************/
/* Now output of the results : */

// Exemple: "d:\test\ads2544.fits"
  strcpy(buffer, filename);
  ilast = -1;
  for(i = 0; i < 80; i++)
      {
      if(buffer[i] == '\\' || buffer[i] == ':') ilast = i;
      if(!buffer[i]) break;
      }
  strcpy(directory, buffer);
  directory[ilast+1] = '\0';

  strcpy(file_ext, &buffer[ilast+1]);

/* Remove trailing blanks: */
  pc = file_ext;
  while(*pc && *pc != ' ') pc++;
  *pc='\0';
// Look for ".fits":
  pc = file_ext;
  while(*pc && *pc != '.') pc++;
  *pc = '\0';

#ifdef DEBUG
  sprintf(buffer,"generic name: >%s< directory: >%s<",file_ext, directory);
  wxMessageBox(wxString(buffer), wxT("ext_and_dir_from_filename"),
               wxICON_INFORMATION | wxOK);

#endif
return(0);
}
