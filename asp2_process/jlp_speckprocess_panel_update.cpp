/****************************************************************************
* Name: jlp_speckprocess_panel_update.cpp
*
* JLP
* Version 26/08/2015
****************************************************************************/
#include "jlp_speckprocess_panel.h"

//---------------------------------------------------------------------------
// Displaying the new parameters read from Pset1
//---------------------------------------------------------------------------
int JLP_SpeckProcessPanel::DisplayNewValues()
{
wxString buffer;
wxFont font1;

/*---------------------------------------------------------------------------
* ProcessingMode = 0 : no processing (only numerisation)
* ProcessingMode = 1 : Only integration
* ProcessingMode = 2 : autoc - inter, FFT modulus, long integration
* ProcessingMode = 3 : autoc - inter, FFT modulus, long integration, quadrant
* ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
* ProcessingMode = 5 : Bispectrum, autoc, quadrant
* ProcessingMode = 6 : Spectroscopy
* ProcessingMode = 7 : SCIDAR Log
* ProcessingMode = 8 : SCIDAR Lin
* ProcessingMode = 9 : Statistics
*/
//---------------------------------------------------------------------------
     ProcCmb_Mode.combo->SetSelection(Pset1.ProcessingMode);

// nz_cube
     buffer.Printf(_T("%d"), Pset1.nz_cube);
     ProcCtrl_nz_cube->SetValue(buffer);

// Save elementary frames to 2D/3D Fits file
// 0 = none, 1 = all, 2=selection_only
     if(Pset1.SaveFramesToFitsFile == 0) {
       ProcCmb_Save_3DFits.combo->SetSelection(0);
     } else if(Pset1.SaveSelectionOnlyToFitsFile){
       ProcCmb_Save_3DFits.combo->SetSelection(2);
     } else {
       ProcCmb_Save_3DFits.combo->SetSelection(1);
     }

// GroupBox
// 0:4 1:5 2:6 3:7 4:8 5:9 6:10
  if(Pset1.KK_CrossCorr <= 4) Pset1.KK_CrossCorr = 4;
  ProcCmb_KK_CrossCorr.combo->SetSelection(Pset1.KK_CrossCorr - 4);

// ir_bisp
     buffer.Printf(wxT("%d"), Pset1.ir_bisp);
     ProcCtrl_Bisp_ir->SetValue(buffer);
     if(Pset1.ProcessingMode == 4 || Pset1.ProcessingMode == 5)
        ProcCtrl_Bisp_ir->Enable(true);
     else
        ProcCtrl_Bisp_ir->Enable(false);

// nmax_bisp (max. number of closure relations)
     buffer.Printf(wxT("%d"), Pset1.nmax_bisp);
     if(Pset1.ProcessingMode == 4 || Pset1.ProcessingMode == 5)
        ProcCtrl_Bisp_nclos->Enable(true);
     else
        ProcCtrl_Bisp_nclos->Enable(false);

// Slit angle:
     buffer.Printf(wxT("%.2f"), Pset1.slangle);
     ProcCtrl_SlitAngle->SetValue(buffer);
     if(Pset1.ProcessingMode == 6)
       ProcCtrl_SlitAngle->Enable(true);
     else
       ProcCtrl_SlitAngle->Enable(false);

// Spectrum angle:
     buffer.Printf(wxT("%.2f"), Pset1.spangle);
     ProcCtrl_SpectAngle->SetValue(buffer);
     if(Pset1.ProcessingMode == 6)
       ProcCtrl_SpectAngle->Enable(true);
     else
       ProcCtrl_SpectAngle->Enable(false);

// DirectVectorCheckBox:
     ProcCheck_DirectVector->SetValue(Pset1.DirectVector);

// LuckyCheckBox:
     ProcCheck_Lucky->SetValue(Pset1.LuckyImaging);

// 1=long int, 2=autoc, 3=autoc+quadrant
     if(Pset1.ProcessingMode >= 1 && Pset1.ProcessingMode <= 3) {
       ProcCheck_Lucky->Enable(true);
       ProcCheck_DirectVector->Enable(true);
     } else {
       ProcCheck_Lucky->Enable(false);
       ProcCheck_DirectVector->Enable(false);
     }

// CheckBoxStatistics:
// Statistics and no selection:
     ProcCheck_Stats->SetValue(Pset1.Statistics_is_wanted);

     ProcCheck_SeeingSelect->Enable(ProcCheck_Stats->IsChecked());
     ProcCheck_SeeingSelect->SetValue(Pset1.Selection_fwhm);
     ProcCtrl_SeeingThresh->Enable(Pset1.Selection_fwhm);

     ProcCheck_MaxSelect->Enable(ProcCheck_Stats->IsChecked());
     ProcCheck_MaxSelect->SetValue(Pset1.Selection_maxval);
     ProcCtrl_MaxThresh->Enable(Pset1.Selection_maxval);

// Seeing selection LowCut/HighCut
     buffer.Printf(wxT("%.2f,%.2f"), Pset1.FwhmSelect.LowCut,
                   Pset1.FwhmSelect.HighCut);
     ProcCtrl_SeeingThresh->SetValue(buffer);

// Max value selection LowCut/HighCut
     buffer.Printf(wxT("%.2f,%.2f"), Pset1.MaxValSelect.LowCut,
                   Pset1.MaxValSelect.HighCut);
     ProcCtrl_MaxThresh->SetValue(buffer);

// Flat Field correction
     if(Pset1.FlatFieldCorrection)
       ProcRad_FFieldCorr->SetValue(true);
     else
       ProcRad_NoFFieldCorr->SetValue(true);

// Offset correction
     if(Pset1.OffsetFrameCorrection)
       ProcRad_OffsetThresholdAndFrame->SetValue(true);
     else
       ProcRad_OffsetThreshold->SetValue(true);

// Offset Threshold:
     buffer.Printf(wxT("%d"), Pset1.OffsetThreshold);
     ProcCtrl_OffsetThresh->SetValue(buffer);

// Directories:
     ProcCtrl_FFTW_dir->SetValue(Pset1.FFTW_directory);
     ProcCtrl_3DFits_dir->SetValue(Pset1.out3DFITS_directory);
     ProcCtrl_Results_dir->SetValue(Pset1.outResults_directory);

// Change fonts for labels:
     font1 = ProcStatic_OffsetFile->GetFont();
     font1.SetPointSize(8);
     font1.SetWeight(wxFONTWEIGHT_BOLD);
     ProcStatic_OffsetFile->SetFont(font1);
     ProcStatic_FFieldFile->SetFont(font1);
     ProcStatic_PhotModsqFile->SetFont(font1);
     ProcStatic_UnresAutocFile->SetFont(font1);
     ProcStatic_UnresModsqFile->SetFont(font1);

// Filenames:
     buffer.Printf(wxT("%s"), Pset1.OffsetFileName);
     ProcStatic_OffsetFile->SetLabel(buffer);
     buffer.Printf(wxT("%s"), Pset1.FlatFieldFileName);
     ProcStatic_FFieldFile->SetLabel(buffer);
     buffer.Printf(wxT("%s"), Pset1.PhotModsqFileName);
     ProcStatic_PhotModsqFile->SetLabel(buffer);
     buffer.Printf(wxT("%s"), Pset1.UnresolvedAutocName);
     ProcStatic_UnresAutocFile->SetLabel(buffer);
     buffer.Printf(wxT("%s"), Pset1.UnresolvedModsqName);
     ProcStatic_UnresModsqFile->SetLabel(buffer);

// Refresh the screen:
  Refresh();

return 0;
}
/***************************************************************************
*
* Save process setting configuration to new config. file
*
***************************************************************************/
int JLP_SpeckProcessPanel::SaveProcessSettingsDialog()
{
wxString save_filename;
char filename[128];
int status;

// Prompt for filename
wxFileDialog saveFileDialog(this, wxT("Save processing settings to file"),
                            wxT(""), wxT(""),
                            wxT("Config files (*.cfg;*.CFG)|*.cfg;*.CFG"),
                            wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

// Stops the timers:
// TOBEDONE
// StopDisplayTimers();
 status = saveFileDialog.ShowModal();
// TOBEDONE
// RestartDisplayTimers();

// If "cancel", exit from here without saving files
 if (status == wxID_CANCEL) return(-1);

  save_filename = saveFileDialog.GetFilename();
  strcpy(filename, save_filename.c_str());

// Save configuration file:
 SaveProcessSettingsToFile(filename, Pset1);

return(0);
}
/***************************************************************************
*
* Load process setting configuration from previously saved config. file
*
***************************************************************************/
int JLP_SpeckProcessPanel::LoadProcessSettingsDialog()
{
wxString filename1;
char filename[128];

// Prompt for filename
  filename1.Empty();
  filename1 = wxFileSelector(_T("Select file with processing settings"), _T(""),
                            _T(""), _T(""),
                            _T("Config files (*.cfg;*.CFG)|*.cfg;*.CFG"));

// If "cancel", exit from here without loading files
  if (filename1.IsEmpty() ) return(-1);

  strcpy(filename, filename1.c_str());

// Load configuration file:
  LoadProcessSettingsFromFile(filename, &Pset1);

return(0);
}
//---------------------------------------------------------------------------
// Save Process Settings to Config File
//---------------------------------------------------------------------------
int JLP_SpeckProcessPanel::SaveProcessSettingsToFile(char *filename,
                                                PROCESS_SETTINGS Pset1)
{
FILE *fp_out;
wxString buffer;

if((fp_out = fopen(filename,"w"))== NULL) {
  buffer.Printf(wxT("Error opening output configuration file >%s< \n"), filename);
  wxMessageBox(buffer, wxT("SaveProcessSettingsToFile/Error"),
               wxOK | wxICON_ERROR);
  return(-1);
  }

// Version 2: March 2011
fprintf(fp_out, "ProcessSettingsV2 Configuration File\n");
fprintf(fp_out, "FwhmSelect.LowCut = %.3f\n", Pset1.FwhmSelect.LowCut);
fprintf(fp_out, "FwhmSelect.HighCut = %.3f\n", Pset1.FwhmSelect.HighCut);
fprintf(fp_out, "MaxValSelect.LowCut = %.3f\n", Pset1.MaxValSelect.LowCut);
fprintf(fp_out, "MaxValSelect.HighCut = %.3f\n", Pset1.MaxValSelect.HighCut);
fprintf(fp_out, "slangle = %.3f\n", Pset1.slangle);
fprintf(fp_out, "spangle = %.3f\n", Pset1.spangle);
fprintf(fp_out, "ProcessingMode = %d\n", Pset1.ProcessingMode);
fprintf(fp_out, "Selection_fwhm = %d\n", Pset1.Selection_fwhm);
fprintf(fp_out, "Selection_maxval = %d\n", Pset1.Selection_maxval);
fprintf(fp_out, "Statistics_is_wanted = %d\n", Pset1.Statistics_is_wanted);
fprintf(fp_out, "SubtractUnresolvedAutoc = %d\n", Pset1.SubtractUnresolvedAutoc);
fprintf(fp_out, "OffsetThreshold = %d\n", Pset1.OffsetThreshold);
fprintf(fp_out, "OffsetFrameCorrection = %d\n", (int)Pset1.OffsetFrameCorrection);
fprintf(fp_out, "FlatFieldCorrection = %d\n", (int)Pset1.FlatFieldCorrection);
fprintf(fp_out, "nz_cube = %d\n", Pset1.nz_cube);
fprintf(fp_out, "nx_num = %d\n", Pset1.nx_num);
fprintf(fp_out, "ny_num = %d\n", Pset1.ny_num);
fprintf(fp_out, "KK_CrossCorr = %d\n", Pset1.KK_CrossCorr);
fprintf(fp_out, "ir_bisp = %d\n", Pset1.ir_bisp);
fprintf(fp_out, "nmax_bisp = %d\n", Pset1.nmax_bisp);
fprintf(fp_out, "DirectVector = %d\n", (int)Pset1.DirectVector);
fprintf(fp_out, "LuckyImaging = %d\n", (int)Pset1.LuckyImaging);
fprintf(fp_out, "SaveFramesToFitsFile = %d\n", (int)Pset1.SaveFramesToFitsFile);
fprintf(fp_out, "SaveSelectionOnlyToFitsFile = %d\n", (int)Pset1.SaveSelectionOnlyToFitsFile);
fprintf(fp_out, "SlowProcessing = %d\n", (int)Pset1.SlowProcessing);
fprintf(fp_out, "OffsetFileName =%s\n", Pset1.OffsetFileName);
fprintf(fp_out, "UnresolvedAutocName =%s\n", Pset1.UnresolvedAutocName);
fprintf(fp_out, "UnresolvedModsqName =%s\n", Pset1.UnresolvedModsqName);
fprintf(fp_out, "FlatFieldFileName =%s\n", Pset1.FlatFieldFileName);
fprintf(fp_out, "PhotModsqFileName =%s\n", Pset1.PhotModsqFileName);
fprintf(fp_out, "FFTW_directory =%s\n", Pset1.FFTW_directory);
fprintf(fp_out, "out3DFITS_directory =%s\n", Pset1.out3DFITS_directory);
fprintf(fp_out, "outResults_directory =%s\n", Pset1.outResults_directory);

fclose(fp_out);

return(0);
}
//---------------------------------------------------------------------------
// Load Process Settings from File
//---------------------------------------------------------------------------
int JLP_SpeckProcessPanel::LoadProcessSettingsFromFile(char *filename,
                                                  PROCESS_SETTINGS *Pset1)
{
FILE *fp_in;
wxString err_msg;
char buffer[128];

if((fp_in = fopen(filename,"r"))== NULL) {
  err_msg.Printf(wxT("Error opening input configuration file >%s< \n"),
                filename);
  wxMessageBox(err_msg, wxT("LoadProcessSettingsFromFile/Error"),
               wxOK | wxICON_ERROR);
  return(-1);
  }
// Read the first line to see if it is a good file:
fgets(buffer, 128, fp_in);
// Version 2: March 2011
if(strncmp(buffer, "ProcessSettingsV2", 17)) {
  wxMessageBox(wxT("Invalid format!"), wxT("LoadProcessSettingsFromFile/Error"),
               wxOK | wxICON_ERROR);
  fclose(fp_in);
  return(-2);
  }
fclose(fp_in);

//ReadStringFromConfigFile(filename, "HeadModel", ASet1->HeadModel);
//ReadIntFromConfigFile(filename, "gblXPixels", &ASet1->gblXPixels);
ReadDoubleFromConfigFile(filename, "FwhmSelect.LowCut", &Pset1->FwhmSelect.LowCut);
ReadDoubleFromConfigFile(filename, "FwhmSelect.HighCut", &Pset1->FwhmSelect.HighCut);
ReadDoubleFromConfigFile(filename, "MaxValSelect.LowCut", &Pset1->MaxValSelect.LowCut);
ReadDoubleFromConfigFile(filename, "MaxValSelect.HighCut", &Pset1->MaxValSelect.HighCut);
ReadDoubleFromConfigFile(filename, "slangle", &Pset1->slangle);
ReadDoubleFromConfigFile(filename, "spangle", &Pset1->spangle);
ReadIntFromConfigFile(filename, "ProcessingMode", &Pset1->ProcessingMode);
ReadBoolFromConfigFile(filename, "Selection_fwhm", &Pset1->Selection_fwhm);
ReadBoolFromConfigFile(filename, "Selection_maxval", &Pset1->Selection_maxval);
ReadBoolFromConfigFile(filename, "Statistics_is_wanted", &Pset1->Statistics_is_wanted);
ReadBoolFromConfigFile(filename, "SubtractUnresolvedAutoc", &Pset1->SubtractUnresolvedAutoc);
ReadIntFromConfigFile(filename, "OffsetThreshold", &Pset1->OffsetThreshold);
ReadBoolFromConfigFile(filename, "OffsetFrameCorrection", &Pset1->OffsetFrameCorrection);
ReadBoolFromConfigFile(filename, "FlatFieldCorrection", &Pset1->FlatFieldCorrection);
ReadIntFromConfigFile(filename, "nz_cube", &Pset1->nz_cube);
ReadIntFromConfigFile(filename, "nx_num", &Pset1->nx_num);
ReadIntFromConfigFile(filename, "ny_num", &Pset1->ny_num);
ReadIntFromConfigFile(filename, "KK_CrossCorr", &Pset1->KK_CrossCorr);
ReadIntFromConfigFile(filename, "ir_bisp", &Pset1->ir_bisp);
ReadIntFromConfigFile(filename, "nmax_bisp", &Pset1->nmax_bisp);
ReadIntFromConfigFile(filename, "SaveFramesToFitsFile", &Pset1->SaveFramesToFitsFile);
ReadBoolFromConfigFile(filename, "DirectVector", &Pset1->DirectVector);
ReadBoolFromConfigFile(filename, "LuckyImaging", &Pset1->LuckyImaging);
ReadBoolFromConfigFile(filename, "SaveSelectionOnlyToFitsFile", &Pset1->SaveSelectionOnlyToFitsFile);
ReadBoolFromConfigFile(filename, "SlowProcessing",  &Pset1->SlowProcessing);
ReadStringFromConfigFile(filename, "OffsetFileName", Pset1->OffsetFileName);
ReadStringFromConfigFile(filename, "UnresolvedAutocName", Pset1->UnresolvedAutocName);
ReadStringFromConfigFile(filename, "UnresolvedModsqName", Pset1->UnresolvedModsqName);
ReadStringFromConfigFile(filename, "FlatFieldFileName", Pset1->FlatFieldFileName);
ReadStringFromConfigFile(filename, "PhotModsqFileName", Pset1->PhotModsqFileName);
ReadStringFromConfigFile(filename, "FFTW_directory", Pset1->FFTW_directory);
ReadStringFromConfigFile(filename, "out3DFITS_directory", Pset1->out3DFITS_directory);
ReadStringFromConfigFile(filename, "outResults_directory", Pset1->outResults_directory);

// Display new values:
DisplayNewValues();

return(0);
}
