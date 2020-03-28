/************************************************************************
* "jlp_raptor_cam1.cpp"
* JLP_FitsCubeCam1 class
*
* JLP
* Version 17/09/2015
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_fitscube_cam1.h"
#include "asp2_3dfits_rd_utils.h" // open_input_fits_cube1()

#ifndef MAXI
#define MAXI(a,b) ((a) > (b)) ? (a) : (b)
#endif // MAXI
#ifndef MINI
#define MINI(a,b) ((a) < (b)) ? (a) : (b)
#endif // MINI


/***************************************************************************
* Start dialog with FitsCube camera
****************************************************************************/
JLP_FitsCubeCam1::JLP_FitsCubeCam1(char *filename0)
{
int status;
wxString error_msg;

not_connected = true;
inFitsFile1 = NULL;
ImageCube1 = NULL;
FitsCubeSet1.is_initialized = 0;

strcpy(FitsCubeSet1.Filename1, filename0);
status = Cam1_ConnectToCamera();

if(status == 0) {
  not_connected = false;
  JLP_FitsCube_GetCapabilities();
  }

FitsCubeSettingsToDefault(&FitsCubeSet1, FitsCubeSet1.gblXPixels,
                          FitsCubeSet1.gblYPixels);
// Allocate memory and set FitsCubeSet1.is_initialized to one
  JLP_FitsCube_FirstInit();

return;
}
/***************************************************************************
* Release the FitsCube camera
****************************************************************************/
JLP_FitsCubeCam1::~JLP_FitsCubeCam1()
{
 Cam1_ShutDown();
}
/*************************************************************************
* Open connection to FitsCube camera for the first time (if present)
*
**************************************************************************/
int JLP_FitsCubeCam1::Cam1_ConnectToCamera()
{
int status = -1;
 inFitsFile1 = new JLP_3D_inFITSFile(FitsCubeSet1.Filename1);

  // If error, display an error message:
 if(!inFitsFile1->IsOpen()) {
   wxMessageBox(wxT("Error opening FITS cube"), wxT("JLP_3DFitsThread/Setup"),
                wxOK | wxICON_ERROR);
   delete inFitsFile1;
   inFitsFile1 = NULL;
 } else {
   FitsCubeSet1.gblXPixels = inFitsFile1->Nx();
   FitsCubeSet1.gblYPixels = inFitsFile1->Ny();
   FitsCubeSet1.nz_total = inFitsFile1->Nz_Total();
   status = 0;
 }


return(status);
}
/*************************************************************************
* Get the capabilities of the connected FitsCube camera
**************************************************************************/
int JLP_FitsCubeCam1::JLP_FitsCube_GetCapabilities()
{

return(0);
}
/*************************************************************************
* Initialize FitsCube camera for the first time (called
* when creating FitsCubeSetupBox)
* Selection of the settings included in FitsCubeSet
*
* Set FitsCubeSet1.is_initialized to 1
*
**************************************************************************/
int JLP_FitsCubeCam1::JLP_FitsCube_FirstInit()
{
 if(not_connected) {
   return(-1);
 }

 FitsCubeSet1.nz1 = 50;
 ImageCube1 = new AWORD[FitsCubeSet1.gblXPixels * FitsCubeSet1.gblYPixels
                        * FitsCubeSet1.nz1];

 FitsCubeSet1.is_initialized = 1;
// Apply all the current settings:
 JLP_FitsCube_ApplyBasicSettings();
 JLP_FitsCube_ApplySettingsForExposures();

return(0);
}
/****************************************************************************
* Shutdown FitsCube camera
* and take care of cooling system (slow re-heating...)
*
*****************************************************************************/
int JLP_FitsCubeCam1::Cam1_ShutDown()
{
if(FitsCubeSet1.is_initialized == 0) return(-1);

if(inFitsFile1 != NULL) delete inFitsFile1;
inFitsFile1 = NULL;

if(ImageCube1 != NULL) delete[] ImageCube1;
ImageCube1 = NULL;

FitsCubeSet1.is_initialized = 0;

return(0);
}
/************************************************************************
* Display current FitsCube settings in a popup window
* for all image acquisition modes
************************************************************************/
int  JLP_FitsCubeCam1::Cam1_DisplaySettings()
{

return(0);
}
/***************************************************************************
* Get FITSCUBE_SETTINGS structure
****************************************************************************/
int JLP_FitsCubeCam1::Get_FitsCubeSettings(FITSCUBE_SETTINGS* FitsCubeSet0)
{
int status;
// Copy from FitsCubeSet1 to FitsCubeSet0
status = Copy_FitsCubeSettings(FitsCubeSet1, FitsCubeSet0);
return(status);
}
/***************************************************************************
* Load FITSCUBE_SETTINGS structure
****************************************************************************/
int JLP_FitsCubeCam1::Load_FitsCubeSettings(FITSCUBE_SETTINGS FitsCubeSet0)
{
int status;
// Copy from FitsCubeSet0 to FitsCubeSet1
status = Copy_FitsCubeSettings(FitsCubeSet0, &FitsCubeSet1);
return(status);
}
/***************************************************************************
* Get JLP_CAMERA_SETTINGS structure
****************************************************************************/
int JLP_FitsCubeCam1::Cam1_Get_CameraSettings(JLP_CAMERA_SETTINGS* CamSet0)
{
int status;
// Copy FitsCubeSet1 to CamSet0
status = Copy_FitsCubeToCameraSettings(FitsCubeSet1, CamSet0);
return(status);
}
/***************************************************************************
* Get JLP_CAMERA_SETTINGS structure
****************************************************************************/
int JLP_FitsCubeCam1::Cam1_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0)
{
int status;
// Copy CamSet0 to FitsCubeSet1
status = Copy_CameraToFitsCubeSettings(CamSet0, &FitsCubeSet1);
return(status);
}
