/**************************************************************************
* jlp_camera_display_mutex.h
*
* Class defining to handle display1 mutex
* Purpose: dialog between the main process (read for display)
*          and the camera thread (write after acquisition)
* JLP
* Version 14/01/2016
**************************************************************************/
#ifndef jlp_camera_display_mutex_h
#define jlp_camera_display_mutex_h

#include "asp2_typedef.h"    // AWORD

class JLP_CameraDisplayMutex
{
public:
  JLP_CameraDisplayMutex(const int nx, const int ny);
  ~JLP_CameraDisplayMutex();
  int ReadImage(double *short_exposure, const int nx0, const int ny0);
  int WriteImage(AWORD *short_exposure, const int nx0, const int ny0);

private:
  wxMutex *hCameraDisplayMutex;
  double *image2;
  int nx2, ny2;
};
#endif
