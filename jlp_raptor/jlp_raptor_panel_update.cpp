/****************************************************************************
* Name: jlp_raptor_panel2.cpp
*
* JLP
* Version 20/10/2015
****************************************************************************/
#include "jlp_raptor_panel.h"
#include "asp2_frame_id.h"
#include "asp2_frame.h"     // StopDisplayTimers()

//----------------------------------------------------------------------------
// Update Final Image labels according to ROI size stored in RaptorSet1
//----------------------------------------------------------------------------
void  JLP_RaptorPanel::UpdateFinalImageSizeLabels()
{
wxString buffer;
int nx0, ny0;

// Set new center of images in case of a very large ROI:
nx0 = RaptorSet1.nx1 * RaptorSet1.xbin;
ny0 = RaptorSet1.ny1 * RaptorSet1.ybin;
if((RaptorSet1.gblXPixels < nx0)
                             || (RaptorSet1.gblYPixels < ny0)){
  RaptorSet1.xc0 = RaptorSet1.gblXPixels/2;
  RaptorSet1.yc0 = RaptorSet1.gblYPixels/2;
  RaptorSet1.nx1 = RaptorSet1.gblXPixels;
  RaptorSet1.ny1 = RaptorSet1.gblYPixels;
  RaptorSet1.xbin = 1;
  RaptorSet1.ybin = 1;
  buffer.Printf(wxT("%d,%d"), RaptorSet1.xc0, RaptorSet1.yc0);
  RaptorCtrl_ROICenter->SetValue(buffer);
  }

  buffer.Printf(wxT("Images: %d * %d pixels"),
                RaptorSet1.nx1, RaptorSet1.ny1);
  RaptorStatic_ImageFormat->SetLabel(buffer);

  buffer.Printf(wxT("Detector: %d * %d pixels"),
                RaptorSet1.gblXPixels, RaptorSet1.gblYPixels);
  RaptorStatic_DetectorFormat->SetLabel(buffer);

return;
}
/************************************************************
* Update ExtTriggerMode according to value stored in RaptorSet1
* ExtTrigger selection :
* 0 = Off => RaptorSet1.ExtTriggerMode = 0;
* 1 = On(Ext +ve Edge) => RaptorSet1.ExtTriggerMode = 1;
* 2 = On(Ext -ve Edge) => RaptorSet1.ExtTriggerMode = 2;
*************************************************************/
void JLP_RaptorPanel::UpdateExtTriggerMode()
{
if(!RaptorCmb_ExtTrigger.combo) return;

 RaptorCmb_ExtTrigger.combo->SetSelection(RaptorSet1.ExtTrigger);

// Enable/Disable TriggerDelay edit box /Frame rate combo box:
// FrameRate activated only when internal trigger is activated
 if(RaptorSet1.ExtTrigger == 0) {
  RaptorCtrl_TriggerDelay->Enable(false);
  RaptorCmb_FrameRate.combo->Enable(true);
 } else {
  RaptorCtrl_TriggerDelay->Enable(true);
  RaptorCmb_FrameRate.combo->Enable(false);
 }

return;
}
/************************************************************
* Update Shutter combo box according to value stored in RaptorSet1
*
* Values of RaptorSet1.ShutterMode:
* -1 not available, 0 (automatic), 1 (always open) or  2 (always closed),
* Corresponding values of combobox selection: 0, 1, 2, 3
*
*************************************************************/
void JLP_RaptorPanel::UpdateShutterMode()
{

if(!RaptorCmb_Shutter.combo) return;

//
 if(RaptorSet1.ShutterMode >= -1 && RaptorSet1.ShutterMode <= 2) {
   RaptorCmb_Shutter.combo->SetSelection(RaptorSet1.ShutterMode + 1);
 } else {
   RaptorCmb_Shutter.combo->SetSelection(2);
   RaptorSet1.ShutterMode = 1;
 }
return;
}
/************************************************************
* Update MirrorMode according to value stored in RaptorSet1
* Values: 0 (no symmetry), 1 (vertical mirror)
*         2 (horizontal mirror), 3 (hori. and vert.)
*************************************************************/
void JLP_RaptorPanel::UpdateMirrorMode()
{
if(!RaptorCmb_Mirror.combo) return;

 if(RaptorSet1.MirrorMode >= 0 && RaptorSet1.MirrorMode <= 3) {
   RaptorCmb_Mirror.combo->SetSelection(RaptorSet1.MirrorMode);
 } else {
   RaptorCmb_Mirror.combo->SetSelection(0);
   RaptorSet1.MirrorMode = 0;
 }
return;
}
/************************************************************
* Update ROISizeComboBox according to RaptorSet1.nx0, RaptorSet1.ny0
* Values:
* 0 = 64x64
* 1 = 128x128
* 2 = 256x256
* 3 = 512x512
* 4 = full size
*************************************************************/
void JLP_RaptorPanel::UpdateROISizeComboBox()
{
int nx0;
if(!RaptorCmb_ROISize.combo) return;

nx0 = RaptorSet1.nx1 * RaptorSet1.xbin;
 switch(nx0) {
   case 64:
     RaptorCmb_ROISize.combo->SetSelection(0);
     break;
   case 128:
     RaptorCmb_ROISize.combo->SetSelection(1);
     break;
   case 256:
     RaptorCmb_ROISize.combo->SetSelection(2);
     break;
   case 512:
     RaptorCmb_ROISize.combo->SetSelection(3);
     break;
   default:
     RaptorCmb_ROISize.combo->SetSelection(4);
     break;
 }

return;
}
/************************************************************
* Update Frame Rate ComboBox according to value stored in RaptorSet1
*************************************************************/
void JLP_RaptorPanel::UpdateFrameRate()
{
if(!RaptorCmb_FrameRate.combo) return;

   RaptorCmb_FrameRate.combo->SetSelection(RaptorSet1.FrameRate);

return;
}
/************************************************************
* Update NUC State ComboBox according to value stored in RaptorSet1
*************************************************************/
void JLP_RaptorPanel::UpdateNucState()
{
if(!RaptorCmb_NUC_State.combo) return;

   RaptorCmb_NUC_State.combo->SetSelection(RaptorSet1.NUC_State);

return;
}
/************************************************************
* Update RotationMode ComboBox according to value stored in RaptorSet1
* Values: 0 (no rotation), 1 (+90 degrees), 2 (+180 degrees) or 3 (+270 degrees)
*************************************************************/
void JLP_RaptorPanel::UpdateRotationMode()
{
if(!RaptorCmb_Rotation.combo) return;

 if(RaptorSet1.RotationMode >= 0 && RaptorSet1.RotationMode <= 3) {
   RaptorCmb_Rotation.combo->SetSelection(RaptorSet1.RotationMode);
 } else {
   RaptorCmb_Rotation.combo->SetSelection(0);
   RaptorSet1.RotationMode = 0;
 }
return;
}
/************************************************************
* Update check box and enable/Disable ExpoTime edit box
*************************************************************/
void JLP_RaptorPanel::UpdateAutoExposure()
{
if(RaptorSet1.AutoExposure == 1) {
 RaptorCheck_AutoExposure->SetValue(true);
 RaptorCtrl_ExpoTime->Enable(false);
 } else  {
 RaptorCheck_AutoExposure->SetValue(false);
 RaptorCtrl_ExpoTime->Enable(true);
}
return;
}
/************************************************************
*
*************************************************************/
void JLP_RaptorPanel::UpdateImageSharpen()
{
if(RaptorSet1.ImageSharpen == 1) {
 RaptorCheck_ImageSharpen->SetValue(true);
 } else  {
 RaptorCheck_ImageSharpen->SetValue(false);
}
return;
}
/************************************************************
* Update bin factor according to value stored in RaptorSet1
* Values: 1, 2, 4 or 8
*************************************************************/
void JLP_RaptorPanel::UpdateBinFactor()
{
if(!RaptorCmb_BinFactor.combo)return;

switch (RaptorSet1.xbin) {
   default:
   case 1:
     RaptorCmb_BinFactor.combo->SetSelection(0);
     RaptorSet1.xbin = RaptorSet1.ybin = 1;
     break;
   case 2:
     RaptorCmb_BinFactor.combo->SetSelection(1);
     break;
   case 4:
     RaptorCmb_BinFactor.combo->SetSelection(2);
     break;
   case 8:
     RaptorCmb_BinFactor.combo->SetSelection(3);
     break;
   }

// Update output size in static text label:
UpdateFinalImageSizeLabels();

return;
}
/**********************************************************************
* Select options according to Acquisition mode:
* item=0  long exposure
* item=1  video till abort
* item=2  video sequence
* item=3  accumulation of short exposures
* item=4  video till abort (video3 Expert mode)
*
* Acquisition Mode:
* mode=1 Single Scan    (= long integration)
* mode=2 Accumulate     (= integration of short exposures)
* mode=3 Kinetics       (= sequence of images)
* mode=4 Fast Kinetics  (= fast transfer on a hidden area of the CCD, not implemented)
* mode=5 Run till abort (= short exposures until "stop" button is clicked)
*
**********************************************************************/
void JLP_RaptorPanel::UpdateAcquisitionMode()
{

if((RaptorSet1.AcquisitionMode < 1)
   || (RaptorSet1.AcquisitionMode > 5)){
   RaptorSet1.AcquisitionMode = 5;
   }
RaptorCmb_AcquiMode.combo->SetSelection(RaptorSet1.AcquisitionMode - 1);

// Enable/Disable exposition/integration time edit boxes:
 if((RaptorSet1.AcquisitionMode == 1)
     || (RaptorSet1.AcquisitionMode == 3)
     || (RaptorSet1.AcquisitionMode == 4)) {
   RaptorCtrl_IntegTime->Enable(true);
 } else {
   RaptorCtrl_IntegTime->Enable(false);
 }

 if(RaptorSet1.AcquisitionMode == 1)
   RaptorCtrl_ExpoTime->Enable(false);
 else
   RaptorCtrl_ExpoTime->Enable(true);

return;
}
/************************************************************
* Update cooler settings according to value stored in RaptorSet1
*
*************************************************************/
void JLP_RaptorPanel::UpdateCoolerSettings()
{
wxString buffer;
bool checked0;

if(initialized != 1234) return;

//***************** Cooler:
// Check/uncheck cooler regul:
  checked0 = (RaptorSet1.CoolerOn == 1);
  RaptorCheck_CoolerRegul->SetValue(checked0);

// Check/uncheck cooler fan:
  checked0 = (RaptorSet1.FanIsOn == 1);
  RaptorCheck_CoolerFan->SetValue(checked0);

// Cooler temperature:
  buffer.Printf(wxT("%.2f"), RaptorSet1.CoolerSetTemp);
  RaptorCtrl_CoolerSetTemp->SetValue(buffer);

// Enable/disable cooler temperature edit box:
  checked0 = (RaptorSet1.CoolerOn == 1);
  RaptorCtrl_CoolerSetTemp->Enable(checked0);

return;
}
/***************************************************************************
* Display new settings:
****************************************************************************/
void JLP_RaptorPanel::DisplayNewSettings(bool update_from_screen)
{

  if(initialized != 1234) return;

/* Acquisition Mode Menu:
* item=0  long exposure
* item=1  video till abort
* item=2  video sequence
* item=3  accumulation of short exposures
*/
   UpdateAcquisitionMode();

// Trigger ComboBox:
   UpdateExtTriggerMode();

// Shutter ComboBox:
   UpdateShutterMode();

// Rotation ComboBox :
   UpdateRotationMode();

// Mirror ComboBox :
   UpdateMirrorMode();

// Frame Rate :
   UpdateFrameRate();

// NUC State:
   UpdateNucState();

// ROISize ComboBox:
// Select good item in ROI ComboBox:
   UpdateROISizeComboBox();

// BinFactor ComboBox:
// Update output size according to bin factor in static text label:
   UpdateBinFactor();

// Set AutoExposure CheckBox and enable/disable ExpoTime EditBox
   UpdateAutoExposure();

// Set ImageSharpen CheckBox
   UpdateImageSharpen();

// Update final image size in static labels:
   UpdateFinalImageSizeLabels();

// Image center (relative to the full CCD matrix):
   UpdateROICenter(update_from_screen);

// Frame exposure time in milliseconds:
   UpdateExpoTime(update_from_screen);

// Long exposure time in seconds:
   UpdateIntegTime(update_from_screen);
   RaptorCtrl_IntegTime->Enable(false);

//***************** Cooler:
   UpdateCoolerSettings();

return;
}
/*****************************************************************
* CancelChanges
*
* Go back to the "original" settings
*****************************************************************/
void JLP_RaptorPanel::CamPanel_CancelNonValidatedChanges()
{
bool update_from_screen = false;

// Copy Original_RaptorSet1 to  current RaptorSet1:
    Copy_RaptorSettings(Original_RaptorSet1, &RaptorSet1);

// Display those new settings:
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
void JLP_RaptorPanel::CamPanel_ValidateNewSettings()
{
int status;
bool update_from_screen = true;
wxString error_msg;

  jcam_changes1->ChangesValidated = true;

// Update TextCtrl values from screen:
if(jcam_changes1->ChangesDone == true) {
  UpdateROICenter(update_from_screen);
  UpdateDigitalGain(update_from_screen);
  UpdateTriggerDelay(update_from_screen);
  UpdateCoolerSetPoint(update_from_screen);
  UpdateExpoTime(update_from_screen);
  UpdateIntegTime(update_from_screen);

  m_cam1->CloseShutterWhenExit(RaptorSet1.CloseShutterWhenExit);

// Display those new settings (that have been corrected if an error was found):
  DisplayNewSettings(update_from_screen);
}

// Load current settings to the camera:
// BEOF ONLY_WITH_RAPTOR
if(!m_cam1->NotConnected()) {

// Load Raptor settings first (OTHERWISE changes will NOT operate properly !!!)
  m_cam1->Load_RaptorSettings(RaptorSet1);

// Apply new changes of cooling options:
  if(Original_RaptorSet1.CoolerOn != RaptorSet1.CoolerOn) {
    if(RaptorSet1.CoolerOn) {
       m_cam1->JLP_Raptor_StartCooler();
    } else {
       m_cam1->JLP_Raptor_StopCooler();
       wxMessageBox(wxT("OK: stopping the cooler..."),
                    wxT("JLP_RaptorPanel"), wxOK);
    }
  }
// Apply new changes of cooler set point:
  if(Original_RaptorSet1.CoolerSetTemp != RaptorSet1.CoolerSetTemp) {
     status = m_cam1->Cam1_SetCoolerTemp(error_msg);
     if(status) wxMessageBox(error_msg, wxT("JLP_RaptorPanel/Error"),
                             wxOK | wxICON_ERROR);
  }
// Apply new changes of fan status:
  if(Original_RaptorSet1.FanIsOn != RaptorSet1.FanIsOn) {
    status = m_cam1->JLP_Raptor_Set_FPGA_Options();
    if(status) {
     wxMessageBox("Error", wxT("JLP_Raptor_Set_FPGA_Options"),
                  wxOK | wxICON_ERROR);
     }
  }
// Apply new changes of NUC state:
  if(Original_RaptorSet1.NUC_State != RaptorSet1.NUC_State) {
     status = m_cam1->JLP_Raptor_Set_NUC_State();
     if(status) wxMessageBox("Error", wxT("JLP_Raptor_Set_NUC_State"),
                             wxOK | wxICON_ERROR);
  }
// Apply new changes of TriggerDelay
  if(Original_RaptorSet1.TriggerDelay != RaptorSet1.TriggerDelay) {
     status = m_cam1->JLP_Raptor_SetTriggerDelay();
     if(status) wxMessageBox("Error", wxT("JLP_Raptor_SetTriggerDelay"),
                             wxOK | wxICON_ERROR);
  }
// Apply new changes of AutoExposure mode:
// Apply new changes of exposure time:
  if((Original_RaptorSet1.AutoExposure != RaptorSet1.AutoExposure)
     || (Original_RaptorSet1.ExpoTime_msec != RaptorSet1.ExpoTime_msec)) {
     status = m_cam1->JLP_Raptor_SetExpoTimeOrAutoExpo();
     if(status) wxMessageBox("Error", wxT("JLP_Raptor_SetExpoTimeOrAutoExpo"),
                             wxOK | wxICON_ERROR);
  }
// Apply new changes of HighGain mode:
// Apply new changes of gain value:
  if((Original_RaptorSet1.HighGain != RaptorSet1.HighGain)
     || (Original_RaptorSet1.DigitalGain != RaptorSet1.DigitalGain)) {
     status = m_cam1->JLP_Raptor_SetHighOrDigitalGain(error_msg);
     if(status) wxMessageBox(error_msg, wxT("JLP_Raptor_SetHighOrDigitalGain"),
                             wxOK | wxICON_ERROR);
  }
// Apply new changes of frame rate:
  if(Original_RaptorSet1.FrameRate != RaptorSet1.FrameRate) {
     status = m_cam1->JLP_Raptor_SetFrameRate();
     if(status) wxMessageBox("Error", wxT("JLP_Raptor_SetFrameRate"),
                             wxOK | wxICON_ERROR);
  }
// Apply new changes of image sharpen:
  if(Original_RaptorSet1.ImageSharpen != RaptorSet1.ImageSharpen) {
     status = m_cam1->JLP_Raptor_SetImageSharpen();
     if(status) wxMessageBox("Error", wxT("JLP_Raptor_SetImageSharpen"),
                             wxOK | wxICON_ERROR);
  }

}

// USED even if RAPTOR is not connected to set nx, ny:
// Copy current settings (i.e. RaptorSet1) to Original_RaptorSet1:
Copy_RaptorSettings(RaptorSet1, &Original_RaptorSet1);

// Init ChangesDone for further validation
// (and since those changes have been loaded to camera)
jcam_changes1->ChangesDone = false;

return;
}
/***************************************************************************
*
* Save Raptor setting configuration to new config. file
*
***************************************************************************/
int JLP_RaptorPanel::SaveRaptorSettingsDialog()
{
wxString save_filename;
char filename[128];
int status;

// Prompt for filename
wxFileDialog saveFileDialog(this, wxT("Save RAPTOR settings to file"), wxT(""),
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
  SaveRaptorSettingsToFile(filename, RaptorSet1);

return(0);
}
/***************************************************************************
*
* Load Raptor setting configuration from previously saved config. file
*
***************************************************************************/
int JLP_RaptorPanel::LoadRaptorSettingsDialog()
{
wxString filename1;
bool update_from_screen = false;
char filename[128];

// Prompt for filename
  filename1.Empty();
  filename1 = wxFileSelector(_T("Select file with RAPTOR settings"), _T(""),
                            _T(""), _T(""),
                            _T("Config files (*.cfg;*.CFG)|*.cfg;*.CFG"));

// If "cancel", exit from here without loading files
  if (filename1.IsEmpty() ) return(-1);

  strcpy(filename, filename1.c_str());

// Load configuration file:
  LoadRaptorSettingsFromFile(filename, &RaptorSet1);
  DisplayNewSettings(update_from_screen);

jcam_changes1->ChangesDone = true;
return(0);
}
