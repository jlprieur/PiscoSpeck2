/*****************************************************************************
* jlp_camera_thread.h
*
* JLP_CameraThread
*
* JLP version 06/09/2015
*
*****************************************************************************/
#ifndef _jlp_camera_thread_h_
#define _jlp_camera_thread_h_

// ---------------------------------------------------------------------------
// Headers
// ---------------------------------------------------------------------------

#include "wx/wx.h"
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"

#include "jlp_camera1.h"     // JLP_Camera1 class
#include "jlp_camera_utils.h"    // JLP_CAMERA_SETTINGS
#include "jlp_camera_display.h" // JLP_CameraDisplay(),...

class JLP_CameraDisplayMutex;

class AspFrame;

// ----------------------------------------------------------------------------
// Decode thread (simple)
// ----------------------------------------------------------------------------

class JLP_CameraThread : public wxThread
{
public:
  JLP_CameraThread(AspFrame *frame, JLP_CameraDisplayMutex *camera_display_mutex0,
                   JLP_Camera1 *cam1);

// Destructor, in "PiscoSpeck2.cpp" since it uses wxGetApp()
  virtual ~JLP_CameraThread();

// Thread execution starts here
  virtual wxThread::ExitCode Entry();
  void SetupForAcquisition(int nx0, int ny0, int nz0, int xc0, int yc0,
                           int xbin, int ybin, bool simulation0,
                           bool no_processing0);
  int AllocateMemoryForDataCube(const int nx, const int ny, const int nz);

// Cube loading:
  void LoadCubeForStreaming();
  void LoadCubeForProcessing();
  int StartLoadingCubeForProcessing();
  int StartLoadingCubeForStreaming();
  int GetResults(AWORD *ImageCube0, const int nx10, const int ny10,
                 const int nz10);

// "jlp_camera_thread_display.cpp"
  void DisplayImageToScreen(AWORD *image0, const int nx0, const int ny0);
  int JLP_CameraSimulator(AWORD *ImageCube0, const int nx0,
                          const int ny0, const int nz0);

// Called when the thread exits - whether it terminates normally or is
// stopped with Delete() (but not when it is Kill()ed!)
  virtual void OnExit();

// Accessors:
  int Nx(){return(nx1);};
  int Ny(){return(ny1);};
  int Nz(){return(nz1);};
  long Nz_total(){return(nz_total1);};
  int IsLoading(){return(cube_to_be_loaded);};

private:
  AspFrame *m_aspframe;
  JLP_Camera1 *m_cam1;
  JLP_CameraDisplayMutex *m_CameraDisplayMutex1;
  AWORD *ImageCube1;
  double *dble_image1;
  bool cube_to_be_loaded, simulation1, no_processing1;
  int nx1, ny1, nz1;
  long nz_total1, nz_current1;

};

#endif // EOF sentry
