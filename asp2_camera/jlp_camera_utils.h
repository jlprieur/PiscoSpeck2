/************************************************************************
* "jlp_camera_utils.h"
*
* JLP
* Version 05/10/2017
*************************************************************************/
#ifndef jlp_camera_utils_h_
#define jlp_camera_utils_h_

#include "asp2_typedef.h" // JLP_CAMERA_SETTINGS, EXPOSURE_PARAM, BYTE, etc

// In "jlp_camera_utils.cpp":

void CameraSettingsToDefault(JLP_CAMERA_SETTINGS* out_CamSet, int nx_full, int ny_full);
int Copy_CameraSettings(JLP_CAMERA_SETTINGS in_CamSet, JLP_CAMERA_SETTINGS* out_CamSet);
int SaveCameraSettingsToFile(char *filename, JLP_CAMERA_SETTINGS in_CamSet);
int LoadCameraSettingsFromFile(char *filename, JLP_CAMERA_SETTINGS* out_CamSet);
int Write_ExposureParametersToFITSDescriptors(DESCRIPTORS *descrip,
                                         EXPOSURE_PARAM ExpoSet1);

#endif // EOF sentry
