/******************************************************************************
* Name:        asp2_frame_menu_onclick (AspFrame class)
* Purpose:     handling menu events of AspFrame clas (see Filespeck2.cpp)
* Author:      JLP
* Version:     22/02/2016
******************************************************************************/
#include "asp2_frame.h"
#include "asp2_frame_id.h"  // Menu identifiers
//#include "asp2_display_panel.h" // JLP_AspDispPanel class

/******************************************************************
* Display statistics on the last processing
*
******************************************************************/
void AspFrame::OnShowStatistics(wxCommandEvent& WXUNUSED(event))
{
int status;
wxString buffer;
PROCESS_SETTINGS Pset0;

 if(initialized != 1234) return;

 if(m_decode_thread == NULL) return;

// Get Results, i.e. ask if processing is finished
// and jlp_decode1 pointer is free
 status = m_decode_thread->GetResults();
 if(status) {
  wxMessageBox(wxT("Error retrieving data from DecodeThread"),
               wxT("OnShowStatistics"), wxOK | wxICON_ERROR);
  return;
 }

/* Compute statistics
* FwhmStats and MeanStats character strings with information about
* FWHM and mean level statistics
*/
  jlp_decode1->ComputeStatistics();

  jlp_decode1->Get_PSET(Pset0);
  buffer.Printf(wxT("Preprocessing: Thresh=%d Offset=%d FField=%d\n\n\
Frame selection with MaxVal = %d (min=%.1f max=%.1f)\n\n\
Frame selection with FWHM = %d (min=%.1f max=%.1f)\n\n\
############ Results obtained: ############ \n\n\
 %s\n\n %s"),
  Pset0.OffsetThreshold,
  (int)Pset0.OffsetFrameCorrection,
  (int)Pset0.FlatFieldCorrection,
  Pset0.Selection_maxval, Pset0.MaxValSelect.LowCut,
  Pset0.MaxValSelect.HighCut, Pset0.Selection_fwhm,
  Pset0.FwhmSelect.LowCut, Pset0.FwhmSelect.HighCut,
  Pset0.MaxValSelect.Stats, Pset0.FwhmSelect.Stats);
  wxMessageBox(buffer,wxT("Statistics on the results"), wxOK);

return;
}
/******************************************************************
* Restore Image from bispectrum
******************************************************************/
void AspFrame::OnBispRestore(wxCommandEvent& event)
{
int status = -1;
double xphot;
wxString buffer;
PROCESS_SETTINGS Pset0;

 if(initialized != 1234) return;

if(input_processed_data1 == false) {
 if(m_decode_thread == NULL) return;
// Get Results, i.e. ask if processing is finished
// and jlp_decode1 pointer is free
 status = m_decode_thread->GetResults();
 if(status || (jlp_decode1 == NULL)) {
  wxMessageBox(wxT("Error retrieving data from DecodeThread"),
               wxT("OnBispRestore"), wxOK | wxICON_ERROR);
  return;
 }
}

if(Pset1.PhotModsqFileName[0] == '\0') {
  status = jlp_speckprocess_panel->LoadPhotModsqFilenameToPset1();
  if(status == 0) {
    jlp_speckprocess_panel->Get_PSET(Pset0);
    jlp_decode1->DC_LoadPhotModsqFrame(Pset0.PhotModsqFileName);
    strcpy(Pset1.PhotModsqFileName, Pset0.PhotModsqFileName);
    } else {
    wxMessageBox(wxT("Error loading PhotModsqFilename"),
               wxT("OnBispRestore"), wxOK | wxICON_ERROR);
     return;
    }
 }

/* int JLP_Decode::DC_ImageFromBispectrum(double *xphot)
*/
 status = jlp_decode1->DC_ImageFromBispectrum(&xphot);

// Display RestoreImage to first child panel:
if(status == 0) {
 event.SetId(ID_DISPMENU_OPT10);
 OnSelectDisplayOption(event);
 menuDisplay->Check(ID_DISPMENU_OPT10, true);
 }

return;
}
/******************************************************************
* OnSelectDisplayOption
*
* ModelDisplayOption:
* -1=none, 0=Offset, 1=FlatField, 2=UnresAutoc, 3=UnresModsq
* 4=Quadrant, 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector 8=PhotModsq,
* 9=Bispectrum, 10=RestoredImage
*
* decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*
**************************************************************************/
void AspFrame::OnSelectDisplayOption(wxCommandEvent& event)
{
int ModelDisplayOption1, status, iPage;

// Select graphic panel:
iPage = 2;
m_notebook->SetSelection(iPage);

switch(event.GetId()) {
  default:
  case ID_DISPMENU_DEFAULT:
    ModelDisplayOption1 = -1;
    break;
  case ID_DISPMENU_OFFSET:
    ModelDisplayOption1 = 0;
    break;
  case ID_DISPMENU_FFIELD:
    ModelDisplayOption1 = 1;
    break;
  case ID_DISPMENU_UNRES_AUTOC:
    ModelDisplayOption1 = 2;
    break;
  case ID_DISPMENU_UNRES_MODSQ:
    ModelDisplayOption1 = 3;
    break;
  case ID_DISPMENU_OPT4:
    ModelDisplayOption1 = 4;
    break;
  case ID_DISPMENU_OPT5:
    ModelDisplayOption1 = 5;
    break;
  case ID_DISPMENU_OPT6:
    ModelDisplayOption1 = 6;
    break;
  case ID_DISPMENU_OPT7:
    ModelDisplayOption1 = 7;
    break;
  case ID_DISPMENU_OPT8:
    ModelDisplayOption1 = 8;
    break;
  case ID_DISPMENU_OPT9:
    ModelDisplayOption1 = 9;
    break;
  case ID_DISPMENU_OPT10:
    ModelDisplayOption1 = 10;
    break;
}

if(m_decode_thread->IsAlive()) {
// Get Results, i.e. ask if processing is finished
// and jlp_decode1 pointer is free
  status = m_decode_thread->GetResults();
  if((status == 0) && (jlp_decode1 != NULL)) {
    jlp_aspdisp_panel->UpdateChildWindows(jlp_decode1, ModelDisplayOption1);
    jlp_aspdisp_panel->RefreshDisplay();
   }
}
return;
}
/************************************************************************
* Change LUT
* lut_type: 'l'=log rainbow1 'r'=rainbow2 's'=saw 'g'=gray 'c'=curves
*           'p'=pisco
*
* NB: wxWidgets automatically toggles the flag value when the item is clicked
************************************************************************/
void AspFrame::OnChangeLUT(wxCommandEvent& event)
{
char lut_type[64];

  switch (event.GetId())
  {
// Rainbow1:
   case ID_LUT_RAIN1:
     strcpy(lut_type,"log rainbow1");
     break;
// Rainbow2:
   case ID_LUT_RAIN2:
     strcpy(lut_type,"rainbow2");
     break;
// Saw:
   case ID_LUT_SAW:
     strcpy(lut_type,"saw");
     break;
// Gray:
   case ID_LUT_GRAY:
     strcpy(lut_type,"gray");
     break;
// For curves:
   case ID_LUT_CUR:
     strcpy(lut_type,"curves");
     break;
// Pisco-like:
   default:
   case ID_LUT_PISCO:
     strcpy(lut_type,"pisco");
     break;
   }

 jlp_aspdisp_panel->SetLUT_ActivePanel(lut_type);

return;
}
/************************************************************************
* Change the type of the ITT (Intensity Transfer Table used for the display)
*
* The ITT table is filled with the indices k to be used for LUT conversion
* of a double precision image
************************************************************************/
void AspFrame::OnChangeITT(wxCommandEvent& event)
{
int is_linear = 1;

  switch (event.GetId())
  {
// Switch to linear scale:
   default:
   case ID_ITT_LIN:
     is_linear = 1;
     break;
// Switch to logarithmic scale
   case ID_ITT_LOG:
     is_linear = 0;
     break;
  }

 jlp_aspdisp_panel->SetITT_ActivePanel(is_linear);


return;
}
