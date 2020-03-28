/************************************************************************
* "jlp_fitscube_utils.h"
*
* JLP
* Version 11/10/2017
*************************************************************************/
#ifndef jlp_fitscube_utils_h_
#define jlp_fitscube_utils_h_

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

//***************************************************************************
// Acquisition parameters here to be set in common.c
typedef struct {
char Filename1[128]; // filename
int  gblXPixels;     // Detector dimensions
int  gblYPixels;
int is_initialized; // Flag set to one when Fitscube was loaded
int  RotationMode;   // 0 for 0, 1 for +90, 2 for +180, 3 for +270
int  MirrorMode;     // 0=none, 1=vertical, 2=horizontal 3=horiz+vert mirror
int  xc0, yc0;       // Center of ROI
int  xbin, ybin;     // Binning factors
int  nx1, ny1;       // Size of the images (ROI size : nx1 * xbin,  ny1 * ybin
int  nz1;            // Number of planes of the elementary cubes
int  nz_total;       // Number of planes of the whole data FITS cube
AWORD *ImageCube1;    // Array pointer of temporary elementary data cube
}
FITSCUBE_SETTINGS;

// In "jlp_fitscube_cam1_utils.cpp":
    void FitsCubeSettingsToDefault(FITSCUBE_SETTINGS* out_RSet, int nx_full, int ny_full);
    int Copy_FitsCubeSettings(FITSCUBE_SETTINGS in_RSet, FITSCUBE_SETTINGS* out_RSet);
    int Copy_FitsCubeToCameraSettings(FITSCUBE_SETTINGS in_RSet, JLP_CAMERA_SETTINGS* out_CamSet);
    int Copy_CameraToFitsCubeSettings(JLP_CAMERA_SETTINGS in_CamSet, FITSCUBE_SETTINGS* out_RSet);
    int SaveFitsCubeSettingsToFile(char *filename, FITSCUBE_SETTINGS RSet1);
    int LoadFitsCubeSettingsFromFile(char *filename, FITSCUBE_SETTINGS *RSet1);
    int Write_FitsCubeSettingsToFITSDescriptors(DESCRIPTORS *descrip,
                                                FITSCUBE_SETTINGS RSet1);

#endif // EOF sentry
