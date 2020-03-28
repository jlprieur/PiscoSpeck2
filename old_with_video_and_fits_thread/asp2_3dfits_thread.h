/*****************************************************************************
* asp2_3dfits_thread.h
*
* JLP_3DFitsThread
*
* JLP version 06/09/2015
*
*****************************************************************************/
#ifndef _asp2_3dfits_thread_h_
#define _asp2_3dfits_thread_h_

// ---------------------------------------------------------------------------
// Headers
// ---------------------------------------------------------------------------

#include "wx/wx.h"
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/filename.h"    //wxFileName class

#include "asp2_3D_infits.h"   // JLP_3D_inFITSFile class

class AspFrame;

// ----------------------------------------------------------------------------
// Decode thread (simple)
// ----------------------------------------------------------------------------

class JLP_3DFitsThread : public wxThread
{
public:
  JLP_3DFitsThread(AspFrame *frame);

// Destructor, in "PiscoSpeck2.cpp" since it uses wxGetApp()
  virtual ~JLP_3DFitsThread();

// Thread execution starts here
  virtual wxThread::ExitCode Entry();

  int Setup(char *inFITS_filename0, int *nx0, int *ny0, const int nz0);

// Cube loading:
  void LoadCube();
  void StartLoadingCubeForProcessing();
  int GetResults(AWORD *ImageCube0, const int nx0, const int ny0,
                 const int nz0);

// Called when the thread exits - whether it terminates normally or is
// stopped with Delete() (but not when it is Kill()ed!)
  virtual void OnExit();

// Accessors:
  int Nx(){return(nx1);};
  int Ny(){return(ny1);};
  int Nz(){return(nz1);};
  int Nz_total(){return(nz_total);};
  int IsLoading(){return(cube_to_be_loaded);};

private:
  AspFrame *m_aspframe;
  char filename1[128];

  JLP_3D_inFITSFile *inFITSFile1;
  AWORD *ImageCube1;
  bool cube_to_be_loaded;
  int nx1, ny1, nz1, nz_read, nz_total;

};

#endif // EOF sentry
