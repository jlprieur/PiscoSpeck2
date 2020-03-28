/*****************************************************************************
* asp2_camera_thread.cpp
*
* JLP_Camera_Thread
*
* JLP version 06/10/2017
*****************************************************************************/
#include <stdio.h>
#include <math.h>

#include "jlp_camera_thread.h"         // JLP_CameraThread()...
#include "asp2_frame_id.h"             // ID_CAMERA_THREAD
#include "jlp_numeric.h"               // SQUARE
#include "jlp_camera_display_mutex.h"       // CameraDisplayMutex

/* defined in jlp_numeric.h now:
#define ABS(a) ((a) < 0) ? (-(a)) : (a)
#define SQUARE(a) ((a)*(a))
#define MINI(a,b) ((a) < (b)) ? (a) : (b)
#define MAXI(a,b) ((a) < (b)) ? (b) : (a)
*/

#ifndef PI
#define PI 3.141592654
#endif

/*
#define DEBUG
*/

/****************************************************************************
* JLP_CameraThread
* Constructor
****************************************************************************/
JLP_CameraThread::JLP_CameraThread(AspFrame *asp_frame,
                                 JLP_CameraDisplayMutex *camera_display_mutex0,
                                 JLP_Camera1 *cam1)
                                 : wxThread(wxTHREAD_DETACHED)
{
JLP_CAMERA_SETTINGS CamSet0;

// Save to private variables:
 m_aspframe = asp_frame;
 m_cam1 = cam1;
 m_CameraDisplayMutex1 = camera_display_mutex0;
 m_cam1->Cam1_Get_CameraSettings(&CamSet0);
 nx1 = CamSet0.nx1;
 ny1 = CamSet0.ny1;
 nz1 = 50;
 nz_total1 = 1000;
 nz_current1 = 0;
 dble_image1 = NULL;
 ImageCube1 = NULL;

 simulation1 = false;
 no_processing1 = true;
// Allocate memory (NEEDED BEFORE CALLING SetupForAcquisition the first time)
 AllocateMemoryForDataCube(nx1, ny1, nz1);

//SetupForAcquisition(int nx0, int ny0, int nz0,
//                      int xc0, int yc0, int xbin0, int ybin0,
//                                           bool simulation0, bool no_processing0)
 SetupForAcquisition(nx1, ny1, nz1, CamSet0.xc0, CamSet0.yc0,
                     CamSet0.xbin, CamSet0.ybin, simulation1, no_processing1);

// Initialize thread to non-active status:
 cube_to_be_loaded = false;
}
/****************************************************************************
* Allocate memory for data cubes
*
* INPUT:
*  nx, ny, nz : size of data cubes
****************************************************************************/
int JLP_CameraThread::AllocateMemoryForDataCube(const int nx, const int ny,
                                               const int nz)
{
wxString buffer;

 if(nx == 0 || ny == 0 || nz == 0) {
   buffer.Printf(wxT("Bad size:nx=%d ny=%d nz=%d"), nx, ny, nz);
   wxMessageBox(buffer, "JLP_CameraThread::AllocateMemoryForDataCube",
                wxOK | wxICON_ERROR);
   return(-1);
   }

// Free memory previously allocated and allocate new memory:
 if(dble_image1 != NULL) delete[] dble_image1;
 dble_image1 = new double[nx * ny];
 if(ImageCube1 != NULL) delete[] ImageCube1;
 ImageCube1 = new AWORD[nx * ny * nz];

return(0);
}
/****************************************************************************
* Setup before new acquisition
*
* INPUT:
*   nx0, ny0 : size of elementary images
*   xc0, yc0 : start location of ROI
*   xbin, ybin : binningg factor
****************************************************************************/
void JLP_CameraThread::SetupForAcquisition(int nx0, int ny0, int nz0,
                                           int xc0, int yc0, int xbin0, int ybin0,
                                           bool simulation0, bool no_processing0)
{
int status;
wxString buffer, error_msg;

// Check if new size is identical to the previous one:
// and allocate memory if size has changed:
 if((nx1 != nx0) || (ny1 != ny0) || (nz1 != nz0)) {
// Allocate memory:
  status = AllocateMemoryForDataCube(nx0, ny0, nz0);
// status = -1 if null size, hence return from here:
   if(status != 0) {
        wxMessageBox(wxT("Error allocating memory"),
                    wxT("JLP_CameraThread::JLP_Camera_InitAcuisition"),
                    wxOK | wxICON_ERROR);
    return;
    }
  }

 simulation1 = simulation0;
 no_processing1 = no_processing0;

// Save new values to private variables:
 nx1 = nx0;
 ny1 = ny0;
 nz1 = nz0;
 nz_total1 = m_cam1->Nz_total();
 nz_current1 = 0;

// Initialize the Camera detector for the selected acquisition mode:
if(simulation1 == false) {
 status = m_cam1->Cam1_InitAcquisition(error_msg);
 if(status) wxMessageBox(error_msg,
                    wxT("JLP_CameraThread::JLP_Camera_InitAcuisition"),
                    wxOK | wxICON_ERROR);

// Set system for continuous acquisition:
 status = m_cam1->Cam1_SetSystemForContinuousAcquisition(nx1, ny1, xc0, yc0, xbin0, ybin0);
 if(status) wxMessageBox(wxT("Error setting the system for  continuous acquisition"),
   wxT("JLP_CameraThread::JLP_Camera_SetSystemForContinuousAcquisition"),
   wxOK | wxICON_ERROR);
}

 cube_to_be_loaded = false;

return;
}
/****************************************************************************
* Called when the thread exits - whether it terminates normally or is
* stopped with Delete() (but not when it is Kill()ed!)
*
****************************************************************************/
void JLP_CameraThread::OnExit()
{
 delete[] ImageCube1;
 ImageCube1 = NULL;
}

/****************************************************************************
* Thread execution starts here
*
****************************************************************************/
wxThread::ExitCode JLP_CameraThread::Entry()
{
wxString error_msg;
int status;

#ifdef DEBUG
long iloop = 0;
wxString buffer;
#endif

  while( !TestDestroy()) {
#ifdef DEBUG
// Display immediately after testing if it is still alive !
    buffer.Printf(_T("JLP_CameraThread (iloop = %ld)."), iloop);
    wxLogMessage(buffer);
    iloop++;
#endif
// Sleep or work...
// JLP_CAMERA_SETTINGS:
// int  acquisitionMode; // 5 for continuous streaming
// Hence it is always the case
    if(cube_to_be_loaded) {
     if(no_processing1){
        LoadCubeForStreaming();
      } else {
        LoadCubeForProcessing();
      }
// Need some time to avoid saturation...
    } else {
      Sleep(10);
    }
  }

// Stop the acquisition if it is still running
 status = m_cam1->Cam1_StopAcquisition(error_msg);
 if(status) wxMessageBox(error_msg,
                         wxT("JLP_CameraThread/JLP_Camera_StopAcquisition"),
                         wxOK | wxICON_ERROR);

return (wxThread::ExitCode)0;  // Success
}
/****************************************************************************
* Start loading the cube for processing
*
****************************************************************************/
int JLP_CameraThread::StartLoadingCubeForProcessing()
{
int status = 10;

 cube_to_be_loaded = false;
 no_processing1 = true;

// NB: nz_total < 0 for ANDOR/RAPTOR cameras
if((nz_total1 < 0) || (nz_current1 + nz1 < nz_total1)) {
 cube_to_be_loaded = true;
 no_processing1 = false;
 status = 0;
 }

return(status);
}
/****************************************************************************
* Start loading the cube for streaming
*
****************************************************************************/
int JLP_CameraThread::StartLoadingCubeForStreaming()
{
int status = 10;

 cube_to_be_loaded = false;
 no_processing1 = true;

// NB: nz_total < 0 for ANDOR/RAPTOR cameras
if((nz_total1 < 0) || (nz_current1 + nz1 < nz_total1)) {
 cube_to_be_loaded = true;
 no_processing1 = true;
 status = 0;
 }

return(status);
}
/****************************************************************************
* Load cube for processing
*
****************************************************************************/
void JLP_CameraThread::LoadCubeForProcessing()
{
int status, stat0, k, nz0;
wxThreadEvent *CubeIsLoadedEvent;
wxString error_msg;

// Starting continuous acquisition:
if(!simulation1) {
status = m_cam1->Cam1_StartContinuousAcquisition();
if(status != 0) {
    wxMessageBox(wxT("Fatal error starting continuous acquisition"),
                 wxT("JLP_CameraThread::Cam1_StartContinuousAcquisition"),
                 wxOK | wxICON_ERROR);
   exit(-1);
   }
}

// Load a cube from the Camera camera in many folds, by slices of 10 images,
// to refresh screen 5 times per second
nz0 = 10;
status = 0;
 for(k = 0; k < nz1; k += nz0) {
// NB: nz_total < 0 for ANDOR/RAPTOR cameras
// Should be done here !
   if(nz_total1 > 0) nz_current1 += nz0;

   if (simulation1) {
// Load cube from simulator
     status = JLP_CameraSimulator(&ImageCube1[k*nx1*ny1], nx1, ny1, nz0);
    } else {
// Load cube from Camera camera:
     status = m_cam1->Cam1_Continuous_GetTheImages(&ImageCube1[k*nx1*ny1],
                                                        nx1, ny1, nz0);
    }

// Load to DisplayMutex the first image of the sequence
   if(status == 0 || status == 10)
      m_CameraDisplayMutex1->WriteImage(&ImageCube1[k*nx1*nx1], nx1, ny1);

// If error, exit from loop here:
  if(status != 0) {
        break;
  }
// Sleep for 100 msec
  Sleep(100);

  }

// Stop the acquisition since cube has been loaded
 if(!simulation1){
 stat0 = m_cam1->Cam1_StopAcquisition(error_msg);
 if(stat0) wxMessageBox(error_msg,
              wxT("JLP_CameraThread/JLP_Camera_StopAcquisition"),
              wxOK | wxICON_ERROR);
 }

// If error, return without sending an event:
 if(status == 0 || status == 10) {
// Create new event: can be used only once, since it is deleted by wxQueueEvent
  CubeIsLoadedEvent = new wxThreadEvent(wxEVT_THREAD, ID_CAMERA_THREAD_DONE);
// Send event to AspFrame:
  wxQueueEvent( (wxFrame*)m_aspframe, CubeIsLoadedEvent);
 }

cube_to_be_loaded = false;

return;
}
/****************************************************************************
* Load cube for streaming
*
****************************************************************************/
void JLP_CameraThread::LoadCubeForStreaming()
{
int status, stat0, k, nz0;
wxThreadEvent *StreamingEvent;
wxString error_msg;

// Starting continuous acquisition:
if(!simulation1) {
status = m_cam1->Cam1_StartContinuousAcquisition();
if(status != 0) {
    wxMessageBox(wxT("Fatal error starting continuous acquisition"),
                 wxT("JLP_CameraThread::Cam1_StartContinuousAcquisition"),
                 wxOK | wxICON_ERROR);
   exit(-1);
   }
}

// Streaming from the Camera camera by 10 sequences of 1 image
// and sleep for 100 msec (to display about 10 images/second)
nz0 = 1;
// 1 sec = 10 times (load one big image + sleep 100 msec)
 for(k = 0; k < 10; k += nz0) {
// NB: nz_total < 0 for ANDOR/RAPTOR cameras
// Should be done here !
   if(nz_total1 > 0) nz_current1 += nz0;

   if (simulation1) {
// Load image from simulator
     status = JLP_CameraSimulator(ImageCube1, nx1, ny1, nz0);
    } else {
// Load image from Camera camera:
     status = m_cam1->Cam1_Continuous_GetTheImages(ImageCube1, nx1, ny1, nz0);
    }
// Load to DisplayMutex the first image of the sequence
   if(status == 0 || status == 10)
     m_CameraDisplayMutex1->WriteImage(ImageCube1, nx1, ny1);

// If error, or all data has been read (for fistcubes) , stop here and return:
  if(status) break;

// Sleep for 100 msec
  Sleep(100);

  }

// Stop the acquisition since cube has been loaded
if(!simulation1) {
 stat0 = m_cam1->Cam1_StopAcquisition(error_msg);
 if(stat0) wxMessageBox(error_msg,
              wxT("JLP_CameraThread/JLP_Camera_StopAcquisition"),
              wxOK | wxICON_ERROR);
}

// Create new event: can be used only once, since it is deleted by wxQueueEvent
  StreamingEvent = new wxThreadEvent(wxEVT_THREAD, ID_CAMERA_THREAD_STREAM);
// Send event to AspFrame (to allow the user to stop streaming):
  wxQueueEvent( (wxFrame*)m_aspframe, StreamingEvent);


cube_to_be_loaded = false;

}
/****************************************************************************
* Retrieve results
*
****************************************************************************/
int JLP_CameraThread::GetResults(AWORD *ImageCube0, const int nx0,
                                const int ny0, const int nz0)
{
wxString buffer;
int i;

if(nx0 != nx1 || ny0 != ny1 || nz0 != nz1) {
  buffer.Printf(wxT("Fatal error incompatible size: (required: nx0=%d ny0=%d) available: nx1=%d ny1=%d\n"),
                nx0, ny0, nx1, ny1),
  wxMessageBox(buffer, wxT("JLP_CameraThread::GetResults"), wxOK | wxICON_ERROR);
  exit(-1);
  }


if(cube_to_be_loaded) {
  wxLogError(wxT("JLP_CameraThread/Warning: can't get results: cube not fully loaded yet !\n"));
  return(-1);
  }

for(i = 0; i < nx0 * ny0 * nz0; i++) ImageCube0[i] = ImageCube1[i];

return(0);
}
