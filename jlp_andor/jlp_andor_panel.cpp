/****************************************************************************
* Name: jlp_andor.cpp
* JLP_AndorPanel class
*
* JLP
* Version 17/07/2015
****************************************************************************/
#include "jlp_andor_panel.h"

// ============================================================================
// implementation
// ============================================================================

BEGIN_EVENT_TABLE(JLP_AndorPanel, wxPanel)

// Buttons:
EVT_BUTTON  (ID_APANEL_CONFIG_OK, JLP_AndorPanel::OnConfig)
EVT_BUTTON  (ID_APANEL_CONFIG_CANCEL, JLP_AndorPanel::OnConfig)
EVT_BUTTON  (ID_APANEL_CONFIG_SAVE, JLP_AndorPanel::OnConfig)
EVT_BUTTON  (ID_APANEL_CONFIG_LOAD, JLP_AndorPanel::OnConfig)
EVT_BUTTON  (ID_APANEL_CONFIG_DEFAULT, JLP_AndorPanel::OnConfig)

// Text controls:
EVT_TEXT  (ID_APANEL_ROI_CENTER, JLP_AndorPanel::OnROICenterOnExit)
EVT_TEXT  (ID_APANEL_COOLER_SETPOINT, JLP_AndorPanel::OnCoolerSetPointEditExit)
EVT_TEXT  (ID_APANEL_EMGAIN, JLP_AndorPanel::OnEMGainEditExit)

EVT_TEXT  (ID_APANEL_KIN_CLOCKTIME, JLP_AndorPanel::OnKineticClockTimeEditExit)
EVT_TEXT  (ID_APANEL_KIN_CYCLETIME, JLP_AndorPanel::OnKineticCycleTimeEditExit)
EVT_TEXT  (ID_APANEL_KIN_EXPOTIME, JLP_AndorPanel::OnKineticExpoTimeEditExit)
EVT_TEXT  (ID_APANEL_KIN_NCYCLE, JLP_AndorPanel::OnKineticNCyclesEditExit)
EVT_TEXT  (ID_APANEL_KIN_NEXP, JLP_AndorPanel::OnKineticCycleNExposuresEditExit)

// Check boxes:
EVT_CHECKBOX (ID_APANEL_COOLER_REGUL, JLP_AndorPanel::OnCoolerCheckBoxClick)
EVT_CHECKBOX (ID_APANEL_EMGAIN_CHK, JLP_AndorPanel::OnEMGainCheckBoxClick)
EVT_CHECKBOX (ID_APANEL_FR_TRANSF, JLP_AndorPanel::OnFrameTransferCheckBoxClick)
EVT_CHECKBOX (ID_APANEL_BLINE_CLAMP, JLP_AndorPanel::OnBaselineClampCheckBoxClick)
EVT_CHECKBOX (ID_APANEL_CLOSESHUTTER0_CHK, JLP_AndorPanel::OnCloseShutterWhenExitCheckBoxClick)

// Combo boxes:
EVT_COMBOBOX (ID_APANEL_ACQUI_MODE, JLP_AndorPanel::OnAcquiModeComboBoxChange)
EVT_COMBOBOX (ID_APANEL_ROI_SIZE, JLP_AndorPanel::OnROISizeComboBoxChange)
EVT_COMBOBOX (ID_APANEL_ROTATION, JLP_AndorPanel::OnRotationComboBoxChange)
EVT_COMBOBOX (ID_APANEL_MIRROR, JLP_AndorPanel::OnMirrorComboBoxChange)
EVT_COMBOBOX (ID_APANEL_TRIGGER, JLP_AndorPanel::OnTriggerComboBoxChange)
EVT_COMBOBOX (ID_APANEL_BIN_FACTOR, JLP_AndorPanel::OnBinFactorComboBoxChange)
EVT_COMBOBOX (ID_APANEL_VSPEED, JLP_AndorPanel::OnVShiftSpeedComboBoxChange)
EVT_COMBOBOX (ID_APANEL_VVOLT, JLP_AndorPanel::OnVShiftVoltageComboBoxChange)
EVT_COMBOBOX (ID_APANEL_HSPEED, JLP_AndorPanel::OnHShiftSpeedComboBoxChange)
EVT_COMBOBOX (ID_APANEL_PREAMP_GAIN, JLP_AndorPanel::OnHShiftPreAmpGainComboBoxChange)
EVT_COMBOBOX (ID_APANEL_ADCHANNEL, JLP_AndorPanel::OnADComboBoxChange)
EVT_COMBOBOX (ID_APANEL_SHUTTER, JLP_AndorPanel::OnShutterComboBoxChange)

END_EVENT_TABLE()

/*******************************************************************************
* Constructor from wxFrame:
*******************************************************************************/
JLP_AndorPanel::JLP_AndorPanel(AspFrame *asp_frame, wxFrame *frame,
                               const int my_wxID, JLP_AndorCam1 *cam1,
                               JLP_CAMERA_PANEL_CHANGES *jcam_changes0,
                               wxString *str_messg, const int n_messg,
                               bool small_version)
                               : wxPanel( frame, my_wxID)
{
bool update_from_screen;
wxString input_filename;
int k;

 initialized = 0;
 m_aspframe = asp_frame;
// JLP2017: should be true by default for Andor(see UpdateAcquisitionMode()...)
 ExpertMode = true;

// Reset changes to false
 jcam_changes1 = jcam_changes0;
 jcam_changes1->ChangesDone = false;
 jcam_changes1->ChangesValidated = false;

// Transform coma into point for numbers:
 setlocale(LC_NUMERIC, "C");

// Smaller fonts (needed for Windows, to have a similar appearance as Linux)
   if(small_version) SetWindowVariant(wxWINDOW_VARIANT_SMALL);

// Messages (in different languages)
 m_messg = new wxString[n_messg];
 for(k = 0; k < n_messg; k++) m_messg[k] = str_messg[k];

// Get information from JLP_AndorCam1 object:
 m_cam1 = cam1;

// Set current values of Andor settings:
 m_cam1->Cam1_Get_AndorSettings(&AndorSet1);

 andor_not_connected = m_cam1->NotConnected();

// Setup panel
 AndorPanel_Setup();

// Copy AndorSet1 to  "original" AndorSet1:
 Copy_AndorSettings(AndorSet1, &Original_AndorSet1);

 initialized = 1234;

// Update display (after setting initialized to 1234) :
 update_from_screen = false;
 DisplayNewSettings(update_from_screen);

// Update Enable status according to the selected acauisition mode:
 UpdateAcquisitionMode();

return;
}
/********************************************************************
* Andor Setup panel
********************************************************************/
void JLP_AndorPanel::MyFreeMemory()
{
// Error here so I leave it allocated:
// if(m_messg != NULL) delete m_messg;
}
/********************************************************************
* Andor Setup panel
********************************************************************/
int JLP_AndorPanel::AndorPanel_Setup()
{
wxBoxSizer *w_topsizer, *w_hsizer1, *w_hsizer2;
wxBoxSizer *andor_left_sizer, *andor_right_sizer;
wxButton *AndorOKButton, *AndorCancelButton, *AndorSaveButton, *AndorLoadButton;
wxButton *AndorDefaultButton;
int status;

status = -1;
if(!andor_not_connected)
  status = InitComboBoxItems_with_Andor();

if(status) InitComboBoxItems_without_Andor();

  w_topsizer = new wxBoxSizer(wxVERTICAL);

  w_hsizer1 = new wxBoxSizer( wxHORIZONTAL );

// Create camera setup:
  andor_left_sizer = new wxBoxSizer(wxVERTICAL);
  AndorPanel_LeftSetup(andor_left_sizer);
  w_hsizer1->Add(andor_left_sizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 6);

  andor_right_sizer = new wxBoxSizer(wxVERTICAL);
  AndorPanel_RightSetup(andor_right_sizer);
  w_hsizer1->Add(andor_right_sizer, 0, wxALIGN_CENTRE_VERTICAL);

  w_topsizer->Add(w_hsizer1, 0, wxALIGN_CENTER);

///////////////////////////////////////////////////////////////////////
// Create five buttons at bottom:
// Validate, Standard value, Cancel, Save config, Load config
 w_hsizer2 = new wxBoxSizer( wxHORIZONTAL );

// i=200 "Validate"
 AndorOKButton = new wxButton(this, ID_APANEL_CONFIG_OK, m_messg[200]);
// i=201 "Cancel"
 AndorCancelButton = new wxButton(this, ID_APANEL_CONFIG_CANCEL, m_messg[201]);
// i=202 "Save the config"
 AndorSaveButton = new wxButton(this, ID_APANEL_CONFIG_SAVE, m_messg[202]);
// i=203 "Load a config"
 AndorLoadButton = new wxButton(this, ID_APANEL_CONFIG_LOAD, m_messg[203]);
// i=204 "Standard values"
 AndorDefaultButton = new wxButton(this, ID_APANEL_CONFIG_DEFAULT, m_messg[204]);

// Add buttons, horizontally unstretchable, with minimal size:
 w_hsizer2->Add( AndorOKButton, 0);
 w_hsizer2->Add( AndorCancelButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( AndorSaveButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( AndorLoadButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( AndorDefaultButton, 0, wxLEFT | wxRIGHT, 20);

// Add button sizer with minimal size:
  w_topsizer->Add(w_hsizer2, 0, wxALIGN_CENTER | wxALL, 10 );

  this->SetSizer(w_topsizer);

  Centre();

return(0);
}
/********************************************************************
* Andor Setup panel (left side)
********************************************************************/
void JLP_AndorPanel::AndorPanel_LeftSetup(wxBoxSizer *andor_left_sizer)
{
wxStaticBoxSizer *geom_sizer, *acqui_sizer;
int i, isize0, irows, icols, vgap = 12, hgap = 12, wwidth = 160;
wxFlexGridSizer *fgs1, *fgs2;
wxString str0;

// *************** Image geometry: ***************************************

// Sizer surrounded with a rectangle, with a title on top:
// i=205 "Image geometry"
 geom_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[205]);
 irows = 6;
 icols = 2;
 wwidth = 100;
 fgs1 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// ROI image format:
// i=206 "ROI size"
 AndorCmb_ROISize.label = m_messg[206];
// 64, 128, 256, 512, full size
 isize0 = 32;
  for(i = 0; i < 4; i++) {
   isize0 *= 2;
   str0.Printf(wxT("%dx%d"), isize0, isize0);
   AndorCmb_ROISize.choices[i] = str0;
   }
 str0.Printf(wxT("%dx%d"), AndorSet1.gblXPixels, AndorSet1.gblYPixels);
 AndorCmb_ROISize.choices[4] = str0;
 AndorCmb_ROISize.nchoices = 5;
 AndorCmb_ROISize.combo = new wxComboBox(this, ID_APANEL_ROI_SIZE, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             AndorCmb_ROISize.nchoices,
                                             AndorCmb_ROISize.choices);
 fgs1->Add(new wxStaticText(this, -1, AndorCmb_ROISize.label));
 fgs1->Add(AndorCmb_ROISize.combo);

// Image rotation (to have north on top, for example)
// Values: 0 (no rotation), 1 (+90 degrees), 2 (+180 degrees) or 3 (+270 degrees)
// i=207 "Rotation (deg)"
 AndorCmb_Rotation.label = m_messg[207];
 AndorCmb_Rotation.nchoices = 4;
 AndorCmb_Rotation.choices[0] = wxT("0");
 AndorCmb_Rotation.choices[1] = wxT("90");
 AndorCmb_Rotation.choices[2] = wxT("+180");
 AndorCmb_Rotation.choices[3] = wxT("+270");
 AndorCmb_Rotation.combo = new wxComboBox(this, ID_APANEL_ROTATION, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             AndorCmb_Rotation.nchoices,
                                             AndorCmb_Rotation.choices);
 fgs1->Add(new wxStaticText(this, -1, AndorCmb_Rotation.label));
 fgs1->Add(AndorCmb_Rotation.combo);

// Image compression
// Values: 1, 2, 4 or 8
// i=208 "Binning"
 AndorCmb_BinFactor.label = m_messg[208];
 AndorCmb_BinFactor.nchoices = 4;
 AndorCmb_BinFactor.choices[0] = wxT("1x1");
 AndorCmb_BinFactor.choices[1] = wxT("2x2");
 AndorCmb_BinFactor.choices[2] = wxT("4x4");
 AndorCmb_BinFactor.choices[3] = wxT("8x8");
 AndorCmb_BinFactor.combo = new wxComboBox(this, ID_APANEL_BIN_FACTOR, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             AndorCmb_BinFactor.nchoices,
                                             AndorCmb_BinFactor.choices);
 fgs1->Add(new wxStaticText(this, -1, AndorCmb_BinFactor.label));
 fgs1->Add(AndorCmb_BinFactor.combo);

// Mirror effect (to have North-East orientation for instance)
// Values: 0 (no symmetry), 1 (vertical mirror), 2 (horizontal mirror), 3 (vertical and horizontal)
// i=209 "Mirror effect"
 AndorCmb_Mirror.label = m_messg[209];
 AndorCmb_Mirror.nchoices = 4;
// i=210 "None"
 AndorCmb_Mirror.choices[0] = m_messg[210];
// i=211 "Vertical"
 AndorCmb_Mirror.choices[1] = m_messg[211];
// i=212 "Horizontal"
 AndorCmb_Mirror.choices[2] = m_messg[212];
// i=261 "Hori. and vert."
 AndorCmb_Mirror.choices[3] = m_messg[261];
 AndorCmb_Mirror.combo = new wxComboBox(this, ID_APANEL_MIRROR, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             AndorCmb_Mirror.nchoices,
                                             AndorCmb_Mirror.choices);
 fgs1->Add(new wxStaticText(this, -1, AndorCmb_Mirror.label));
 fgs1->Add(AndorCmb_Mirror.combo);

// i=213 "ROI center"
 fgs1->Add(new wxStaticText(this, -1, m_messg[213]));
 AndorCtrl_ROICenter = new wxTextCtrl(this, ID_APANEL_ROI_CENTER, wxT("256,256"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
 fgs1->Add(AndorCtrl_ROICenter);
 AndorStatic_DetectorFormat = new wxStaticText(this, -1,
                                       _T("Detector: 512x512 pixels"));
 fgs1->Add(AndorStatic_DetectorFormat);
 AndorStatic_ImageFormat = new wxStaticText(this, -1,
                                       _T("Images: 128x128 pixels"));
 fgs1->Add(AndorStatic_ImageFormat);
 geom_sizer->Add(fgs1, 0, wxALL, 10);
 andor_left_sizer->Add(geom_sizer, 0, wxEXPAND | wxLEFT | wxBOTTOM, 10);

// *************** Acquisition mode: ***************************************

// i=214 "Acquisition settings"
 acqui_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[214]);
 irows = 3;
 icols = 2;
 wwidth = 160;
 fgs2 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=215 "Mode"
 AndorCmb_AcquiMode.label = m_messg[215];
 AndorCmb_AcquiMode.nchoices = 5;
// i=216 "Long exposure"
 AndorCmb_AcquiMode.choices[0] = m_messg[216];
// i=217 "Video (continuous)"
 AndorCmb_AcquiMode.choices[1] = m_messg[217];
// i=218 "Sequence of short exp."
 AndorCmb_AcquiMode.choices[2] = m_messg[218];
// i=219 "Accumulation of short exp."
 AndorCmb_AcquiMode.choices[3] = m_messg[219];
// i=217 "Video (continuous)"
 AndorCmb_AcquiMode.choices[4] = m_messg[217] + wxT(" (Expert Mode)");
 AndorCmb_AcquiMode.combo = new wxComboBox(this, ID_APANEL_ACQUI_MODE, wxT(""),
                                         wxDefaultPosition, wxSize(wwidth, 28),
                                         AndorCmb_AcquiMode.nchoices,
                                         AndorCmb_AcquiMode.choices);
 fgs2->Add(new wxStaticText(this, -1, AndorCmb_AcquiMode.label));
 fgs2->Add(AndorCmb_AcquiMode.combo);

// *************** Trigger, shutter, temp. regul: *****************************

// Trigger and shutter:

// Trigger
// 0 (internal), 1 (external) or  10  (software)
// i=220 "Trigger"
 AndorCmb_Trigger.label = m_messg[220];
 AndorCmb_Trigger.nchoices = 3;
// i=221 "Internal"
 AndorCmb_Trigger.choices[0] = m_messg[221];
// i=222 "External"
 AndorCmb_Trigger.choices[1] = m_messg[222];
// i=223 "Software"
 AndorCmb_Trigger.choices[2] = m_messg[223];
 AndorCmb_Trigger.combo = new wxComboBox(this, ID_APANEL_TRIGGER, wxT(""),
                                         wxDefaultPosition, wxSize(wwidth, 28),
                                         AndorCmb_Trigger.nchoices,
                                         AndorCmb_Trigger.choices);
 fgs2->Add(new wxStaticText(this, -1, AndorCmb_Trigger.label));
 fgs2->Add(AndorCmb_Trigger.combo);

/** Shutter
* Values of AndorSet1.ShutterMode:
* -1 not available, 0 (automatic), 1 (open) or  2  (closed),
* Corresponding values of combobox selection: 0, 1, 2, 3
**/
// i=224 "Shutter"
 AndorCmb_Shutter.label = m_messg[224];
 AndorCmb_Shutter.nchoices = 4;
// i=225 "Unavailable"
 AndorCmb_Shutter.choices[0] = m_messg[225];
// i=226 "Automatic"
 AndorCmb_Shutter.choices[1] = m_messg[226];
// i=227 "Kept open"
 AndorCmb_Shutter.choices[2] = m_messg[227];
// i=228 "Kept closed"
 AndorCmb_Shutter.choices[3] = m_messg[228];
 AndorCmb_Shutter.combo = new wxComboBox(this, ID_APANEL_SHUTTER, wxT(""),
                                         wxDefaultPosition, wxSize(wwidth, 28),
                                         AndorCmb_Shutter.nchoices,
                                         AndorCmb_Shutter.choices);
 fgs2->Add(new wxStaticText(this, -1, AndorCmb_Shutter.label),
             0, wxBOTTOM, 10);
 fgs2->Add(AndorCmb_Shutter.combo);

 acqui_sizer->Add(fgs2, 0, wxALL, 10);

 andor_left_sizer->Add(acqui_sizer, 0, wxALL , 5);

}
/********************************************************************
* Andor Setup panel (right side)
********************************************************************/
void JLP_AndorPanel::AndorPanel_RightSetup(wxBoxSizer *andor_right_sizer)
{
wxStaticBoxSizer *VShift_sizer, *Kin_sizer, *Read_sizer;
wxStaticBoxSizer *EMGain_sizer, *ShutdownOptions_sizer, *temp_sizer;
wxBoxSizer *hsizer1, *vsizer1, *vsizer2;
int irows, icols, vgap = 10, hgap = 12, wwidth = 80;
wxFlexGridSizer *fgs1, *fgs2, *fgs3, *fgs4, *fgs5, *fgs6;

// Vertical shift:
hsizer1 = new wxBoxSizer(wxHORIZONTAL);
vsizer1 = new wxBoxSizer(wxVERTICAL);
// i=229 "Vertical shift"
VShift_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[229]);
irows = 2;
icols = 2;
vgap = 20;
hgap = 12;
fgs1 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// Vertical speed
// i=230 "Vshift speed"
 AndorCmb_VShiftSpeed.label = m_messg[230];
 AndorCmb_VShiftSpeed.combo = new wxComboBox(this, ID_APANEL_VSPEED, wxT(""),
                                             wxDefaultPosition, wxSize(160, 28),
                                             AndorCmb_VShiftSpeed.nchoices,
                                             AndorCmb_VShiftSpeed.choices);
 fgs1->Add(new wxStaticText(this, -1, AndorCmb_VShiftSpeed.label));
 fgs1->Add(AndorCmb_VShiftSpeed.combo);

//  Clock Vshift voltage : not available for LUCA:
// i=231 "Clock vshift voltage" (Tension d'horloge)
 AndorCmb_VShiftVolt.label = m_messg[231];
 if(AndorCmb_VShiftVolt.nchoices > 0) {
 AndorCmb_VShiftVolt.combo = new wxComboBox(this, ID_APANEL_VVOLT, wxT(""),
                                             wxDefaultPosition, wxSize(160, 28),
                                             AndorCmb_VShiftVolt.nchoices,
                                             AndorCmb_VShiftVolt.choices);
 fgs1->Add(new wxStaticText(this, -1, AndorCmb_VShiftVolt.label));
 fgs1->Add(AndorCmb_VShiftVolt.combo);
 } else {
 AndorCmb_VShiftVolt.combo = NULL;
 }
VShift_sizer->Add(fgs1, 0, wxALL, 10);
vsizer1->Add(VShift_sizer, 0, wxALL, 10);

// Kinetic mode:
// i=232 "Kinetic mode"
Kin_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[232]);
irows = 6;
icols = 2;
vgap = 20;
hgap = 12;
fgs2 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=233 "Frame exposure time (msec)"
fgs2->Add(new wxStaticText(this, -1, m_messg[233]));
AndorCtrl_KinExpoTime = new wxTextCtrl(this, ID_APANEL_KIN_EXPOTIME, wxT("20"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
fgs2->Add(AndorCtrl_KinExpoTime);
// i=234 "Clock time (= frame period) (msec)"
fgs2->Add(new wxStaticText(this, -1, m_messg[234]));
AndorCtrl_KinClockTime = new wxTextCtrl(this, ID_APANEL_KIN_CLOCKTIME, wxT("40"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
fgs2->Add(AndorCtrl_KinClockTime);
// i=235 "Number of frames/cycle"
fgs2->Add(new wxStaticText(this, -1, m_messg[235]));
AndorCtrl_KinNExp = new wxTextCtrl(this, ID_APANEL_KIN_NEXP, wxT("1"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
fgs2->Add(AndorCtrl_KinNExp);
// i=236 "Cycle time (= cycle period) (msec)"
fgs2->Add(new wxStaticText(this, -1, m_messg[236]));
AndorCtrl_KinCycleTime = new wxTextCtrl(this, ID_APANEL_KIN_CYCLETIME, wxT("40"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
fgs2->Add(AndorCtrl_KinCycleTime);
// i=237 "Number of cycles"
fgs2->Add(new wxStaticText(this, -1, m_messg[237]));
AndorCtrl_KinNCycle = new wxTextCtrl(this, ID_APANEL_KIN_NCYCLE, wxT("1000"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
fgs2->Add(AndorCtrl_KinNCycle);
// i=238 "Integration time (sec)"
fgs2->Add(new wxStaticText(this, -1, m_messg[238]));
AndorCtrl_LongExpoTime = new wxTextCtrl(this, wxID_ANY, wxT("10"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
fgs2->Add(AndorCtrl_LongExpoTime);

Kin_sizer->Add(fgs2, 0, wxALL, 10);
vsizer1->Add(Kin_sizer, 0, wxEXPAND | wxALL, 10);

hsizer1->Add(vsizer1, 0, wxRIGHT, 10);

// Read outRegister:
vsizer2 = new wxBoxSizer(wxVERTICAL);
// i=239 "Read-out register"
Read_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[239]);
irows = 3;
icols = 2;
vgap = 20;
hgap = 12;
wwidth = 100;
fgs3 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// OutRegister read speed
// i=240 "H-shift speed"
 AndorCmb_HShiftSpeed.label = m_messg[240];
 AndorCmb_HShiftSpeed.combo = new wxComboBox(this, ID_APANEL_HSPEED, wxT(""),
                                             wxDefaultPosition,
                                             wxSize(wwidth, 28),
                                             AndorCmb_HShiftSpeed.nchoices,
                                             AndorCmb_HShiftSpeed.choices);
 fgs3->Add(new wxStaticText(this, -1, AndorCmb_HShiftSpeed.label));
 fgs3->Add(AndorCmb_HShiftSpeed.combo);

// OutRegister preampli gain:
// i=241 "Preampli. gain"
 AndorCmb_PreAmpGain.label = m_messg[241];
 AndorCmb_PreAmpGain.combo = new wxComboBox(this, ID_APANEL_PREAMP_GAIN,
                                             wxT(""), wxDefaultPosition,
                                             wxSize(wwidth, 28),
                                             AndorCmb_PreAmpGain.nchoices,
                                             AndorCmb_PreAmpGain.choices);
 fgs3->Add(new wxStaticText(this, -1, AndorCmb_PreAmpGain.label));
 fgs3->Add(AndorCmb_PreAmpGain.combo);

// OutRegister A/D channel:
// i=242 "A/D channel"
 AndorCmb_ADChannel.label = m_messg[242];
 AndorCmb_ADChannel.combo = new wxComboBox(this, ID_APANEL_ADCHANNEL, wxT(""),
                                           wxDefaultPosition,
                                           wxSize(wwidth, 28),
                                           AndorCmb_ADChannel.nchoices,
                                           AndorCmb_ADChannel.choices);
 fgs3->Add(new wxStaticText(this, -1, AndorCmb_ADChannel.label));
 fgs3->Add(AndorCmb_ADChannel.combo);

Read_sizer->Add(fgs3, 0, wxALL, 10);
vsizer2->Add(Read_sizer, 0, wxTOP | wxBOTTOM, 10);

// EMCCD gain:
// i=243 "EMCCD gain"
EMGain_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[243]);
irows = 1;
icols = 4;
vgap = 20;
hgap = 12;
fgs4 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=244 "Activated"
AndorCheck_EMCCD = new wxCheckBox(this, ID_APANEL_EMGAIN_CHK, m_messg[244]);
fgs4->Add(AndorCheck_EMCCD, 0);
fgs4->Add(new wxStaticText(this, -1, wxT("")), 0, wxLEFT, 20);

// i=245 "EMGain"
fgs4->Add(new wxStaticText(this, -1, m_messg[245]));
AndorCtrl_EMGain = new wxTextCtrl(this, ID_APANEL_EMGAIN, wxT("3000"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
fgs4->Add(AndorCtrl_EMGain);

EMGain_sizer->Add(fgs4, 0, wxALL, 10);
vsizer2->Add(EMGain_sizer, 0, wxEXPAND | wxBOTTOM, 10);


// Cooler settings:
// i=246 "Cooler control"
 temp_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[246]);
 irows = 2;
 icols = 2;
 fgs6 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=244 "Activated"
 AndorCheck_CoolerRegul = new wxCheckBox(this, ID_APANEL_COOLER_REGUL,
                                         m_messg[244]);
 AndorCtrl_CoolerSetPoint = new wxTextCtrl(this, ID_APANEL_COOLER_SETPOINT,
                                           wxT("-76"),
                                           wxPoint(-1, -1), wxSize(wwidth, 28));
 fgs6->Add(AndorCheck_CoolerRegul, 0, wxBOTTOM, 6);
 fgs6->Add(new wxStaticText(this, -1, wxT("")));
// i=247 "Set point (deg. C)"
 fgs6->Add(new wxStaticText(this, -1, m_messg[247]));
 fgs6->Add(AndorCtrl_CoolerSetPoint);
 temp_sizer->Add(fgs6, 0, wxALL, 10);
 vsizer2->Add(temp_sizer, 0, wxEXPAND | wxBOTTOM, 10);

// Miscellaneous: baseline clamp and frame transfer
irows = 1;
icols = 2;
fgs5 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=248 "Baseline clamp"
AndorCheck_BaselineClamp = new wxCheckBox(this, ID_APANEL_BLINE_CLAMP,
                                          m_messg[248]);
fgs5->Add(AndorCheck_BaselineClamp, 0, wxRIGHT, 10);

// i=249 "Frame transfer"
AndorCheck_FrameTransfer = new wxCheckBox(this, ID_APANEL_FR_TRANSF,
                                          m_messg[249]);
fgs5->Add(AndorCheck_FrameTransfer, 0, wxLEFT, 20);

vsizer2->Add(fgs5, 0, wxTOP, 10);

// CloseShutterWhenExit(true) Check
ShutdownOptions_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[270]);
/*
AndorStatic_ShutdownOptions = new wxStaticText(this, -1, m_messg[270]);
*/
AndorCheck_CloseShutterWhenExit = new wxCheckBox(this, ID_APANEL_CLOSESHUTTER0_CHK,
                                                 m_messg[271]);
ShutdownOptions_sizer->Add(AndorCheck_CloseShutterWhenExit, 0, wxALL, 10);

vsizer2->Add(ShutdownOptions_sizer, 0, wxALL, 10);

hsizer1->Add(vsizer2, 0, wxRIGHT, 10);

andor_right_sizer->Add(hsizer1, 0, wxALL, 5);
}
/********************************************************************
* Initialization of combo bowes without Andor camera
********************************************************************/
void JLP_AndorPanel::InitComboBoxItems_without_Andor()
{
// Image format:
 AndorCmb_ROISize.nchoices = 5;
 AndorCmb_ROISize.choices[0] = wxT("64x64");
 AndorCmb_ROISize.choices[1] = wxT("128x128");
 AndorCmb_ROISize.choices[2] = wxT("256x256");
 AndorCmb_ROISize.choices[3] = wxT("512x512");
// i=260 "Full frame"
 AndorCmb_ROISize.choices[4] = m_messg[260];

// Vertical speed
 AndorCmb_VShiftSpeed.nchoices = 1;
 AndorCmb_VShiftSpeed.choices[0] = wxT("0.5000");

//  Clock Vshift voltage
 AndorCmb_VShiftVolt.nchoices = 1;
 AndorCmb_VShiftVolt.choices[0] = wxT("+2");

// Outregister read speed
 AndorCmb_HShiftSpeed.nchoices = 1;
 AndorCmb_HShiftSpeed.choices[0] = wxT("5.0 MHz");

// Outregister preampli gain:
 AndorCmb_PreAmpGain.nchoices = 1;
 AndorCmb_PreAmpGain.choices[0] = wxT("2.4 x");

// Outregister A/D channel:
 AndorCmb_ADChannel.nchoices = 1;
 AndorCmb_ADChannel.choices[0] = wxT("0");
}
/****************************************************************************
* Initialization of the items of the ComboBoxes
****************************************************************************/
int JLP_AndorPanel::InitComboBoxItems_with_Andor()
{
float fspeed, fgain;
int conventional_mode, i, k, index, errorValue, status = 0;
wxString buffer;

  if(andor_not_connected) return(-1);

// ROI size into ROISizeComboBox
// 64, 128, 256, 512, 960:
  k = 0;
  i = 64;
  while(i < AndorSet1.gblXPixels) {
    AndorCmb_ROISize.choices[k].Printf("%dx%d", i, i);
    i *= 2; k++;
    if(i == 1024) i = 960;
    }
// Last item:
// i=260 "Full frame"
    AndorCmb_ROISize.choices[k] = m_messg[260];
    AndorCmb_ROISize.nchoices = k + 1;

// Create the items of VShiftSpeed ComboBox
    errorValue = GetNumberVSSpeeds(&index);
    if(errorValue != DRV_SUCCESS){
      wxMessageBox(wxT("Error in GetNumberVSSpeeds"), wxT("InitComboBoxItems"),
                   wxOK | wxICON_ERROR);
      status = -1;
    } else {
      AndorCmb_VShiftSpeed.nchoices = index;
      for(i = 0; i < index; i++){
        GetVSSpeed(i, &fspeed);
        AndorCmb_VShiftSpeed.choices[i].Printf("%.4f", fspeed);
      }
    }

// Create the items of VShift Voltage Amplitude ComboBox
    errorValue = GetNumberVSAmplitudes(&index);
    if(errorValue != DRV_SUCCESS){
      buffer = wxT("Error in GetNumberVSAmplitudes\n");
      if(errorValue == DRV_NOT_AVAILABLE) {
       buffer.Append(wxT("VShift Voltage number is not available for this camera"));
       wxMessageBox(buffer, wxT("InitComboBoxItems"), wxOK | wxICON_ERROR);
       }
      AndorCmb_VShiftVolt.nchoices = 0;
    } else {
      AndorCmb_VShiftVolt.nchoices = index;
      AndorCmb_VShiftVolt.choices[0] = _T("Normal");
      for(i = 1; i < index; i++){
        AndorCmb_VShiftVolt.choices[i].Printf("+%d", i);
      }
    }

// Create the items of HShiftSpeed ComboBox
// LinearMode when EMGainMode = 2:
    conventional_mode = (AndorSet1.EMGainMode == 2) ? 1 : 0;
    errorValue = GetNumberHSSpeeds(AndorSet1.AD_Channel, conventional_mode,
                                   &index);
    if(errorValue != DRV_SUCCESS){
      wxMessageBox(wxT("Error in GetNumberHSSpeeds"), wxT("InitComboBoxItems"),
                   wxOK | wxICON_ERROR);
      status = -1;
    } else {
      AndorCmb_HShiftSpeed.nchoices = index;
      for(i = 0; i < index; i++){
        buffer = "";
        GetHSSpeed(AndorSet1.AD_Channel, conventional_mode, i, &fspeed);

// If using an iXon the speed is given in MHz
       if(AndorSet1.AndorCaps.ulCameraType == 1)
         AndorCmb_HShiftSpeed.choices[i].Printf("%.1f MHz", fspeed);
       else
         AndorCmb_HShiftSpeed.choices[i].Printf("%.4f microsec/pixel shift",fspeed);
      }
    }

// Create the items of HShift preampli Gain ComboBox
// Linear mode when GaimMode = 2
    conventional_mode = (AndorSet1.EMGainMode == 2) ? 1 : 0;
    errorValue = GetNumberPreAmpGains(&index);
    if(errorValue != DRV_SUCCESS){
      wxMessageBox(wxT("Error in GetNumberPreAmpGains"),
                   wxT("InitComboBoxItems"), wxOK | wxICON_ERROR);
      status = -1;
    } else {
      AndorCmb_PreAmpGain.nchoices = index;
      for(i = 0; i < index; i++){
        GetPreAmpGain(i, &fgain);
        AndorCmb_PreAmpGain.choices[i].Printf(wxT("%.1f x"), fgain);
      }
    }

// Create the items of AD ComboBox
    errorValue = GetNumberADChannels(&index);
    if(errorValue != DRV_SUCCESS){
      wxMessageBox(wxT("Error in GetNumberADChannels"),
                   wxT("InitComboBoxItems"), wxOK | wxICON_ERROR);
      status = -1;
    } else {
      AndorCmb_ADChannel.nchoices = index;
      for(i = 0; i < index; i++){
        AndorCmb_ADChannel.choices[i].Printf(wxT("%d"), i);
      }
    }

return(status);
}
