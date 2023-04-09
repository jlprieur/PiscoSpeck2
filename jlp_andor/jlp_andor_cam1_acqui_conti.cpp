/************************************************************************
* "jlp_andor_cam1_acqui_conti.cpp"
* JLP_AndorCam1 class : continuous acquisition
*
* JLP
* Version 17/08/2015
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_andor_cam1.h"      // ANDOR_SETTINGS, ...
#include "jlp_andor_utils.h"     // Copy_AndorSettings, ...

//---------------------------------------------------------------------------

// #define DEBUG

/*******************************************************************************
*  FUNCTION NAME:  Cam1_SetSystemForContinuousAcquisition()
*
* Called by the Andor Thread
*
*  DESCRIPTION:  This function sets up the acquisition settings exposure time
*		  shutter, trigger for a new acquisition.
*********************************************************************************/
int JLP_AndorCam1::Cam1_SetSystemForContinuousAcquisition(int nx0, int ny0, 
                                                          int xc0, int yc0, 
                                                          int xbin0, int ybin0)
{
wxString aBuffer2, buff0, error_msg;
int errorValue, xstart, xend, ystart, yend, status = 0;

// Allocate memory for image data.
// Size is returned for GetAcquiredData which needs the buffer size
  JLP_Andor_AllocateImageBuffer(nx0, ny0);
  AndorSet1.xc0 = xc0;
  AndorSet1.yc0 = yc0;
  AndorSet1.xbin = xbin0;
  AndorSet1.ybin = ybin0;
  buff0.Printf(wxT("nx0=%d ny0=%d xc0=%d yc0=%d xbin0=%d ybin0=%d \n"),
               nx0, ny0, xc0, yc0, xbin0, ybin0);

// JLP2023: inquire status of camera (ANDOR routine) and AbortAcquisition if  needed
  GetStatus(&status);
    if(status == DRV_ACQUIRING){
      errorValue = AbortAcquisition();
      if(errorValue != DRV_SUCCESS){
        error_msg = wxT("Error aborting acquisition");
        wxMessageBox(error_msg, wxT("Cam1_SetSystemForContinuousAcquisition"),
                     wxOK | wxICON_ERROR);
        return(-1);
      }
    }

// Prepare Acquisition:
  errorValue = PrepareAcquisition();
  if (errorValue != DRV_SUCCESS) {
    switch(errorValue) {
       case DRV_NOT_INITIALIZED:
         aBuffer2 = wxT("Error in PrepareAcquisition: DRV_NOT_INITIALIZED\r\n");
         break;
       case DRV_ACQUIRING:
         aBuffer2 = wxT("Error in PrepareAcquisition: DRV_ACQUIRING\r\n");
         break;
       case DRV_VXDNOTINSTALLED:
         aBuffer2 = wxT("Error in PrepareAcquisition: Vxd not installed\r\n");
         break;
       case DRV_INIERROR:
         aBuffer2 = wxT("Error in PrepareAcquisition, reading DETECTOR.INI\r\n");
         break;
       case DRV_ACQUISITION_ERRORS:
         aBuffer2 = wxT("Error in PrepareAcquisition: bad acquisition settings\r\n");
         break;
       case DRV_ERROR_PAGELOCK:
         aBuffer2 = wxT("Error in PrepareAcquisition: unable to allocate memory\r\n");
         break;
       case DRV_INVALID_FILTER:
         aBuffer2 = wxT("Error in PrepareAcquisition: invalid filter\r\n");
         break;
       case DRV_ERROR_SCAN:
         aBuffer2 = wxT("Error in PrepareAcquisition: DRV_ERROR_SCAN\r\n");
         break;
       default:
         aBuffer2.Printf(wxT("Error in PrepareAcquisition Andor routine error=%d\r\n"),
                 errorValue);
         break;
      }
    buff0.Append(aBuffer2);
    wxMessageBox(buff0, wxT("Cam1_SetSystemForContinuousAcquisition"),
                 wxOK | wxICON_ERROR);
    return(-2);
    }


// This function only needs to be called when acquiring an image. It sets
// the horizontal and vertical binning and the area of the image to be
// captured. In this example it is set to 1x1 binning and is acquiring the
// whole image
// SetImage(int hbin, int vbin, int hstart, int hend, int vstart, int vend)
// Before:
//  SetImage(1, 1, 1, AndorSet1.gblXPixels, 1, AndorSet1.gblYPixels);
// Now:
//  errorValue = SetImage(AndorSet1.xbin, AndorSet1.ybin, 1,
//                        AndorSet1.nx1 * AndorSet1.xbin,
//                        1, AndorSet1.ny1 * AndorSet1.ybin);
  xstart = MAXI(1, AndorSet1.xc0 - (nx0 / 2));
  xend = xstart + nx0 -1;
  ystart = MAXI(1, AndorSet1.yc0 - (ny0 / 2));
  yend = ystart + ny0 -1;
  errorValue = SetImage(AndorSet1.xbin, AndorSet1.ybin, xstart,
                        xend, ystart, yend);
  if(errorValue != DRV_SUCCESS) {
    aBuffer2.Printf(wxT("Set Image Error: xbin=%d ybin=%d, nx0=%d ny0=%d xc0=%d yc0=%d xstart=%d xend=%d ystart=%d yend=%d\r\n"),
            AndorSet1.xbin, AndorSet1.ybin, nx0, ny0, AndorSet1.xc0, AndorSet1.yc0, xstart, xend, ystart, yend);
    wxMessageBox(aBuffer2, wxT("JLP_Andor_SetSystemForContinuousAcquisition"),
                     wxOK | wxICON_ERROR);
    return(-3);
    }

return(0);
}
/*******************************************************************************
*  FUNCTION NAME:  Cam1_StartContinuousAcquisition()
*
* Called by the Camera Thread
*
*  DESCRIPTION:  This function starts a new acquisition.
*********************************************************************************/
int JLP_AndorCam1::Cam1_StartContinuousAcquisition()
{
wxString error_msg;
int errorValue;
int status = 0;

// JLP2023: exit from here if already in continuous acquisition 
// Inquire status of camera (ANDOR routine):
  GetStatus(&status);
  if(status == DRV_ACQUIRING) {
     return(0);
  }

// If not, start the acquisition now
  errorValue = StartAcquisition();
  if(errorValue != DRV_SUCCESS){
    error_msg.Append(wxT("Error returned from \"StartAcquisition\" \r\n"));
    if(errorValue == DRV_NOT_INITIALIZED) {
        error_msg.Append(wxT("System not initialized \n\r"));
    } else if(errorValue == DRV_ACQUIRING) {
        error_msg.Append(wxT("Acquisition in progress \n\r"));
    } else if(errorValue == DRV_VXDNOTINSTALLED) {
        error_msg.Append(wxT("VxD not loaded \n\r"));
    } else if(errorValue == DRV_ERROR_ACK) {
        error_msg.Append(wxT("Unable to communicate with card \n\r"));
    } else if(errorValue == DRV_ACQUISITION_ERRORS) {
        error_msg.Append(wxT("Acquisition settings invalid \n\r"));
    } else if(errorValue == DRV_ERROR_PAGELOCK) {
        error_msg.Append(wxT("Unable to allocate memory \n\r"));
    } else if(errorValue == DRV_INVALID_FILTER) {
        error_msg.Append(wxT("Filter not available for current acquisition \n\r"));
        }
    AbortAcquisition();
    gblData_is_ready = false;
    status = -1;
  } else {
    error_msg.Append(wxT("Start acquisition was successful\r\n"));
    status = 0;
//      JLP_Andor_Continuous_GetTheImages();
// Start processing by sending an event to the AndorThread
//     SetEvent(StartProcessingEvent);
  }

if(status) wxMessageBox( error_msg,
                        wxT("JLP_Andor_StartContinuousAcquisition"),
                        wxICON_ERROR);
#ifdef DEBUG
else
           wxMessageBox( error_msg,
                      "JLP_Andor_StartContinuousAcquisition",
                      wxOK);
#endif

return(status);
}
/**********************************************************************
*
* Every "TIMER" event, a request is sent to ANDOR (in ProcessTimer)
* When the acquisition is complete,
* the data is read from the card and displayed in the paint area.
*
* return 0 if OK, -1 if error reading image
**********************************************************************/
int JLP_AndorCam1::JLP_Andor_WaitForSingleImage(wxString& error_msg)
{
bool image_is_read;
long sec, max_duration;
int status;

// max_duration (in milliseconds): 5 seconds + exposure_time
max_duration = AndorSet1.KineticExpoTime_msec + 5000.;

// Create a timer with a time out (= time interval) of max_duration
// (in milliseconds)

// Conversion to 0.1 seconds for "sec":
sec = max_duration / 100.;

// Start the loop (does not occupy the cpu)
while (sec > 0)
{
sec--;
// Wait for 100 ms (= 0.1 second) at each step:
wxMilliSleep(100);
status = JLP_Andor_Single_ProcessTimer(error_msg, image_is_read);
if(status) {
   wxMessageBox( error_msg, "JLP_Andor_ProcessTimer",
              wxICON_ERROR);
   image_is_read = false;
   break;
  }
if(image_is_read) break;
}

if(image_is_read) status = -1;
else status = 0;

return status;
}
//------------------------------------------------------------------------------
//  FUNCTION NAME:  JLP_Andor_Single_ProcessTimer()
// Every timer event, a request is sent to ANDOR to know whether
// the acquisition is complete.
// If so, the data is read from the card and displayed in the paint area.
//
//  RETURNS:	    -1 if error when reading data
//                   0 if image is not ready or if image was succesfully loaded
//                image_is_read: true if image was successfully read
//------------------------------------------------------------------------------
int JLP_AndorCam1::JLP_Andor_Single_ProcessTimer(wxString& error_msg,
                                                 bool& image_is_read)
{
int  status, istat = 0;

  image_is_read = false;

// Inquire status of camera (ANDOR routine):
  GetStatus(&status);
// If idle, transfer last image to buffer (in ANDOR_AcquireImageData):
  if(status == DRV_IDLE){
    if(JLP_Andor_AcquireImageData() == true){
      image_is_read = true;
    } else {
      error_msg = wxT("JLP_Andor_Single_ProcessTimer/Acquisition Error! \r\n");
      istat = -1;
    }
  }
return istat;
}
//------------------------------------------------------------------------------
//  FUNCTION NAME: JLP_Andor_AcquireImageData()
//                 (called by Andor_Single_ProcessTimer)
//
//  RETURNS:	 true: Image data acquired and displayed successfully
//		 false: Error acquiring or displaying data
//
//  LAST MODIFIED: PMcK	03/11/98
//
//  DESCRIPTION: This function gets the acquired data from the card and
//		 stores it in the global buffer pImageArray. It is called
//		 from WM_TIMER after the acquisition is complete and goes on
//		 to display the data using DrawLines() and kill the timer.
//
//  ARGUMENTS: 	 NONE
//------------------------------------------------------------------------------
bool JLP_AndorCam1::JLP_Andor_AcquireImageData()
{
int   errorValue;
long image_size0;
wxString aBuffer;
// long  MaxValue,	MinValue;

// Get data from Andor system:
  image_size0 = (long)(AndorSet1.nx1 * AndorSet1.ny1);
  errorValue = GetAcquiredData16(AndorSet1.pImageArray, image_size0);
  if(errorValue != DRV_SUCCESS){
    aBuffer = wxT("JLP_Andor_AcquireImageData/Acquisition error in GetAcquiredData!");
    wxLogError(aBuffer);
    return false;
  }

/*
// Display data and query max data value to be displayed in status box
  FillRectangle();
  if(!DrawLines(&MaxValue,&MinValue)){
    sprintf(aBuffer, "Data range is zero");
   	SendMessage(ebStatus,WM_SETTEXT,0,(LPARAM)(LPSTR)aBuffer);
    return false;
  }

// Destroy the timer:
  KillTimer(hwnd, ANDOR_timer);                    // kill status timer
*/

// Flag ANDOR_gblData is set when first acquisition is taken, tells
// system whether there is data to display.

// Tell user acquisition was aborted
  if(gblData_is_ready == false){						// If there is no data the acq has
    aBuffer = wxT("Acquisition was aborted\r\n");       // been aborted
    wxLogError(aBuffer);
  } else {
// Tell user acquisition is complete
    aBuffer = wxT("Acquisition complete: full image acquired\r\n");
//    sprintf(aBuffer2,"Max data value is %d counts\r\n",MaxValue);
//    strcat(aBuffer,aBuffer2);
//    sprintf(aBuffer2,"Min data value is %d counts",MinValue);
//    strcat(aBuffer,aBuffer2);
  }
//  SendMessage(ebStatus,WM_SETTEXT,0,(LPARAM)(LPSTR)aBuffer);

  return true;
}
/***************************************************************************
* Called by Main_StartAndorProcessing() in "asp2_main_utils.cpp"
* Handle the "Start processing" button,
* to start image acquisition and processing
***************************************************************************/
int JLP_AndorCam1::Cam1_StartAcquisition(int nx0, int ny0, wxString& error_msg)
{
int status = 0;

// Inquire the Andor camera status:
    GetStatus(&status);
    if(status == DRV_IDLE){
// Flag gblData is set when first acquisition is taken, tells
// system that there is now data to display:
      gblData_is_ready = true;
// Set hardware and start acquisition:
    if(AndorSet1.AcquisitionMode == 1) {
      status = Cam1_SetSystemForSingleAcquisition(nx0, ny0, AndorSet1.xc0,
                                                  AndorSet1.yc0, AndorSet1.xbin,
                                                  AndorSet1.ybin);
    } else {
      status = Cam1_SetSystemForContinuousAcquisition(nx0, ny0, AndorSet1.xc0,
                                                  AndorSet1.yc0, AndorSet1.xbin,
                                                  AndorSet1.ybin);
/*
      SetEvent(StartProcessingEvent);
*/
    }

// Clear window ready for data trace:
//      FillRectangle();
      status = 0;
    } else {
      gblData_is_ready= false;
      error_msg = wxT("Cannot start aquisition: system is busy!");
      status = -1;
    }

return status;
}
/***************************************************************************
* Handle "stop" button, to stop image acquisition
***************************************************************************/
int JLP_AndorCam1::Cam1_StopAcquisition(wxString& error_msg)
{
int status = 0, errorValue;

// Abort acquisition if in progress
    GetStatus(&status);
    if(status == DRV_ACQUIRING){
      errorValue = AbortAcquisition();
      if(errorValue != DRV_SUCCESS){
        error_msg = wxT("Error aborting acquisition");
        return(-1);
      }
      else{
        error_msg = wxT("OK: aborting Acquisition");
      }
// Flag ANDOR_gblData is set when first acquisition is taken, tells
// system that there is now no data to display:
      gblData_is_ready = false;   // tell system no acq data in place
    }

return(0);
}
/***********************************************************************
* Get a sequence of images from the Andor camera
*
***********************************************************************/
int JLP_AndorCam1::Cam1_Continuous_GetTheImages(AWORD *ImageCube0,
                                             int nx0, int ny0, int nz0)
{
int k, i, status;
char error_msg[128];
wxString aBuffer;

  if((AndorSet1.ImageSize != nx0 * ny0)) {
   aBuffer.Printf(wxT("JLP_AndorCam1/JLP_Andor_Continuous_GetTheImages\n\
Error: incompatible size: nx0=%d ny0=%d ImageSize=%d"),
                  nx0, ny0, AndorSet1.ImageSize);
   wxLogError(aBuffer);
   return(-1);
   }
  for (k = 0; k < nz0; k++) {

// Send a software trigger to start the acquisition for each frame:
// (not needed if only internal trigger is available)
  if(AndorSet1.TriggerMode == 10) SendSoftwareTrigger();

// Wait for signal from camera:
    WaitForAcquisition();

// Retrieve image from camera:
   status = GetMostRecentImage16(AndorSet1.pImageArray, AndorSet1.ImageSize);
   if(status != DRV_SUCCESS) {
          switch(status) {
      default:
         sprintf(error_msg,"GetMostRecentImage16/Acquisition error, \
please change the Andor settings !");
         break;
      case DRV_NOT_INITIALIZED:
         sprintf(error_msg,"GetMostRecentImage16/Error: driver not initialized!");
         break;
      case DRV_ERROR_ACK:
         sprintf(error_msg,"GetMostRecentImage16/Error: Unable to communicate with card!");
         break;
      case DRV_P1INVALID:
         sprintf(error_msg,"GetMostRecentImage16/Error: Invalid pointer (i.e. NULL)!");
         break;
      case DRV_P2INVALID:
         sprintf(error_msg,
                 "GetMostRecentImage16/Error: Array size is too small (image_size=%ld) nx=%d ny=%d)!",
                 AndorSet1.ImageSize, nx0, ny0);
         break;
      case DRV_NO_NEW_DATA:
         sprintf(error_msg,"GetMostRecentImage16/Error: There is no new data yet!");
         break;
      }

// or: GetMostRecentImage(ANDOR_pImageArray, ANDOR_nx * ANDOR_ny);
// Only for first image of cube, and then break:
      aBuffer = wxString(error_msg);
      wxLogError(aBuffer);
      return(-2);
    } else {
      for(i = 0; i < AndorSet1.ImageSize; i++)
            ImageCube0[i + k * AndorSet1.ImageSize] = (AWORD)AndorSet1.pImageArray[i];
//      sprintf(aBuffer,"Got Image %d",i);
//      wxMessageBox(aBuffer,"JLP_AndorCam1/GetTheImage",wxOK);
    }
  }

// Before 2015:
//  AbortAcquisition();

return(0);
}
