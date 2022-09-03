/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* asp2_decod2.cpp
* JLP_Decode Class
*
* To compute autocorrelation, power spectrum and bispectrum of elementary frames
*
* ProcessingMode = 0 : no processing (only numerisation)
* ProcessingMode = 1 : Only integration
* ProcessingMode = 2 : autoc - crossc, FFT modulus, long integration
* ProcessingMode = 3 : autoc - crossc, FFT modulus, long integration, quadrant
* ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
* ProcessingMode = 5 : Bispectrum, autoc, quadrant
* ProcessingMode = 6 : Spectroscopy
* ProcessingMode = 7 : SCIDAR Log
* ProcessingMode = 8 : SCIDAR Lin
* ProcessingMode = 9 : Statistics
*
* JLP
* Version 10-02-2016
---------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include "jlp_itt1.h"         // Min_Max_Itt (in jlp_splot)
#include "jlp_numeric.h"      // recent_fft
#include "jlp_wx_ipanel_filters.h"   // subtract_back_model ...

#include "asp2_defs.h"        // enum ChildIndex
#include "asp2_photon_corr.h"       // definition of clean_deconvolution, ...
#include "asp2_covermas.h"    // bisp2D_to_2D_Image
#include "jlp_decode.h"
#include "jlp_time0.h"  // jlp_italian_date() ...

static int CopyFromBiggerArray(float *ima_in, double *ima_out, int nx_in, int ny_in,
                               int nx_out, int ny_out);

// #define DEBUG

/************************************************************************
* Read current values of some arrays (to be displayed by Main_Refresh())
*
* INPUT:
* decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*
* OUTPUT:
* dble_image0: image corresponding to decode_index
*************************************************************************/
int JLP_Decode::ReadDbleImage(double *dble_image0, const int nx0,
                              const int ny0, const int k_decode_index)
{
double xframes;
int isize, status = -1;
register int i;

// Check if size of dble_image0 is correct:
if((nx0 != nx1) || (ny0 != ny1)) {
  fprintf(stderr, "JLP_Decode::ReadDbleImage/Error: wrong size nx1=%d nx0=%d ny1=%d ny0=%d\n",
          nx1, nx0, ny1, ny0);
  return(-1);
  }

isize = nx1 * ny1;


// Frames are normalized in OutputFilesProcessCube(),
// otherwise they are only integrated arrays with very big numbers...
if((ngood_frames > 0) && (Files_already_saved == false)) xframes = ngood_frames;
else xframes = 1.;

/*
* ipanel_decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*/

switch (k_decode_index) {
// ShortExp:
 default:
 case 0:
    if(ShortExp_v) {
       for(i = 0; i < isize; i++) dble_image0[i] = ShortExp_v[i];
       status = 0;
       }
    break;
// Modsq:
 case 1:
    if(modsq_centered_v) {
       for(i = 0; i < isize; i++) dble_image0[i] = modsq_centered_v[i] / xframes;
       status = 0;
       }
    break;
// Autocc:
 case 2:
    if(autocc_centered_v) {
       for(i = 0; i < isize; i++) dble_image0[i] = autocc_centered_v[i] / xframes;
       status = 0;
       }
    break;
// LongInt:
 case 3:
    if(long_int) {
       for(i = 0; i < isize; i++) dble_image0[i] = long_int[i] / xframes;
       status = 0;
       }
    break;
// Quadrant:
 case 4:
    if(quadr_centered_v) {
       for(i = 0; i < isize; i++) dble_image0[i] = quadr_centered_v[i] / xframes;
       status = 0;
       }
    break;
// FlattenedAutoc:
 case 5:
    if(autocc_flattened) {
       for(i = 0; i < isize; i++) dble_image0[i] = autocc_flattened[i] / xframes;
       status = 0;
       }
    break;
// LuckyImage:
 case 6:
    if(lucky_image) {
       for(i = 0; i < isize; i++) dble_image0[i] = lucky_image[i] / xframes;
       status = 0;
       }
    break;
// DirectVector:
 case 7:
    if(direct_vector) {
       for(i = 0; i < isize; i++) dble_image0[i] = direct_vector[i] / xframes;
       status = 0;
       }
    break;
}

// In case of problem load a dummy image (used as a diagnostic...):
if(status){
 fprintf(stderr, "JLP_Decode::ReadDbleImage/Error: wrong index k_decode=%d\n",
          k_decode_index);
 for(i = 0; i < isize; i++) dble_image0[i] = -1.;
 }

return(status);
}
/************************************************************************
* Read OffsetFrame (to be displayed by Main_Refresh())
*
* OUTPUT:
* dble_image0: Offset frame array
*************************************************************************/
int JLP_Decode::ReadOffsetFrame(double *dble_image0, const int nx0,
                                const int ny0)
{
wxString buffer;
register int i;

if(OffsetFrame == NULL) return(-1);

// Check if size of dble_image0 is correct:
if(nx0 != nx1 || ny0 != ny1) {
  buffer.Printf(wxT("Inconsistent size: nx0,ny0:%d,%d, nx1,ny1=%d,%d"),
                nx0, ny0, nx1, ny1);
  wxMessageBox(buffer, wxT("JLP_Decode/ReadOffsetFrame"),
               wxOK | wxICON_ERROR);
  return(-1);
}

for(i = 0; i < nx1 * ny1; i++) dble_image0[i] = OffsetFrame[i];

return(0);
}
/************************************************************************
* Read Flat Field Frame (to be displayed by Main_Refresh())
*
* OUTPUT:
* dble_image0: flat field frame array
*************************************************************************/
int JLP_Decode::ReadFlatFieldFrame(double *dble_image0, const int nx0,
                                   const int ny0)
{
register int i;

if(FlatFieldFrame == NULL) {
   wxMessageBox(wxT("FlatFieldFrame == Null"),
               wxT("ReadFlatFieldFrame/Error"), wxICON_ERROR);
   return(-1);
   }

// Check if size of dble_image0 is correct:
if(nx0 != nx1 || ny0 != ny1) {
   wxMessageBox(wxT("Incompatible sizes"),
               wxT("ReadFlatFieldFrame/Error"), wxICON_ERROR);
   return(-1);
   }

for(i = 0; i < nx1 *ny1; i++) dble_image0[i] = FlatFieldFrame[i];

return(0);
}
/************************************************************************
* Read Unresolved autocorrelation Frame (to be displayed by Main_Refresh())
*
* OUTPUT:
* dble_image0: unres. autoc frame array
*************************************************************************/
int JLP_Decode::ReadUnresAutocFrame(double *dble_image0, const int nx0,
                                    const int ny0)
{
register int i;

if(UnresolvedAutocFrame == NULL) {
   wxMessageBox(wxT("UnresolvedAutocFrame == Null"),
               wxT("ReadUnresAutocFrame/Error"), wxICON_ERROR);
   return(-1);
   }

// Check if size of dble_image0 is correct:
if(nx0 != nx1 || ny0 != ny1) {
   wxMessageBox(wxT("Incompatible sizes"),
               wxT("ReadUnresAutocFrame/Error"), wxICON_ERROR);
   return(-1);
   }

for(i = 0; i < nx1 * ny1; i++) dble_image0[i] = UnresolvedAutocFrame[i];

return(0);
}
/************************************************************************
* Read Unresolved Modsq Frame (to be displayed by Main_Refresh())
*
* OUTPUT:
* dble_image0: modsq frame array
*************************************************************************/
int JLP_Decode::ReadUnresModsqFrame(double *dble_image0, const int nx0,
                                    const int ny0)
{

if(UnresolvedModsqFrame == NULL) {
   wxMessageBox(wxT("UnresolvedAutocFrame == Null"),
               wxT("ReadUnresModsqFrame/Error"), wxICON_ERROR);
   return(-1);
   }

// Check if size of dble_image0 is correct:
if(nx0 != nx1 || ny0 != ny1) {
   wxMessageBox(wxT("Incompatible sizes"),
               wxT("ReadUnresModsqFrame/Error"), wxICON_ERROR);
   return(-1);
   }

RECENT_FFT_DOUBLE(UnresolvedModsqFrame, dble_image0, &nx1, &ny1, &nx1);

return(0);
}
/************************************************************************
* Read PhotModsq Frame (to be displayed by Main_Refresh())
*
* OUTPUT:
* dble_image0: PhotModsq frame array
*************************************************************************/
int JLP_Decode::ReadPhotModsqFrame(double *dble_image0, const int nx0,
                                   const int ny0)
{
register int i;

if(PhotModsqFrame == NULL) {
   wxMessageBox(wxT("PhotModsqFrame == Null"),
               wxT("ReadPhotModsqFrame/Error"), wxICON_ERROR);
   return(-1);
   }

// Check if size of dble_image0 is correct:
if(nx0 != nx1 || ny0 != ny1) {
   wxMessageBox(wxT("Incompatible sizes"),
               wxT("ReadPhotModsqFrame/Error"), wxICON_ERROR);
   return(-1);
   }

for(i = 0; i < nx1 * ny1; i++) dble_image0[i] = PhotModsqFrame[i];

return(0);
}
/************************************************************************
* Read Bispectrum Frame (to be displayed by Main_Refresh())
*
* OUTPUT:
* dble_image0: Bispectrum frame array
*************************************************************************/
int JLP_Decode::ReadBispectrumFrame(double *dble_image0, const int nx0,
                                    const int ny0)
{
register int i;

if(BispectrumFrame == NULL) {
   wxMessageBox(wxT("BispectrumFrame == Null"),
               wxT("ReadBispectrumFrame/Error"), wxICON_ERROR);
   return(-1);
   }

// Check if size of dble_image0 is correct:
if(nx0 != nx1 || ny0 != ny1) {
   wxMessageBox(wxT("Incompatible sizes"),
               wxT("ReadBispectrumFrame/Error"), wxICON_ERROR);
   return(-1);
   }

for(i = 0; i < nx1 * ny1; i++) dble_image0[i] = BispectrumFrame[i];

return(0);
}
/************************************************************************
* Read RestoredImage Frame (to be displayed by Main_Refresh())
*
* OUTPUT:
* dble_image0: restored image frame array
*************************************************************************/
int JLP_Decode::ReadRestoredImageFrame(double *dble_image0, const int nx0,
                                   const int ny0)
{
register int i;

if(RestoredImageFrame == NULL) {
   wxMessageBox(wxT("RestoredImageFrame == Null"),
               wxT("ReadRestoredImageFrame/Error"), wxICON_ERROR);
   return(-1);
   }

// Check if size of dble_image0 is correct:
if(nx0 != nx1 || ny0 != ny1) {
   wxMessageBox(wxT("Incompatible sizes"),
               wxT("ReadRestoredImageFrame/Error"), wxICON_ERROR);
   return(-1);
   }

for(i = 0; i < nx1 * ny1; i++) dble_image0[i] = RestoredImageFrame[i];

return(0);
}
/************************************************************************
* Processing Andor cube (from ANDOR camera)
*
* INPUT:
* AndorCube0: data cube of frames to be processed
* nx0, ny0: size of frames
* nz0: number of frames to be processed
* icube: index of CubeImage (0 or 1)
*
* OUTPUT:
* error_message if returned status != 0
*************************************************************************/
int JLP_Decode::DC_LoadAndorCube(AWORD *AndorCube0, int nx0, int ny0,
                                 int nz0_cube, int icube)
{
wxString buffer;
register int i;

if(nx1 != nx0 || ny1 != ny0 || nz1_cube != nz0_cube) {
 buffer.Printf(wxT("Incompatible size: nx0=%d ny0=%d nz0_cube=%d (Andor setup)\
 whereas nx=%d ny=%d nz_cube=%d (decode setup)"),
          nx0, ny0, nz0_cube, nx1, ny1, nz1_cube);
  wxMessageBox(buffer, wxT("DC_LoadAndorCube/Error"), wxOK | wxICON_ERROR);
  return(-1);
  }

if(icube < 0 || icube > 1) {
 buffer.Printf(wxT("Wrong value: icube=%d"), icube);
  wxMessageBox(buffer, wxT("DC_LoadAndorCube/Error"), wxOK | wxICON_ERROR);
  return(-1);
  }

// Setup all parameters and erase all the arrays
// (only for the first time that this routine is called)
if(!DecodeInitialized) SetupProcessCube();

for(i = 0; i < nx1 * ny1 * nz1_cube; i++) CubeImage[icube][i] = AndorCube0[i];

return(0);
}
/*************************************************************************
* Extract from the full filename the directory, the radical
*
* INPUT:
* filename, with the directory
*            Exemple: "d:\test\ads2544.fits"   (if Windows)
*            Exemple: "toto/test\ads2544.fits" (if Linux)
*
* OUTPUT:
* generic_name and directory (separated)
* Exemple: "d:\test\" = directory + "ads2544" = generic_name
*************************************************************************/
int JLP_Decode::DC_ExtractDirAndGeneric(char *filename, char *generic_name,
                                        char *directory)
{
char *pc, buffer[256];
register int i;
int ilast;

// Exit if generic name "filename" is empty
if(filename[0] == '\0') return(-1);

/*********************************************************/
/* Now output of the results : */

// Exemple: "d:\test\ads2544.fits"
  strcpy(buffer, filename);
  ilast = -1;
  for(i = 0; i < 80; i++)
      {
      if(buffer[i] == '\\' || buffer[i] == '/' || buffer[i] == ':') ilast = i;
      if(!buffer[i]) break;
      }
  strcpy(directory, buffer);
  directory[ilast+1] = '\0';

  strcpy(generic_name, &buffer[ilast+1]);

/* Remove trailing blanks: */
  pc = generic_name;
  while(*pc && *pc != ' ') pc++;
  *pc='\0';
// Look for ".fits" and truncate it to form the radical:
  pc = generic_name;
  while(*pc && *pc != '.') pc++;
  *pc = '\0';

#ifdef DEBUG
  sprintf(buffer,"generic name: >%s< directory: >%s<",generic_name, directory);
  wxMessageBox(wxString(buffer), wxT("DC_ExtractDirAndGeneric"),
               wxICON_INFORMATION | wxOK);

#endif
return(0);
}
/*************************************************************************
* DC_LoadprocessingResults
*
* INPUT:
*  generic name: for the input files
*
* OUTPUT:
*  return status (0 if OK or -1 otherwise)
*************************************************************************/
int JLP_Decode::DC_LoadProcessingResults(char *generic_name,
                                         bool bispectrum_was_computed)
{
char filename[128];
wxString buffer;

if(generic_name[0] == '\0') return(-1);

// Erase all frames:
EraseProcessCube(1);

// Load processing result files:
sprintf(filename, "%s_l.fits", generic_name);
DC_LoadLongInt(filename);
sprintf(filename, "%s_m.fits", generic_name);
DC_LoadModsq(filename);
sprintf(filename, "%s_a.fits", generic_name);
DC_LoadAutoc(filename);
sprintf(filename, "%s_q.fits", generic_name);
DC_LoadQuadrant(filename);

 if(bispectrum_was_computed) {
   sprintf(filename, "%s_b.fits", generic_name);
   DC_LoadBispectrum(filename);
   }

// Set logical variable:
input_processed_frames = true;

return(0);
}
/*************************************************************************
* DC_LoadAutoc load autocorrelation from file
* Called by DC_LoadProcessingResults
*
* INPUT:
*  filename: input filename
*
* OUTPUT:
*  return status (0 if OK or -1 otherwise)
*************************************************************************/
int JLP_Decode::DC_LoadAutoc(char *filename)
{
float *image0;
int nx0, ny0, status;
char errmess[128], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

// Open FITS file and check if size is OK:
  status = readimag_float_fits(filename, &image0, &nx0, &ny0,
                               comments, errmess);
  if(status){
    buffer = wxString(errmess);
    wxMessageBox(buffer, wxT("DC_LoadAutoc/Error"), wxOK | wxICON_ERROR);
    return(-1);
  }
// If size is not OK, return with error message:
  if((nx0 < nx1) || (ny0 < ny1)) {
    buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
                  nx0, ny0, nx1, ny1);
    wxMessageBox(buffer, wxT("DC_LoadAutoc/Error"), wxOK | wxICON_ERROR);

    status = -1;
    }
  else {
// IF OK:
   if(autocc != NULL) delete[] autocc;
   if(autocc_centered_v != NULL) delete[] autocc_centered_v;
   autocc = new double[nx1 * ny1];
   autocc_centered_v = new double[nx1 * ny1];
   CopyFromBiggerArray(image0, autocc, nx0, ny0, nx1, ny1);
   CopyFromBiggerArray(image0, autocc_centered_v, nx0, ny0, nx1, ny1);
   status = 0;
  }

// Free memory:
delete[] image0;
return(status);
}
/*************************************************************************
* DC_LoadLongInt load long integration from file
* Called by DC_LoadProcessingResults
*
* INPUT:
*  filename: input filename
*
* OUTPUT:
*  return status (0 if OK or -1 otherwise)
*************************************************************************/
int JLP_Decode::DC_LoadLongInt(char *filename)
{
float *image0;
int nx0, ny0, status;
char errmess[128], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

// Open FITS file and check if size is OK:
  status = readimag_float_fits(filename, &image0, &nx0, &ny0,
                               comments, errmess);
  if(status){
    buffer = wxString(errmess);
    wxMessageBox(buffer, wxT("DC_LoadLongInt/Error"), wxOK | wxICON_ERROR);
    return(-1);
  }
// If size is not OK, return with error message:
  if((nx0 < nx1) || (ny0 < ny1)) {
    buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
                  nx0, ny0, nx1, ny1);
    wxMessageBox(buffer, wxT("DC_LoadLongInt/Error"), wxOK | wxICON_ERROR);

    status = -1;
    }
  else {
// IF OK:
   if(long_int != NULL) delete[] long_int;
   long_int = new double[nx1 * ny1];
   CopyFromBiggerArray(image0, long_int, nx0, ny0, nx1, ny1);
   status = 0;
  }

// Free memory:
delete[] image0;
return(status);
}
/*************************************************************************
* DC_LoadModsq load modsq from file
* Called by DC_LoadProcessingResults
*
* INPUT:
*  filename: input filename
*
* OUTPUT:
*  return status (0 if OK or -1 otherwise)
*************************************************************************/
int JLP_Decode::DC_LoadModsq(char *filename)
{
float *image0;
int nx0, ny0, status;
char errmess[128], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

// Open FITS file and check if size is OK:
  status = readimag_float_fits(filename, &image0, &nx0, &ny0,
                               comments, errmess);
  if(status){
    buffer = wxString(errmess);
    wxMessageBox(buffer, wxT("DC_LoadModsq/Error"), wxOK | wxICON_ERROR);
    return(-1);
  }
// If size is not OK, return with error message:
  if((nx0 < nx1) || (ny0 < ny1)) {
    buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
                  nx0, ny0, nx1, ny1);
    wxMessageBox(buffer, wxT("DC_LoadModsq/Error"), wxOK | wxICON_ERROR);

    status = -1;
    }
  else {
// IF OK:
   if(modsq != NULL) delete[] modsq;
   if(modsq_centered_v != NULL) delete[] modsq_centered_v;
   modsq = new double[nx1 * ny1];
   modsq_centered_v = new double[nx1 * ny1];
   CopyFromBiggerArray(image0, modsq, nx0, ny0, nx1, ny1);
   CopyFromBiggerArray(image0, modsq_centered_v, nx0, ny0, nx1, ny1);
   status = 0;
  }

// Free memory:
delete[] image0;
return(status);
}
/*************************************************************************
* DC_LoadBispectrum load modsq from file
* Called by DC_LoadBispectrum
*
* INPUT:
*  filename: input filename
*
* OUTPUT:
*  return status (0 if OK or -1 otherwise)
*************************************************************************/
int JLP_Decode::DC_LoadBispectrum(char *filename)
{
float *image0;
int nx0, ny0, status;
register int i;
char errmess[128], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

// Open FITS file and check if size is OK:
  status = readimag_float_fits(filename, &image0, &nx0, &ny0,
                               comments, errmess);
  if(status){
    buffer = wxString(errmess);
    wxMessageBox(buffer, wxT("DC_LoadBispectrum/Error"), wxOK | wxICON_ERROR);
    return(-1);
  }
// First check that the size of the input bispectrum is compatible
// with the current settings:
  if(ny0 != 3) {
    buffer.Printf(wxT("Incompatible size ny0=%d. Should be 3"), ny0);
    wxMessageBox(buffer, wxT("DC_LoadBispectrum"), wxOK | wxICON_ERROR);
    return(-1);
  }
  if(nx0 != ngamma) {
    ngamma = nx0;
  }

// IF OK:
 if(bispp != NULL) delete[] bispp;
 bispp = new double[ngamma * 4];
 for(i = 0; i < ngamma * 3; i++) bispp[i] = (double)image0[i];

// Free memory:
delete[] image0;
return(0);
}
/*************************************************************************
* DC_LoadQuadrant load quadrant from file
* Called by DC_LoadQuadrant
*
* INPUT:
*  filename: input filename
*
* OUTPUT:
*  return status (0 if OK or -1 otherwise)
*************************************************************************/
int JLP_Decode::DC_LoadQuadrant(char *filename)
{
float *image0;
int nx0, ny0, status;
char errmess[128], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

// Open FITS file and check if size is OK:
  status = readimag_float_fits(filename, &image0, &nx0, &ny0,
                               comments, errmess);
  if(status){
    buffer = wxString(errmess);
    wxMessageBox(buffer, wxT("DC_LoadQuadrant/Error"), wxOK | wxICON_ERROR);
    return(-1);
  }
// If size is not OK, return with error message:
  if((nx0 < nx1) || (ny0 < ny1)) {
    buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
                  nx0, ny0, nx1, ny1);
    wxMessageBox(buffer, wxT("DC_LoadQuadrant/Error"), wxOK | wxICON_ERROR);

    status = -1;
    }
  else {
// IF OK:
   if(quadr_centered_v != NULL) delete[] quadr_centered_v;
   quadr_centered_v = new double[nx1 * ny1];
   CopyFromBiggerArray(image0, quadr_centered_v, nx0, ny0, nx1, ny1);
   status = 0;
  }

// Free memory:
delete[] image0;
return(status);
}
/*************************************************************************
* Open offset frame, read the image and store it into OffsetFrame[]
**************************************************************************/
int JLP_Decode::DC_LoadOffsetFrame(char *filename)
{
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

   if(OffsetFrame != NULL) {
     delete[] OffsetFrame;
     OffsetFrame = NULL;
     }

// Open FITS file and check if size is OK:
      status = readimag_float_fits(filename, &image0,
                                   &nx0, &ny0, comments, errmess);
      if(status){
         wxMessageBox(wxString(errmess), wxT("DC_LoadOffsetFrame/Error"),
                      wxOK | wxICON_ERROR);
        return(-1);
        }
// If size is not OK, return with error message:
     if((nx0 < nx1) || (ny0 < ny1)) {
       buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
               nx0, ny0, nx1, ny1);
       wxMessageBox(buffer, wxT("DC_LoadOffsetFrame/Error"),
                    wxOK | wxICON_ERROR);
// Free memory:
       delete[] image0;
       return(-1);
       }
     else {
// IF OK:
       OffsetFrame = new double[nx1 * ny1];
       CopyFromBiggerArray(image0, OffsetFrame, nx0, ny0, nx1, ny1);
       delete[] image0;
       }

strcpy(Pset1.OffsetFileName, filename);
return(0);
}
/*************************************************************************
* Open flat field frame, read the image and store a "cleaned" version
* into FlatFieldFrame[]
**************************************************************************/
int JLP_Decode::DC_LoadFlatFieldFrame(char *filename)
{
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

   if(FlatFieldFrame != NULL) {
     delete[] FlatFieldFrame;
     FlatFieldFrame = NULL;
     }

// File obtained by Marco in March 2008:
// strcpy(FlatFieldFileName,"170308_median_l.fits");

// Open FITS file and check if size is OK:
      status = readimag_float_fits(filename, &image0,
                                   &nx0, &ny0, comments, errmess);
      if(status){
        wxMessageBox(wxString(errmess), wxT("DC_LoadFlatFieldFrame/Error"),
                     wxICON_ERROR);
        return(-1);
        }
// If size is not OK, return with error message:
    if((nx0 < nx1) || (ny0 < ny1)) {
       buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
               nx0, ny0, nx1, ny1);
       wxMessageBox(wxString(errmess), wxT("DC_LoadFlatFieldFrame/Error"),
                    wxICON_ERROR);
// Free memory:
       delete[] image0;
       return(-1);
       }
     else {
// IF OK:
       FlatFieldFrame = new double[nx1 * ny1];
       CopyFromBiggerArray(image0, FlatFieldFrame, nx0, ny0, nx1, ny1);
       delete[] image0;
// Clip Flat Field to reduce the range and subsequent errors:
       ClipFlatField();
       }

strcpy(Pset1.FlatFieldFileName, filename);
return(0);
}
/*************************************************************************
* Open photon modsq frame
**************************************************************************/
int JLP_Decode::DC_LoadPhotModsqFrame(char *filename)
{
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

   if(PhotModsqFrame != NULL) {
     delete[] PhotModsqFrame;
     PhotModsqFrame = NULL;
     }

// Open FITS file and check if size is OK:
      status = readimag_float_fits(filename, &image0,
                                   &nx0, &ny0, comments, errmess);
      if(status){
        wxMessageBox(wxString(errmess), wxT("DC_LoadPhotModsqFrame/Error"),
                     wxICON_ERROR);
        return(-1);
        }
// If size is not OK, return with error message:
    if((nx0 < nx1) || (ny0 < ny1)) {
      buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
               nx0, ny0, nx1, ny1);
       wxMessageBox(wxString(errmess), wxT("DC_LoadPhotModsqFrame/Error"),
                    wxICON_ERROR);
// Free memory:
       delete[] image0;
       return(-1);
       }
     else {
// If OK:
       PhotModsqFrame = new double[nx1 * ny1];
       CopyFromBiggerArray(image0, PhotModsqFrame, nx0, ny0, nx1, ny1);
       delete[] image0;
       }

strcpy(Pset1.PhotModsqFileName, filename);
return(0);
}
/*************************************************************************
* Open unresolved autocorrelation frame, read the image
* and store it into UnresolvedAutocFrame[]
**************************************************************************/
int JLP_Decode::DC_LoadUnresolvedAutocFrame(char *filename)
{
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

if(UnresolvedAutocFrame != NULL) {
  delete[] UnresolvedAutocFrame;
  UnresolvedAutocFrame = NULL;
}

// Open FITS file and check if size is OK:
status = readimag_float_fits(filename, &image0,
                             &nx0, &ny0, comments, errmess);
if(status){
    wxMessageBox(wxString(errmess), wxT("DC_LoadUnresolvedAutocFrame/Error"),
                 wxICON_ERROR);
    return(-1);
}
// If size is not OK, return with error message:
  if((nx0 < nx1) || (ny0 < ny1)) {
    buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
           nx0, ny0, nx1, ny1);
   wxMessageBox(wxString(errmess), wxT("DC_LoadUnresolvedAutocFrame/Error"),
                wxICON_ERROR);
// Free memory:
   delete[] image0;
   return(-1);
} else {
// If OK:
   UnresolvedAutocFrame = new double[nx1 * ny1];
   CopyFromBiggerArray(image0, UnresolvedAutocFrame, nx0, ny0, nx1, ny1);
   delete[] image0;
}

strcpy(Pset1.UnresolvedAutocName, filename);
return(0);
}
/*************************************************************************
* Open unresolved modsq frame, read the image
* and store it into UnresolvedModsqFrame[]
**************************************************************************/
int JLP_Decode::DC_LoadUnresolvedModsqFrame(char *filename)
{
float *f_image0;
double *d_image0;
int nx0, ny0, status;
double ww, ww1, ww2;
int nn, imax, jmax;
register int i, j;
char errmess[256], comments[80];
wxString buffer;

if(filename[0] == '\0') return(-1);

SigmaUnresolvedModsq = 1.e-6;

if(UnresolvedModsqFrame != NULL) {
   delete[] UnresolvedModsqFrame;
   UnresolvedModsqFrame = NULL;
}

// Open FITS file and check if size is OK:
status = readimag_float_fits(filename, &f_image0,
                             &nx0, &ny0, comments, errmess);
if(status){
  wxMessageBox(wxString(errmess), wxT("DC_LoadUnresolvedModsqFrame/Error"),
               wxICON_ERROR);
  return(-1);
}

// If size is not OK, return with error message:
  if((nx0 < nx1) || (ny0 < ny1)) {
    buffer.Printf(wxT("Size: nx0=%d ny0=%d is too small: window size is nx=%d ny=%d"),
           nx0, ny0, nx1, ny1);
  wxMessageBox(wxString(errmess), wxT("DC_LoadUnresolvedModsqFrame/Error"),
               wxICON_ERROR);

// Free memory:
       delete[] f_image0;
       return(-1);
       }

// If OK:
UnresolvedModsqFrame = new double[nx1 * ny1];
d_image0 = new double[nx1 * ny1];

CopyFromBiggerArray(f_image0, d_image0, nx0, ny0, nx1, ny1);

// Compute sigma in the lower left corner:
nn = 0;

jmax = MAXI(8, ny1/16);
imax = MAXI(8, nx1/16);
ww1 = 0.;
ww2 = 0.;
nn = 0;
for(j = 0; j < jmax; j++) {
 for(i = 0; i < imax; i++) {
   ww = d_image0[i + j * nx1];
   ww1 += ww;
   ww2 += ww * ww;
   nn++;
   }
 }
if(nn > 3) {
  SigmaUnresolvedModsq = ww2/(double)nn - SQUARE(ww1/(double)nn);
  SigmaUnresolvedModsq = MAXI(SigmaUnresolvedModsq, 1.e-9);
  SigmaUnresolvedModsq = sqrt(SigmaUnresolvedModsq);
  }

// Recenter it since it will be used with c_re, c_im:
RECENT_FFT_DOUBLE(d_image0, UnresolvedModsqFrame, &nx1, &ny1, &nx1);

strcpy(Pset1.UnresolvedModsqName, filename);

delete[] f_image0;
delete[] d_image0;
return(0);
}
/************************************************************************
* Get message for Status Bar
*
* OUTPUT:
* message2
*************************************************************************/
int JLP_Decode::GetMessageForStatusBar(wxString &message2)
{
wxString buff0;
char cdate[40], ctime[20], csaved[40], cphotons[64];
float rate;

// When only one image, assumes the data has been read from a file:
if(PhotonsPerFrame > 0)
 sprintf(cphotons, "Photons/frame=%.1f", PhotonsPerFrame);
else
 cphotons[0] = '\0';

// Give information about the status of the current data:
if(Files_already_saved)
// i=503 "(already saved)"
 strcpy(csaved, m_messg[503].mb_str());
else
// i=504 "(not yet saved)"
 strcpy(csaved, m_messg[504].mb_str());

// Read time and date:
switch(iLang) {
 default:
 case 0:
   jlp_english_date(cdate, ctime, Pset1.utime_offset);
   break;
 case 1:
   jlp_french_date(cdate, ctime, Pset1.utime_offset);
   break;
 case 2:
   jlp_italian_date(cdate, ctime, Pset1.utime_offset);
   break;
}

// Case when the user has loaded the bispectrum and autocorrelation from files:
// if(ngood_frames == 1) {
//    message2.Printf(wxT("File: %s    \t %s"), Pset1.InputFileName, cphotons);
//    return(0);
// }

// Case when data was directly processed by the program:
// i=505 "Number of frames:"
   message2.Printf(wxT("%s %d"),
             m_messg[505], (int)ngood_frames);
  if(Pset1.Selection_fwhm || Pset1.Selection_maxval) {
    if(ngood_frames > 0 || nbad_frames != 0)
      rate = 100. * ngood_frames / ((float)nbad_frames + ngood_frames);
    else rate = 100.;
// i=506 "selection"
    buff0.Printf(wxT(" (%s=%d%%)"), m_messg[506], (int)rate);
    message2.Append(buff0);
  }

  buff0.Printf(wxT(" %s  %s %s %s"),
               csaved, cdate, ctime, cphotons);
  message2.Append(buff0);

/*
  wxMessageBox(message2, wxT("MessageForStatusBar/DDEBUG"),
               wxICON_INFORMATION | wxOK);
*/
return(0);
}

/************************************************************************
* Update ShortExp_v for further display
* Copy input data if correct size,
* write 0 otherwise...
*************************************************************************/
int JLP_Decode::UpdateShortExp_v(AWORD *short_exposure0, int nx0, int ny0)
{
int nx_ny, status = 0;
register int i;

nx_ny = nx1 * ny1;

 if(nx0 == nx1 && ny0 == ny1) {
   for(i = 0; i < nx_ny; i++) ShortExp_v[i] = (double)short_exposure0[i];
 } else {
   status = -1;
   for(i = 0; i < nx_ny; i++) ShortExp_v[i] = 0.;
 }

return(status);
}
/*************************************************************************
* Copy the inner part of an array
*
*************************************************************************/
static int CopyFromBiggerArray(float *ima_in, double *ima_out, int nx_in, int ny_in,
                               int nx_out, int ny_out)
{
int i, j, i0, j0, status = -1;

i0 = (nx_in - nx_out) / 2;
j0 = (ny_in - ny_out) / 2;
if((i0 >= 0) && (j0 >=0)) {
  for(j = 0; j < ny_out; j++)
    for(i = 0; i < nx_out; i++)
      ima_out[i + j * nx_out] = (double)ima_in[i + i0 + (j + j0) * nx_in];
  status = 0;
  }

return(status);
}
