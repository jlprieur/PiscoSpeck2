/******************************************************************************
* Name: "asp2_menu_rwfits.cpp"  (AspFrame class)
* Handling input/output menu commands for FITS files
*
* Author:      JLP
* Version:     07/01/2015
******************************************************************************/
#include "asp2_frame.h"
#include "asp2_frame_id.h"
#include "asp_display_panel.h"
#include "jlp_camera1.h"

/* Prototypes of routines contained here and defined in "gdp_frame.h":
    void OnSaveFramesTo3DFits(wxCommandEvent& event);
    void OnLoadProcessingResultsForDisplay(wxCommandEvent& event);
    void OnSaveProcessingResults(wxCommandEvent& event);
    int SaveProcessingResults();
    int SaveFramesTo3DFits();
*/

/****************************************************************************
* Load processing results to DecodeThread
* Old results without bispectrum : LongInt, Autoc, Modsq
*
****************************************************************************/
void AspFrame::OnLoadProcessingResults(wxCommandEvent &event )
{
wxString filename, str1, path, extension;
wxString full_filename;
int i, imax, last_index, status = 0;
bool bispectrum_was_computed;
char fname0[128], comments0[80], err_messg0[128], generic_name0[128];
double *dble_image0;
int nx0, ny0, nz0;
DESCRIPTORS descrip0;
descrip0.ndescr = 0;
JLP_CAMERA_SETTINGS CamSet0;

if(event.GetId() == ID_LOAD_BISPECTRUM) bispectrum_was_computed = true;
else bispectrum_was_computed = false;

// Ask for generic name:

// When empty this string is used by StartProcessing to detect a failure in file selection:
  m_filename1 = wxT("");

// Prompt the user for the FITS filename, if not set by the calling routine:
  filename.Empty();
  filename = wxFileSelector(_T("Select one input FITS file of the results to be loaded"), _T(""), _T(""),
                      _T("fits|fit|FIT"),
                      _T("FITS files (*.fits;*.fit)|*.fits;*.fit;*.FITS;*.FIT"));

  if (filename.IsEmpty() ) {
    return;
  }
  full_filename = filename;

// Removes the directory name (since the full path is generally too long...)
  wxFileName::SplitPath(full_filename, &path, &filename, &extension);
  m_filename1 = filename + wxT(".") + extension;

// Look for generic name without extension (with path !) and copy to char array:
 strcpy(generic_name0, full_filename.c_str());
 last_index = -1;
 imax = strlen(generic_name0);
 for(i = 0; i < imax; i++) {
  if(generic_name0[i] == '\0') break;
  if(generic_name0[i] == '_') last_index = i;
 }
 if(last_index >= 0) generic_name0[last_index] = '\0';

// Write filename to status bar and title
SetTitle(m_filename1);
str1 = wxT("File ") + m_filename1 + wxT(" successfully loaded");
SetText_to_StatusBar(str1,0);

if(bispectrum_was_computed) {
    menuProcessing->Enable(ID_PROCMENU_BISP_RESTORE, true);
    m_toolbar->EnableTool(ID_BISP_RESTORE, true);
  } else {
    menuProcessing->Enable(ID_PROCMENU_BISP_RESTORE, false);
    m_toolbar->EnableTool(ID_BISP_RESTORE, false);
  }

// Load the long integration image to determine nx, ny
sprintf(fname0, "%s_l.fits", generic_name0);
status = JLP_RDFITS_2D_dble(&dble_image0, &nx0, &ny0, &nz0, 1, fname0,
                             comments0, err_messg0);
if (status) {
  str1.Printf(_T("Couldn't load image : >%s< from %s"), fname0, generic_name0);
  wxLogError(str1);
  return;
  }

// JLP2016: now always assume that no processing has been done yet
 if(jlp_decode1 != NULL) delete jlp_decode1;
 // Case when ANDOR camera is connected:
// Load information from ANDOR into the descriptors for output FITS file:
 m_cam1->Cam1_Get_CameraSettings(&CamSet0);
 m_cam1->Cam1_Write_CameraSettingsToFITSDescriptors(&descrip0);
 jlp_decode1 = new JLP_Decode(Pset1, CamSet0, descrip0,  nx0, ny0,
                              &Str0[iLang][0], NMAX_MESSAGES, iLang);
 status = jlp_decode1->DC_LoadProcessingResults(generic_name0,
                                                bispectrum_was_computed);

// Update the message bar to indicate all arrays have been erased:
 Main_UpdateMessageBar(jlp_decode1);

// Update images in child windows from jlp_decode1:
 jlp_aspdisp_panel->UpdateChildWindows(jlp_decode1, -1);
 jlp_aspdisp_panel->RefreshDisplay();

// Set the bool flag :
 input_processed_data1 = true;

return;
}
/******************************************************************
* Save input frames to a single 3D FITS file
*******************************************************************/
void AspFrame::OnSaveFramesTo3DFits( wxCommandEvent &event )
{

SaveFramesTo3DFits();

return;
}
/******************************************************************
* Save processing results to 2D FITS files
*******************************************************************/
void AspFrame::OnSaveProcessingResults( wxCommandEvent &event )
{
int status;

status = SaveProcessingResults();

if(status == 0) m_toolbar->EnableTool(ID_SAVE_RESULTS, false);

return;
}
/****************************************************************************
* Save processing results contained in Decode_work
* and also temporary 3DFITS file if Pset1.SaveFramesToFitsFile is not null
*
****************************************************************************/
int AspFrame::SaveProcessingResults()
{
wxString str1, path1, extension1;
wxString full_filename1, generic_name1, directory1, comments1;
char generic_name0[80], comments0[80], directory0[80];
wxFileDialog *saveFileDialog;
int status;

 if(initialized != 1234){
  return(-1);
 }

// Prompt for filename
 saveFileDialog = new wxFileDialog(this,
                     wxT("Save processing results to FITS files"),
                     wxT(""), wxT(""),
                     wxT("FITS files (*.fit;*.fits)|*.fit;*.fits;*.FIT;*.FITS"),
                     wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

// Set default directory to the value selected in the process settings panel:
 saveFileDialog->SetDirectory(wxString(Pset1.outResults_directory));
// wxString(Pset1.out3DFITS_directory);

// Stops the timers:
 StopDisplayTimers();
 status = saveFileDialog->ShowModal();
 RestartDisplayTimers();

// If "cancel", exit from here without saving files
 if (status == wxID_CANCEL) return(-1);

 full_filename1 = saveFileDialog->GetFilename();

 directory1 = saveFileDialog->GetDirectory();
 strcpy(directory0, directory1.c_str());

// Removes the directory name (since the full path is generally too long...)
 wxFileName::SplitPath(full_filename1, &path1, &generic_name1, &extension1);
 m_filename1 = generic_name1 + wxT(".") + extension1;

 strncpy(generic_name0, generic_name1.c_str(), 80);

// Prompt for comments
 PromptForComments(comments1);
 strncpy(comments0, comments1.c_str(), 80);

// Get Results, i.e. ask if processing is finished
// and jlp_decode1 pointer is free
 status = m_decode_thread->GetResults();
 if(status) {
  wxMessageBox(wxT("Error retrieving data from DecodeThread"),
               wxT("SaveProcessingResults"), wxOK | wxICON_ERROR);
  return(-1);
 }

// Save all results and also 3D FITS cube if required by PROCESS_SETTINGS Pset1
 status = jlp_decode1->OutputFilesProcessCube(generic_name0, directory0, comments0);
 if(status != 0) return(-1);
// Update the message bar to indicate data has been saved:
 Main_UpdateMessageBar(jlp_decode1);

 m_toolbar->EnableTool(ID_SAVE_RESULTS, false);
 menuFileSave->Enable(ID_SAVE_RESULTS2, false);

// Disable also 3DFits file saving if it was done here:
 if(Pset1.SaveFramesToFitsFile != 0) {
   menuFileSave->Enable(ID_SAVE_TO_3DFITS, false);
   }

return(0);
}
/****************************************************************************
* Save elementary frames to a 3D FITS file
*
****************************************************************************/
int AspFrame::SaveFramesTo3DFits()
{
wxString str1, path1, extension1;
wxString full_filename1, generic_name1, comments1, directory1;
char generic_name0[80], comments0[80], directory0[80];
wxFileDialog *saveFileDialog;
DESCRIPTORS descrip;
bool expand_files;
int status;
descrip.ndescr = 0;

  if(initialized != 1234) return(-1);

// Prompt for filename
  saveFileDialog = new wxFileDialog(this,
                     wxT("Save elementary frames to FITS file"),
                     wxT(""), wxT(""),
                     wxT("FITS files (*.fit;*.fits)|*.fit;*.fits;*.FIT;*.FITS"),
                     wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

// Set default directory to the value selected in the process settings panel:
 saveFileDialog->SetDirectory(wxString(Pset1.out3DFITS_directory));

// Stops the timers:
 StopDisplayTimers();
 status = saveFileDialog->ShowModal();
 RestartDisplayTimers();

// If "cancel", exit from here without saving files
 if (status == wxID_CANCEL) return(-1);

 full_filename1 = saveFileDialog->GetFilename();
 directory1 = saveFileDialog->GetDirectory();
 strncpy(directory0, directory1.c_str(), 80);

// Removes the directory name (since the full path is generally too long...)
 wxFileName::SplitPath(full_filename1, &path1, &generic_name1, &extension1);
 m_filename1 = generic_name1 + wxT(".") + extension1;

 strncpy(generic_name0, generic_name1.c_str(), 80);

// Load information from ANDOR into the descriptors for output FITS file:
 descrip.ndescr = 0;
 if(m_cam1 != NULL) m_cam1->Cam1_Write_CameraSettingsToFITSDescriptors(&descrip);

// Prompt for comments
 PromptForComments(comments1);
 strncpy(comments0, comments1.c_str(), 80);

// Get Results, i.e. ask if processing is finished
// and jlp_decode1 pointer is free
 status = m_decode_thread->GetResults();
// If failure retrieving data, process new data and "return" from here:
 if(status) {
  wxMessageBox(wxT("Error retrieving processed data from DecodeThread!"),
               wxT("SaveFramesTo3DFits"), wxOK | wxICON_ERROR);
  return(-1);
  }

// Save temporary 3D File and save to 3D FITS file:
  expand_files = false;
  jlp_decode1->Close_outFITS_3DFile_and_Save(generic_name0, directory0,
                                             comments0, descrip, expand_files);

  menuFileSave->Enable(ID_SAVE_TO_3DFITS, false);

return(0);
}
/****************************************************************************
* Get comments from the user
*
****************************************************************************/
int AspFrame::PromptForComments(wxString &comments0)
{
int status;
wxTextEntryDialog dlg(this, wxT("Comments"), wxT("Enter your comments"));

// Initialization (in case of "Cancel"):
comments0 = wxT("");

// Limitation to ascii
dlg.SetTextValidator(wxFILTER_ASCII);

// Other possibilities:
// Letters:
//dlg.SetTextValidator(wxFILTER_ALPHA)
// Letters and numbers:
//dlg.SetTextValidator(wxFILTER_ALPHANUMERIC)

 dlg.SetValue(wxT(""));

// Stops the timers:
 StopDisplayTimers();
 status = dlg.ShowModal();
 RestartDisplayTimers();

// If "cancel", exit from here without saving files
 if (status == wxID_CANCEL) return(-1);

 comments0 = dlg.GetValue();

return(0);
}
