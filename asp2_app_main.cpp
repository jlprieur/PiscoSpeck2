/****************************************************************************
* Asp2_app_main.cpp
* Application project for AndorSpeck2.cpp/PiscoSpeck2.cpp (AspFrame class)
* Purpose: :
*   - mode FitsCube: display and process FITS images obtained with PISCO and PISCO2
*   - mode Andor: display and process images obtained with ANDOR cameras
*   - mode Raptor: display and process images obtained with RAPTOR cameras
*
* JLP
* Version 05/09/2015
****************************************************************************/
#include <stdlib.h>   // exit()

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #define USE_XPM
#endif

#ifdef USE_XPM
    #include "mondrian.xpm"
#endif

#include "asp2_app.h"
#include "asp2_frame.h"    // AspFrame class (includes also the threads)

// Create a new application object: (to be able to use wxGetApp())
IMPLEMENT_APP(Asp2_App)

/****************************************************************************
* Constructor:
****************************************************************************/
Asp2_App::Asp2_App()
{
  m_andor_shuttingDown = false;
  m_decode_shuttingDown = false;
  m_3dfits_shuttingDown = false;
}

/****************************************************************************
* Setup (called when opening application)
****************************************************************************/
bool Asp2_App::OnInit()
{
// Transform coma into point for numbers:
setlocale(LC_NUMERIC, "C");

// use standard command line handling:
    if ( !wxApp::OnInit() )
        return false;

// Uncomment this to get some debugging messages from the trace code
// on the console (or just set WXTRACE env variable to include "thread")
    wxLog::AddTraceMask("thread");

// parse the cmd line
    int GdpWidth = 300, GdpHeight = 300;
    if ( argc == 3 )
    {
        wxSscanf(wxString(argv[1]), wxT("%d"), &GdpWidth);
        wxSscanf(wxString(argv[2]), wxT("%d"), &GdpHeight);
    }

#if wxUSE_HELP
    wxHelpProvider::Set( new wxSimpleHelpProvider );
#endif // wxUSE_HELP

// Create the main frame window
    AspFrame *frame = new AspFrame(_T("PiscoSpeck2"), GdpWidth, GdpHeight);

// Give it an icon
// The wxICON() macros loads an icon from a resource under Windows
// and uses an #included XPM image under GTK+ and Motif
#ifdef USE_XPM
    frame->SetIcon( wxICON(mondrian) );
#endif

    frame->Show(true);

    return true;
}
/**********************************************************************
* AspFrame: stop the threads
*
* it's better to do any thread cleanup in the OnClose()
* event handler, rather than in the destructor.
* This is because the event loop for a top-level window is not
* active anymore when its destructor is called and if the thread
* sends events when ending, they won't be processed unless
* you ended the thread from OnClose.

* I put it here to be able to use wxGetApp()
***********************************************************************/
 void AspFrame::DeleteAllThreads()
{

wxLog::SetActiveTarget(m_oldLogger);

// Erase all frames and save processing results if needed:
  EraseAllAfterProcessing();


// Stop threads:
// NB: although the OS will terminate all the threads anyhow when the main
//     one exits, it's good practice to do it ourselves

// Tell all the threads to terminate: note that they can't terminate while
// We're deleting them because they will block in their OnExit() -- this is
// important as otherwise we might access invalid array elements

{
  wxCriticalSectionLocker locker(wxGetApp().m_critsect);

// check if we have any threads running and
// set the flag indicating that each thread should exit

  if(m_camera_thread != NULL) {
    if(m_camera_thread->IsPaused()) m_camera_thread->Resume();
    wxGetApp().m_andor_shuttingDown = true;
  }
  if(m_decode_thread != NULL) {
    if(m_decode_thread->IsPaused())m_decode_thread->Resume();
    wxGetApp().m_decode_shuttingDown = true;
  }


// now wait for them to really terminate
// NOT GOOD: can be stalled here !
/*
  if(m_camera_thread != NULL) wxGetApp().m_sem_camera_done.Wait();
  if(m_decode_thread != NULL) wxGetApp().m_sem_decode_done.Wait();
*/

// Attempt to delete them if they are still alive:
 if(m_decode_thread != NULL) {
   m_decode_thread->Delete();
   m_decode_thread = NULL;
   }
 if(m_camera_thread != NULL) {
   m_camera_thread->Delete();
   m_camera_thread = NULL;
   }
}  // EOF critical section

// Sleep 1 second to wait for stopping the threads
wxSleep(1);
return;
}
/****************************************************************************
* JLP_DecodeThread: destructor
*
****************************************************************************/
JLP_DecodeThread::~JLP_DecodeThread()
{
  wxCriticalSectionLocker locker(wxGetApp().m_critsect);

  if(wxGetApp().m_decode_shuttingDown) wxGetApp().m_decode_shuttingDown = false;

  wxGetApp().m_sem_decode_done.Post();
}
/****************************************************************************
* JLP_CameraThread: destructor
*
****************************************************************************/
JLP_CameraThread::~JLP_CameraThread()
{
  wxCriticalSectionLocker locker(wxGetApp().m_critsect);

  if(wxGetApp().m_andor_shuttingDown) wxGetApp().m_andor_shuttingDown = false;

  wxGetApp().m_sem_andor_done.Post();
}
