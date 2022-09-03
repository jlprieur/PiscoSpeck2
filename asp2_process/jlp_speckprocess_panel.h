/************************************************************************
* "jlp_speckprocess_panel.h"
* JLP_SpeckProcessPanel class
*
* JLP
* Version 28/08/2015
*************************************************************************/
#ifndef jlp_speckprocess_panel_h_
#define jlp_speckprocess_panel_h_

#include "asp2_defs.h"       // JLP_ComboBox
#include "asp2_typedef.h"    // PROCESS_SETTINGS
#include "asp2_rw_config_files.h"  // Copy_PSET()

enum {

// Buttons:

  ID_PROC_CONFIG_OK = 3000,
  ID_PROC_CONFIG_CANCEL,
  ID_PROC_CONFIG_SAVE,
  ID_PROC_CONFIG_LOAD,
  ID_PROC_CONFIG_DEFAULT,

// Load files:
  ID_PROC_LOAD_FFIELD,
  ID_PROC_LOAD_OFFSET,
  ID_PROC_LOAD_UNRES_AUTOC,
  ID_PROC_LOAD_UNRES_MODSQ,
  ID_PROC_LOAD_PHOT_MODSQ,

// Text controls:
  ID_PROC_LUCKY_THRESH,
  ID_PROC_SEEING_THRESH,
  ID_PROC_MAX_THRESH,
  ID_PROC_BISP_IR,
  ID_PROC_BISP_NCLOS,
  ID_PROC_SLIT_ANGLE,
  ID_PROC_SPECT_ANGLE,
  ID_PROC_NZ_CUBE,
  ID_PROC_FFTW_DIR,
  ID_PROC_3DFITS_DIR,
  ID_PROC_RESULTS_DIR,

// Check boxes:
  ID_PROC_DIRECT_VECTOR,
  ID_PROC_LUCKY_CHCK,
  ID_PROC_STATS_CHCK,
  ID_PROC_SEEING_SELECT,
  ID_PROC_MAX_SELECT,

// Offset/FField correction:
  ID_PROC_OFFSET_THRESH,
  ID_PROC_OFFSET_FRAME,
  ID_PROC_NO_FFCORR,
  ID_PROC_FFCORR,

// Combo boxes:
  ID_PROC_MODES,
  ID_PROC_KK_CROSSCOR,
  ID_PROC_SAVE_3DFITS,

};

class JLP_SpeckProcessPanel : wxPanel
{

public:

// In "jlp_speckprocess_panel.cpp":

  JLP_SpeckProcessPanel(wxFrame *frame, wxString *str_messg,
                        const int n_messg, bool small_version);
  ~JLP_SpeckProcessPanel(){};

   int SpeckProcessPanelSetup();
   void SpeckProcessPanel_LeftSetup(wxBoxSizer *processing_left_sizer);
   void SpeckProcessPanel_CenterSetup(wxBoxSizer *processing_right_sizer);
   void SpeckProcessPanel_RightSetup(wxBoxSizer *processing_right_sizer);

// Accessors:
   int Get_PSET(PROCESS_SETTINGS &Pset) {
      Copy_PSET(Pset1, Pset);
      return(0);
    };
   int Put_PSET(PROCESS_SETTINGS Pset) {
      Copy_PSET(Pset, Original_Pset1);
      return(0);
    };
   int Get_nz_cube(){return Pset1.nz_cube;}

// Called when changing pages:
   bool ChangesDone(){return(ChangesDone1);}
   bool ValidatedChanges(){return(ValidatedChanges1);}
   void ValidateChanges();
   void CancelNonValidatedChanges();
   int LoadPhotModsqFilenameToPset1();

protected:

// In "jlp_speckprocess_panel_onclick.cpp":
   void UpdateBisp_nclos_FromScreen();
   void UpdateBisp_ir_FromScreen();
   void UpdateSlitAngleFromScreen();
   void UpdateSpectrumAngleFromScreen();
   void UpdateMaxValSelectionFromScreen();
   void UpdateSeeingSelectionFromScreen();
   void Update_nz_cube_FromScreen();
   void UpdateOffsetThreshFromScreen();
   void UpdateOut3DFitsDirFromScreen();
   void UpdateOutResultsDirFromScreen();
   void UpdatefftwDirFromScreen();

   void OnConfig(wxCommandEvent& event);
   void OnSelectProcMode(wxCommandEvent& event);
   void OnBisp_nclosEditExit(wxCommandEvent& event);
   void OnSlitAngle(wxCommandEvent& event);
   void OnSpectrumAngle(wxCommandEvent& event);
   void OnSeeingSelection(wxCommandEvent& event);
   void OnMaxValSelection(wxCommandEvent& event);
   void On_nz_cube(wxCommandEvent& event);
   void OnMaxValSelectionCheckBoxClick(wxCommandEvent& event);
   void OnStatisticsCheckBoxClick(wxCommandEvent& event);
   void OnSeeingSelectionCheckBoxClick(wxCommandEvent& event);
   void OnBisp_irEditExit(wxCommandEvent& event);
   void OnRadGroupOffsetClick(wxCommandEvent& event);
   void OnRadGroupFFieldClick(wxCommandEvent& event);
   void OnOffsetThreshEditExit(wxCommandEvent& event);
   void OnLoadOffsetFrame(wxCommandEvent& event);
   void OnLoadUnresolvedAutocFrame(wxCommandEvent& event);
   void OnLoadUnresolvedModsqFrame(wxCommandEvent& event);
   void OnLoadFFieldFrame(wxCommandEvent& event);
   void OnLoadPhotModsqFrame(wxCommandEvent& event);
   void OnOut3DFitsDirEditExit(wxCommandEvent& event);
   void OnOutResultsDirEditExit(wxCommandEvent& event);
   void OnSaveConfigButtonClick(wxCommandEvent& event);
   void OnLoadConfigButtonClick(wxCommandEvent& event);
   void OnfftwDirEditExit(wxCommandEvent& event);
   void OnSelectOffsetCorr(wxCommandEvent& event);
   void OnSelectFFieldCorr(wxCommandEvent& event);
   void OnSelectSave3DFits(wxCommandEvent& event);
   void OnSelect_KK_CrossCorr(wxCommandEvent& event);
   void OnDirectVectorCheckBoxClick(wxCommandEvent& event);
   void OnLuckyCheckBoxClick(wxCommandEvent& event);

// In "jlp_speckprocess_panel_update.cpp":
   int DisplayNewValues();
   int SaveProcessSettingsDialog();
   int LoadProcessSettingsDialog();
   int SaveProcessSettingsToFile(char *filename, PROCESS_SETTINGS PSet1);
   int LoadProcessSettingsFromFile(char *filename, PROCESS_SETTINGS *ASet1);


private:

  PROCESS_SETTINGS Pset1, Original_Pset1;
  int initialized;
  bool ChangesDone1, ValidatedChanges1;
  wxString *m_messg;

  wxCheckBox *ProcCheck_DirectVector, *ProcCheck_Lucky, *ProcCheck_Stats;
  wxCheckBox *ProcCheck_SeeingSelect, *ProcCheck_MaxSelect;
  wxRadioButton *ProcRad_OffsetThreshold, *ProcRad_OffsetThresholdAndFrame;
  wxRadioButton *ProcRad_NoFFieldCorr, *ProcRad_FFieldCorr;
  wxTextCtrl *ProcCtrl_Bisp_ir, *ProcCtrl_Bisp_nclos;
  wxTextCtrl *ProcCtrl_SlitAngle, *ProcCtrl_SpectAngle;
  wxTextCtrl *ProcCtrl_SeeingThresh, *ProcCtrl_MaxThresh;
  wxTextCtrl *ProcCtrl_nz_cube, *ProcCtrl_OffsetThresh;
  wxTextCtrl *ProcCtrl_FFTW_dir, *ProcCtrl_3DFits_dir, *ProcCtrl_Results_dir;
  wxStaticText *ProcStatic_FFieldFile, *ProcStatic_OffsetFile;
  wxStaticText *ProcStatic_UnresAutocFile, *ProcStatic_UnresModsqFile;
  wxStaticText *ProcStatic_PhotModsqFile;
  JLP_ComboBox ProcCmb_Mode, ProcCmb_Save_3DFits, ProcCmb_KK_CrossCorr;

  DECLARE_EVENT_TABLE()
};

#endif // EOF sentry
