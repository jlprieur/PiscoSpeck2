/************************************************************************
* "jlp_andor_cam1_set.cpp"
* JLP_AndorCam1 class
*
* JLP
* Version 17/09/2015
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_andor_cam1.h"      // ANDOR_SETTINGS, ...
#include "jlp_andor_utils.h"     // Copy_AndorSettings, ...

/****************************************************************************
*
****************************************************************************/
int JLP_AndorCam1::Cam1_SaveCameraSettingsToFile(char *filename)
{
 int status;
 status = SaveAndorSettingsToFile(filename, AndorSet1);
 return(status);
}
/****************************************************************************
*
****************************************************************************/
int JLP_AndorCam1::Cam1_LoadCameraSettingsFromFile(char *filename)
{
 int status;
 status = LoadAndorSettingsFromFile(filename, &AndorSet1);
 return(status);
}
/****************************************************************************
*
****************************************************************************/
int JLP_AndorCam1::Cam1_Write_CameraSettingsToFITSDescriptors(DESCRIPTORS *descrip)
{
 int status;
 status = Write_AndorSettingsToFITSDescriptors(descrip, AndorSet1);
 return(status);
}
/****************************************************************************
*
* Shutter config. is made up of TTL level, shutter mode and open close times
* Shutter mode: 0=automatic 1=open 2=close -1=NoShutterAvailable
*
*  ShutterMode = AndorSet1.ShutterMode;
*
****************************************************************************/
int JLP_AndorCam1::JLP_Andor_SetShutter(int ShutterMode, bool& ShutterAvailable)
{
int errorValue;
int TTLtype, OpeningTime, ClosingTime;

ShutterAvailable = false;


// Times in msec for opening and closing the shutter
OpeningTime = 1;
ClosingTime = 1;

// TTL level type: 0 if "low", 1 if "high"
TTLtype = 1;


// Set shutter to always open, closed, or automatic:
if(ShutterMode >= 0) {
  errorValue=SetShutter(TTLtype, ShutterMode, OpeningTime, ClosingTime);
  if(errorValue!=DRV_SUCCESS) {
    ShutterAvailable = false;
  } else {
    ShutterAvailable = true;
  }
}
return(0);
}
/****************************************************************************
* Check the value of the basic EM gain mode of the output register
****************************************************************************/
int JLP_AndorCam1::JLP_Andor_SetEMGainBasicMode(wxString& error_msg)
{
int errorValue, status = 0;

error_msg =wxT("");

// Try gain_mode=1 to have the [0,4096] range
  AndorSet1.EMGainMode = 1;
  errorValue = SetEMGainMode(1);
  if(errorValue != DRV_SUCCESS){
    switch(errorValue) {
      case DRV_NOT_INITIALIZED:
        error_msg.Printf(wxT("SetEMGainBasicMode/SetEMGainMode to 1/Error: system not initialized!\n"));
        break;
      case DRV_ACQUIRING:
        error_msg.Printf(wxT("SetEMGainBasicMode/SetEMGainMode to 1/Error: system is acquiring!\n"));
        break;
      case DRV_P1INVALID:
        error_msg.Printf(wxT("SetEMGainBasicMode/SetEMGainMode to 1/Error: EMGain mode 1 is invalid!\n"));
        break;
      case DRV_NOT_AVAILABLE:
        error_msg.Printf(wxT("SetEMGainBasicMode/SetEMGainMode to 1/Error: EMGain mode 1 is not allowed by this camera!\n"));
        break;
      default:
        error_msg.Printf(wxT("SetEMGainBasicMode/SetEMGainMode to 1/Error setting EMgain mode to 1 (status=%d)\n"),
                errorValue);
        break;
    }
  status = -2;
  } else {
  AndorSet1.EMGainBasicMode = 1;
  status = 0;
  }

// If error with 1, try with 0:
if(status) {
  AndorSet1.EMGainMode = 0;
  errorValue = SetEMGainMode(0);
  if(errorValue != DRV_SUCCESS){
    error_msg.Printf(wxT("JLP_Andor_SetEMGainBasicMode/SetEMGainMode/Error setting EMgain mode to 0 (status=%d)\n"),
            errorValue);
    AndorSet1.EMGainBasicMode = 0;
    AndorSet1.EMGainLowValue = 0;
    AndorSet1.EMGainHighValue = 0;
    return(-1);
  } else {
   AndorSet1.EMGainBasicMode = 0;
   status = 0;
  }
  }

/* Setting EMCCD mode
Mode 0: The EM Gain is controlled by DAC settings in the range 0-255.
        Default mode.
Mode 1: The EM Gain is controlled by DAC settings in the range 0-4095.
Mode 2: Linear mode.
Mode 3: Real EM gain
*/
// LUCA EMCCD: mode = 0
// IXON/DU 887 EMCCD: mode = 1
// linear: mode = 2
 errorValue = GetEMGainRange(&(AndorSet1.EMGainLowValue),
                             &(AndorSet1.EMGainHighValue));
  if(errorValue != DRV_SUCCESS){
    error_msg.Append(wxT("JLP_Andor_SetEMGainBasicMode/Error getting EMCCD gain range\n"));
    AndorSet1.EMGainLowValue = 0;
    AndorSet1.EMGainHighValue = 0;
    status = -1;
    }

return(status);
}
/****************************************************************************
* Set the EM gain to the output register
****************************************************************************/
int JLP_AndorCam1::JLP_Andor_SetEMGain(wxString& error_msg)
{
int errorValue, status = 0, ilow, ihigh;

error_msg =wxT("");

 if(AndorSet1.EMGainValue < AndorSet1.EMGainLowValue
    || AndorSet1.EMGainValue > AndorSet1.EMGainHighValue) {
    error_msg.Printf(wxT("JLP_Andor_SetEMGain/Error: gain value = %d out of allowed range [%d %d]"),
            AndorSet1.EMGainValue, AndorSet1.EMGainLowValue, AndorSet1.EMGainHighValue);
    return(-1);
    }

// Set gain of EMCCD
// LUCA: between 0 and 255 since EM gain mode is set to 0
// IXON: between 0 and 4095 since EM gain mode is set to 1
  errorValue = SetEMCCDGain(AndorSet1.EMGainValue);
  if (errorValue == DRV_P1INVALID) {
    GetEMGainRange(&ilow, &ihigh);
    error_msg.Printf(
     wxT("Error setting EMCCD gain to %d\r\n Current valid range (according to temp.): [%d,%d] (will become: [%d,%d])\r\n"),
     AndorSet1.EMGainValue, ilow, ihigh, AndorSet1.EMGainLowValue, AndorSet1.EMGainHighValue);
    status = -2;
    } else if (errorValue != DRV_SUCCESS) {
    error_msg.Printf(wxT("Error setting EMCCD gain to value=%d Error_status=%d\r\n"),
            AndorSet1.EMGainValue, errorValue);
    status = -2;
    }

return(status);
}
/***************************************************************************
* Set default values for JLP_Andor_Settings
*
* WARNING: SHOULD NOT CHANGE gblXPixels, ... !
*
* Size of subwindow, center of subwindow, bin factor, exposure time
* Acquisition mode, EMGainValue, number of exposures
*
****************************************************************************/
int JLP_AndorCam1::Cam1_SettingsToDefault(int nx_full, int ny_full)
{
int errorValue;
float fspeed;

// For Single Image: AndorSet1.acquisitionMode = 1;
// For Continuous acquisition: AndorSet1.acquisitionMode = 5;
// For series of images : AndorSet1.acquisitionMode = 3;
/* AcquisitionMode=5 Run till abort (= short exposures until "stopped")
*/
   AndorSet1.AcquisitionMode = 5;

// Image mode: readMode = 4;
   AndorSet1.readMode = 4;

// Cooler on: (set it to false, so that the user can select it aftwerwards)
   AndorSet1.CoolerOn = false;

// Cooler to 13 degrees (for DDEBUG):
   AndorSet1.CoolerSetTemp = 13.;

// Size of elementary images:
// 128x128
   AndorSet1.nx1 = 128;
   AndorSet1.ny1 = 128;

// Bin factor :
   AndorSet1.xbin = 1;
   AndorSet1.ybin = 1;

// Full CCD matrix:
   AndorSet1.gblXPixels = nx_full;
   AndorSet1.gblYPixels = ny_full;

   AndorSet1.xc0 = AndorSet1.gblXPixels/2;
   AndorSet1.yc0 = AndorSet1.gblYPixels/2;

// Rotation of the elementary images (0=no rotation)
   AndorSet1.RotationMode = 0;

// TriggerMode (0=internal 1=external 10=software)
   AndorSet1.TriggerMode = 0;

// Gain of EMCCD:
   if(AndorSet1.is_initialized == 0) {
     AndorSet1.EMGainLowValue = 0;
     AndorSet1.EMGainHighValue = 3000;
     AndorSet1.EMGainBasicMode = 0;
   }
   AndorSet1.EMGainMode = AndorSet1.EMGainBasicMode;
   AndorSet1.EMGainValue = AndorSet1.EMGainLowValue +
                         (90 * (AndorSet1.EMGainHighValue - AndorSet1.EMGainLowValue)) / 100;

// Set trigger mode:
// 0 = internal
// 1 = external
// 10 = software trigger (better for continuous mode, when available):
   if(AndorSet1.is_initialized == 0) {
     AndorSet1.TriggerMode = 0;
     } else {
      if(IsTriggerModeAvailable(10) == DRV_SUCCESS) {
        AndorSet1.TriggerMode = 10;
       } else {
       AndorSet1.TriggerMode = 0;
       }
    }

// Shutter mode: 0=automatic 1=open 2=close -1=NoShutterAvailable
   AndorSet1.ShutterMode = 1;
   AndorSet1.CloseShutterWhenExit = 1;

// Mirroring the elementary images (0=no mirror)
   AndorSet1.MirrorMode = 0;

// Kinetic exposure time in milliseconds:
   AndorSet1.KineticExpoTime_msec = 20;

// Kinetic clock time in milliseconds:
   AndorSet1.KineticClockTime_msec = AndorSet1.KineticExpoTime_msec + 1;

// Kinetic number of exposures/cyle:
   AndorSet1.KineticCycleNExposures = 1;

// Kinetic cycle time in milliseconds:
   AndorSet1.KineticCycleTime_msec = AndorSet1.KineticCycleNExposures
                                     * AndorSet1.KineticClockTime_msec;

// Kinetic number of cyles:
   AndorSet1.KineticNCycles = 100;

// Frame Transfer Mode:
   AndorSet1.FrameTransferMode = 0;

// Baseline Clamp
   AndorSet1.BaselineClamp = 1;

// Long exposure time (in seconds)
   AndorSet1.IntegTime_sec = 10.0;

// Horizontal and vertical shift speeds:
   AndorSet1.VShiftSpeed = 0;
   AndorSet1.VShiftVoltage = 0;
   AndorSet1.HShiftPreAmpGain = 0;
   AndorSet1.HShiftSpeed = 0;

// AD_Channel:
   AndorSet1.AD_Channel = 0;

// Set Vertical speed to maximum value:
   if(AndorSet1.is_initialized == 0) {
     AndorSet1.VShiftSpeed = 0;
   } else {
     errorValue = GetFastestRecommendedVSSpeed (&AndorSet1.VShiftSpeed,&fspeed);
     if(errorValue != DRV_SUCCESS){
      wxMessageBox(_T("Error in GetFastestRecommendedVSSpeed"),
                   _T("JLP_AndorCam1::Cam1_SettingsToDefault"), wxICON_ERROR);
      return(-4);
     }
   }

return(0);
}
/************************************************************************
* Start Cooler
************************************************************************/
int  JLP_AndorCam1::JLP_Andor_StartCooler()
{
wxString error_msg;
wxString buffer;
int status = 0, errorValue;
bool errorFlag = false;

error_msg = wxT("");

if(AndorSet1.CoolerOn == false) return(0);

// Set the temperature:
  status = Cam1_SetCoolerTemp(buffer);
  if(status) {
    errorFlag = true;
    error_msg.Append(buffer);
    status = -1;
    }
// Start cooler:
  if(!status) {
    errorValue = CoolerON();
    if(errorValue == DRV_SUCCESS) {
      error_msg.Append(wxT("Cooler is on\r\n"));
      } else {
      errorFlag = true;
      error_msg.Append(wxT("Error: cannot start cooler\r\n"));
      status = -1;
      }
    }

// Display current settings to popup window:
  if(errorFlag)
   wxMessageBox( error_msg, _T("Error starting Andor Cooler"), wxICON_ERROR);
#ifdef DEBUG
  else
   wxMessageBox( error_msg, _T("Starting Andor Cooler"), wxOK);
#endif

return(status);
}
/************************************************************************
* Modify cooler temperature
************************************************************************/
int JLP_AndorCam1::Cam1_SetCoolerTemp(wxString& error_msg)
{
char buffer[128];
int status = 0, errorValue;
int min_temp, max_temp;

// SetTemperature not available for the LUCA R: automatically set to -20 degrees
if(!strncmp(AndorSet1.HeadModel, "Luc", 3)) {
 AndorSet1.CoolerSetTemp = -20.;
 return(0);
 }

 strcpy(buffer, "");

// Enquire the temperature range:
 errorValue = GetTemperatureRange(&min_temp, &max_temp);
 if(errorValue == DRV_SUCCESS) {
   error_msg.Printf(wxT("Cooler temperature can be set between %d and %d deg. C\r\n"),
                    min_temp, max_temp);
   } else {
   error_msg.Printf(wxT("Error: cannot get cooler temperature valid range.\r\n"));
   return(-1);
   }

 if((AndorSet1.CoolerSetTemp < min_temp) || (AndorSet1.CoolerSetTemp > max_temp)) {
  error_msg.Printf(wxT("Invalid cooling temperature (%.2f deg C): valid range is [%d, %d]\r\n"),
              AndorSet1.CoolerSetTemp, min_temp, max_temp);
  return(-1);
  }

// Set the temperature:
 errorValue = SetTemperature(AndorSet1.CoolerSetTemp);
 if(errorValue == DRV_SUCCESS) {
   printf(buffer,"Setting temperature to %.2f deg C (DDEBUG)\r\n", AndorSet1.CoolerSetTemp);
   error_msg.Append(wxString(buffer));
   } else {
   sprintf(buffer,"SetTemperature/Error setting temperature to %.2f deg C\r\n", AndorSet1.CoolerSetTemp);
   error_msg.Append(wxString(buffer));
   if(errorValue == DRV_P1INVALID) error_msg.Append(wxT("Invalid Temperature"));
     else if(errorValue == DRV_NOT_INITIALIZED) error_msg.Append(wxT("System not initialized"));
     else if(errorValue == DRV_ERROR_ACK) error_msg.Append(wxT("Error communicating with card"));
     else if(errorValue == DRV_NOT_SUPPORTED) error_msg.Append(wxT("Not supported"));
   status = -1;
   }

return(status);
}
/************************************************************************
* Stop Cooler
************************************************************************/
int  JLP_AndorCam1::JLP_Andor_StopCooler()
{
int status = 0;
int errorValue;

  errorValue = CoolerOFF();        // Switch off cooler (if used)
  if(errorValue != DRV_SUCCESS) {
    wxMessageBox(_T("Warning: error switching cooler off"),
                 _T("JLP_Andor_StopCooler"), wxICON_ERROR);
    status = -1;
    }

return(status);
}
