/************************************************************************
* "jlp_raptor_cam1.h"
* JLP_RaptorCam1 class from virtual class JLP_Camera&
*
* JLP
* Version 11/10/2017
*************************************************************************/
#ifndef jlp_raptor_cam1_h_
#define jlp_raptor_cam1_h_

#include "jlp_camera1.h"     // JLP_Camera1 virtual class
#include "jlp_raptor_utils.h" // RAPTOR_SETTINGS, ...
#ifdef WIN32
#include "epix_xcliball.h" // in ./jlp_raptor/epix_inc
#endif

#define UNITS 1
#define UNITMASK 1
#define UNITSOPENMAP 1
#define UNITSMAP 1

// Definition from virtual class JLP_Camera1
class JLP_RaptorCam1 : public JLP_Camera1
{

public:
// In "jlp_raptor_cam1.cpp":
    JLP_RaptorCam1();
    ~JLP_RaptorCam1();

    int Cam1_ConnectToCamera();
    int JLP_Raptor_FirstInit();
    int Cam1_DisplaySettings();
    void JLP_Raptor_InitCommonParameters();
//    int Cam1_Init(JLP_CAMERA_SETTINGS *CamSet0);
    int Cam1_ShutDown();
    int JLP_Raptor_ApplyBasicSettings();
    int Cam1_InitAcquisition(wxString& error_msg);
    int JLP_Raptor_Serial_Initialize();
    int JLP_Raptor_DecodeSerialNumber(unsigned char *data_read0, int nread0);

// In "jlp_raptor_cam1_get.cpp":
    int Cam1_GetTemperature(double *sensor_temp, double *box_temp,
                            wxString& error_msg);
    int JLP_Raptor_GetSensorTemperature(double *current_temp, wxString& error_msg);
    int JLP_Raptor_GetPCBTemperature(double *current_temp, wxString& error_msg);
    int JLP_Serial_ReadOutputRegister1(unsigned char *data_read0,
                                       int *nread0, char *comments0);
    int Cam1_GetExpoTime(double *expo_time0);
    int Cam1_GetDigitalGain(double *digital_gain0);
    int JLP_Raptor_GetFrameRate(int *frame_rate0);
    int JLP_Raptor_GetFPGAStatus(bool *horiz_flip0, bool *video_inverted0,
                                 bool *fan_enabled0, bool *auto_exposure_enabled0,
                                 bool *TEC_enabled0);
    int JLP_Raptor_GetCoolerSetTemp(double *cooler_set_temp0);
    int JLP_Raptor_GetNUCState(double *current_nuc0);

// In "jlp_raptor_cam1_set.cpp":
    int Cam1_LoadCameraSettingsFromFile(char *filename);
    int Cam1_SaveCameraSettingsToFile(char *filename);
    int Cam1_Write_CameraSettingsToFITSDescriptors(DESCRIPTORS *descrip);
    int Cam1_SettingsToDefault(int nx_full, int ny_full);
    int JLP_Raptor_SetHighGainAndTriggerMode();
    int JLP_Raptor_SetHighOrDigitalGain(wxString& error_msg);
    int Cam1_SetCoolerTemp(wxString& error_msg);
    int JLP_Raptor_StartCooler();
    int JLP_Raptor_StopCooler();
    int JLP_Raptor_SetShutter(int ShutterMode, bool& ShutterAvailable);
    int Cam1_Get_CameraSettings(JLP_CAMERA_SETTINGS* CamSet0);
    int Cam1_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0);
    int Get_RaptorSettings(RAPTOR_SETTINGS* RaptorSet0);
    int Load_RaptorSettings(RAPTOR_SETTINGS RaptorSet0);
    int JLP_Raptor_SetExpoTimeOrAutoExpo();
    int JLP_Raptor_SetFrameRate();
    int JLP_Raptor_SetTriggerDelay();
    int JLP_Raptor_Set_FPGA_Options();
    int JLP_Raptor_Set_NUC_State();
    int JLP_Raptor_SetImageSharpen();

// In "jlp_raptor_cam1_tools.cpp":
    int DisableMicro();
    int SetSystemState(unsigned char nState);
    unsigned char GetSystemState();
    int ConvertTECTempToValue(double dTemp);
    double ConvertTECValueToTemp(int nValue);
    double GetTECSetPoint();
    void SetTECSetPoint(double dValue);


// In "jlp_raptor_cam1_serial.cpp":
    int serialWriteReadCmd(int unitopenmap, int unit,
                           unsigned char* bufin, int insize,
                           unsigned char* bufout,
                           int outsize, bool bChkSum=true);
    int JLP_Raptor_SerialSend(unsigned char *data_send0, int nbytes0);
    int JLP_Raptor_SerialSendReceive(unsigned char *data_send0,
                                     unsigned char *data_read0,
                                     const int nsend0, const int expected_nread0,
                                     int *nread0);
    int JLP_Raptor_SerialClearPort();
    int JLP_Raptor_SerialReceive_0(unsigned char *data_read0,
                                   int expected_nread0, int *nread0);
    int JLP_Raptor_SerialReceive_1(unsigned char *data_read0,
                                   int expected_nread0, int *nread0);
    int serialReadRaptorRegister1(int unit, unsigned char nReg,
                                  unsigned char* val);
    int serialReadRaptorRegister2(int unit, unsigned char nReg,
                                  unsigned char* val );
    int serial_log_command(unsigned char *data_send0, int nsend0,
                                unsigned char *data_read0, int nread0,
                                char *comments);

// In "jlp_raptor_cam1_acqui.cpp":
    int JLP_Raptor_ApplySettingsForExposures();
    int Cam1_StopAcquisition(wxString &error_msg);
    int Cam1_StartAcquisition(int nx0, int ny0, wxString &error_msg);
    void Cam1_DisplaySettings(JLP_CAMERA_SETTINGS *CamSet0);
    int Cam1_SetSystemForSingleAcquisition(int nx0, int ny0, int xc0,
                                           int yc0, int xbin, int ybin);

// In "jlp_raptor_cam1_acqui_conti.cpp":
    int Cam1_SetSystemForContinuousAcquisition(int nx0, int ny0, int xc0,
                                              int yc0, int xbin, int ybin);
    int Cam1_StartContinuousAcquisition();
    int Cam1_Continuous_GetTheImages(AWORD *ImageCube0,
                                          int nx0, int ny0, int nz0);

// Accessors:
    bool NotConnected(){return(not_connected);};
    void CloseShutterWhenExit(bool close_shutter_when_exit0) {
// Not used yet
         return;
         };
    double Get_Saturation_Value() {
      int pixel_depth;
      double value = 0.;
      if(!not_connected) {
// This function always return 16 (AD converter)
//        GetBitDepth(RaptorSet1.AD_Channel, &pixel_depth);
// Ixon ultra : 14 bits ?
        pixel_depth = 14;
        value = pow(2., (double)pixel_depth);
        }
      return(value);
    };
    long Nz_total(){return(-1);};

private:
  RAPTOR_SETTINGS RaptorSet1;
  bool not_connected, check_sum1;
  long EPROM_SerialNumber;
  char EPROM_BuildCode[6];
  long EPROM_ADC_Cal_0C, EPROM_ADC_Cal_40C;
  long EPROM_DAC_Cal_0C, EPROM_DAC_Cal_40C;
  long EPROM_BuildDateDD, EPROM_BuildDateMM, EPROM_BuildDateYY;
};

#endif // EOF sentry
