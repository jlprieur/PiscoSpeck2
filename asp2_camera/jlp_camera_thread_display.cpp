/*****************************************************************************
* jlp_camera_thread_display.cpp
*
* JLP_Camera_Thread
*
* JLP version 08/10/2017
*****************************************************************************/
#include <stdio.h>
#include <math.h>

#include "jlp_camera_thread.h"     // JLP_CameraThread()...
#include "asp2_frame_id.h"         // ID_ANDOR_THREAD

#define DEBUG

#ifdef OLD_VERSION
/*************************************************************************
* DisplayImageToScreen
*
* Note : under GTK, no creation of top-level windows is allowed
* in any thread but the main one.
* This is why I created the Andor display frame from the main thread
*************************************************************************/
void JLP_CameraThread::DisplayImageToScreen(AWORD *image0, const int nx0,
                                           const int ny0)
{
wxString buffer;
int i;

if((nx0 != nx1) || (ny0 != ny1)) {
  buffer.Printf(wxT("Fatal error: Incompatible size\n nx0,ny0=%d,%d and nx1,ny1=%d,%d"),
                nx0, ny0, nx1, ny1);
  wxMessageBox(buffer, wxT("AndorThread/DisplayimageToScreen"),
                wxOK | wxICON_ERROR);
  exit(-1);
}

 for(i = 0; i < nx0*ny0; i++) dble_image1[i] = (double)image0[i];

// inform the GUI toolkit that we're going to use GUI functions
// from a secondary thread:
 wxMutexGuiEnter();

  JLP_CameraDisplay1->LoadImage(dble_image1, nx0, ny0);

// if we don't release the GUI mutex the MyImageDialog won't be able to refresh
  wxMutexGuiLeave();

return;
}
#endif
/*************************************************************************
* Simulation of Camera images with two stars
* (two Gaussian functions in the image)
*************************************************************************/
int JLP_CameraThread::JLP_CameraSimulator(AWORD *ImageCube0, const int nx0,
                                         const int ny0, const int nz0)
{
register int k, i, j;
int ic1, jc1, ic2, jc2, ivalue;
double ww, x_shift, y_shift, rho_new, theta_new, rho_old, theta_old;
double rho, theta, dble_value;


ic1 = nx0/2 - 10;
jc1 = ny0/2 - 10;
ic2 = nx0/2 + 10;
jc2 = ny0/2 + 10;
rho_new = (double)rand() / (double)RAND_MAX;
theta_new = (double)rand() / (double)RAND_MAX;
ww = 0.;
rho_old = rho_new;   // Just in case of a bad initialization in k loop...
theta_old = theta_new;
  for(k = 0; k < nz0; k++) {
    if(((10 * k )% 10) == 0) {
      ww = 0.;
      rho_old = rho_new;
      theta_old = theta_new;
      rho_new = (double)rand() / (double)RAND_MAX;
      theta_new = (double)rand() / (double)RAND_MAX;
    }
    ww += 0.1;
    rho = 30. * (rho_old * (1. - ww) + rho_new * ww) ;
    theta = 2. * PI * (theta_old * (1. - ww) + theta_new * ww);
    x_shift = rho * cos(theta);
    y_shift = rho * sin(theta);
    for(j = 0; j < ny0; j++) {
      for(i = 0; i < nx0; i++) {
        dble_value = 50. * (double)rand()/(double)RAND_MAX
                 + 100. * exp(-((double)SQUARE(i - (ic1 + x_shift)) +
                              (double)SQUARE(j - (jc1 + y_shift))) / 64.)
                 + 50. * exp(-((double)SQUARE(i - (ic2 + x_shift)) +
                              (double)SQUARE(j - (jc2 + y_shift))) / 64.);
        ivalue = (int)dble_value;
//        dble_image1[i + j * nx0] = dble_value;
        ImageCube0[i + j * nx0 + k * nx0 * ny0] = (AWORD)ivalue;
        }
     }
   }

return(0);
}

