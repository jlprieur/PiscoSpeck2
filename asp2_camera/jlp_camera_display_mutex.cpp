/**************************************************************************
* jlp_camera_display_mutex.h
*
* Routines to handle display1 mutex
* Purpose: dialog between the main process (read for display)
*          and camera thread (write after acquisition)
* JLP
* Version 14/01/2016
**************************************************************************/
//
//***************************************************************************
// Mutexes are global objects that can be accessed by other processes
// When two processes open a mutex with the same name, they refer to the
// same mutex. This allows multiple processes to be synchronized.
// Create mutex to stop streaming:
//  hStopStreamingMutex = CreateMutex(NULL,      // No security attributes
//                                    FALSE,     // Initially not owned
//                                    "StopStreamingMutex");   // Name
// Once created the mutex can be accessed by (wait time in milliseconds)
//  if(WaitForSingleObject(hStopStreamingMutex, 10000) == WAIT_TIMEOUT)
//     {
// When access is ended, release it to other threads with
//     ReleaseMutex(hStopStreamingMutex);
//     }
//***************************************************************************

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/thread.h" // also for wxMutex

#include <stdlib.h>
#include <stdio.h>
#include "jlp_camera_display_mutex.h"

/**************************************************************************
* Constructor
* Mutex for handling display of short exposures:
*
**************************************************************************/
JLP_CameraDisplayMutex::JLP_CameraDisplayMutex(const int nx, const int ny)
{
int i;

 image2 = NULL;

 hCameraDisplayMutex = new wxMutex();

 if(hCameraDisplayMutex == NULL) {
   wxMessageBox(wxT("Error creating mutex"), wxT("CreateCameraDisplayMutex"),
                wxICON_ERROR);
   }

 nx2 = nx;
 ny2 = ny;
 image2 = new double[nx2 *ny2];
 for(i = 0; i < nx2 * ny2; i++) image2[i] = 0.;

return;
}
/**************************************************************************
* Destructor
**************************************************************************/
JLP_CameraDisplayMutex::~JLP_CameraDisplayMutex()
{

 if(hCameraDisplayMutex != NULL) {
   delete hCameraDisplayMutex;
   hCameraDisplayMutex = NULL;
   }

 if(image2 != NULL) delete[] image2;

return;
}
/**************************************************************************
* Routine to access the StopStreaming Mutex and read the current status:
**************************************************************************/
int JLP_CameraDisplayMutex::ReadImage(double *short_exposure, const int nx0, const int ny0)
{
wxString buffer;
int status, i;
wxMutexError mutex_status;

 if(hCameraDisplayMutex == NULL) return(-1);

// timeout set to 100 msec:
 mutex_status=hCameraDisplayMutex->LockTimeout(100);
 switch (mutex_status) {
   case wxMUTEX_NO_ERROR:
     status = 0;
     break;
   default:
   case wxMUTEX_DEAD_LOCK:
   case wxMUTEX_TIMEOUT:
     status = -1;
     buffer.Printf(wxT("hCameraDisplayMutex->LockTimeout(100) returned %d"),
             (int)mutex_status);
     wxMessageBox(buffer, wxT("Read_CameraDisplayMutex"), wxICON_ERROR);
     break;
  }

// Now access to image_values and unlock the mutex:
 if(status == 0) {
    if(nx0 * ny0 <= nx2 * ny2) {
      for(i = 0; i < nx0 * ny0; i++) short_exposure[i] = image2[i];
      }
    hCameraDisplayMutex->Unlock();
    status = 0;
 }

return(status);
}
/**************************************************************************
* Routine to access the StopStreaming Mutex and modify the current status:
**************************************************************************/
int JLP_CameraDisplayMutex::WriteImage(AWORD *short_exposure, const int nx0, const int ny0)
{
wxString buffer;
int status, i;
wxMutexError mutex_status;

 if(hCameraDisplayMutex == NULL) return(-1);

// timeout set to 100 msec:
 mutex_status=hCameraDisplayMutex->LockTimeout(100);
 switch (mutex_status) {
   case wxMUTEX_NO_ERROR:
     status = 0;
     break;
   default:
   case wxMUTEX_DEAD_LOCK:
   case wxMUTEX_TIMEOUT:
     status = -1;
     buffer.Printf(wxT("hCameraDisplayMutex->LockTimeout(100) returned %d"),
             (int)mutex_status);
     wxMessageBox(buffer, wxT("Write_CameraDisplayMutex"), wxICON_ERROR);
     break;
  }

// Now access to image_values and unlock the mutex:
 if(status == 0) {
    if(nx0 * ny0 <= nx2 * ny2) {
      for(i = 0; i < nx0 * ny0; i++) image2[i] = (double)short_exposure[i];
      }
    hCameraDisplayMutex->Unlock();
    status = 0;
 }

return(status);
}
