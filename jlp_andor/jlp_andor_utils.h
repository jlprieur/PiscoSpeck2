/************************************************************************
* "jlp_andor_utils.h"
*
* JLP
* Version 17/08/2015
*************************************************************************/
#ifndef jlp_andor_utils_h_
#define jlp_andor_utils_h_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/tglbtn.h"
#include "wx/utils.h"  // wxSleep()
#include "wx/imaglist.h"
#include "wx/cshelp.h"

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#include "asp2_typedef.h" // DESCRIPTORS, EXPOSURE_PARAM, BYTE, etc


#include "Atmcd32d.h"      // Andor functions
// #include "jlp_Atmcd32d_linux.h"      // Andor functions

//***************************************************************************
// Acquisition parameters here to be set in common.c
typedef struct {
AndorCapabilities AndorCaps; // AndorCapabilities structure
char HeadModel[128];      // Head Model
int  gblXPixels;     // Detector dimensions
int  gblYPixels;
int is_initialized;  // Flag set to one when ANDOR was initialized
int  AD_Channel;     // Analog-Digital converter channel
int  VShiftSpeed;    // Vertical shift speed
int  VShiftVoltage;  // Vertical shift voltage
int  HShiftPreAmpGain; // Horizontal shift pre-ampli gain
int  HShiftSpeed;    // Horizontal shift speed
/* AcuisitionMode:
* mode=1 Single Scan    (= long integration)
* mode=2 Accumulate     (= integration of short exposures)
* mode=3 Kinetics       (= sequence of images)
* mode=4 Fast Kinetics  (= fast transfer on a hidden area of the CCD)
* mode=5 Run till abort (= short exposures until "stop" button is clicked)
*/
int  AcquisitionMode; // 5 for continuous streaming
int  readMode;       // 4 for imaging mode
int  TriggerMode;    // 0 for internal, 10 for software
int  ShutterMode;    // Shutter: 0=auto 1=open 2=closed
int  RotationMode;   // 0 for 0, 1 for +90, 2 for +180, 3 for +270
int  MirrorMode;     // 0=none, 1=vertical, 2=horizontal 3=horiz+vert mirror
int  EMGainValue;    // Value of EMCCD gain
int  EMGainLowValue; // Smallest value of EMCCD gain
int  EMGainHighValue; // Largest value of EMCCD gain
int  EMGainMode;      // EMCCD gain mode (0=[0,255],1=[0,4096],2=linear)
int  EMGainBasicMode;  // EMCCD gain allowed mode (0=[0,255] or 1=[0,4096])
int  FrameTransferMode;  // Frame transfer Mode (0/1)
int  BaselineClamp;  // Baseline Clamp (0/1)
int  CloseShutterWhenExit;  // CloseShutterWhenExit (0/1)
//JLP2015
// int  NExposures;     // Number of exposures per sequence
// AndorSet1.NExposures = AndorSet1.KineticCycleNExposures * AndorSet1.KineticNCycles;
int  xc0, yc0;       // Center of ROI
int  xbin, ybin;     // Binning factors
int  nx1, ny1;       // Size of the images (siwe of the ROI: nx0 = nx1 * xbin,  ny0 = ny1 * ybin
int  IntegTime_sec;  // Long exposure time in seconds
int  KineticExpoTime_msec;  // Kinetic: exposure time in milliseconds
int  KineticClockTime_msec; // Kinetic: clock time in milliseconds
int  KineticCycleTime_msec; // Kinetic: cycle time in milliseconds
int  KineticCycleNExposures; // Kinetic: number of exposures/cycle (for accumulation)
int  KineticNCycles;  // Kinetic: number of cycles (for accumulation)
int  CoolerOn;        // 0=Off 1=On Flag set to one if Cooler is wanted
double  CoolerSetTemp;   // Cooler temperature when Cooler is on
long ImageSize;       // Size of pImageArray
WORD *pImageArray;    // Main image buffer read from card
}
ANDOR_SETTINGS;

// In "jlp_andor_cam1_utils.cpp":
    void AndorSettingsToDefault(ANDOR_SETTINGS* out_ASet, int nx_full, int ny_full);
    int Copy_AndorSettings(ANDOR_SETTINGS in_ASet, ANDOR_SETTINGS* out_ASet);
    int Copy_AndorToCameraSettings(ANDOR_SETTINGS in_ASet, JLP_CAMERA_SETTINGS* out_CamSet);
    int Copy_CameraToAndorSettings(JLP_CAMERA_SETTINGS in_CamSet, ANDOR_SETTINGS* out__ASet);
    int SaveAndorSettingsToFile(char *filename, ANDOR_SETTINGS Aset1);
    int LoadAndorSettingsFromFile(char *filename, ANDOR_SETTINGS *ASet1);
    int Write_AndorSettingsToFITSDescriptors(DESCRIPTORS *descrip,
                                         ANDOR_SETTINGS Aset1);


#endif // EOF sentry
