/************************************************************************
* "jlp_raptor_utils.h"
*
* JLP
* Version 11/10/2017
*************************************************************************/
#ifndef jlp_raptor_utils_h_
#define jlp_raptor_utils_h_

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

// JLP2022:
#ifndef WIN32
#define WORD AWORD
#endif

#include "asp2_typedef.h" // DESCRIPTORS, EXPOSURE_PARAM, BYTE, etc

//***************************************************************************
// Acquisition parameters here to be set in common.c
typedef struct {
char HeadModel[128];      // Head Model
int  gblXPixels;     // Detector dimensions
int  gblYPixels;
int AutoExposure;  // 0=Off 1=On
int DigitalGain;   // 25 (0--48)
int ExpoTime_msec;  // 28 ms (0--1000)
int ExtTrigger;   // 0=Off, 1=On:(Ext +ve Edge), 2=On:(Ext -ve Edge)
int FrameRate;    // 0=25 Hz, 1=30 Hz, 2=50 Hz, 3=60 Hz, 4=90 Hz, 5=120 Hz
int HighGain;     // 0=Off, 1=On
int ImageSharpen; // 0=Off, 1=On
int NUC_State;
/* NUC state:
* 1: Offset+Gain corr.
* 2: Normal
* 3: Offset+Gain+Dark
* 4: 8 bit Offset - 32
* 5: 8 bit Dark - 2^19
* 6: 8 bit Gain - 128
* 7: Reversed map
* 8: Ramp Test Pattern
*/
int TriggerDelay;      // 0 (0--104)
int CoolerOn;          // 0=Off 1=On Flag set to one if Cooler is wanted
double CoolerSetTemp;  // Cooler temperature when Cooler is on
int is_initialized;    // Flag set to one when RAPTOR was initialized
/* AcuisitionMode:
* mode=1 Single Scan    (= long integration)
* mode=2 Accumulate     (= integration of short exposures)
* mode=3 Kinetics       (= sequence of images)
* mode=4 Fast Kinetics  (= fast transfer on a hidden area of the CCD)
* mode=5 Run till abort (= short exposures until "stop" button is clicked)
*/
int  AcquisitionMode; // 5 for continuous streaming
int  ShutterMode;     // Shutter: 0=auto 1=open 2=closed
int  RotationMode;    // 0 for 0, 1 for +90, 2 for +180, 3 for +270
int  MirrorMode;      // 0=none, 1=vertical, 2=horizontal 3=horiz+vert mirror
int  CloseShutterWhenExit;  // CloseShutterWhenExit (0/1)
int  FanIsOn;         // Fan is on (0/1)
//JLP2015
// int  NExposures;     // Number of exposures per sequence
// RaptorSet1.NExposures = RaptorSet1.KineticCycleNExposures * RaptorSet1.KineticNCycles;
int  xc0, yc0;        // Center of ROI
int  xbin, ybin;      // Binning factors
int  nx1, ny1;        // Size of the images (ROI size : nx1 * xbin,  ny1 * ybin
int  IntegTime_sec;   // Long exposure time in seconds
WORD *pImageArray;    // Image buffer to be used when reading from card
}
RAPTOR_SETTINGS;

// In "jlp_raptor_cam1_utils.cpp":
    void RaptorSettingsToDefault(RAPTOR_SETTINGS* out_RSet, int nx_full, int ny_full);
    int Copy_RaptorSettings(RAPTOR_SETTINGS in_RSet, RAPTOR_SETTINGS* out_RSet);
    int Copy_RaptorToCameraSettings(RAPTOR_SETTINGS in_RSet, JLP_CAMERA_SETTINGS* out_CamSet);
    int Copy_CameraToRaptorSettings(JLP_CAMERA_SETTINGS in_CamSet, RAPTOR_SETTINGS* out_RSet);
    int SaveRaptorSettingsToFile(char *filename, RAPTOR_SETTINGS RSet1);
    int LoadRaptorSettingsFromFile(char *filename, RAPTOR_SETTINGS *RSet1);
    int Write_RaptorSettingsToFITSDescriptors(DESCRIPTORS *descrip,
                                             RAPTOR_SETTINGS RSet1);
    int JLP_DecimalToHexadecimal(long int in_decimal,
                                 char *out_Hexadecimal, int out_length);

#endif // EOF sentry
