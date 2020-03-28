/************************************************************************
* "asp_display_panel_onclick.cpp"
* JLP_AspDispPanel class
*
* JLP
* Version 07/09/2015
*************************************************************************/
#include "asp_display_panel.h"
#include "jlp_decode.h"  // JLP_Decode class

 #define DEBUG

BEGIN_EVENT_TABLE(JLP_AspDispPanel, wxPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX0, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX1, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX2, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX3, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX4, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX5, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX6, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX7, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX8, JLP_AspDispPanel::OnSelectIPanel)
EVT_CHECKBOX(ID_IPANEL_CHECKBOX9, JLP_AspDispPanel::OnSelectIPanel)
END_EVENT_TABLE()

/********************************************************************
* Selection of a panel
********************************************************************/
void JLP_AspDispPanel::OnSelectIPanel(wxCommandEvent &event)
{
#ifdef DEBUG
wxString buffer;
#endif
int k_index;

if(initialized != 1234) return;

// Determine corresponding k index:
k_index = event.GetId() - ID_IPANEL_CHECKBOX0;
#ifdef DEBUG
buffer.Printf("DDEBUG/Getid=%d Selected panel index is : k_index=%d\n",
              event.GetId(), k_index);
wxMessageBox(buffer, wxT("OnSelectIPanel"), wxOK);
#endif

// Set active panel:
SetActiveIPanel(k_index);

// Update menu radio-items
m_aspframe->InitDisplaySettings(ipanel_settings1[k_index].is_linear,
                                ipanel_settings1[k_index].lut_type);

return;
}
/*********************************************************************
* Load an image to an IPanel
*
*********************************************************************/
void JLP_AspDispPanel::AspDisp_LoadImage(double *dble_image0, int nx0, int ny0,
                                 const int ipanel_index)
{
int index;

  if(ipanel_index < 0 || ipanel_index >= nChild_max) {
    index = active_ipanel_index;
    } else {
    index = ipanel_index;
    }

 pChildVisu[index]->wxIP_LoadImage(dble_image0, nx0, ny0);

return;
}
/**********************************************************************
* Update all the display panels according
* to ModelDisplayOption0 for the model panel
*
* ModelDisplayOption:
* -1=none, 0=Offset, 1=FlatField, 2=UnresAutoc, 3=UnresModsq
* 4=Quadrant, 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector 8=PhotModsq,
* 9=Bispectrum, 10=RestoredImage
***********************************************************************/
int JLP_AspDispPanel::UpdateChildWindows(JLP_Decode *Decod2,
                                         int ModelDisplayOption0)
{
int k, k_start, k_decode_index;
double* dble_image2 = NULL;
int nx2, ny2, nz2_cube, status;
char lut_type[32];

// Return -1 if Decod2 is not yet initialized:
if(Decod2 == NULL) return(-1);

// Get current values of arrays from JLP_Decode:
  Decod2->GetNxNyNz_cube(&nx2, &ny2, &nz2_cube);

  dble_image2 = new double[nx2 * ny2];

// Update the ChildVisu1 windows:
  k_start = 0;
  for(k = k_start; k < nChild; k++) {
    status = -1;
/*
* k_decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*/

   if(pChildVisu[k] != NULL) {
     k_decode_index = ipanel_decode_index[k];
// Special case of Model (first child window):
     if(k == 0) {
       status = UpdateFirstChildWindow(Decod2, dble_image2, nx2, ny2, ModelDisplayOption0);
// Switch to color LUT:
       strcpy(lut_type, "pisco");
       pChildVisu[0]->wxIP_SetLUT(lut_type);
       } else {
       status = Decod2->ReadDbleImage(dble_image2, nx2, ny2, k_decode_index);
       }

     if(!status) {
       pChildVisu[k]->wxIP_LoadImage(dble_image2, nx2, ny2);
       }
   } // EOF pChildVisu[k] != NULL
 }  //EOF loop on k


if(dble_image2 != NULL) delete[] dble_image2;

return 0;
}
/**********************************************************************
* Update all the display panels from Display2Mutex
*
* ModelDisplayOption:
* -1=none, 0=Offset, 1=FlatField, 2=UnresAutoc, 3=UnresModsq
* 4=Quadrant, 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector 8=PhotModsq,
* 9=Bispectrum, 10=RestoredImage
***********************************************************************/
int JLP_AspDispPanel::UpdateChildWindows0(double *child_image2[NCHILDS_MAX],
                                          const int nx2, const int ny2)
{
int k;

// Update the ChildVisu1 windows:
  for(k = 0; k < nChild; k++) {
/*
* k_decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*/
   if(pChildVisu[k] != NULL) {
//      k_decode_index = ipanel_decode_index[k];
//       status = Decod2->ReadDbleImage(dble_image2, nx2, ny2, k_decode_index);
      pChildVisu[k]->wxIP_LoadImage(child_image2[k], nx2, ny2);
   } // EOF pChildVisu[k] != NULL
 }  //EOF loop on k

return(0);
}
/**********************************************************************
* Called by external routines (e.g. DecodeThread or AspFrame
***********************************************************************/
void JLP_AspDispPanel::RefreshDisplay()
{
Refresh();
}
/**********************************************************************
* Refresh the model panel
*
* ModelDisplayOption:
* -1=none, 0=Offset, 1=FlatField, 2=UnresAutoc, 3=UnresModsq
* 4=Quadrant, 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector 8=PhotModsq,
* 9=Bispectrum, 10=RestoredImage
*
* decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*
***********************************************************************/
int JLP_AspDispPanel::UpdateFirstChildWindow(JLP_Decode *Decod2,
                                            double *dble_image2,
                                            int nx2, int ny2,
                                            const int ModelDisplayOption0)
{
int status, imodel = 0, k_decode_index;

// Return -1 if Decod2 is not yet initialized:
if(!Decod2) return(-1);

   switch(ModelDisplayOption0) {
// Display offset:
      case -1:
         k_decode_index = ipanel_decode_index[0];
         IPanelTitle[imodel] = DecodeName[k_decode_index];
         status = Decod2->ReadDbleImage(dble_image2, nx2, ny2, k_decode_index);
         if(status) wxMessageBox(wxT("Error loading short exposure"),
                               "Main_Refresh", wxOK | wxICON_ERROR);
         break;
// Display offset:
      case 0:
         IPanelTitle[imodel] = wxT("Offset");
         status = Decod2->ReadOffsetFrame(dble_image2, nx2, ny2);

         if(status) wxMessageBox(wxT("Error loading Offset"),
                               "Main_Refresh", wxOK | wxICON_ERROR);
         break;
// Display flat field:
       case 1:
         IPanelTitle[imodel] = wxT("Flat field");
         status = Decod2->ReadFlatFieldFrame(dble_image2, nx2, ny2);
         break;
// Display unresolved autocorrelation:
       case 2:
         IPanelTitle[imodel] = wxT("Unres. autoc.");
         status = Decod2->ReadUnresAutocFrame(dble_image2, nx2, ny2);
         break;
// Display unresolved power spectrum (modsq):
       case 3:
         IPanelTitle[imodel] = wxT("Unres. modsq");
         status = Decod2->ReadUnresModsqFrame(dble_image2, nx2, ny2);
         break;
// Quadrant:
       case 4:
         IPanelTitle[imodel] = wxT("Quadrant");
         status = Decod2->ReadDbleImage(dble_image2, nx2, ny2, 4);
        break;
// Display flattened autocorrelation:
       case 5:
         IPanelTitle[imodel] = wxT("Autoc. - unres.");
         status = Decod2->ReadDbleImage(dble_image2, nx2, ny2, 5);
        break;
// Display the "lucky image"
       case 6:
         IPanelTitle[imodel] = wxT("Lucky image");
         status = Decod2->ReadDbleImage(dble_image2, nx2, ny2, 6);
         break;
// Display the "direct vector"
       case 7:
         IPanelTitle[imodel] = wxT("Direct vector");
         status = Decod2->ReadDbleImage(dble_image2, nx2, ny2, 7);
         break;
// Display PhotModsq:
       case 8:
         IPanelTitle[imodel] = wxT("PhotModsq");
         status = Decod2->ReadPhotModsqFrame(dble_image2, nx2, ny2);
         break;
// Display Bispectrum:
       case 9:
         IPanelTitle[imodel] = wxT("Bispectrum");
         status = Decod2->ReadBispectrumFrame(dble_image2, nx2, ny2);
         break;
// Display RestoredImage:
       case 10:
         IPanelTitle[imodel] = wxT("RestoredImage");
         status = Decod2->ReadRestoredImageFrame(dble_image2, nx2, ny2);
         break;
       default:
        status = -1;
        break;
      }

// Set label:
  if(MyIPanelCheckBox[0]) {
    (MyIPanelCheckBox[0])->SetLabel(IPanelTitle[imodel]);
    }

return(status);
}
/****************************************************************************
* Update Child windows of DisplayPanel according to processing setup Pset0
****************************************************************************/
void JLP_AspDispPanel::NewDisplaySetup(PROCESS_SETTINGS Pset0)
{
int k, index;
DISPLAY_SETTINGS ipanel_settings0;

  if(initialized != 1234) return;

// Determine new value of nChild:
  NewSettingsOfIPanels(Pset0, nChild);
  if(nChild == old_nChild) return;

// Save new value of nChild:
  old_nChild = nChild;

// Show the new child windows and associated check boxes
// (in case they were previously hidden):
  for(k = 0; k < nChild; k++) {
    if(MyIPanelCheckBox[k]) {
      index = ipanel_decode_index[k];
      IPanelTitle[k] = DecodeTitle[index];
      (MyIPanelCheckBox[k])->SetLabel(IPanelTitle[k]);
      (MyIPanelCheckBox[k])->Show(true);
      }

// Initialize all the options:
    Init_IPanelOptions(pChildVisu[k], index, &ipanel_settings0);
// Load settings to private structure:
    ipanel_settings1[k] = ipanel_settings0;

// Erase panel and show it:
    (pChildVisu[k])->wxIP_Erase();
    (pChildVisu[k])->Show(true);
   }

// Hide all the non-needed child windows and associated check boxes:
  for(k = nChild; k < nChild_max; k++) {
    if(MyIPanelCheckBox[k]) (MyIPanelCheckBox[k])->Show(false);
    (pChildVisu[k])->Show(false);
   }

// Valid changes:
 GetSizer()->Layout();

// Send an event equivalent to resize,
// which repaints the panel with a good format
 Refresh();
 Update();

return;
}
