/************************************************************************
* "jlp_fitscube_cam1.h"
* JLP_FitsCubeCam1 class from virtual class JLP_Camera
*
* JLP
* Version 11/10/2017
*************************************************************************/
#ifndef jlp_fitscube_cam1_h_
#define jlp_fitscube_cam1_h_

#include "jlp_camera1.h"     // JLP_Camera1 virtual class
#include "asp2_3D_infits.h"
#include "jlp_fitscube_utils.h"     // FITSCUBE_SETTINGS

// Definition from virtual class JLP_Camera1
class JLP_FitsCubeCam1 : public JLP_Camera1
{

public:
// In "jlp_fitscube_cam1.cpp":
    JLP_FitsCubeCam1(char *filename);
    ~JLP_FitsCubeCam1();

    int Cam1_ConnectToCamera();
    int JLP_FitsCube_GetCapabilities();
    int JLP_FitsCube_FirstInit();
    int Cam1_DisplaySettings();
    int Load_FitsCubeSettings(FITSCUBE_SETTINGS FitsCubeSet0);
    int Get_FitsCubeSettings(FITSCUBE_SETTINGS* FitsCubeSet0);
    int Cam1_Get_CameraSettings(JLP_CAMERA_SETTINGS *CamSet0);
    int Cam1_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0);
    void JLP_FitsCube_InitCommonParameters();
//    int Cam1_Init(JLP_CAMERA_SETTINGS *CamSet0);
    int Cam1_ShutDown();
    int JLP_FitsCube_ApplyBasicSettings();
    int Cam1_InitAcquisition(wxString& error_msg);
    int Cam1_GetTemperature(double *sensor_temp, double *box_temp,
                            wxString& error_msg)
     {
// Return empty string and dummy value:
     error_msg = wxT("");
     *sensor_temp = 25.;
     *box_temp = 25.;
     return(-1);
     }
// Return dummy value:
    int Cam1_GetExpoTime(double *expo_time0)
     {
     *expo_time0 = 0.;
     return(-1);
     }
// Return dummy value:
    int Cam1_GetDigitalGain(double *digital_gain0)
     {
     *digital_gain0 = 1.;
     return(-1);
     }

// In "jlp_fitscube_cam1_set.cpp":
    int Cam1_LoadCameraSettingsFromFile(char *filename);
    int Cam1_SaveCameraSettingsToFile(char *filename);
    int Cam1_Write_CameraSettingsToFITSDescriptors(DESCRIPTORS *descrip);
    int Cam1_SettingsToDefault(int nx_full, int ny_full);
    int Cam1_SetCoolerTemp(wxString& error_msg);

// In "jlp_fitscube_cam1_acqui.cpp":
    int JLP_FitsCube_ApplySettingsForExposures();
    int Cam1_StopAcquisition(wxString &error_msg);
    int Cam1_StartAcquisition(int nx0, int ny0, wxString &error_msg);
    void Cam1_DisplaySettings(JLP_CAMERA_SETTINGS *CamSet0);
    int Cam1_SetSystemForSingleAcquisition(int nx0, int ny0, int xc0,
                                           int yc0, int xbin, int ybin);

// In "jlp_fitscube_cam1_acqui_conti.cpp":
    int Cam1_SetSystemForContinuousAcquisition(int nx0, int ny0, int xc0,
                                           int yc0, int xbin, int ybin);
    int Cam1_StartContinuousAcquisition();
    int Cam1_Continuous_GetTheImages(AWORD *ImageCube0,
                                          int nx0, int ny0, int nz0);

// Accessors:
    bool NotConnected(){return(not_connected);};
    void CloseShutterWhenExit(bool close_shutter_when_exit0) {
         return;
         };
    double Get_Saturation_Value() {
      int pixel_depth;
      double value = 0.;
      if(!not_connected) {
// This function always return 16 (AD converter)
//        GetBitDepth(FitsCubeSet1.AD_Channel, &pixel_depth);
// Ixon ultra : 14 bits ?
        pixel_depth = 14;
        value = pow(2., (double)pixel_depth);
        }
      return(value);
    };
    long Nz_total(){return(inFitsFile1->Nz_Total());};

private:
  FITSCUBE_SETTINGS FitsCubeSet1;
  bool not_connected, all_data_read;
  JLP_3D_inFITSFile *inFitsFile1;
// Temporary memory:
  AWORD *ImageCube1;
};

#endif // EOF sentry
