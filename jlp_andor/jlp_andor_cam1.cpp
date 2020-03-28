/************************************************************************
* "jlp_andor_cam1.cpp"
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

//---------------------------------------------------------------------------

// #define DEBUG

/***************************************************************************
* Start dialog with Andor camera
****************************************************************************/
JLP_AndorCam1::JLP_AndorCam1()
{
int status;
wxString error_msg;

not_connected = true;
close_shutter_when_exit = true;

AndorSet1.pImageArray = NULL;

status = Cam1_ConnectToCamera();

if(status == 0) {
  not_connected = false;
  JLP_Andor_GetCapabilities();
  }

// Set AndorSet1.is_initialized to 1 or 0:
  JLP_Andor_FirstInit();

return;
}
/***************************************************************************
* Release the Andor camera
****************************************************************************/
JLP_AndorCam1::~JLP_AndorCam1()
{
if(!not_connected) Cam1_ShutDown();
}
/*************************************************************************
* Open connection to ANDOR camera for the first time (if present)
*
**************************************************************************/
int JLP_AndorCam1::Cam1_ConnectToCamera()
{
int errorValue;
long index, n_cameras, camera_handle;
char buffer0[128];
wxString buffer;
wxString error_msg, current_directory;

  AndorSet1.is_initialized = 0;

// Declare Image Buffers
   AndorSet1.pImageArray = NULL;

// This flag is set when first acquisition is taken, tells
// system that there is data to display:
   gblData_is_ready = false;

   error_msg = _T("Cam1_ConnectToCamera");

// Number of available cameras:
   errorValue = GetAvailableCameras(&n_cameras);
   if(errorValue != DRV_SUCCESS || n_cameras == 0){
      error_msg.Append(_T("ANDOR_GetAvailableCameras: error, no connected camera !\n"));
      wxMessageBox(error_msg, _T("Cam1_ConnectToCamera"), wxOK | wxICON_ERROR);
      return(-1);
    }

// Select first camera:
   if(n_cameras > 1) {
   index = 0;
   GetCameraHandle(index, &camera_handle);
   errorValue = SetCurrentCamera(camera_handle);
   if(errorValue != DRV_SUCCESS){
      error_msg.Append(_T("SetCurrentCamera: error selecting first camera !\n"));
      wxMessageBox(error_msg, _T("Cam1_ConnectToCamera"), wxOK | wxICON_ERROR);
      return(-1);
     }
   }

// Initialize driver in current directory by reading DETECTOR.INI
// (only for old cameras: not needed any longer ...)
// Look in current working directory for driver files
  current_directory = wxGetCwd();

// Initialize() belongs to Andor DLL...
   strcpy(buffer0, current_directory.c_str());
   errorValue = Initialize(buffer0);
   error_msg = _T("Initialize opening DETECTOR.INI in ") + current_directory + "\n";

   if(errorValue != DRV_SUCCESS){
      error_msg.Append(_T("Initialize error with DETECTOR.INI (not a problem for modern cameras)\n"));
      wxMessageBox(error_msg, wxT("Cam1_ConnectToCamera"), wxOK | wxICON_INFORMATION);
    }

return(0);
}
/*************************************************************************
* Get the capabilities of the connected ANDOR camera
**************************************************************************/
int JLP_AndorCam1::JLP_Andor_GetCapabilities()
{
int errorValue, status;
wxString error_msg;
bool errorFlag = false;

// Get AndorCapabilities structure from common.c
  AndorSet1.AndorCaps.ulSize = sizeof(AndorCapabilities);

// GetCapabilities() belongs to Andor DLL...
// Get camera capabilities
    errorValue = GetCapabilities(&AndorSet1.AndorCaps);
    if(errorValue != DRV_SUCCESS){
      error_msg.Append(_T("ANDOR_GetCapabilities/Error : no information ?\n"));
      errorFlag = true;
    } else {
// To allow selection of continuous mode,
// make sure we have the appropriate hardware for this mode:
// Test if software trigger can be used
      if(((AndorSet1.AndorCaps.ulTriggerModes & AC_TRIGGERMODE_CONTINUOUS)
          == false) &&
// Test if internal trigger can be used
         ((AndorSet1.AndorCaps.ulTriggerModes & AC_TRIGGERMODE_INTERNAL)
          == false)) {
        error_msg.Append(_T("Incorrect hardware (Trigger) for Continuous Mode\n"));
        errorFlag = true;
    }
  }

// GetHeadModel() belongs to Andor DLL...
// Get Head Model
    errorValue = GetHeadModel(AndorSet1.HeadModel);
    if(errorValue == DRV_SUCCESS){
      error_msg.Append(_T("GetHeadModel:") + wxString(AndorSet1.HeadModel));
    } else {
      error_msg.Append(_T("Error in GetHeadModel (no information ?)\n"));
      strcpy(AndorSet1.HeadModel,"Unknown model");
      errorFlag = true;
    }
 if(errorFlag) {
    wxMessageBox(error_msg, wxT("JLP_Andor_GetCapabilities"), wxOK | wxICON_ERROR);
    status = -1;
 } else {
    wxMessageBox(error_msg, wxT("JLP_Andor_GetCapabilities"), wxOK);
    status = 0;
 }
return(status);
}
/*************************************************************************
* Initialize ANDOR camera for the first time (called
* when creating AndorSetupBox)
* Selection of the settings included in AndorSet
*
* Set AndorSet1.is_initialized to 1 or 0
*
**************************************************************************/
int JLP_AndorCam1::JLP_Andor_FirstInit()
{
int errorValue, status, nx_full, ny_full;
bool errorFlag = false;
wxString buffer;
wxString error_msg;

// Set default values to AndorSet1, with nx_full=512, ny_full=512
// to avoid problems when Andor is not connected:
 nx_full = 512;
 ny_full = 512;
 Cam1_SettingsToDefault(nx_full, ny_full);

 if(not_connected) {
    wxMessageBox(wxT("Error: camera not connected!"),
                 wxT("JLP_Andor_FirstInit"), wxICON_ERROR);
    AndorSet1.is_initialized = 0;
   return(-1);
 }

// GetDetector() belongs to Andor DLL...
// Get detector information : gblXPixels, gblYPixels
    errorValue = GetDetector(&nx_full, &ny_full);
    if(errorValue != DRV_SUCCESS){
      error_msg.Append(_T("Error in GetDetector\n"));
      errorFlag = true;
    }
 AndorSettingsToDefault(&AndorSet1, nx_full, ny_full);

// Set EM basic mode and get gain valid range:
  status = JLP_Andor_SetEMGainBasicMode(buffer);
  if(status) {
      error_msg.Append(buffer);
      error_msg.Append(_T("Error getting EM basic mode and gain range"));
      errorFlag = true;
      }

// Default value for the EMgain:
   AndorSet1.EMGainValue = AndorSet1.EMGainLowValue +
                         (90 * (AndorSet1.EMGainHighValue
                             - AndorSet1.EMGainLowValue)) / 100;

  /*
#ifdef DEBUG
  sprintf(debug_msg,"xbin=%d ybin=%d", AndorSet1.xbin, AndorSet1.ybin);
  wxMessageBox( debug_msg, "DDEBUG", wxOK);
#endif
  */

// Display all error messages if present:
    if(errorFlag) {

    	wxMessageBox(error_msg, _T("Cam1_FirstInit"),
                     wxICON_ERROR);
        status = -1;

    } else {

        AndorSet1.is_initialized = 1;

// Apply all the current settings:
        JLP_Andor_ApplyBasicSettings();
        JLP_Andor_ApplySettingsForExposures();
        status = JLP_Andor_SetEMGain(buffer);
        if(status) wxMessageBox( buffer,
                   _T("JLP_Andor_FirstInit/JLP_Andor_SetEMGain"),
                   wxICON_ERROR);

// This is the first time cooler is called:
        if(AndorSet1.CoolerOn) JLP_Andor_StartCooler();

        status = 0;
        }


return status;
}
/****************************************************************************
* Shutdown ANDOR camera
* and take care of cooling system (slow re-heating...)
*
*****************************************************************************/
int JLP_AndorCam1::Cam1_ShutDown()
{
int errorValue, status = 0;
bool shutter_available;
int shutter_mode;

  if(AndorSet1.is_initialized == 0) return(-1);

// Switch off cooler (if used)
  status = JLP_Andor_StopCooler();

// Close shutter:
//  int JLP_AndorCam1::JLP_Andor_SetShutter(int ShutterMode, bool& ShutterAvailable)
// Shutter mode: 0=automatic 1=open 2=close -1=NoShutterAvailable
if(close_shutter_when_exit) {
  shutter_mode = 2;
  } else {
  wxMessageBox(wxT("Warning: shutting down with shutter opened !"),
               wxT("JLP_Andor_Shutdown"), wxICON_INFORMATION);
  shutter_mode = 1;
  }
  JLP_Andor_SetShutter(shutter_mode, shutter_available);

// Shut down system
  errorValue = ShutDown();
  if(errorValue != DRV_SUCCESS) {
    wxMessageBox(_T("Warning: error shutting down"),_T("JLP_Andor_Shutdown"),
               wxICON_ERROR);
    status = -3;
    }

// Free Image Buffer if allocated
 JLP_Andor_FreeImageBuffer();

AndorSet1.is_initialized = 0;

return(status);
}
/************************************************************************
* Display current Andor settings in a popup window
* for all image acquisition modes
************************************************************************/
int  JLP_AndorCam1::Cam1_DisplaySettings()
{
wxString  aBuffer, aBuffer1;
char aBuffer2[128];
float speed;
float	fExposure,fAccumTime,fKineticTime;
int errorValue, min_temp, max_temp;
double sensor_temp0, box_temp0;
int gain;

if(AndorSet1.is_initialized == 0) {
    wxMessageBox(
    _T("Not initialized! Please initialize the ANDOR camera"),
    _T("JLP_Andor_DisplaySettings/Error"), wxICON_ERROR);
    return(-1);
    }

// Build status messages
  aBuffer.Printf(wxT("Head Model %s\r\n"), AndorSet1.HeadModel);
  switch(AndorSet1.TriggerMode) {
    case 0:
    default:
      aBuffer.Append(wxT("Internal trigger\r\n"));
      break;
    case 1:
      aBuffer.Append(wxT("External trigger\r\n"));
      break;
    case 2:
      aBuffer.Append(wxT("Software trigger\r\n"));
      break;
    }

  if(AndorSet1.readMode == 4) aBuffer.Append(wxT("Set to Image Mode\r\n"));

/*
* mode=1 Single Scan    (= long integration)
* mode=2 Accumulate     (= integration of short exposures)
* mode=3 Kinetics       (= sequence of images)
* mode=4 Fast Kinetics  (= fast transfer on a hidden area of the CCD)
* mode=5 Run till abort (= short exposures until "stop" button is clicked)
*/
  switch(AndorSet1.AcquisitionMode) {
    default:
    case 1:
      aBuffer.Append(wxT("Single Scan (long integration)\r\n"));
      break;
    case 2:
      aBuffer.Append(wxT("Accumulate (integration of short exposures)\r\n"));
      break;
    case 3:
      aBuffer.Append(wxT("Kinetics (sequence of images) (Video2)\r\n"));
      break;
    case 4:
      aBuffer.Append(wxT("Fast Kinetics (fast transfer on a hidden area od the CCD)\r\n"));
      break;
    case 5:
      aBuffer.Append(wxT("Run till abort (short exposures until \"stop\" button is pressed) (Video1)\r\n"));
      break;
  }
  sprintf(aBuffer2,"Full frame size: %d x %d pixels\r\n",
           AndorSet1.gblXPixels, AndorSet1.gblYPixels);
  aBuffer.Append(wxString(aBuffer2));

// Prompt to Andor camera the value of Vertical speed:
  errorValue = GetVSSpeed(AndorSet1.VShiftSpeed, &speed);
  if(errorValue == DRV_SUCCESS) {
    sprintf(aBuffer2,"Vertical Speed set to %g microsec/pixel shift\r\n",speed);
    aBuffer.Append(wxString(aBuffer2));
    } else {
    sprintf(aBuffer2,"Error: cannot get vertical speed value\r\n");
    aBuffer.Append(wxString(aBuffer2));
    }

// Prompt to Andor camera the value of Horizontal speed:
  errorValue = GetHSSpeed(0, 0, AndorSet1.HShiftSpeed, &speed);
  if(errorValue == DRV_SUCCESS) {
// If using an iXon the speed is given in MHz
    if(AndorSet1.AndorCaps.ulCameraType == 1)
      sprintf(aBuffer2,"Horizontal Speed set to %g MHz\r\n",speed);
    else
      sprintf(aBuffer2,"Horizontal Speed set to %g microseconds per pixel shift\r\n",speed);
    aBuffer.Append(wxString(aBuffer2));
    } else {
    sprintf(aBuffer2,"Error: cannot get horizontal speed value\r\n");
    aBuffer.Append(wxString(aBuffer2));
    }

// Enquire the temperature range if not a Luc285_MONO model:
  if(strncmp(AndorSet1.HeadModel, "Luc", 3) != 0) {
    errorValue = GetTemperatureRange(&min_temp, &max_temp);
    if(errorValue == DRV_SUCCESS) {
        sprintf(aBuffer2,"Temperature can be set between %d and %d deg. C\r\n",
                min_temp, max_temp);
      aBuffer.Append(wxString(aBuffer2));
      } else {
      sprintf(aBuffer2,"Error: cannot get temperature range.\r\n");
      aBuffer.Append(wxString(aBuffer2));
      }
  }

// Enquire the temperature:
     Cam1_GetTemperature(&sensor_temp0, &box_temp0, aBuffer1);
     aBuffer.Append(aBuffer1);

// Display read out time
  errorValue = GetAcquisitionTimings(&fExposure,&fAccumTime,&fKineticTime);
  if(errorValue == DRV_SUCCESS) {
      sprintf(aBuffer2,"Acquisition: Exposure=%.3f AccumTime=%.3f KineticTime=%.3f\r\n",
              fExposure, fAccumTime, fKineticTime );
    aBuffer.Append(wxString(aBuffer2));
    } else {
    sprintf(aBuffer2,"Error: cannot get Acquisition timings.\r\n");
    aBuffer.Append(wxString(aBuffer2));
    }

// Get gain of EMCCD:
//if(!(AndorSet1.AndorCaps.ulGetFunctions & AC_GETFUNCTION_EMCCDGAIN)) {
  errorValue = GetEMCCDGain(&gain);
  if (errorValue == DRV_SUCCESS) {
    sprintf(aBuffer2,"Current value of EMCCD Gain = %d\r\n", gain);
    aBuffer.Append(wxString(aBuffer2));
    } else {
      switch (errorValue) {
        case DRV_NOT_INITIALIZED:
          sprintf(aBuffer2,"Error reading EMCCD Gain (system not initialized)!\r\n");
          break;
        case DRV_ACQUIRING:
          sprintf(aBuffer2,"Error reading EMCCD Gain (system is acquiring)!\r\n");
          break;
        case DRV_ERROR_ACK:
          sprintf(aBuffer2,"Error reading EMCCD Gain (unable to communicate with card) !\r\n");
          break;
        default:
          sprintf(aBuffer2,"Error: cannot read EMCCD Gain\r\n");
          break;
      }
    aBuffer.Append(wxString(aBuffer2));
    }
/*
  } else {
    sprintf(aBuffer2,"This camera does not allow to read EMCCD gain value...\r\n");
    aBuffer.Append(wxString(aBuffer2));
  }
*/

// Display EMCCD gain range:
    sprintf(aBuffer2,"Valid range for EMCCD Gain is [%d %d]\r\n",
            AndorSet1.EMGainLowValue, AndorSet1.EMGainHighValue);
    aBuffer.Append(wxString(aBuffer2));

// Display current settings to popup window:
  wxMessageBox( aBuffer, _T("Andor current settings"), wxOK);

return(0);
}
/**************************************************************************
* Enquire the current exposure time to Andor camera
*
* OUTOUT:
* expo_time0: current exposure time in msec
**************************************************************************/
int  JLP_AndorCam1::Cam1_GetExpoTime(double *expo_time0)
{
// ZZZ Not implemented yet...
*expo_time0 = 0.;
return(0);
}
/**************************************************************************
* Enquire the current digital gain to Andor camera
*
* OUTOUT:
* digital_gain0: gain value
**************************************************************************/
int  JLP_AndorCam1::Cam1_GetDigitalGain(double *digital_gain0)
{
// ZZZ Not implemented yet...
*digital_gain0 = 1.;
return(0);
}
/**************************************************************************
* Enquire the current temperature to Andor camera
* Need a short output message since it is displayed on the main form
* of the program
*
* OUTOUT:
* sensor_temp: current sensor temperature value in deg. C
* box_temp: current electronic board temperature value in deg. C
* info_msg: miscellaneous information
**************************************************************************/
int  JLP_AndorCam1::Cam1_GetTemperature(double *sensor_temp, double *box_temp,
                                             wxString& info_msg)
{
int errorValue, status = -1, current_temp0;

*sensor_temp = 10.;
*box_temp = 10.;
info_msg = wxT("");

  errorValue = GetTemperature(&current_temp0);
  if(errorValue == DRV_TEMP_STABILIZED) {
      info_msg.Printf(wxT("Stabilized temperature at %d deg. C"),
                      current_temp0);
      *sensor_temp = (double)current_temp0;
      status = 0;
    } else {
    switch (errorValue) {
      default:
        info_msg.Printf(wxT("Error: cannot read temperature."));
        break;
      case DRV_NOT_INITIALIZED:
        info_msg.Printf(wxT("Error: system not initialized"));
        break;
      case DRV_ACQUIRING:
        info_msg.Printf(wxT("T=%d ? (running acquisition)"), current_temp0);
        break;
      case DRV_ERROR_ACK:
        info_msg.Printf(wxT("Error: unable to communicate with card"));
        break;
      case DRV_TEMP_OFF:
        info_msg.Printf(wxT("T=%d (cooler is off)"), current_temp0);
        break;
      case DRV_TEMP_NOT_REACHED:
        info_msg.Printf(wxT("T=%d (set point not reached yet)"), current_temp0);
        status = 0;
        break;
      case DRV_TEMP_NOT_STABILIZED:
      case DRV_TEMP_DRIFT:
        info_msg.Printf(wxT("T=%d (not stabilized yet)"), current_temp0);
        status = 0;
        break;
      }
    }

return(status);
}
//------------------------------------------------------------------------
// Inquire if cooling is on, and what is the current temperature:
//------------------------------------------------------------------------
int JLP_AndorCam1::JLP_Andor_CheckIfCoolerIsOn(bool *Cooler_is_already_on,
                                          double *sensor_temp)
{
double box_temp;
int status, icoolerStatus;
wxString error_msg;

*Cooler_is_already_on = false;
*sensor_temp = 10.;

status = IsCoolerOn(&icoolerStatus);
if(status == DRV_SUCCESS) {
 *Cooler_is_already_on = (icoolerStatus == 1) ? true: false;
 } else {
  return(-1);
 }


status = Cam1_GetTemperature(sensor_temp, &box_temp, error_msg);

return(status);
}
/***************************************************************************
* Get ANDOR_SETTINGS structure
****************************************************************************/
int JLP_AndorCam1::Cam1_Get_AndorSettings(ANDOR_SETTINGS* AndorSet0)
{
int status;
// Copy from AndorSet1 to AndorSet0
status = Copy_AndorSettings(AndorSet1, AndorSet0);
return(status);
}
/***************************************************************************
* Load ANDOR_SETTINGS structure
****************************************************************************/
int JLP_AndorCam1::Cam1_Load_AndorSettings(ANDOR_SETTINGS AndorSet0)
{
int status;
// Copy from AndorSet0 to AndorSet1
status = Copy_AndorSettings(AndorSet0, &AndorSet1);
return(status);
}
/***************************************************************************
* Get JLP_CAMERA_SETTINGS structure
****************************************************************************/
int JLP_AndorCam1::Cam1_Get_CameraSettings(JLP_CAMERA_SETTINGS* CamSet0)
{
int status;
// Copy AndorSet1 to CamSet0
status = Copy_AndorToCameraSettings(AndorSet1, CamSet0);
return(status);
}
/***************************************************************************
* Load JLP_CAMERA_SETTINGS structure
****************************************************************************/
int JLP_AndorCam1::Cam1_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0)
{
int status;
// Copy CamSet0 to AndorSet1
status = Copy_CameraToAndorSettings(CamSet0, &AndorSet1);
return(status);
}
/*******************************************************************************
*  FUNCTION NAME: JLP_Andor_AllocateImageBuffer()
*
*  RETURNS:	   ImageSize:  size of the image buffer
*
*  DESCRIPTION:  This function allocates memory for the image buffer (if not
*		  allocated already).
*
********************************************************************************/
int JLP_AndorCam1::JLP_Andor_AllocateImageBuffer(int nx0, int ny0)
{

  JLP_Andor_FreeImageBuffer();

// Allocate only if necessary
  if(AndorSet1.pImageArray == NULL) {
    AndorSet1.ImageSize = nx0 * ny0;
  	AndorSet1.pImageArray = new AWORD[AndorSet1.ImageSize];
  }
return(0);
}
/*******************************************************************************
* FUNCTION NAME: JLP_Andor_FreeImageBuffer()
*
* RETURNS:	  NONE
*
* DESCRIPTION:  This function frees the memory allocated for the image buffer.
*
********************************************************************************/
int JLP_AndorCam1::JLP_Andor_FreeImageBuffer()
{
int status = -1;

// Free all allocated memory
  if(AndorSet1.pImageArray != NULL){
    delete[] AndorSet1.pImageArray;
    AndorSet1.pImageArray = NULL;
    status = 0;
  }

return(status);
}

