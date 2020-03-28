/************************************************************************
* "jlp_raptor_cam1_acqui.cpp"
* JLP_FitsCubeCam1 class : single acquisition and setup for acquisition
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

//------------------------------------------------------------------------
// Setting ANDOR camera to the configuration stored in FitsCubeSet
// ForExposures:
// EMCCD gain, exposure time, kinetic cycle
//------------------------------------------------------------------------
int JLP_FitsCubeCam1::JLP_FitsCube_ApplySettingsForExposures()
{

return(0);
}
//------------------------------------------------------------------------
// Basic settings of the ANDOR camera according
// to the configuration stored in FitsCubeSet1
//
// AD channel, preampli gain
// vertical, horizontal shift settings
// Shutter and trigger
//------------------------------------------------------------------------
int JLP_FitsCubeCam1::JLP_FitsCube_ApplyBasicSettings()
{

return(0);
}
/*************************************************************************
* Initialize camera for Single or Continuous Acquisition
* when acquisitionMode has been changed.
* Use the settings included in FitsCubeSet
*
* INPUT:
*  FitsCubeSet1.acquisitionMode
**************************************************************************/
int JLP_FitsCubeCam1::Cam1_InitAcquisition(wxString& error_msg)
{
error_msg = wxT("");

if(not_connected) return(-1);

  if(FitsCubeSet1.is_initialized == 0) {
    error_msg.Append(wxT("JLP_FitsCubeCam1::Cam1_InitAcquisition/Error: FitsCubeSet not initialized!\n"));
    return(-1);
    }

// Wait for 0.1 second to allow MCD to calibrate fully before allowing an
// acquisition to begin
  wxMilliSleep(100);

return(0);
}
/*******************************************************************************
*
*  FUNCTION NAME:  ANDOR_SetSystemForSingleAcquisition()
*
*  DESCRIPTION:  This function sets up the acquisition settings exposure time
*		  shutter, trigger and starts an acquisition. It also starts a
*		  timer to check when the acquisition has finished.
********************************************************************************/
int JLP_FitsCubeCam1::Cam1_SetSystemForSingleAcquisition(int nx0, int ny0, int xc0,
                                                         int yc0, int xbin0, int ybin0)
{
int  status = 0;

if(not_connected) return(-1);

 FitsCubeSet1.xc0 = xc0;
 FitsCubeSet1.yc0 = yc0;
 FitsCubeSet1.xbin = xbin0;
 FitsCubeSet1.ybin = ybin0;
 JLP_FitsCube_ApplySettingsForExposures();

return(status);
}
