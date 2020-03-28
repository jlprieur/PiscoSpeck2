/***************************************************************************
* asp2_rw_config_files
*
* JLP
* Version 13/11/2017
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "asp2_typedef.h"
#include "asp2_rw_config_files.h"

//---------------------------------------------------------------------------
// Initialize structure PROCESS_SETTINGS
//    (the prototype is declared in "asp2_typedef.h")
//---------------------------------------------------------------------------
int Init_PSET(PROCESS_SETTINGS& Pset2) {

    Pset2.FwhmSelect.Stats[0] = '\0';
    Pset2.FwhmSelect.LowCut = 0;
    Pset2.FwhmSelect.HighCut = 0;
    Pset2.FwhmSelect.MinVal = 100000;
    Pset2.FwhmSelect.MaxVal = -1;
    Pset2.FwhmSelect.Sum = 0.;
    Pset2.FwhmSelect.Sumsq = 0.;
    Pset2.FwhmSelect.Nval = 0;
    Pset2.MaxValSelect.Stats[0] = '\0';
    Pset2.MaxValSelect.LowCut = 0;
    Pset2.MaxValSelect.HighCut = 0;
    Pset2.MaxValSelect.MinVal = 100000;
    Pset2.MaxValSelect.MaxVal = -1;
    Pset2.MaxValSelect.Sum = 0.;
    Pset2.MaxValSelect.Sumsq = 0.;
    Pset2.MaxValSelect.Nval = 0;
    Pset2.slangle = 90.;
    Pset2.spangle = 0.;
    Pset2.KK_CrossCorr = 6;
    Pset2.ProcessingMode = 3;
    Pset2.OnlyVisuMode = false;
    Pset2.Selection_fwhm = false;
    Pset2.Selection_maxval = false;
    Pset2.Statistics_is_wanted = true;
    Pset2.SubtractUnresolvedAutoc = true;
// 500 for the OCA SPICA camera
// 5 for Merate 3D FITS files:
    Pset2.OffsetThreshold = 5;
    Pset2.OffsetFrameCorrection = false;
    Pset2.FlatFieldCorrection = false;
// 3000 frames = 1 minute with 50 images/second
// 30000 frames = 10 minutes with 50 images/second
    Pset2.out3DFITS_nz_max = 30000.;  // maximum size of FITS cube used
                                     // for saving elementary frames
// For loading/processing:
    Pset2.nz_cube = 50;
// For spectroscopy: dummy values
    Pset2.nx_num = 128;
    Pset2.ny_num = 128;
// For bispectrum:
    Pset2.ir_bisp = 30;
    Pset2.nmax_bisp = 200;

    Pset2.DirectVector = true;
    Pset2.LuckyImaging = false;
#ifndef FILE_SPECK1
// By default, save frames to 3D Fits files if ANDOR is used
    Pset2.SaveFramesToFitsFile = 3;
    Pset2.SaveSelectionOnlyToFitsFile = true;
#else
// By default, do not save frames to 3D Fits files if processing recorded file
    Pset2.SaveFramesToFitsFile = 0;
    Pset2.SaveSelectionOnlyToFitsFile = false;
#endif
    Pset2.SlowProcessing = false;     // True when 2 cubes are used for processing
    Pset2.OffsetFileName[0] = '\0';
    Pset2.UnresolvedAutocName[0] = '\0';
    Pset2.UnresolvedModsqName[0] = '\0';
    Pset2.FlatFieldFileName[0] = '\0';
    Pset2.PhotModsqFileName[0] = '\0';
    Pset2.InputFileName[0] = '\0';

    strcpy(Pset2.FFTW_directory, ".");
    strcpy(Pset2.out3DFITS_directory, ".");
    strcpy(Pset2.outResults_directory, ".");
/* Villaries:
    strcpy(Pset2.FFTW_directory, "H:\\TMP_FFTW");
    strcpy(Pset2.out3DFITS_directory, "H:\\MERATE_3D");
    strcpy(Pset2.outResults_directory, "H:\\MERATE_3D");
// OMP/NICE:
    strcpy(Pset2.FFTW_directory, "F:\\TMP_FFTW");
    strcpy(Pset2.out3DFITS_directory, "F:\\NICE_3D");
    strcpy(Pset2.outResults_directory, "F:\\NICE_Results");
   Merate::
    strcpy(Pset2.FFTW_directory, "C:\\TMP_FFTW");
    strcpy(Pset2.out3DFITS_directory, "K:\\AndorCubes");
    strcpy(Pset2.outResults_directory, "J:\\TMP");
*/

// UT versus local time: UT = local_time + utime_offset
   Pset2.utime_offset = -1;
return 0;
}
//---------------------------------------------------------------------------
// Copy structure PROCESS_SETTINGS
//    (the prototype is declared in "asp2_typedef.h")
// INPUT:
//   Pset1
//
// OUTPUT:
//   Pset2
//---------------------------------------------------------------------------
int Copy_PSET(const PROCESS_SETTINGS Pset1, PROCESS_SETTINGS& Pset2)
{
    strcpy(Pset2.FwhmSelect.Stats, Pset1.FwhmSelect.Stats);
    Pset2.FwhmSelect.LowCut = Pset1.FwhmSelect.LowCut;
    Pset2.FwhmSelect.HighCut = Pset1.FwhmSelect.HighCut;
    Pset2.FwhmSelect.MinVal = Pset1.FwhmSelect.MinVal;
    Pset2.FwhmSelect.MaxVal = Pset1.FwhmSelect.MaxVal;
    Pset2.FwhmSelect.Sum = Pset1.FwhmSelect.Sum;
    Pset2.FwhmSelect.Sumsq = Pset1.FwhmSelect.Sumsq;
    Pset2.FwhmSelect.Nval = Pset1.FwhmSelect.Nval;
    strcpy(Pset2.MaxValSelect.Stats, Pset1.MaxValSelect.Stats);
    Pset2.MaxValSelect.LowCut = Pset1.MaxValSelect.LowCut;
    Pset2.MaxValSelect.HighCut = Pset1.MaxValSelect.HighCut;
    Pset2.MaxValSelect.MinVal = Pset1.MaxValSelect.MinVal;
    Pset2.MaxValSelect.MaxVal = Pset1.MaxValSelect.MaxVal;
    Pset2.MaxValSelect.Sum = Pset1.MaxValSelect.Sum;
    Pset2.MaxValSelect.Sumsq = Pset1.MaxValSelect.Sumsq;
    Pset2.MaxValSelect.Nval = Pset1.MaxValSelect.Nval;
    Pset2.slangle = Pset1.slangle;
    Pset2.spangle = Pset1.spangle;
    Pset2.ProcessingMode = Pset1.ProcessingMode;
    Pset2.OnlyVisuMode = Pset1.OnlyVisuMode;
    Pset2.Selection_fwhm = Pset1.Selection_fwhm;
    Pset2.Selection_maxval = Pset1.Selection_maxval;
    Pset2.Statistics_is_wanted = Pset1.Statistics_is_wanted;
    Pset2.SubtractUnresolvedAutoc = Pset1.SubtractUnresolvedAutoc;
    Pset2.OffsetThreshold = Pset1.OffsetThreshold;
    Pset2.OffsetFrameCorrection = Pset1.OffsetFrameCorrection;
    Pset2.FlatFieldCorrection = Pset1.FlatFieldCorrection;
    Pset2.out3DFITS_nz_max = Pset1.out3DFITS_nz_max;
    Pset2.nz_cube = Pset1.nz_cube;
    Pset2.nx_num = Pset1.nx_num;
    Pset2.ny_num = Pset1.ny_num;
    Pset2.ir_bisp = Pset1.ir_bisp;
    Pset2.nmax_bisp = Pset1.nmax_bisp;
    Pset2.KK_CrossCorr = Pset1.KK_CrossCorr;
    Pset2.DirectVector = Pset1.DirectVector;
    Pset2.LuckyImaging = Pset1.LuckyImaging;
    Pset2.SaveFramesToFitsFile = Pset1.SaveFramesToFitsFile;
    Pset2.SaveSelectionOnlyToFitsFile = Pset1.SaveSelectionOnlyToFitsFile;
    Pset2.SlowProcessing = Pset1.SlowProcessing;
    strcpy(Pset2.OffsetFileName, Pset1.OffsetFileName);
    strcpy(Pset2.UnresolvedAutocName, Pset1.UnresolvedAutocName);
    strcpy(Pset2.UnresolvedModsqName, Pset1.UnresolvedModsqName);
    strcpy(Pset2.FlatFieldFileName, Pset1.FlatFieldFileName);
    strcpy(Pset2.PhotModsqFileName, Pset1.PhotModsqFileName);
    strcpy(Pset2.InputFileName, Pset1.InputFileName);
    strcpy(Pset2.FFTW_directory, Pset1.FFTW_directory);
    strcpy(Pset2.out3DFITS_directory, Pset1.out3DFITS_directory);
    strcpy(Pset2.outResults_directory, Pset1.outResults_directory);
    Pset2.utime_offset = Pset1.utime_offset;

return 0;
}
/***************************************************************************
*
***************************************************************************/
int ReadBoolFromConfigFile(char *filename, const char *keyword, bool *bvalue)
{
int status, ivalue = 0;
char buffer[80];

 status = ReadStringFromConfigFile(filename, keyword, buffer);
 if(!status) sscanf(buffer, "%d", &ivalue);

 if(ivalue)
  *bvalue = true;
 else
  *bvalue = false;

return(status);
}
/***************************************************************************
*
***************************************************************************/
int ReadIntFromConfigFile(char *filename, const char *keyword, int *ivalue)
{
int status;
char buffer[80];
*ivalue = 0;
 status = ReadStringFromConfigFile(filename, keyword, buffer);
 if(!status) sscanf(buffer, "%d", ivalue);

return(status);
}
/***************************************************************************
*
***************************************************************************/
int ReadDoubleFromConfigFile(char *filename, const char *keyword, double *dvalue)
{
int status;
char buffer[80];
*dvalue = 0.;
 status = ReadStringFromConfigFile(filename, keyword, buffer);
 if(!status) sscanf(buffer, "%lf", dvalue);

return(status);
}
/***************************************************************************
*
***************************************************************************/
int ReadStringFromConfigFile(char *filename, const char *keyword, char *cvalue)
{
int status = -1, found;
FILE *fp_in;
wxString buf0;
char buffer[128], *pc;

strcpy(cvalue, "");

if((fp_in = fopen(filename,"r"))== NULL) {
  buf0.Printf(wxT("Error opening input file >%s< \n"), filename);
  wxMessageBox(buf0, wxT("ReadStringFromConfigFile"),
               wxOK | wxICON_ERROR);
  return(-1);
  }

found = 0;
while((found == 0) && !feof(fp_in)) {
  fgets(buffer, 80, fp_in);
  pc = buffer;
  while(*pc && (*pc != '=') && strncmp(pc, keyword, strlen(keyword))) pc++;
  if(!strncmp(pc, keyword, strlen(keyword))) {
    found = 1;
    while(*pc && (*pc != '=')) pc++;
    if(*pc == '=') {
      pc++;
      strcpy(cvalue, pc);
      status = 0;
      }
    }

 } // EOF while(!found)

fclose(fp_in);

if(status != 0) {
  buf0.Printf(wxT("Error: keyword= >%s< (length=%d) not found in %s\n"),
              keyword, (int)strlen(keyword), filename);
  wxMessageBox(buf0, wxT("ReadStringFromConfigFile"),
               wxOK | wxICON_ERROR);
} else {
// Cleanup cvalue:
  strcpy(buffer, cvalue);
// Remove blanks at the beginning:
  pc = cvalue;
  while(*pc && (*pc == ' ')) pc++;
  strcpy(cvalue, pc);

// Remove cr or line feed
  pc = cvalue;
  while(*pc && (*pc != '\r') && (*pc != '\n')) pc++;
  *pc = '\0';
}

return(status);
}
