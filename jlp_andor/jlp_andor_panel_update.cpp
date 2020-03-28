/****************************************************************************
* Name: jlp_andor_panel2.cpp
*
* JLP
* Version 20/10/2015
****************************************************************************/
#include "jlp_andor_panel.h"
#include "asp2_frame_id.h"
#include "asp2_frame.h"     // StopDisplayTimers()

//----------------------------------------------------------------------------
// Update Final Image labels according to ROI size stored in AndorSet1
//----------------------------------------------------------------------------
void  JLP_AndorPanel::UpdateFinalImageSizeLabels()
{
wxString buffer;
int nx0, ny0;

// Set new center of images in case of a very large ROI:
 nx0 = AndorSet1.nx1 * AndorSet1.xbin;
 ny0 = AndorSet1.ny1 * AndorSet1.ybin;
if((AndorSet1.gblXPixels < nx0)
                             || (AndorSet1.gblYPixels < ny0)){
  AndorSet1.xc0 = AndorSet1.gblXPixels/2;
  AndorSet1.yc0 = AndorSet1.gblYPixels/2;
  AndorSet1.nx1 = AndorSet1.gblXPixels;
  AndorSet1.ny1 = AndorSet1.gblYPixels;
  AndorSet1.xbin = 1;
  AndorSet1.ybin = 1;
  buffer.Printf(wxT("%d,%d"), AndorSet1.xc0, AndorSet1.yc0);
  AndorCtrl_ROICenter->SetValue(buffer);
  }

  buffer.Printf(wxT("Images: %d * %d pixels"),
                AndorSet1.nx1, AndorSet1.ny1);
  AndorStatic_ImageFormat->SetLabel(buffer);

  buffer.Printf(wxT("Detector: %d * %d pixels"),
                AndorSet1.gblXPixels, AndorSet1.gblYPixels);
  AndorStatic_DetectorFormat->SetLabel(buffer);

return;
}
/************************************************************
* Update TriggerMode according to value stored in AndorSet1
* Values: 0 (internal), 1 (external) or  10  (software)
*************************************************************/
void JLP_AndorPanel::UpdateTriggerMode()
{
if(!AndorCmb_Trigger.combo) return;

 switch (AndorSet1.TriggerMode) {
   default:
   case 0:
     AndorCmb_Trigger.combo->SetSelection(0);
     break;
   case 1:
     AndorCmb_Trigger.combo->SetSelection(1);
     break;
   case 10:
     AndorCmb_Trigger.combo->SetSelection(2);
     break;
 }
return;
}
/************************************************************
* Update Shutter combo box according to value stored in AndorSet1
*
* Values of AndorSet1.ShutterMode:
* -1 not available, 0 (automatic), 1 (always open) or  2 (always closed),
* Corresponding values of combobox selection: 0, 1, 2, 3
*
*************************************************************/
void JLP_AndorPanel::UpdateShutterMode()
{

if(!AndorCmb_Shutter.combo) return;

//
 if(AndorSet1.ShutterMode >= -1 && AndorSet1.ShutterMode <= 2) {
   AndorCmb_Shutter.combo->SetSelection(AndorSet1.ShutterMode + 1);
 } else {
   AndorCmb_Shutter.combo->SetSelection(2);
   AndorSet1.ShutterMode = 1;
 }
return;
}
/************************************************************
* Update MirrorMode according to value stored in AndorSet1
* Values: 0 (no symmetry), 1 (vertical mirror)
*         2 (horizontal mirror), 3 (hori. and vert.)
*************************************************************/
void JLP_AndorPanel::UpdateMirrorMode()
{
if(!AndorCmb_Mirror.combo) return;

 if(AndorSet1.MirrorMode >= 0 && AndorSet1.MirrorMode <= 3) {
   AndorCmb_Mirror.combo->SetSelection(AndorSet1.MirrorMode);
 } else {
   AndorCmb_Mirror.combo->SetSelection(0);
   AndorSet1.MirrorMode = 0;
 }
return;
}
/************************************************************
* Update ROISizeComboBox according to AndorSet1.nx0, AndorSet1.ny0
* Values:
* 0 = 64x64
* 1 = 128x128
* 2 = 256x256
* 3 = 512x512
* 4 = full size
*************************************************************/
void JLP_AndorPanel::UpdateROISizeComboBox()
{
int nx0;

if(!AndorCmb_ROISize.combo) return;

nx0 = AndorSet1.nx1 * AndorSet1.xbin;
 switch(nx0) {
   case 64:
     AndorCmb_ROISize.combo->SetSelection(0);
     break;
   case 128:
     AndorCmb_ROISize.combo->SetSelection(1);
     break;
   case 256:
     AndorCmb_ROISize.combo->SetSelection(2);
     break;
   case 512:
     AndorCmb_ROISize.combo->SetSelection(3);
     break;
   default:
     AndorCmb_ROISize.combo->SetSelection(4);
     break;
 }

return;
}
/************************************************************
* Update VShift Speed ComboBox according to AndorSet1
*************************************************************/
void JLP_AndorPanel::UpdateVShiftSpeed()
{
if(!AndorCmb_VShiftSpeed.combo) return;

 if((AndorSet1.VShiftSpeed >= 0) &&
    (AndorSet1.VShiftSpeed < AndorCmb_VShiftSpeed.nchoices)) {
   AndorCmb_VShiftSpeed.combo->SetSelection(AndorSet1.VShiftSpeed);
 } else {
   AndorCmb_VShiftSpeed.combo->SetSelection(0);
 }
return;
}
/************************************************************
* Update VShift Voltage Amplitude ComboBox according to AndorSet1
*************************************************************/
void JLP_AndorPanel::UpdateVShiftVoltage()
{
// Not available for the ANDOR LUCA camera:
// (combo box is NULL in that case)
if(!AndorCmb_VShiftVolt.combo) return;

 if((AndorSet1.VShiftVoltage >= 0)
    && (AndorSet1.VShiftVoltage < AndorCmb_VShiftVolt.nchoices)){
   AndorCmb_VShiftVolt.combo->SetSelection(AndorSet1.VShiftVoltage);
 } else {
   AndorCmb_VShiftVolt.combo->SetSelection(0);
   AndorSet1.VShiftVoltage = 0;
 }
return;
}
/************************************************************
* Update HShift Speed ComboBox according to value stored in AndorSet1
*************************************************************/
void JLP_AndorPanel::UpdateHShiftSpeed()
{
if(!AndorCmb_HShiftSpeed.combo) return;

 if((AndorSet1.HShiftSpeed >= 0)
     && (AndorSet1.HShiftSpeed < AndorCmb_HShiftSpeed.nchoices)){
   AndorCmb_HShiftSpeed.combo->SetSelection(AndorSet1.HShiftSpeed);
 } else {
   AndorCmb_HShiftSpeed.combo->SetSelection(0);
   AndorSet1.HShiftSpeed = 0;
 }
return;
}
/************************************************************
* Update HShift preampli Gain ComboBox according to value stored in AndorSet1
*************************************************************/
void JLP_AndorPanel::UpdateHShiftPreAmpGain()
{
if(!AndorCmb_PreAmpGain.combo) return;

 if((AndorSet1.HShiftPreAmpGain >= 0)
    && (AndorSet1.HShiftPreAmpGain < AndorCmb_PreAmpGain.nchoices)) {
   AndorCmb_PreAmpGain.combo->SetSelection(AndorSet1.HShiftPreAmpGain);
 } else {
   AndorCmb_PreAmpGain.combo->SetSelection(0);
   AndorSet1.HShiftPreAmpGain = 0;
 }
return;
}
/************************************************************
* Update AD ComboBox according to value stored in AndorSet1
*************************************************************/
void JLP_AndorPanel::UpdateAD_Channel()
{
if(!AndorCmb_ADChannel.combo) return;

 if((AndorSet1.AD_Channel >= 0)
    && (AndorSet1.AD_Channel < AndorCmb_ADChannel.nchoices)) {
   AndorCmb_ADChannel.combo->SetSelection(AndorSet1.AD_Channel);
 } else {
   AndorCmb_ADChannel.combo->SetSelection(0);
   AndorSet1.AD_Channel = 0;
 }
return;
}
/************************************************************
* Update RotationMode ComboBox according to value stored in AndorSet1
* Values: 0 (no rotation), 1 (+90 degrees), 2 (+180 degrees) or 3 (+270 degrees)
*************************************************************/
void JLP_AndorPanel::UpdateRotationMode()
{
if(!AndorCmb_Rotation.combo) return;

 if(AndorSet1.RotationMode >= 0 && AndorSet1.RotationMode <= 3) {
   AndorCmb_Rotation.combo->SetSelection(AndorSet1.RotationMode);
 } else {
   AndorCmb_Rotation.combo->SetSelection(0);
   AndorSet1.RotationMode = 0;
 }
return;
}
/************************************************************
* Update bin factor according to value stored in AndorSet1
* Values: 1, 2, 4 or 8
*************************************************************/
void JLP_AndorPanel::UpdateBinFactor()
{
if(!AndorCmb_BinFactor.combo)return;

switch (AndorSet1.xbin) {
   default:
   case 1:
     AndorCmb_BinFactor.combo->SetSelection(0);
     AndorSet1.xbin = AndorSet1.ybin = 1;
     break;
   case 2:
     AndorCmb_BinFactor.combo->SetSelection(1);
     break;
   case 4:
     AndorCmb_BinFactor.combo->SetSelection(2);
     break;
   case 8:
     AndorCmb_BinFactor.combo->SetSelection(3);
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
*
* Acquisition Mode:
* mode=1 Single Scan    (= long integration)
* mode=2 Accumulate     (= integration of short exposures)
* mode=3 Kinetics       (= sequence of images)
* mode=4 Fast Kinetics  (= fast transfer on a hidden area of the CCD, not implemented)
* mode=5 Run till abort (= short exposures until "stop" button is clicked)
*
**********************************************************************/
void JLP_AndorPanel::UpdateAcquisitionMode()
{

// Default possibilities:
 AndorCtrl_KinExpoTime->Enable(true);
 AndorCtrl_KinClockTime->Enable(true);
 AndorCtrl_KinNExp->Enable(true);
// In Case of Internal Trigger Mode enable Cycle Time edit:
 if(AndorSet1.TriggerMode == 10) AndorCtrl_KinCycleTime->Enable(true);
  else AndorCtrl_KinCycleTime->Enable(false);
 AndorCtrl_KinNCycle->Enable(true);
 AndorCtrl_LongExpoTime->Enable(false);

switch (AndorSet1.AcquisitionMode) {
// Long exposure
   case 1:
     AndorCmb_AcquiMode.combo->SetSelection(0);
     AndorCtrl_KinExpoTime->Enable(true);
     AndorCtrl_KinNExp->Enable(false);
     AndorCtrl_KinClockTime->Enable(false);
     AndorCtrl_KinNCycle->Enable(true);
     break;
// Accumulate = integration of short exposures
   case 2:
     AndorCmb_AcquiMode.combo->SetSelection(3);
     AndorCtrl_KinExpoTime->Enable(true);
     AndorCtrl_KinNExp->Enable(true);
     AndorCtrl_KinClockTime->Enable(true);
     AndorCtrl_KinNCycle->Enable(true);
     break;
// Video2 = Kinetic = sequence of N short exposures
   case 3:
     AndorCmb_AcquiMode.combo->SetSelection(2);
     AndorCtrl_KinExpoTime->Enable(true);
     AndorCtrl_KinNExp->Enable(true);
     AndorCtrl_KinClockTime->Enable(true);
     AndorCtrl_KinNCycle->Enable(true);
     break;
// Video1 = Run till abort (short exposures till abort)
   default:
   case 5:
     if(ExpertMode == false) {
       AndorCmb_AcquiMode.combo->SetSelection(1);
       AndorCtrl_KinExpoTime->Enable(true);
       AndorCtrl_KinNExp->Enable(false);
       AndorCtrl_KinCycleTime->Enable(false);
       AndorCtrl_KinClockTime->Enable(false);
       AndorCtrl_KinNCycle->Enable(false);
       } else {
       AndorCmb_AcquiMode.combo->SetSelection(4);
       }
     break;
   }

return;
}
/************************************************************
* Update cooler settings according to value stored in AndorSet1
*
*************************************************************/
void JLP_AndorPanel::UpdateCoolerSettings()
{
wxString buffer;

if(initialized != 1234) return;

//***************** Cooler:
// Check/uncheck cooler regul:
  AndorCheck_CoolerRegul->SetValue(AndorSet1.CoolerOn);

// Enable/disable cooler temperature edit box:
  AndorCtrl_CoolerSetPoint->Enable(AndorSet1.CoolerOn);

// Cooler temperature:
  buffer.Printf(wxT("%.2f"), AndorSet1.CoolerSetTemp);
  AndorCtrl_CoolerSetPoint->SetValue(buffer);

return;
}
/***************************************************************************
* Display new settings:
****************************************************************************/
void JLP_AndorPanel::DisplayNewSettings(bool update_from_screen)
{
char buffer[80];

  if(initialized != 1234) return;

/* Acquisition Mode Menu:
* item=0  long exposure
* item=1  video till abort
* item=2  video sequence
* item=3  accumulation of short exposures
*/
   UpdateAcquisitionMode();

// Trigger ComboBox:
   UpdateTriggerMode();

// Shutter ComboBox:
   UpdateShutterMode();

// Rotation ComboBox :
   UpdateRotationMode();

// Mirror ComboBox :
   UpdateMirrorMode();

// VShift Speed ComboBox :
   UpdateVShiftSpeed();

// VShift Voltage Amplitude ComboBox :
   UpdateVShiftVoltage();

// HShift Speed ComboBox :
   UpdateHShiftSpeed();

// HShift Gain ComboBox :
   UpdateHShiftPreAmpGain();

// AD Channel ComboBox :
   UpdateAD_Channel();

// ROISize ComboBox:
// Select good item in ROI ComboBox:
   UpdateROISizeComboBox();

// BinFactor ComboBox:
// Update output size according to bin factor in static text label:
   UpdateBinFactor();

// Update final image size in static labels:
   UpdateFinalImageSizeLabels();

// Image center (relative to the full CCD matrix):
   sprintf(buffer, "%d,%d", AndorSet1.xc0, AndorSet1.yc0);
   AndorCtrl_ROICenter->SetValue(buffer);

// Long exposure time in seconds:
   sprintf(buffer, "%d", AndorSet1.IntegTime_sec);
   AndorCtrl_LongExpoTime->SetValue(buffer);

// Kinetic exposure time in milliseconds:
   sprintf(buffer, "%d", AndorSet1.KineticExpoTime_msec);
   AndorCtrl_KinExpoTime->SetValue(buffer);

// Kinetic clock time in milliseconds:
   sprintf(buffer, "%d", AndorSet1.KineticClockTime_msec);
   AndorCtrl_KinClockTime->SetValue(buffer);

// Kinetic cycle time in milliseconds:
   sprintf(buffer, "%d", AndorSet1.KineticCycleTime_msec);
   AndorCtrl_KinCycleTime->SetValue(buffer);

// Kinetic number of exposures/cycle:
   sprintf(buffer, "%d", AndorSet1.KineticCycleNExposures);
   AndorCtrl_KinNExp->SetValue(buffer);

// Kinetic number of cycles:
   sprintf(buffer, "%d", AndorSet1.KineticNCycles);
   AndorCtrl_KinNCycle->SetValue(buffer);

//************** EMCCD gain

// EMCCD gain option (checkbox) :
   if(AndorSet1.EMGainMode != 2)
      AndorCheck_EMCCD->SetValue(true);
    else
      AndorCheck_EMCCD->SetValue(false);

// EMCCD gain value:
   sprintf(buffer, "%d", AndorSet1.EMGainValue);
   AndorCtrl_EMGain->SetValue(buffer);

// Enable/disable preampli gain edit box:
   if(AndorSet1.EMGainMode != 2)
      AndorCtrl_EMGain->Enable(true);
   else
      AndorCtrl_EMGain->Enable(false);

//***************** Cooler:
   UpdateCoolerSettings();

//********** Other check boxes:
// Check/uncheck FrameTransfer:
   AndorCheck_FrameTransfer->SetValue(AndorSet1.FrameTransferMode);

// Check/uncheck Baseline Clamp:
   AndorCheck_BaselineClamp->SetValue(AndorSet1.BaselineClamp);

// Check/uncheck Baseline Clamp:
   AndorCheck_CloseShutterWhenExit->SetValue(AndorSet1.CloseShutterWhenExit);

return;
}
/*****************************************************************
* CancelChanges
*
* Go back to the "original" settings
*****************************************************************/
void JLP_AndorPanel::CamPanel_CancelNonValidatedChanges()
{
bool update_from_screen = false;

// Copy Original_AndorSet1 to  current AndorSet1:
  Copy_AndorSettings(Original_AndorSet1, &AndorSet1);

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
void JLP_AndorPanel::CamPanel_ValidateNewSettings()
{
int status;
bool update_from_screen = true;
wxString error_msg;

  jcam_changes1->ChangesValidated = true;

// Update TextCtrl values from screen:
if(jcam_changes1->ChangesDone == true) {

  UpdateROICenter(update_from_screen);
  UpdateEMGain(update_from_screen);
  UpdateCoolerSetPoint(update_from_screen);

// validate seetings : order is importtant
  UpdateKineticExpoTime(update_from_screen);
  UpdateKineticClockTime(update_from_screen);
  UpdateKineticCycleTime(update_from_screen);
  UpdateKineticCycleNExposures(update_from_screen);
  UpdateKineticNCycles(update_from_screen);
  m_cam1->CloseShutterWhenExit(AndorSet1.CloseShutterWhenExit);

// Update total exposure time (conversion from msec to seconds):
  AndorSet1.IntegTime_sec = AndorSet1.KineticCycleTime_msec
                                    * AndorSet1.KineticNCycles / 1000.;
  UpdateLongExposureTime(update_from_screen);
// Display those new settings (that have been corrected if an error was found):
  DisplayNewSettings(update_from_screen);
}

// Load current settings to the camera:
// BEOF ONLY_WITH_ANDOR
  if(!andor_not_connected) {

// Load Andor settings first (OTHERWISE changes will NOT operate properly !!!)
  m_cam1->Cam1_Load_AndorSettings(AndorSet1);

// Apply new changes of cooling options:
  if(Original_AndorSet1.CoolerOn != AndorSet1.CoolerOn) {
    if(AndorSet1.CoolerOn) {
       m_cam1->JLP_Andor_StartCooler();
    } else {
       m_cam1->JLP_Andor_StopCooler();
       wxMessageBox(wxT("OK: stopping the cooler..."),
                    wxT("JLP_AndorPanel"), wxOK);
    }
  }

  if(Original_AndorSet1.CoolerSetTemp != AndorSet1.CoolerSetTemp) {
       status = m_cam1->Cam1_SetCoolerTemp(error_msg);
       if(status)
         wxMessageBox(error_msg, wxT("JLP_AndorPanel/Error"),
                      wxOK | wxICON_ERROR);
  }

// Apply new changes of EMCCD gain:
  if(Original_AndorSet1.EMGainMode != AndorSet1.EMGainMode
     || Original_AndorSet1.EMGainValue != AndorSet1.EMGainValue) {
    status = m_cam1->JLP_Andor_SetEMGain(error_msg);
    if(status) {
     wxMessageBox(error_msg, wxT("asp2_AndorSetup/Error"), wxOK | wxICON_ERROR);
     return;
     }
  }

// Apply new changes of Vertical or Horizontal shift, etc:
  if(Original_AndorSet1.HShiftSpeed != AndorSet1.HShiftSpeed
     || Original_AndorSet1.HShiftPreAmpGain != AndorSet1.HShiftPreAmpGain
     || Original_AndorSet1.VShiftSpeed != AndorSet1.VShiftSpeed
     || Original_AndorSet1.VShiftVoltage != AndorSet1.VShiftVoltage
     || Original_AndorSet1.AD_Channel != AndorSet1.AD_Channel
     || Original_AndorSet1.ShutterMode != AndorSet1.ShutterMode
     || Original_AndorSet1.TriggerMode != AndorSet1.TriggerMode) {
       status = m_cam1->JLP_Andor_ApplyBasicSettings();
       if(status)return;
     }

  if(Original_AndorSet1.KineticExpoTime_msec != AndorSet1.KineticExpoTime_msec
     || Original_AndorSet1.KineticClockTime_msec != AndorSet1.KineticClockTime_msec
     || Original_AndorSet1.KineticCycleNExposures != AndorSet1.KineticCycleNExposures
     || Original_AndorSet1.KineticNCycles != AndorSet1.KineticNCycles
     || Original_AndorSet1.KineticCycleTime_msec != AndorSet1.KineticCycleTime_msec
     || Original_AndorSet1.AcquisitionMode != AndorSet1.AcquisitionMode) {
       status = m_cam1->JLP_Andor_ApplySettingsForExposures();
       if(status)return;
       }
  } // EOF ONLY_WITH_ANDOR

// USED even if ANDOR is not connected to set nx, ny:
// Copy current settings (i.e. AndorSet1) to Original_AndorSet1:
Copy_AndorSettings(AndorSet1, &Original_AndorSet1);

// Init ChangesDone for further validation
// (and since those changes have been loaded to camera)
  jcam_changes1->ChangesDone = false;

return;
}
/***************************************************************************
*
* Save Andor setting configuration to new config. file
*
***************************************************************************/
int JLP_AndorPanel::SaveAndorSettingsDialog()
{
wxString save_filename;
char filename[128];
int status;

// Prompt for filename
wxFileDialog saveFileDialog(this, wxT("Save ANDOR settings to file"), wxT(""),
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
  SaveAndorSettingsToFile(filename, AndorSet1);

return(0);
}
/***************************************************************************
*
* Load Andor setting configuration from previously saved config. file
*
***************************************************************************/
int JLP_AndorPanel::LoadAndorSettingsDialog()
{
wxString filename1;
char filename[128];

// Prompt for filename
  filename1.Empty();
  filename1 = wxFileSelector(_T("Select file with ANDOR settings"), _T(""),
                            _T(""), _T(""),
                            _T("Config files (*.cfg;*.CFG)|*.cfg;*.CFG"));

// If "cancel", exit from here without loading files
  if (filename1.IsEmpty() ) return(-1);

  strcpy(filename, filename1.c_str());

// Load configuration file:
  LoadAndorSettingsFromFile(filename, &AndorSet1);

jcam_changes1->ChangesDone = true;
return(0);
}
