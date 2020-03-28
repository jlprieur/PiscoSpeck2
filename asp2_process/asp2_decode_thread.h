/*****************************************************************************
* asp2_decode_thread.h
*
* JLP_DecodeThread
*
* JLP version 16/08/2015
*****************************************************************************/
#ifndef _asp2_decode_thread_h_
#define _asp2_decode_thread_h_

// ---------------------------------------------------------------------------
// Headers
// ---------------------------------------------------------------------------

#include "wx/wx.h"
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"

#include "jlp_decode.h"        // JLP_Decode class

class AspFrame;
class JLP_AspDispPanel;
class JLP_Display2Mutex;

// ----------------------------------------------------------------------------
// Decode thread
// ----------------------------------------------------------------------------

class JLP_DecodeThread : public wxThread
{
public:
  JLP_DecodeThread(AspFrame *frame, JLP_AspDispPanel *jlp_display_panel0,
                   JLP_Display2Mutex *jlp_display2_mutex0);

// Destructor, in "PiscoSpeck2.cpp" since it uses wxGetApp()
  virtual ~JLP_DecodeThread();

// Thread execution starts here
  virtual wxThread::ExitCode Entry();

// Processing:
  void Setup(JLP_Decode *jlp_decode0);
  void Decode_ProcessCube();
  void Decode_EraseAll();
  void StartProcessingCube(AWORD *ImageCube, const int nx0,
                           const int ny0, const int nz0);
  int GetResults();
  int EraseAll();
  bool IsProcessing(){return(cube_to_be_processed);};

// Called when the thread exits - whether it terminates normally or is
// stopped with Delete() (but not when it is Kill()ed!)
  virtual void OnExit();

private:
  AspFrame *m_aspframe;
  JLP_AspDispPanel *jlp_display_panel1;
  JLP_Display2Mutex *jlp_display2_mutex;
  JLP_Decode *jlp_decode1;
  double *child_image1[NCHILDS_MAX];
  int nchild1, ipanel_decode_index[NCHILDS_MAX];
  bool cube_to_be_processed, erase_all;
  int icube1, nx1, ny1, nz1;

};

#endif // EOF sentry
