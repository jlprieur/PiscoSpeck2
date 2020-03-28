/****************************************************************************
* Name: jlp_fitscube.cpp
* JLP_FitsCubePanel class
*
* JLP
* Version 17/07/2015
****************************************************************************/
#include "jlp_fitscube_panel.h"

// ============================================================================
// implementation
// ============================================================================

BEGIN_EVENT_TABLE(JLP_FitsCubePanel, wxPanel)

// Buttons:
EVT_BUTTON  (ID_FPANEL_CONFIG_OK, JLP_FitsCubePanel::OnConfig)
EVT_BUTTON  (ID_FPANEL_CONFIG_CANCEL, JLP_FitsCubePanel::OnConfig)
EVT_BUTTON  (ID_FPANEL_CONFIG_SAVE, JLP_FitsCubePanel::OnConfig)
EVT_BUTTON  (ID_FPANEL_CONFIG_LOAD, JLP_FitsCubePanel::OnConfig)
EVT_BUTTON  (ID_FPANEL_CONFIG_DEFAULT, JLP_FitsCubePanel::OnConfig)

// Text controls:
EVT_TEXT  (ID_FPANEL_ROI_CENTER, JLP_FitsCubePanel::OnROICenterOnExit)

// Combo boxes:
EVT_COMBOBOX (ID_FPANEL_ROI_SIZE, JLP_FitsCubePanel::OnROISizeComboBoxChange)
EVT_COMBOBOX (ID_FPANEL_ROTATION, JLP_FitsCubePanel::OnRotationComboBoxChange)
EVT_COMBOBOX (ID_FPANEL_MIRROR, JLP_FitsCubePanel::OnMirrorComboBoxChange)
EVT_COMBOBOX (ID_FPANEL_BIN_FACTOR, JLP_FitsCubePanel::OnBinFactorComboBoxChange)

END_EVENT_TABLE()

/*******************************************************************************
* Constructor from wxFrame:
*******************************************************************************/
JLP_FitsCubePanel::JLP_FitsCubePanel(AspFrame *asp_frame, wxFrame *frame,
                                     const int my_wxID, JLP_FitsCubeCam1 *fitscube_cam0,
                                     JLP_CAMERA_PANEL_CHANGES *jcam_changes0,
                                     wxString *str_messg,
                                     const int n_messg, bool small_version)
                                     : wxPanel( frame, my_wxID)
{
wxString input_filename;
bool update_from_screen;
int k;

 initialized = 0;
 m_aspframe = asp_frame;
 m_cam1 = fitscube_cam0;
 m_cam1->Get_FitsCubeSettings(&FitsCubeSet1);
// Copy Original_FitsCubeSet1 to  current FitsCubeSet1:
 Copy_FitsCubeSettings(FitsCubeSet1, &Original_FitsCubeSet1);

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

// Setup panel
 FitsCubePanel_Setup();

 initialized = 1234;

// Update display (after setting initialized to 1234) :
 update_from_screen = false;
 DisplayNewSettings(update_from_screen);

return;
}
/********************************************************************
* FitsCube Setup panel
********************************************************************/
void JLP_FitsCubePanel::MyFreeMemory()
{
// Error here so I leave it allocated:
// if(m_messg != NULL) delete m_messg;
}
/********************************************************************
* FitsCube Setup panel
********************************************************************/
int JLP_FitsCubePanel::FitsCubePanel_Setup()
{
wxBoxSizer *w_topsizer, *w_hsizer1, *w_hsizer2;
wxBoxSizer *FitsCube_left_sizer;
wxButton *FitsCubeOKButton, *FitsCubeCancelButton, *FitsCubeSaveButton, *FitsCubeLoadButton;
wxButton *FitsCubeDefaultButton;

  InitComboBoxItems();

  w_topsizer = new wxBoxSizer(wxVERTICAL);

  w_hsizer1 = new wxBoxSizer( wxHORIZONTAL );

// Create camera setup:
  FitsCube_left_sizer = new wxBoxSizer(wxVERTICAL);
  FitsCubePanel_LeftSetup(FitsCube_left_sizer);
  w_hsizer1->Add(FitsCube_left_sizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 6);

  w_topsizer->Add(w_hsizer1, 0, wxALIGN_CENTER);

///////////////////////////////////////////////////////////////////////
// Create five buttons at bottom:
// Validate, Standard value, Cancel, Save config, Load config
 w_hsizer2 = new wxBoxSizer( wxHORIZONTAL );

// i=200 "Validate"
 FitsCubeOKButton = new wxButton(this, ID_FPANEL_CONFIG_OK, m_messg[200]);
// i=201 "Cancel"
 FitsCubeCancelButton = new wxButton(this, ID_FPANEL_CONFIG_CANCEL, m_messg[201]);
// i=202 "Save the config"
 FitsCubeSaveButton = new wxButton(this, ID_FPANEL_CONFIG_SAVE, m_messg[202]);
// i=203 "Load a config"
 FitsCubeLoadButton = new wxButton(this, ID_FPANEL_CONFIG_LOAD, m_messg[203]);
// i=204 "Standard values"
 FitsCubeDefaultButton = new wxButton(this, ID_FPANEL_CONFIG_DEFAULT, m_messg[204]);

// Add buttons, horizontally unstretchable, with minimal size:
 w_hsizer2->Add( FitsCubeOKButton, 0);
 w_hsizer2->Add( FitsCubeCancelButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( FitsCubeSaveButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( FitsCubeLoadButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( FitsCubeDefaultButton, 0, wxLEFT | wxRIGHT, 20);

// Add button sizer with minimal size:
  w_topsizer->Add(w_hsizer2, 0, wxALIGN_CENTER | wxALL, 10 );

  this->SetSizer(w_topsizer);

  Centre();

return(0);
}
/********************************************************************
* FitsCube Setup panel (left side)
********************************************************************/
void JLP_FitsCubePanel::FitsCubePanel_LeftSetup(wxBoxSizer *FitsCube_left_sizer)
{
wxStaticBoxSizer *geom_sizer;
int i, imax, nchoices, isize0, irows, icols, vgap = 12, hgap = 12, wwidth = 160;
wxFlexGridSizer *fgs1;
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
 FitsCubeCmb_ROISize.label = m_messg[206];
// 64, 128, 256, 512, full size
 isize0 = 64;
 imax = MINI(FitsCubeSet1.gblXPixels, FitsCubeSet1.gblYPixels);
 imax = log2((double)imax) - log2(64.);
 nchoices = -1;
  for(i = 0; i < imax; i++) {
   nchoices ++;
   str0.Printf(wxT("%dx%d"), isize0, isize0);
   FitsCubeCmb_ROISize.choices[nchoices] = str0;
   isize0 *= 2;
   }
 nchoices ++;
 str0.Printf(wxT("%dx%d"), FitsCubeSet1.gblXPixels, FitsCubeSet1.gblYPixels);
 FitsCubeCmb_ROISize.choices[nchoices] = str0;
 FitsCubeCmb_ROISize.nchoices = nchoices + 1;
 FitsCubeCmb_ROISize.combo = new wxComboBox(this, ID_FPANEL_ROI_SIZE, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             FitsCubeCmb_ROISize.nchoices,
                                             FitsCubeCmb_ROISize.choices);
 fgs1->Add(new wxStaticText(this, -1, FitsCubeCmb_ROISize.label));
 fgs1->Add(FitsCubeCmb_ROISize.combo);

// Image rotation (to have north on top, for example)
// Values: 0 (no rotation), 1 (+90 degrees), 2 (+180 degrees) or 3 (+270 degrees)
// i=207 "Rotation (deg)"
 FitsCubeCmb_Rotation.label = m_messg[207];
 FitsCubeCmb_Rotation.nchoices = 4;
 FitsCubeCmb_Rotation.choices[0] = wxT("0");
 FitsCubeCmb_Rotation.choices[1] = wxT("90");
 FitsCubeCmb_Rotation.choices[2] = wxT("+180");
 FitsCubeCmb_Rotation.choices[3] = wxT("+270");
 FitsCubeCmb_Rotation.combo = new wxComboBox(this, ID_FPANEL_ROTATION, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             FitsCubeCmb_Rotation.nchoices,
                                             FitsCubeCmb_Rotation.choices);
 fgs1->Add(new wxStaticText(this, -1, FitsCubeCmb_Rotation.label));
 fgs1->Add(FitsCubeCmb_Rotation.combo);

// Image compression
// Values: 1, 2, 4 or 8
// i=208 "Binning"
 FitsCubeCmb_BinFactor.label = m_messg[208];
 FitsCubeCmb_BinFactor.nchoices = 4;
 FitsCubeCmb_BinFactor.choices[0] = wxT("1x1");
 FitsCubeCmb_BinFactor.choices[1] = wxT("2x2");
 FitsCubeCmb_BinFactor.choices[2] = wxT("4x4");
 FitsCubeCmb_BinFactor.choices[3] = wxT("8x8");
 FitsCubeCmb_BinFactor.combo = new wxComboBox(this, ID_FPANEL_BIN_FACTOR, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             FitsCubeCmb_BinFactor.nchoices,
                                             FitsCubeCmb_BinFactor.choices);
 fgs1->Add(new wxStaticText(this, -1, FitsCubeCmb_BinFactor.label));
 fgs1->Add(FitsCubeCmb_BinFactor.combo);

// Mirror effect (to have North-East orientation for instance)
// Values: 0 (no symmetry), 1 (vertical mirror), 2 (horizontal mirror), 3 (vertical and horizontal)
// i=209 "Mirror effect"
 FitsCubeCmb_Mirror.label = m_messg[209];
 FitsCubeCmb_Mirror.nchoices = 4;
// i=210 "None"
 FitsCubeCmb_Mirror.choices[0] = m_messg[210];
// i=211 "Vertical"
 FitsCubeCmb_Mirror.choices[1] = m_messg[211];
// i=212 "Horizontal"
 FitsCubeCmb_Mirror.choices[2] = m_messg[212];
// i=261 "Hori. and vert."
 FitsCubeCmb_Mirror.choices[3] = m_messg[261];
 FitsCubeCmb_Mirror.combo = new wxComboBox(this, ID_FPANEL_MIRROR, wxT(""),
                                             wxDefaultPosition, wxSize(wwidth, 28),
                                             FitsCubeCmb_Mirror.nchoices,
                                             FitsCubeCmb_Mirror.choices);
 fgs1->Add(new wxStaticText(this, -1, FitsCubeCmb_Mirror.label));
 fgs1->Add(FitsCubeCmb_Mirror.combo);

// i=213 "ROI center"
 fgs1->Add(new wxStaticText(this, -1, m_messg[213]));
 FitsCubeCtrl_ROICenter = new wxTextCtrl(this, ID_FPANEL_ROI_CENTER, wxT("256,256"),
                                        wxPoint(-1, -1), wxSize(wwidth, 28));
 fgs1->Add(FitsCubeCtrl_ROICenter);
 FitsCubeStatic_DetectorFormat = new wxStaticText(this, -1,
                                       _T("Detector: 512x512 pixels"));
 fgs1->Add(FitsCubeStatic_DetectorFormat);

 FitsCubeStatic_ImageFormat = new wxStaticText(this, -1,
                                       _T("Images: 128x128 pixels"));
 fgs1->Add(FitsCubeStatic_ImageFormat);
 geom_sizer->Add(fgs1, 0, wxALL, 10);
 FitsCube_left_sizer->Add(geom_sizer, 0, wxEXPAND | wxLEFT | wxBOTTOM, 10);


}
/********************************************************************
* Initialization of combo boxes
********************************************************************/
void JLP_FitsCubePanel::InitComboBoxItems()
{
// Image format:
 FitsCubeCmb_ROISize.nchoices = 5;
 FitsCubeCmb_ROISize.choices[0] = wxT("64x64");
 FitsCubeCmb_ROISize.choices[1] = wxT("128x128");
 FitsCubeCmb_ROISize.choices[2] = wxT("256x256");
 FitsCubeCmb_ROISize.choices[3] = wxT("512x512");
// i=260 "Full frame"
 FitsCubeCmb_ROISize.choices[4] = m_messg[260];

}
