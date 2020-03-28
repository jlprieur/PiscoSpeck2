/****************************************************************************
* Name: jlp_speckprocess_panel.cpp
*
* JLP
* Version 17/07/2015
****************************************************************************/
#include "jlp_speckprocess_panel.h"

/*******************************************************************************
* Constructor from wxFrame:
*******************************************************************************/
JLP_SpeckProcessPanel::JLP_SpeckProcessPanel(wxFrame *frame,
                                             wxString *str_messg,
                                             const int n_messg,
                                             bool small_version)
                                            : wxPanel( frame )
{
wxString input_filename;
int k;

 initialized = 0;
 ChangesDone1 = false;
 ValidatedChanges1 = false;

// Transform coma into point for numbers:
 setlocale(LC_NUMERIC, "C");

// Smaller fonts (needed for Windows, to have a similar appearance as Linux)
if(small_version) SetWindowVariant(wxWINDOW_VARIANT_SMALL);

// Messages (in different languages)
 m_messg = new wxString[n_messg];
 for(k = 0; k < n_messg; k++) m_messg[k] = str_messg[k];

// Init PROCESS_SETTINGS structure:
 Init_PSET(Original_Pset1);

// Copy data to Pset1:
 Copy_PSET(Original_Pset1, Pset1);

// Setup panel
 SpeckProcessPanelSetup();

// Display new values:
   DisplayNewValues();

 initialized = 1234;

return;
}
/********************************************************************
* Speckle Processing Setup panel
********************************************************************/
int JLP_SpeckProcessPanel::SpeckProcessPanelSetup()
{
wxBoxSizer *w_topsizer, *w_hsizer1, *w_hsizer2;
wxBoxSizer *processing_left_sizer, *processing_center_sizer;
wxBoxSizer *processing_right_sizer;
wxButton *SpeckProcessOKButton, *SpeckProcessCancelButton, *SpeckProcessSaveButton;
wxButton *SpeckProcessLoadButton, *SpeckProcessDefaultButton;

  w_topsizer = new wxBoxSizer(wxVERTICAL);

  w_hsizer1 = new wxBoxSizer( wxHORIZONTAL );

// Create speckle processing setup:
  processing_left_sizer = new wxBoxSizer(wxVERTICAL);
  SpeckProcessPanel_LeftSetup(processing_left_sizer);
  w_hsizer1->Add(processing_left_sizer, 0, wxALL, 10);

  processing_center_sizer = new wxBoxSizer(wxVERTICAL);
  SpeckProcessPanel_CenterSetup(processing_center_sizer);
  w_hsizer1->Add(processing_center_sizer, 0, wxALIGN_CENTRE_VERTICAL);

  processing_right_sizer = new wxBoxSizer(wxVERTICAL);
  SpeckProcessPanel_RightSetup(processing_right_sizer);
  w_hsizer1->Add(processing_right_sizer, 0,
                 wxALIGN_CENTRE_VERTICAL | wxALL, 10);

  w_topsizer->Add(w_hsizer1, 0, wxALIGN_CENTER | wxTOP, 6);

///////////////////////////////////////////////////////////////////////
// Create five buttons at bottom:
// OK, Standard value, Cancel, Save config, Load config
 w_hsizer2 = new wxBoxSizer( wxHORIZONTAL );

// i=200 "Validate"
 SpeckProcessOKButton = new wxButton(this, ID_PROC_CONFIG_OK, m_messg[200]);
// i=201 "Cancel"
 SpeckProcessCancelButton = new wxButton(this, ID_PROC_CONFIG_CANCEL,
                                          m_messg[201]);
// i=202 "Save the config"
 SpeckProcessSaveButton = new wxButton(this, ID_PROC_CONFIG_SAVE,
                                        m_messg[202]);
// i=203 "Load a config"
 SpeckProcessLoadButton = new wxButton(this, ID_PROC_CONFIG_LOAD,
                                        m_messg[203]);
// i=204 "Standard values"
 SpeckProcessDefaultButton = new wxButton(this, ID_PROC_CONFIG_DEFAULT,
                                           m_messg[204]);

// Add buttons, horizontally unstretchable, with minimal size:
 w_hsizer2->Add( SpeckProcessOKButton, 0);
 w_hsizer2->Add( SpeckProcessCancelButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( SpeckProcessSaveButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( SpeckProcessLoadButton, 0, wxLEFT | wxRIGHT, 20);
 w_hsizer2->Add( SpeckProcessDefaultButton, 0, wxLEFT | wxRIGHT, 20);

// Add button sizer with minimal size:
  w_topsizer->Add(w_hsizer2, 0, wxALIGN_CENTER | wxALL, 6);

  this->SetSizer(w_topsizer);

  Centre();
return(0);
}
/********************************************************************
* SpeckProcess Setup panel (left side)
********************************************************************/
void JLP_SpeckProcessPanel::SpeckProcessPanel_LeftSetup(wxBoxSizer *processing_left_sizer)
{
wxStaticBoxSizer *mode_sizer, *bisp_sizer, *spect_sizer;
wxStaticBoxSizer *stat_sizer, *save_sizer, *cube_sizer;
wxBoxSizer *stat_sizer1;
wxBoxSizer *hsizer0, *hsizer1, *vsizer1, *hsizer2;
int irows, icols, vgap, hgap;
wxFlexGridSizer *fgs0, *fgs1, *fgs2, *fgs3, *fgs4;

 vsizer1 = new wxBoxSizer(wxVERTICAL);

// i=300 "Type of processing"
 mode_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[300]);
// *************** Processing type: ***************************************
 irows = 1;
 icols = 2;
 vgap = 12;
 hgap = 12;
 fgs0 = new wxFlexGridSizer(irows, icols, vgap, hgap);
// i=301 "Mode :"
 ProcCmb_Mode.label = m_messg[301];
 ProcCmb_Mode.nchoices = 9;
// i=302 "None (display only)"
 ProcCmb_Mode.choices[0] = m_messg[302];
// i=303 "Integration only (with/without centering"
 ProcCmb_Mode.choices[1] = m_messg[303];
// i=304 "Autoc, spectrum"
 ProcCmb_Mode.choices[2] = m_messg[304];
// i=305 "Autoc, spectrum, quadrant"
 ProcCmb_Mode.choices[3] = m_messg[305];
// i=306 "Bispectrum, spectrum"
 ProcCmb_Mode.choices[4] = m_messg[306];
// i=307 "Bispectrum, autoc, spectrum, quadrant"
 ProcCmb_Mode.choices[5] = m_messg[307];
// i=308 "Spectroscopy"
 ProcCmb_Mode.choices[6] = m_messg[308];
// i=309 "SCIDAR-Linear"
 ProcCmb_Mode.choices[7] = m_messg[309];
// i=310 "SCIDAR-Logarithm"
 ProcCmb_Mode.choices[8] = m_messg[310];
 ProcCmb_Mode.combo = new wxComboBox(this, ID_PROC_MODES, wxT(""),
                                             wxDefaultPosition,
                                             wxSize(350, 28),
                                             ProcCmb_Mode.nchoices,
                                             ProcCmb_Mode.choices);
 fgs0->Add(new wxStaticText(this, -1, ProcCmb_Mode.label));
 fgs0->Add(ProcCmb_Mode.combo);

 mode_sizer->Add(fgs0, 0, wxALIGN_CENTER | wxBOTTOM, 10);

 hsizer0 = new wxBoxSizer(wxHORIZONTAL);

// ****************** Bispectrum ********************************************/
// i=311 "Bispectrum"
 bisp_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[311]);
 irows = 2;
 icols = 2;
 vgap = 5;
 hgap = 12;
 fgs1 = new wxFlexGridSizer(irows, icols, vgap, hgap);
 fgs1->Add(new wxStaticText(this, -1, _T("ir max :")));
 fgs1->Add(new wxStaticText(this, -1, _T("nclos max :")));

 ProcCtrl_Bisp_ir = new wxTextCtrl(this, ID_PROC_BISP_IR, wxT("30"),
                                   wxPoint(-1, -1), wxSize(70, 28));
 fgs1->Add(ProcCtrl_Bisp_ir);
 ProcCtrl_Bisp_nclos = new wxTextCtrl(this, ID_PROC_BISP_NCLOS, wxT("200"),
                                      wxPoint(-1, -1), wxSize(70, 28));
 fgs1->Add(ProcCtrl_Bisp_nclos);
 bisp_sizer->Add(fgs1, 0, wxALL, 10);
 hsizer0->Add(bisp_sizer, 0, wxEXPAND);

// ****************** Spectroscopy *******************************************/
// i=308 "Spectroscopy"
 spect_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[308]);
 irows = 2;
 icols = 2;
 vgap = 8;
 hgap = 8;
 fgs2 = new wxFlexGridSizer(irows, icols, vgap, hgap);
// i=312 "Slit angle :"
 fgs2->Add(new wxStaticText(this, -1, m_messg[312]));
 ProcCtrl_SlitAngle = new wxTextCtrl(this, ID_PROC_SLIT_ANGLE, wxT("90.00"),
                                     wxPoint(-1, -1), wxSize(80, 28));
 fgs2->Add(ProcCtrl_SlitAngle);
// i=313 "Spectrum angle :"
 fgs2->Add(new wxStaticText(this, -1, m_messg[313]));
 ProcCtrl_SpectAngle = new wxTextCtrl(this, ID_PROC_SPECT_ANGLE, wxT("0.00"),
                                      wxPoint(-1, -1), wxSize(80, 28));
 fgs2->Add(ProcCtrl_SpectAngle);
 spect_sizer->Add(fgs2, 0, wxEXPAND | wxALL, 10);
 hsizer0->Add(spect_sizer, 0, wxEXPAND | wxLEFT, 10);

 mode_sizer->Add(hsizer0, 0, wxEXPAND | wxALL, 10);

 processing_left_sizer->Add(mode_sizer, 0, wxEXPAND | wxBOTTOM, 10);

// ****************** Statistics *******************************************/
// i=314 "Statistics and image selection"
 stat_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[314]);

// Check boxes:
 irows = 2; icols = 2;
 vgap = 5; hgap = 60;
 fgs3 = new wxFlexGridSizer(irows, icols, vgap, hgap);
// i=315 "Statistics"
 ProcCheck_Stats = new wxCheckBox(this, ID_PROC_STATS_CHCK, m_messg[315]);
 fgs3->Add(ProcCheck_Stats);
// i=316 "Selection with the seeing"
 ProcCheck_SeeingSelect = new wxCheckBox(this, ID_PROC_SEEING_SELECT, m_messg[316]);
 fgs3->Add(ProcCheck_SeeingSelect);
 fgs3->Add(new wxStaticText(this, -1, wxT("")));
// i=317 "Selection with the maximum value"
 ProcCheck_MaxSelect = new wxCheckBox(this, ID_PROC_MAX_SELECT, m_messg[317]);
 fgs3->Add(ProcCheck_MaxSelect);
 stat_sizer->Add(fgs3, 0, wxALL, 10);

// i=318 "Min/max thresholds for the selection :"
 stat_sizer->Add(new wxStaticText(this, -1, m_messg[318]),
                 0, wxLEFT, 10);

// Thresholds:
 stat_sizer1 = new wxBoxSizer( wxHORIZONTAL );
// i=319 "Seeing val. :"
 stat_sizer1->Add(new wxStaticText(this, -1, m_messg[319]),
                  0, wxLEFT, 4);
 ProcCtrl_SeeingThresh = new wxTextCtrl(this, ID_PROC_SEEING_THRESH,
                                        wxT("0.00,0.00"),
                                        wxPoint(-1, -1), wxSize(100, 28));
 stat_sizer1->Add(ProcCtrl_SeeingThresh, 0, wxLEFT, 6);
// i=320 "Max val. :"
 stat_sizer1->Add(new wxStaticText(this, -1, m_messg[320]),
                  0, wxLEFT, 12);
 ProcCtrl_MaxThresh = new wxTextCtrl(this, ID_PROC_MAX_THRESH, wxT("0.00,0.00"),
                                        wxPoint(-1, -1), wxSize(100, 28));
 stat_sizer1->Add(ProcCtrl_MaxThresh, 0, wxLEFT, 6);
 stat_sizer->Add(stat_sizer1, 0, wxALL, 10);
 processing_left_sizer->Add(stat_sizer, 0, wxEXPAND | wxBOTTOM, 10);

 hsizer2 = new wxBoxSizer(wxHORIZONTAL);

//********************** Image cubes: ********************************
// i=321 "Image cubes"
 cube_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[321]);
 irows = 2; icols = 2;
 vgap = 8; hgap = 10;
 fgs4 = new wxFlexGridSizer(irows, icols, vgap, hgap);
 fgs4->Add(new wxStaticText(this, -1, wxT("nz cubes :")));
 ProcCtrl_nz_cube = new wxTextCtrl(this, ID_PROC_NZ_CUBE, wxT("50"),
                                      wxPoint(-1, -1), wxSize(60, 28));
 fgs4->Add(ProcCtrl_nz_cube);

// KK (intercorrelations of two elementary frames separated by KK indices)
// i=322 "KK crosscor. :"
 ProcCmb_KK_CrossCorr.label = m_messg[322];
 ProcCmb_KK_CrossCorr.nchoices = 7;
 ProcCmb_KK_CrossCorr.choices[0] = wxT("4");
 ProcCmb_KK_CrossCorr.choices[1] = wxT("5");
 ProcCmb_KK_CrossCorr.choices[2] = wxT("6");
 ProcCmb_KK_CrossCorr.choices[3] = wxT("7");
 ProcCmb_KK_CrossCorr.choices[4] = wxT("8");
 ProcCmb_KK_CrossCorr.choices[5] = wxT("9");
 ProcCmb_KK_CrossCorr.choices[6] = wxT("10");
 ProcCmb_KK_CrossCorr.combo = new wxComboBox(this, ID_PROC_KK_CROSSCOR, wxT(""),
                                         wxDefaultPosition, wxSize(60, 28),
                                         ProcCmb_KK_CrossCorr.nchoices,
                                         ProcCmb_KK_CrossCorr.choices);
 fgs4->Add(new wxStaticText(this, -1, ProcCmb_KK_CrossCorr.label));
 fgs4->Add(ProcCmb_KK_CrossCorr.combo);
 cube_sizer->Add(fgs4, 0, wxALL, 10);

 hsizer2->Add(cube_sizer, 0, wxEXPAND);

//*********************** Save elementary images: **********************
// i=323 "Backup of the elementary images"
 save_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[323]);

// KK (intercorrelations of two elementary frames separated by KK indices)
// i=324 "Backup :"
 ProcCmb_Save_3DFits.label = m_messg[324];
 ProcCmb_Save_3DFits.nchoices = 3;
// i=325 "No backup"
 ProcCmb_Save_3DFits.choices[0] = m_messg[325];
// i=326 "All the images"
 ProcCmb_Save_3DFits.choices[1] = m_messg[326];
// i=327 "Selection only"
 ProcCmb_Save_3DFits.choices[2] = m_messg[327];
 ProcCmb_Save_3DFits.combo = new wxComboBox(this, ID_PROC_SAVE_3DFITS, wxT(""),
                                         wxDefaultPosition, wxSize(200, 28),
                                         ProcCmb_Save_3DFits.nchoices,
                                         ProcCmb_Save_3DFits.choices);
// save1_sizer->Add(new wxStaticText(this, -1, ProcCmb_Save_3DFits.label));
 save_sizer->Add(ProcCmb_Save_3DFits.combo, 0, wxALL, 10);

 hsizer1 = new wxBoxSizer(wxHORIZONTAL);

// i=328 "U.T./L.T. shift :"
 ProcCmb_UToffset.label = m_messg[328];
 ProcCmb_UToffset.nchoices = 3;
 ProcCmb_UToffset.choices[0] = "0";
 ProcCmb_UToffset.choices[1] = "-1";
 ProcCmb_UToffset.choices[2] = "-2";
 ProcCmb_UToffset.combo = new wxComboBox(this, ID_PROC_UT_OFFSET, wxT(""),
                                         wxDefaultPosition, wxSize(80, 28),
                                         ProcCmb_UToffset.nchoices,
                                         ProcCmb_UToffset.choices);
 hsizer1->Add(new wxStaticText(this, -1, ProcCmb_UToffset.label));
 hsizer1->Add(ProcCmb_UToffset.combo, 0, wxLEFT, 5);
 save_sizer->Add(hsizer1, 0, wxALL, 10);

 hsizer2->Add(save_sizer, 0, wxEXPAND | wxLEFT, 10);

 processing_left_sizer->Add(hsizer2, 0, wxALIGN_LEFT | wxBOTTOM, 10);

}
/********************************************************************
* SpeckProcess Setup panel (central part)
********************************************************************/
void JLP_SpeckProcessPanel::SpeckProcessPanel_CenterSetup(
                                    wxBoxSizer *processing_center_sizer)
{
wxButton *SelectOffsetButton, *SelectFFieldButton;
wxStaticBoxSizer *offset_sizer, *ffield_sizer, *options_sizer;
wxBoxSizer *hoffset, *hoffset1, *hffield;

//*********************** Offset correction: **********************
// i=330 "Offset : correction"
 offset_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[330]);

 hoffset = new wxBoxSizer(wxHORIZONTAL);
// i=331 "Threshold only"
 ProcRad_OffsetThreshold = new wxRadioButton(this, ID_PROC_OFFSET_THRESH,
                                             m_messg[331], wxPoint(-1,-1),
                                             wxSize(-1,-1), wxRB_GROUP);
 hoffset->Add(ProcRad_OffsetThreshold, 0);

// i=332 "Threshold and file"
 ProcRad_OffsetThresholdAndFrame = new wxRadioButton(this, ID_PROC_OFFSET_FRAME,
                                                     m_messg[332]);
 hoffset->Add(ProcRad_OffsetThresholdAndFrame, 0, wxLEFT, 10);
 offset_sizer->Add(hoffset, 0, wxALL, 10);

 hoffset1 = new wxBoxSizer(wxHORIZONTAL);
// i=333 "Offset threshold :"
 hoffset1->Add(new wxStaticText(this, -1, m_messg[333]),
              0, wxALIGN_CENTER);
 ProcCtrl_OffsetThresh = new wxTextCtrl(this, ID_PROC_OFFSET_THRESH,
                                        wxT("0.00"),
                                        wxPoint(-1, -1), wxSize(80, 28));
 hoffset1->Add(ProcCtrl_OffsetThresh, 0, wxLEFT, 10);
 offset_sizer->Add(hoffset1, 0, wxALL, 10);

// Offset file:
// i=334 "Offset file:"
 ProcStatic_OffsetFile = new wxStaticText(this, -1, m_messg[334]);
 offset_sizer->Add(ProcStatic_OffsetFile, 0, wxEXPAND | wxLEFT | wxBOTTOM, 10);

// i=335 "Select offset file:"
 SelectOffsetButton = new wxButton(this, ID_PROC_LOAD_OFFSET, m_messg[335]);
 offset_sizer->Add(SelectOffsetButton, 0, wxALL, 10);

 processing_center_sizer->Add(offset_sizer, 0, wxEXPAND | wxALL, 10);

//*********************** FField correction: **********************
// i=336 "Flat field corection"
 ffield_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[336]);
 hffield = new wxBoxSizer(wxHORIZONTAL);
// i=337 "No correction"
 ProcRad_NoFFieldCorr = new wxRadioButton(this, ID_PROC_NO_FFCORR,
                                          m_messg[337], wxPoint(-1,-1),
                                          wxSize(-1,-1), wxRB_GROUP);
 hffield->Add(ProcRad_NoFFieldCorr, 0, wxLEFT, 10);
// i=338 "Corr. with a file:"
 ProcRad_FFieldCorr = new wxRadioButton(this, ID_PROC_FFCORR, m_messg[338]);
 hffield->Add(ProcRad_FFieldCorr, 0, wxLEFT, 10);
// ffield_sizer->Add(hffield, 0, wxALL, 10);
 ffield_sizer->Add(hffield);

// FField file:
// i=339 "FField file:"
 ProcStatic_FFieldFile = new wxStaticText(this, -1, m_messg[339]);
 ffield_sizer->Add(ProcStatic_FFieldFile, 0, wxEXPAND | wxALL, 10);
// i=340 "Select a flat field file:"
 SelectFFieldButton = new wxButton(this, ID_PROC_LOAD_FFIELD,
                                   m_messg[340]);
 ffield_sizer->Add(SelectFFieldButton, 0, wxALL, 10);
 processing_center_sizer->Add(ffield_sizer, 0, wxEXPAND | wxALL, 10);

// ****************** Options ********************************************/
// i=341 "Options"
 options_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[341]);
// i=342 "Orientation vector"
 ProcCheck_DirectVector = new wxCheckBox(this, ID_PROC_DIRECT_VECTOR,
                                         m_messg[342]);
 options_sizer->Add(ProcCheck_DirectVector, 0, wxLEFT, 10);

// i=343 "Lucky imaging (with selection)"
 ProcCheck_Lucky = new wxCheckBox(this, ID_PROC_LUCKY_CHCK,
                                  m_messg[343]);
 options_sizer->Add(ProcCheck_Lucky, 0, wxALL, 10);

 processing_center_sizer->Add(options_sizer, 0, wxEXPAND | wxALL, 10);

}
/********************************************************************
* SpeckProcess Setup panel (right side)
********************************************************************/
void JLP_SpeckProcessPanel::SpeckProcessPanel_RightSetup(
                                   wxBoxSizer *processing_right_sizer)
{
wxButton *SelectUModsqButton, *SelectUAutocButton, *SelectPhotModsqButton;
wxStaticBoxSizer *aux_sizer, *dir_sizer;
wxBoxSizer *vsizer1, *vsizer2, *vsizer3;
int irows, icols, vgap = 10, hgap = 12;
wxFlexGridSizer *fgs4;

//*********************** Auxiliary files: **********************
// i=350 "Auxiliary files
 aux_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[350]);

// Unresolved autoc:
 vsizer1 = new wxBoxSizer(wxVERTICAL);
// i=351 "Autoc. of an unresolved star"
 vsizer1->Add(new wxStaticText(this, -1, m_messg[351]));
 ProcStatic_UnresAutocFile = new wxStaticText(this, -1, wxT(""));
 vsizer1->Add(ProcStatic_UnresAutocFile);
// i=352 "Select unresolved autoc"
 SelectUAutocButton = new wxButton(this, ID_PROC_LOAD_UNRES_AUTOC,
                                   m_messg[352]);
 vsizer1->Add(SelectUAutocButton);
 aux_sizer->Add(vsizer1, 0, wxALL, 10);

// Unresolved modsq:
 vsizer2 = new wxBoxSizer(wxVERTICAL);
// i=353 "Modsq. of an unresolved star"
 vsizer2->Add(new wxStaticText(this, -1, m_messg[353]));
 ProcStatic_UnresModsqFile = new wxStaticText(this, -1, wxT(""));
 vsizer2->Add(ProcStatic_UnresModsqFile);
// i=354 "Select unresolved modsq"
 SelectUModsqButton = new wxButton(this, ID_PROC_LOAD_UNRES_MODSQ,
                                   m_messg[354]);
 vsizer2->Add(SelectUModsqButton);
 aux_sizer->Add(vsizer2, 0, wxALL, 10);

// Photon modsq:
 vsizer3 = new wxBoxSizer(wxVERTICAL);
// i=355 "Modsq. of photon response"
 vsizer3->Add(new wxStaticText(this, -1, m_messg[355]));
 ProcStatic_PhotModsqFile = new wxStaticText(this, -1, wxT(""));
 vsizer3->Add(ProcStatic_PhotModsqFile);
// i=356 "Select photon modsq"
 SelectPhotModsqButton = new wxButton(this, ID_PROC_LOAD_PHOT_MODSQ,
                                      m_messg[356]);
 vsizer3->Add(SelectPhotModsqButton);
 aux_sizer->Add(vsizer3, 0, wxALL, 10);

 processing_right_sizer->Add(aux_sizer, 0, wxEXPAND | wxBOTTOM, 10);

//*********************** Directories: **********************
// i=360 "Directories"
 dir_sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_messg[360]);
 irows = 3;
 icols = 2;
 vgap = 16;
 hgap = 6;
 fgs4 = new wxFlexGridSizer(irows, icols, vgap, hgap);
// i=361 "Temporary files"
 fgs4->Add(new wxStaticText(this, -1, m_messg[361]));
 ProcCtrl_FFTW_dir = new wxTextCtrl(this, ID_PROC_FFTW_DIR, wxT("C:\\TMP_FFTW"),
                                    wxPoint(-1, -1), wxSize(100, 28));
 fgs4->Add(ProcCtrl_FFTW_dir);
// i=362 "Cubes 3D Fits"
 fgs4->Add(new wxStaticText(this, -1, m_messg[362]));
 ProcCtrl_3DFits_dir = new wxTextCtrl(this, ID_PROC_3DFITS_DIR,
                                      wxT("D:\\AndorCubes"),
                                      wxPoint(-1, -1), wxSize(100, 28));
 fgs4->Add(ProcCtrl_3DFits_dir);
// i=363 "Results"
 fgs4->Add(new wxStaticText(this, -1, m_messg[363]));
 ProcCtrl_Results_dir = new wxTextCtrl(this, ID_PROC_RESULTS_DIR,
                                       wxT("D:\\Nice_results"),
                                       wxPoint(-1, -1), wxSize(100, 28));
 fgs4->Add(ProcCtrl_Results_dir);
 dir_sizer->Add(fgs4, 0, wxALL, 8);

 processing_right_sizer->Add(dir_sizer, 0, wxEXPAND | wxBOTTOM, 8);

return;
}
