/****************************************************************************
* Name: jlp_raptor.cpp
* JLP_RaptorPanel class
*
* JLP
* Version 17/07/2015
****************************************************************************/
#include "jlp_raptor_panel.h"

// ============================================================================
// implementation
// ============================================================================

BEGIN_EVENT_TABLE(JLP_RaptorPanel, wxPanel)

// Buttons:
EVT_BUTTON  (ID_RPANEL_CONFIG_OK, JLP_RaptorPanel::OnConfig)
EVT_BUTTON  (ID_RPANEL_CONFIG_CANCEL, JLP_RaptorPanel::OnConfig)
EVT_BUTTON  (ID_RPANEL_CONFIG_SAVE, JLP_RaptorPanel::OnConfig)
EVT_BUTTON  (ID_RPANEL_CONFIG_LOAD, JLP_RaptorPanel::OnConfig)
EVT_BUTTON  (ID_RPANEL_CONFIG_DEFAULT, JLP_RaptorPanel::OnConfig)
EVT_BUTTON  (ID_RPANEL_CURRENT_STATUS, JLP_RaptorPanel::OnCurrentStatus)

// Text controls:
EVT_TEXT  (ID_RPANEL_ROI_CENTER, JLP_RaptorPanel::OnROICenterOnExit)
EVT_TEXT  (ID_RPANEL_COOLER_SETPOINT, JLP_RaptorPanel::OnCoolerSetPointEditExit)
EVT_TEXT  (ID_RPANEL_DIGITAL_GAIN, JLP_RaptorPanel::OnDigitalGainEditExit)
EVT_TEXT  (ID_RPANEL_TRIGGER_DELAY, JLP_RaptorPanel::OnTriggerDelayOnExit)
EVT_TEXT  (ID_RPANEL_EXPO_TIME, JLP_RaptorPanel::OnExpoTimeOnExit)
EVT_TEXT  (ID_RPANEL_INTEG_TIME, JLP_RaptorPanel::OnIntegTimeOnExit)

// Check boxes:
EVT_CHECKBOX (ID_RPANEL_COOLER_REGUL, JLP_RaptorPanel::OnCoolerCheckBoxClick)
EVT_CHECKBOX (ID_RPANEL_COOLER_FAN, JLP_RaptorPanel::OnCoolerFanCheckBoxClick)
EVT_CHECKBOX (ID_RPANEL_HIGH_GAIN_CHK, JLP_RaptorPanel::OnHighGainCheckBoxClick)
EVT_CHECKBOX (ID_RPANEL_IMAGE_SHARPEN, JLP_RaptorPanel::OnImageSharpenCheckBoxClick)
EVT_CHECKBOX (ID_RPANEL_AUTO_EXPOSURE, JLP_RaptorPanel::OnAutoExposureCheckBoxClick)

// Combo boxes:
EVT_COMBOBOX (ID_RPANEL_ACQUI_MODE, JLP_RaptorPanel::OnAcquiModeComboBoxChange)
EVT_COMBOBOX (ID_RPANEL_ROI_SIZE, JLP_RaptorPanel::OnROISizeComboBoxChange)
EVT_COMBOBOX (ID_RPANEL_ROTATION, JLP_RaptorPanel::OnRotationComboBoxChange)
EVT_COMBOBOX (ID_RPANEL_MIRROR, JLP_RaptorPanel::OnMirrorComboBoxChange)
EVT_COMBOBOX (ID_RPANEL_EXT_TRIGGER, JLP_RaptorPanel::OnExtTriggerComboBoxChange)
EVT_COMBOBOX (ID_RPANEL_BIN_FACTOR, JLP_RaptorPanel::OnBinFactorComboBoxChange)
EVT_COMBOBOX (ID_RPANEL_FRAME_RATE, JLP_RaptorPanel::OnFrameRateComboBoxChange)
EVT_COMBOBOX (ID_RPANEL_NUC_STATE, JLP_RaptorPanel::OnNucStateComboBoxChange)
EVT_COMBOBOX (ID_RPANEL_SHUTTER, JLP_RaptorPanel::OnShutterComboBoxChange)


END_EVENT_TABLE()

/*******************************************************************************
* Constructor from wxFrame:
*******************************************************************************/
JLP_RaptorPanel::JLP_RaptorPanel(AspFrame *asp_frame, wxFrame *frame,
                               const int my_wxID, JLP_RaptorCam1 *cam1,
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

// Get information from JLP_RaptorCam1 object:
 m_cam1 = cam1;

// Get current values of raptor settings:
 m_cam1->Get_RaptorSettings(&RaptorSet1);

// Copy RaptorSet1 to  "original" RaptorSet1:
 Copy_RaptorSettings(RaptorSet1, &Original_RaptorSet1);

// Setup panel
 RaptorPanel_Setup();

 initialized = 1234;

// Update display (after setting initialized to 1234) :
 update_from_screen = false;
 DisplayNewSettings(update_from_screen);

// Update Enable status according to the selected acauisition mode:
 UpdateAcquisitionMode();

return;
}
/********************************************************************
* Raptor Setup panel
********************************************************************/
void JLP_RaptorPanel::MyFreeMemory()
{
// Error here so I leave it allocated:
// if(m_messg != NULL) delete m_messg;
}
/********************************************************************
* Raptor Setup panel
********************************************************************/
int JLP_RaptorPanel::RaptorPanel_Setup()
{
wxBoxSizer *w_topsizer, *w_hsizer1, *w_hsizer2;
wxBoxSizer *raptor_left_sizer, *raptor_cent_sizer, *raptor_right_sizer;
wxButton *RaptorOKButton, *RaptorCancelButton, *RaptorSaveButton, *RaptorLoadButton;
wxButton *RaptorDefaultButton;
int status;

status = -1;
if(!m_cam1->NotConnected())
  status = InitComboBoxItems_with_Raptor();

if(status) InitComboBoxItems_without_Raptor();

  w_topsizer = new wxBoxSizer(wxVERTICAL);

  w_hsizer1 = new wxBoxSizer( wxHORIZONTAL );

// Create camera setup:
  raptor_left_sizer = new wxBoxSizer(wxVERTICAL);
  RaptorPanel_LeftSetup(raptor_left_sizer);
  w_hsizer1->Add(raptor_left_sizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 6);

  raptor_cent_sizer = new wxBoxSizer(wxVERTICAL);
  RaptorPanel_CentSetup(raptor_cent_sizer);
  w_hsizer1->Add(raptor_cent_sizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 6);

  raptor_right_sizer = new wxBoxSizer(wxVERTICAL);
  RaptorPanel_RightSetup(raptor_right_sizer);
  w_hsizer1->Add(raptor_right_sizer, 0, wxALIGN_CENTRE_VERTICAL);

  w_topsizer->Add(w_hsizer1, 0, wxALIGN_CENTER);

///////////////////////////////////////////////////////////////////////
// Create five buttons at bottom:
// Validate, Standard value, Cancel, Save config, Load config
 w_hsizer2 = new wxBoxSizer( wxHORIZONTAL );

// i=200 "Validate"
 RaptorOKButton = new wxButton(this, ID_RPANEL_CONFIG_OK, m_messg[200]);
// i=201 "Cancel"
 RaptorCancelButton = new wxButton(this, ID_RPANEL_CONFIG_CANCEL, m_messg[201]);
// i=202 "Save the config"
 RaptorSaveButton = new wxButton(this, ID_RPANEL_CONFIG_SAVE, m_messg[202]);
// i=203 "Load a config"
 RaptorLoadButton = new wxButton(this, ID_RPANEL_CONFIG_LOAD, m_messg[203]);
// i=204 "Standard values"
 RaptorDefaultButton = new wxButton(this, ID_RPANEL_CONFIG_DEFAULT, m_messg[204]);

// Add buttons, horizontally unstretchable, with minimal size:
 w_hsizer2->Add( RaptorOKButton, 0);
 w_hsizer2->Add( RaptorCancelButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( RaptorSaveButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( RaptorLoadButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( RaptorDefaultButton, 0, wxLEFT | wxRIGHT, 20);

// Add button sizer with minimal size:
  w_topsizer->Add(w_hsizer2, 0, wxALIGN_CENTER | wxALL, 10 );

  this->SetSizer(w_topsizer);

  Centre();

return(0);
}
/********************************************************************
* Raptor Setup panel (left side)
********************************************************************/
void JLP_RaptorPanel::RaptorPanel_LeftSetup(wxBoxSizer *raptor_left_sizer)
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
 RaptorCmb_ROISize.label = m_messg[206];
/*
* 0 = 64x64
* 1 = 128x128
* 2 = 256x256
* 3 = 512x512
* 4 = full size
*/
 isize0 = 32;
  for(i = 0; i < 4; i++) {
   isize0 *= 2;
   str0.Printf(wxT("%dx%d"), isize0, isize0);
   RaptorCmb_ROISize.choices[i] = str0;
   }
 str0.Printf(wxT("%dx%d"), RaptorSet1.gblXPixels, RaptorSet1.gblYPixels);
 RaptorCmb_ROISize.choices[4] = str0;
 RaptorCmb_ROISize.nchoices = 5;
 RaptorCmb_ROISize.combo = new wxComboBox(this, ID_RPANEL_ROI_SIZE, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             RaptorCmb_ROISize.nchoices,
                                             RaptorCmb_ROISize.choices);
 fgs1->Add(new wxStaticText(this, -1, RaptorCmb_ROISize.label));
 fgs1->Add(RaptorCmb_ROISize.combo);

// Image rotation (to have north on top, for example)
// Values: 0 (no rotation), 1 (+90 degrees), 2 (+180 degrees) or 3 (+270 degrees)
// i=207 "Rotation (deg)"
 RaptorCmb_Rotation.label = m_messg[207];
 RaptorCmb_Rotation.nchoices = 4;
 RaptorCmb_Rotation.choices[0] = wxT("0");
 RaptorCmb_Rotation.choices[1] = wxT("90");
 RaptorCmb_Rotation.choices[2] = wxT("+180");
 RaptorCmb_Rotation.choices[3] = wxT("+270");
 RaptorCmb_Rotation.combo = new wxComboBox(this, ID_RPANEL_ROTATION, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             RaptorCmb_Rotation.nchoices,
                                             RaptorCmb_Rotation.choices);
 fgs1->Add(new wxStaticText(this, -1, RaptorCmb_Rotation.label));
 fgs1->Add(RaptorCmb_Rotation.combo);

// Image compression
// Values: 1, 2, 4 or 8
// i=208 "Binning"
 RaptorCmb_BinFactor.label = m_messg[208];
 RaptorCmb_BinFactor.nchoices = 4;
 RaptorCmb_BinFactor.choices[0] = wxT("1x1");
 RaptorCmb_BinFactor.choices[1] = wxT("2x2");
 RaptorCmb_BinFactor.choices[2] = wxT("4x4");
 RaptorCmb_BinFactor.choices[3] = wxT("8x8");
 RaptorCmb_BinFactor.combo = new wxComboBox(this, ID_RPANEL_BIN_FACTOR, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             RaptorCmb_BinFactor.nchoices,
                                             RaptorCmb_BinFactor.choices);
 fgs1->Add(new wxStaticText(this, -1, RaptorCmb_BinFactor.label));
 fgs1->Add(RaptorCmb_BinFactor.combo);

// Mirror effect (to have North-East orientation for instance)
// Values: 0 (no symmetry), 1 (vertical mirror), 2 (horizontal mirror), 3 (vertical and horizontal)
// i=209 "Mirror effect"
 RaptorCmb_Mirror.label = m_messg[209];
 RaptorCmb_Mirror.nchoices = 4;
// i=210 "None"
 RaptorCmb_Mirror.choices[0] = m_messg[210];
// i=211 "Vertical"
 RaptorCmb_Mirror.choices[1] = m_messg[211];
// i=212 "Horizontal"
 RaptorCmb_Mirror.choices[2] = m_messg[212];
// i=261 "Hori. and vert."
 RaptorCmb_Mirror.choices[3] = m_messg[261];
 RaptorCmb_Mirror.combo = new wxComboBox(this, ID_RPANEL_MIRROR, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             RaptorCmb_Mirror.nchoices,
                                             RaptorCmb_Mirror.choices);
 fgs1->Add(new wxStaticText(this, -1, RaptorCmb_Mirror.label));
 fgs1->Add(RaptorCmb_Mirror.combo);

// i=213 "ROI center"
 fgs1->Add(new wxStaticText(this, -1, m_messg[213]));
 RaptorCtrl_ROICenter = new wxTextCtrl(this, ID_RPANEL_ROI_CENTER, wxT("256,256"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
 fgs1->Add(RaptorCtrl_ROICenter);
 RaptorStatic_DetectorFormat = new wxStaticText(this, -1,
                                       _T("Detector: 512x512 pixels"));
 fgs1->Add(RaptorStatic_DetectorFormat);
 RaptorStatic_ImageFormat = new wxStaticText(this, -1,
                                       _T("Images: 128x128 pixels"));
 fgs1->Add(RaptorStatic_ImageFormat);
 geom_sizer->Add(fgs1, 0, wxALL, 10);
 raptor_left_sizer->Add(geom_sizer, 0, wxEXPAND | wxLEFT | wxBOTTOM, 10);

// *************** Acquisition mode: ***************************************

// i=214 "Acquisition settings"
 acqui_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[214]);
 irows = 3;
 icols = 2;
 wwidth = 160;
 fgs2 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=215 "Mode"
 RaptorCmb_AcquiMode.label = m_messg[215];
 RaptorCmb_AcquiMode.nchoices = 5;
// i=216 "Long exposure"
 RaptorCmb_AcquiMode.choices[0] = m_messg[216];
// i=217 "Video (continuous)"
 RaptorCmb_AcquiMode.choices[1] = m_messg[217];
// i=218 "Sequence of short exp."
 RaptorCmb_AcquiMode.choices[2] = m_messg[218];
// i=219 "Accumulation of short exp."
 RaptorCmb_AcquiMode.choices[3] = m_messg[219];
// i=217 "Video (continuous)" Expert
 RaptorCmb_AcquiMode.choices[4] = m_messg[217] + wxT(" (Expert)");
 RaptorCmb_AcquiMode.combo = new wxComboBox(this, ID_RPANEL_ACQUI_MODE, wxT(""),
                                         wxDefaultPosition, wxSize(wwidth, 28),
                                         RaptorCmb_AcquiMode.nchoices,
                                         RaptorCmb_AcquiMode.choices);
 fgs2->Add(new wxStaticText(this, -1, RaptorCmb_AcquiMode.label));
 fgs2->Add(RaptorCmb_AcquiMode.combo);

// *************** Trigger, shutter, temp. regul: *****************************

// Trigger and shutter:

// ExtTrigger
//   0=Off, 1=On(Ext +ve Edge), 2=On(Ext -ve Edge)
// i=620 "Ext. Trigger"
 RaptorCmb_ExtTrigger.label = m_messg[620];
 RaptorCmb_ExtTrigger.nchoices = 3;
// i=621 "Off"
 RaptorCmb_ExtTrigger.choices[0] = m_messg[621];
// i=622 "On (+edge)"
 RaptorCmb_ExtTrigger.choices[1] = m_messg[622];
// i=623 "On (-edge)"
 RaptorCmb_ExtTrigger.choices[2] = m_messg[623];
 RaptorCmb_ExtTrigger.combo = new wxComboBox(this, ID_RPANEL_EXT_TRIGGER, wxT(""),
                                         wxDefaultPosition, wxSize(wwidth, 28),
                                         RaptorCmb_ExtTrigger.nchoices,
                                         RaptorCmb_ExtTrigger.choices);
 fgs2->Add(new wxStaticText(this, -1, RaptorCmb_ExtTrigger.label));
 fgs2->Add(RaptorCmb_ExtTrigger.combo);

/** Shutter
* Values of RaptorSet1.ShutterMode:
* -1 not available, 0 (automatic), 1 (open) or  2  (closed),
* Corresponding values of combobox selection: 0, 1, 2, 3
**/
// i=224 "Shutter"
 RaptorCmb_Shutter.label = m_messg[224];
 RaptorCmb_Shutter.nchoices = 4;
// i=225 "Unavailable"
 RaptorCmb_Shutter.choices[0] = m_messg[225];
// i=226 "Automatic"
 RaptorCmb_Shutter.choices[1] = m_messg[226];
// i=227 "Kept open"
 RaptorCmb_Shutter.choices[2] = m_messg[227];
// i=228 "Kept closed"
 RaptorCmb_Shutter.choices[3] = m_messg[228];
 RaptorCmb_Shutter.combo = new wxComboBox(this, ID_RPANEL_SHUTTER, wxT(""),
                                          wxDefaultPosition, wxSize(wwidth, 28),
                                          RaptorCmb_Shutter.nchoices,
                                          RaptorCmb_Shutter.choices);
 fgs2->Add(new wxStaticText(this, -1, RaptorCmb_Shutter.label),
           0, wxBOTTOM, 10);
 fgs2->Add(RaptorCmb_Shutter.combo);

 acqui_sizer->Add(fgs2, 0, wxALL, 10);

// To fill available space (and align with previous frame)
 raptor_left_sizer->Add(acqui_sizer, 0, wxEXPAND | wxLEFT | wxBOTTOM, 10);

}
/********************************************************************
* Raptor Setup panel (right side)
********************************************************************/
void JLP_RaptorPanel::RaptorPanel_RightSetup(wxBoxSizer *raptor_cent_sizer)
{
wxStaticBoxSizer *CameraStatus_sizer;
wxBoxSizer *hsizer0, *hsizer1, *vsizer2;
int irows, icols, vgap = 10, hgap = 12, wwidth = 80;
wxFlexGridSizer *fgs1;

// *************** Current camera status: ***************************************

// i=630 "Current camera status"
 CameraStatus_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[630]);
 irows = 5;
 icols = 2;
 wwidth = 160;
 fgs1 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=631 "Sensor temp. (deg. C)"
 fgs1->Add(new wxStaticText(this, -1, m_messg[631]));
 RaptorStatic_StatusSensorTemp = new wxStaticText(this, -1, _T("-00.00"));
 fgs1->Add(RaptorStatic_StatusSensorTemp);

// i=632 "Box temp. (deg. C)"
 fgs1->Add(new wxStaticText(this, -1, m_messg[632]));
 RaptorStatic_StatusBoxTemp = new wxStaticText(this, -1, _T("+00.00"));
 fgs1->Add(RaptorStatic_StatusBoxTemp);

// i=633 "Gain"
 fgs1->Add(new wxStaticText(this, -1, m_messg[633]));
 RaptorStatic_StatusDigitalGain = new wxStaticText(this, -1, _T("0000"));
 fgs1->Add(RaptorStatic_StatusDigitalGain);

// i=634 "Expo. time (msec)"
 fgs1->Add(new wxStaticText(this, -1, m_messg[634]));
 RaptorStatic_StatusExpoTime = new wxStaticText(this, -1, _T("0000"));
 fgs1->Add(RaptorStatic_StatusExpoTime);

// Add button for checking current settings:
 hsizer0 = new wxBoxSizer(wxHORIZONTAL);

// i=639 "Update camera status"
 RaptorBut_CheckCurrentStatus = new wxButton(this, ID_RPANEL_CURRENT_STATUS,
                                              m_messg[639]);

 hsizer0->Add(RaptorBut_CheckCurrentStatus, 0, wxCENTER, 20);

 fgs1->Add(hsizer0);

// To fill available space (and align with previous frame)
 CameraStatus_sizer->Add(fgs1, 0, wxALL, 10);

 raptor_cent_sizer->Add(CameraStatus_sizer, 0, wxALL, 10);

return;
}
/********************************************************************
* Raptor Setup panel (central part)
********************************************************************/
void JLP_RaptorPanel::RaptorPanel_CentSetup(wxBoxSizer *raptor_right_sizer)
{
wxStaticBoxSizer *Camera_sizer;
wxStaticBoxSizer *Gain_sizer, *ShutdownOptions_sizer, *temp_sizer;
wxBoxSizer *hsizer1, *vsizer2;
int irows, icols, vgap = 10, hgap = 12, wwidth = 80;
wxFlexGridSizer *fgs3, *fgs4, *fgs6;

/*
Auto_Exposure:
    Off
    On
Digital Gain: 25 (0--48)
Exposure: 28 ms (0--1000)
Ext Trigger:
    Off
    On: (Ext +ve Edge)
    On: (Ext -ve Edge)
Frame rate :
    25 Hz
    30 Hz
    50 Hz
    60 Hz
    90 Hz
    120 Hz
Frame rate manual: 25
High Gain:
    Off
    On
Image Sharpen:
    Off
NUC State:
    0: Offset+Gain corr.
    1: Normal
    2: Offset+Gain+Dark
    3: 8 bit Offset - 32
    4: 8 bit Dark - 2^19
    5: 8 bit Gain - 128
    6: Reversed map
    7: Ramp Test Pattern
System:
    Default
    Gain Gain
TE Cooler:
    Off
    On
TEC Setpoint: -15 (-68--119)
Trigger Delay: 0 (0--104)
*/

// Camera setup:
vsizer2 = new wxBoxSizer(wxVERTICAL);
// i=640 "Camera setup"
Camera_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[640]);
irows = 6;
icols = 2;
vgap = 8;
hgap = 20;
wwidth = 140;
fgs3 = new wxFlexGridSizer(irows, icols, vgap, hgap);

/* NUC State:
* 0: Offset corr.
* 1: Offset+Gain corr.
* 2: Normal
* 3: Offset+Gain+Dark
* 4: 8 bit Offset / 32
* 5: 8 bit Dark * 2^19
* 6: 8 bit Gain / 128
* 7: Reserved map
* 8: Ramp Test Pattern
*/

// i=641 "NUC State"
 RaptorCmb_NUC_State.label = m_messg[641];
 RaptorCmb_NUC_State.nchoices = 9;
 RaptorCmb_NUC_State.choices[0] = wxT("Offset corr.");
 RaptorCmb_NUC_State.choices[1] = wxT("Offset+Gain corr.");
 RaptorCmb_NUC_State.choices[2] = wxT("Normal");
 RaptorCmb_NUC_State.choices[3] = wxT("Offset+Gain+Dark");
 RaptorCmb_NUC_State.choices[4] = wxT("8bit Offset/32");
 RaptorCmb_NUC_State.choices[5] = wxT("8bit Dark*2^19");
 RaptorCmb_NUC_State.choices[6] = wxT("8bit Gain/128");
 RaptorCmb_NUC_State.choices[7] = wxT("Reserved map");
 RaptorCmb_NUC_State.choices[8] = wxT("Ramp Test Pattern");
 RaptorCmb_NUC_State.combo = new wxComboBox(this, ID_RPANEL_NUC_STATE,
                                             wxT(""), wxDefaultPosition,
                                             wxSize(wwidth, 28),
                                             RaptorCmb_NUC_State.nchoices,
                                             RaptorCmb_NUC_State.choices);
 fgs3->Add(new wxStaticText(this, -1, RaptorCmb_NUC_State.label));
 fgs3->Add(RaptorCmb_NUC_State.combo);

/* Frame rate :
* 0: 25 Hz
* 1: 30 Hz
* 2: 50 Hz
* 3: 60 Hz
* 4: 90 Hz
* 5: 120 Hz
*/
// i=642 "Frame rate"
 RaptorCmb_FrameRate.label = m_messg[642];
 RaptorCmb_FrameRate.nchoices = 6;
 RaptorCmb_FrameRate.choices[0] = wxT("25 Hz");
 RaptorCmb_FrameRate.choices[1] = wxT("30 Hz");
 RaptorCmb_FrameRate.choices[2] = wxT("50 Hz");
 RaptorCmb_FrameRate.choices[3] = wxT("60 Hz");
 RaptorCmb_FrameRate.choices[4] = wxT("90 Hz");
 RaptorCmb_FrameRate.choices[5] = wxT("120 Hz");
 RaptorCmb_FrameRate.combo = new wxComboBox(this, ID_RPANEL_FRAME_RATE,
                                                   wxT(""), wxDefaultPosition,
                                                   wxSize(wwidth, 28),
                                                   RaptorCmb_FrameRate.nchoices,
                                                   RaptorCmb_FrameRate.choices);
 fgs3->Add(new wxStaticText(this, -1, RaptorCmb_FrameRate.label));
 fgs3->Add(RaptorCmb_FrameRate.combo);

// ExpoTime : short exp. time
 RaptorCtrl_ExpoTime = new wxTextCtrl(this, ID_RPANEL_EXPO_TIME, wxT("10"),
                                          wxPoint(-1, -1), wxSize(wwidth, 28));
// i=233 "Short exp. time (msec)"
 fgs3->Add(new wxStaticText(this, -1, m_messg[233]));
 fgs3->Add(RaptorCtrl_ExpoTime);

 RaptorCtrl_IntegTime = new wxTextCtrl(this, ID_RPANEL_INTEG_TIME, wxT("1000"),
                                          wxPoint(-1, -1), wxSize(wwidth, 28));
// i=238 "Integration time (sec)"
 fgs3->Add(new wxStaticText(this, -1, m_messg[238]));
 fgs3->Add(RaptorCtrl_IntegTime);

// Miscellaneous: TriggerDelay AutoExposure, ImageSharpen
// Trigger delay:
 RaptorCtrl_TriggerDelay = new wxTextCtrl(this, ID_RPANEL_TRIGGER_DELAY, wxT("10"),
                                          wxPoint(-1, -1), wxSize(wwidth, 28));
// i=647 "Trigger delay"
 fgs3->Add(new wxStaticText(this, -1, m_messg[647]));
 fgs3->Add(RaptorCtrl_TriggerDelay);

// i=648 "Auto Exposure"
 RaptorCheck_AutoExposure = new wxCheckBox(this, ID_RPANEL_AUTO_EXPOSURE,
                                          m_messg[648]);
 fgs3->Add(RaptorCheck_AutoExposure, 0, wxRIGHT, 10);

// i=649 "Image Sharpen"
 RaptorCheck_ImageSharpen = new wxCheckBox(this, ID_RPANEL_IMAGE_SHARPEN,
                                           m_messg[649]);
 fgs3->Add(RaptorCheck_ImageSharpen, 0, wxLEFT, 20);

 Camera_sizer->Add(fgs3, 0, wxALL, 10);

// To fill available space (and align with next frame)
 vsizer2->Add(Camera_sizer, 0, wxEXPAND | wxLEFT | wxBOTTOM, 10);

// Gain:
// i=643 "Gain"
Gain_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[643]);
irows = 1;
icols = 4;
vgap = 20;
hgap = 12;
fgs4 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=644 "High Gain"
RaptorCheck_HighGain = new wxCheckBox(this, ID_RPANEL_HIGH_GAIN_CHK, m_messg[644]);
fgs4->Add(RaptorCheck_HighGain, 0);
fgs4->Add(new wxStaticText(this, -1, wxT("")), 0, wxLEFT, 20);

// i=645 "Digital Gain"
fgs4->Add(new wxStaticText(this, -1, m_messg[645]));
RaptorCtrl_DigitalGain = new wxTextCtrl(this, ID_RPANEL_DIGITAL_GAIN, wxT("30"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
fgs4->Add(RaptorCtrl_DigitalGain);

Gain_sizer->Add(fgs4, 0, wxALL, 10);

// To fill available space (and align with next frame)
 vsizer2->Add(Gain_sizer, 0, wxEXPAND | wxLEFT | wxBOTTOM, 10);

// Cooler settings:
// i=246 "Cooler control"
 temp_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[246]);
 irows = 2;
 icols = 4;
 fgs6 = new wxFlexGridSizer(irows, icols, vgap, hgap);

// i=250 "Activated"
 RaptorCheck_CoolerRegul = new wxCheckBox(this, ID_RPANEL_COOLER_REGUL,
                                         m_messg[250]);
// i=251 "Fan"
 RaptorCheck_CoolerFan = new wxCheckBox(this, ID_RPANEL_COOLER_REGUL,
                                         m_messg[251]);
 RaptorCtrl_CoolerSetTemp = new wxTextCtrl(this, ID_RPANEL_COOLER_SETPOINT,
                                           wxT("-76.0"),
                                           wxPoint(-1, -1), wxSize(60, 28));
 fgs6->Add(RaptorCheck_CoolerRegul, 0, wxBOTTOM, 6);
 fgs6->Add(new wxStaticText(this, -1, wxT("")));
// i=247 "Set point (deg. C)"
 fgs6->Add(new wxStaticText(this, -1, m_messg[247]));
 fgs6->Add(RaptorCtrl_CoolerSetTemp);
// CoolerFan
 fgs6->Add(RaptorCheck_CoolerFan, 0, wxLEFT, 20);
 temp_sizer->Add(fgs6, 0, wxALL, 10);

// To fill available space (and align with next frame)
 vsizer2->Add(temp_sizer, 0, wxEXPAND | wxLEFT | wxBOTTOM, 10);


hsizer1 = new wxBoxSizer(wxHORIZONTAL);
hsizer1->Add(vsizer2, 0, wxRIGHT, 10);

raptor_right_sizer->Add(hsizer1, 0, wxALL, 5);

}
/********************************************************************
* Initialization of combo bowes without Raptor camera
********************************************************************/
void JLP_RaptorPanel::InitComboBoxItems_without_Raptor()
{
// Image format:
 RaptorCmb_ROISize.nchoices = 5;
 RaptorCmb_ROISize.choices[0] = wxT("64x64");
 RaptorCmb_ROISize.choices[1] = wxT("128x128");
 RaptorCmb_ROISize.choices[2] = wxT("256x256");
 RaptorCmb_ROISize.choices[3] = wxT("512x512");
// i=260 "Full frame"
 RaptorCmb_ROISize.choices[4] = m_messg[260];

// NUC State:
 RaptorCmb_NUC_State.nchoices = 1;
 RaptorCmb_NUC_State.choices[0] = wxT("2.4 x");

// Frame Rate:
 RaptorCmb_FrameRate.nchoices = 1;
 RaptorCmb_FrameRate.choices[0] = wxT("0");
}
/****************************************************************************
* Initialization of the items of the ComboBoxes
****************************************************************************/
int JLP_RaptorPanel::InitComboBoxItems_with_Raptor()
{
return(0);
}
