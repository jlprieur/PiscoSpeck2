/************************************************************************
* "jlp_raptor_panel.h"
* JLP_RaptorPanel class
*
* JLP
* Version 16/01/2018
*************************************************************************/
#ifndef jlp_raptor_panel_h_
#define jlp_raptor_panel_h_

#include "asp2_defs.h"       // JLP_ComboBox
#include "jlp_raptor_utils.h" // RAPTOR_SETTINGS, ...
#include "jlp_raptor_cam1.h"  // JLP_RaptorCam1
#include "jlp_camera_panel.h"  // JLP_CameraPanel

enum {

// Buttons:
  ID_RPANEL_CONFIG_OK = 9700,
  ID_RPANEL_CONFIG_CANCEL,
  ID_RPANEL_CONFIG_SAVE,
  ID_RPANEL_CONFIG_LOAD,
  ID_RPANEL_CONFIG_DEFAULT,
  ID_RPANEL_CURRENT_STATUS,

// Text controls:
  ID_RPANEL_ROI_CENTER,
  ID_RPANEL_COOLER_SETPOINT,
  ID_RPANEL_DIGITAL_GAIN,
  ID_RPANEL_TRIGGER_DELAY,
  ID_RPANEL_EXPO_TIME,
  ID_RPANEL_INTEG_TIME,

// Check boxes:
  ID_RPANEL_COOLER_REGUL,
  ID_RPANEL_COOLER_FAN,
  ID_RPANEL_HIGH_GAIN_CHK,
  ID_RPANEL_AUTO_EXPOSURE,
  ID_RPANEL_IMAGE_SHARPEN,

// Combo boxes:
  ID_RPANEL_HEAD_MODEL,
  ID_RPANEL_ACQUI_MODE,
  ID_RPANEL_ROI_SIZE,
  ID_RPANEL_ROTATION,
  ID_RPANEL_MIRROR,

  ID_RPANEL_EXT_TRIGGER,
  ID_RPANEL_BIN_FACTOR,
  ID_RPANEL_FRAME_RATE,
  ID_RPANEL_NUC_STATE,
  ID_RPANEL_SHUTTER,

};

class AspFrame;

 class JLP_RaptorPanel : wxPanel
{

public:

// In "jlp_raptor_panel_setup.cpp":

  JLP_RaptorPanel(AspFrame *asp_frame, wxFrame *frame, const int my_wxID,
                 JLP_RaptorCam1 *cam1, JLP_CAMERA_PANEL_CHANGES *jcam_changes0,
                 wxString *str_messg, const int n_messg, bool small_version);
  ~JLP_RaptorPanel(){
   MyFreeMemory();
   };

  void MyFreeMemory();
  int RaptorPanel_Setup();
  void RaptorPanel_LeftSetup(wxBoxSizer *raptor_left_sizer);
  void RaptorPanel_CentSetup(wxBoxSizer *raptor_cent_sizer);
  void RaptorPanel_RightSetup(wxBoxSizer *raptor_right_sizer);
  void InitComboBoxItems_without_Raptor();
  int InitComboBoxItems_with_Raptor();

// Virtual routines, called when changing pages:
  void CamPanel_ValidateChanges();
  void CamPanel_CancelNonValidatedChanges();
  void CamPanel_ValidateNewSettings();

// In "jlp_raptor_panel_onclick1.cpp":
  void OnIntegTimeOnExit(wxCommandEvent& event);
  void UpdateIntegTime(const bool update_from_screen);
  void OnExpoTimeOnExit(wxCommandEvent& event);
  void UpdateExpoTime(const bool update_from_screen);
  void OnTriggerDelayOnExit(wxCommandEvent& event);
  void UpdateTriggerDelay(const bool update_from_screen);
  void OnROICenterOnExit(wxCommandEvent& event);
  void UpdateROICenter(const bool update_from_screen);
  void OnExtTriggerComboBoxChange(wxCommandEvent& event);
  void OnAcquiModeComboBoxChange(wxCommandEvent& event);

  void OnCoolerSetPointEditExit(wxCommandEvent& event);
  void UpdateCoolerSetPoint(const bool update_from_screen);
  void OnCoolerCheckBoxClick(wxCommandEvent& event);
  void OnCoolerFanCheckBoxClick(wxCommandEvent& event);

  void OnHighGainCheckBoxClick(wxCommandEvent& event);
  void OnDigitalGainEditExit(wxCommandEvent& event);
  void UpdateDigitalGain(const bool update_from_screen);

  void OnBinFactorComboBoxChange(wxCommandEvent& event);
  void OnROISizeComboBoxChange(wxCommandEvent& event);
  void OnMirrorComboBoxChange(wxCommandEvent& event);
  void OnRotationComboBoxChange(wxCommandEvent& event);

  void OnFrameRateComboBoxChange(wxCommandEvent& event);
  void OnNucStateComboBoxChange(wxCommandEvent& event);
  void OnShutterComboBoxChange(wxCommandEvent& event);
  void OnAutoExposureCheckBoxClick(wxCommandEvent& event);
  void OnImageSharpenCheckBoxClick(wxCommandEvent& event);
  void OnConfig(wxCommandEvent& event);
  void OnCurrentStatus(wxCommandEvent& event);

  void Get_RaptorSettings(RAPTOR_SETTINGS& RaptorSet0){
   Copy_RaptorSettings(RaptorSet1, &RaptorSet0);
   return;
   }

protected:

// In "jlp_raptor_panel_update.cpp":

  void UpdateFinalImageSizeLabels();
  void UpdateAcquisitionMode();
  void UpdateROISizeComboBox();
  void UpdateAutoExposure();
  void UpdateImageSharpen();
  void UpdateBinFactor();
  void UpdateExtTriggerMode();
  void UpdateShutterMode();
  void UpdateMirrorMode();
  void UpdateFrameRate();
  void UpdateNucState();
  void UpdateRotationMode();
  void UpdateCoolerSettings();
  void DisplayNewSettings(bool update_from_screen);
  int LoadRaptorSettingsDialog();
  int SaveRaptorSettingsDialog();

private:

  AspFrame *m_aspframe;
  JLP_RaptorCam1 *m_cam1;
  wxString *m_messg;
  JLP_CAMERA_PANEL_CHANGES *jcam_changes1;
  RAPTOR_SETTINGS RaptorSet1, Original_RaptorSet1;
  bool ExpertMode;
  int initialized;

  wxButton *RaptorBut_CheckCurrentStatus;
  wxTextCtrl *RaptorCtrl_ROICenter, *RaptorCtrl_CoolerSetTemp;
  wxTextCtrl *RaptorCtrl_DigitalGain, *RaptorCtrl_IntegTime;
  wxTextCtrl *RaptorCtrl_ExpoTime, *RaptorCtrl_TriggerDelay;
  wxStaticText *RaptorStatic_DetectorFormat, *RaptorStatic_ImageFormat;
  wxStaticText *RaptorStatic_StatusSensorTemp, *RaptorStatic_StatusBoxTemp;
  wxStaticText *RaptorStatic_StatusExpoTime, *RaptorStatic_StatusDigitalGain;
  JLP_ComboBox RaptorCmb_AcquiMode, RaptorCmb_ROISize, RaptorCmb_Rotation;
  JLP_ComboBox RaptorCmb_BinFactor, RaptorCmb_Mirror, RaptorCmb_Shutter;
  JLP_ComboBox RaptorCmb_ExtTrigger, RaptorCmb_FrameRate, RaptorCmb_NUC_State;
  wxCheckBox *RaptorCheck_CoolerRegul, *RaptorCheck_ImageSharpen;
  wxCheckBox *RaptorCheck_AutoExposure, *RaptorCheck_HighGain;
  wxCheckBox *RaptorCheck_CoolerFan;

/*
*********** Check boxes:
Cooler fan:
    Off
    On
Auto_Exposure:
    Off
    On
High Gain:
    Off
    On
Image Sharpen:
    Off
    On
CoolerRegul (TE_Cooler):
    Off
    On

*********** Edit boxes:

Digital Gain: 25 (0--48)
Exposure: 28 ms (0--1000)
TEC Setpoint: -15 (-68--119)
Trigger Delay: 0 (0--104)

*********** Combo boxes:

Ext Trigger:
    Off
    On: (Ext +ve Edge)
    On: (Ext -ve Edge)
Frame rate :
    0: 25 Hz
    1: 30 Hz
    3: 50 Hz
    4: 60 Hz
    5: 90 Hz
    6: 120 Hz
NUC State:
    1: Offset+Gain corr.
    2: Normal
    3: Offset+Gain+Dark
    5: 8 bit Offset - 32
    5: 8 bit Dark - 2^19
    6: 8 bit Gain - 128
    7: Reversed map
    8: Ramp Test Pattern
*/

 DECLARE_EVENT_TABLE()
};

#endif // EOF sentry
