/****************************************************************************
* Name: jlp_raptor_panel_onclick1.cpp
*
* JLP
* Version 16/01/2018
****************************************************************************/
#include "jlp_raptor_panel.h"

#define ABS(a) ((a) >= 0 ? (a) : (-(a)))

/*****************************************************************
* Setting trigger delay value (0_104)
*****************************************************************/
void JLP_RaptorPanel::UpdateTriggerDelay(const bool update_from_screen)
{
wxString buffer;
long ival;
int LowValue = 0, HighValue = 104;

if(update_from_screen) {
 buffer = RaptorCtrl_TriggerDelay->GetValue();
 if(!buffer.ToLong(&ival)) return;
} else {
 ival = RaptorSet1.TriggerDelay;
}

 if((ival >= LowValue) && (ival <= HighValue)){
    RaptorSet1.TriggerDelay = ival;
// If error, erase the new value and display the default value
  } else {
    buffer.Printf(wxT("Error/Valid range for TriggerDelay is [%d,%d]"),
                  LowValue, HighValue);
    wxMessageBox(buffer, wxT("UpdateTriggerDelay/Error"),
                  wxOK | wxICON_ERROR);
    RaptorSet1.TriggerDelay = 0;
   }

// Display current value:
 buffer.Printf(wxT("%d"), RaptorSet1.TriggerDelay);
 RaptorCtrl_TriggerDelay->SetValue(buffer);

return;
}
/********************************************************************
* TriggerDelayEdit box
* WARNING: should wait before doing something anything to avoid blocking !
*
********************************************************************/
void JLP_RaptorPanel::OnTriggerDelayOnExit(wxCommandEvent& event)
{
if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Setting frame exposure time value
*****************************************************************/
void JLP_RaptorPanel::UpdateExpoTime(const bool update_from_screen)
{
wxString buffer;
long ival;
int LowValue = 1, HighValue = 5000;

if(update_from_screen) {
 buffer = RaptorCtrl_ExpoTime->GetValue();
 if(buffer.ToLong(&ival)) RaptorSet1.ExpoTime_msec = ival;
}

// If error, erase the new value and display the default value
 if((RaptorSet1.ExpoTime_msec < LowValue)
         && (RaptorSet1.ExpoTime_msec > HighValue)){
     buffer.Printf(wxT("Error/Valid range for ExpoTime (msec) is [%d,%d]"),
                   LowValue, HighValue);
     wxMessageBox(buffer, wxT("UpdateExpoTime/Error"),
                  wxOK | wxICON_ERROR);
     RaptorSet1.ExpoTime_msec = 10;
   }

// Display current value:
 buffer.Printf(wxT("%d"), RaptorSet1.ExpoTime_msec);
 RaptorCtrl_ExpoTime->SetValue(buffer);

return;
}
/*****************************************************************
* Setting long integration value
*****************************************************************/
void JLP_RaptorPanel::UpdateIntegTime(const bool update_from_screen)
{
wxString buffer0;
long ival;
int LowValue = 0, HighValue = 1000;

if(update_from_screen == true) {
 buffer0 = RaptorCtrl_IntegTime->GetValue();
 if(buffer0.ToLong(&ival)) RaptorSet1.IntegTime_sec = ival;
 }
// If error, erase the new value and display the default value
 if((RaptorSet1.IntegTime_sec < LowValue) || (RaptorSet1.IntegTime_sec > HighValue)){
     buffer0.Printf(wxT("Error/Valid range for LongExposureTime (sec) is [%d,%d]"),
                   LowValue, HighValue);
     wxMessageBox(buffer0, wxT("UpdateIntegTime/Error"),
                  wxOK | wxICON_ERROR);
     RaptorSet1.IntegTime_sec = (LowValue + HighValue ) / 2;
   }

// Display current value:
 buffer0.Printf(wxT("%d"), RaptorSet1.IntegTime_sec);
 RaptorCtrl_IntegTime->SetValue(buffer0);

return;
}
/********************************************************************
* ExpoTimeEdit box
* WARNING: should wait before doing something anything to avoid blocking !
*
********************************************************************/
void JLP_RaptorPanel::OnExpoTimeOnExit(wxCommandEvent& event)
{
if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/********************************************************************
* IntegTimeEdit box
* WARNING: should wait before doing something anything to avoid blocking !
*
********************************************************************/
void JLP_RaptorPanel::OnIntegTimeOnExit(wxCommandEvent& event)
{
if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/********************************************************************
* ROICenter LabeledEdit box
* WARNING: should wait before doing something anything to avoid blocking !
*
********************************************************************/
void JLP_RaptorPanel::OnROICenterOnExit(wxCommandEvent& event)
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
void JLP_RaptorPanel::UpdateROICenter(const bool update_from_screen)
{
wxString buffer0;
char buffer[64];
int ix, iy, ixc0, iyc0, ixc, iyc;
int delta_xmax, delta_ymax;

 if(update_from_screen) {
  buffer0 = RaptorCtrl_ROICenter->GetValue();
  strcpy(buffer, buffer0.mb_str());
  if(sscanf(buffer, "%d,%d", &ix, &iy) == 2) {
   RaptorSet1.xc0 = ix;
   RaptorSet1.yc0 = iy;
   }
 }

// Image center:
 ixc0 = (RaptorSet1.nx1 * RaptorSet1.xbin) /2;
 iyc0 = (RaptorSet1.ny1 * RaptorSet1.ybin) /2;
 ixc = RaptorSet1.gblXPixels/2;
 iyc = RaptorSet1.gblYPixels/2;
 delta_xmax = MAXI(0, ixc - ixc0);
 delta_ymax = MAXI(0, iyc - iyc0);

// If error, erase the new value and set to the default value
 if((RaptorSet1.xc0 < ixc - delta_xmax) || (RaptorSet1.xc0 > ixc + delta_xmax)
     || (RaptorSet1.yc0 < iyc - delta_ymax) || (RaptorSet1.yc0 > iyc + delta_ymax)) {
   buffer0.Printf(wxT("Value=%d,%d: valid range is [%d %d] for xcent and [%d %d] for ycent"),
                  RaptorSet1.xc0, RaptorSet1.yc0, ixc - delta_xmax, ixc + delta_xmax,
                  iyc - delta_ymax, iyc + delta_ymax);
   wxMessageBox(buffer0, wxT("ROICenterOnExit/Error"), wxOK | wxICON_ERROR);
   RaptorSet1.xc0 = RaptorSet1.gblXPixels/2;
   RaptorSet1.yc0 = RaptorSet1.gblYPixels/2;
   }

// Display new value :
 buffer0.Printf(wxT("%d,%d"), RaptorSet1.xc0, RaptorSet1.yc0);
 RaptorCtrl_ROICenter->SetValue(buffer0);

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
void JLP_RaptorPanel::OnAcquiModeComboBoxChange(wxCommandEvent& WXUNUSED(event))
{
int index;

 if(initialized != 1234) return;

 index = RaptorCmb_AcquiMode.combo->GetSelection();
 RaptorSet1.AcquisitionMode = index + 1;

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

 jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* ExtTrigger selection
* 0 = Off => RaptorSet1.ExtTriggerMode = 0;
* 1 = On(Ext +ve Edge) => RaptorSet1.ExtTriggerMode = 1;
* 2 = On(Ext -ve Edge) => RaptorSet1.ExtTriggerMode = 2;
*****************************************************************/
void JLP_RaptorPanel::OnExtTriggerComboBoxChange(wxCommandEvent& event)
{
 if(initialized != 1234) return;

 RaptorSet1.ExtTrigger = RaptorCmb_ExtTrigger.combo->GetSelection();

// Enable/Disable TriggerDelay edit box /Frame rate combo box:
// FrameRate activated only when internal trigger is activated
 if(RaptorSet1.ExtTrigger == 0) {
  RaptorCtrl_TriggerDelay->Enable(false);
  RaptorCmb_FrameRate.combo->Enable(true);
 } else {
  RaptorCtrl_TriggerDelay->Enable(true);
  RaptorCmb_FrameRate.combo->Enable(false);
 }

 jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Edit box for cooler temperature
*****************************************************************/
void JLP_RaptorPanel::OnCoolerSetPointEditExit(wxCommandEvent& event)
{

 if(initialized != 1234) return;

 jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Edit box for cooler temperature
*****************************************************************/
void JLP_RaptorPanel::UpdateCoolerSetPoint(const bool update_from_screen)
{
wxString buffer;
double dval, TempLowValue = -15., TempHighValue = 15.;

 if(update_from_screen == true) {
   buffer = RaptorCtrl_CoolerSetTemp->GetValue();
   if(buffer.ToDouble(&dval)) RaptorSet1.CoolerSetTemp = dval;
   }

// If error, erase the new value and display the default value
 if((dval < TempLowValue) || (dval > TempHighValue)){
   buffer.Printf(wxT("Error/Valid range for Cooler Set Temperature is [%.2f,%.2f]"),
                 TempLowValue, TempHighValue);
   wxMessageBox(buffer, wxT("CoolerSetTemp/Error"), wxOK | wxICON_ERROR);
   RaptorSet1.CoolerSetTemp = (TempLowValue + TempHighValue ) / 2.;
   }

// Display current value:
 buffer.Printf(wxT("%.2f"), RaptorSet1.CoolerSetTemp);
 RaptorCtrl_CoolerSetTemp->SetValue(buffer);

return;
}
/*****************************************************************
* Cooler: On/Off
*****************************************************************/
void JLP_RaptorPanel::OnCoolerCheckBoxClick(wxCommandEvent& event)
{
  if(initialized != 1234) return;

  RaptorSet1.CoolerOn = RaptorCheck_CoolerRegul->IsChecked();

// Enable/disable cooler temperature edit box:
  RaptorCtrl_CoolerSetTemp->Enable(RaptorSet1.CoolerOn);

  jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Cooler fan: On/Off
*****************************************************************/
void JLP_RaptorPanel::OnCoolerFanCheckBoxClick(wxCommandEvent& event)
{
  if(initialized != 1234) return;

  RaptorSet1.FanIsOn = RaptorCheck_CoolerFan->IsChecked();

  jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* High Gain checkbox
*****************************************************************/
void JLP_RaptorPanel::OnHighGainCheckBoxClick(wxCommandEvent& event)
{

if(initialized != 1234) return;

 if(RaptorCheck_HighGain->IsChecked()) {
   RaptorSet1.HighGain = 1;
// Disable gain edit box:
   RaptorCtrl_DigitalGain->Enable(false);
  } else {
   RaptorSet1.HighGain = 0;
// Enable gain edit box:
   RaptorCtrl_DigitalGain->Enable(true);
  }

 jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Digital Gain value has been modified...
*****************************************************************/
void JLP_RaptorPanel::OnDigitalGainEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

 jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Setting Digital gain value
*****************************************************************/
void JLP_RaptorPanel::UpdateDigitalGain(const bool update_from_screen)
{
wxString buffer;
long ival;
int status = -1, GainLowValue = 0, GainHighValue = 48;

 buffer = RaptorCtrl_DigitalGain->GetValue();

 if(buffer.ToLong(&ival)) {
   status = 1;
   if((ival >= GainLowValue)
         && (ival <= GainHighValue)) {
       RaptorSet1.DigitalGain = ival;
       status = 0;
    }
 }
// If error, erase the new value and display the default value
 if(status) {
     if(status == 1) {
     buffer.Printf(wxT("Error/Valid range for DigitalGain is [%d,%d]"),
                   GainLowValue, GainHighValue);
     wxMessageBox(buffer, wxT("DigitalGain/Error"),
                  wxOK | wxICON_ERROR);
     }
     RaptorSet1.DigitalGain = (GainLowValue + GainHighValue ) / 2;
   }

// Display current value:
 buffer.Printf(wxT("%d"), RaptorSet1.DigitalGain);
 RaptorCtrl_DigitalGain->SetValue(buffer);

return;
}
/****************************************************************************
*
****************************************************************************/
void JLP_RaptorPanel::OnBinFactorComboBoxChange(wxCommandEvent& event)
{
int index, nx0, ny0;

if(initialized != 1234) return;

nx0 = RaptorSet1.nx1 * RaptorSet1.xbin;
ny0 = RaptorSet1.ny1 * RaptorSet1.ybin;

// Handle Binning ComboBox for bin factor
// Values: 1, 2, 4 or 8

index = RaptorCmb_BinFactor.combo->GetSelection();
switch (index) {
   default:
   case 0:
     RaptorSet1.xbin = 1;
     RaptorSet1.ybin = 1;
     break;
   case 1:
     RaptorSet1.xbin = 2;
     RaptorSet1.ybin = 2;
     break;
   case 2:
     RaptorSet1.xbin = 4;
     RaptorSet1.ybin = 4;
     break;
   case 3:
     RaptorSet1.xbin = 8;
     RaptorSet1.ybin = 8;
     break;
   }

// ROI size should remain the same afetr bin change:
 RaptorSet1.nx1 = nx0 / RaptorSet1.xbin;
 RaptorSet1.ny1 = ny0 / RaptorSet1.ybin;

// Update final image size in static text label:
 UpdateFinalImageSizeLabels();

 jcam_changes1->ChangesDone = true;

return;
}
/***************************************************************************
* Selection of new item in Region Of Interest Size ComboBox
*
* New ROI size RaptorSet1.nx0, RaptorSet1.ny0
* 0 = 64x64
* 1 = 128x128
* 2 = 256x256
* 3 = 512x512
* 4 = full size
*
***************************************************************************/
void JLP_RaptorPanel::OnROISizeComboBoxChange(wxCommandEvent& event)
{
int index, nx0, ny0;

if(initialized != 1234) return;

index = RaptorCmb_ROISize.combo->GetSelection();
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
     if((RaptorSet1.gblXPixels >= 256)
         && (RaptorSet1.gblXPixels >= 256)) {
       nx0 = 256;
       ny0 = 256;
     } else {
       nx0 = RaptorSet1.gblXPixels;
       ny0 = RaptorSet1.gblYPixels;
     }
     break;
   case 3:
     if((RaptorSet1.gblXPixels >= 512)
         && (RaptorSet1.gblXPixels >= 512)) {
       nx0 = 512;
       ny0 = 512;
     } else {
       nx0 = RaptorSet1.gblXPixels;
       ny0 = RaptorSet1.gblYPixels;
     }
     break;
   case 4:
     nx0 = RaptorSet1.gblXPixels;
     ny0 = RaptorSet1.gblYPixels;
     break;
   }
   RaptorSet1.nx1 = nx0 / RaptorSet1.xbin;
   RaptorSet1.ny1 = ny0 / RaptorSet1.ybin;
   UpdateFinalImageSizeLabels();

jcam_changes1->ChangesDone = true;

return;
}
/****************************************************************************
* Mirror ComboBox
****************************************************************************/
void JLP_RaptorPanel::OnMirrorComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(RaptorCmb_Mirror.combo)
    RaptorSet1.MirrorMode = RaptorCmb_Mirror.combo->GetSelection();

jcam_changes1->ChangesDone = true;

return;
}
/****************************************************************************
* Rotation ComboBox
****************************************************************************/
void JLP_RaptorPanel::OnRotationComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(RaptorCmb_Rotation.combo)
    RaptorSet1.RotationMode = RaptorCmb_Rotation.combo->GetSelection();

jcam_changes1->ChangesDone = true;
}
/****************************************************************************
*
* Frame rate :
* 0: 25 Hz
* 1: 30 Hz
* 2: 50 Hz
* 3: 60 Hz
* 4: 90 Hz
* 5: 120 Hz
****************************************************************************/
void JLP_RaptorPanel::OnFrameRateComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(RaptorCmb_FrameRate.combo)
       RaptorSet1.FrameRate = RaptorCmb_FrameRate.combo->GetSelection();

jcam_changes1->ChangesDone = true;

return;
}
/****************************************************************************
* NUC State:
* 0: Offset+Gain corr.
* 1: Normal
* 2: Offset+Gain+Dark
* 3: 8 bit Offset - 32
* 4: 8 bit Dark - 2^19
* 5: 8 bit Gain - 128
* 6: Reversed map
* 7: Ramp Test Pattern
****************************************************************************/
void JLP_RaptorPanel::OnNucStateComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

if(RaptorCmb_NUC_State.combo)
       RaptorSet1.NUC_State = RaptorCmb_NUC_State.combo->GetSelection();

jcam_changes1->ChangesDone = true;
return;
}
/****************************************************************************
* ShutterMode combo box:
*
* Values of RaptorSet1.ShutterMode:
* -1 not available, 0 (automatic), 1 (always open) or  2 (always closed),
* Corresponding values of combobox selection: 0, 1, 2, 3
****************************************************************************/
void JLP_RaptorPanel::OnShutterComboBoxChange(wxCommandEvent& event)
{

if(initialized != 1234) return;

/*
if(RaptorCmb_Shutter.combo)
       RaptorSet1.ShutterMode = RaptorCmb_Shutter.combo->GetSelection() - 1;
*/
jcam_changes1->ChangesDone = true;

return;
}
/****************************************************************************
* Auto Exposure:
****************************************************************************/
void JLP_RaptorPanel::OnAutoExposureCheckBoxClick(wxCommandEvent& event)
{
bool is_checked;

if(initialized != 1234) return;

 is_checked = RaptorCheck_AutoExposure->GetValue();
  if(is_checked) RaptorSet1.AutoExposure = 1;
   else RaptorSet1.AutoExposure = 0;

// Enable/disable ExpoTime EditBox :
   UpdateAutoExposure();

jcam_changes1->ChangesDone = true;

return;
}
/****************************************************************************
* ImageSharpen
****************************************************************************/
void JLP_RaptorPanel::OnImageSharpenCheckBoxClick(wxCommandEvent& event)
{
bool is_checked;

if(initialized != 1234) return;

 is_checked = RaptorCheck_ImageSharpen->GetValue();
  if(is_checked) RaptorSet1.ImageSharpen = 1;
   else RaptorSet1.ImageSharpen = 0;

jcam_changes1->ChangesDone = true;

return;
}
/**************************************************************************
* Handle "Config" buttons:
**************************************************************************/
void JLP_RaptorPanel::OnConfig( wxCommandEvent& event )
{
bool update_from_screen;

if(initialized != 1234) return;

switch(event.GetId()) {

  case ID_RPANEL_CONFIG_OK:
// Apply those settings to the camera, and store them to "original" settings:
    CamPanel_ValidateNewSettings();
    break;

  case ID_RPANEL_CONFIG_CANCEL:
// Cancel button: go back to initial settings
    CamPanel_CancelNonValidatedChanges();
    break;

// Save current config to file:
  case ID_RPANEL_CONFIG_SAVE:
// Apply those settings to the camera, and store them to "original" settings:
    CamPanel_ValidateNewSettings();
    SaveRaptorSettingsDialog();

    break;

// Load config from file:
  case ID_RPANEL_CONFIG_LOAD:
    LoadRaptorSettingsDialog();
// Display those new settings:
    update_from_screen = false;
    DisplayNewSettings(update_from_screen);
    jcam_changes1->ChangesDone = true;

    break;

// Load default config
  case ID_RPANEL_CONFIG_DEFAULT:
// Set default values:
    m_cam1->Cam1_SettingsToDefault(RaptorSet1.gblXPixels,
                                   RaptorSet1.gblYPixels);
    m_cam1->Get_RaptorSettings(&RaptorSet1);

// Display those new settings:
    update_from_screen = false;
    DisplayNewSettings(update_from_screen);
    jcam_changes1->ChangesDone = true;

    break;
}

return;
}
/**************************************************************************
* Handle "Camera Current status" button:
**************************************************************************/
void JLP_RaptorPanel::OnCurrentStatus( wxCommandEvent& event )
{
double sensor_temp0, box_temp0, expo_time0, digital_gain0;
wxString buffer, info_msg;

 m_cam1->Cam1_GetTemperature(&sensor_temp0, &box_temp0, info_msg);
 buffer.Printf("%.2f", box_temp0);
 RaptorStatic_StatusBoxTemp->SetLabel(buffer);
 buffer.Printf("%.2f", sensor_temp0);
 RaptorStatic_StatusSensorTemp->SetLabel(buffer);

 m_cam1->Cam1_GetExpoTime(&expo_time0);
 buffer.Printf("%.2f", expo_time0);
 RaptorStatic_StatusExpoTime->SetLabel(buffer);

 m_cam1->Cam1_GetDigitalGain(&digital_gain0);
 buffer.Printf("%d", (int)digital_gain0);
 RaptorStatic_StatusDigitalGain->SetLabel(buffer);

}
