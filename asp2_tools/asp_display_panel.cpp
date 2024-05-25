/************************************************************************
* "asp_display_panel.cpp"
* JLP_AspDispPanel class
*
* JLP
* Version 07/09/2015
*************************************************************************/
#include "asp_display_panel.h"
#include "jlp_decode.h"  // JLP_Decode class

// #define DEBUG

/*******************************************************************************
* Constructor:
*******************************************************************************/
JLP_AspDispPanel::JLP_AspDispPanel(AspFrame *asp_frame0, wxFrame *frame0,
                                   JLP_wxLogbook *logbook,
                                   wxStatusBar *status_bar, wxString *str_messg,
                                   const int n_messg,
                                   const int width0, const int height0,
                                   bool small_version)
                                   : wxPanel(frame0)
{
int k;
wxString input_filename;

 initialized = 0;

 m_aspframe = asp_frame0;
 old_nChild = 0;

// Transform coma into point for numbers:
 setlocale(LC_NUMERIC, "C");

// Smaller fonts (needed for Windows, to have a similar appearance as Linux)
if(small_version) SetWindowVariant(wxWINDOW_VARIANT_SMALL);

// Messages (in different languages)
 m_messg = new wxString[n_messg];
 for(k = 0; k < n_messg; k++) m_messg[k] = str_messg[k];

 m_StatusBar = status_bar;
 m_Logbook = logbook;
 nChild_max = 6;

// Setup panel
 DisplayPanelSetup(width0, height0);

 initialized = 1234;

// Select first panel (after initialized = 1234 !)
  MyIPanelCheckBox[0]->SetValue(true);
  SetActiveIPanel(0);

return;
}
/*******************************************************************************
* To avoid memory leaks...
*******************************************************************************/
void JLP_AspDispPanel::MyFreeMemory()
{
// Error here, so I don't free memory:
// if(m_messg != NULL) delete[] m_messg;
return;
}
/**********************************************************************
* DisplayPanel setup
* Create DisplayPanel with image panels:
*
***********************************************************************/
void JLP_AspDispPanel::DisplayPanelSetup(const int width0, const int height0)
{
wxBoxSizer *main_vtop_sizer, *hbox_sizer0[2];
wxBoxSizer *vbox_sizer1[NCHILDS_MAX], *hbox_sizer1[NCHILDS_MAX];
int width1, height1, should_fit_inside0;
int ix, iy, i, j, k;

main_vtop_sizer = new wxBoxSizer( wxVERTICAL );

// Create 10 subpanels (from ID_IPANEL_CHECKBOX0 to ID_IPANEL_CHECKBOX9):

// Size of subpanels:
 width1 = (width0 - 40)/3;
 height1 = (height0 - 40)/2;

 k = 0;
 for(j = 0; j < 2; j++) {
   hbox_sizer0[j] = new wxBoxSizer( wxHORIZONTAL );
   for(i = 0; i < nChild_max/2; i++) {
     hbox_sizer1[k] = new wxBoxSizer( wxHORIZONTAL );
     vbox_sizer1[k] = new wxBoxSizer( wxVERTICAL );
// wxID values:
     MyIPanelCheckBox_id[k] = ID_IPANEL_CHECKBOX0 + k;
     k++;
    }
  }

Init_IPanelTitles();

/*
* ipanel_decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*/
k = 0;
for(j = 0; j < 2; j++) {
  iy = j * height1;
  for(i = 0; i < nChild_max/2; i++) {
    ix = i * width1;
// Title:
    MyIPanelCheckBox[k] = new wxCheckBox(this, MyIPanelCheckBox_id[k],
                                         IPanelTitle[k],
                                         wxPoint(0,0), wxSize(250, 25));
//    Connect(MyIPanelCheckBox_id[k], wxEVT_CHECKBOX,
//            wxCommandEventHandler(JLP_AspDispPanel::OnSelectIPanel));
    hbox_sizer1[k]->Add(MyIPanelCheckBox[k], 10, wxLEFT);
    vbox_sizer1[k]->Add(hbox_sizer1[k], 0, 1, wxALL);

// JLP2016: full frame should fit in sub-panel
   should_fit_inside0 = 1;

// Scrollwindow for image display (and NULL for JLP_wxLogbook):
// pChildVisu[k] = new JLP_wxImagePanel((wxFrame *)this, NULL, m_StatusBar,
//                                      20 + ix, 20 + iy, width1, height1, 1);
// New: attempt with JLP_wxLogbook:
    pChildVisu[k] = new JLP_wxImagePanel((wxFrame *)this, m_Logbook,
                                         m_StatusBar,
                                         20 + ix, 20 + iy, width1, height1,
                                         should_fit_inside0);

// 1: vertically strechable
// wxEXPAND | xALL: horizontally strechable without borders
    vbox_sizer1[k]->Add(pChildVisu[k], 1, wxEXPAND);
    hbox_sizer0[j]->Add(vbox_sizer1[k], 1, wxEXPAND);
    k++;
   }
// 1 : vertically strechable
// wxEXPAND : horizontally strechable
  main_vtop_sizer->Add(hbox_sizer0[j], 1, wxEXPAND);
}

this->SetSizerAndFit(main_vtop_sizer);

return;
}
/********************************************************************
* Initialize panel names
*
* ipanel_decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*
********************************************************************/
void JLP_AspDispPanel::Init_IPanelTitles()
{
int i;

// i=400 "Short exposure"
DecodeName[0] = wxT("ShortExp");
DecodeTitle[0] = m_messg[400];
// i=401 "Power spectrum"
DecodeName[1] = wxT("Modsq");
DecodeTitle[1] = m_messg[401];
// i=402 "Autocorrelation"
DecodeName[2] = wxT("Autocc");
DecodeTitle[2] = m_messg[402];
// i=403 "Long integration"
DecodeName[3] = wxT("LongInt");
DecodeTitle[3] = m_messg[403];
// i=404 "Quadrant"
DecodeName[4] = wxT("Quadrant");
DecodeTitle[4] = m_messg[404];
// i=404 "Autoc - unres. autoc"
DecodeName[5] = wxT("FlattenedAutoc");
DecodeTitle[5] = m_messg[405];
// i=405 "Lucky image"
DecodeName[6] = wxT("LuckyImage");
DecodeTitle[6] = m_messg[406];
// i=342 "Direct vector"
DecodeName[7] = wxT("DirectVector");
DecodeTitle[7] = m_messg[342];

DecodeName[8] = wxT("");
DecodeTitle[8] = wxT("Model");

// First initialization (NB: later "model" ipanel will be modified...)
nChild_max = 6;
for(i = 0; i < nChild_max; i++) {
  IPanelTitle[i] = DecodeTitle[i];
  ipanel_decode_index[i] = i;
  }

return;
}
/****************************************************************************
*
* decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
****************************************************************************/
void JLP_AspDispPanel::Init_IPanelOptions(JLP_wxImagePanel *pChildVisu,
                                          int decode_index,
                                          DISPLAY_SETTINGS *ipanel_settings0)
{
int itt_is_linear, filter_iopt;
char lut_type[32], itt_type[32];

  if(initialized != 1234) return;

// Default options:
 itt_is_linear = 1;
 strcpy(lut_type, "pisco");
 strcpy(itt_type, "MinMax");

// filter : 0= none
// 1=soft unsharp (UNSH1) 2=medium unsharp (UNSH2) 3=hard unsharp (UNSH3)
// 4=high contrast1 (HC1) 5=high contrast2 (HC2) 6=Very high contrast (VHC)
// No filter by default

 filter_iopt = 0;

// Display settings according to displayed data:
 switch (decode_index) {
// ShortExp
   case 0:
// Log
     itt_is_linear = 0;
     strcpy(lut_type, "gray");
     break;
// Modsq
   case 1:
     strcpy(itt_type, "HC");
     break;
// Autocc
   case 2:
//     filter_iopt = 3;
     break;
// LongInt
   case 3:
// Log
     itt_is_linear = 0;
     break;
// Quadrant
   case 4:
     break;
   default:
     strcpy(itt_type, "MinMax");
     strcpy(lut_type, "pisco");
     itt_is_linear = 1;
     break;
  }

 pChildVisu->wxIP_SetLUT(lut_type);
 pChildVisu->wxIP_SetITT_type(itt_type);
 pChildVisu->wxIP_SetITT_linear(itt_is_linear);
 pChildVisu->wxIP_SetFilter(filter_iopt);

 ipanel_settings0->is_linear = itt_is_linear;
 ipanel_settings0->fixed_thresholds = 0;
 strcpy(ipanel_settings0->lut_type, lut_type);
return;
}
/****************************************************************************
* Called by the menu of AspFrame
****************************************************************************/
void JLP_AspDispPanel::SetITT_ActivePanel(const int is_linear)
{
// Set ITT of active panel:
  active_ipanel->wxIP_SetITT_linear(is_linear);

// Update display with new setup:
  active_ipanel->wxIP_UpdateImageDisplay();

  ipanel_settings1[active_ipanel_index].is_linear = is_linear;
}
/****************************************************************************
* Called by the menu of AspFrame
****************************************************************************/
void JLP_AspDispPanel::SetLUT_ActivePanel(char *lut_type)
{
// Set LUT type to active panel:
  active_ipanel->wxIP_SetLUT(lut_type);

// Update display with new setup:
  active_ipanel->wxIP_UpdateImageDisplay();
  strcpy(ipanel_settings1[active_ipanel_index].lut_type, lut_type);
}
/****************************************************************************
*
****************************************************************************/
void JLP_AspDispPanel::NewSettingsOfIPanels(PROCESS_SETTINGS Pset0,
                                            int &nChild0)
{
int i;

/*
* ProcessingMode = 0 : no processing (only numerisation)
* ProcessingMode = 1 : Only integration
* ProcessingMode = 2 : autoc - inter, FFT modulus, long integration
* ProcessingMode = 3 : autoc - inter, FFT modulus, long integration, quadrant
* ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
* ProcessingMode = 5 : Bispectrum, autoc, quadrant
* ProcessingMode = 6 : Spectroscopy
* ProcessingMode = 7 : SCIDAR Log
* ProcessingMode = 8 : SCIDAR Lin
* ProcessingMode = 9 : Statistics
*/
/*
* decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*/

 switch (Pset0.ProcessingMode) {
//**** ProcessingMode = 0 : no processing (only numerisation)
   case 0:
     nChild0 = 1;
// Short exp
     i = 0;
     ipanel_decode_index[i] = 0;
     break;
//**** ProcessingMode = 1 : Only integration
   case 1:
     nChild0 = 2;
// Short exp
     i = 0;
     ipanel_decode_index[i] = 0;
// LongInt
     i++;
     ipanel_decode_index[i] = 3;
     break;
//**** ProcessingMode = 2 : autoc-inter, FFT modulus, long integration
//**** ProcessingMode = 3 : autoc-inter, FFT modulus, long integration, quadrant
//**** ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
//**** ProcessingMode = 5 : Bispectrum, autoc, quadrant
   default:
   case 2:
   case 3:
   case 4:
   case 5:
     nChild0 = 6;
// Short exp
     i = 0;
     ipanel_decode_index[i] = 0;
// LongInt
     i++;
     ipanel_decode_index[i] = 3;
// Modsq
     i++;
     ipanel_decode_index[i] = 1;
// Autocc
     i++;
     ipanel_decode_index[i] = 2;
// Quadrant
     i++;
     ipanel_decode_index[i] = 4;
// FlattenedAutoc
     i++;
     ipanel_decode_index[i] = 5;
// Add direct vector instead of flattened autoc:
     if(Pset0.DirectVector) ipanel_decode_index[5] = 7;
     break;
/* 
* ProcessingMode = 7 : SCIDAR Log
* Processcube routine updates the arrays:
*  long_int[], autocc[], modsq[], ShortExp_v[]
* Long integration of the fluctuations: (Scidar Log mode only)
*  scidar_lf[]
*
* ProcessingMode = 8 : SCIDAR Lin
* Processcube routine updates the arrays:
*  long_int[], autocc[], modsq[], ShortExp_v[]
*/
   case 7:
   case 8:
    nChild0 = 4;
// Short exp
     i = 0;
     ipanel_decode_index[i] = 0;
// LongInt
     i++;
     ipanel_decode_index[i] = 3;
// Modsq
     i++;
     ipanel_decode_index[i] = 1;
// Autocc
     i++;
     ipanel_decode_index[i] = 2;
     break;
   }


return;
}
/********************************************************************
* Selection of active panel
********************************************************************/
void JLP_AspDispPanel::SetActiveIPanel(int k_index)
{
int k;

if(initialized != 1234) return;

active_ipanel_index = k_index;
active_ipanel = pChildVisu[k_index];
active_ipanel->SetFocus();

// Uncheck all boxes:
for(k= 0; k < nChild; k++) MyIPanelCheckBox[k]->SetValue(false);

// Check selected panel checkbox:
MyIPanelCheckBox[k_index]->SetValue(true);

return;
}
