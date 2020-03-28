/************************************************************************
* "jlp_raptor_serial.h"
* JLP_RaptorCam1 class from virtual class JLP_Camera&
*
* JLP
* Version 11/10/2017
*************************************************************************/
#ifndef jlp_raptor_serial_h_
#define jlp_raptor_serial_h_

#if 0
// #include "RaptorEPIX.h"
#include <cstdio>
#include <string>
#include <math.h>
/*
#include "../../MMDevice/ModuleInterface.h"
*/
#include <sstream>
#include <algorithm>

#include <iostream>
#include <sys/stat.h>
#if defined (MMLINUX32) || defined(MMLINUX64)
	#include <sys/time.h>
	#include <stdio.h>
	#include <unistd.h>
	#define sprintf_s snprintf
	void Sleep(double x) {usleep(1000.0*x);};
#endif

const char* g_strVersion = "v1.13.12, 7/8/2017";


#define _RAPTOR_CAMERA_KITE 1
#define _RAPTOR_CAMERA_OWL  2
#define _RAPTOR_CAMERA_FALCON 4
#define _RAPTOR_CAMERA_COMMONCMDS1 8
#define _RAPTOR_CAMERA_RGB 16
#define _RAPTOR_CAMERA_COMMONCMDS1_RGB (_RAPTOR_CAMERA_COMMONCMDS1 + _RAPTOR_CAMERA_RGB)
#define _RAPTOR_CAMERA_OWL_320  (_RAPTOR_CAMERA_OWL + 1024)
#define _RAPTOR_CAMERA_OWL_640  (_RAPTOR_CAMERA_OWL + 2048)
#define _RAPTOR_CAMERA_OWL_NINOX_640  (_RAPTOR_CAMERA_OWL + 4096)


#define _RAPTOR_CAMERA_OSPREY				( 32 + _RAPTOR_CAMERA_COMMONCMDS1)
#define _RAPTOR_CAMERA_KINGFISHER_674		( 64 + _RAPTOR_CAMERA_COMMONCMDS1)
#define _RAPTOR_CAMERA_KINGFISHER_694		(128 + _RAPTOR_CAMERA_COMMONCMDS1)
#define _RAPTOR_CAMERA_CYGNET				(256 + _RAPTOR_CAMERA_COMMONCMDS1)
#define _RAPTOR_CAMERA_UNKNOWN1				(512 + _RAPTOR_CAMERA_COMMONCMDS1)
#define _RAPTOR_CAMERA_EAGLE				(8192 + _RAPTOR_CAMERA_COMMONCMDS1)
#define _RAPTOR_CAMERA_EAGLE_V				(8192*3 + _RAPTOR_CAMERA_COMMONCMDS1)
#define _RAPTOR_CAMERA_EAGLE_V_4240			(8192*5 + _RAPTOR_CAMERA_COMMONCMDS1)
#define _RAPTOR_CAMERA_EAGLE_V_4210			(8192*9 + _RAPTOR_CAMERA_COMMONCMDS1)

#define _RAPTOR_CAMERA_OSPREY_RGB			(_RAPTOR_CAMERA_OSPREY			+ _RAPTOR_CAMERA_RGB)
#define _RAPTOR_CAMERA_KINGFISHER_674_RGB	(_RAPTOR_CAMERA_KINGFISHER_674	+ _RAPTOR_CAMERA_RGB)
#define _RAPTOR_CAMERA_KINGFISHER_694_RGB	(_RAPTOR_CAMERA_KINGFISHER_694	+ _RAPTOR_CAMERA_RGB)
#define _RAPTOR_CAMERA_CYGNET_RGB			(_RAPTOR_CAMERA_CYGNET			+ _RAPTOR_CAMERA_RGB)
#define _RAPTOR_CAMERA_UNKNOWN1_RGB			(_RAPTOR_CAMERA_UNKNOWN1		+ _RAPTOR_CAMERA_RGB)

#define _RAPTOR_CAMERA_OSPREY_BASE			32
#define _RAPTOR_CAMERA_KINGFISHER_BASE		(64 + 128)
#define _RAPTOR_CAMERA_CYGNET_BASE			256
#define _RAPTOR_CAMERA_UNKNOWN_BASE			512
#define _RAPTOR_CAMERA_EAGLE_BASE			8192


#define _IS_CAMERA_OWL_FAMILY ((cameraType_ & _RAPTOR_CAMERA_OWL)>0)
#define _NOT_CAMERA_OWL_FAMILY ((cameraType_ & _RAPTOR_CAMERA_OWL)==0)

#define LIVEPAIRTEST 0
#undef DOLIVEPAIR

using namespace std;
// const double CRaptorEPIX::nominalPixelSizeUm_ = 1.0;
// double g_IntensityFactor_ = 1.0;
bool bEagle4210 = false;

// External names used used by the rest of the system
// to load particular device from the "RaptorEPIX.dll" library

// Falcon                 =>           EFIS
// Kite                   =>           ATOR
// Osprey                 =>           IRIS

#ifdef HORIBA_COMPILE
#define RAPTOR "Horiba"
#define FALCON "EFIS"
#define KITE   "ATOR"
#define OSPREY "IRIS"
#define OWL    "Undefined"
#define KINGFISHER "Undefined"
#define CYGNET "Undefined"
#define NINOX "Undefined"
#define EAGLE "Undefined"
#else
#define RAPTOR "Raptor"
#define FALCON "Falcon"
#define KITE   "Kite"
#define OSPREY "Osprey"
#define OWL "Owl"
#define KINGFISHER "Kingfisher"
#define CYGNET "Cygnet"
#define NINOX "Ninox"
#define EAGLE "Eagle"
#endif
#define SPC " "
#define CAM "Camera"

const char* g_RaptorCameraKITEDeviceName			= RAPTOR SPC KITE SPC CAM;
const char* g_RaptorCameraOwl320DeviceName			= RAPTOR SPC OWL SPC CAM " 320";
const char* g_RaptorCameraOwl640DeviceName			= RAPTOR SPC OWL SPC CAM " 640";
const char* g_RaptorCameraOwlNinox640DeviceName		= RAPTOR SPC NINOX SPC CAM " 640";
const char* g_RaptorCameraFalconDeviceName			= RAPTOR SPC FALCON SPC CAM;
const char* g_RaptorCameraEagleDeviceName			= RAPTOR SPC EAGLE SPC CAM;
const char* g_RaptorCameraEagleV4240DeviceName		= RAPTOR SPC EAGLE SPC "V 4240" SPC CAM;
const char* g_RaptorCameraEagleV4210DeviceName		= RAPTOR SPC EAGLE SPC "V 4210" SPC CAM;
const char* g_RaptorCameraOspreyDeviceName			= RAPTOR SPC OSPREY SPC CAM;
const char* g_RaptorCameraOspreyRGBDeviceName		= RAPTOR SPC OSPREY " RGB " CAM;
const char* g_RaptorCameraKingfisher674DeviceName	= RAPTOR SPC KINGFISHER " 674";
const char* g_RaptorCameraKingfisher694DeviceName	= RAPTOR SPC KINGFISHER " 694";
const char* g_RaptorCameraKingfisher674RGBDeviceName= RAPTOR SPC KINGFISHER " 674 RGB";
const char* g_RaptorCameraKingfisher694RGBDeviceName= RAPTOR SPC KINGFISHER " 694 RGB";
const char* g_RaptorCameraCygnetDeviceName			= RAPTOR SPC CYGNET SPC CAM;
const char* g_RaptorCameraCygnetRGBDeviceName		= RAPTOR SPC CYGNET " RGB " CAM;
const char* g_RaptorCameraUnknown1DeviceName		= RAPTOR SPC "XCAP Export";
const char* g_RaptorCameraUnknown1RGBDeviceName		= RAPTOR SPC "XCAP RGB Export";

const char* g_RaptorKITE = KITE;
const char* g_RaptorOwl320 = OWL " 320";
const char* g_RaptorOwl640 = OWL " 640";
const char* g_RaptorNinox640 = NINOX " 640";
const char* g_RaptorEagle = EAGLE;
const char* g_RaptorEagleV4240 = EAGLE " V 4240";
const char* g_RaptorEagleV4210 = EAGLE " V 4210";
const char* g_RaptorFalcon = FALCON;
const char* g_RaptorOsprey = OSPREY;
const char* g_RaptorOspreyRGB = OSPREY " RGB";
const char* g_RaptorKingfisher674 = KINGFISHER " 674";
const char* g_RaptorKingfisher694 = KINGFISHER " 694";
const char* g_RaptorKingfisher674RGB = KINGFISHER " 674 RGB";
const char* g_RaptorKingfisher694RGB = KINGFISHER " 694 RGB";
const char* g_RaptorCygnet		= CYGNET;
const char* g_RaptorCygnetRGB   = CYGNET " RGB";
const char* g_RaptorUnknown1    = "XCAP Export";
const char* g_RaptorUnknown1RGB = "XCAP Export RGB";

/*
	const char* g_RaptorCameraKITEDeviceName			= "Raptor KITE Camera";
	const char* g_RaptorCameraOWLDeviceName				= "Raptor OWL Camera";
	const char* g_RaptorCameraFalconDeviceName			= "Raptor Falcon Camera";
	const char* g_RaptorCameraOspreyDeviceName			= "Raptor Osprey Camera";
	const char* g_RaptorCameraOspreyRGBDeviceName		= "Raptor Osprey RGB Camera";
	const char* g_RaptorCameraKingfisher674DeviceName	= "Raptor Kingfisher 674";
	const char* g_RaptorCameraKingfisher694DeviceName	= "Raptor Kingfisher 694";
	const char* g_RaptorCameraKingfisher674RGBDeviceName= "Raptor Kingfisher 674 RGB";
	const char* g_RaptorCameraKingfisher694RGBDeviceName= "Raptor Kingfisher 694 RGB";
	const char* g_RaptorCameraCygnetDeviceName			= "Raptor Cygnet Camera";
	const char* g_RaptorCameraCygnetRGBDeviceName		= "Raptor Cygnet RGB Camera";
	const char* g_RaptorCameraUnknown1DeviceName		= "Raptor XCAP Export";
	const char* g_RaptorCameraUnknown1RGBDeviceName		= "Raptor XCAP RGB Export";

	const char* g_RaptorKITE = "KITE";
	const char* g_RaptorOWL  = "OWL";
	const char* g_RaptorFalcon = "Falcon";
	const char* g_RaptorOsprey = "Osprey";
	const char* g_RaptorOspreyRGB = "Osprey RGB";
	const char* g_RaptorKingfisher674 = "Kingfisher 674";
	const char* g_RaptorKingfisher694 = "Kingfisher 694";
	const char* g_RaptorKingfisher674RGB = "Kingfisher 674 RGB";
	const char* g_RaptorKingfisher694RGB = "Kingfisher 694 RGB";
	const char* g_RaptorCygnet		= "Cygnet";
	const char* g_RaptorCygnetRGB   = "Cygnet RGB";
	const char* g_RaptorUnknown1    = "XCAP Export";
	const char* g_RaptorUnknown1RGB = "XCAP Export RGB";
*/

const char* g_Keyword_ExposureMax = "Exposure Max";
const char* g_Keyword_PCBTemp     = "Temp PCB (oC)";
const char* g_Keyword_CCDTemp     = "Temp CCD (oC)";
const char* g_Keyword_SensorTemp     = "Temp Sensor (oC)";
const char* g_Keyword_MicroReset  = "Micro Reset";
const char* g_Keyword_TestPattern = "Test Pattern";
const char* g_Keyword_TECooler    = "TE Cooler";
const char* g_Keyword_TECFan      = "TEC Fan";
const char* g_Keyword_TECooler_neg5oC = "Set Point -5oC";
const char* g_Keyword_TECooler_neg20oC = "Set Point -20oC";
const char* g_Keyword_TECooler_Reset = "Temp Trip Reset";

const char* g_Keyword_AntiBloom   = "Anti-Bloom";

const char* g_Keyword_ROI_AOI_Left    = "ROI X";
const char* g_Keyword_ROI_AOI_Top     = "ROI Y";
const char* g_Keyword_ROI_AOI_Width   = "ROI X Size";
const char* g_Keyword_ROI_AOI_Height  = "ROI Y Size";
const char* g_Keyword_AGC_AOI_Left    = "ROI (AGC) X";
const char* g_Keyword_AGC_AOI_Top     = "ROI (AGC) Y";
const char* g_Keyword_AGC_AOI_Width   = "ROI (AGC) X Size";
const char* g_Keyword_AGC_AOI_Height  = "ROI (AGC) Y Size";

const char* g_Keyword_AOI_Left    = g_Keyword_ROI_AOI_Left;
const char* g_Keyword_AOI_Top     = g_Keyword_ROI_AOI_Top;
const char* g_Keyword_AOI_Width   = g_Keyword_ROI_AOI_Width;
const char* g_Keyword_AOI_Height  = g_Keyword_ROI_AOI_Height;

const char* g_Keyword_UseAOI      = "ROI Use";
const char* g_Keyword_TECSetPoint = "TEC Set Point (oC)";
const char* g_Keyword_NUCState    = "Xpert: NUC State";
const char* g_Keyword_NUCState0    = "0: Offset Corrected";
const char* g_Keyword_NUCState1    = "1: Offset+Gain Corr";
const char* g_Keyword_NUCState2    = "2: Normal";
const char* g_Keyword_NUCState3    = "3: Offset+Gain+Dark";
const char* g_Keyword_NUCState4    = "4: 8bit Offset /32";
const char* g_Keyword_NUCState5    = "5: 8bit Dark *2^19";
const char* g_Keyword_NUCState6    = "6: 8bit Gain /128";
const char* g_Keyword_NUCState7a   = "7: Off+Gain+Dark+BAD";
const char* g_Keyword_NUCState7b   = "7: Reserved Map";
const char* g_Keyword_NUCState8    = "8: Ramp Test Pattern";
const char* g_Keyword_PeakAvgLevel = "Xpert: AE Peak/Avg Level";
const char* g_Keyword_AGCSpeed     = "Xpert: AE Gain Speed";
const char* g_Keyword_ExpSpeed     = "Xpert: AE Exp Speed";
const char* g_Keyword_AutoExpLevel = "Xpert: AE Set Level";
const char* g_Keyword_UseSerialLog = "Xpert: Use Serial Log";
const char* g_Keyword_EPIXUnit     = "EPIX_Unit";
const char* g_Keyword_EPIXUnit2    = "EPIX Unit";
const char* g_Keyword_EPIXMultiUnitMask    = "EPIX Units to Open";

const char* g_Keyword_HighGain      = "High Gain";
const char* g_Keyword_ROIAppearance = "ROI Appearance";
const char* g_Keyword_AutoExposure = "Exposure: Auto";
const char* g_Keyword_TrigDelay   = "Ext. Trig. Delay (ms)";
const char* g_Keyword_TrigITR   = "Live: Integrate Then Read";
const char* g_Keyword_TrigFFR   = "Live: Fixed Frame Rate (FFR)";
const char* g_Keyword_ExtTrigger  = "Ext. Trigger";
const char* g_Keyword_ExtTrigger_posTrig = "On: (Ext. +ve Edge)";
const char* g_Keyword_ExtTrigger_negTrig = "On: (Ext. -ve Edge)";
const char* g_Keyword_ExtTrigger_Abort = "Abort Exposure";
const char* g_Keyword_CaptureMode = "Capture Mode";
const char* g_Keyword_CaptureMode_SnapShot = "Snapshot";
const char* g_Keyword_CaptureMode_Live = "Live";
const char* g_Keyword_FixedFrameRate = "Frame Rate (FPS)";
const char* g_Keyword_FrameRate = "Frame Rate";
const char* g_Keyword_FrameRate_25Hz = "25 Hz";
const char* g_Keyword_FrameRate_30Hz = "30 Hz";
const char* g_Keyword_FrameRate_50Hz = "50 Hz";
const char* g_Keyword_FrameRate_60Hz = "60 Hz";
const char* g_Keyword_FrameRate_90Hz = "90 Hz";
const char* g_Keyword_FrameRate_120Hz = "120 Hz";
const char* g_Keyword_FrameRate_User = "User Defined";
const char* g_Keyword_FrameRate_ExtTrig = "Ext. Trig.";
const char* g_Keyword_ROI_Normal = "Normal";
const char* g_Keyword_ROI_Bright = "Highlight";
const char* g_Keyword_ROI_Dark   = "Border";
const char* g_Keyword_VideoPeak   = "Video Peak";
const char* g_Keyword_VideoAvg    = "Video Avg";
const char* g_Keyword_BuildInfo   = "Build Info";
const char* g_Keyword_FrameRateUser = "Frame Rate (User)";
const char* g_Keyword_BlackOffset = "Black Offset";
const char* g_Keyword_ForceUpdate = "Force Update";
const char* g_Keyword_On	      = "On";
const char* g_Keyword_Off	      = "Off";
const char* g_Keyword_PostCaptureROI = "Xpert: Post Snap Crop";
const char* g_Keyword_Defaults = "Use Defaults";
const char* g_Keyword_UseDefaults = "Use Defaults";
const char* g_Keyword_FrameAccumulate = "Frame Average";
const char* g_Keyword_TriggerTimeout = "Trig. Timeout (sec)";
const char* g_Keyword_FrameInterval = "Frame Interval (ms)";
const char* g_Keyword_HDR = "High Dynamic Range";
const char* g_Keyword_NUCMap = "View NUC Map";
const char* g_Keyword_DebayerMethod = "Color Debayer";
const char* g_Keyword_Debayer_Bilinear = "On";
const char* g_Keyword_Debayer_Nearest = "Nearest";
const char* g_Keyword_Debayer_None = "Off";

const char* g_Keyword_HorizontalFlip = "Horizontal Flip";
const char* g_Keyword_InvertVideo = "Invert Video";
const char* g_Keyword_BadPixel = "Show Bad Pixels";
const char* g_Keyword_ImageSharpen = "Image Sharpen";

const char* g_Keyword_ReadoutRate = "Readout Rate (MHz)";
const char* g_Keyword_ReadoutRate2 = "Readout Rate (kHz)";
const char* g_Keyword_ReadoutMode = "Readout Mode";
const char* g_Keyword_ReadoutMode_Baseline = "Baseline Clamped";
const char* g_Keyword_ReadoutMode_CDS	   = "CDS (default)";
const char* g_Keyword_ReadoutMode_TestPattern = "Test Pattern";
const char* g_Keyword_ReadoutMode_Normal = "Normal";

const char* g_Keyword_ShutterMode		   = "Shutter Mode";
const char* g_Keyword_ShutterMode_Closed   = "Closed";
const char* g_Keyword_ShutterMode_Open     = "Open";
const char* g_Keyword_ShutterMode_Exposure = "Exposure";

const char* g_Keyword_ShutterDelayOpen  = "Shutter Delay Open";
const char* g_Keyword_ShutterDelayClose = "Shutter Delay Close";

const char* g_Keyword_HighPreAmpGain	   = "High Pre-Amp Gain";
const char* g_Keyword_BinningX			= "Binning X" ;
const char* g_Keyword_BinningY			= "Binning Y" ;

// constants for naming pixel types (allowed values of the "PixelType" property)
const char* g_PixelType_8bit = "8 bit";
const char* g_PixelType_16bit = "16 bit";
const char* g_PixelType_24bitRGB = "24 bitRGB";
const char* g_PixelType_32bitRGB = "32 bitRGB";
const char* g_PixelType_64bitRGB = "64 bitRGB";
const char* g_PixelType_32bit = "32 bit";  // floating point greyscale

/*
MMThreadLock g_serialLock_[64];
FILE *fidSerial[64];
long gUseSerialLog[64];
double gClockZero[64];
double gClockCurrent[64];
void mySleep(double nTime_ms);
double myClock();
int g_PIXCI_DriverLoaded = 0;
unsigned char g_ucSerialBuf[256];
int g_SerialOK=false;
bool g_bCheckSum = true;
*/

int serialWriteReadCmd(int unitopenmap, int unit, unsigned char* bufin, int insize,
                       unsigned char* bufout, int outsize, bool bChkSum=true ) ;

//*********** Start EPIX Setup *****************
/*
 *  Set number of expected PIXCI(R) image boards, from 1 to 4.
 *  The XCLIB Simple 'C' Functions expect that the boards are
 *  identical and operated at the same resolution.
 *
 *  For PIXCI(R) imaging boards with multiple, functional units,
 *  the XCLIB presents the two halves of the
 *  PIXCI\*(Rg\ E1DB, E4DB, ECB2, EL1DB, ELS2, SI2, or SV7 imaging boards
 *  or the four quarters of the PIXCI\*(Rg\ SI4 imaging board
 *  as two or four independent PIXCI\*(Rg\ imaging boards, respectively.
 *
 */
extern "C" {
#include "epix_xcliball.h"
}
#endif
#endif // EOF sentry
