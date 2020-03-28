/************************************************************************
* "jlp_fitscube_panel.h"
* JLP_FitsCubePanel class
*
* JLP
* Version 16/11/2017
*************************************************************************/
#ifndef jlp_fitscube_panel_h_
#define jlp_fitscube_panel_h_

#include "asp2_defs.h"       // JLP_ComboBox#include "jlp_camera_panel.h"  // JLP_CameraPanel
#include "jlp_camera_panel.h"        // JLP_CAMERA_PANEL_CHANGES
#include "jlp_fitscube_cam1.h"  // JLP_FitsCubeCam1
#include "jlp_fitscube_utils.h" // FITSCUBE_SETTINGS

enum {

// Buttons:
  ID_FPANEL_CONFIG_OK = 9300,
  ID_FPANEL_CONFIG_CANCEL,
  ID_FPANEL_CONFIG_SAVE,
  ID_FPANEL_CONFIG_LOAD,
  ID_FPANEL_CONFIG_DEFAULT,

// Text controls:
  ID_FPANEL_ROI_CENTER,

// Combo boxes:
  ID_FPANEL_ROI_SIZE,
  ID_FPANEL_ROTATION,
  ID_FPANEL_MIRROR,
  ID_FPANEL_BIN_FACTOR,

};

class AspFrame;

 class JLP_FitsCubePanel : wxPanel
{

public:

// In "jlp_fitscube_panel_setup.cpp":

  JLP_FitsCubePanel(AspFrame *asp_frame, wxFrame *frame, const int my_wxID,
                    JLP_FitsCubeCam1 *fitscube_cam0, JLP_CAMERA_PANEL_CHANGES *jcam_changes0,
                    wxString *str_messg, const int n_messg, bool small_version);
  ~JLP_FitsCubePanel(){
   MyFreeMemory();
   };

  void MyFreeMemory();
  int FitsCubePanel_Setup();
  void FitsCubePanel_LeftSetup(wxBoxSizer *fitscube_left_sizer);
  void InitComboBoxItems();

// In "jlp_fitscube_panel_onclick1.cpp":
// In "jlp_raptor_panel_onclick1.cpp":
  void OnROICenterOnExit(wxCommandEvent& event);
  void UpdateROICenter(const bool update_from_screen);
  void OnBinFactorComboBoxChange(wxCommandEvent& event);
  void OnROISizeComboBoxChange(wxCommandEvent& event);
  void OnMirrorComboBoxChange(wxCommandEvent& event);
  void OnRotationComboBoxChange(wxCommandEvent& event);
  void OnConfig(wxCommandEvent& event);

  void Get_CameraSettings(JLP_CAMERA_SETTINGS *CamSet0){
   Copy_FitsCubeToCameraSettings(FitsCubeSet1, CamSet0);
   return;
   }

// Virtual routines, called when changing pages:
  void CamPanel_ValidateChanges();
  void CamPanel_CancelNonValidatedChanges();
  void CamPanel_ValidateNewSettings();

protected:

// In "jlp_fitscube_panel_update.cpp":

  void UpdateFinalImageSizeLabels();
  void UpdateROISizeComboBox();
  void UpdateBinFactor();
  void UpdateMirrorMode();
  void UpdateRotationMode();
  void DisplayNewSettings(bool update_from_screen);
  int LoadFitsCubeSettingsDialog();
  int SaveFitsCubeSettingsDialog();

private:
  JLP_CAMERA_PANEL_CHANGES *jcam_changes1;
  FITSCUBE_SETTINGS FitsCubeSet1, Original_FitsCubeSet1;

  JLP_FitsCubeCam1 *m_cam1;
  AspFrame *m_aspframe;
  wxString *m_messg;
  int initialized;

  wxTextCtrl *FitsCubeCtrl_ROICenter;
  wxStaticText *FitsCubeStatic_DetectorFormat,*FitsCubeStatic_ImageFormat;
  JLP_ComboBox FitsCubeCmb_ROISize, FitsCubeCmb_Rotation;
  JLP_ComboBox FitsCubeCmb_BinFactor, FitsCubeCmb_Mirror;

 DECLARE_EVENT_TABLE()
};

#endif // EOF sentry
