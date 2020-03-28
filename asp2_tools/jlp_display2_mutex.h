/**************************************************************************
* jlp_display2_mutex.h
*
* Class defining to handle display2 mutex
* Purpose: dialog between the main process (read for display)
*          and Decode thread (write after processing)
* JLP
* Version 17/01/2016
**************************************************************************/
#ifndef jlp_display2_mutex_h
#define jlp_display2_mutex_h

#include "asp2_defs.h"    // NCHILDS_MAX

class JLP_Display2Mutex
{
public:
  JLP_Display2Mutex(const int nx, const int ny, const int nchild);
  ~JLP_Display2Mutex();
  int ReadImages(double *child_image0[NCHILDS_MAX], const int nx0,
                 const int ny0, const int nchild0,
                 wxString& messg_for_status_bar);
  int WriteImages(double *child_image0[NCHILDS_MAX], const int nx0,
                 const int ny0, const int nchild0,
                 wxString messg_for_status_bar);

private:
  wxMutex *hDisplay2Mutex;
  double *child_image2[NCHILDS_MAX];
  int nx2, ny2, nchild2;
  wxString message2;
};
#endif
