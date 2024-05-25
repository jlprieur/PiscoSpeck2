/*****************************************************************************
* asp2_decode_thread.h
*
* JLP_DecodeThread
*
* JLP version 19/01/2016
*
*****************************************************************************/
#include "asp2_app.h"
#include "asp2_decode_thread.h"        // JLP_DecodeThread()...
#include "asp2_frame_id.h"             // ID_DECODE_THREAD_DONE
#include "asp2_frame.h"                // AspFrame class
#include "asp_display_panel.h"        // JLP_AspDispPanel class
#include "jlp_display2_mutex.h"        // JLP_Display2Mutex class

/*
#define DEBUG
*/

/****************************************************************************
* JLP_DecodeThread
* Constructor
****************************************************************************/
JLP_DecodeThread::JLP_DecodeThread(AspFrame *asp_frame,
                                   JLP_AspDispPanel *jlp_aspdisp_panel,
                                   JLP_Display2Mutex *jlp_display2_mutex0)
                                   : wxThread(wxTHREAD_DETACHED)
{
int k, i;

  m_aspframe = asp_frame;
  jlp_display2_mutex = jlp_display2_mutex0;
  jlp_display_panel1 = jlp_aspdisp_panel;
  cube_to_be_processed = false;
  erase_all = false;
  jlp_decode1 = NULL;
  icube1 = 0;

// Dummy values of nx1, ny1, nz1 for cubes:
  nx1 = 8;
  ny1 = 8;
  nz1 = 4;

// Prompt for current value of nchild:
// and update ipanel_decode_index:
 m_aspframe->Get_DisplayPanel_decode_idx(ipanel_decode_index, &nchild1);

// Allocate memory:
 for(k = 0; k < nchild1; k++) {
   child_image1[k] = new double[nx1 *ny1];
   for(i = 0; i < nx1 * ny1; i++) child_image1[k][i] = 0.;
 }

return;
}
/****************************************************************************
* Setup before starting new processing
*
****************************************************************************/
void JLP_DecodeThread::Setup(JLP_Decode *jlp_decode0)
{
int k, i, nx2, ny2, nz2, nchild2;

 cube_to_be_processed = false;
 erase_all = false;

// New Decode1 pointer:
 jlp_decode1 = jlp_decode0;

// Prompt for current value of nchild:
// and update ipanel_decode_index:
 m_aspframe->Get_DisplayPanel_decode_idx(ipanel_decode_index, &nchild2);

// Get current values of nx1, ny1, nz1 for cubes:
 jlp_decode1->GetNxNyNz_cube(&nx2, &ny2, &nz2);

// Allocate memory
 if((nchild2 != nchild1) || (nx2 != nx1) || (ny2 != ny1)) {
   for(k = 0; k < nchild1; k++) {
     delete[] child_image1[k];
     }
   for(k = 0; k < nchild2; k++) {
     child_image1[k] = new double[nx2 *ny2];
     for(i = 0; i < nx2 * ny2; i++) child_image1[k][i] = 0.;
     }
 }

// Copy to private parameters:
nchild1 = nchild2;
nx1 = nx2;
ny1 = ny2;
nz1 = nz2;

return;
}
/****************************************************************************
* Called when the thread exits - whether it terminates normally or is
* stopped with Delete() (but not when it is Kill()ed!)
*
****************************************************************************/
void JLP_DecodeThread::OnExit()
{
}

/****************************************************************************
* Thread execution starts here
*
****************************************************************************/
wxThread::ExitCode JLP_DecodeThread::Entry()
{
#ifdef DEBUG
long iloop = 0;
wxString buffer;
#endif

  while( !TestDestroy()) {
#ifdef DEBUG
// Display immediately
 after testing if it is still alive !
    buffer.Printf(_T("JLP_DecodeThread (iloop = %ld) cube_to_be_processed=%d."),
                  iloop, (int)cube_to_be_processed);
    wxLogMessage(buffer);
    iloop++;
#endif
// Sleep or work...
    if(cube_to_be_processed) {
      if(erase_all) {
        Decode_EraseAll();
        } else {
        Decode_ProcessCube();
        }
      } else {
      Sleep(1000);
      }
  }

return NULL;  // Success
}
/****************************************************************************
* Start processing the cube
*
****************************************************************************/
void JLP_DecodeThread::StartProcessingCube(AWORD *ImageCube, const int nx0,
                                           const int ny0, const int nz0)
{
int status;

  status = jlp_decode1->DC_LoadAndorCube(ImageCube, nx0, ny0, nz0, icube1);
  if(status) {
    wxMessageBox(wxT("Fatal error loading ImageCube to Decode1!"),
                 wxT("JLP_DecodeThread::StartprocessingCube"), wxOK | wxICON_ERROR);
    exit(-1);
    }

cube_to_be_processed = true;
}
/****************************************************************************
* Process cube
*
****************************************************************************/
void JLP_DecodeThread::Decode_ProcessCube()
{
int status, k, k_decode_index;
wxString buffer, messg_for_status_bar;
wxThreadEvent *CubeIsProcessedEvent;

// Put it here, to avoid problems in case of errors in Decode1->ProcessCube
cube_to_be_processed = false;

// Process the digitized frames stored in CubeImage.
 status = jlp_decode1->ProcessCube(icube1);
 if(status) {
  buffer.Printf(wxT("ProcessCube:Error status = %d"), status);
  wxMessageBox(buffer, wxT("DecodeThread"), wxOK | wxICON_ERROR);
  return;
  }

#ifdef OLD_GUI_VERSION
// Update Screen
// Refresh autocorrelation, modsq, quadrant and long integration:
 jlp_display_panel1->UpdateChildWindows(jlp_decode1, -1);

//**************************************************************************
// inform the GUI toolkit that we're going to use GUI functions
// from a secondary thread:
 wxMutexGuiEnter();
  jlp_display_panel1->RefreshDisplay();
// if we don't release the GUI mutex the MyImageDialog won't be able to refresh
 wxMutexGuiLeave();
//*********************************************************************************
#endif

// Get the message to be displayed on top of the screen:
 jlp_decode1->GetMessageForStatusBar(messg_for_status_bar);

// Read the current values of the images from decode:
 for(k = 0; k < nchild1; k++){
  k_decode_index = ipanel_decode_index[k];
  status = jlp_decode1->ReadDbleImage(child_image1[k], nx1, ny1, k_decode_index);
  if(status) {
     fprintf(stderr, "JLP_DecodeThread/Error retrieving data ReadDbleImage (k=%d k_decode_index=%d\n",
             k, k_decode_index);
     return;
    }
 }

// Load those images to DisplayMutex for further display by main program:
  jlp_display2_mutex->WriteImages(child_image1, nx1, ny1, nchild1,
                                  messg_for_status_bar);

// Create new event: (can be used only once, since it is deleted by wxQueueEvent
  CubeIsProcessedEvent = new wxThreadEvent(wxEVT_THREAD, ID_DECODE_THREAD_DONE);

// Send event to AspFrame:
  wxQueueEvent( (wxFrame*)m_aspframe, CubeIsProcessedEvent);

}
/****************************************************************************
* Erase all data in Decode1
*
****************************************************************************/
void JLP_DecodeThread::Decode_EraseAll()
{
int status, k, k_decode_index;
wxString buffer, messg_for_status_bar;

erase_all = false;

// Erase the processed frames stored in CubeImage.
 status = jlp_decode1->EraseProcessCube(0);
 if(status) {
  buffer.Printf(wxT("EraseProcessCube:Error status = %d"), status);
  wxMessageBox(buffer, wxT("DecodeThread"), wxOK | wxICON_ERROR);
  return;
  }

// Read the current values of the images from decode:
 for(k = 0; k < nchild1; k++){
  k_decode_index = ipanel_decode_index[k];
  if(k_decode_index >= 0 && k_decode_index < nchild1)
    jlp_decode1->ReadDbleImage(child_image1[k], nx1, ny1, k_decode_index);
 }

// Get the message to be displayed on top of the screen:
 jlp_decode1->GetMessageForStatusBar(messg_for_status_bar);

// Load those images to DisplayMutex for further display by main program:
  jlp_display2_mutex->WriteImages(child_image1, nx1, ny1, nchild1,
                                  messg_for_status_bar);


}
/****************************************************************************
* Get results (new version : simply ask if processing is finished)
*
****************************************************************************/
int JLP_DecodeThread::GetResults()
{
int status;
wxString buffer;

if(cube_to_be_processed) {
  buffer = wxT("Error can't get results: cube not fully processed yet\n Please wait and try again !\n");
  wxMessageBox(buffer, wxT("JLP_DecodeThread/GetFinalResults"), wxOK | wxICON_ERROR);
  status = -1;
  } else {
  status = 0;
  }

return(status);
}
/****************************************************************************
* Erase all data (when processing is running)
*
****************************************************************************/
int JLP_DecodeThread::EraseAll()
{

erase_all = true;

return(0);
}
