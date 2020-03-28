/************************************************************************
* "jlp_camera_display.h"
* JLP_CameraDisplay class
*
* JLP
* Version 07/10/2017
*************************************************************************/
#ifndef jlp_camera_display_h_
#define jlp_camera_display_h_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/tglbtn.h"
#include "wx/bookctrl.h"
#include "wx/imaglist.h"
#include "wx/cshelp.h"
#include "wx/thread.h"
#include "wx/event.h"  // wxThreadEvent

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#include "jlp_wx_ipanel.h"      // JLP_wxImagePanel
#include "jlp_gdev_wxwid.h"     // JLP_GDev_wxWID
#include "jlp_camera_utils.h"   // JLP_CAMERA_SETTINGS
#include "jlp_camera1.h"        // JLP_Camera1

class JLP_CameraDisplay : public wxFrame
{

public:

// In "jlp_camera_display.cpp":
  JLP_CameraDisplay(JLP_CAMERA_SETTINGS CamSet0,
                   const int width0, const int height0);
  ~JLP_CameraDisplay(){
   return;
   };
  void LoadNewSettings(JLP_CAMERA_SETTINGS CamSet0, const int nx0_ROI_0,
                       const int ny0_ROI_0);
  void CameraDisplay_LoadImage(double *dble_image0, int nx0, int ny0);
  void CameraDisplay_LoadDummyImage();
  void OnStartProcessing(wxCommandEvent& event){};
  void DisplayROI();
// Public to allow writing on it from parent frame:
  void DisplayImageInfoOnTop(wxString buffer){
   wxString buffer0;
// Copy the input string:
   buffer0.Append(buffer);
// And remove trailing blanks:
   buffer0.Trim();
   Static_ImageInfo->SetLabel(buffer0);
   Static_Temperature->SetLabel(temperature_msg1);
   };
// Load current temperature for further display
// of the mirror mode and the temperature on top of the screen:
   void UpdateCurrentTemperature(wxString temp_msg0)
   {
    temperature_msg1 = temp_msg0;
   };

// Maximum value of the displayed frame:
  double Get_Max_Value() {
    double value = 0.;
    if(initialized == 1234) {
      if(pChildVisu1 != NULL) {
      value = pChildVisu1->Get_Up_Threshold();
      }
    }
  return(value);
  };

protected:
  void DisplayPanelSetup(const int width0, const int height0);

private:
  wxStatusBar *m_StatusBar;
  JLP_wxImagePanel *pChildVisu1;
  JLP_CAMERA_SETTINGS CamSet1;
  int initialized, nx0_ROI, ny0_ROI;
  wxStaticText *Static_ImageInfo, *Static_Temperature;
  wxString temperature_msg1, image_info_msg1;
};

#endif // EOF sentry
