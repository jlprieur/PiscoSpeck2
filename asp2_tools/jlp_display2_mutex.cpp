/**************************************************************************
* jlp_display2_mutex.h
*
* Routines to handle display2 mutex
* Purpose: dialog between the main process (read for display)
*          and Decode thread (write after processing)
* JLP
* Version 03/02/2016
**************************************************************************/
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/thread.h" // also for wxMutex

#include <stdlib.h>
#include <stdio.h>
#include "jlp_display2_mutex.h"

/**************************************************************************
* Constructor
* Mutex for handling display of short exposures:
*
**************************************************************************/
JLP_Display2Mutex::JLP_Display2Mutex(const int nx, const int ny,
                                     const int nchild)
{
int i, k;

 hDisplay2Mutex = new wxMutex();

 if(hDisplay2Mutex == NULL) {
   wxMessageBox(wxT("Error creating mutex"), wxT("CreateDisplay2Mutex"),
                wxICON_ERROR);
   }

 if(nx > 0 && ny > 0 && nchild > 0) {
   nx2 = nx;
   ny2 = ny;
   nchild2 = nchild;
 } else {
  wxMessageBox(wxT("Fatal error: bad parameters"),
               wxT("JLP_Displa2Mutex"), wxICON_ERROR);
 }

 for(k = 0; k < nchild2; k++) {
   child_image2[k] = new double[nx2 *ny2];
   for(i = 0; i < nx2 * ny2; i++) child_image2[k][i] = 0.;
 }

// Initialize the message to be displayed on the status bar:
 message2 = wxT("");

return;
}
/**************************************************************************
* Destructor
**************************************************************************/
JLP_Display2Mutex::~JLP_Display2Mutex()
{
int k;

 if(hDisplay2Mutex != NULL) {
   delete hDisplay2Mutex;
   hDisplay2Mutex = NULL;
   }

 for(k = 0; k < nchild2; k++) {
  if(child_image2[k] != NULL) delete[] child_image2[k];
  }

return;
}
/**************************************************************************
* Routine to access the StopStreaming Mutex and read the current status:
**************************************************************************/
int JLP_Display2Mutex::ReadImages(double *child_image0[DECODE_NAME_MAX],
                                  const int nx0, const int ny0,
                                  const int nchild0,
                                  wxString& messg_for_status_bar)

{
wxString buffer;
int status, i, k;
wxMutexError mutex_status;

 if(hDisplay2Mutex == NULL) return(-1);

// timeout set to 100 msec:
 mutex_status=hDisplay2Mutex->LockTimeout(100);
 switch (mutex_status) {
   case wxMUTEX_NO_ERROR:
     status = 0;
     break;
   default:
   case wxMUTEX_DEAD_LOCK:
   case wxMUTEX_TIMEOUT:
     status = -1;
     buffer.Printf(wxT("hDisplay2Mutex->LockTimeout(100) returned %d"),
             (int)mutex_status);
     wxMessageBox(buffer, wxT("Read_Display2Mutex"), wxICON_ERROR);
     break;
  }

// Now access to image_values and unlock the mutex:
 if(status == 0) {
  if((nchild0 <= nchild2) && (nx0 * ny0 <= nx2 * ny2)) {
    for(k = 0; k < nchild0; k++) {
      for(i = 0; i < nx0 * ny0; i++) child_image0[k][i] = child_image2[k][i];
      }
// Load the current value of meassage2 to output status bar message for further display
    messg_for_status_bar = message2;
    } else {
    fprintf(stderr, "JLP_Display2Mutex/ReadImages/error incompatible size: nx0=%d ny0=%d nx2=%d iny2=%d nchild0=%d nchild2=%d\n",
            nx0, ny0, nx2, ny2, nchild0, nchild2);
    }
  hDisplay2Mutex->Unlock();
  status = 0;
 }

return(status);
}
/**************************************************************************
* Routine to access the StopStreaming Mutex and modify the current status:
**************************************************************************/
int JLP_Display2Mutex::WriteImages(double *child_image0[DECODE_NAME_MAX],
                                   const int nx0, const int ny0,
                                   const int nchild0,
                                   wxString messg_for_status_bar)
{
wxString buffer;
int status, i, k;
wxMutexError mutex_status;

 if(hDisplay2Mutex == NULL) return(-1);

// timeout set to 100 msec:
 mutex_status=hDisplay2Mutex->LockTimeout(100);
 switch (mutex_status) {
   case wxMUTEX_NO_ERROR:
     status = 0;
     break;
   default:
   case wxMUTEX_DEAD_LOCK:
   case wxMUTEX_TIMEOUT:
     status = -1;
     buffer.Printf(wxT("hDisplay2Mutex->LockTimeout(100) returned %d"),
             (int)mutex_status);
     wxMessageBox(buffer, wxT("Write_Display2Mutex"), wxICON_ERROR);
     break;
  }

// Now access to image_values and unlock the mutex:
 if(status == 0) {
  if((nchild0 <= nchild2) && (nx0 * ny0 <= nx2 * ny2)) {
    for(k = 0; k < nchild0; k++) {
      for(i = 0; i < nx0 * ny0; i++) child_image2[k][i] = child_image0[k][i];
      }
    } else {
    fprintf(stderr, "JLP_Display2Mutex/WriteImages/error incompatible size: nx0=%d ny0=%d nx2=%d iny2=%d nchild0=%d nchild2=%d\n",
            nx0, ny0, nx2, ny2, nchild0, nchild2);
    }

// Save status bar message to private variable:
  message2 = messg_for_status_bar;
  hDisplay2Mutex->Unlock();
  status = 0;
 }

return(status);
}
