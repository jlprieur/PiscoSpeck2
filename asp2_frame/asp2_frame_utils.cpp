/****************************************************************************
* Name: asp_frame_utils.cpp
* AspFrame class
*
* JLP
* Version 08/10/2017
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "asp2_frame.h"
#include "asp2_frame_id.h" // Definition of identifiers
#include "jlp_camera_display.h"
#include "jlp_camera_display_mutex.h"
#include "jlp_display2_mutex.h"

/******************************************************************
* Update nframes in Message Bar
*
*******************************************************************/
void AspFrame::Main_UpdateMessageBar(JLP_Decode *Decod2)
{
wxString str1;

// Return  if Decod2 is not yet initialized:
if(!Decod2) return;

Decod2->GetMessageForStatusBar(str1);

// Refresh Message in tool bar:
DispStatic1->SetLabel(str1);

return;
}
/****************************************************************
* Handle timer events, for displaying short exposures
* (called every 100 milliseconds )
*****************************************************************/
void AspFrame::OnCameraDisplay1Timer(wxTimerEvent &WXUNUSED(event))
{
double value_maxi, value_saturation, sensor_temp0, box_temp0;
wxString buffer, buffer1;
JLP_CAMERA_SETTINGS CamSet0;

if((initialized != 1234) || (m_cam1 == NULL) || (m_CameraDisplayMutex == NULL)
   || (m_CameraDisplay == NULL) || (m_camera_thread == NULL)) return;

buffer = wxT("");

if((new_data_is_needed == false) && (m_camera_thread->IsLoading() == false)) {
// Checking cooler temperature (once every 2000 milliseconds )
 if(cooler_counter == 0) {

  m_cam1->Cam1_GetTemperature(&sensor_temp0, &box_temp0, buffer1);
  m_CameraDisplay->UpdateCurrentTemperature(buffer1);

// Mirror:
// MirrorImage(&CubeImage[iz_nx_ny],nx1,ny1, Aset1.MirrorMode);
  m_cam1->Cam1_Get_CameraSettings(&CamSet0);
 if(CamSet0.MirrorMode != 0) {
   buffer1.Printf(wxT("mirror=%d, "), CamSet0.MirrorMode);
   buffer.Append(buffer1);
   }

// Max of image:
   value_maxi = m_CameraDisplay->Get_Max_Value();
   value_saturation = m_cam1->Get_Saturation_Value();
   buffer1.Printf(wxT("maxi=%.0f, satur=%.0f"), value_maxi, value_saturation);
   buffer.Append(buffer1);

// Remove trailing blanks:
 buffer.Trim();
// Display the mirror mode and the max of the image on top of the screen:
 m_CameraDisplay->DisplayImageInfoOnTop(buffer);

/*
// Write to status bar:
 wxFont font1;
 font1 = m_StatusBar->GetFont();
 font1.SetPointSize(8);
 font1.SetWeight(wxFONTWEIGHT_BOLD);
 m_StatusBar->SetFont(font1);
 m_StatusBar->SetStatusText(buffer, 0);
*/    }
    cooler_counter++;
    if(cooler_counter > cooler_counter_max) cooler_counter = 0;
// m_andor_thread->IsLoading() is true:
  } else {
   if(short_exposure1 == NULL) short_exposure1 = new double[nx1 * ny1];
   m_CameraDisplayMutex->ReadImage(short_exposure1, nx1, ny1);

// Display short exposure image:
   m_CameraDisplay->CameraDisplay_LoadImage(short_exposure1, nx1, ny1);
  }

return;
}
/****************************************************************
* Display processing results
* (called by "OnReceiveDoneEventFromDecode"
  *when an event is received)
*****************************************************************/
void AspFrame::RefreshDisplayWhenDecodeEvent()
{
int k, nchild0, ipanel_decode_index0[NCHILDS_MAX];
wxString messg_for_status_bar;

if((initialized != 1234)
   || (m_Display2Mutex == NULL)) return;

// Prompt for current value of nchild:
// and read ipanel_decode_index0 (not used here):
 Get_DisplayPanel_decode_idx(ipanel_decode_index0, &nchild0);

// Allocate memory if needed:
if((nchild0 != nchild_image1) || (nx1 != nx1_image1)
   || (ny1 != ny1_image1)) {
  for(k = 0; k < nchild0; k ++) {
   if(child_image1[k] != NULL) delete[] child_image1[k];
   child_image1[k] = new double[nx1 * ny1];
   }
// Save new values to private variables:
  nchild_image1 = nchild0;
  nx1_image1 = nx1;
  ny1_image1 = ny1;
  }

// Read current values from mutex (from Decodethread):
  m_Display2Mutex->ReadImages(child_image1, nx1, ny1, nchild_image1,
                              messg_for_status_bar);

// Display images of the processing results:
   jlp_aspdisp_panel->UpdateChildWindows0(child_image1, nx1, ny1);

// Refresh Message in tool bar:
   DispStatic1->SetLabel(messg_for_status_bar);

return;
}
