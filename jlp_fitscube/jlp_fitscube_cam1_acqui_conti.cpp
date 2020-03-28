/************************************************************************
* "jlp_fitscube_cam1_acqui_conti.cpp"
* JLP_FitsCubeCam1 class : continuous acquisition
*
* JLP
* Version 17/08/2015
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_fitscube_cam1.h"      // FITSCUBE_SETTINGS, ...
#include "jlp_fitscube_utils.h"     // Copy_FitsCubeSettings, ...

//---------------------------------------------------------------------------

// #define DEBUG

/*******************************************************************************
*  FUNCTION NAME:  Cam1_SetSystemForContinuousAcquisition()
*
* Called by the FitsCube Thread
*
*  DESCRIPTION:  This function sets up the acquisition settings exposure time
*		  shutter, trigger for a new acquisition.
*********************************************************************************/
int JLP_FitsCubeCam1::Cam1_SetSystemForContinuousAcquisition(int nx0, int ny0, int xc0,
                                                             int yc0, int xbin0, int ybin0)
{
int status;

 FitsCubeSet1.xc0 = xc0;
 FitsCubeSet1.yc0 = yc0;
 FitsCubeSet1.xbin = xbin0;
 FitsCubeSet1.ybin = ybin0;

 JLP_FitsCube_ApplySettingsForExposures();

// Rewind the file to start at the beginning of the file
 status = inFitsFile1->Rewind_3DFile();

return(status);
}
/*******************************************************************************
*  FUNCTION NAME:  Cam1_StartContinuousAcquisition()
*
* Called by the Camera Thread
*
*  DESCRIPTION:  This function starts a new acquisition (not use here)
*********************************************************************************/
int JLP_FitsCubeCam1::Cam1_StartContinuousAcquisition()
{
return(0);
}
/***************************************************************************
* Called by Main_StartFitsCubeProcessing() in "asp2_main_utils.cpp"
* Handle the "Start processing" button,
* to start image acquisition and processing
***************************************************************************/
int JLP_FitsCubeCam1::Cam1_StartAcquisition(int nx0, int ny0, wxString& error_msg)
{
int status = 0;

if(not_connected) return(-1);

return(status);
}
/***************************************************************************
* Handle "stop" button, to stop image acquisition
***************************************************************************/
int JLP_FitsCubeCam1::Cam1_StopAcquisition(wxString& error_msg)
{
// Do not return anything since it may be called when end of file has been reached
error_msg = wxT("");

return(0);
}
/***********************************************************************
* Get a sequence of images from the FitsCube camera
*
* return 10 when all data has been read
***********************************************************************/
int JLP_FitsCubeCam1::Cam1_Continuous_GetTheImages(AWORD *ImageCube0,
                                                   int nx0, int ny0, int nz0)
{
long istart, jstart, i1, j1, i2, j2, i, j, ii, jj, k, kk1, kk0;
double sum0;
char err_messg[128];
wxString buffer;
int status;

if(inFitsFile1 == NULL || ImageCube1 == NULL) {
  buffer.Printf(wxT("Fatal error: unitialized parameters inFitsFile1 !\n"));
  wxMessageBox(buffer, wxT("JLP_FitsCubeCam1::Cam1_Continuous_GetTheImages"),
               wxOK | wxICON_ERROR);
  exit(-1);
  }

if(nz0 > FitsCubeSet1.nz1) {
  buffer.Printf(wxT("Fatal error: nz0=%d larger than max value for fitscubes: nz_max = %d !\n"),
                nz0, FitsCubeSet1.nz1);
  wxMessageBox(buffer, wxT("JLP_FitsCubeCam1::Cam1_Continuous_GetTheImages"),
               wxOK | wxICON_ERROR);
  exit(-1);
}

// If file not fully read, read a new cube, otherwise, simply send the last cube a few times
// (to have a similar behaviour as the andor thread: the process should be stopped
// by asp2_process.cpp when receiving the last event from the decode thread)
// Return status = 10 when file has been completely read:
  status = inFitsFile1->Read_aword_cube_from_3DFitsFile(ImageCube1, FitsCubeSet1.gblXPixels,
                                                 FitsCubeSet1.gblYPixels, nz0,
                                                 err_messg);
  if(status == 0) {
    if((nx0 != FitsCubeSet1.gblXPixels) || (ny0 != FitsCubeSet1.gblYPixels)) {
// ROI start is x=istart, y=jstart
    istart = FitsCubeSet1.xc0 - (FitsCubeSet1.xbin * nx0) / 2;
    jstart = FitsCubeSet1.yc0 - (FitsCubeSet1.ybin * ny0) / 2;
    for(k = 0; k < nz0; k++) {
     kk1 = k * FitsCubeSet1.gblXPixels * FitsCubeSet1.gblYPixels;
     kk0 = k * nx0 * ny0;
        for(j = 0; j < ny0; j++) {
         j1 = j * FitsCubeSet1.ybin + jstart;
         j2 = j1 + FitsCubeSet1.ybin;
           for(i = 0; i < nx0; i++) {
            i1 = i * FitsCubeSet1.xbin + istart;
            i2 = i1 + FitsCubeSet1.xbin;
// Binning sum:
            sum0 = 0.;
            for(jj = j1; jj < j2; jj++)
             for(ii = i1; ii < i2; ii++)
              sum0 += ImageCube1[ii + jj * FitsCubeSet1.gblXPixels + kk1];
           ImageCube0[i + j * nx0 + kk0] = sum0;
           } // EOF loop on i
        } // EOF loop on j
     } // EOF loop on k
// Case when nx0 == FitsCubeSet1.gblXPixels and ny0 == FitsCubeSet1.gblYPixels
    } else {
      for(i = 0; i < nx0 * ny0 * nz0; i++) ImageCube0[i] = ImageCube1[i];
    }
  } // EOF status == 0

return(status);
}
