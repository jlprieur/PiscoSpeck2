/****************************************************************************
* Name: asp2_process.cpp
*
* JLP
* Version 08/10/2017
****************************************************************************/
#include "asp2_frame.h"
#include "asp2_frame_id.h"
#include "asp_display_panel.h" // JLP_AspDispPanel class
#include "jlp_camera_display_mutex.h" // JLP_CameraDisplayMutex class
#include "jlp_display2_mutex.h" // JLP_Display2Mutex class
#include "jlp_andor_cam1.h" // JLP_AndorCam1 class

/********************************************************************
* Create Camera data acquisition thread
* with an independent frame to display streaming images
*
*******************************************************************/
void AspFrame::CreateCameraThread()
{
JLP_CAMERA_SETTINGS CamSet0;

// should not be too large since this is the minimum size,
// but this window can be enlarged by the user
int width = 200, height = 240;

 if((m_cam1 == NULL) || (m_camera_thread != NULL)) return;

// Create streaming display:
 m_cam1->Cam1_Get_CameraSettings(&CamSet0);

 m_CameraDisplay = new JLP_CameraDisplay(CamSet0, width, height);

 m_CameraDisplay->Show(true);

// Create mutex to handle live mode (displaying of short exposures):
 m_CameraDisplayMutex = new JLP_CameraDisplayMutex(CamSet0.gblXPixels, CamSet0.gblYPixels);

// Create thread:
 m_camera_thread = new JLP_CameraThread(this, m_CameraDisplayMutex, m_cam1);
 if ( m_camera_thread->Run() != wxTHREAD_NO_ERROR ) {
      wxMessageBox(wxT("Fatal error: can't create camera_thread!"),
                   wxT("CreateCameraThread"), wxOK | wxICON_ERROR);
      exit(-1);
    }

// Start the timer to display short exposures:
  m_camera_display1_timer->Start(display1_timer_ms_delay, wxTIMER_CONTINUOUS);

// To direct timer events to this class:
  m_camera_display1_timer->SetOwner(this, ID_CAMDISP_TIMER);

return;
}
/********************************************************************
* Create processing thread
*******************************************************************/
void AspFrame::CreateDecodeThread()
{
   if(m_decode_thread != NULL) return;

// Create mutex to handle live mode (displaying of short exposures):
   m_Display2Mutex = new JLP_Display2Mutex(NX1_MAX, NY1_MAX, NCHILDS_MAX);

// Create thread:
   m_decode_thread = new JLP_DecodeThread(this, jlp_aspdisp_panel,
                                          m_Display2Mutex);

    if ( m_decode_thread->Run() != wxTHREAD_NO_ERROR ) {
      wxMessageBox(wxT("Fatal error: can't create decode_thread!"),
                   wxT("CreateDecodeThread"), wxOK | wxICON_ERROR);
      exit(-1);
    }
// after the call to wxThread::Run(), the m_decode_thread pointer is "unsafe":
// at any moment the thread may cease to exist (because it completes its work).
// To avoid dangling pointers OnThreadExit() will set m_decode_thread
// to NULL when the thread dies.

return;
}

/********************************************************************
* Start processing
********************************************************************/
int AspFrame::StartProcessing()
{
JLP_CAMERA_SETTINGS CamSet0;
wxString buffer;
int status, iPage, nx0_ROI, ny0_ROI;
char filename0[128];
DESCRIPTORS descrip0;
descrip0.ndescr = 0;

// Select graphic panel:
iPage = 2;
m_notebook->SetSelection(iPage);

 if(m_decode_thread == NULL){
   wxMessageBox(wxT("Error: DecodeThread is not started yet !"),
                wxT("StartProcessing"), wxOK | wxICON_ERROR);
   return(-1);
   }

 if(m_camera_thread == NULL){
   wxMessageBox(wxT("Error: CameraThread is not started yet !"),
                wxT("StartProcessing"), wxOK | wxICON_ERROR);
   return(-1);
   }

// Erase frame (and ask for results saving if not done yet)
  EraseAllAfterProcessing();

// Retrieve processing settings (in Pset1) from ProcessingPanel:
// Retrieve latest Andor settings (in CamSet1) from Andor Panel:
// And update display child panels with new values of nx1, ny1:
  UpdateDisplaySetup();

// Get current settings from the camera:
  m_cam1->Cam1_Get_CameraSettings(&CamSet0);
  nx1 = CamSet0.nx1;
  ny1 = CamSet0.ny1;
// Preparing short exposure display (in m_AndorDisplay, with m_andor_display1_timer)
  if(short_exposure1 != NULL) delete[] short_exposure1;
  short_exposure1 = new double[nx1 * ny1];
// Set size of ROI to zero to avoid displaying the small rectangle:
  nx0_ROI = 0;
  ny0_ROI = 0;
// nx0_ROI, ny0_ROI : size of ROI (used for displaying a white box)
// Load new settings to display (for displaying the ROI white rectangle)
  m_CameraDisplay->LoadNewSettings(CamSet0, nx0_ROI, ny0_ROI);

// simulation=false, no_processing=false
  m_camera_thread->SetupForAcquisition(nx1, ny1, Pset1.nz_cube, CamSet0.xc0,
                                       CamSet0.yc0, CamSet0.xbin, CamSet0.ybin,
                                       false, false);

// Allocate memory for temporary cube:
  if(ImageCube[0] != NULL) delete[] ImageCube[0];
  if(ImageCube[1] != NULL) delete[] ImageCube[1];
  ImageCube[0] = new AWORD[nx1 * ny1 * Pset1.nz_cube];
  ImageCube[1] = new AWORD[nx1 * ny1 * Pset1.nz_cube];

// Init decode thread:
  if(jlp_decode1 != NULL) delete jlp_decode1;
  if(m_cam1 != NULL)
    m_cam1->Cam1_Write_CameraSettingsToFITSDescriptors(&descrip0);
  else
    descrip0.ndescr = 0;
  jlp_decode1 = new JLP_Decode(Pset1, CamSet0, descrip0, nx1, ny1,
                               &Str0[iLang][0], NMAX_MESSAGES, iLang);
  m_decode_thread->Setup(jlp_decode1);
  Cube_is_processed = true;


// Start the process
  nz_processed = 0;
  nz_sent = 0;

  status = m_camera_thread->StartLoadingCubeForProcessing();
  if(status == 0) {
    nz_sent += Pset1.nz_cube;
    Cube_is_loaded[0] = false;
    Cube_is_loaded[1] = false;
// Update flag :
    new_data_is_needed = true;
// Enable display menu since decode_thread has been initialized now:
    menu_bar->EnableTop( menu_bar->FindMenu(_T("Display")), true);
  }
return(status);
}
/********************************************************************
* Start direct visualisation without processing
********************************************************************/
int AspFrame::StartStreaming()
{
JLP_CAMERA_SETTINGS CamSet0;
wxString buffer;
int nx0_ROI, ny0_ROI;

// Not allowed if no camera:
 if(m_cam1 == NULL) return(-1);

// Return if last processing/streaming is still running:
 if(new_data_is_needed) return(-1);

 if(m_camera_thread == NULL){
   wxMessageBox(wxT("Error: CameraThread is not be started yet !"),
                wxT("StartStreaming"), wxOK | wxICON_ERROR);
   return(-1);
   }

// Retrieve processing settings (in Pset1) from ProcessingPanel:
// Retrieve latest Camera settings (in CamSet1) from Camera Panel:
// And update display child panels
  UpdateDisplaySetup();

// Set nx1, ny1, nz1 for the cubes:
// and setup all threads
  Pset1.nz_cube = 20;

// Get current settings from the camera:
  m_cam1->Cam1_Get_CameraSettings(&CamSet0);

// Old values of nx1, ny1 will be stored into nx0_ROI, ny_ROI:
  nx0_ROI = CamSet0.nx1 * CamSet0.xbin;
  ny0_ROI = CamSet0.ny1 * CamSet0.ybin;

// Full size, no binning, for streaming:
  CamSet0.nx1 = CamSet0.gblXPixels;
  CamSet0.ny1 = CamSet0.gblYPixels;
  CamSet0.xbin = 1;
  CamSet0.ybin = 1;
  nx1 = CamSet0.nx1;
  ny1 = CamSet0.ny1;

// nx0_ROI, ny0_ROI : size of ROI (used for displaying a white box)
// Load new settings to display (for displaying the ROI white rectangle)
  m_CameraDisplay->LoadNewSettings(CamSet0, nx0_ROI, ny0_ROI);

// Preparing short exposure display (in m_CameraDisplay, with m_camera_display1_timer)
  if(short_exposure1 != NULL) delete[] short_exposure1;
  short_exposure1 = new double[nx1 * ny1];

// simulation=false, no_processing=true
//TEST  m_camera_thread->Pause();
  m_camera_thread->SetupForAcquisition(nx1, ny1, Pset1.nz_cube, CamSet0.xc0,
                                       CamSet0.yc0, CamSet0.xbin, CamSet0.ybin,
                                       false, true);

  m_camera_thread->StartLoadingCubeForStreaming();

// Update flag :
  new_data_is_needed = true;

return(0);
}
/********************************************************************
* Stop is required by the user
********************************************************************/
void AspFrame::StopIsRequired()
{

// Set flag "new_data_is_needed" to false:
// To stop the process at the next reception of "Decode Event or Streaming Event"
  new_data_is_needed = false;

// If DecodeThread is not processing, stop immediately;
  if(m_decode_thread->IsProcessing() == false) StopProcessing();

}
/****************************************************************
* Event received from DecodeThread: data cube has been processed
* and thread is ready for processing new cube
****************************************************************/
void AspFrame::OnReceiveDoneEventFromDecode(wxThreadEvent &event)
{
int status;

if(initialized != 1234) return;

// Set processed flag to true:
 Cube_is_processed = true;
 nz_processed += Pset1.nz_cube;
 if(camera_type1 == 0) {
   if(nz_processed > (m_cam1->Nz_total() - Pset1.nz_cube))
     new_data_is_needed = false;
   }

// Refresh the display:
  RefreshDisplayWhenDecodeEvent();

// Stop here if no new data is needed (signal for stopping processing):
 if(!new_data_is_needed) {
    StopProcessing();
    return;
   }

// Start processing the waiting cube: priority to cube 0
// In case of 3D FITS file, loading is immediate, so cube 1 is not used
  if(Cube_is_loaded[0] == true) {
    m_decode_thread->StartProcessingCube(ImageCube[0], nx1, ny1,
                                         Pset1.nz_cube);
    Cube_is_processed = false;
    Cube_is_loaded[0] = false;
    } else if(Cube_is_loaded[1] == true) {
    m_decode_thread->StartProcessingCube(ImageCube[1], nx1, ny1,
                                         Pset1.nz_cube);
    Cube_is_processed = false;
    Cube_is_loaded[1] = false;
    }

// Start loading new cube:
  if((Cube_is_loaded[0] == false) || (Cube_is_loaded[1] == false)) {
     if(m_camera_thread->IsLoading() == false) {
      if((camera_type1 > 0) || (nz_sent <= (m_cam1->Nz_total() - Pset1.nz_cube))) {
        m_camera_thread->StartLoadingCubeForProcessing();
        nz_sent += Pset1.nz_cube;
        }
      }
  }
return;
}
/****************************************************************
* Event received from CameraThread: data cube has been loaded
* and this cube is ready for retrieval
****************************************************************/
void AspFrame::OnReceiveDoneEventFromCamera(wxThreadEvent &event)
{
int icube, status;

 if(new_data_is_needed) {
// Get loaded cube from CameraThread: priority to cube 0
// In case of Andor/Raptor, cubes are equivalent, whatever the time when they are taken
    if(Cube_is_loaded[0] == false) {
      icube = 0;
      } else {
      icube = 1;
      }
// WARNING: should not pause the thread before calling GetResults !
    status = m_camera_thread->GetResults(ImageCube[icube], nx1, ny1,
                                        Pset1.nz_cube);

// If failure retrieving data, ask for new data and "return" from here:
    if(status) {
// Prepare thread for loading:
      m_camera_thread->StartLoadingCubeForProcessing();
      return;
      }


// Else set loaded flag to true:
    Cube_is_loaded[icube] = true;

// Start processing this cube if queue was empty:
   if(Cube_is_processed == true) {
     m_decode_thread->StartProcessingCube(ImageCube[icube], nx1, ny1,
                                          Pset1.nz_cube);
     Cube_is_processed = false;
     Cube_is_loaded[icube] = false;
   }

// Start loading new cube:
  if(((Cube_is_loaded[0] == false) || (Cube_is_loaded[1] == false)) &&
     (m_camera_thread->IsLoading() == false)) {
       status = m_camera_thread->StartLoadingCubeForProcessing();
       if(status == 0) nz_sent += Pset1.nz_cube;
      }
  }

return;
}
/****************************************************************
* Event received from CameraThread: streaming has halted
* and is waiting for new start
****************************************************************/
void AspFrame::OnReceiveStreamingEventFromCamera(wxThreadEvent &event)
{
int status = -1;

 if(new_data_is_needed) {
// Return 10 when all data has been read (for fitscubes)
    status = m_camera_thread->StartLoadingCubeForStreaming();
  }


// Stop here when no new data is needed (signal for stopping processing)
// or when all data has been read (for fitscubes)
  if(status != 0) {
    StopStreaming();
  }

return;
}
/********************************************************************
* Handle click on start streaming button
********************************************************************/
void AspFrame::OnStartStreaming(wxCommandEvent &WXUNUSED(event))
{
int status;

 if(initialized != 1234) return;

// Go to display panel (and generate page changing event)
 m_notebook->SetSelection(2);

// Call StartStreaming
 status = StartStreaming();

// Enable/disable buttons
 if((status == 0) && (m_toolbar != NULL)) {
  m_toolbar->EnableTool(ID_START_STREAM, false);
  m_toolbar->EnableTool(ID_START_PROCESS, false);
  m_toolbar->EnableTool(ID_SAVE_RESULTS, false);
  m_toolbar->EnableTool(ID_ERASE_ALL, false);
  m_toolbar->EnableTool(ID_STATISTICS, false);
  m_toolbar->EnableTool(ID_BISP_RESTORE, false);
  m_toolbar->EnableTool(ID_STOP_STREAM, true);
  m_toolbar->EnableTool(ID_STOP_PROCESS, false);
  }

}
/********************************************************************
* Handle click on start processing button
********************************************************************/
void AspFrame::OnStartProcessing(wxCommandEvent &WXUNUSED(event))
{
int status;

 if(initialized != 1234) return;

// Go to display panel (and generate page changing event)
 m_notebook->SetSelection(2);

// Call StartProcessing
 status = StartProcessing();

// Enable/disable buttons
 if((status == 0) && (m_toolbar != NULL)) {
  m_toolbar->EnableTool(ID_START_STREAM, false);
  m_toolbar->EnableTool(ID_START_PROCESS, false);
  m_toolbar->EnableTool(ID_SAVE_RESULTS, false);
  m_toolbar->EnableTool(ID_ERASE_ALL, true);
  m_toolbar->EnableTool(ID_STATISTICS, false);
  m_toolbar->EnableTool(ID_BISP_RESTORE, false);
  m_toolbar->EnableTool(ID_STOP_STREAM, false);
  m_toolbar->EnableTool(ID_STOP_PROCESS, true);
  }

}
/********************************************************************
* Handle click on stop processing tool button
********************************************************************/
void AspFrame::OnStopProcessing(wxCommandEvent &WXUNUSED(event))
{
   if(initialized != 1234) return;

// Set new_data_is_needed to false:
// and wait until processing/streaming event is received
   StopIsRequired();
   if(m_toolbar != NULL) {
    m_toolbar->EnableTool(ID_STOP_PROCESS, false);
   }
}
/********************************************************************
* Stop processing : enable/disable buttons
* (called when processing is finished,
* or when the user has pressed the "stop" button: in that
* case StopProcessing is called a few times !)
********************************************************************/
void AspFrame::StopProcessing()
{
wxTimerEvent uu;

  if(initialized != 1234) return;

if(m_toolbar != NULL) {
  m_toolbar->EnableTool(ID_SAVE_RESULTS, true);
  m_toolbar->EnableTool(ID_ERASE_ALL, true);
  m_toolbar->EnableTool(ID_STATISTICS, true);
//  m_toolbar->EnableTool(ID_BISP_RESTORE, true);

  m_toolbar->EnableTool(ID_START_STREAM, true);
  m_toolbar->EnableTool(ID_START_PROCESS, true);
  m_toolbar->EnableTool(ID_STOP_STREAM, false);
  m_toolbar->EnableTool(ID_STOP_PROCESS, false);
}

/*
* ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
* ProcessingMode = 5 : Bispectrum, autoc, quadrant
*/
  if((Pset1.ProcessingMode == 4) || (Pset1.ProcessingMode == 5)) {
    menuProcessing->Enable(ID_PROCMENU_BISP_RESTORE, true);
    if(m_toolbar != NULL) m_toolbar->EnableTool(ID_BISP_RESTORE, true);
  } else {
    menuProcessing->Enable(ID_PROCMENU_BISP_RESTORE, false);
    if(m_toolbar != NULL) m_toolbar->EnableTool(ID_BISP_RESTORE, false);
  }

// To see those changes:
Refresh();
return;
}
/********************************************************************
* Handle click on stop streaming tool button
********************************************************************/
void AspFrame::OnStopStreaming(wxCommandEvent &WXUNUSED(event))
{
  if(initialized != 1234) return;

// Set new_data_is_needed to false:
// and wait until processing/streaming event is received
  StopIsRequired();
  if(m_toolbar != NULL) m_toolbar->EnableTool(ID_STOP_STREAM, false);

}
/********************************************************************
* Stop streaming: enable/disable buttons
* (called when processing is finished)
********************************************************************/
void AspFrame::StopStreaming()
{
  if(initialized != 1234) return;

  new_data_is_needed = false;

  if(m_toolbar != NULL) {
   m_toolbar->EnableTool(ID_SAVE_RESULTS, false);
   m_toolbar->EnableTool(ID_ERASE_ALL, false);
   m_toolbar->EnableTool(ID_STATISTICS, false);
   m_toolbar->EnableTool(ID_BISP_RESTORE, false);
   m_toolbar->EnableTool(ID_START_STREAM, true);
   m_toolbar->EnableTool(ID_STOP_STREAM, false);
   m_toolbar->EnableTool(ID_START_PROCESS, true);
   m_toolbar->EnableTool(ID_STOP_PROCESS, false);
   }

// To see those changes:
Refresh();
}
/*********************************************************************
* Update toolbar static text
*
*********************************************************************/
void AspFrame::UpdateToolbarLabel(wxString filename0)
{
wxString buffer;

   buffer = wxT("");

// Disable Start/Stop button in DisplayPanel if not connected:
  switch(camera_type1) {
      default:
      case 0:
// i=14 "Processing 3D files."
       buffer = Str0[iLang][14] + wxT(" (FITS data cube : ") + filename0 + wxT(")");
       break;
      case 1:
// i=15 "ANDOR camera is connected."
       buffer = Str0[iLang][15];
       break;
      case 2:
// i=16 "RAPTOR camera is connected."
       buffer = Str0[iLang][16];
       break;
   }

// Update label in toolbar (with the filename if camera is not connected)
 if(m_toolbar != NULL) DispStatic1->SetLabel(buffer);

return;
}
/********************************************************************
* Handle click on "EraseAll" tool button
********************************************************************/
void AspFrame::OnEraseAll(wxCommandEvent &WXUNUSED(event))
{
  if(initialized != 1234) return;

  if(new_data_is_needed) {
    EraseAllWhenProcessing();
   } else {
    EraseAllAfterProcessing();
   }
}
/****************************************************************************
* Erase all arrays after processing (when processing is finished)
* Return -1 if data not yet saved and when the user want to save it later)
*
****************************************************************************/
int AspFrame::EraseAllAfterProcessing()
{
int status;

// Called when closing the windows, with m_decode_thread = NULL
 if(m_decode_thread == NULL) return(-1);

// Get Results, i.e. ask if processing is finished
// and jlp_decode1 pointer is free
 status = m_decode_thread->GetResults();
 if((status  == 0) && (jlp_decode1 != NULL)) {
// Erase arrays in Decode (return -1 when data not yet saved, and
// the user wants to save data)                           )
  if(jlp_decode1 != NULL) {
   status = jlp_decode1->EraseProcessCube(1);
   if(status) SaveProcessingResults();
   }
 } else {
  return(-1);
 }

// Update the message bar to indicate all arrays have been erased:
  Main_UpdateMessageBar(jlp_decode1);

// Update images in child windows from jlp_decode1:
  jlp_aspdisp_panel->UpdateChildWindows(jlp_decode1, -1);
  jlp_aspdisp_panel->RefreshDisplay();

return(0);
}
/****************************************************************************
* Erase all arrays when processing is running
*
****************************************************************************/
void AspFrame::EraseAllWhenProcessing()
{

 if(m_decode_thread == NULL) return;

 m_decode_thread->EraseAll();

return;
}
