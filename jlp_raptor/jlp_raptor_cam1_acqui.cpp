/************************************************************************
* "jlp_raptor_cam1_acqui.cpp"
* JLP_RaptorCam1 class : single acquisition and setup for acquisition
*
* JLP
* Version 17/08/2015
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_raptor_cam1.h"      // ANDOR_SETTINGS, ...
#include "jlp_raptor_utils.h"     // Copy_RaptorSettings, ...

//---------------------------------------------------------------------------

// #define DEBUG

//------------------------------------------------------------------------
// Setting ANDOR camera to the configuration stored in RaptorSet
// ForExposures:
// Update memory space or configuration if needed after a change of:
// RaptorSet1.xc0, RaptorSet1.yc0
// RaptorSet1.xbin, RaptorSet1.ybin
//------------------------------------------------------------------------
int JLP_RaptorCam1::JLP_Raptor_ApplySettingsForExposures()
{
return(0);
}
//------------------------------------------------------------------------
// Basic settings of the ANDOR camera according
// to the configuration stored in RaptorSet1
//
// AD channel, preampli gain
// vertical, horizontal shift settings
// Shutter and trigger
//------------------------------------------------------------------------
int JLP_RaptorCam1::JLP_Raptor_ApplyBasicSettings()
{
int status;
wxString err_msg;

 JLP_Raptor_Set_FPGA_Options();
 JLP_Raptor_SetExpoTimeOrAutoExpo();
 JLP_Raptor_Set_NUC_State();
 JLP_Raptor_SetImageSharpen();

 status = JLP_Raptor_SetHighOrDigitalGain(err_msg);
 if(status) wxMessageBox( err_msg,
                   _T("JLP_Raptor_ApplyBasicSettings/JLP_Raptor_SetHighOrDigitalGain"),
                   wxICON_ERROR);
 JLP_Raptor_SetHighGainAndTriggerMode();
 JLP_Raptor_SetFrameRate();
 JLP_Raptor_SetTriggerDelay();

return(status);
}
/*************************************************************************
* Initialize RAPTOR camera for Single or Continuous Acquisition
* when acquisitionMode has been changed.
* Use the settings included in RaptorSet
*
* INPUT:
*  RaptorSet1.acquisitionMode
**************************************************************************/
int JLP_RaptorCam1::Cam1_InitAcquisition(wxString& error_msg)
{
int status = 0;

if(not_connected) return(-1);

error_msg = wxT("");
  if(RaptorSet1.is_initialized == 0) {
    error_msg.Append(wxT("JLP_Raptor_InitAcquisition/Error: RaptorSet not initialized!\n"));
    return(-1);
    }

// Wait for 0.1 second to allow MCD to calibrate fully before allowing an
// acquisition to begin
  wxMilliSleep(100);

return status;
}
/*******************************************************************************
*
*  FUNCTION NAME:  ANDOR_SetSystemForSingleAcquisition()
*
*  DESCRIPTION:  This function sets up the acquisition settings exposure time
*		  shutter, trigger and starts an acquisition. It also starts a
*		  timer to check when the acquisition has finished.
********************************************************************************/
int JLP_RaptorCam1::Cam1_SetSystemForSingleAcquisition(int nx0, int ny0, int xc0, int yc0,
                                                       int xbin0, int ybin0)
{
int  status = 0;

if(not_connected) return(-1);
 RaptorSet1.xc0 = xc0;
 RaptorSet1.yc0 = yc0;
 RaptorSet1.xbin = xbin0;
 RaptorSet1.ybin = ybin0;
 JLP_Raptor_ApplySettingsForExposures();

return(status);
}
/*******************************************************************************
*  FUNCTION NAME:  Cam1_SetSystemForContinuousAcquisition()
*
* Called by the Raptor Thread
*
*  DESCRIPTION:  This function sets up the acquisition settings exposure time
*		  shutter, trigger for a new acquisition.
*********************************************************************************/
int JLP_RaptorCam1::Cam1_SetSystemForContinuousAcquisition(int nx0, int ny0, int xc0,
                                                           int yc0, int xbin0, int ybin0)
{
 RaptorSet1.xc0 = xc0;
 RaptorSet1.yc0 = yc0;
 RaptorSet1.xbin = xbin0;
 RaptorSet1.ybin = ybin0;
 JLP_Raptor_ApplySettingsForExposures();

return(0);
}
/*******************************************************************************
* Cam1_StartContinuousAcquisition()
* Called by the Camera Thread
*
*********************************************************************************/
int JLP_RaptorCam1::Cam1_StartContinuousAcquisition()
{

if(not_connected) return(-1);

return(0);
}
/***************************************************************************
* Called by the camera thread
* to start image acquisition and processing
***************************************************************************/
int JLP_RaptorCam1::Cam1_StartAcquisition(int nx0, int ny0, wxString& error_msg)
{

if(not_connected) return(-1);

return(0);
}
/***************************************************************************
* Called by the camera thread, to stop image acquisition
***************************************************************************/
int JLP_RaptorCam1::Cam1_StopAcquisition(wxString& error_msg)
{

return(0);
}
/***********************************************************************
* Get a sequence of images from the Raptor camera
*
***********************************************************************/
int JLP_RaptorCam1::Cam1_Continuous_GetTheImages(AWORD *ImageCube0,
                                                 int nx0, int ny0, int nz0)
{
double sum0;
int i, j, k, i1, j1, i1_start, j1_start, nx1, ny1, kstart;
unsigned int ix_start, iy_start;
unsigned int ix_end, iy_end;
wxString buffer;

nx1 = nx0 * RaptorSet1.xbin;
ny1 = ny0 * RaptorSet1.ybin;
ix_start = RaptorSet1.xc0 - nx1 /2;
iy_start = RaptorSet1.yc0 - ny1 /2;
if(ix_start < 0) ix_start = 0;
if(iy_start < 0) iy_start = 0;
ix_end = ix_start + nx1;
iy_end = iy_start + ny1;
/*
buffer.Printf(wxT("DisplayROI: nx0,ny0=%d,%d xc0,yc0=%d %d x1,y1= %d,%d"),
              nx0, ny0, RaptorSet1.xc0, RaptorSet1.yc0, ix_start, iy_start);
wxMessageBox(buffer, _T(""), wxOK);
*/

for(k = 0; k < nz0; k++) {
 pxd_doSnap(0x1, 1, 0);
 pxd_readushort(0x1,                // select PIXCI(R) unit 1
                1,                  // select frame buffer 1
                ix_start, iy_start, // upper left X, Y AOI of buffer
                ix_end, iy_end,     // lower right X, Y AOI of buffer,
                                // -1 is an abbreviation for the maximum X or Y
                RaptorSet1.pImageArray,  // program buffer to be filled
                nx1 * ny1,          // size of program buffer in short's
                "Grey");            // color space to access

// AWORD = unsigned short
 kstart = k * nx0 * ny0;
 for(j = 0; j < ny0; j++) {
   for(i = 0; i < nx0; i++) {
     i1_start = i * RaptorSet1.xbin;
     j1_start = j * RaptorSet1.ybin;
     sum0 = 0.;
     for(j1 = 0; j1 < RaptorSet1.ybin; j1++)
       for(i1 = 0; i1 < RaptorSet1.xbin; i1++)
         sum0 += RaptorSet1.pImageArray[(i1 + i1_start) + (j1 + j1_start) * nx1];
     ImageCube0[kstart + i + j * nx0] = (AWORD)sum0;
   } // EOF loop on i
  } // EOF loop on j
} // EOF loop on k

return(0);
}
