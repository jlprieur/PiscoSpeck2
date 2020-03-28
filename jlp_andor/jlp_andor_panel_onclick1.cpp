/****************************************************************************
* Name: jlp_andor_panel_onclick1.cpp
*
* JLP
* Version 26/08/2015
****************************************************************************/
#include "jlp_andor_panel.h"

#define ABS(a) ((a) >= 0 ? (a) : (-(a)))

/********************************************************************
* ROICenter LabeledEdit box
* WARNING: should wait before doing something anything to avoid blocking !
*
********************************************************************/
void JLP_AndorPanel::OnROICenterOnExit(wxCommandEvent& event)
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
void JLP_AndorPanel::UpdateROICenter(const bool update_from_screen)
{
wxString buffer0;
char buffer[64];
int ix, iy, ixc0, iyc0, ixc, iyc;
int delta_xmax, delta_ymax;

if(update_from_screen) {
 buffer0 = AndorCtrl_ROICenter->GetValue();
 strcpy(buffer, buffer0.mb_str());
 if(sscanf(buffer, "%d,%d", &ix, &iy) == 2) {
   AndorSet1.xc0 = ix;
   AndorSet1.yc0 = iy;
 }
}

// Image center:
 ixc0 = (AndorSet1.nx1 * AndorSet1.xbin) /2;
 iyc0 = (AndorSet1.ny1 * AndorSet1.ybin) /2;
 ixc = AndorSet1.gblXPixels/2;
 iyc = AndorSet1.gblYPixels/2;
 delta_xmax = MAXI(0, ixc - ixc0);
 delta_ymax = MAXI(0, iyc - iyc0);

// If error, erase the new value and set to the default value
 if((AndorSet1.xc0 < ixc - delta_xmax) || (AndorSet1.xc0 > ixc + delta_xmax)
     || (AndorSet1.yc0 < iyc - delta_ymax) || (AndorSet1.yc0 > iyc + delta_ymax)) {
   buffer0.Printf(wxT("Value=%d,%d: valid range is [%d %d] for xcent and [%d %d] for ycent"),
                  AndorSet1.xc0, AndorSet1.yc0, ixc - delta_xmax, ixc + delta_xmax,
                  iyc - delta_ymax, iyc + delta_ymax);
   wxMessageBox(buffer0, wxT("ROICenterOnExit/Error"), wxOK | wxICON_ERROR);
   AndorSet1.xc0 = AndorSet1.gblXPixels/2;
   AndorSet1.yc0 = AndorSet1.gblYPixels/2;
  }

// Display new value :
  buffer0.Printf(wxT("%d,%d"), AndorSet1.xc0, AndorSet1.yc0);
  AndorCtrl_ROICenter->SetValue(buffer0);

return;
}
/*****************************************************************
* Acquisition Mode Menu:
* item=0  long exposure
* item=1  video till abort (video3 default)
* item=2  video : sequence of short exposures
* item=3  accumulation of short exposures
* item=4  video till abort (video3 Expert mode)
*
* Acquisition Mode:
* mode=1 Single Scan    (= long integration)
* mode=2 Accumulate     (= integration of short exposures)
* mode=3 Kinetics       (= sequence of images)
* mode=4 Fast Kinetics  (= fast transfer on a hidden area of the CCD, not implemented)
* mode=5 Run till abort (= short exposures until "stop" button is clicked)
*****************************************************************/
void JLP_AndorPanel::OnAcquiModeComboBoxChange(wxCommandEvent& WXUNUSED(event))
{
int index;

if(initialized != 1234) return;

index = AndorCmb_AcquiMode.combo->GetSelection();

switch(index) {
// 0 = Long exposure
   case 0:
     AndorSet1.AcquisitionMode = 1;
     break;
// 1 = Video1, i.e., run till abort (short exposures till abort)
   default:
   case 1:
     AndorSet1.AcquisitionMode = 5;
     ExpertMode = false;
     break;
// 2 = Video2, i.e., kinetic, sequence of N short exposures
   case 2:
     AndorSet1.AcquisitionMode = 3;
     break;
// 3 = Accumulate, i.e., integration of short exposures
   case 3:
     AndorSet1.AcquisitionMode = 2;
     break;
// 1 = Video1, i.e., run till abort (short exposures till abort) Expert Mode
   case 4:
     AndorSet1.AcquisitionMode = 5;
     ExpertMode = true;
     break;
   }

// Select options according to Acquisition mode:
 UpdateAcquisitionMode();

 jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Trigger selection
* 0 = Internal trigger => AndorSet1.TriggerMode = 0;
* 1 = External trigger => AndorSet1.TriggerMode = 1;
* 2 = Software trigger => AndorSet1.TriggerMode = 10 (if possible);
*
* Acquisition Mode:
* mode=1 Single Scan    (= long integration)
* mode=2 Accumulate     (= integration of short exposures)
* mode=3 Kinetics       (= sequence of images)
* mode=4 Fast Kinetics  (= fast transfer on a hidden area of the CCD)
* mode=5 Run till abort (= short exposures until "stop" button is clicked)
*****************************************************************/
void JLP_AndorPanel::OnTriggerComboBoxChange(wxCommandEvent& event)
{
int index;

if(initialized != 1234) return;

index = AndorCmb_Trigger.combo->GetSelection();
  switch(index){
// Internal trigger:
    default:
    case 0:
      AndorSet1.TriggerMode = 0;
// In Case of Internal Trigger Mode enable Cycle Time edit:
      if(AndorSet1.AcquisitionMode == 5) AndorCtrl_KinCycleTime->Enable(true);
      break;
// External trigger:
    case 1:
      AndorSet1.TriggerMode = 1;
      break;
// Software trigger:
    case 2:
      if(IsTriggerModeAvailable(10) != DRV_SUCCESS) {
        wxMessageBox(wxT("Error: hardware does not allow \"Software Trigger\"!"),
                   "TriggerComboBoxChange", wxOK | wxICON_ERROR);
        AndorSet1.TriggerMode = 0;
        AndorCmb_Trigger.combo->SetSelection(0);
        if(AndorSet1.AcquisitionMode == 5) AndorCtrl_KinCycleTime->Enable(true);
      } else {
        AndorSet1.TriggerMode = 10;
// In Case of Internal Trigger Mode disable Cycle Time edit:
        if(AndorSet1.AcquisitionMode == 5) AndorCtrl_KinCycleTime->Enable(false);
      }
      break;
  }

jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Edit box for cooler temperature
*****************************************************************/
void JLP_AndorPanel::OnCoolerSetPointEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Edit box for cooler temperature
*****************************************************************/
void JLP_AndorPanel::UpdateCoolerSetPoint(const bool update_from_screen)
{
wxString buffer0;
double dval, low_value = -100., high_value = 30.;

if(update_from_screen == true) {
 buffer0 = AndorCtrl_CoolerSetPoint->GetValue();
 if(buffer0.ToDouble(&dval)) AndorSet1.CoolerSetTemp = dval;
 }
// If error, erase the new value and display the default value
 if((dval < low_value) || (dval > high_value)) {
    buffer0.Printf(wxT("Valid range is %.2f/%.2f deg C!"),
                   low_value, high_value);
    wxMessageBox(buffer0, wxT("UpdateCoolerSetPoint/Error"),
                 wxOK | wxICON_ERROR);
    AndorSet1.CoolerSetTemp = 10.;
    }

// Display current value:
 buffer0.Printf(wxT("%.2f"), AndorSet1.CoolerSetTemp);
 AndorCtrl_CoolerSetPoint->SetValue(buffer0);

return;
}
/*****************************************************************
* Cooler: On/Off
*****************************************************************/
void JLP_AndorPanel::OnCoolerCheckBoxClick(wxCommandEvent& event)
{

if(initialized != 1234) return;

  AndorSet1.CoolerOn = AndorCheck_CoolerRegul->IsChecked();

// Enable/disable cooler temperature edit box:
  AndorCtrl_CoolerSetPoint->Enable(AndorSet1.CoolerOn);

  jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* EMCCD/Linear CCD
*****************************************************************/
void JLP_AndorPanel::OnEMGainCheckBoxClick(wxCommandEvent& event)
{

if(initialized != 1234) return;

  if(AndorCheck_EMCCD->IsChecked()) {
// EM CCD Mode:
   AndorSet1.EMGainMode = AndorSet1.EMGainBasicMode;
// Enable gain edit box:
   AndorCtrl_EMGain->Enable(true);
  } else {
// Linear CCD Mode:
   AndorSet1.EMGainMode = 2;
// Disable gain edit box:
   AndorCtrl_EMGain->Enable(false);
  }

jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Setting EM gain value
*****************************************************************/
void JLP_AndorPanel::OnEMGainEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Setting EM gain value
*****************************************************************/
void JLP_AndorPanel::UpdateEMGain(const bool update_from_screen)
{
wxString buffer0;
long ival;

 if(update_from_screen == true) {
  buffer0 = AndorCtrl_EMGain->GetValue();
  if(buffer0.ToLong(&ival)) AndorSet1.EMGainValue = ival;
  }

// If error, erase the new value and set to the default value
 if((AndorSet1.EMGainValue < AndorSet1.EMGainLowValue)
     || (AndorSet1.EMGainValue > AndorSet1.EMGainHighValue)){
     buffer0.Printf(wxT("Error/Valid range for EMGain is [%d,%d]"),
             AndorSet1.EMGainLowValue, AndorSet1.EMGainHighValue);
     wxMessageBox(buffer0, wxT("UpdateEMGain/Error"),
                  wxOK | wxICON_ERROR);
     AndorSet1.EMGainValue = AndorSet1.EMGainLowValue;
   }

 buffer0.Printf(wxT("%d"), AndorSet1.EMGainValue);
 AndorCtrl_EMGain->SetValue(buffer0);

return;
}
/****************************************************************************
*
****************************************************************************/
void JLP_AndorPanel::OnBinFactorComboBoxChange(wxCommandEvent& event)
{
int index, nx0, ny0;

if(initialized != 1234) return;

nx0 = AndorSet1.nx1 * AndorSet1.xbin;
ny0 = AndorSet1.ny1 * AndorSet1.ybin;

// Handle Binning ComboBox for bin factor
// Values: 1, 2, 4 or 8

index = AndorCmb_BinFactor.combo->GetSelection();
switch (index) {
   default:
   case 0:
     AndorSet1.xbin = AndorSet1.ybin = 1;
     break;
   case 1:
     AndorSet1.xbin = AndorSet1.ybin = 2;
     break;
   case 2:
     AndorSet1.xbin = AndorSet1.ybin = 4;
     break;
   case 3:
     AndorSet1.xbin = AndorSet1.ybin = 8;
     break;
   }

// ROI size should remain the same afetr bin change:
AndorSet1.nx1 = nx0 / AndorSet1.xbin;
AndorSet1.ny1 = ny0 / AndorSet1.ybin;

// Update final image size in static text label:
UpdateFinalImageSizeLabels();

jcam_changes1->ChangesDone = true;

return;
}
/***************************************************************************
* Selection of new item in Region Of Interest Size ComboBox
*
* New ROI size AndorSet1.nx0, AndorSet1.ny0
* 0 = 64x64
* 1 = 128x128
* 2 = 256x256
* 3 = 512x512
* 4 = 960x960
* 5 = full size
*
***************************************************************************/
void JLP_AndorPanel::OnROISizeComboBoxChange(wxCommandEvent& event)
{
int index, nx0, ny0;

if(initialized != 1234) return;

index = AndorCmb_ROISize.combo->GetSelection();
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
     if(AndorSet1.gblXPixels > 256) {
       nx0 = 256;
       ny0 = 256;
     } else {
       nx0 = AndorSet1.gblXPixels;
       ny0 = AndorSet1.gblYPixels;
     }
     break;
   case 3:
     if(AndorSet1.gblXPixels > 512) {
       nx0 = 512;
       ny0 = 512;
     } else {
       nx0 = AndorSet1.gblXPixels;
       ny0 = AndorSet1.gblYPixels;
     }
     break;
   case 4:
     if(AndorSet1.gblXPixels > 960) {
       nx0 = 560;
       ny0 = 960;
     } else {
       nx0 = AndorSet1.gblXPixels;
       ny0 = AndorSet1.gblYPixels;
     }
     break;
   case 5:
     nx0 = AndorSet1.gblXPixels;
     ny0 = AndorSet1.gblYPixels;
     break;
   }
   AndorSet1.nx1 = nx0 / AndorSet1.xbin;
   AndorSet1.ny1 = ny0 / AndorSet1.ybin;
   UpdateFinalImageSizeLabels();

jcam_changes1->ChangesDone = true;

return;
}
/****************************************************************************
* Mirror ComboBox
****************************************************************************/
void JLP_AndorPanel::OnMirrorComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(AndorCmb_Mirror.combo)
    AndorSet1.MirrorMode = AndorCmb_Mirror.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* Rotation ComboBox
****************************************************************************/
void JLP_AndorPanel::OnRotationComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(AndorCmb_Rotation.combo)
    AndorSet1.RotationMode = AndorCmb_Rotation.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* VShift Speed ComboBox
****************************************************************************/
void JLP_AndorPanel::OnVShiftSpeedComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(AndorCmb_VShiftSpeed.combo)
      AndorSet1.VShiftSpeed = AndorCmb_VShiftSpeed.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* VShift Voltage ComboBox
* Not available for the ANDOR LUCA camera
****************************************************************************/
void JLP_AndorPanel::OnVShiftVoltageComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(AndorCmb_VShiftVolt.combo)
       AndorSet1.VShiftVoltage = AndorCmb_VShiftVolt.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
//---------------------------------------------------------------------------
// HShift Speed ComboBox
//---------------------------------------------------------------------------
void JLP_AndorPanel::OnHShiftSpeedComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(AndorCmb_HShiftSpeed.combo)
       AndorSet1.HShiftSpeed = AndorCmb_HShiftSpeed.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* HShift Gain ComboBox
****************************************************************************/
void JLP_AndorPanel::OnHShiftPreAmpGainComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(AndorCmb_PreAmpGain.combo)
       AndorSet1.HShiftPreAmpGain = AndorCmb_PreAmpGain.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
*
****************************************************************************/
void JLP_AndorPanel::OnADComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(AndorCmb_ADChannel.combo)
       AndorSet1.AD_Channel = AndorCmb_ADChannel.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* ShutterMode combo box:
*
* Values of AndorSet1.ShutterMode:
* -1 not available, 0 (automatic), 1 (always open) or  2 (always closed),
* Corresponding values of combobox selection: 0, 1, 2, 3
****************************************************************************/
void JLP_AndorPanel::OnShutterComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(AndorCmb_Shutter.combo)
       AndorSet1.ShutterMode = AndorCmb_Shutter.combo->GetSelection() - 1;

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* Frame Transfer Mode :
****************************************************************************/
void JLP_AndorPanel::OnFrameTransferCheckBoxClick(wxCommandEvent& event)
{
bool is_checked;

if(initialized != 1234) return;

 is_checked = AndorCheck_FrameTransfer->GetValue();
  if(is_checked) AndorSet1.FrameTransferMode = 1;
   else AndorSet1.FrameTransferMode = 0;

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* Baseline clamp
****************************************************************************/
void JLP_AndorPanel::OnBaselineClampCheckBoxClick(wxCommandEvent& event)
{
bool is_checked;

if(initialized != 1234) return;

 is_checked = AndorCheck_BaselineClamp->GetValue();
  if(is_checked) AndorSet1.BaselineClamp = 1;
   else AndorSet1.BaselineClamp = 0;

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* Close shutter when exiting from this program
****************************************************************************/
void JLP_AndorPanel::OnCloseShutterWhenExitCheckBoxClick(wxCommandEvent& event)
{
bool is_checked;

if(initialized != 1234) return;

 is_checked = AndorCheck_CloseShutterWhenExit->GetValue();
  if(is_checked) AndorSet1.CloseShutterWhenExit = 1;
   else AndorSet1.CloseShutterWhenExit = 0;

jcam_changes1->ChangesDone = true;
return;
}
/**************************************************************************
* Handle "Config" buttons:
**************************************************************************/
void JLP_AndorPanel::OnConfig( wxCommandEvent& event )
{
bool update_from_screen;

if(initialized != 1234) return;

switch(event.GetId()) {

  case ID_APANEL_CONFIG_OK:
// Apply those settings to the camera, and store them to "original" settings:
    CamPanel_ValidateNewSettings();
    break;

  case ID_APANEL_CONFIG_CANCEL:
// Cancel button: go back to initial settings
    CamPanel_CancelNonValidatedChanges();
    break;

// Save current config to file:
  case ID_APANEL_CONFIG_SAVE:
// Apply those settings to the camera, and store them to "original" settings:
    CamPanel_ValidateNewSettings();
    SaveAndorSettingsDialog();

    break;

// Load config from file:
  case ID_APANEL_CONFIG_LOAD:
    LoadAndorSettingsDialog();
// Display those new settings:
    update_from_screen = false;
    DisplayNewSettings(update_from_screen);
    jcam_changes1->ChangesDone = true;

    break;

// Load default config
  case ID_APANEL_CONFIG_DEFAULT:
// Set default values:
    m_cam1->Cam1_SettingsToDefault(AndorSet1.gblXPixels,
                                   AndorSet1.gblYPixels);
    m_cam1->Cam1_Get_AndorSettings(&AndorSet1);

// Display those new settings:
    update_from_screen = false;
    DisplayNewSettings(update_from_screen);
    jcam_changes1->ChangesDone = true;

    break;
}

return;
}
