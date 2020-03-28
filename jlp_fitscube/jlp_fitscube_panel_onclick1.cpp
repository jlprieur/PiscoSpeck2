/****************************************************************************
* Name: jlp_fitscube_panel_onclick1.cpp
*
* JLP
* Version 16/11/2017
****************************************************************************/
#include "jlp_fitscube_panel.h"

#define ABS(a) ((a) >= 0 ? (a) : (-(a)))

/********************************************************************
* ROICenter LabeledEdit box
* WARNING: should wait before doing something anything to avoid blocking !
*
********************************************************************/
void JLP_FitsCubePanel::OnROICenterOnExit(wxCommandEvent& event)
{
if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/********************************************************************
* ROICenter LabeledEdit box
* Set new value for the center of elementary images
*     (relative to the full CCD matrix)
*
********************************************************************/
void JLP_FitsCubePanel::UpdateROICenter(const bool update_from_screen)
{
wxString buffer0;
char buffer[64];
int ix, iy, ixc0, iyc0, ixc, iyc;
int delta_xmax, delta_ymax;

if(update_from_screen) {
 buffer0 = FitsCubeCtrl_ROICenter->GetValue();
 strcpy(buffer, buffer0.mb_str());
 if(sscanf(buffer, "%d,%d", &ix, &iy) == 2) {
   FitsCubeSet1.xc0 = ix;
   FitsCubeSet1.yc0 = iy;
 }
}

// Image center:
 ixc0 = (FitsCubeSet1.nx1 * FitsCubeSet1.xbin) /2;
 iyc0 = (FitsCubeSet1.ny1 * FitsCubeSet1.ybin) /2;
 ixc = FitsCubeSet1.gblXPixels/2;
 iyc = FitsCubeSet1.gblYPixels/2;
 delta_xmax = MAXI(0, ixc - ixc0);
 delta_ymax = MAXI(0, iyc - iyc0);

// If error, erase the new value and set to the default value
 if((FitsCubeSet1.xc0 < ixc - delta_xmax) || (FitsCubeSet1.xc0 > ixc + delta_xmax)
     || (FitsCubeSet1.yc0 < iyc - delta_ymax) || (FitsCubeSet1.yc0 > iyc + delta_ymax)) {
/*
   buffer0.Printf(wxT("Value=%d,%d: valid range is [%d %d] for xcent and [%d %d] for ycent"),
                  FitsCubeSet1.xc0, FitsCubeSet1.yc0, ixc - delta_xmax, ixc + delta_xmax,
                  iyc - delta_ymax, iyc + delta_ymax);
*/
   buffer0.Printf(wxT("Value=%d,%d: gbl %d %d ixc, iyc [%d %d] ixc0, iyc0 [%d %d]"),
                  FitsCubeSet1.xc0, FitsCubeSet1.yc0, FitsCubeSet1.gblXPixels, FitsCubeSet1.gblYPixels,
                  ixc, iyc, ixc0, iyc0);
   wxMessageBox(buffer0, wxT("ROICenterOnExit/Error"), wxOK | wxICON_ERROR);
   FitsCubeSet1.xc0 = FitsCubeSet1.gblXPixels/2;
   FitsCubeSet1.yc0 = FitsCubeSet1.gblYPixels/2;
  }

// Display new value :
  buffer0.Printf(wxT("%d,%d"), FitsCubeSet1.xc0, FitsCubeSet1.yc0);
  FitsCubeCtrl_ROICenter->SetValue(buffer0);

return;
}
/****************************************************************************
*
****************************************************************************/
void JLP_FitsCubePanel::OnBinFactorComboBoxChange(wxCommandEvent& event)
{
int index, nx0, ny0;

if(initialized != 1234) return;

nx0 = FitsCubeSet1.nx1 * FitsCubeSet1.xbin;
ny0 = FitsCubeSet1.ny1 * FitsCubeSet1.ybin;

// Handle Binning ComboBox for bin factor
// Values: 1, 2, 4 or 8

index = FitsCubeCmb_BinFactor.combo->GetSelection();
switch (index) {
   default:
   case 0:
     FitsCubeSet1.xbin = 1;
     FitsCubeSet1.ybin = 1;
     break;
   case 1:
     FitsCubeSet1.xbin = 2;
     FitsCubeSet1.ybin = 2;
     break;
   case 2:
     FitsCubeSet1.xbin = 4;
     FitsCubeSet1.ybin = 4;
     break;
   case 3:
     FitsCubeSet1.xbin = 8;
     FitsCubeSet1.ybin = 8;
     break;
   }

// ROI size should remain the same afetr bin change:
 FitsCubeSet1.nx1 = nx0 / FitsCubeSet1.xbin;
 FitsCubeSet1.ny1 = ny0 / FitsCubeSet1.ybin;

// Update final image size in static text label:
 UpdateFinalImageSizeLabels();

 jcam_changes1->ChangesDone = true;

return;
}
/***************************************************************************
* Selection of new item in Region Of Interest Size ComboBox
*
* New ROI size FitsCubeSet1.nx0, FitsCubeSet1.ny0
* 0 = 64x64
* 1 = 128x128
* 2 = 256x256
* 3 = 512x512
* 4 = full size
*
***************************************************************************/
void JLP_FitsCubePanel::OnROISizeComboBoxChange(wxCommandEvent& event)
{
int index, nx0, ny0;

if(initialized != 1234) return;

index = FitsCubeCmb_ROISize.combo->GetSelection();
switch (index) {
   case 0:
     nx0 = 64;
     ny0 = 64;
     break;
   default:
   case 1:
     nx0 = 128;
     ny0 = 128;
     break;
   case 2:
     if((FitsCubeSet1.gblXPixels >= 256)
         && (FitsCubeSet1.gblXPixels >= 256)) {
       nx0 = 256;
       ny0 = 256;
     } else {
       nx0 = FitsCubeSet1.gblXPixels;
       ny0 = FitsCubeSet1.gblYPixels;
     }
     break;
   case 3:
     if((FitsCubeSet1.gblXPixels >= 512)
         && (FitsCubeSet1.gblXPixels >= 512)) {
       nx0 = 512;
       ny0 = 512;
     } else {
       nx0 = FitsCubeSet1.gblXPixels;
       ny0 = FitsCubeSet1.gblYPixels;
     }
     break;
   case 4:
     nx0 = FitsCubeSet1.gblXPixels;
     ny0 = FitsCubeSet1.gblYPixels;
     break;
   }
   FitsCubeSet1.nx1 = nx0 / FitsCubeSet1.xbin;
   FitsCubeSet1.ny1 = ny0 / FitsCubeSet1.ybin;
   UpdateFinalImageSizeLabels();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* Mirror ComboBox
****************************************************************************/
void JLP_FitsCubePanel::OnMirrorComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(FitsCubeCmb_Mirror.combo)
    FitsCubeSet1.MirrorMode = FitsCubeCmb_Mirror.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* Rotation ComboBox
****************************************************************************/
void JLP_FitsCubePanel::OnRotationComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(FitsCubeCmb_Rotation.combo)
    FitsCubeSet1.RotationMode = FitsCubeCmb_Rotation.combo->GetSelection();

jcam_changes1->ChangesDone = true;
}
/**************************************************************************
* Handle "Config" buttons:
**************************************************************************/
void JLP_FitsCubePanel::OnConfig( wxCommandEvent& event )
{
bool update_from_screen;

if(initialized != 1234) return;

switch(event.GetId()) {

  case ID_FPANEL_CONFIG_OK:
// Apply those settings to the camera, and store them to "original" settings:
    CamPanel_ValidateNewSettings();
    break;

  case ID_FPANEL_CONFIG_CANCEL:
// Cancel button: go back to initial settings
    CamPanel_CancelNonValidatedChanges();
    break;

// Save current config to file:
  case ID_FPANEL_CONFIG_SAVE:
// Apply those settings to the camera, and store them to "original" settings:
    CamPanel_ValidateNewSettings();
    SaveFitsCubeSettingsDialog();

    break;

// Load config from file:
  case ID_FPANEL_CONFIG_LOAD:
    LoadFitsCubeSettingsDialog();
// Display those new settings:
    update_from_screen = false;
    DisplayNewSettings(update_from_screen);
    jcam_changes1->ChangesDone = true;

    break;

// Load default config
  case ID_FPANEL_CONFIG_DEFAULT:
// Set default values:
    m_cam1->Cam1_SettingsToDefault(FitsCubeSet1.gblXPixels,
                                   FitsCubeSet1.gblYPixels);
    m_cam1->Get_FitsCubeSettings(&FitsCubeSet1);

// Display those new settings:
    update_from_screen = false;
    DisplayNewSettings(update_from_screen);
    jcam_changes1->ChangesDone = true;

    break;
}

return;
}
