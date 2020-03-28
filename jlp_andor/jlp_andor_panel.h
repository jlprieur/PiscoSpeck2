/************************************************************************
* "jlp_andor_panel.h"
* JLP_AndorPanel class
*
* JLP
* Version 17/08/2015
*************************************************************************/
#ifndef jlp_andor_panel_h_
#define jlp_andor_panel_h_

#include "asp2_defs.h"       // JLP_ComboBox
#include "jlp_andor_utils.h" // ANDOR_SETTINGS, ...
#include "jlp_andor_cam1.h"  // JLP_AndorCam1
#include "jlp_camera_panel.h"  // JLP_CameraPanel

enum {

// Buttons:
  ID_APANEL_CONFIG_OK = 9000,
  ID_APANEL_CONFIG_CANCEL,
  ID_APANEL_CONFIG_SAVE,
  ID_APANEL_CONFIG_LOAD,
  ID_APANEL_CONFIG_DEFAULT,

// Text controls:
  ID_APANEL_ROI_CENTER,
  ID_APANEL_COOLER_SETPOINT,
  ID_APANEL_EMGAIN,
  ID_APANEL_KIN_CLOCKTIME,
  ID_APANEL_KIN_CYCLETIME,
  ID_APANEL_KIN_EXPOTIME,
  ID_APANEL_KIN_NCYCLE,
  ID_APANEL_KIN_NEXP,

// Check boxes:
  ID_APANEL_COOLER_REGUL,
  ID_APANEL_EMGAIN_CHK,
  ID_APANEL_FR_TRANSF,
  ID_APANEL_BLINE_CLAMP,

// Combo boxes:
  ID_APANEL_ACQUI_MODE,
  ID_APANEL_ROI_SIZE,
  ID_APANEL_ROTATION,
  ID_APANEL_MIRROR,
  ID_APANEL_TRIGGER,
  ID_APANEL_BIN_FACTOR,
  ID_APANEL_VSPEED,
  ID_APANEL_VVOLT,
  ID_APANEL_HSPEED,
  ID_APANEL_PREAMP_GAIN,
  ID_APANEL_ADCHANNEL,
  ID_APANEL_SHUTTER,
  ID_APANEL_CLOSESHUTTER0_CHK,

};

class AspFrame;

 class JLP_AndorPanel : wxPanel
{

public:

// In "jlp_andor_panel_setup.cpp":

  JLP_AndorPanel(AspFrame *asp_frame, wxFrame *frame, const int my_wxID,
                 JLP_AndorCam1 *cam1, JLP_CAMERA_PANEL_CHANGES *jcam_changes0,
                 wxString *str_messg, const int n_messg, bool small_version);
  ~JLP_AndorPanel(){
   MyFreeMemory();
   };

  void MyFreeMemory();
  int AndorPanel_Setup();
  void AndorPanel_LeftSetup(wxBoxSizer *andor_left_sizer);
  void AndorPanel_RightSetup(wxBoxSizer *andor_right_sizer);
  void InitComboBoxItems_without_Andor();
  int InitComboBoxItems_with_Andor();

// In "jlp_andor_panel_onclick1.cpp":
  void OnROICenterOnExit(wxCommandEvent& event);
  void UpdateROICenter(const bool update_from_screen);
  void OnTriggerComboBoxChange(wxCommandEvent& event);
  void OnAcquiModeComboBoxChange(wxCommandEvent& event);

  void OnCoolerSetPointEditExit(wxCommandEvent& event);
  void UpdateCoolerSetPoint(const bool update_from_screen);
  void OnCoolerCheckBoxClick(wxCommandEvent& event);

  void OnEMGainCheckBoxClick(wxCommandEvent& event);
  void OnEMGainEditExit(wxCommandEvent& event);
  void UpdateEMGain(const bool update_from_screen);

  void UpdateLongExposureTime(const bool update_from_screen);
  void OnBinFactorComboBoxChange(wxCommandEvent& event);
  void OnROISizeComboBoxChange(wxCommandEvent& event);
  void OnMirrorComboBoxChange(wxCommandEvent& event);
  void OnRotationComboBoxChange(wxCommandEvent& event);

  void OnVShiftSpeedComboBoxChange(wxCommandEvent& event);
  void OnVShiftVoltageComboBoxChange(wxCommandEvent& event);
  void OnHShiftSpeedComboBoxChange(wxCommandEvent& event);
  void OnHShiftPreAmpGainComboBoxChange(wxCommandEvent& event);

  void OnADComboBoxChange(wxCommandEvent& event);
  void OnShutterComboBoxChange(wxCommandEvent& event);
  void OnFrameTransferCheckBoxClick(wxCommandEvent& event);
  void OnBaselineClampCheckBoxClick(wxCommandEvent& event);
  void OnCloseShutterWhenExitCheckBoxClick(wxCommandEvent& event);
  void OnConfig(wxCommandEvent& event);

// In "jlp_andor_panel_onclick2.cpp":
  void OnKineticExpoTimeEditExit(wxCommandEvent& event);
  void UpdateKineticExpoTime(const bool update_from_screen);
  void CheckAndCorrectKineticParameters();
  void OnKineticCycleNExposuresEditExit(wxCommandEvent& event);
  void UpdateKineticCycleNExposures(const bool update_from_screen);
  void OnKineticCycleTimeEditExit(wxCommandEvent& event);
  void UpdateKineticCycleTime(const bool update_from_screen);
  void OnKineticClockTimeEditExit(wxCommandEvent& event);
  void UpdateKineticClockTime(const bool update_from_screen);
  void OnKineticNCyclesEditExit(wxCommandEvent& event);
  void UpdateKineticNCycles(const bool update_from_screen);

  void Get_AndorSettings(ANDOR_SETTINGS* AndorSet0){
   Copy_AndorSettings(AndorSet1, AndorSet0);
   return;
   }

// Virtual routines, called when changing pages:
  void CamPanel_ValidateChanges();
  void CamPanel_CancelNonValidatedChanges();
  void CamPanel_ValidateNewSettings();

protected:

// In "jlp_andor_panel_update.cpp":

  void UpdateFinalImageSizeLabels();
  void UpdateAcquisitionMode();
  void UpdateROISizeComboBox();
  void UpdateBinFactor();
  void UpdateTriggerMode();
  void UpdateShutterMode();
  void UpdateMirrorMode();
  void UpdateVShiftSpeed();
  void UpdateVShiftVoltage();
  void UpdateHShiftSpeed();
  void UpdateHShiftPreAmpGain();
  void UpdateAD_Channel();
  void UpdateRotationMode();
  void UpdateCoolerSettings();
  void DisplayNewSettings(bool update_from_screen);
  int LoadAndorSettingsDialog();
  int SaveAndorSettingsDialog();

private:

  JLP_CAMERA_SETTINGS CamSet1, Original_CamSet1;

  AspFrame *m_aspframe;
  JLP_AndorCam1 *m_cam1;
  wxString *m_messg;
  ANDOR_SETTINGS AndorSet1, Original_AndorSet1;
  bool ExpertMode;
  int initialized, andor_not_connected;
  JLP_CAMERA_PANEL_CHANGES *jcam_changes1;

  wxTextCtrl *AndorCtrl_ROICenter, *AndorCtrl_CoolerSetPoint;
  wxTextCtrl *AndorCtrl_KinExpoTime, *AndorCtrl_KinClockTime;
  wxTextCtrl *AndorCtrl_KinNExp;
  wxTextCtrl *AndorCtrl_KinCycleTime, *AndorCtrl_KinNCycle;
  wxTextCtrl *AndorCtrl_EMGain, *AndorCtrl_LongExpoTime;
  wxStaticText *AndorStatic_DetectorFormat, *AndorStatic_ImageFormat;
  wxStaticText *AndorStatic_ShutdownOptions;
  JLP_ComboBox AndorCmb_AcquiMode, AndorCmb_ROISize, AndorCmb_Rotation;
  JLP_ComboBox AndorCmb_BinFactor, AndorCmb_Mirror, AndorCmb_Trigger;
  JLP_ComboBox AndorCmb_Shutter, AndorCmb_VShiftSpeed, AndorCmb_VShiftVolt;
  JLP_ComboBox AndorCmb_HShiftSpeed, AndorCmb_PreAmpGain, AndorCmb_ADChannel;
  wxCheckBox *AndorCheck_CoolerRegul, *AndorCheck_EMCCD;
  wxCheckBox *AndorCheck_BaselineClamp, *AndorCheck_FrameTransfer;
  wxCheckBox *AndorCheck_CloseShutterWhenExit;

 DECLARE_EVENT_TABLE()
};

#endif // EOF sentry
