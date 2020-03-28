/************************************************************************
* "jlp_camera_utils.h"
*
* JLP
* Version 05/10/2017
*************************************************************************/
#include <stdio.h>
#include <string.h>

#include "jlp_camera_utils.h"
#include "asp2_rw_config_files.h" // ReadString....

/****************************************************************************
* First initialization of Camera Settings (needed for before creating JLP_CameraCam1)
****************************************************************************/
void CameraSettingsToDefault(JLP_CAMERA_SETTINGS* out_CamSet, int nx_full, int ny_full)
{

// Head Model
strcpy(out_CamSet->HeadModel, "");
// Detector dimensions
out_CamSet->gblXPixels = nx_full;
out_CamSet->gblYPixels = ny_full;
// 5 for continuous streaming
out_CamSet->AcquisitionMode = 5;
// Shutter: 0=auto 1=open 2=closed
out_CamSet->ShutterMode = 1;
// Rotation mode: 0 for 0, 1 for +90, 2 for +180, 3 for +270
out_CamSet->RotationMode = 0;
// MirrorMode: 0=none, 1=vertical, 2=horizontal 3=vertical+horizontal
out_CamSet->MirrorMode = 0;
// CloseShutterWhenExit (0/1)
out_CamSet->CloseShutterWhenExit = 1;
// Center of subwindow
out_CamSet->xc0 = out_CamSet->gblXPixels/ 2;
out_CamSet->yc0 = out_CamSet->gblYPixels/ 2;
// Binning factors
out_CamSet->xbin = 1;
out_CamSet->ybin = 1;
// Size of elementary frames
out_CamSet->nx1 = 128;
out_CamSet->ny1 = 128;
// Long exposure time in seconds
out_CamSet->IntegTime_sec = 5;
// Kinetic: exposure time in milliseconds
out_CamSet->ExpoTime_msec = 10;
// Flag set to true if Cooler is wanted
out_CamSet->CoolerOn = 1;
// Cooler temperature when Cooler is on
out_CamSet->CoolerSetTemp = 10.;

return;
}
/***************************************************************************
* Copy JLP_CAMERA_SETTINGS structure
*
* INPUT:
*  in_CamSet
*
* OUTPUT:
*  out_CamSet
****************************************************************************/
int Copy_CameraSettings(JLP_CAMERA_SETTINGS in_CamSet, JLP_CAMERA_SETTINGS *out_CamSet)
{
strcpy(out_CamSet->HeadModel, in_CamSet.HeadModel);
out_CamSet->gblXPixels = in_CamSet.gblXPixels;
out_CamSet->gblYPixels = in_CamSet.gblYPixels;
out_CamSet->is_initialized = in_CamSet.is_initialized;
out_CamSet->AcquisitionMode = in_CamSet.AcquisitionMode;
out_CamSet->ShutterMode = in_CamSet.ShutterMode;
out_CamSet->MirrorMode = in_CamSet.MirrorMode;
out_CamSet->RotationMode = in_CamSet.RotationMode;
out_CamSet->CloseShutterWhenExit = in_CamSet.CloseShutterWhenExit;
out_CamSet->xc0 = in_CamSet.xc0;
out_CamSet->yc0 = in_CamSet.yc0;
out_CamSet->nx1 = in_CamSet.nx1;
out_CamSet->ny1 = in_CamSet.ny1;
out_CamSet->xbin = in_CamSet.xbin;
out_CamSet->ybin = in_CamSet.ybin;
out_CamSet->CoolerOn = in_CamSet.CoolerOn;
out_CamSet->CoolerSetTemp = in_CamSet.CoolerSetTemp;
out_CamSet->IntegTime_sec = in_CamSet.IntegTime_sec;
out_CamSet->ExpoTime_msec = in_CamSet.ExpoTime_msec;

return(0);
}
/*****************************************************************************
* Write information about the exposure in the descriptors
******************************************************************************/
int Write_ExposureParametersToFITSDescriptors(DESCRIPTORS *descrip,
                                              EXPOSURE_PARAM ExpoSet1)
{
int k;

// Date
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "OBS_DATE");
 sprintf((descrip->descr[k]).cvalue, "%s", ExpoSet1.date);
 (descrip->ndescr)++;

// Time
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "OBS_TIME");
 sprintf((descrip->descr[k]).cvalue, "Start (UT) = %.5f Exposure (sec) = %.2f",
         ExpoSet1.start_time, ExpoSet1.exposure_time);
 (descrip->ndescr)++;

return(0);
}
/*****************************************************************************
*
******************************************************************************/
int SaveCameraSettingsToFile(char *filename, JLP_CAMERA_SETTINGS in_CamSet)
{
// TBD
return(0);
}
/*****************************************************************************
*
******************************************************************************/
int LoadCameraSettingsFromFile(char *filename, JLP_CAMERA_SETTINGS* out_CamSet)
{
// TBD
return(0);
}
