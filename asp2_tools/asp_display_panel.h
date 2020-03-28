/************************************************************************
* "asp2_display_panel.h"
* JLP_AspDispPanel class
*
* JLP
* Version 07/09/2015
*************************************************************************/
#ifndef asp2_display_panel_h_
#define asp2_display_panel_h_

#include "asp2_defs.h"         // NCHILDS_MAX,...
#include "asp2_typedef.h"      // PROCESS_SETTINGS
#include "asp2_frame.h"        // AspFrame
#include "jlp_wx_ipanel.h"     // JLP_wxImagePanel
#include "jlp_wxlogbook.h"     // JLP_wxLogbook class

class JLP_Decode;
class AspFrame;

enum {

// Check boxes on top of each sub-panel
  ID_IPANEL_CHECKBOX0 = 6600,
  ID_IPANEL_CHECKBOX1,
  ID_IPANEL_CHECKBOX2,
  ID_IPANEL_CHECKBOX3,
  ID_IPANEL_CHECKBOX4,
  ID_IPANEL_CHECKBOX5,
  ID_IPANEL_CHECKBOX6,
  ID_IPANEL_CHECKBOX7,
  ID_IPANEL_CHECKBOX8,
  ID_IPANEL_CHECKBOX9
};

class JLP_AspDispPanel : wxPanel
{

public:

// In "asp2_display_panel.cpp":

  JLP_AspDispPanel(AspFrame *asp_frame0, wxFrame *frame0,
                   JLP_wxLogbook *logbook, wxStatusBar *status_bar,
                   wxString *str_messg, const int n_messg,
                   const int width0, const int height0,
                   bool small_version);
  ~JLP_AspDispPanel(){MyFreeMemory();};

  void NewDisplaySetup(PROCESS_SETTINGS Pset0);
  void NewSettingsOfIPanels(PROCESS_SETTINGS Pset0, int &nChild0);
  void OnSelectIPanel(wxCommandEvent& event);
  void AspDisp_LoadImage(double *dble_image0, int nx0, int ny0, int ipanel_index);
  int UpdateChildWindows(JLP_Decode *Decod2, const int ModelDisplayOption0);
  int UpdateChildWindows0(double *child_image2[NCHILDS_MAX],
                          const int nx2, const int ny2);
  int UpdateFirstChildWindow(JLP_Decode *Decod2, double *dble_image2,
                              int nx2, int ny2, const int ModelDisplayOption0);
  void RefreshDisplay();
  void SetITT_ActivePanel(const int is_linear);
  void SetLUT_ActivePanel(char *lut_type);

// Accessors:
  void Get_nChild(int *nchild0) {*nchild0 = nChild; return;};
  void Get_decode_index(int *decode_index) {
    int k;
    for(k = 0; k < nChild; k++) decode_index[k] = ipanel_decode_index[k];
  }

//  JLP_wxImagePanel *ActiveImagePanel(){return active_ipanel;};

protected:

  void DisplayPanelSetup(const int width0, const int height0);
  void Init_IPanelTitles();
  void Init_IPanelOptions(JLP_wxImagePanel *pChildVisu, int decode_index,
                          DISPLAY_SETTINGS *ipanel_settings0);
  void SetActiveIPanel(int k_index);
  void MyFreeMemory();

private:

  AspFrame *m_aspframe;
  wxString *m_messg;
  JLP_wxLogbook *m_Logbook;
  wxStatusBar *m_StatusBar;
  JLP_wxImagePanel *active_ipanel, *pChildVisu[NCHILDS_MAX];
  int old_nChild, nChild, nChild_max, active_ipanel_index, initialized;
  int ModelDisplayOption1, ipanel_decode_index[NCHILDS_MAX];
  wxString DecodeName[DECODE_NAME_MAX], DecodeTitle[DECODE_NAME_MAX];
  wxString IPanelTitle[NCHILDS_MAX];
  DISPLAY_SETTINGS ipanel_settings1[NCHILDS_MAX];
  wxWindowID MyIPanelCheckBox_id[NCHILDS_MAX];
  wxCheckBox *MyIPanelCheckBox[NCHILDS_MAX];

  DECLARE_EVENT_TABLE()
};

#endif // EOF sentry
