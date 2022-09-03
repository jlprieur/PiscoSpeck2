/****************************************************************************
* Name: AspFrame.cpp
* AspFrame class
* Purpose: display and process 3D-FITS or Camera images
*
* JLP
* Version 16/09/2015
****************************************************************************/
#include <stdlib.h>   // exit()
#include "time.h"

#include "wx/progdlg.h"

/*
#if !wxUSE_TOGGLEBTN
    #define wxToggleButton wxCheckBox
    #define EVT_TOGGLEBUTTON EVT_CHECKBOX
#endif
*/

// JLP routines:
#include "asp2_app.h"           // MyAppl()
#include "jlp_wx_ipanel.h"      // JLP_wxImagePanel class
#include "asp_display_panel.h"  // JLP_AspDispPanel class
#include "asp2_frame.h"
#include "asp2_frame_id.h"      // Definition of identifiers
#include "jlp_language_dlg.h"   // JLP_Language_Dlg class
#include "jlp_utshift_dlg.h"    // JLP_UTShift_Dlg class
#include "asp_camera_dlg.h"     // AspCamera_Dlg class
#include "jlp_camera_panel.h"   // JLP_CameraPanel class

/**********************************************************************
* AspFrame constructor
*
* INPUT:
*   GdpWidth,GdpHeight : size of created window
*
***********************************************************************/
AspFrame::AspFrame(const wxChar *title, int GdpWidth, int GdpHeight)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(-1, -1),
         wxSize(GdpWidth, GdpHeight))
{
int status;

// Transform coma into point for numbers:
 setlocale(LC_NUMERIC, "C");

// iLang: 0=English 1=French 2=Italian 3=Spanish 4=German
// Language as default:
// French
  iLang = 1;

// Small fonts by default:
  small_version1 = 0;

// Prompt for camera type
/* Camera type:
* 0 = FitsCube
* 1 = Andor
* 2 = Raptor
*/
  camera_type1 = 0;
// OLD VERSION  PromptForCameraType();

// Prompt the user for a new value of camera type:
  SelectCameraSetup();

// Prompt the user for a new value of iLang:
  SelectLanguageSetup();

// Prompt the UTshift value:
  SelectUTShiftSetup();

// Load menu messages to Str0:
  status = LoadMenuMessages();
  if(status) Close();

// Smaller fonts (needed for Windows, to have a similar appearance as Linux)
if(small_version1) SetWindowVariant(wxWINDOW_VARIANT_SMALL);

// Initialization of private variables
  Main_Init();

// Used for displaying log messages for threads (?)
  m_oldLogger = wxLog::GetActiveTarget();

// Toolbar:
  CreateMyToolbar();

// Status bar:
// Create a status bar with two fields at the bottom:
  m_StatusBar = CreateStatusBar(3);
// First field has a variable length, second and third have a fixed length:
  int widths[3];
  widths[0] = -1;
  widths[1] = 200;
  widths[2] = 200;
  SetStatusWidths( 3, widths );

// Create notebook and its pages (opening camera connection and settings camera_type1)
  NotebookSetup(GdpWidth, GdpHeight);

// Create a menu on top of the window:
  Gdp_SetupMenu();

  initialized = 1234;

// Update labels in DisplayPanel:
  UpdateToolbarLabel(wxT(""));

// Update display panel setup (and get nx1, ny1, Aset1, Pset1, Pset1.nz_cube):
  nx1 = 1; ny1 = 1;
  Init_PSET(Pset1);   // To force camera display setup updating
  Pset1.utime_offset = UTShift;  // Load the value that was entered by the user
  UpdateDisplaySetup();

// DEBUG: xMessageBox(_T("DEBUG:"), _T("Main"), wxOK);

// Create all threads (after initialization of nz1 by UpdateDisplaySetup):
  CreateDecodeThread();

// Camera display1 timer for short exp. display (set it BEFORE creating Camera thread !)
  m_camera_display1_timer = new wxTimer(this, ID_CAMDISP_TIMER);
// Create an independent frame to display streaming images and the Camera thread
  CreateCameraThread();

// Enable/disable buttons:
  if(m_toolbar != NULL) {
   m_toolbar->EnableTool(ID_START_STREAM, true);
   m_toolbar->EnableTool(ID_START_PROCESS, true);
   m_toolbar->EnableTool(ID_SAVE_RESULTS, false);
   m_toolbar->EnableTool(ID_ERASE_ALL, false);
   m_toolbar->EnableTool(ID_STOP_STREAM, false);
   m_toolbar->EnableTool(ID_STOP_PROCESS, false);
   m_toolbar->EnableTool(ID_STATISTICS, false);
   m_toolbar->EnableTool(ID_BISP_RESTORE, false);
  }

// Disable Display menu and update DisplayPanel labels:
  menuProcessing->Enable(ID_PROCMENU_BISP_RESTORE, false);
  menu_bar->EnableTop( menu_bar->FindMenu(_T("Display")), false);

// Clock display on the left side:
  CDisplayClockOnStatusBar();

return;
}
/**********************************************************************
* Main shutdown, called by destructor and OnClose
***********************************************************************/
void AspFrame::Main_ShutDown()
{

// Step 1 : stop and delete timers:
   if(m_camera_display1_timer) {
     m_camera_display1_timer->Stop();
     delete m_camera_display1_timer;
     m_camera_display1_timer = NULL;
   }

// Step2: delete the threads:
   DeleteAllThreads();

// Step3: shutdown of Camera (if connected)
  if(m_cam1 != NULL) {
    m_cam1->Cam1_ShutDown();
    delete m_cam1;
    m_cam1 = NULL;
    }

// Step 4: delete panels
   if(jlp_aspdisp_panel != NULL) {
     delete jlp_aspdisp_panel;
     jlp_aspdisp_panel = NULL;
     }
   if(jlp_camera_panel != NULL) {
     delete jlp_camera_panel;
     jlp_camera_panel = NULL;
     }
   if(jlp_speckprocess_panel != NULL) {
     delete jlp_speckprocess_panel;
     jlp_speckprocess_panel = NULL;
     }

// Step 5: delete separate Camera display window:
  if(m_CameraDisplay) {
    m_CameraDisplay->Destroy();
    m_CameraDisplay = NULL;
   }

};
/***************************************************************************
*
****************************************************************************/
void AspFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{

 Main_ShutDown();

// Close window at the end:
  Destroy();
return;
}
/**********************************************************************
* Update display setup according to process and camera settings
* and initialization of Pset1.nz_cube needed for loadin/processing cubes
*
***********************************************************************/
void AspFrame::UpdateDisplaySetup()
{
int nx1_old, ny1_old, nz1_old, processing_mode_old;
JLP_CAMERA_SETTINGS CamSet0;

  if(initialized != 1234) return;

nx1_old = nx1;
ny1_old = ny1;
nz1_old = Pset1.nz_cube;
processing_mode_old = Pset1.ProcessingMode;

// First retrieve Camera settings from Camera panel1:
  m_cam1->Cam1_Get_CameraSettings(&CamSet0);

// Retrieve processing settings from ProcessingPanel:
  jlp_speckprocess_panel->Get_PSET(Pset1);
  Pset1.utime_offset = UTShift;  // Load the value that was entered by the user

// Compute nx1, ny1:
  nx1 = CamSet0.nx1;
  ny1 = CamSet0.nx1;

// Update display panel setup if needed:
  if((processing_mode_old != Pset1.ProcessingMode)
     || (nx1 != nx1_old) || (ny1 != ny1_old)
     || (nz1_old != Pset1.nz_cube)) {
        jlp_aspdisp_panel->NewDisplaySetup(Pset1);
     }

return;
}
/**********************************************************************
* Initialization of private variables
*
***********************************************************************/
void AspFrame::Main_Init()
{
int k;

// camera display set to NULL (in case Camera is not connected, to avoid pb
// at the end when closing it
  m_CameraDisplay = NULL;
  m_CameraDisplayMutex = NULL;
  m_Display2Mutex = NULL;
  m_cam1 = NULL;

  new_data_is_needed = false;
  Init_PSET(Pset1);
  Pset1.utime_offset = UTShift;  // Load the value that was entered by the user
  jlp_decode1 = NULL;
  input_processed_data1 = false;

// Data cubes:
  ImageCube[0] = NULL;
  ImageCube[1] = NULL;

// For Camera timer:
  short_exposure1 = NULL;

// Dummy values:
  nx1 = 24;
  ny1 = 24;

// For RefreshDisplayWhenDecodeEvent routine:
  nchild_image1 = 0;
  nx1_image1 = 0;
  ny1_image1 = 0;
  for(k = 0; k < NCHILDS_MAX; k++) child_image1[k] = NULL;

// Threads:
  m_camera_thread = NULL;
  m_decode_thread = NULL;

// Text panel as default in notebook:
  i_NotebookPage = 0;

// Set delay value to obtain a rate of 10 images/second for camera images:
  display1_timer_ms_delay = 100;

// Update temperature every 10 seconds (timer is called every 100 msec):
  cooler_counter = 0;
  cooler_counter_max = 100;

return;
}
/**********************************************************************
* Notebook setup
*
***********************************************************************/
void AspFrame::NotebookSetup(int width0, int height0)
{
wxString str1;
int width1, height1;

// Create topsizer to locate panels and log window
  m_topsizer = new wxBoxSizer( wxVERTICAL );
// Create book control (multi-panels):
//  m_notebook = new wxBookCtrl(this, ID_BOOK_CTRL);
  m_notebook = new wxNotebook(this, ID_NOTEBOOK);

// Create Logbook panel first:
  str1 = wxString("");
  width1 = width0;
  height1 = (int)((double)height0 / 8.);
  jlp_Logbook = new JLP_wxLogbook(this, str1, width1, height1);

  wxLog::SetActiveTarget(new wxLogTextCtrl(jlp_Logbook));

// Create CameraSetup panel:
  m_CameraPanel = new wxPanel(m_notebook, ID_APANEL_PAGE);
// i=11 "Andor Setup"
  m_notebook->AddPage(m_CameraPanel, Str0[iLang][11]);
  CameraPanelSetup();

// Create SpeckProcessSetup panel:
// i=12 "Processing Setup"
  m_SpeckProcessPanel = new wxPanel(m_notebook, ID_PROCESS_PAGE);
  m_notebook->AddPage(m_SpeckProcessPanel, Str0[iLang][12]);
  SpeckProcessPanelSetup();

// Create Display panel:
// i=13 "Display Panel"
  m_DisplayPanel = new wxPanel(m_notebook, ID_DISPLAY_PAGE);
  m_notebook->AddPage(m_DisplayPanel, Str0[iLang][13]);
  width1 = width0;
  height1 = (int)((double)height0 * 7. / 8.);
  DisplayPanelSetup(width1, height1);

// Proportion set to 7, i.e., graphic panel will be 7/8 of the window
  m_topsizer->Add(m_notebook, 7, wxEXPAND | wxALL);

// Proportion set to 1, i.e., log window will be 1/8 of the window
  m_topsizer->Add(jlp_Logbook, 1, wxEXPAND);

// Sizer implementation on the panel:
  SetSizerAndFit(m_topsizer);

 }
/*****************************************************************
* Quit (menu item)
*****************************************************************/
void AspFrame::OnQuit (wxCommandEvent& WXUNUSED(event) )
{
    Close(true);
}
/*****************************************************************
* Help (menu item)
*****************************************************************/
void AspFrame::OnHelp( wxCommandEvent& WXUNUSED(event) )
{
// i=97 ("Sorry: \"Help\" is not implemented yet\n"
// i=98 ("Current version: October 2015"),
 (void)wxMessageBox(Str0[iLang][97] + wxT("\n") + Str0[iLang][98],
                    _T("PiscoSpeck2"), wxICON_INFORMATION | wxOK );
}
/*****************************************************************
* About (menu item)
*****************************************************************/
void AspFrame::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
wxString buffer;

// i=99 ("Created with wxWidgets"),
 buffer.Printf(wxT("PiscoSpeck2\nJean-Louis Prieur (c) 2016 \n\
Max image size: %d,%d Max childs=%d\n"), NX1_MAX, NY1_MAX, NCHILDS_MAX);
 buffer.Append(Str0[iLang][99]);
 (void)wxMessageBox( buffer, _T("PiscoSpeck2"),
                     wxICON_INFORMATION | wxOK );

}
/*****************************************************************
* Context help
*****************************************************************/
void AspFrame::OnContextHelp(wxCommandEvent& WXUNUSED(event))
{
// starts a local event loop
    wxContextHelp chelp(this);
}
/************************************************************************
* Display text in status bar
*************************************************************************/
void AspFrame::SetText_to_StatusBar(wxString str1, const int icol)
{
// Update the first field (since 2nd argument is 0 here) of the status bar:
  if(m_StatusBar != NULL) m_StatusBar->SetStatusText(str1, icol);
}

/**********************************************************************
* CameraPanel setup
* Create CameraPanel with JLP_CameraPanel class (widget panel)
*
***********************************************************************/
void AspFrame::CameraPanelSetup()
//int width0, int height0)
{
int panel_ID, ipage, index0;
wxBoxSizer *vbox_sizer;
wxFrame *m_subpanel;

 vbox_sizer = new wxBoxSizer( wxVERTICAL );

// Try to open CameraConnection for the first time:
 jlp_camera_panel = new JLP_CameraPanel(this, (wxFrame *)m_CameraPanel, &m_cam1,
                                        &camera_type1, &Str0[iLang][0],
                                        NMAX_MESSAGES, small_version1,
                                        &panel_ID, &m_subpanel);

// Exit from here if error:
 if(m_cam1 != NULL) {
  if(m_cam1->NotConnected() == true) {
    wxMessageBox(wxT("Fatal error: camera not connected and fits cube not loaded !"),
                 wxT("AspFrame::CameraPanelSetup"), wxICON_ERROR);
    exit(-1);
   }
 } else {
    wxMessageBox(wxT("Fatal error: error instantiating camera object !"),
                 wxT("AspFrame::CameraPanelSetup"), wxICON_ERROR);
  exit(-1);
 }

// 1 : vertically strechable
// wxEXPAND : horizontally strechable
// vbox_sizer->Add((wxFrame *)jlp_camera_panel, 1, wxEXPAND);
 vbox_sizer->Add(m_subpanel, 1, wxEXPAND);

 m_CameraPanel->SetSizerAndFit(vbox_sizer);

// i=9 "FitsCube Setup"  (camera_type == 0)
// i=10 "Andor Setup" (camera_type == 1)
// i=11 "Raptor Setup" (camera_type == 2)
  ipage = 0;
  index0 = 9 + camera_type1;
  m_notebook->SetPageText(ipage, Str0[iLang][index0]);

return;
}
/**********************************************************************
* DisplayPanel setup
* Create CameraPanel with JLP_CameraPanel class (widget panel)
*
***********************************************************************/
void AspFrame::DisplayPanelSetup(int width0, int height0)
{
wxBoxSizer *vbox_sizer;

vbox_sizer = new wxBoxSizer( wxVERTICAL );

// Display Panel
 jlp_aspdisp_panel = new JLP_AspDispPanel(this, (wxFrame *)m_DisplayPanel,
                                          jlp_Logbook, m_StatusBar,
                                          &Str0[iLang][0], NMAX_MESSAGES,
                                          width0, height0,
                                          small_version1);

// 1 : vertically strechable
// wxEXPAND : horizontally strechable
 vbox_sizer->Add((wxFrame *)jlp_aspdisp_panel, 1, wxEXPAND);

 m_DisplayPanel->SetSizerAndFit(vbox_sizer);

return;
}
/**********************************************************************
* Accessor to nchild of JLP_AspDispPanel
* (should be here and not in asp2_frame.h to avoid compilation pb)
***********************************************************************/
int AspFrame::Get_DisplayPanel_decode_idx(int *decode_index0, int *nchild0)
{

 jlp_aspdisp_panel->Get_decode_index(decode_index0);
 jlp_aspdisp_panel->Get_nChild(nchild0);

return(0);
}
/**********************************************************************
* SpeckProcessPanel setup
* Create SpeckProcessPanel with JLP_SpeckProcessPanel class (widget panel)
*
***********************************************************************/
void AspFrame::SpeckProcessPanelSetup()
//int width0, int height0)
{
wxBoxSizer *vbox_sizer;

//int width1, height1;
//int ix, iy, i, j, k;
//width1 = (width0 - 40)/3;
//height1 = (height0 - 40)/2;

vbox_sizer = new wxBoxSizer( wxVERTICAL );

// SpeckProcess Panel
 jlp_speckprocess_panel = new JLP_SpeckProcessPanel((wxFrame *)m_SpeckProcessPanel,
                                                    &Str0[iLang][0],
                                                    NMAX_MESSAGES,
                                                    small_version1);

// 1 : vertically strechable
// wxEXPAND : horizontally strechable
 vbox_sizer->Add((wxFrame *)jlp_speckprocess_panel, 1, wxEXPAND);

 m_SpeckProcessPanel->SetSizerAndFit(vbox_sizer);

return;
}
/**************************************************************************
*
***************************************************************************/
void AspFrame::SelectLanguageSetup()
{
JLP_Language_Dlg *LanguageDlg;
int status, i_lang;

  LanguageDlg = new JLP_Language_Dlg(this, wxT("Language Selection"));

  status = LanguageDlg->ShowModal();

// Retrieve the object/orbit parameters:
  LanguageDlg->RetrieveData(&i_lang);

// Set private variable iLang if status is OK:
  if(status == 0 && i_lang >= 0 && i_lang < NLANG) {
   iLang = i_lang;
   }

delete LanguageDlg;

// Exit if Cancel
if(status) {
 exit(-1);
}

return;
}
/**************************************************************************
*
***************************************************************************/
void AspFrame::SelectUTShiftSetup()
{
JLP_UTShift_Dlg *UTShift_Dlg;
int status;

  UTShift_Dlg = new JLP_UTShift_Dlg(this, wxT("UTShift Selection"));

  status = UTShift_Dlg->ShowModal();

// Retrieve the UTShift value:
  UTShift_Dlg->RetrieveData(&UTShift);

delete UTShift_Dlg;

return;
}
/*************************************************************************
* Prompt for camera type
* Camera type:
* 0 = none
* 1 = Andor
* 2 = Raptor
* 3 = FitsCube
*
*************************************************************************/
/* OLD_VERSION
void AspFrame::PromptForCameraType()
{
wxString s_values, s_question;
long ivalue;

s_question.Printf(wxT("Camera type (0=FitsCube 1=Andor 2=Raptor ) :"), camera_type1);

s_values.Printf(wxT("%d"), camera_type1);

// Prompt for a new value of iframe with a dialog box:
wxString result = wxGetTextFromUser(s_question, _T("PromptCameraType"),
                                   s_values, NULL);
if(!result.IsEmpty()){
  if(result.ToLong(&ivalue) == true) {
    if(ivalue >= 0 && ivalue < 3) {
      camera_type1 = ivalue;
      } else {
      wxLogError(_T("Error/Bad value for camera type"));
      }
  }
}

return;
}
*/
/**************************************************************************
*
* Camera type:
* 0 = FitsCube
* 1 = Andor
* 2 = Raptor
*
***************************************************************************/
void AspFrame::SelectCameraSetup()
{
AspCamera_Dlg *CameraDlg;
int status, icamera;

  CameraDlg = new AspCamera_Dlg(this, wxT("Camera Selection"));

  status = CameraDlg->ShowModal();

// Retrieve the object/orbit parameters:
  CameraDlg->RetrieveData(&icamera);

// Set private variable iLang if status is OK:
// icamera = 0 : FitsCube
// icamera = 1 : Andor
// icamera = 2 : Raptor
  if(status == 0 && icamera >= 0 && icamera < NCAMERA) {
// camera_type = 0 : FitsCube
// camera_type = 1 : Andor
// camera_type = 2 : Raptor
   camera_type1 = icamera;
// Exit if Cancel
   } else {
     exit(-1);
   }

delete CameraDlg;


return;
}
/**********************************************************************
*
***********************************************************************/
void AspFrame::OnIdle(wxIdleEvent& event)
{
//    UpdateThreadStatus();

    event.Skip();
}
/**********************************************************************
* Notebook page is changing
*
* 0 = Camera Panel
* 1 = Processing Panel
* 2 = Display Panel
*
***********************************************************************/
void AspFrame::OnPageChanging( wxBookCtrlEvent &event )
{
int status = 0;
wxString buffer;
int selOld, selNew;

if(initialized != 1234) return;

selOld = event.GetOldSelection();
selNew = event.GetSelection();

 buffer = _T("");

/* DEBUG
   buffer.Printf(wxT("ChangesDone=%d, validated=%d"),
                 jlp_camera_panel->CamPanel_ChangesDone(),
                 jlp_camera_panel->CamPanel_ValidatedChanges());
   wxMessageBox(buffer, _T("PiscoSpeck2"), wxOK);
*/

// Check is changes have been done and not validated
 if((selOld == 0) &&
    (jlp_camera_panel->CamPanel_ChangesDone() == true) &&
     (jlp_camera_panel->CamPanel_ValidatedChanges() == false)) {
   buffer = _T("Exit from Camera parameter selection.\n");
   status = -1;
 } else if((selOld == 1) &&
    (jlp_speckprocess_panel->ChangesDone() == true) &&
     (jlp_speckprocess_panel->ValidatedChanges() == false)) {
   buffer = _T("Exit from Processing selection.\n");
   status = -2;
 } else {
   status = 0;
 }

/* DEBUG
buffer.Printf(wxT("selOld = %d selNew = %d status=%d Camera: changed=%d validated=%d\n"),
              selOld, selNew, status,
              (int)jlp_camera_panel->ChangesDone(),
              (int)jlp_camera_panel->ValidatedChanges());
wxMessageBox(buffer, wxT("OnPageChanging"), wxOK);
*/

// Prompt the user if changes have not been validated (nor cancelled):
 if(status) {
   buffer.Append(_T("Warning: changes have not been validated, nor cancelled yet!\n"));
   buffer.Append(_T("\n Do you want to validate those changes ?\n"));
   if(wxMessageBox(buffer, _T("PiscoSpeck2"),
                 wxICON_QUESTION | wxYES_NO, this) == wxYES ) {
     if(selOld == 0) jlp_camera_panel->CamPanel_ValidateNewSettings();
     else if (selOld == 1) jlp_speckprocess_panel->ValidateChanges();
   } else {
     if(selOld == 0) jlp_camera_panel->CamPanel_CancelNonValidatedChanges();
     else if (selOld == 1) jlp_speckprocess_panel->CancelNonValidatedChanges();
   }
 }

   if(selOld == 0) {
// Update Display Setup if changes have been done (and validated)
    if(jlp_camera_panel->CamPanel_ChangesDone() == true) {
       UpdateDisplaySetup();
// Needed to reset ChangesDone and ValidatedChanges for preparing when
// the camera panel is opened again
       jlp_camera_panel->CamPanel_CancelNonValidatedChanges();
       }
   } else if(selOld == 1) {
// Update Display Setup if changes have been done (and validated)
    if(jlp_speckprocess_panel->ChangesDone() == true) {
       UpdateDisplaySetup();
// Needed to reset ChangesDone and ValidatedChanges for preparing when
// the processing panel is opened again
       jlp_speckprocess_panel->CancelNonValidatedChanges();
       }
    }

// Block page change
//   event.Veto();

// Allow page change:
   event.Skip();

return;
}
/**********************************************************************
* Notebook page has changed
*
* 0 = camera Panel
* 1 = Processing Panel
* 2 = Display Panel
*
***********************************************************************/
void AspFrame::OnPageChanged( wxBookCtrlEvent &event )
{
 int selNew;

if(initialized != 1234) return;

selNew = event.GetSelection();

/*
  printf("OnPageChanged: selNew=%d\n", selNew);

  if(selNew == 2)
     wxMessageBox(_T("OK Display Panel selected\n"), _T("PiscoSpeck2"),
                  wxOK);
*/

  event.Skip();
}
