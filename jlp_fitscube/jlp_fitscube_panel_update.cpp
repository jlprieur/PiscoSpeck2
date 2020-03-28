/****************************************************************************
* Name: jlp_fitscube_panel2.cpp
*
* JLP
* Version 20/10/2015
****************************************************************************/
#include "jlp_fitscube_panel.h"
#include "asp2_frame_id.h"
#include "asp2_frame.h"     // StopDisplayTimers()

//----------------------------------------------------------------------------
// Update Final Image labels according to ROI size stored in FitsCubeSet1
//----------------------------------------------------------------------------
void  JLP_FitsCubePanel::UpdateFinalImageSizeLabels()
{
int nx0, ny0;
wxString buffer;

nx0 = FitsCubeSet1.nx1 * FitsCubeSet1.xbin;
ny0 = FitsCubeSet1.ny1 * FitsCubeSet1.ybin;

// Set new center of images in case of a very large ROI:
if((FitsCubeSet1.gblXPixels < nx0)
                             || (FitsCubeSet1.gblYPixels < ny0)){
  FitsCubeSet1.xc0 = FitsCubeSet1.gblXPixels/2;
  FitsCubeSet1.yc0 = FitsCubeSet1.gblYPixels/2;
  FitsCubeSet1.nx1 = FitsCubeSet1.gblXPixels;
  FitsCubeSet1.ny1 = FitsCubeSet1.gblYPixels;
  FitsCubeSet1.xbin = 1;
  FitsCubeSet1.ybin = 1;
  buffer.Printf(wxT("%d,%d"), FitsCubeSet1.xc0, FitsCubeSet1.yc0);
  FitsCubeCtrl_ROICenter->SetValue(buffer);
  }

  buffer.Printf(wxT("Images: %d * %d pixels"),
                FitsCubeSet1.nx1, FitsCubeSet1.ny1);
  FitsCubeStatic_ImageFormat->SetLabel(buffer);

  buffer.Printf(wxT("Detector: %d * %d pixels"),
                FitsCubeSet1.gblXPixels, FitsCubeSet1.gblYPixels);
  FitsCubeStatic_DetectorFormat->SetLabel(buffer);

return;
}
/************************************************************
* Update MirrorMode according to value stored in FitsCubeSet1
* Values: 0 (no symmetry), 1 (vertical mirror)
*         2 (horizontal mirror), 3 (hori. and vert.)
*************************************************************/
void JLP_FitsCubePanel::UpdateMirrorMode()
{
if(!FitsCubeCmb_Mirror.combo) return;

 if(FitsCubeSet1.MirrorMode >= 0 && FitsCubeSet1.MirrorMode <= 3) {
   FitsCubeCmb_Mirror.combo->SetSelection(FitsCubeSet1.MirrorMode);
 } else {
   FitsCubeCmb_Mirror.combo->SetSelection(0);
   FitsCubeSet1.MirrorMode = 0;
 }
return;
}
/************************************************************
* Update ROISizeComboBox according to FitsCubeSet1.nx0, FitsCubeSet1.ny0
* Values:
* 0 = 64x64
* 1 = 128x128
* 2 = 256x256
* 3 = 512x512
* 4 = 960x960
* 5 = full size
*************************************************************/
void JLP_FitsCubePanel::UpdateROISizeComboBox()
{
int nx0;
if(!FitsCubeCmb_ROISize.combo) return;

nx0 = FitsCubeSet1.nx1 * FitsCubeSet1.xbin;
 switch(nx0) {
   case 64:
     FitsCubeCmb_ROISize.combo->SetSelection(0);
     break;
   case 128:
     FitsCubeCmb_ROISize.combo->SetSelection(1);
     break;
   case 256:
     FitsCubeCmb_ROISize.combo->SetSelection(2);
     break;
   case 512:
     FitsCubeCmb_ROISize.combo->SetSelection(3);
     break;
   case 960:
     FitsCubeCmb_ROISize.combo->SetSelection(4);
     break;
   default:
     if(FitsCubeSet1.gblXPixels > 960)
       FitsCubeCmb_ROISize.combo->SetSelection(5);
     else if(FitsCubeSet1.gblXPixels > 512)
       FitsCubeCmb_ROISize.combo->SetSelection(4);
     else if(FitsCubeSet1.gblXPixels > 256)
       FitsCubeCmb_ROISize.combo->SetSelection(3);
     break;
 }

return;
}
/************************************************************
* Update RotationMode ComboBox according to value stored in FitsCubeSet1
* Values: 0 (no rotation), 1 (+90 degrees), 2 (+180 degrees) or 3 (+270 degrees)
*************************************************************/
void JLP_FitsCubePanel::UpdateRotationMode()
{
if(!FitsCubeCmb_Rotation.combo) return;

 if(FitsCubeSet1.RotationMode >= 0 && FitsCubeSet1.RotationMode <= 3) {
   FitsCubeCmb_Rotation.combo->SetSelection(FitsCubeSet1.RotationMode);
 } else {
   FitsCubeCmb_Rotation.combo->SetSelection(0);
   FitsCubeSet1.RotationMode = 0;
 }
return;
}
/************************************************************
* Update bin factor according to value stored in FitsCubeSet1
* Values: 1, 2, 4 or 8
*************************************************************/
void JLP_FitsCubePanel::UpdateBinFactor()
{
if(!FitsCubeCmb_BinFactor.combo)return;

switch (FitsCubeSet1.xbin) {
   default:
   case 1:
     FitsCubeCmb_BinFactor.combo->SetSelection(0);
     FitsCubeSet1.xbin = FitsCubeSet1.ybin = 1;
     break;
   case 2:
     FitsCubeCmb_BinFactor.combo->SetSelection(1);
     break;
   case 4:
     FitsCubeCmb_BinFactor.combo->SetSelection(2);
     break;
   case 8:
     FitsCubeCmb_BinFactor.combo->SetSelection(3);
     break;
   }

// Update output size in static text label:
UpdateFinalImageSizeLabels();

return;
}
/***************************************************************************
* Display new settings:
****************************************************************************/
void JLP_FitsCubePanel::DisplayNewSettings(bool update_from_screen)
{
char buffer[80];

  if(initialized != 1234) return;

// Rotation ComboBox :
   UpdateRotationMode();

// Mirror ComboBox :
   UpdateMirrorMode();

// ROISize ComboBox:
// Select good item in ROI ComboBox:
   UpdateROISizeComboBox();

// Update Edit box with ROI center:
   UpdateROICenter(update_from_screen);

// BinFactor ComboBox:
// Update output size according to bin factor in static text label:
   UpdateBinFactor();

// Update final image size in static labels:
   UpdateFinalImageSizeLabels();

// Image center (relative to the full CCD matrix):
   sprintf(buffer, "%d,%d", FitsCubeSet1.xc0, FitsCubeSet1.yc0);
   FitsCubeCtrl_ROICenter->SetValue(buffer);

return;
}
/*****************************************************************
* CancelChanges
*
* Go back to the "original" settings
*****************************************************************/
void JLP_FitsCubePanel::CamPanel_CancelNonValidatedChanges()
{
bool update_from_screen;
// Copy Original_FitsCubeSet1 to  current FitsCubeSet1:
    Copy_FitsCubeSettings(Original_FitsCubeSet1, &FitsCubeSet1);

// Display those new settings:
    update_from_screen = false;
    DisplayNewSettings(update_from_screen);
    jcam_changes1->ChangesDone = false;
    jcam_changes1->ChangesValidated = false;

return;
}
/*****************************************************************
* ValidateNewSettings()
*
* Apply the new settings to the camera
* and load them to the "original" settings
*****************************************************************/
void JLP_FitsCubePanel::CamPanel_ValidateNewSettings()
{
bool update_from_screen;

  jcam_changes1->ChangesValidated = true;

if(jcam_changes1->ChangesDone == true) {
// Copy current FitsCubeSet1 to Original_FitsCubeSet1:
  Copy_FitsCubeSettings(FitsCubeSet1, &Original_FitsCubeSet1);

// Update TextCtrl values from screen:
  update_from_screen = true;
  DisplayNewSettings(update_from_screen);
}
// Load current settings to the camera:
  if(m_cam1->NotConnected() == false) {

// Load Raptor settings first (OTHERWISE changes will NOT operate properly !!!)
  m_cam1->Load_FitsCubeSettings(FitsCubeSet1);
  }

// Init ChangesDone for further validation
// (and since those changes have been loaded to camera)
  jcam_changes1->ChangesDone = false;

return;
}
/***************************************************************************
*
* Save FitsCube setting configuration to new config. file
*
***************************************************************************/
int JLP_FitsCubePanel::SaveFitsCubeSettingsDialog()
{
wxString save_filename;
char filename[128];
int status;

// Prompt for filename
wxFileDialog saveFileDialog(this, wxT("Save FitsCube settings to file"), wxT(""),
                            wxT(""),
                            wxT("Config files (*.cfg;*.CFG)|*.cfg;*.CFG"),
                            wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

// Stops the timers:
 m_aspframe->StopDisplayTimers();
 status = saveFileDialog.ShowModal();
 m_aspframe->RestartDisplayTimers();

// If "cancel", exit from here without saving files
 if (status == wxID_CANCEL) return(-1);

  save_filename = saveFileDialog.GetFilename();
  strcpy(filename, save_filename.c_str());

// Save configuration file:
  SaveFitsCubeSettingsToFile(filename, FitsCubeSet1);

return(0);
}
/***************************************************************************
*
* Load FitsCube setting configuration from previously saved config. file
*
***************************************************************************/
int JLP_FitsCubePanel::LoadFitsCubeSettingsDialog()
{
wxString filename1;
char filename[128];

// Prompt for filename
  filename1.Empty();
  filename1 = wxFileSelector(_T("Select file with FitsCube settings"), _T(""),
                            _T(""), _T(""),
                            _T("Config files (*.cfg;*.CFG)|*.cfg;*.CFG"));

// If "cancel", exit from here without loading files
  if (filename1.IsEmpty() ) return(-1);

  strcpy(filename, filename1.c_str());

// Load configuration file:
  LoadFitsCubeSettingsFromFile(filename, &FitsCubeSet1);

jcam_changes1->ChangesDone = true;
return(0);
}
