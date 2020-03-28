/****************************************************************************
* Name: jlp_andor_panel_onclick2.cpp
*
* JLP
* Version 26/08/2015
****************************************************************************/
#include "jlp_andor_panel.h"

/****************************************************************************
* To select the exposure time in milliseconds
* Time allowed between 1 msec and 10 seconds
* NB: AndorSet1.KineticExpoTime_msec <= AndorSet1.KineticClockTime_msec
*
****************************************************************************/
void JLP_AndorPanel::OnKineticExpoTimeEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/****************************************************************************
* To select the exposure time in milliseconds
* Time allowed between 1 msec and 10 seconds
* NB: AndorSet1.KineticExpoTime_msec <= AndorSet1.KineticClockTime_msec
****************************************************************************/
void JLP_AndorPanel::UpdateKineticExpoTime(const bool update_from_screen)
{
wxString buffer;
long ival;
int status = -1;

 buffer = AndorCtrl_KinExpoTime->GetValue();
 if(buffer.ToLong(&ival)) {
    status = 1;
    if((ival > 1) && (ival <= 10000)) {
       AndorSet1.KineticExpoTime_msec = ival;
       status = 0;
    }
 }
// If error, erase the new value and display the previously saved value
 if(status) {
     if(status == 1) {
       buffer.Printf(wxT("Value=%ld : valid range is 1 10000 milliseconds!"),
                     ival);
       wxMessageBox(buffer,
                  wxT("KineticExpoTime/Error"), wxOK | wxICON_ERROR);
     }
     buffer.Printf(wxT("%d"), AndorSet1.KineticExpoTime_msec);
     AndorCtrl_KinExpoTime->SetValue(buffer);
   }

CheckAndCorrectKineticParameters();
return;
}
/*****************************************************************
* Make parameters compatible and update long exposure
*****************************************************************/
void JLP_AndorPanel::CheckAndCorrectKineticParameters()
{
wxString buffer;

// Step 1: assume that exposure time is OK
// Increase Kinetic clock time if needed:
 if(AndorSet1.KineticExpoTime_msec >= AndorSet1.KineticClockTime_msec) {
    AndorSet1.KineticClockTime_msec = AndorSet1.KineticExpoTime_msec + 1;
    buffer.Printf(wxT("%d"), AndorSet1.KineticClockTime_msec);
    AndorCtrl_KinClockTime->SetValue(buffer);
    }

// Step 2: assume that clock time is OK
// Increase Kinetic cycle time if needed:
  if(AndorSet1.KineticClockTime_msec > AndorSet1.KineticCycleTime_msec) {
    AndorSet1.KineticCycleTime_msec =
       AndorSet1.KineticClockTime_msec * AndorSet1.KineticCycleNExposures + 1;
    buffer.Printf(wxT("%d"), AndorSet1.KineticCycleTime_msec);
    AndorCtrl_KinCycleTime->SetValue(buffer);
    }

// Step 3: assume that cycle time and Ncycles are OK
// Update total number of exposures:
//     AndorSet1.NExposures = AndorSet1.KineticCycleNExposures * AndorSet1.KineticNCycles;
// Update total exposure time (conversion from msec to seconds):
 AndorSet1.IntegTime_sec = AndorSet1.KineticCycleTime_msec
                                 * AndorSet1.KineticNCycles / 1000.;
 buffer.Printf(wxT("%d"), AndorSet1.IntegTime_sec);
 AndorCtrl_LongExpoTime->SetValue(buffer);

return;
}
/*****************************************************************
* Number of exposures/cycle
*****************************************************************/
void JLP_AndorPanel::OnKineticCycleNExposuresEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Number of exposures/cycle
*****************************************************************/
void JLP_AndorPanel::UpdateKineticCycleNExposures(const bool update_from_screen)
{
wxString buffer;
long ival;
int status = -1;

buffer = AndorCtrl_KinNExp->GetValue();

if(buffer.ToLong(&ival)) {
  status = 1;
  if((ival >= 1) && (ival < 100)){
    AndorSet1.KineticCycleNExposures = ival;
    status = 0;
  }
}
// If error, erase the new value and display the previously saved value
 if(status) {
    if(status == 1) {
    wxMessageBox(wxT("Valid range is [1 100] for KineticCycleNexposures"),
                wxT("KineticCycleNExposures/Error"),
                wxOK | wxICON_ERROR);
    }
    buffer.Printf(wxT("%d"), AndorSet1.KineticCycleNExposures);
    AndorCtrl_KinNExp->SetValue(buffer);
  }

CheckAndCorrectKineticParameters();
return;
}
/*****************************************************************
* Kinetic of the clock used to start the elementary exposures
*****************************************************************/
void JLP_AndorPanel::OnKineticClockTimeEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Kinetic of the clock used to start the elementary exposures
*****************************************************************/
void JLP_AndorPanel::UpdateKineticClockTime(const bool update_from_screen)
{
wxString buffer;
long ival;
int status = -1;

buffer = AndorCtrl_KinClockTime->GetValue();
if(buffer.ToLong(&ival)) {
  status = 1;
  if((ival > 2) && (ival <= 10000)) {
    AndorSet1.KineticClockTime_msec = ival;
 // Correct KineticExpoTime if needed
    if(ival < AndorSet1.KineticExpoTime_msec) {
      AndorSet1.KineticExpoTime_msec = ival - 1;
      buffer.Printf(wxT("%d"), AndorSet1.KineticExpoTime_msec);
      AndorCtrl_KinExpoTime->SetValue(buffer);
      }
    status = 0;
    }
  }

// If error, erase the new value. Then set and display the previously saved value
if(status) {
 if(status == 1) {
   wxMessageBox(wxT("Valid range is 2 10000 milliseconds and should be larger than ExposureTime"),
                wxT("KineticClockTime/Error"), wxOK | wxICON_ERROR);
   }
 buffer.Printf(wxT("%d"), AndorSet1.KineticClockTime_msec);
 AndorCtrl_KinClockTime->SetValue(buffer);
}


CheckAndCorrectKineticParameters();
return;
}
/*****************************************************************
* Kinetic of the clock used to start the elementary exposures
*****************************************************************/
void JLP_AndorPanel::OnKineticCycleTimeEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/*****************************************************************
* Kinetic of the clock used to start the elementary exposures
*****************************************************************/
void JLP_AndorPanel::UpdateKineticCycleTime(const bool update_from_screen)
{
wxString buffer;
long ival;
int status = -1;

buffer = AndorCtrl_KinCycleTime->GetValue();
if(buffer.ToLong(&ival)) {
  status = 1;
  if((ival > 3) && (ival <= 10000)
      && (ival > AndorSet1.KineticExpoTime_msec)
      && (ival >= AndorSet1.KineticClockTime_msec
                  * AndorSet1.KineticCycleNExposures)) {
      AndorSet1.KineticCycleTime_msec = ival;
      status = 0;
  }
}

// If error, erase the new value and display the default value
if(status) {
  if(status == 1) {
     wxMessageBox(wxT("Valid range is 3 10000 milliseconds \
and should be larger than KineticExpoTime_msec and KineticClockTime * KineticCycleNExposures!"),
                  wxT("KineticCycleTime/Error"),
                  wxOK | wxICON_ERROR);
  }
     AndorSet1.KineticCycleTime_msec = 10
              + AndorSet1.KineticClockTime_msec * AndorSet1.KineticCycleNExposures;
     if(AndorSet1.KineticCycleTime_msec < AndorSet1.KineticExpoTime_msec) {
        AndorSet1.KineticCycleTime_msec = AndorSet1.KineticExpoTime_msec + 1;
     buffer.Printf(wxT("%d"), AndorSet1.KineticCycleTime_msec);
     AndorCtrl_KinCycleTime->SetValue(buffer);
  }
}

//JLP2015
// Update total number of exposures:
//     AndorSet1.NExposures = AndorSet1.KineticCycleNExposures * AndorSet1.KineticNCycles;
// Update total exposure time (conversion from msec to seconds):
 AndorSet1.IntegTime_sec = AndorSet1.KineticCycleTime_msec
                                 * AndorSet1.KineticNCycles / 1000.;
 buffer.Printf(wxT("%d"), AndorSet1.IntegTime_sec);
 AndorCtrl_LongExpoTime->SetValue(buffer);

CheckAndCorrectKineticParameters();
return;
}
/****************************************************************************
* Long exposure (seconds)
****************************************************************************/
void JLP_AndorPanel::UpdateLongExposureTime(const bool update_from_screen)
{
wxString buffer;
double val;
int status = -1;

  AndorCtrl_LongExpoTime->GetValue();
  if(buffer.ToDouble(&val)) {
     status = 1;
     if((val > 0.01) && (val <= 1000.)) {
       AndorSet1.IntegTime_sec = val;
       status = 0;
     }
  }

  if(status) {
// If error, erase the new value and display the old value
// WARNING: this field is not always accesible, so generally do not dsplay error message:
     if(status == 1) {
       buffer.Printf(wxT("Value=%f whereas the valid range is 0.1 1000 seconds!"),
                    val);
       wxMessageBox(buffer, wxT("LongExposureTime/Error"),
                    wxOK | wxICON_ERROR);
       }
     buffer.Printf(wxT("%d"), AndorSet1.IntegTime_sec);
     AndorCtrl_LongExpoTime->SetValue(buffer);
  }

CheckAndCorrectKineticParameters();
return;
}
/****************************************************************************
* Number of kinetic cycles
****************************************************************************/
void JLP_AndorPanel::OnKineticNCyclesEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

jcam_changes1->ChangesDone = true;

return;
}
/****************************************************************************
* Number of kinetic cycles
****************************************************************************/
void JLP_AndorPanel::UpdateKineticNCycles(const bool update_from_screen)
{
wxString buffer;
long ival;
int status = -1;

  buffer = AndorCtrl_KinNCycle->GetValue();

  if(buffer.ToLong(&ival)) {
    status = 1;
    if((ival >= 1) && (ival < 1000000)){
      AndorSet1.KineticNCycles = ival;
      status = 0;
    }
  }
// If error, erase the new value and display the previously saved value
  if(status) {
    if(status == 1) {
    wxMessageBox(wxT("Valid range is [1 1000000] for KineticNCycles"),
                 wxT("KineticNCycles/Error"), wxOK | wxICON_ERROR);
    }
    buffer.Printf(wxT("%d"), AndorSet1.KineticNCycles);
    AndorCtrl_KinNCycle->SetValue(buffer);
  }

CheckAndCorrectKineticParameters();
return;
}
