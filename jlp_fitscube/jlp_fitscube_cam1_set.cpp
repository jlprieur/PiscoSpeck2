/************************************************************************
* "jlp_fitscube_cam1_set.cpp"
* JLP_FitsCubeCam1 class
*
* JLP
* Version 17/11/2017
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_fitscube_cam1.h"    // FITSCUBE_SETTINGS, ...
#include "jlp_fitscube_utils.h"   // Copy_FitsCubeSettings, ...

/****************************************************************************
*
****************************************************************************/
int JLP_FitsCubeCam1::Cam1_SaveCameraSettingsToFile(char *filename)
{
 int status;
 status = SaveFitsCubeSettingsToFile(filename, FitsCubeSet1);
 return(status);
}
/****************************************************************************
*
****************************************************************************/
int JLP_FitsCubeCam1::Cam1_LoadCameraSettingsFromFile(char *filename)
{
 int status;
 status = LoadFitsCubeSettingsFromFile(filename, &FitsCubeSet1);
 return(status);
}
/****************************************************************************
*
****************************************************************************/
int JLP_FitsCubeCam1::Cam1_Write_CameraSettingsToFITSDescriptors(DESCRIPTORS *descrip)
{
 int status;
 status = Write_FitsCubeSettingsToFITSDescriptors(descrip, FitsCubeSet1);
 return(status);
}
/***************************************************************************
* Set default values for JLP_FitsCube_Settings
*
* WARNING: SHOULD NOT CHANGE gblXPixels, ... or FitCubeSet1.nz1
* (since Image1 was allocated with previous settings) !
*
* Size of subwindow, center of subwindow, bin factor, size of elementary arrays
*
****************************************************************************/
int JLP_FitsCubeCam1::Cam1_SettingsToDefault(int nx_full, int ny_full)
{
FitsCubeSet1.gblXPixels = nx_full;
FitsCubeSet1.gblYPixels = ny_full;

// Rotation mode: 0 for 0, 1 for +90, 2 for +180, 3 for +270
FitsCubeSet1.RotationMode = 0;
// MirrorMode: 0=none, 1=vertical, 2=horizontal 3=vertical+horizontal
FitsCubeSet1.MirrorMode = 0;
// Center of ROI subwindow
FitsCubeSet1.xc0 = FitsCubeSet1.gblYPixels/ 2;
FitsCubeSet1.yc0 = FitsCubeSet1.gblYPixels/ 2;
// Binning factors
FitsCubeSet1.xbin = 1;
FitsCubeSet1.ybin = 1;
// Size of elementary frames
FitsCubeSet1.nx1 = 128;
FitsCubeSet1.ny1 = 128;

return(0);
}
/****************************************************************************
*
****************************************************************************/
int JLP_FitsCubeCam1::Cam1_SetCoolerTemp(wxString &error_msg)
{
error_msg = wxT("Cooling not available for FitsCube!");
return(-1);
}
