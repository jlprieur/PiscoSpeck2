/************************************************************************
* "jlp_andor_cam1_acqui.cpp"
* JLP_AndorCam1 class : single acquisition and setup for acquisition
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

//------------------------------------------------------------------------
// Setting ANDOR camera to the configuration stored in AndorSet
// ForExposures:
// EMCCD gain, exposure time, kinetic cycle
//------------------------------------------------------------------------
int JLP_AndorCam1::JLP_Andor_ApplySettingsForExposures()
{
float fExposure,fAccumTime,fKineticTime;
bool errorFlag = false;
int errorValue, status = 0;
wxString error_msg, buff0;
char buffer2[128];

if(not_connected) return(-1);

error_msg=wxT("");

// SetReadMode() belongs to Andor DLL...
// Set read mode to required setting for imaging mode
    errorValue = SetReadMode(AndorSet1.readMode);
    if(errorValue != DRV_SUCCESS){
      buff0.Printf(wxT("Trying to set read_mode= %d\n"), AndorSet1.readMode);
      error_msg.Append(buff0);
      error_msg.Append(wxT("JLP_Andor_InitAcquisition/Error in SetReadMode\n"));
      errorFlag = true;
    }


// Set Kinetic Exposure Time (fExposure in seconds)
fExposure = AndorSet1.KineticExpoTime_msec / 1000.;
errorValue = SetExposureTime(fExposure);
  if (errorValue != DRV_SUCCESS) {
    sprintf(buffer2,"Error setting exposure time to %d msec!\r\n",
            AndorSet1.KineticExpoTime_msec);
    error_msg.Append(wxString(buffer2));
    errorFlag = true;
  } else {
    sprintf(buffer2,"Setting exposure time to %d msec.\r\n",
            AndorSet1.KineticExpoTime_msec);
    error_msg.Append(wxString(buffer2));
  }

// Set Kinetic cycle Time to minimum value if "run till abort"
if(AndorSet1.AcquisitionMode == 5) {
// If Software trigger can set kinetic cycle time to minimum value:
  if(AndorSet1.TriggerMode == 2) {
  errorValue = SetKineticCycleTime(0);
    if (errorValue != DRV_SUCCESS) {
      sprintf(buffer2,"Soft. Trigger/Error setting Kinetic cycle time to minimum value!\r\n");
      error_msg.Append(wxString(buffer2));
      errorFlag = true;
    } else {
      sprintf(buffer2,"Soft. Trigger/Setting Kinetic Cycle time to minimum value\r\n");
      error_msg.Append(wxString(buffer2));
    }
  } else {
// If internal trigger mode, set kinetic cycle to kinetic cycle time (in seconds)
  fKineticTime = AndorSet1.KineticCycleTime_msec / 1000.;
  errorValue = SetKineticCycleTime(fKineticTime);
    if (errorValue != DRV_SUCCESS) {
      sprintf(buffer2,"Error setting Kinetic cycle time to %d msec!\r\n",
              AndorSet1.KineticCycleTime_msec);
      error_msg.Append(wxString(buffer2));
      errorFlag = true;
    } else {
      sprintf(buffer2,"Setting Kinetic Cycle time to %d msec.\r\n",
              AndorSet1.KineticCycleTime_msec);
      error_msg.Append(wxString(buffer2));
    }
  }
// Set Kinetic cycle parameters:
// if acquisitionMode = 2 (accumulate)
// or                 = 3 (kinetics)
// or                 = 4 (fast kinetics)
} else if(AndorSet1.AcquisitionMode == 2
   || AndorSet1.AcquisitionMode == 3
   || AndorSet1.AcquisitionMode == 4) {

// Set Kinetic cycle Time (in seconds)
  fKineticTime = AndorSet1.KineticCycleTime_msec / 1000.;
  errorValue = SetKineticCycleTime(fKineticTime);
    if (errorValue != DRV_SUCCESS) {
      sprintf(buffer2,"Error setting Kinetic cycle time to %d msec!\r\n",
              AndorSet1.KineticCycleTime_msec);
      error_msg.Append(wxString(buffer2));
      errorFlag = true;
    } else {
      sprintf(buffer2,"Setting Kinetic Cycle time to %d msec.\r\n",
              AndorSet1.KineticCycleTime_msec);
      error_msg.Append(wxString(buffer2));
    }

// Set Kinetic nber of exposures per cycle
  errorValue = SetNumberAccumulations(AndorSet1.KineticCycleNExposures);
    if (errorValue != DRV_SUCCESS) {
      sprintf(buffer2,"Error setting nber of exposures/cycle to %d\r\n",
            AndorSet1.KineticCycleNExposures);
      error_msg.Append(wxString(buffer2));
      errorFlag = true;
    } else {
      sprintf(buffer2,"Setting nber of exposures/cycle to %d\r\n",
              AndorSet1.KineticCycleNExposures);
      error_msg.Append(wxString(buffer2));
    }

// Set Kinetic nber of cycles
  errorValue = SetNumberKinetics(AndorSet1.KineticNCycles);
    if (errorValue != DRV_SUCCESS) {
      sprintf(buffer2,"Error setting total nber of cycles to %d\r\n",
            AndorSet1.KineticNCycles);
      error_msg.Append(wxString(buffer2));
      errorFlag = true;
    } else {
      sprintf(buffer2,"Setting total nber of cycles to %d\r\n",
              AndorSet1.KineticNCycles);
      error_msg.Append(wxString(buffer2));
    }

// Set Kinetic Clock Time (for successive exposures)
  fAccumTime = AndorSet1.KineticClockTime_msec / 1000.;
  errorValue = SetAccumulationCycleTime(fAccumTime);
    if (errorValue != DRV_SUCCESS) {
      sprintf(buffer2,"Error setting Accumulation cycle time to %d msec\r\n",
              AndorSet1.KineticClockTime_msec);
      error_msg.Append(wxString(buffer2));
      errorFlag = true;
    } else {
      sprintf(buffer2,"Error setting Accumulation cycle time to %d msec\r\n",
              AndorSet1.KineticClockTime_msec);
      error_msg.Append(wxString(buffer2));
    }
} // EOF case of acquisitionMode = 2, 3 or 4

// It is necessary to get the actual times as the system will calculate the
// nearest possible time. eg if you set exposure time to be 0, the system
// will use the closest value (around 0.01s)
  errorValue = GetAcquisitionTimings(&fExposure,&fAccumTime,&fKineticTime);
  if (errorValue == DRV_SUCCESS) {
    sprintf(buffer2 ,"Actual Exposure Time is %.3f sec\r\n", fExposure);
    error_msg.Append(wxString(buffer2));
    sprintf(buffer2 ,"Actual Accumulation Time is %.3f sec\r\n", fAccumTime);
    error_msg.Append(wxString(buffer2));
    sprintf(buffer2 ,"Actual Kinetic Time is %.3f sec\r\n", fKineticTime);
    error_msg.Append(wxString(buffer2));
    }

// Display all error messages:
    if(errorFlag) {
    	wxMessageBox( error_msg, "JLP_Andor_UpdateSettingsForExposures/Error!",
                   wxICON_ERROR);
        status = -1;
        }
#ifdef DEBUG
    else {
    	wxMessageBox( error_msg, "JLP_Andor_UpdateSettingsForExposures",
                   wxOK);
    }
#endif

return(status);
}
//------------------------------------------------------------------------
// Basic settings of the ANDOR camera according
// to the configuration stored in AndorSet1
//
// AD channel, preampli gain
// vertical, horizontal shift settings
// Shutter and trigger
//------------------------------------------------------------------------
int JLP_AndorCam1::JLP_Andor_ApplyBasicSettings()
{
bool errorFlag = false, ShutterAvailable0;
int errorValue, status = 0, conventional_mode;
wxString error_msg;
char buffer2[128];

if(not_connected) return(-1);

error_msg = wxT("");

// AD Channel:
errorValue = SetADChannel(AndorSet1.AD_Channel);
  if(errorValue != DRV_SUCCESS){
    sprintf(buffer2,"JLP_Andor_Init/Error setting AD channel to %d\n",
            AndorSet1.AD_Channel);
    error_msg.Append(wxString(buffer2));
    errorFlag = true;
    if(AndorSet1.AD_Channel != 0) SetADChannel(0);
  }


// Set vertical speed:
  errorValue = SetVSSpeed(AndorSet1.VShiftSpeed);
    if(errorValue != DRV_SUCCESS){
      error_msg.Append(wxT("JLP_Andor_InitAcquisition/Set Vertical Speed Error\n"));
      errorFlag = true;
    }

// Set Horizontal Speed:
    conventional_mode = (AndorSet1.EMGainMode == 2) ? 1 : 0;
    errorValue = SetHSSpeed(conventional_mode, AndorSet1.HShiftSpeed);
    if(errorValue != DRV_SUCCESS){
     error_msg.Append(wxT("JLP_Andor_InitAcquisition/Set Horizontal Speed Error\n"));
      errorFlag = true;
    }

// Pre-ampli gain:
errorValue = SetPreAmpGain(AndorSet1.HShiftPreAmpGain);
  if(errorValue != DRV_SUCCESS){
    sprintf(buffer2,"JLP_Andor_Init/Error setting PreAmpGain to %d\n",
            AndorSet1.HShiftPreAmpGain);
    error_msg.Append(wxString(buffer2));
    errorFlag = true;
    if(AndorSet1.AD_Channel != 0) SetPreAmpGain(0);
  }

// Vertical voltage amplitude:
errorValue = SetVSAmplitude(AndorSet1.VShiftVoltage);
  if(errorValue != DRV_SUCCESS){
    sprintf(buffer2,"JLP_Andor_Init/Error setting Vertical Shift Voltage to %d\n",
            AndorSet1.VShiftVoltage);
    error_msg.Append(wxString(buffer2));
//    errorFlag = true;
    if(AndorSet1.VShiftVoltage != 0) SetVSAmplitude(0);
  }

// Shutter config. is made up of TTL level, shutter mode and open close times
// Shutter mode: 0=automatic 1=open 2=close -1=NoShutterAvailable
  if(AndorSet1.ShutterMode != -1) {
    JLP_Andor_SetShutter(AndorSet1.ShutterMode, ShutterAvailable0);
    if(!ShutterAvailable0) AndorSet1.ShutterMode = -1;
    }

// Set trigger mode:
// internal: TriggerMode=0
// external: TriggerMode=1
// software: TriggerMode=10
  errorValue = SetTriggerMode(AndorSet1.TriggerMode);
  if(errorValue != DRV_SUCCESS) {
    sprintf(buffer2, "Error setting Trigger Mode to %d\r\n",
            AndorSet1.TriggerMode);
    error_msg.Append(wxString(buffer2));
    errorFlag = true;
  } else {
    error_msg.Append(wxT("OK: Trigger successfully set\r\n"));
  }

// Display all error messages:
    if(errorFlag) {
    	wxMessageBox( error_msg, "JLP_Andor_UpdateBasicSettings/Error!",
                   wxICON_ERROR);
        status = -1;
        }
#ifdef DEBUG
    else {
    	wxMessageBox( error_msg, "JLP_Andor_UpdateBasicSettings",
                   wxOK);
    }
#endif

return(status);
}
/*************************************************************************
* Initialize ANDOR camera for Single or Continuous Acquisition
* when acquisitionMode has been changed.
* Use the settings included in AndorSet
*
* INPUT:
*  AndorSet1.acquisitionMode
**************************************************************************/
int JLP_AndorCam1::Cam1_InitAcquisition(wxString& error_msg)
{
int errorValue, status = 0;
wxString buff0;

 if(not_connected) return(-1);

 error_msg = wxT("");
 if(AndorSet1.is_initialized == 0) {
    error_msg.Append(wxT("JLP_Andor_InitAcquisition/Error: AndorSet not initialized!\n"));
    return(-1);
    }

// JLP2015: Set BaselineClamp to one or zero according to AndorSet:
  SetBaselineClamp(AndorSet1.BaselineClamp);

// Set frame transfer to one or zero according to AndorSet:
  SetFrameTransferMode(AndorSet1.FrameTransferMode);

// SetAcquisitionMode() belongs to Andor DLL...
// For Single Image: AndorSet1.acquisitionMode = 1;
// For Continuous acquisition: AndorSet1.acquisitionMode = 5;
/* AcquisitionMode=5 Video1, Run till abort (= short exposures until "stopped")
*/
/*
* mode=1 Single Scan    (= long integration)
* mode=2 Accumulate     (= integration of short exposures)
* mode=3 Kinetics       (= video2, sequence of images)
* mode=4 Fast Kinetics  (= fast transfer on a hidden area of the CCD)
* mode=5 Run till abort (= video1, short exposures until "stop" button is clicked)
*/
 if((AndorSet1.AcquisitionMode != 1)
    && (AndorSet1.AcquisitionMode != 3)
    && (AndorSet1.AcquisitionMode != 5)) {
     error_msg.Printf(wxT("JLP_Andor_InitAcquisition/Error, unsupported acquisitionMode (=%d)"),
                      AndorSet1.AcquisitionMode);
     return(-2);
    }

// Continuous acquisition:
// Set Number loops to default
/*
  sprintf(aBuffer,"10");
  SendMessage(ebNumberLoops, WM_SETTEXT, 0, (LPARAM)(LPSTR)aBuffer);

// Select SoftTrigger as default
  sprintf(aBuffer,"SoftTrigger");
  SendMessage(cbTrigger, CB_SELECTSTRING,0,(LPARAM)(LPSTR)aBuffer);
*/

// Set acquisition mode:
  errorValue = SetAcquisitionMode(AndorSet1.AcquisitionMode);
  if(errorValue != DRV_SUCCESS){
     buff0.Printf(wxT("Setting acquisition mode = %d\n"), AndorSet1.AcquisitionMode);
     error_msg.Append(buff0);
     error_msg.Append(wxT("JLP_Andor_InitAcquisition/Error in SetAcquisitionMode: please change Andor settings\n"));
     status = -2;
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
int JLP_AndorCam1::Cam1_SetSystemForSingleAcquisition(int nx0, int ny0, int xc0, int yc0,
                                                      int xbin0, int ybin0)
{
int   errorValue, status = 0;
wxString error_msg, aBuffer2;

if(not_connected) return(-1);


// Allocate memory for image data.
// Size is returned for GetAcquiredData which needs the buffer size
  JLP_Andor_AllocateImageBuffer(nx0, ny0);
  AndorSet1.xbin = xbin0;
  AndorSet1.ybin = ybin0;
  AndorSet1.xc0 = xc0;
  AndorSet1.yc0 = yc0;

// This function needs only to be called when acquiring an image. It sets
// the horizontal and vertical binning and the area of the image to be
// captured. In this example it is set to 1x1 binning and is acquiring the
// whole image
// SetImage(int hbin, int vbin, int hstart, int hend, int vstart, int vend)
// Before:
//  SetImage(1, 1, 1, AndorSet1.gblXPixels, 1, AndorSet1.gblYPixels);
// Now:
  errorValue = SetImage(xbin0, ybin0, xc0 + 1, xc0 + nx0,
                        yc0 + 1, yc0 + ny0);
  if(errorValue != DRV_SUCCESS){
   error_msg.Printf(wxT("SetImage/Error with: xc0=%d, yc0=%d, xbin=%d ybin=%d nx0=%d ny0=%d\r\n"),
                        xc0, yc0, xbin0, ybin0, nx0, ny0);
   wxMessageBox( error_msg,
                      "JLP_Andor_SetSystemForSingleAcquisition",
                      wxICON_ERROR);
   status = -1;
   }

// DDEBUG: Display current settings in a popup window:
  Cam1_DisplaySettings();

// Starts an acquisition:
  status = Andor_StartSingleAcquisition();

return(status);
}
/*******************************************************************************
*
*  FUNCTION NAME:  ANDOR_StartSingleAcquisition()
*
*  DESCRIPTION:  This function starts an acquisition. It also starts a
*		  timer to check when the acquisition has finished.
********************************************************************************/
int JLP_AndorCam1::Andor_StartSingleAcquisition()
{
int   errorValue, status = 0;
wxString error_msg, aBuffer2;

if(not_connected) return(-1);

// Start the acquisition
  errorValue=StartAcquisition();
  if(AndorSet1.TriggerMode == 1)
    error_msg.Append(wxT("Waiting for external trigger\r\n"));
  if(errorValue != DRV_SUCCESS){
    status = -1;
    error_msg.Append(wxT("Start acquisition error\r\n"));
    Cam1_StopAcquisition(error_msg);
// Flag ANDOR_gblData is set when first acquisition is taken, tells
// system that there is now no data to display:
    gblData_is_ready = false;
  }
  else{
    error_msg.Append(wxT("Starting acquisition....\r\n"));
/* Every "TIMER" event, a request is sent to ANDOR (in ProcessTimer)
* When the acquisition is complete,
* the data is read from the card and displayed in the paint area.
*/
    errorValue = JLP_Andor_WaitForSingleImage(aBuffer2);
    if(errorValue) {
      status = -1;
      error_msg.Append(wxT("Cam1_WaitForSingleImage/Error waiting for single image\r\n"));
    } else {
      error_msg.Append(wxT("Cam1_WaitForSingleImage/Single image was retrieved\r\n"));
      gblData_is_ready = true;
      status = 0;
      }
  }

if(status) wxMessageBox( error_msg,
                      "JLP_Andor_SetSystemAndStartSingleAcquisition",
                      wxICON_ERROR);
#ifdef DEBUG
else
           wxMessageBox( error_msg,
                      "JLP_Andor_SetSystemAndStartSingleAcquisition",
                      wxICON_ERROR);
#endif

return status;
}
