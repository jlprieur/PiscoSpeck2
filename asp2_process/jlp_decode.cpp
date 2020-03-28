/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* asp2_decod.cpp
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
* Version 06-09-2015
---------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include "jlp_wx_ipanel_filters.h" // subtract_back_model ...
#include "jlp_numeric.h"           // fft_2D, ... "fftw3.h", etc

#include "jlp_time0.h"             // jlp_italian_date ...

#include "asp2_photon_corr.h"      // definition of clean_deconvolution, ...
#include "asp2_covermas.h"
#include "jlp_decode.h"


/* Cannot be used in debug mode
#define DEBUG
*/

/************************************************************************
* JLP_Decode: creator of JLP_Decode
*
*************************************************************************/
JLP_Decode::JLP_Decode(PROCESS_SETTINGS Pset0, JLP_CAMERA_SETTINGS CamSet0,
                       DESCRIPTORS descrip0, const int nx0, const int ny0,
                       wxString *str_messg, const int n_messg, const int ilang)
{
int status;
register int i, k;

// Messages (in different languages)
 m_messg = new wxString[n_messg];
 for(k = 0; k < n_messg; k++) m_messg[k] = str_messg[k];
 iLang = ilang;

 // Copy input descriptors to temporary descriptors:
  descrip1.ndescr = descrip0.ndescr;
  for(i = 0; i < descrip0.ndescr; i++) {
        strcpy((descrip1.descr[i]).cvalue, (descrip0.descr[i]).cvalue);
        strcpy((descrip1.descr[i]).keyword, (descrip0.descr[i]).keyword);
  }

// Read input processing parameters:
 Copy_PSET(Pset0, Pset1);

// Save to private parameters:
 nx1 = nx0;
 ny1 = ny0;
 nz1_cube = Pset0.nz_cube;

descrip1.ndescr = descrip0.ndescr;
for(k = 0; k < descrip1.ndescr; k++) {
  strcpy(descrip1.descr[k].cvalue, descrip0.descr[k].cvalue);
  strcpy(descrip1.descr[k].keyword, descrip0.descr[k].keyword);
  }

// Read Camera1 parameters
// (necessary for FITS header (output):
// if CamSet0==NULL, i.e. Camera not connected, CamSet0.is_initialized = 0)
 Copy_CameraSettings(CamSet0, &CamSet1);

 nbeta = 0;
 ngamma = 0;
 Files_already_saved = false;
 DecodeInitialized = false;
 CubeSavedToFile = false;
 outFITS_3DFile = NULL;
 strcpy(out3DFITS_fname, "");

// Initializing array pointers to NULL:
 dble_data = NULL;
 cplx_data_sq = NULL;
 cplx_data = NULL;
 for(i = 0; i < DATA_KK; i++) {
   data_old_Re[i] = NULL;
   data_old_Im[i] = NULL;
   }
 CrossSpec_Re = NULL;
 CrossSpec_Im = NULL;
 Quadrant_Re = NULL,
 Quadrant_Im = NULL,
 bispp = NULL;
 snrm = NULL;
 real_part = NULL;
 imag_part = NULL;
 work1 = NULL;
 autocc = NULL;
 autocc_centered_v = NULL;
 quadr_centered_v = NULL;
 ShortExp_v = NULL;
 crossc = NULL;
 modsq = NULL;
 modsq_centered_v = NULL;
 long_int = NULL;
 direct_vector = NULL;
 lucky_image = NULL;
// Initialization in case of problems in ComputeCenterOfImage:
 lucky_x0 = (double)nx1 / 2;
 lucky_y0 = (double)ny1 / 2;
// Scidar: long integ. of fluctuations:
 scidar_lf = NULL;
 scidar_func = NULL;
 autocc_flattened = NULL;
 PhotModsqFrame = NULL;
 UnresolvedModsqFrame = NULL;
 UnresolvedAutocFrame = NULL;
 SigmaUnresolvedModsq = 1.e-6;
 FlatFieldFrame = NULL;
 OffsetFrame = NULL;
 BispectrumFrame = NULL;
 RestoredImageFrame = NULL;

 input_processed_frames = false;

// Load unresolved autocorrelation:
 DC_LoadUnresolvedAutocFrame(Pset1.UnresolvedAutocName);

// Load unresolved modsq:
 DC_LoadUnresolvedModsqFrame(Pset1.UnresolvedModsqName);

// Load flat field frame:
 DC_LoadFlatFieldFrame(Pset1.FlatFieldFileName);

// Load photon modsq frame:
 DC_LoadPhotModsqFrame(Pset1.PhotModsqFileName);

// Load offset frame (=very short exposures in darkness):
 DC_LoadOffsetFrame(Pset1.OffsetFileName);

 ShortExp_v = new double[nx1 * ny1];
 CurrentImage = new AWORD[nx1 * ny1];
 CubeImage[0] = new AWORD[nx1 * ny1 * nz1_cube];
 CubeImage[1] = new AWORD[nx1 * ny1 * nz1_cube];
 CubeOfGoodFrames = new AWORD[nx1 * ny1 * nz1_cube];
 FFTImage = new AWORD[nx1 * ny1];

// Get memory and initialize arrays:
 status = SetupProcessCube();
 if(!status) DecodeInitialized = true;

// Erasing the arrays (should set ngood_frames = 0 first to avoid error message)
 ngood_frames = 0;

 EraseProcessCube(1);

return;
}
/************************************************************************
* Destructor
************************************************************************/
JLP_Decode::~JLP_Decode()
{
// Free allocated memory for arrays:
   FreeMemory();
}
/************************************************************************
* Copy new settings if they do not imply changes in memory allocation
* or processing mode
************************************************************************/
int JLP_Decode::DC_LoadNewSettings(PROCESS_SETTINGS Pset, int nx, int ny)
{
int status;

 if(((nx != nx1) || (ny != ny1) || (Pset.nz_cube != nz1_cube))
     && (Pset.ProcessingMode != Pset1.ProcessingMode)) {
    wxMessageBox(wxT("Changes are too important!"),
                 wxT("DC_LoadNewSettings/Error"), wxICON_ERROR);
   return(-1);
  }

// Should erase the arrays here to make things simpler:
 status = EraseProcessCube(1);
 if(status) return(-1);

// Read and copy input processing parameters:
 Copy_PSET(Pset, Pset1);
 nx1 = nx; ny1 = ny;

// Load unresolved autocorrelation:
 if(UnresolvedAutocFrame != NULL) {
     delete[] UnresolvedAutocFrame;
     UnresolvedAutocFrame = NULL;
   }
 DC_LoadUnresolvedAutocFrame(Pset1.UnresolvedAutocName);

// Load unresolved modsq:
 if(UnresolvedModsqFrame != NULL) {
     delete[] UnresolvedModsqFrame;
     UnresolvedModsqFrame = NULL;
   }
 DC_LoadUnresolvedModsqFrame(Pset1.UnresolvedModsqName);

// Load flat field frame:
 if(FlatFieldFrame != NULL) {
     delete[] FlatFieldFrame;
     FlatFieldFrame = NULL;
   }
 DC_LoadFlatFieldFrame(Pset1.FlatFieldFileName);

// Load photon modsq frame:
 if(PhotModsqFrame != NULL) {
     delete[] PhotModsqFrame;
     PhotModsqFrame = NULL;
   }
 DC_LoadPhotModsqFrame(Pset1.PhotModsqFileName);

// Load offset frame (=very short exposures in darkness):
 if(OffsetFrame != NULL) {
     delete[] OffsetFrame;
     OffsetFrame = NULL;
   }
 DC_LoadOffsetFrame(Pset1.OffsetFileName);

return(0);
}
/************************************************************************
* SetupProcessCube
* initialize parameters for processing
*
* ProcessingMode = 0 : no processing (only integration and numerisation)
* ProcessingMode = 1 : Only integration
* ProcessingMode = 2 : autoc - crossc, FFT modulus, long integration
* ProcessingMode = 3 : autoc - crossc, FFT modulus, long integration, quadrant
* ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
* ProcessingMode = 5 : Bispectrum, autoc, quadrant
* ProcessingMode = 6 : Spectroscopy
* ProcessingMode = 7 : SCIDAR Log
* ProcessingMode = 8 : SCIDAR Lin
* ProcessingMode = 9 : Statistics
*************************************************************************/
int JLP_Decode::SetupProcessCube()
{
/* Images nx*ny, irmax=25, ngmax=187566 : */
int nx1_fftw, ny1_fftw, isize, status;
char fmask_name[61], err_message[128];
register int k;
#ifdef DEBUG
char buffer[80];
#endif

if(DecodeInitialized == true) {
  wxMessageBox(wxT("Error:already initialized!"),
                 wxT("SetupProcessCube"), wxICON_ERROR);
  return(-1);
  }

/* Special case of only numerisation */
 if(Pset1.ProcessingMode == 0) {
   DecodeInitialized = true;
   ngood_frames = 0;
   nbad_frames = 0;
   return(0);
   }

/* Allocation of memory: */
 long_int = new double[nx1 * ny1];

 isize = nx1 * ny1;
// Scidar: bigger auto-correlations for a wider field:
 if(Pset1.ProcessingMode == 7 || Pset1.ProcessingMode == 8) isize = 4 * nx1 * ny1;

 modsq_centered_v = new double[isize];
 modsq = new double[isize];
 dble_data = new double[isize];
 cplx_data = new FFTW_COMPLEX[isize];

/********** Long int, autoc, bispectrum modes (1,2,3,4,5) ********/
if(Pset1.ProcessingMode >= 1
            && Pset1.ProcessingMode <= 5) {
 direct_vector = new double[isize];
 lucky_image = new double[isize];
 cplx_data_sq = new FFTW_COMPLEX[isize];
 autocc = new double[isize];
 autocc_centered_v = new double[isize];
 autocc_flattened = new double[isize];
/* New declarations to improve performances: (Feb 2007)*/
 CrossSpec_Re = new double[isize];
 CrossSpec_Im = new double[isize];
 for(k = 0; k < DATA_KK; k++) {
    data_old_Re[k] = new double[isize];
    data_old_Im[k] = new double[isize];
    }
// I add the quadrant possibility (load processing results) ;
 quadr_centered_v = new double[isize];
 Quadrant_Re = new double[isize];
 Quadrant_Im = new double[isize];
 }

/******************** Bispectrum mode (5,6) *********************/
/* Computing the uv coverage for computing the bispectrum:
* extend possible modes to be compatible with "Load processing Results"*/
if(Pset1.ProcessingMode == 4 || Pset1.ProcessingMode == 5)
  {
/* Before may 1999: ir = 15; max_nclosure = 100;
*/
  *fmask_name = '\0';
  compute_uv_coverage(fmask_name,Pset1.ir_bisp,&nbeta,&ngamma,
                      Pset1.nmax_bisp,err_message);

#ifdef DDEBUG
  sprintf(buffer," ir=%d nbeta=%d ngamma=%d nmax=%d",
          Pset1.ir_bisp, nbeta, ngamma, Pset1.nmax_bisp);
  wxMessageBox(wxString(buffer), wxT("SetupProcessCube"),
               wxICON_INFORMATION | wxOK);
#endif

/* Allocating memory space for the bispectrum: */
  bispp = new double[4 * ngamma];
// and for signal to noise ratio of sq. modulus:
  snrm = new double[nx1 * ny1];
  }

/********************* Scidar mode (7,8) **********************/
// nx11,ny11 will be used for fftw initialization
// 2D Fourier transform by default:
if(Pset1.ProcessingMode == 7 || Pset1.ProcessingMode == 8)
 {
 autocc = new double[isize];
 autocc_centered_v = new double[isize];
 autocc_flattened = new double[isize];
 CrossSpec_Re = new double[isize];
 CrossSpec_Im = new double[isize];

// Long integ. of fluctuations:
 scidar_lf = new double[nx1*ny1];
#ifdef SCIDAR_CROSSC
 crossc = new double[isize];
 for(k = 0; k < DATA_KK_SCIDAR; k++) {
    data_old_Re[k] = new double[isize];
    data_old_Im[k] = new double[isize];
    }
#endif
 }


/******************* Spectroscopic mode (6) *********************/
if(Pset1.ProcessingMode == 6)
  {
  work1 =  new double[Pset1.nx_num*Pset1.ny_num];
  *fmask_name = '\0';
/* Rotation so that the slit is parallel to Oy
   and the dispersion parallel to Ox (hence ir < ny) */
/* After rotation the spectrum is horizontal, and the slit vertical: */
  compute_uv_coverage_1D(fmask_name,Pset1.ir_bisp,&nbeta,&ngamma,
                         Pset1.nmax_bisp,err_message);

/* Allocating memory space for the bispectrum: */
  bispp = new double[4 * ngamma * nx1];
// and for signal to noise ratio of sq. modulus:
  snrm = new double[nx1 * ny1];
// For Fourier storage:
  real_part = new double[nx1 * ny1];
  imag_part = new double[nx1 * ny1];
  }

/****************** FFTW setup: ***************************************/
// JLP99: warning: NY,NX!!!!
if(Pset1.ProcessingMode != 0)
  {
if(Pset1.ProcessingMode == 7 || Pset1.ProcessingMode == 8){
   nx1_fftw = 2 * nx1;
   ny1_fftw = 2 * ny1;
/* Spectroscopic mode: */
  } else if(Pset1.ProcessingMode == 6) {
// 1D Fourier transform:
   ny1_fftw = 1;
   nx1_fftw = ny1;
// Other processing
  } else {
    nx1_fftw = nx1;
    ny1_fftw = ny1;
  }
// WARNING: it takes quite a long time when creating the wisdom files
// for the first time...
//   strcpy(fftw_directory, "F:\\tmp_fftw");
   status = fftw_setup(Pset1.FFTW_directory, ny1_fftw, nx1_fftw, err_message);
  if(status){
    wxMessageBox(wxString(err_message), wxT("SetupProcessCube/fftw_setup"),
                 wxICON_ERROR);
    return(-1);
    }
  }

// Erasing the arrays:
// (necessary since SetupProcessCube is called by many routines...)
 ngood_frames = 0;
 nbad_frames = 0;
// Erasing the arrays (with ngood_frames = 0 to avoid error message)
 EraseProcessCube(1);

 DecodeInitialized = true;

return(0);
}
/************************************************************************
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
* RETURN:
* -1  will save the results elsewhere
*************************************************************************/
int JLP_Decode::EraseProcessCube(const int confirm)
{
register int i, k;
int isize, status;
wxString buffer;

//if(ngood_frames > 300 && !Files_already_saved) {
if((confirm == 1)&& (ngood_frames > 0) && (!Files_already_saved)) {
// i=500 "Warning:"
// i=501 "results have not been saved!"
   buffer.Printf(wxT("%s %s (ngood_frames=%d) \n"),
                 m_messg[500].mb_str(), m_messg[501].mb_str(), ngood_frames);
// i=502 "Voulez-vous les sauver ?"
   buffer.Append(m_messg[502]);
   if(wxMessageBox(buffer, wxT("EraseprocessCube"),
                   wxICON_QUESTION | wxYES_NO) == wxYES ) return(-1);
  }

nz1_CubeOfGoodFrames = 0;
for(i = 0; i < nx1 * ny1 * nz1_cube; i++) CubeOfGoodFrames[i] = 0;

ngood_frames = 0;
nbad_frames = 0;
PhotonsPerFrame = 0.;

Pset1.FwhmSelect.Sum = 0.;
Pset1.FwhmSelect.Sumsq = 0.;
Pset1.FwhmSelect.Nval = 0.;
Pset1.MaxValSelect.Sum = 0.;
Pset1.MaxValSelect.Sumsq = 0.;
Pset1.MaxValSelect.Nval = 0.;

// Flag to remember the user to save the files:
CubeSavedToFile = false;
Files_already_saved = false;

// Close old and open new
// output FITS file used for saving elementary frames

if(outFITS_3DFile) {
  delete outFITS_3DFile;
  outFITS_3DFile = NULL;
  }

if(outFITS_3DFile == NULL && Pset1.SaveFramesToFitsFile != 0) {
  status = DC_Open_outFITS_3DFile();
  if(status) {
    wxMessageBox(wxT("Error opening outFITS_3DFile"),
                 wxT("EraseProcessCube"), wxICON_ERROR);
    return(-1);
    }
  }

/* Exit here if only visualisation */
if(Pset1.ProcessingMode == 0) return(0);

/* Exit from here if the arrays are not created yet */
if(!DecodeInitialized) return(0);

/* Erasing the arrays : */
for(i = 0; i < nx1 * ny1; i++) {
 ShortExp_v[i] = 0.;
 long_int[i] = 0.;
 }

isize = nx1 * ny1;
// Scidar:
if(Pset1.ProcessingMode == 7 || Pset1.ProcessingMode == 8) isize = 4 * nx1 * ny1;

for(i = 0; i < isize; i++) {
  modsq[i] = 0.;
  modsq_centered_v[i] = 0.;
  }

// Fourier transform can be computed in all modes >= 0
// (at least for image selection)
  for(i = 0; i < isize; i++) {
    c_re(cplx_data[i]) = 0.;
    c_im(cplx_data[i]) = 0.;
    }

if(Pset1.ProcessingMode >= 1 &&
   Pset1.ProcessingMode <= 5) {
  for(i = 0; i < isize; i++) {
    direct_vector[i] = 0.;
    lucky_image[i] = 0.;
    }
  }

// Autoc
if((Pset1.ProcessingMode == 2)
// Autoc & quadrant
  || (Pset1.ProcessingMode == 3)
// Autoc + quadrant + bispectrum
  || (Pset1.ProcessingMode == 5))
  {
  for(i = 0; i < isize; i++)
    {
    autocc[i]=0.;
    autocc_centered_v[i]=0.;
    CrossSpec_Re[i] = 0;
    CrossSpec_Im[i] = 0;
    autocc_flattened[i] = 0.;
    }

    for(k = 0; k < DATA_KK; k++) {
      for(i = 0; i < isize; i++) {
        data_old_Re[k][i] = 0.;
        data_old_Im[k][i] = 0.;
      }
    }
  }

// Quadrant is required in those modes:
if(Pset1.ProcessingMode == 3 || Pset1.ProcessingMode == 5)
  {
  for(i = 0; i < isize; i++) {
    quadr_centered_v[i]=0.;
    Quadrant_Re[i] = 0.;
    Quadrant_Im[i] = 0.;
    }
  }

// Bispectrum mode:
if(Pset1.ProcessingMode == 4 || Pset1.ProcessingMode == 5) {
    for(i = 0; i < 4 * ngamma; i++) bispp[i] = 0.;
    for(i = 0; i < nx1 * ny1; i++) snrm[i] = 0.;
    for(i = 0; i < nx1 * ny1; i++) autocc_flattened[i] = 0.;
    }

// Spectroscopic mode:
if(Pset1.ProcessingMode == 6)
    {
    for(i = 0; i < 4 * ngamma * nx1; i++) bispp[i] = 0.;
    for(i = 0; i < nx1 * ny1; i++) snrm[i] = 0.;
    }

// Scidar
if(Pset1.ProcessingMode == 7 || Pset1.ProcessingMode == 8)
  {
  for(i = 0; i < nx1*ny1; i++) {
    autocc[i] = 0.;
    autocc_centered_v[i] = 0.;
    }
  for(i = 0; i < isize; i++) {
    CrossSpec_Re[i] = 0;
    CrossSpec_Im[i] = 0;
    }

#ifdef SCIDAR_CROSSC
  for(k = 0; k < DATA_KK_SCIDAR; k++)
  for(i = 0; i < isize; i++) {
    data_old_Re[k][i] = 0.;
    data_old_Im[k][i] = 0.;
    }
  for(i = 0; i < isize; i++) crossc[i]=0.;
#endif
// Scidar: long integ. of fluctuations:
  for(i = 0; i < nx1 * ny1; i++) scidar_lf[i]=0.;
  }

return(0);
}
/**********************************************************************
* Called by ~JLP_Decode()
* Free previously allocated memory:
***********************************************************************/
int JLP_Decode::FreeMemory()
{
register int k;

// SHOULD NOT delete wxStrings !
// delete m_messg;

 if(DecodeInitialized == false) {
   wxMessageBox(wxT("Error: Decode not initialized!"),
                 wxT("JLP_Decode::Free Memory"), wxICON_ERROR);
   return(-1);
   }

delete[] CurrentImage;
CurrentImage = NULL;
// Error here so I skip:
// delete[] (CubeImage[0]);
// delete[] (CubeImage[1]);
delete[] CubeOfGoodFrames;
delete[] FFTImage;
FFTImage = NULL;

  if(ShortExp_v) delete[] ShortExp_v;
  ShortExp_v = NULL;
  if(long_int) delete[] long_int;
  long_int = NULL;
  if(modsq_centered_v) delete[] modsq_centered_v;
  modsq_centered_v = NULL;
  if(modsq) delete[] modsq;
  modsq = NULL;
  if(autocc_centered_v) delete[] autocc_centered_v;
  autocc_centered_v = NULL;
  if(autocc) delete[] autocc;
  autocc = NULL;
  if(quadr_centered_v) delete[] quadr_centered_v;
  quadr_centered_v = NULL;
  if(crossc) delete[] crossc;
  crossc = NULL;
  if(autocc_flattened) delete[] autocc_flattened;
  autocc_flattened = NULL;
  if(direct_vector) delete[] direct_vector;
  direct_vector = NULL;
  if(lucky_image) delete[] lucky_image;
  lucky_image = NULL;
  if(bispp) delete[] bispp;
  bispp = NULL;
  if(snrm) delete[] snrm;
  snrm = NULL;

  if(scidar_lf) delete[] scidar_lf;
  scidar_lf = NULL;
  if(scidar_func) delete[] scidar_func;
  scidar_func = NULL;

  if(dble_data) delete[] dble_data;
  dble_data = NULL;
  if(cplx_data) delete[] cplx_data;
  cplx_data = NULL;
  if(cplx_data_sq) delete[] cplx_data_sq;
  cplx_data_sq = NULL;
  for(k = 0; k < DATA_KK; k++) {
   if(data_old_Re[k]) delete[] data_old_Re[k];
   data_old_Re[k] = NULL;
   if(data_old_Im[k]) delete[] data_old_Im[k];
   data_old_Im[k] = NULL;
   }
  if(CrossSpec_Re) delete[] CrossSpec_Re;
  CrossSpec_Re = NULL;
  if(CrossSpec_Im) delete[] CrossSpec_Im;
  CrossSpec_Im = NULL;
  if(Quadrant_Re) delete[] Quadrant_Re;
  Quadrant_Re = NULL;
  if(Quadrant_Im) delete[] Quadrant_Im;
  Quadrant_Im = NULL;
  if(real_part) delete[] real_part;
  real_part = NULL;
  if(imag_part) delete[] imag_part;
  imag_part = NULL;
  if(work1) delete[] work1;
  work1 = NULL;

// Delete allocated forward and backward plans for FFT computation:
  fftw_shutdown();

// Delete 3D FITS file containing all elementary frames:
  if(outFITS_3DFile) delete outFITS_3DFile;
  outFITS_3DFile = NULL;

  DecodeInitialized = false;
  return(0);
}
