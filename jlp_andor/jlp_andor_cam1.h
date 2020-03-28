/************************************************************************
* "jlp_andor_cam1.h"
* JLP_AndorCam1 class from virtual class JLP_Camera&
*
* JLP
* Version 05/10/2017
*************************************************************************/
#ifndef jlp_andor_cam1_h_
#define jlp_andor_cam1_h_

#include "jlp_camera1.h"     // JLP_Camera1 virtual class
#include "jlp_andor_utils.h" // ANDOR_SETTINGS, ...

// Definition from virtual class JLP_Camera1
class JLP_AndorCam1 : public JLP_Camera1
{

public:
// In "jlp_andor_cam1.cpp":
    JLP_AndorCam1();
    ~JLP_AndorCam1();

    int Cam1_ConnectToCamera();
    int JLP_Andor_GetCapabilities();
    int JLP_Andor_FirstInit();
    int Cam1_DisplaySettings();
    void JLP_Andor_InitCommonParameters();
//    int Cam1_Init(JLP_CAMERA_SETTINGS *CamSet0);
    int Cam1_ShutDown();
    int JLP_Andor_ApplyBasicSettings();
    int JLP_Andor_ApplySettingsForExposures();
    int Cam1_InitAcquisition(wxString& error_msg);
    int Cam1_GetTemperature(double *sensor_temp, double *box_temp,
                            wxString& error_msg);
    int Cam1_GetExpoTime(double *expo_time0);
    int Cam1_GetDigitalGain(double *digital_gain0);
    int JLP_Andor_CheckIfCoolerIsOn(bool *Cooler_is_already_on, double *Cooler_temp);
    int JLP_Andor_AllocateImageBuffer(int nx0, int ny0);
    int JLP_Andor_FreeImageBuffer();

// In "jlp_andor_cam1_set.cpp":

    int Cam1_LoadCameraSettingsFromFile(char *filename);
    int Cam1_SaveCameraSettingsToFile(char *filename);
    int Cam1_Write_CameraSettingsToFITSDescriptors(DESCRIPTORS *descrip);
    int Cam1_SettingsToDefault(int nx_full, int ny_full);
    int JLP_Andor_SetEMGain(wxString& error_msg);
    int JLP_Andor_SetEMGainBasicMode(wxString& error_msg);
    int Cam1_SetCoolerTemp(wxString& error_msg);
    int JLP_Andor_StartCooler();
    int JLP_Andor_StopCooler();
    int JLP_Andor_SetShutter(int ShutterMode, bool& ShutterAvailable);
    int Cam1_Get_CameraSettings(JLP_CAMERA_SETTINGS* CamSet0);
    int Cam1_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0);
    int Cam1_Get_AndorSettings(ANDOR_SETTINGS* AndorSet0);
    int Cam1_Load_AndorSettings(ANDOR_SETTINGS AndorSet0);

// In "jlp_andor_cam1_acqui.cpp":
    int Cam1_StopAcquisition(wxString &error_msg);
    int Cam1_StartAcquisition(int nx0, int ny0, wxString &error_msg);
    void Cam1_DisplaySettings(JLP_CAMERA_SETTINGS *CamSet0);
    int Cam1_SetSystemForSingleAcquisition(int nx0, int ny0, int xc0,
                                           int yc0, int xbin, int ybin);
    int Andor_StartSingleAcquisition();

// In "jlp_andor_cam1_acqui_conti.cpp":
    int Cam1_SetSystemForContinuousAcquisition(int nx0, int ny0, int xc0,
                                               int yc0, int xbin, int ybin);
    int Cam1_StartContinuousAcquisition();
    int Cam1_Continuous_GetTheImages(AWORD *ImageCube0,
                                          int nx0, int ny0, int nz0);
    int JLP_Andor_WaitForSingleImage(wxString& error_msg);
    int JLP_Andor_Single_ProcessTimer(wxString& error_msg, bool& image_is_read);
    bool JLP_Andor_AcquireImageData();

// Accessors:
    bool NotConnected(){return(not_connected);};
    void CloseShutterWhenExit(bool close_shutter_when_exit0) {
         close_shutter_when_exit = close_shutter_when_exit0;
         };
    double Get_Saturation_Value() {
      int pixel_depth;
      double value = 0.;
      if(!not_connected) {
// This function always return 16 (AD converter)
//        GetBitDepth(AndorSet1.AD_Channel, &pixel_depth);
// Ixon ultra : 14 bits ?
        pixel_depth = 14;
        value = pow(2., (double)pixel_depth);
        }
      return(value);
    };
    long Nz_total(){return(-1);};

private:
  ANDOR_SETTINGS AndorSet1;
  bool not_connected, close_shutter_when_exit;
// Flag is set when first acquisition is taken, tells
// system that there is data to display
  bool gblData_is_ready;
};

#endif // EOF sentry
