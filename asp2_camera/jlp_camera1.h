/************************************************************************
* "jlp_camera_cam1.h"
* JLP_Camera1 class
* \file jlp_camera1.h
* \class JLP_Camera1
* \author JLP
* \date 05/10/2017
* \brief Definition of camera routines (as a virtual class)
*
* JLP
* Version 05/10/2017
*************************************************************************/
#ifndef jlp_camera1_h_
#define jlp_camera1_h_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "jlp_camera_utils.h" // JLP_CAMERA_SETTINGS, ...

/* Definition of the prototype of all JLP_Camera1 classes
* (i.e. for ANDOR, Raptor, etc)
*/
class JLP_Camera1 {

public:

// Abstract class should not have any definition of the constructors
    ~JLP_Camera1(){return;};

// In "jlp_camera1.cpp":
    int JLPCam1_InitPrivateParameters(JLP_CAMERA_SETTINGS CamSet0);
    int JLPCam1_AllocateImageBuffer(int nx0, int ny0);
    int JLPCam1_FreeImageBuffer();
    int JLPCam1_ShutDown();

// Virtual routines that must be defined in the implemented versions...
    virtual int Cam1_ShutDown() = 0;
    virtual int Cam1_ConnectToCamera() = 0;
    virtual int Cam1_InitAcquisition(wxString& error_msg) = 0;
    virtual int Cam1_GetExpoTime(double *expo_time0) = 0;
    virtual int Cam1_GetDigitalGain(double *digital_gain0) = 0;
    virtual int Cam1_GetTemperature(double *sensor_temp, double *box_temp,
                                    wxString& error_msg) = 0;

    virtual int Cam1_LoadCameraSettingsFromFile(char *filename) = 0;
    virtual int Cam1_SaveCameraSettingsToFile(char *filename) = 0;
    virtual int Cam1_Write_CameraSettingsToFITSDescriptors(DESCRIPTORS *descrip) = 0;
    virtual int Cam1_DisplaySettings() = 0;
    virtual int Cam1_SettingsToDefault(int nx_full, int ny_full) = 0;
    virtual int Cam1_Get_CameraSettings(JLP_CAMERA_SETTINGS* CamSet0) = 0;
    virtual int Cam1_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0) = 0;

    virtual int Cam1_SetCoolerTemp(wxString& error_msg) = 0;
    virtual int Cam1_StopAcquisition(wxString &error_msg) = 0;
    virtual int Cam1_StartAcquisition(int nx0, int ny0, wxString &error_msg) = 0;
    virtual int Cam1_SetSystemForSingleAcquisition(int nx0, int ny0, int xc0,
                                                   int yc0, int xbin, int ybin) = 0;
    virtual int Cam1_SetSystemForContinuousAcquisition(int nx0, int ny0, int xc0,
                                                       int yc0, int xbin, int ybin) = 0;
    virtual int Cam1_StartContinuousAcquisition() = 0;
    virtual int Cam1_Continuous_GetTheImages(AWORD *ImageCube0, int nx0, int ny0, int nz0) = 0;

// Accessors:
    virtual bool NotConnected() = 0;
    virtual void CloseShutterWhenExit(bool close_shutter_when_exit0) = 0;
    virtual double Get_Saturation_Value() = 0;
    virtual long Nz_total() = 0;

private:
  int initialized;
};

#endif // EOF sentry
