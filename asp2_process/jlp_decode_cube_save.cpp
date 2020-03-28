/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* jlp_decod_cube_save.cpp
* JLP_Decode Class
*
* int DC_Open_outFITS_3DFile()
* int SaveToCubeOfGoodFrames(AWORD *CubeImage, int nx1, int ny1,
*                            int good_frame)
* int SaveCubeToFITSFile(AWORD* CubeGoodFrames, int nx, int ny,
*                        int nz_CubeOfGoodFrames)
* int Close_outFITS_3DFile_and_Save(char *generic_name,
*                                   char *directory, char *comments,
*                                   DESCRIPTORS descrip0, bool expand_files)
* int OutputFilesProcessCube(char* generic_name, char* directory,
*                            char* comments)
*
* JLP
* Version 06-09-2015
---------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

//#include "jlp_wx_ipanel_filters.h" // subtract_back_model ...
//#include "jlp_numeric.h"           // fft_2D, ... "fftw3.h", etc

#include "jlp_time0.h"             // jlp_italian_date ...

#include "asp2_photon_corr.h"      // definition of clean_deconvolution, ...
#include "asp2_covermas.h"
#include "jlp_decode.h"
#include "jlp_andor_utils.h"       // Write_AndorSettingsToFITSDescriptors()
#include "asp2_3D_outfits.h"

/* Cannot be used in debug mode
#define DEBUG
*/
static int check_if_file_is_there(char *filename, bool *return_immediately);
/************************************************************************
* Open temporary output 3D FITS cube
* to record the elementary frames (short exposures)
*************************************************************************/
int JLP_Decode::DC_Open_outFITS_3DFile()
{
int status = 0, bitpix = 16;
char errmess[256], comments[80], directory[80], generic_name[80];
DESCRIPTORS descrip02;

descrip02.ndescr = 0;

// Close output file if necessary:
if(outFITS_3DFile) {
  delete outFITS_3DFile;
  outFITS_3DFile = NULL;
  }

// Dummy values:
   strcpy(comments, "Temporary 3D FITS cube with elementary frames");

// Generate filename:
strcpy(directory, Pset1.out3DFITS_directory);
strcpy(generic_name, "AndorSpeck2");

if(!strcmp(directory, ".") || directory[0] == '\0') {
 sprintf(out3DFITS_fname, "%s_cube.fits", generic_name);
} else {
#ifdef LINUX
 sprintf(out3DFITS_fname, "%s/%s_cube.fits", directory, generic_name);
#else
 sprintf(out3DFITS_fname, "%s\\%s_cube.fits", directory, generic_name);
#endif
}

// Will open and then write to a temporary file:
/* Open output FITS file to save the elementary frames:*/
outFITS_3DFile = new JLP_3D_outFITSFile(out3DFITS_fname, nx1, ny1,
                                        Pset1.out3DFITS_nz_max,
                                        comments, descrip02, bitpix, errmess);
if(!outFITS_3DFile->IsOpen()) {
  status = -1;
  outFITS_3DFile = NULL;
  }

return(status);
}
/**************************************************************************
* Save data cube to CubeGoodFrames:
***************************************************************************/
int JLP_Decode::SaveToCubeOfGoodFrames(AWORD *CubeImage0, int nx0, int ny0,
                                       int good_frame)
{
int i0;
register int i;

// Return -1 if bad size:
  if((nx0 != nx1) || (ny0 != ny1)) {
    fprintf(stderr, "SaveToCubeOfGoodFrames/Error inconsistent size!\n");
    return(-1);
  }

// Return -1 if option is disabled:
  if(Pset1.SaveFramesToFitsFile == 0) return(-1);

// Return 0 if no good frame and SaveSelectionOnly is true:
  if(!good_frame && Pset1.SaveSelectionOnlyToFitsFile) return(0);

// Otherwise load current image to CubeOfGoodFrames:
  i0 = nx1 * ny1 * nz1_CubeOfGoodFrames;

  for(i = 0; i < nx1 * ny1; i++) CubeOfGoodFrames[i0 + i] = (double)CubeImage0[i];

// Update the number of planes in CubeOfGoodFrames:
  nz1_CubeOfGoodFrames++;

return(0);
}
/**************************************************************
* Save data cube to FITS file
**************************************************************/
int JLP_Decode::SaveCubeToFITSFile(AWORD* CubeGoodFrames, int nx, int ny,
                                   int nz_CubeOfGoodFrames)
{
int status;
char err_messg[256];

// JLP2010: option to save frames to 3DFits files or not:
if(Pset1.SaveFramesToFitsFile == 0) return(-1);

if(outFITS_3DFile == NULL)
 {
  wxMessageBox(wxT("File not opened"), wxT("SaveCubeToFITS/Error"),
               wxICON_ERROR);
  return(-1);
 }

// Return if no data to be saved:
if(nz_CubeOfGoodFrames <= 0) return(0);

status = outFITS_3DFile->WriteAwordCube_to_3D_cube(CubeGoodFrames, nx, ny,
                                                   nz_CubeOfGoodFrames,
                                                   err_messg);
if(status < 0)
  wxMessageBox(wxString(err_messg), wxT("SaveCubeToFITS/Error"),
               wxICON_ERROR);
if(status > 1)
// If file is full (should never happen), close the output FITS 3D file:
if(outFITS_3DFile->IsFull()) {
//  wxMessageBox(wxT("FITSFile is full"), wxT("SaveCubeToFITS/Error"),
//               wxICON_ERROR);
  status = -2;
}

return(status);
}
/*************************************************************************
* Close the "outFITS_3DFile" (= temporary 3D FITS file),
* and save the frames contained in this file to a new 3D FITS file
* Called by OutputFilesProcessCube or directly from the menu by the user
*
* INPUT:
* directory, generic_name, comments
* descrip0: descrip0tors to be written as FITS keywords
* expand_files: (if true save to individual 2D files
*                otherwise save to 3D FITS file)
*************************************************************************/
int JLP_Decode::Close_outFITS_3DFile_and_Save(char *generic_name,
                               char *directory, char *comments,
                               DESCRIPTORS descrip0, bool expand_files)
{
int status, bitpix;
char error_messg[256];

// Return from here if frames should not be saved:
  if(Pset1.SaveFramesToFitsFile == 0) return(-1);

// Check if already saved:
  if(CubeSavedToFile) {
    wxMessageBox(wxT("Error: outFITS_3DFile already saved!"),
                 wxT("Close_outFITS_3DFile_and_Save"), wxICON_ERROR);
    return(-1);
    }

// Close FITS file if not done yet:
  if(outFITS_3DFile == NULL) {
    wxMessageBox(wxT("Error: outFITS_3DFile already closed!"),
                 wxT("Close_outFITS_3DFile_and_Save"), wxICON_ERROR);
    return(-1);
    }

// Save to file:
  bitpix = 16;
  if(expand_files){
    status = Copy_outFITS_3DFile_to_2DFITSFiles(outFITS_3DFile, generic_name,
                                                directory, comments,
                                                descrip0,
                                                Pset1, bitpix, error_messg);
  } else {
    status = Copy_outFITS_3DFile_to_3DFITSFile(outFITS_3DFile, generic_name,
                                               directory, comments,
                                               descrip0,
                                               Pset1, bitpix, error_messg);
  }
  if(status) {
    wxMessageBox(wxString(error_messg),
                 wxT("Close_outFITS_3DFile_and_Save/Error"), wxICON_ERROR);
    return(-1);
    }

// Close file:
  delete outFITS_3DFile;
  outFITS_3DFile = NULL;

// Indicate the data was saved:
  CubeSavedToFile = true;

return(0);
}
/*******************************************************************
* Save processed data to FITS files
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
* INPUT:
* generic_name: "generic name" used to generate the names of the ouput files
********************************************************************/
int JLP_Decode::OutputFilesProcessCube(char* generic_name, char* directory,
                                       char* comments)
{
/* Images nx*ny, irmax=25, ngmax=187566 : */
double *out_bisp, *tmp2;
int nx2, ny2, status, k_d;
register int i;
char filename[256], fname[256], processing_param[60], buffer[512];
bool expand_files, return_immediately;
DESCRIPTORS descrip0;
descrip0.ndescr = 0;

// Generate basic name to which the extensions will be added later:
if(!strcmp(directory, ".") || directory[0] == '\0') {
 strcpy(fname, generic_name);
} else {
#ifdef LINUX
 sprintf(fname, "%s/%s", directory, generic_name);
#else
 sprintf(fname, "%s\\%s", directory, generic_name);
#endif
}

// Copy input descriptors to temporary descriptors:
  descrip0.ndescr = descrip1.ndescr;
  for(i = 0; i < descrip1.ndescr; i++) {
        strcpy(descrip0.descr[i].cvalue, descrip1.descr[i].cvalue);
        strcpy(descrip0.descr[i].keyword, descrip1.descr[i].keyword);
  }
  k_d = descrip1.ndescr;

  if(!DecodeInitialized) return(-1);

/* Check that some frames have been selected */
 if(ngood_frames <= 0) {
    wxMessageBox(wxT("Sorry: not any image processed yet"),
                 wxT("OutputFilesProcessCube"), wxICON_ERROR);
   return(-1);
#ifdef DEBUG
 } else {
    sprintf(buffer,"%d frames have been processed", (int)ngood_frames);
    wxMessageBox(wxString(buffer), wxT("OutputFilesProcessCube"),
                 wxICON_INFORMATION | wxOK);
#endif
 }

// From September 2007: I am more precise:
   sprintf(processing_param,"n=%d Thresh=%d Offset=%d FField=%d",
           (int)ngood_frames, Pset1.OffsetThreshold,
           (int)Pset1.OffsetFrameCorrection,
           (int)Pset1.FlatFieldCorrection);

/* Data cube with all images : */
 if((outFITS_3DFile != NULL) && (Pset1.SaveFramesToFitsFile != 0)) {
    sprintf((descrip0.descr[k_d]).cvalue,"Data cube %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword,"DESCRIP ");
    descrip0.ndescr = k_d + 1;

// Build a name using to user's choice:
    expand_files = false;
    Close_outFITS_3DFile_and_Save(generic_name, Pset1.out3DFITS_directory,
                                  comments, descrip0, expand_files);
  } // EOF outFITS_3DFile != NULL

// Exit from here if no processing (only numerisation):
  if(Pset1.ProcessingMode == 0) {
// To prevent twice output:
    Files_already_saved = true;
    return(0);
    }

/* Compute statistics
* FwhmStats and MeanStats character strings with information about
* FWHM and mean level statistics
*/
 status = ComputeStatistics();
 if(!status) {
  sprintf(buffer,"OutputFilesProcessCube/Statistics \n\n\
Preprocessing: Thresh=%d Offset=%d FField=%d\n\n\
Frame selection with MaxVal = %d (min=%.1f max=%.1f)\n\n\
Frame selection with FWHM = %d (min=%.1f max=%.1f)\n\n\
############ Results obtained: ############ \n\n\
 %s\n\n %s",
  Pset1.OffsetThreshold, (int)Pset1.OffsetFrameCorrection,
  (int)Pset1.FlatFieldCorrection,
  Pset1.Selection_maxval, Pset1.MaxValSelect.LowCut, Pset1.MaxValSelect.HighCut,
  Pset1.Selection_fwhm, Pset1.FwhmSelect.LowCut, Pset1.FwhmSelect.HighCut,
  Pset1.MaxValSelect.Stats, Pset1.FwhmSelect.Stats);
  wxMessageBox(wxString(buffer), wxT("OutputFilesProcessCube/Statistics"),
               wxICON_INFORMATION | wxOK);
  }

// If already output, exit:
  if(Files_already_saved) {
     wxMessageBox(wxT("Error: cannot save the images twice!"),
                 wxT("OutputFilesProcessCube"), wxICON_ERROR);
     return(-1);
     }

/* Long integration : */
  for(i = 0; i < nx1 * ny1; i++) long_int[i] /= ngood_frames;
    sprintf(filename,"%s_l.fits", fname);

// JLP2016: check if file is already there:
    check_if_file_is_there(filename, &return_immediately);
    if(return_immediately) {
      wxMessageBox(wxT("OK will NOT delete file"), wxT("DEBUG ZZZ"), wxOK);
      return(-1);
    }
    sprintf((descrip0.descr[k_d]).cvalue, "Long int. %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
    descrip0.ndescr = k_d + 1;
    JLP_D_WRITEIMAG(long_int,nx1,ny1,filename,comments,descrip0, Pset1);

/* Lucky imaging */
if(Pset1.LuckyImaging && (lucky_image != NULL)) {
  for(i = 0; i < nx1 * ny1; i++) lucky_image[i] /= ngood_frames;
  sprintf(filename,"%s_lucky.fits", fname);
  sprintf((descrip0.descr[k_d]).cvalue, "LuckyImage %s", processing_param);
  strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
  descrip0.ndescr = k_d + 1;
  JLP_D_WRITEIMAG(lucky_image,nx1,ny1,filename,comments,descrip0, Pset1);
}

/* Direct Vector */
if(Pset1.DirectVector && (direct_vector != NULL)) {
  for(i = 0; i < nx1 * ny1; i++) direct_vector[i] /= ngood_frames;
  sprintf(filename,"%s_dvect.fits", fname);
  sprintf((descrip0.descr[k_d]).cvalue, "DirVector %s", processing_param);
  strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
  descrip0.ndescr = k_d + 1;
  JLP_D_WRITEIMAG(direct_vector,nx1,ny1,filename,comments,descrip0, Pset1);
}

if(Pset1.ProcessingMode == 2 || Pset1.ProcessingMode == 3 || Pset1.ProcessingMode == 5) {
/* Auto-corr - Cross-corr. : */
  for(i = 0; i < nx1 * ny1; i++) autocc_centered_v[i] /= ngood_frames;
    sprintf(filename,"%s_a.fits", fname);
    sprintf((descrip0.descr[k_d]).cvalue, "Autoc - Crossc %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
    descrip0.ndescr = k_d + 1;
    JLP_D_WRITEIMAG(autocc_centered_v,nx1,ny1,filename,comments,descrip0, Pset1);

  }

/* Quadrant : */
if(Pset1.ProcessingMode == 3 || Pset1.ProcessingMode == 5) {
  for(i = 0; i < nx1 * ny1; i++) quadr_centered_v[i] /= ngood_frames;
    sprintf(filename,"%s_q.fits", fname);
    sprintf((descrip0.descr[k_d]).cvalue, "Quadrant file %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
    descrip0.ndescr = k_d + 1;
    JLP_D_WRITEIMAG(quadr_centered_v,nx1,ny1,filename,comments,descrip0, Pset1);
  }

/* Autocorrelation mode with or without quadrant: */
if(Pset1.ProcessingMode == 2 || Pset1.ProcessingMode == 3) {
/* Compute the mean squared modulus : */
   for(i = 0; i < nx1 * ny1; i++) modsq[i] /= ngood_frames;
    RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx1, &ny1, &nx1);
    sprintf(filename,"%s_m.fits", fname);
    sprintf((descrip0.descr[k_d]).cvalue, "Power spectrum %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
    descrip0.ndescr = k_d + 1;
    JLP_D_WRITEIMAG(modsq_centered_v,nx1,ny1,filename,comments,descrip0, Pset1);
  }

/* Compute snrm, SNR of bispectrum, etc: */
if(Pset1.ProcessingMode == 4 || Pset1.ProcessingMode == 5) {
// "prepare_output_bisp" is contained in "covermas.cpp"
// WARNING: "prepare_output_bisp" modify modsq, bispp, snrm, etc,
// when computing their average values
    prepare_output_bisp(bispp, modsq, snrm, nx1, ny1, ngood_frames, nbeta, ngamma);

/* Mean squared modulus : */
    RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx1, &ny1, &nx1);
    sprintf(filename,"%s_m.fits", fname);
    sprintf((descrip0.descr[k_d]).cvalue, "Power spectrum %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
    descrip0.ndescr = k_d + 1;
    JLP_D_WRITEIMAG(modsq_centered_v,nx1,ny1,filename,comments,descrip0, Pset1);


/* SNR of squared modulus (actually 1/sigma if no photon correction): */
    RECENT_FFT_DOUBLE(snrm, snrm, &nx1, &ny1, &nx1);
    sprintf(filename,"%s_snrm.fits", fname);
    sprintf((descrip0.descr[k_d]).cvalue, "SNR of power sp. %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
    descrip0.ndescr = k_d + 1;
    JLP_D_WRITEIMAG(snrm,nx1,ny1,filename,comments,descrip0, Pset1);

/* Bispectrum : */
    out_bisp = new double[3 * ngamma];
    output_bisp(bispp,out_bisp,ngamma);
    sprintf(filename,"%s_b.fits", fname);
    sprintf((descrip0.descr[k_d]).cvalue, "Bispectrum %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
    descrip0.ndescr = k_d + 1;
    JLP_D_WRITEIMAG(out_bisp,ngamma,3,filename,comments,descrip0, Pset1);
   delete[] out_bisp;

/* Save restored image: */
   if(RestoredImageFrame != NULL) {
     sprintf(filename,"%s_r.fits", fname);
     sprintf((descrip0.descr[k_d]).cvalue, "Restored im. %s Ph/fr=%.1f",
             processing_param, PhotonsPerFrame);
     strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
     descrip0.ndescr = k_d + 1;
     JLP_D_WRITEIMAG(RestoredImageFrame,nx1,ny1,filename,comments,descrip0, Pset1);
     }
   }


/******************************** Scidar : ******************************/
if(Pset1.ProcessingMode == 7 || Pset1.ProcessingMode == 8) {
  nx2 = 2 * nx1; ny2 = 2 * ny1;
// Computing the autocorrelation (FFT-1 from the cross-spectrum):
  for(i = 0; i < nx2 * ny2; i++) {
      c_re(cplx_data[i]) = modsq[i];
      c_im(cplx_data[i]) = 0;
      }
// JLP99: warning: NY,NX!!!!
  fftw_fast(cplx_data, ny2, nx2, -1);
  tmp2 = new double[nx2*ny2];
  for(i = 0; i < nx2 * ny2; i++) tmp2[i] = c_re(cplx_data[i])/ngood_frames;
  RECENT_FFT_DOUBLE(tmp2, tmp2, &nx2, &ny2, &nx2);

/* Autoc: */
  sprintf(filename,"%s_a.fits", fname);
  sprintf((descrip0.descr[k_d]).cvalue, "Autocorrelation %s", processing_param);
  strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
  descrip0.ndescr = k_d + 1;
  JLP_D_WRITEIMAG(tmp2,nx2,ny2,filename,comments,descrip0, Pset1);
  delete[] tmp2;

/* Mean squared modulus : */
  for(i = 0; i < nx2 * ny2; i++) modsq[i] /= ngood_frames;
   RECENT_FFT_DOUBLE(modsq, modsq_centered_v, &nx2, &ny2, &nx2);
   sprintf(filename,"%s_m.fits", fname);
   sprintf((descrip0.descr[k_d]).cvalue, "Power spectrum %s", processing_param);
   strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
   descrip0.ndescr = k_d + 1;
   JLP_D_WRITEIMAG(modsq_centered_v,nx2,ny2,filename,comments,descrip0, Pset1);

#ifdef SCIDAR_CROSSC
/* Crosscorrelation: */
   for(i = 0; i < nx2 * ny2; i++) crossc[i] /= ngood_frames;
   sprintf(filename,"%s_i_40ms.fits", fname);
   sprintf((descrip0.descr[k_d]).cvalue, "Crosscor. 40ms %s", processing_param);
   strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
   descrip0.ndescr = k_d + 1;
   JLP_D_WRITEIMAG(crossc,nx2,ny2,filename,comments,descrip0, Pset1);
#endif

/* Scidar: Long integration of the fluctuations: (Scidar Log mode only)*/
  if(Pset1.ProcessingMode == 7) {
    for(i = 0; i < nx1 * ny1; i++) scidar_lf[i] /= ngood_frames;
    sprintf(filename,"%s_lf.fits", fname);
    sprintf((descrip0.descr[k_d]).cvalue, "Integ. fluctuations %s", processing_param);
    strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
    descrip0.ndescr = k_d + 1;
    JLP_D_WRITEIMAG(scidar_lf,nx1,ny1,filename,comments,descrip0, Pset1);
    }
  }

/************************** Spectroscopic mode: ************************/
if(Pset1.ProcessingMode == 6) {
// "prepare_output_bisp_1D" is contained in "covermas.cpp"
// WARNING: "prepare_output_bisp_1D" modify modsq, bispp, snrm, etc,
// when computing their average values
   prepare_output_bisp_1D(bispp, modsq, snrm, nx1, ny1, ngood_frames,
                          nbeta, ngamma);

/* Mean squared modulus : */
   RECENT_FFT_1D_Y(modsq, modsq_centered_v, &nx1, &ny1, &nx1);
   sprintf(filename,"%s_m.fits", fname);
   sprintf((descrip0.descr[k_d]).cvalue, "Power spectrum %s", processing_param);
   strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
   descrip0.ndescr = k_d + 1;
   JLP_D_WRITEIMAG(modsq_centered_v,nx1,ny1,filename,comments,descrip0, Pset1);

/* SNR of squared modulus (actually 1/sigma if no photon correction): */
   RECENT_FFT_1D_Y(snrm, snrm, &nx1, &ny1, &nx1);
   sprintf(filename,"%s_snrm.fits", fname);
   sprintf((descrip0.descr[k_d]).cvalue, "SNR of power sp. %s", processing_param);
   strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
   descrip0.ndescr = k_d + 1;
   JLP_D_WRITEIMAG(snrm,nx1,ny1,filename,comments,descrip0, Pset1);

/* Bispectrum : */
   out_bisp = new double[3 * ngamma * nx1];
   output_bisp_1D(bispp,out_bisp,ngamma,nx1);
   sprintf(filename,"%s_b.fits", fname);
   sprintf((descrip0.descr[k_d]).cvalue, "Bispectrum %s", processing_param);
   strcpy((descrip0.descr[k_d]).keyword, "DESCRIP ");
   descrip0.ndescr = k_d + 1;
#ifdef DEBUG
  sprintf(buffer,"bisp ix=0 ng=0: %f %f %f",
              out_bisp[0*ngamma*3+0],
              out_bisp[0*ngamma*3+0+ngamma],
              out_bisp[0*ngamma*3+0+2*ngamma]);
  wxMessageBox(wxString(buffer), wxT("OutputFilesProcessCube"),
               wxICON_INFORMATION | wxOK);
  sprintf(buffer,"bisp ix=19 ng=5: %f %f %f (nx=%d)",
              out_bisp[19*ngamma*3+5],
              out_bisp[19*ngamma*3+5+ngamma],
              out_bisp[19*ngamma*3+5+2*ngamma],nx);
  wxMessageBox(wxString(buffer), wxT("OutputFilesProcessCube"),
               wxICON_INFORMATION | wxOK);
#endif
// Store the bispectrum as a series of nx lines of 3*ngamma values:
   JLP_D_WRITEIMAG(out_bisp,3*ngamma,nx1,filename,comments,descrip0, Pset1);
   delete[] out_bisp;
  }

// To prevent twice output:
  Files_already_saved = true;

  return(0);
}
/***************************************************************************
* To prevent deleting old file
***************************************************************************/
static int check_if_file_is_there(char *filename, bool *return_immediately)
{
wxMessageDialog *dialog1;
wxString buffer;
FILE *fp;

*return_immediately = false;
if(*filename == '\0') return(-1);

// Try to open it (if it is OK, it means that it exists !)
fp = fopen(filename, "r");
if(fp != NULL) {
 fclose(fp);
 buffer.Printf(wxT("Warning: file >%s< is already there: do you want to delete the old file ?"),
               filename);
  dialog1 = new wxMessageDialog(NULL, buffer, wxT("jlp_decode_cube_save/check_if_file_is_there"),
                                wxICON_QUESTION | wxNO_DEFAULT| wxYES_NO);
  if(dialog1->ShowModal() == wxID_NO) *return_immediately = true;
  delete dialog1;
  }
return(0);
}
