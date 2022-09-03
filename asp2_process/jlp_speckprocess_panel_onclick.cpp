/****************************************************************************
* Name: jlp_speckprocess_panel_onclick.cpp
*
* JLP
* Version 30/08/2015
****************************************************************************/
#include "jlp_speckprocess_panel.h"
#include "jlp_rw_fits0.h"            // readimag_float_fits()

BEGIN_EVENT_TABLE(JLP_SpeckProcessPanel, wxPanel)

// Buttons:
EVT_BUTTON  (ID_PROC_CONFIG_OK, JLP_SpeckProcessPanel::OnConfig)
EVT_BUTTON  (ID_PROC_CONFIG_CANCEL, JLP_SpeckProcessPanel::OnConfig)
EVT_BUTTON  (ID_PROC_CONFIG_SAVE, JLP_SpeckProcessPanel::OnConfig)
EVT_BUTTON  (ID_PROC_CONFIG_LOAD, JLP_SpeckProcessPanel::OnConfig)
EVT_BUTTON  (ID_PROC_CONFIG_DEFAULT, JLP_SpeckProcessPanel::OnConfig)

EVT_BUTTON  (ID_PROC_LOAD_FFIELD, JLP_SpeckProcessPanel::OnLoadFFieldFrame)
EVT_BUTTON  (ID_PROC_LOAD_OFFSET, JLP_SpeckProcessPanel::OnLoadOffsetFrame)
EVT_BUTTON  (ID_PROC_LOAD_UNRES_AUTOC, JLP_SpeckProcessPanel::OnLoadUnresolvedAutocFrame)
EVT_BUTTON  (ID_PROC_LOAD_UNRES_MODSQ, JLP_SpeckProcessPanel::OnLoadUnresolvedModsqFrame)
EVT_BUTTON  (ID_PROC_LOAD_PHOT_MODSQ, JLP_SpeckProcessPanel::OnLoadPhotModsqFrame)

// Text controls:
EVT_TEXT  (ID_PROC_SEEING_THRESH, JLP_SpeckProcessPanel::OnSeeingSelection)
EVT_TEXT  (ID_PROC_MAX_THRESH, JLP_SpeckProcessPanel::OnMaxValSelectionCheckBoxClick)
EVT_TEXT  (ID_PROC_OFFSET_THRESH, JLP_SpeckProcessPanel::OnOffsetThreshEditExit)
EVT_TEXT  (ID_PROC_BISP_IR, JLP_SpeckProcessPanel::OnBisp_irEditExit)
EVT_TEXT  (ID_PROC_BISP_NCLOS, JLP_SpeckProcessPanel::OnBisp_nclosEditExit)
EVT_TEXT  (ID_PROC_SLIT_ANGLE, JLP_SpeckProcessPanel::OnSlitAngle)
EVT_TEXT  (ID_PROC_SPECT_ANGLE, JLP_SpeckProcessPanel::OnSpectrumAngle)
EVT_TEXT  (ID_PROC_NZ_CUBE, JLP_SpeckProcessPanel::On_nz_cube)
EVT_TEXT  (ID_PROC_FFTW_DIR, JLP_SpeckProcessPanel::OnfftwDirEditExit)
EVT_TEXT  (ID_PROC_3DFITS_DIR, JLP_SpeckProcessPanel::OnOut3DFitsDirEditExit)
EVT_TEXT  (ID_PROC_RESULTS_DIR, JLP_SpeckProcessPanel::OnOutResultsDirEditExit)

// Check boxes:
EVT_CHECKBOX(ID_PROC_DIRECT_VECTOR, JLP_SpeckProcessPanel::OnDirectVectorCheckBoxClick)
EVT_CHECKBOX(ID_PROC_LUCKY_CHCK, JLP_SpeckProcessPanel::OnLuckyCheckBoxClick)
EVT_CHECKBOX(ID_PROC_STATS_CHCK, JLP_SpeckProcessPanel::OnStatisticsCheckBoxClick)
EVT_CHECKBOX(ID_PROC_SEEING_SELECT, JLP_SpeckProcessPanel::OnSeeingSelectionCheckBoxClick)
EVT_CHECKBOX(ID_PROC_MAX_SELECT, JLP_SpeckProcessPanel::OnMaxValSelectionCheckBoxClick)

EVT_RADIOBUTTON (ID_PROC_OFFSET_THRESH, JLP_SpeckProcessPanel::OnSelectOffsetCorr)
EVT_RADIOBUTTON (ID_PROC_OFFSET_FRAME, JLP_SpeckProcessPanel::OnSelectOffsetCorr)
EVT_RADIOBUTTON (ID_PROC_FFCORR, JLP_SpeckProcessPanel::OnSelectFFieldCorr)
EVT_RADIOBUTTON (ID_PROC_NO_FFCORR, JLP_SpeckProcessPanel::OnSelectFFieldCorr)

// Combo boxes:
EVT_COMBOBOX(ID_PROC_MODES, JLP_SpeckProcessPanel::OnSelectProcMode)
EVT_COMBOBOX(ID_PROC_SAVE_3DFITS, JLP_SpeckProcessPanel::OnSelectSave3DFits)
EVT_COMBOBOX(ID_PROC_KK_CROSSCOR, JLP_SpeckProcessPanel::OnSelect_KK_CrossCorr)

END_EVENT_TABLE()

/*************************************************************************
* Selection of new processing mode
 ProcRad_Mode_str[0] = _T("Aucun (visualisation seulement)");
 ProcRad_Mode_str[1] = _T("Int\u00e9gration (avec/sans recentrage)");
 ProcRad_Mode_str[2] = _T("Autoc, spectre");
 ProcRad_Mode_str[3] = _T("Autoc, spectre, quadrant");
 ProcRad_Mode_str[4] = _T("Bispectre, spectre");
 ProcRad_Mode_str[5] = _T("Bispectre, autoc, quadrant");
 ProcRad_Mode_str[6] = _T("Spectroscopie");
 ProcRad_Mode_str[7] = _T("SCIDAR-Lin\u00e9aire");
 ProcRad_Mode_str[8] = _T("SCIDAR-Logarithme");
*****************************************************************************/
void JLP_SpeckProcessPanel::OnSelectProcMode(wxCommandEvent& event)
{
wxString buffer;

 if(initialized != 1234) return;

// Get selection from combobox:
  Pset1.ProcessingMode = ProcCmb_Mode.combo->GetSelection();

// Enable/Disable other items according to the selected processing mode:
  ProcCtrl_Bisp_ir->Enable(Pset1.ProcessingMode == 4  || Pset1.ProcessingMode == 5);
  ProcCtrl_Bisp_nclos->Enable(Pset1.ProcessingMode == 4  || Pset1.ProcessingMode == 5);
  ProcCtrl_SlitAngle->Enable(Pset1.ProcessingMode == 6);
  ProcCtrl_SpectAngle->Enable(Pset1.ProcessingMode == 6);
  ProcCheck_MaxSelect->Enable(Pset1.ProcessingMode != 0);
  ProcCheck_SeeingSelect->Enable(Pset1.ProcessingMode != 0);
  if(Pset1.ProcessingMode >= 1 && Pset1.ProcessingMode <= 5) {
    ProcCheck_Lucky->Enable(true);
    ProcCheck_DirectVector->Enable(true);
  } else {
    ProcCheck_Lucky->Enable(false);
    ProcCheck_DirectVector->Enable(false);
  }

// Set nz cubes according to selection:
// 0=visu only, 1=long integration
 if(Pset1.ProcessingMode <= 1) Pset1.nz_cube = 10;
  else Pset1.nz_cube = 200;

// Update screen:
  buffer.Printf(wxT("%d"), Pset1.nz_cube);
  ProcCtrl_nz_cube->SetValue(buffer);

ChangesDone1 = true;
}
/**************************************************************************
* New value of ir_max  (maximum radius of frequency circular support)
***************************************************************************/
void JLP_SpeckProcessPanel::OnBisp_irEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

ChangesDone1 = true;
return;
}
/**************************************************************************
* New value of ir_max  (maximum radius of frequency circular support)
***************************************************************************/
void JLP_SpeckProcessPanel::UpdateBisp_ir_FromScreen()
{
wxString buffer;
long ir;
int status = -1;

buffer = ProcCtrl_Bisp_ir->GetValue();

// Get ir_max:
if(buffer.ToLong(&ir)) {
 status = 1;
 if(ir >= 10 && ir <= 60) {
  Pset1.ir_bisp = ir;
  status = 0;
 }
}

// If error, erase the new value and display the default value
if(status ) {
  if(status == 1) {
     wxMessageBox(wxT("Bad syntax/bad value (should be in range [10, 60])"),
                  wxT("ir_max/Error"), wxOK | wxICON_ERROR);
  }
  Pset1.ir_bisp = 30;
  buffer.Printf(wxT("%d"), Pset1.ir_bisp);
  ProcCtrl_Bisp_ir->SetValue(buffer);
}

return;
}
/**************************************************************************
* Maximum number of closure relations for bispectrum:
*
***************************************************************************/
void JLP_SpeckProcessPanel::OnBisp_nclosEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

ChangesDone1 = true;
return;
}
/**************************************************************************
* Maximum number of closure relations for bispectrum:
*
***************************************************************************/
void JLP_SpeckProcessPanel::UpdateBisp_nclos_FromScreen()
{
wxString buffer;
long nclos;
int status = -1;

  buffer = ProcCtrl_Bisp_nclos->GetValue();

// Get nclos_max:
if(buffer.ToLong(&nclos)) {
  status = 1;
  if(nclos >= 10 && nclos <= 1000) {
    Pset1.nmax_bisp = nclos;
    status = 0;
  }
}
// If error, erase the new value and display the default value
  if(status) {
    if(status == 1) {
     wxMessageBox(wxT("Bad syntax/bad value (should be in range [10, 1000])"),
                  wxT("nclos_max/Error"), wxOK | wxICON_ERROR);
    }
     Pset1.nmax_bisp = 50;
     buffer.Printf(wxT("%d"), Pset1.nmax_bisp);
     ProcCtrl_Bisp_nclos->SetValue(buffer);
   }

return;
}
/***************************************************************************
* New value of slit angle (slit spectroscopic mode)
***************************************************************************/
void JLP_SpeckProcessPanel::OnSlitAngle(wxCommandEvent& event)
{

if(initialized != 1234) return;

ChangesDone1 = true;
return;
}
/***************************************************************************
* New value of slit angle (slit spectroscopic mode)
***************************************************************************/
void JLP_SpeckProcessPanel::UpdateSlitAngleFromScreen()
{
wxString buffer;
double angle;
int status = -1;

  buffer = ProcCtrl_SlitAngle->GetValue();

// Get slangle:
  if(buffer.ToDouble(&angle)) {
    status = 1;
     if(angle > -180. && angle < 360.) {
       Pset1.slangle = angle;
       status = 0;
     }
  }

if(status){
// If error, erase the new value and display the default value
  if(status == 1){
     wxMessageBox(wxT("Bad syntax/bad value (should be in range ]-180, 360[)"),
                  wxT("SlitAngle/Error"), wxOK | wxICON_ERROR);
  }
  Pset1.slangle = 0.;
  buffer.Printf(wxT("%.2f"), Pset1.slangle);
  ProcCtrl_SlitAngle->SetValue(buffer);
}

return;
}
/****************************************************************************
* New value of spectrum angle (slit spectroscopic mode)
*
****************************************************************************/
void JLP_SpeckProcessPanel::OnSpectrumAngle(wxCommandEvent& event)
{

if(initialized != 1234) return;

ChangesDone1 = true;
return;
}
/****************************************************************************
* New value of spectrum angle (slit spectroscopic mode)
*
****************************************************************************/
void JLP_SpeckProcessPanel::UpdateSpectrumAngleFromScreen()
{
wxString buffer;
double angle;
int status = -1;

  buffer = ProcCtrl_SpectAngle->GetValue();

// Get spangle:
  if(buffer.ToDouble(&angle)) {
   status = 1;
     if(angle > -180. && angle < 360.) {
       Pset1.spangle = angle;
       status = 0;
     }
  }
// If error, erase the new value and display the default value
  if(status){
    if(status == 1) {
     wxMessageBox(wxT("Bad syntax/bad value (should be in range ]-180, 360[)"),
                  wxT("SpectrumAngle/Error"), wxOK | wxICON_ERROR);
    }
    Pset1.spangle = 0.;
    buffer.Printf(wxT("%.2f"), Pset1.spangle);
    ProcCtrl_SpectAngle->SetValue(buffer);
   }

return;
}
/****************************************************************************
* New value of seeing selection parameters (min/max)
*****************************************************************************/
void JLP_SpeckProcessPanel::OnSeeingSelection(wxCommandEvent& event)
{

if(initialized != 1234) return;

ChangesDone1 = true;
return;
}
/****************************************************************************
* New value of seeing selection parameters (min/max)
*****************************************************************************/
void JLP_SpeckProcessPanel::UpdateSeeingSelectionFromScreen()
{
wxString buffer;
double minval, maxval;
int status = -1;

  buffer = ProcCtrl_SeeingThresh->GetValue();

// Get min, max:
  if(sscanf(buffer.c_str(), "%lf,%lf", &minval, &maxval) == 2) {
   status = 1;
     if((minval >= 0.) && (minval <= maxval) && (maxval <= 100.)) {
       Pset1.FwhmSelect.LowCut = minval;
       Pset1.FwhmSelect.HighCut = maxval;
       status = 0;
     }
  }
// If error, erase the new value and display the default value
  if(status) {
    if(status == 1) {
     wxMessageBox(wxT("Bad syntax/bad value (should be in range [0, 100])"),
                  wxT("SeeingSelection/Error"), wxOK | wxICON_ERROR);
    }
    Pset1.FwhmSelect.LowCut = 0.;
    Pset1.FwhmSelect.HighCut = 5.;
    buffer.Printf(wxT("%.2f,%.2f"), Pset1.FwhmSelect.LowCut,
                  Pset1.FwhmSelect.HighCut);
    ProcCtrl_SeeingThresh->SetValue(buffer);
    }

return;
}
/****************************************************************************
* New value of max value selection (min/max)
*****************************************************************************/
void JLP_SpeckProcessPanel::OnMaxValSelection(wxCommandEvent& event)
{

if(initialized != 1234) return;

ChangesDone1 = true;
return;
}
/****************************************************************************
* New value of max value selection (min/max)
*****************************************************************************/
void JLP_SpeckProcessPanel::UpdateMaxValSelectionFromScreen()
{
wxString buffer;
double minval, maxval;
int status = -1;

  buffer = ProcCtrl_MaxThresh->GetValue();

// Get min, max:
  if(sscanf(buffer.c_str(), "%lf,%lf", &minval, &maxval) == 2) {
    status = 1;
     if(minval <= maxval) {
       Pset1.MaxValSelect.LowCut = minval;
       Pset1.MaxValSelect.HighCut = maxval;
       status = 0;
     }
  }
// If error, erase the new value and display the default value
  if(status){
    if (status == 1) {
     wxMessageBox(wxT("Bad syntax/ LowCut > HighCut"),
                  wxT("MaxValSelection/Error"), wxOK | wxICON_ERROR);
    }
    Pset1.MaxValSelect.LowCut = 100.;
    Pset1.MaxValSelect.HighCut = 200.;
     buffer.Printf(wxT("%.2f,%.2f"), Pset1.MaxValSelect.LowCut,
                   Pset1.MaxValSelect.HighCut);
     ProcCtrl_MaxThresh->SetValue(buffer);
   }

return;
}
/********************************************************************
* ImageCenter LabeledEdit box
* Set new value for nz_cube, i.e., number of elementary images in data cube
*
********************************************************************/
void JLP_SpeckProcessPanel::On_nz_cube(wxCommandEvent& event)
{


if(initialized != 1234) return;

ChangesDone1 = true;
return;
}
/********************************************************************
* ImageCenter LabeledEdit box
* Set new value for nz_cube, i.e., number of elementary images in data cube
*
********************************************************************/
void JLP_SpeckProcessPanel::Update_nz_cube_FromScreen()
{
wxString buffer;
long nz;
int status = -1;

  buffer = ProcCtrl_nz_cube->GetValue();

// Get nz_cube:
  if(buffer.ToLong(&nz)) {
    status = 1;
    if(nz >= 10 && nz <= 1000) {
      Pset1.nz_cube = nz;
      status = 0;
    }
  }

// If error, erase the new value and display the old value
 if(status) {
    if(status == 1) {
    wxMessageBox(wxT("Bad syntax/bad value (should be in range [10,1000])"),
                 wxT("nz_cube/Error"), wxOK | wxICON_ERROR);
    }
    buffer.Printf(wxT("%d"), Pset1.nz_cube);
    ProcCtrl_nz_cube->SetValue(buffer);
   }

return;
}
//---------------------------------------------------------------------------
// "Direct Vector" Check Box
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnDirectVectorCheckBoxClick(wxCommandEvent& event)
{

if(initialized != 1234) return;

 Pset1.DirectVector = ProcCheck_DirectVector->IsChecked();
 ChangesDone1 = true;
}
//---------------------------------------------------------------------------
// "Lucky imaging" Check Box
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnLuckyCheckBoxClick(wxCommandEvent& event)
{

if(initialized != 1234) return;

 Pset1.LuckyImaging = ProcCheck_Lucky->IsChecked();
 ChangesDone1 = true;
}
//---------------------------------------------------------------------------
// Check box for statistics:
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnStatisticsCheckBoxClick(wxCommandEvent& event)
{

if(initialized != 1234) return;

  Pset1.Statistics_is_wanted = ProcCheck_Stats->IsChecked();
  if(Pset1.Statistics_is_wanted == false) {
     ProcCheck_SeeingSelect->SetValue(false);
     ProcCheck_MaxSelect->SetValue(false);
    }
  ProcCheck_SeeingSelect->Enable(ProcCheck_Stats->IsChecked());
  ProcCheck_MaxSelect->Enable(ProcCheck_Stats->IsChecked());
  ProcCtrl_MaxThresh->Enable(ProcCheck_MaxSelect->IsChecked());
  ProcCtrl_SeeingThresh->Enable(ProcCheck_SeeingSelect->IsChecked());

 ChangesDone1 = true;
}
//---------------------------------------------------------------------------
// Check box for seeing selection:
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnSeeingSelectionCheckBoxClick(
      wxCommandEvent& event)
{

if(initialized != 1234) return;

  Pset1.Selection_fwhm = ProcCheck_SeeingSelect->IsChecked();
  ProcCtrl_SeeingThresh->Enable(ProcCheck_SeeingSelect->IsChecked());

// Statistics are automatically selected when this option is "checked"
  if(ProcCheck_SeeingSelect->IsChecked())
       ProcCheck_Stats->SetValue(true);

 ChangesDone1 = true;
}
//---------------------------------------------------------------------------
// Check box for max value selection:
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnMaxValSelectionCheckBoxClick(
      wxCommandEvent& event)
{

if(initialized != 1234) return;

  Pset1.Selection_maxval = ProcCheck_MaxSelect->IsChecked();
  ProcCtrl_MaxThresh->Enable(ProcCheck_MaxSelect->IsChecked());

// Statistics are automatically selected when this option is "checked"
  if(ProcCheck_MaxSelect->IsChecked())
       ProcCheck_Stats->SetValue(true);

 ChangesDone1 = true;
ChangesDone1 = true;
return;
}
/*****************************************************************************
* Save surrent settings to Original_Pset1
* Called when pressing the "OK" button
* (also called when changing pages if not validated yet)
******************************************************************************/
void JLP_SpeckProcessPanel::ValidateChanges()
{
wxString buffer;

if(initialized != 1234) return;

  ValidatedChanges1 = true;

if(ChangesDone1) {
  UpdateBisp_nclos_FromScreen();
  UpdateBisp_ir_FromScreen();
  UpdateSlitAngleFromScreen();
  UpdateSpectrumAngleFromScreen();
  UpdateMaxValSelectionFromScreen();
  UpdateSeeingSelectionFromScreen();
  Update_nz_cube_FromScreen();
  UpdateOffsetThreshFromScreen();
  UpdateOut3DFitsDirFromScreen();
  UpdateOutResultsDirFromScreen();
  UpdatefftwDirFromScreen();
}

// Check that thresholds are correct when selection was selected:
  if(Pset1.Selection_fwhm) {
    if(Pset1.FwhmSelect.LowCut >= Pset1.FwhmSelect.HighCut) {
     buffer.Printf(wxT( "Low=%.3f >= High=%.3f : bad thresholds for FWHM selection!"),
             Pset1.FwhmSelect.LowCut, Pset1.FwhmSelect.HighCut);
     wxMessageBox(buffer, wxT("JLP_SpeckProcessPanel/Error"),
                  wxOK | wxICON_ERROR);
     return;
    }
  }
  if(Pset1.Selection_maxval) {
    if(Pset1.MaxValSelect.LowCut >= Pset1.MaxValSelect.HighCut) {
     buffer.Printf(wxT("Low=%.3f >= High=%.3f : bad thresholds for MaxVal selection!"),
             Pset1.MaxValSelect.LowCut, Pset1.MaxValSelect.HighCut);
     wxMessageBox(buffer, wxT("JLP_SpeckProcessPanel/Error"),
                  wxOK | wxICON_ERROR);
     return;
    }
  }

// Copy current settings to Original_Pset1 (that will be
// used by the calling program)
  Copy_PSET(Pset1, Original_Pset1);

return;
}
/*****************************************************************************
* Bool parameter used to know whether frame offset correction
* or threshold-only correction is to be made
*
* Offset correction
* 0 : Threshold only
* 1 : Threshold and dark frame
******************************************************************************/
void JLP_SpeckProcessPanel::OnSelectOffsetCorr(wxCommandEvent& event)
{

if(initialized != 1234) return;

  switch(event.GetId()) {
    case ID_PROC_OFFSET_THRESH:
      Pset1.OffsetFrameCorrection = false;
      break;
    case ID_PROC_OFFSET_FRAME:
      Pset1.OffsetFrameCorrection = true;
// Load Offset frame if not yet done:
      if(Pset1.OffsetFileName[0] == '\0') OnLoadOffsetFrame(event);
      break;
    }

ChangesDone1 = true;
return;
}
/****************************************************************************
* Flat Field correction
* Bool parameter used to know whether flat field correction
* is to be made
*
****************************************************************************/
void JLP_SpeckProcessPanel::OnSelectFFieldCorr(wxCommandEvent& event)
{

if(initialized != 1234) return;

  switch(event.GetId()) {
    case ID_PROC_NO_FFCORR:
      Pset1.FlatFieldCorrection = false;
      break;
    case ID_PROC_FFCORR:
      Pset1.FlatFieldCorrection = true;
// Load Flat field frame if not yet done:
      if(Pset1.FlatFieldFileName[0] == '\0') OnLoadFFieldFrame(event);
      break;
    }

ChangesDone1 = true;
return;
}
/****************************************************************************
* Offset threshold value
****************************************************************************/
void JLP_SpeckProcessPanel::OnOffsetThreshEditExit(wxCommandEvent& event)
{

if(initialized != 1234) return;

ChangesDone1 = true;
return;
}
/****************************************************************************
* Offset threshold value
****************************************************************************/
void JLP_SpeckProcessPanel::UpdateOffsetThreshFromScreen()
{
wxString buffer;
long offset;
int status = -5;

if(initialized != 1234) return;

  buffer = ProcCtrl_OffsetThresh->GetValue();

// Get OffsetThreshold:
  if(buffer.ToLong(&offset)) {
    status = 1;
    if(offset >= 0 && offset <= 100000) {
       Pset1.OffsetThreshold = offset;
       status = 0;
    }
  }

// If error, erase the new value and display the default value
  if(status) {
    if(status == 1) {
     buffer.Append(wxT("\n Bad syntax/bad value (should be in range [0,100000])"));
     wxMessageBox(buffer, wxT("OffsetThreshEditExit/Error"),
                  wxOK | wxICON_ERROR);
    }
    Pset1.OffsetThreshold = 0.;
    buffer.Printf(wxT("%d"), Pset1.OffsetThreshold);
    ProcCtrl_OffsetThresh->SetValue(buffer);
   }

return;
}
/****************************************************************************
* Load Offset frame
****************************************************************************/
void JLP_SpeckProcessPanel::OnLoadOffsetFrame(wxCommandEvent& event)
{
wxString filename1, err_msg, buffer;
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80], filename[128];

if(initialized != 1234) return;

strcpy(Pset1.OffsetFileName, "");

// Open Dialog Box for selecting Offset frame:
// Prompt for filename
  filename1.Empty();
  filename1 = wxFileSelector(_T("Loading offset file"), _T(""),
                            _T(""), _T(""),
       _T("FITS files (*.fits;*.fit;*.FITS;*.FIT)|*.fits;*.FITS;*.fit;*.FIT"));

// If "cancel", exit from here without loading files
  if (filename1.IsEmpty() ) return;

  strcpy(filename, filename1.c_str());

// Open FITS file to see if it is OK:
      status = readimag_float_fits(filename, &image0, &nx0, &ny0, comments,
                                   errmess);
      if(status){
        wxMessageBox(wxString(errmess), wxT("LoadOffsetFrame/Error"),
                     wxOK | wxICON_ERROR);
        return;
        }

// If OK, save filename:
    strcpy(Pset1.OffsetFileName, filename);
// Display it:
    buffer.Printf(wxT("%s"), Pset1.OffsetFileName);
    ProcStatic_OffsetFile->SetLabel(buffer);
// Select offset frame correction
    Pset1.OffsetFrameCorrection = true;
    ProcRad_OffsetThresholdAndFrame->SetValue(true);

// Free memory (this file will be re-opened later by JLP_Decode...)
    delete[] image0;

 ChangesDone1 = true;
 return;
}
//---------------------------------------------------------------------------
// Load unresolved autoc frame
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnLoadUnresolvedAutocFrame(wxCommandEvent& event)
{
wxString filename1, err_msg, buffer;
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80], filename[128];

if(initialized != 1234) return;

strcpy(Pset1.UnresolvedAutocName, "");

// Open Dialog Box for selecting frame:
// Prompt for filename
  filename1.Empty();
  filename1 = wxFileSelector(_T("Loading unresolvd autoc file"), _T(""),
                            _T(""), _T(""),
       _T("FITS files (*.fits;*.fit;*.FITS;*.FIT)|*.fits;*.FITS;*.fit;*.FIT"));

// If "cancel", exit from here without loading files
  if (filename1.IsEmpty() ) return;

  strcpy(filename, filename1.c_str());

// Open FITS file and check if it is OK:
      status = readimag_float_fits(filename, &image0, &nx0, &ny0, comments,
                                   errmess);
      if(status){
        wxMessageBox(wxString(errmess), wxT("LoadUnresolvedAutocFrame/Error"),
                     wxOK | wxICON_ERROR);
        return;
        }
// If it is OK, save filename:
    strcpy(Pset1.UnresolvedAutocName, filename);
// Display it:
    buffer.Printf(wxT("%s"), Pset1.UnresolvedAutocName);
    ProcStatic_UnresAutocFile->SetLabel(buffer);

// Free memory (this file will be re-opened later by JLP_Decode...)
    delete[] image0;

 ChangesDone1 = true;
 return;
}
//---------------------------------------------------------------------------
// Load unresolved modsq frame
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnLoadUnresolvedModsqFrame(wxCommandEvent& event)
{
wxString filename1, err_msg, buffer;
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80], filename[128];

if(initialized != 1234) return;

strcpy(Pset1.UnresolvedModsqName, "");

// Open Dialog Box for selecting unresolved modsq frame:
// Prompt for filename
  filename1.Empty();
  filename1 = wxFileSelector(_T("Loading unresolved modsq frame"), _T(""),
                            _T(""), _T(""),
       _T("FITS files (*.fits;*.fit;*.FITS;*.FIT)|*.fits;*.FITS;*.fit;*.FIT"));

// If "cancel", exit from here without loading files
  if (filename1.IsEmpty() ) return;

  strcpy(filename, filename1.c_str());

// Open FITS file and check if it is OK:
      status = readimag_float_fits(filename, &image0, &nx0, &ny0, comments,
                                   errmess);
      if(status){
        wxMessageBox(wxString(errmess), wxT("LoadUnresolvedModsqFrame/Error"),
                   wxOK | wxICON_ERROR);
        return;
        }
// If it is OK, save filename:
  strcpy(Pset1.UnresolvedModsqName, filename);
// Display it:
  buffer.Printf(wxT("%s"), Pset1.UnresolvedModsqName);
  ProcStatic_UnresModsqFile->SetLabel(buffer);

// Free memory (this file will be re-opened later by JLP_Decode...)
  delete[] image0;

 ChangesDone1 = true;
 return;
}
//---------------------------------------------------------------------------
// Load Flat Field frame
// The flat field frame will be used for gain calibration (from september 2007)
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnLoadFFieldFrame(wxCommandEvent& event)
{
wxString filename1, err_msg, buffer;
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80], filename[128];

if(initialized != 1234) return;

strcpy(Pset1.FlatFieldFileName, "");

// Open Dialog Box for selecting Flat field frame:
// Prompt for filename
  filename1.Empty();
  filename1 = wxFileSelector(_T("Loading flat field file"), _T(""),
                            _T(""), _T(""),
       _T("FITS files (*.fits;*.fit;*.FITS;*.FIT)|*.fits;*.FITS;*.fit;*.FIT"));

// If "cancel", exit from here without loading files
  if (filename1.IsEmpty() ) return;

  strcpy(filename, filename1.c_str());

// Open FITS file and check if it is OK:
      status = readimag_float_fits(filename, &image0, &nx0, &ny0, comments,
                                   errmess);
      if(status){
        wxMessageBox(wxString(errmess), wxT("LoadFFieldFrame/Error"),
                     wxOK | wxICON_ERROR);
        return;
        }
/* If it is OK:
       FlatFieldSize = nx * ny;
       FlatFieldFrame = new double[FlatFieldSize];
       for(i = 0; i < FlatFieldSize; i++) FlatFieldFrame[i] = image0[i];
       delete[] image0;
// Clip Flat Field to reduce the range and subsequent errors:
       ClipFlatField();
*/
// If it is OK, save filename:
    strcpy(Pset1.FlatFieldFileName, filename);
// Display its name:
    buffer.Printf(wxT("%s"), Pset1.FlatFieldFileName);
    ProcStatic_FFieldFile->SetLabel(buffer);
// Select flatfield correction
    Pset1.FlatFieldCorrection = true;
    ProcRad_FFieldCorr->SetValue(true);
// Free memory (this file will be re-opened later by JLP_Decode...)
    delete[] image0;


 ChangesDone1 = true;
 return;
}
/****************************************************************************
* Load PhotModsq frame
****************************************************************************/
void JLP_SpeckProcessPanel::OnLoadPhotModsqFrame(wxCommandEvent& event)
{
if(initialized != 1234) return;

LoadPhotModsqFilenameToPset1();
ChangesDone1 = true;

return;
}
/****************************************************************************
* Get the PhotModsq filename and load it to Pset1
****************************************************************************/
int JLP_SpeckProcessPanel::LoadPhotModsqFilenameToPset1()
{
wxString filename1, err_msg, buffer;
float *image0;
int nx0, ny0, status;
char errmess[256], comments[80], filename[128];

strcpy(Pset1.PhotModsqFileName, "");

// Open Dialog Box for selecting frame:
// Prompt for filename
 filename1.Empty();
 filename1 = wxFileSelector(_T("Loading power spectrum of photon reponse (PhotModsq)"),
                            _T(""), _T(""), _T(""),
       _T("FITS files (*.fits;*.fit;*.FITS;*.FIT)|*.fits;*.FITS;*.fit;*.FIT"));

// If "cancel", exit from here without loading files
 if (filename1.IsEmpty()) return(-1);

 strcpy(filename, filename1.c_str());

// Open FITS file and check if it is OK:
 status = readimag_float_fits(filename, &image0, &nx0, &ny0, comments,
                              errmess);
  if(status){
    wxMessageBox(wxString(errmess), wxT("LoadPhotModsqFilenameToPset1/Error"),
                          wxOK | wxICON_ERROR);
    return(-1);
    }

// If it is OK, save filename:
 strcpy(Pset1.PhotModsqFileName, filename);
// Display it:
 buffer.Printf(wxT("%s"), Pset1.PhotModsqFileName);
 ProcStatic_PhotModsqFile->SetLabel(buffer);

// Free memory (this file will be re-opened later by JLP_Decode...)
 delete[] image0;

return(0);
}
/****************************************************************************
* Select FFTW directory  (for FFTW Fourier Transform, or other temporary files)
*****************************************************************************/
void JLP_SpeckProcessPanel::OnfftwDirEditExit(wxCommandEvent& event)
{
 if(initialized != 1234) return;
 ChangesDone1 = true;
return;
}
/****************************************************************************
* Select FFTW directory  (for FFTW Fourier Transform, or other temporary files)
*****************************************************************************/
void JLP_SpeckProcessPanel::UpdatefftwDirFromScreen()
{
wxString directory, buffer;

  directory = ProcCtrl_FFTW_dir->GetValue();
  if(directory.IsEmpty()) directory = wxT(".");

// Check that directory exists:
  if(wxDirExists(directory)) {
       strcpy(Pset1.FFTW_directory, directory.c_str());
// If error, erase the new value and display the old value
   } else {
     wxMessageBox(wxT("Wrong directory: does not exist!"),
                  wxT("fftwDirExit/Error"), wxOK | wxICON_ERROR);
     buffer = wxString(".");
     ProcCtrl_FFTW_dir->SetValue(buffer);
     }

return;
}
/****************************************************************************
* Select 3DFits directory (for saving cubes of elementary frames)
*****************************************************************************/
void JLP_SpeckProcessPanel::OnOut3DFitsDirEditExit(wxCommandEvent& event)
{
 if(initialized != 1234) return;
 ChangesDone1 = true;
return;
}
/****************************************************************************
* Select 3DFits directory (for saving cubes of elementary frames)
*****************************************************************************/
void JLP_SpeckProcessPanel::UpdateOut3DFitsDirFromScreen()
{
wxString directory, buffer;

  directory = ProcCtrl_3DFits_dir->GetValue();
  if(directory.IsEmpty()) directory = wxT(".");

// Check that directory exists:
  if(wxDirExists(directory)) {
       strcpy(Pset1.out3DFITS_directory, directory.c_str());
// If error, erase the new value and display the old value
     } else {
     wxMessageBox(wxT("Wrong directory: does not exist!"),
                  wxT("out3DFitsDirExit/Error"), wxOK | wxICON_ERROR);
     buffer = wxString(".");
     ProcCtrl_3DFits_dir->SetValue(buffer);
     }

return;
}
/****************************************************************************
* Select directory for saving results
*****************************************************************************/
void JLP_SpeckProcessPanel::OnOutResultsDirEditExit(wxCommandEvent& event)
{
 if(initialized != 1234) return;
 ChangesDone1 = true;
return;
}
/****************************************************************************
* Select directory for saving results
*****************************************************************************/
void JLP_SpeckProcessPanel::UpdateOutResultsDirFromScreen()
{
wxString directory, buffer;

  directory = ProcCtrl_Results_dir->GetValue();
  if(directory.IsEmpty()) directory = wxT(".");

// Check that directory exists:
  if(wxDirExists(directory)) {
       strcpy(Pset1.outResults_directory, directory.c_str());
// If error, erase the new value and display the old value
     } else {
     wxMessageBox(wxT("Wrong directory: does not exist!"),
                  wxT("out3DFitsDirExit/Error"), wxOK | wxICON_ERROR);
     buffer = wxString(".");
     ProcCtrl_Results_dir->SetValue(buffer);
     }

return;
}
//---------------------------------------------------------------------------
// Save process setting configuration to new config. file
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnSaveConfigButtonClick(wxCommandEvent& event)
{

 if(initialized != 1234) return;

 SaveProcessSettingsDialog();

}
//---------------------------------------------------------------------------
// Load process setting configuration from previously saved config. file
//---------------------------------------------------------------------------
void JLP_SpeckProcessPanel::OnLoadConfigButtonClick(wxCommandEvent& event)
{

 if(initialized != 1234) return;

 LoadProcessSettingsDialog();

 ChangesDone1 = true;
}
/****************************************************************************
* Frames to be saved to 3DFits file
* 0=nothing 1=All 2=Selection
****************************************************************************/
void JLP_SpeckProcessPanel::OnSelectSave3DFits(wxCommandEvent& event)
{
int index;

if(initialized != 1234) return;


 index  = ProcCmb_Save_3DFits.combo->GetSelection();

switch(index) {
  case 0:
    Pset1.SaveFramesToFitsFile = 0;
    Pset1.SaveSelectionOnlyToFitsFile = false;
    break;
  case 1:
    Pset1.SaveFramesToFitsFile = 3;
    Pset1.SaveSelectionOnlyToFitsFile = false;
    break;
  case 2:
    Pset1.SaveFramesToFitsFile = 3;
    Pset1.SaveSelectionOnlyToFitsFile = true;
    break;
 }

 ChangesDone1 = true;
 return;
}

/****************************************************************************
* 7 positions, with values from 4 to 10
* 0:4 1:5 2:6 3:7 4:8 5:9 6:10
****************************************************************************/
void JLP_SpeckProcessPanel::OnSelect_KK_CrossCorr(wxCommandEvent& event)
{

 if(initialized != 1234) return;

 Pset1.KK_CrossCorr = ProcCmb_KK_CrossCorr.combo->GetSelection() + 4;

 ChangesDone1 = true;
}
/**************************************************************************
* Handle "Config" buttons:
*
* (also called by OnPageChanging when leaving this panel)
*
**************************************************************************/
void JLP_SpeckProcessPanel::CancelNonValidatedChanges()
{
// Go back to original settings:
  Copy_PSET(Original_Pset1, Pset1);
// Display new values:
  DisplayNewValues();

// Reset following values
// (needed when this routines is called by OnPageChanging
// when leaving this panel)
  ChangesDone1 = false;
  ValidatedChanges1 = false;
}
/**************************************************************************
* Handle "Config" buttons:
**************************************************************************/
void JLP_SpeckProcessPanel::OnConfig(wxCommandEvent& event)
{

if(initialized != 1234) return;

switch(event.GetId()) {

  case ID_PROC_CONFIG_OK:
    ValidateChanges();
    break;

// Cancel button: go back to initial settings
  case ID_PROC_CONFIG_CANCEL:
    CancelNonValidatedChanges();
    ChangesDone1 = false;
    break;

// Validate and save current config to file:
  case ID_PROC_CONFIG_SAVE:
    ValidateChanges();
    SaveProcessSettingsDialog();
    break;

// Load config from file:
  case ID_PROC_CONFIG_LOAD:
    LoadProcessSettingsDialog();

// Display those new values:
    DisplayNewValues();
    ChangesDone1 = true;

    break;

// Load default config
  case ID_PROC_CONFIG_DEFAULT:

// Initialize all settings to default values:
    Init_PSET(Pset1);

// Display new values:
    DisplayNewValues();
    ValidateChanges();
    ChangesDone1 = true;

    break;
}

return;
}
