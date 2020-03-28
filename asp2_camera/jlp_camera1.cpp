/************************************************************************
* "jlp_camera1.cpp"
* JLP_Camera1 class
*
* JLP
* Version 05/10/2017
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_camera1.h"      // JLP_CAMERA_SETTINGS, ...
#include "jlp_camera_utils.h"     // Copy_CameraSettings, ...

//---------------------------------------------------------------------------

// #define DEBUG

/***************************************************************************
* Start dialog with Camera camera
****************************************************************************/
int JLP_Camera1::JLPCam1_InitPrivateParameters(JLP_CAMERA_SETTINGS CamSet0)
{
int status;
wxString error_msg;

initialized = 0;

// Load Camera settings CamSet0
Cam1_Load_CameraSettings(CamSet0);

status = Cam1_ConnectToCamera();

// Set initialized to true
if(status == 0) {
  initialized = 1234;
  } else {
  exit(-1);
  }

return(status);
}
/*******************************************************************************
* FUNCTION NAME: JLPCam1_ShutDown()
*
********************************************************************************/
int JLP_Camera1::JLPCam1_ShutDown()
{
int status;
 status = Cam1_ShutDown();

return(status);
}

