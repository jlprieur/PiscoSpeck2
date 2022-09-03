/************************************************************************
* "jlp_camera_panel.h"
* Virtual JLP_CameraPanel class
*
* JLP
* Version 05/10/2017
*************************************************************************/
#ifndef jlp_camera_panel_h_
#define jlp_camera_panel_h_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "jlp_camera_utils.h" // JLP_CAMERA_SETTINGS, ...

typedef struct {
bool ChangesDone;
bool ChangesValidated;
}
JLP_CAMERA_PANEL_CHANGES;

class AspFrame;
class JLP_Camera1;
class JLP_AndorCam1;
class JLP_AndorPanel;
#ifdef USE_RAPTOR
class JLP_RaptorCam1;
class JLP_RaptorPanel;
#endif
class JLP_FitsCubeCam1;
class JLP_FitsCubePanel;

class JLP_CameraPanel : wxPanel
{

public:
  JLP_CameraPanel(){};
  JLP_CameraPanel(AspFrame *asp_frame, wxFrame *frame, JLP_Camera1 **cam1,
                  int *camera_type, wxString *str_messg,
                  const int n_messg, bool small_version, int *panel_wxID,
                  wxFrame **m_subpanel);

  ~JLP_CameraPanel(){};
// In "jlp_camera_panel.cpp"
  void CamPanel_Get_CameraSettings(JLP_CAMERA_SETTINGS* CamSet0);
  void CamPanel_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0);
  int  SelectInput3DFitsFile(char *filename0);

// Called when changing pages:
  void CamPanel_SetChangesDone(const bool bstat){JCamChanges1.ChangesDone = bstat;}
  void CamPanel_SetValidatedChanges(const bool bstat){JCamChanges1.ChangesValidated = bstat;}
  bool CamPanel_ChangesDone(){return(JCamChanges1.ChangesDone);}
  bool CamPanel_ValidatedChanges(){return(JCamChanges1.ChangesValidated);}
  void CamPanel_InitChanges(){
      JCamChanges1.ChangesDone = false;
      JCamChanges1.ChangesValidated = false;
  }

  void CamPanel_ValidateChanges();
  void CamPanel_ValidateNewSettings();
  void CamPanel_CancelNonValidatedChanges();

private:
  int camera_type1;
  JLP_Camera1 *m_cam1;
  JLP_AndorCam1 *m_andor_cam1;
#ifdef USE_RAPTOR
  JLP_RaptorCam1 *m_raptor_cam1;
#endif
  JLP_FitsCubeCam1 *m_fitscube_cam1;
  JLP_AndorPanel *m_andor_panel;
#ifdef USE_RAPTOR
  JLP_RaptorPanel *m_raptor_panel;
#endif
  JLP_FitsCubePanel *m_fitscube_panel;
  JLP_CAMERA_SETTINGS CamSet1, Original_CamSet1;
  JLP_CAMERA_PANEL_CHANGES JCamChanges1;
};

#endif // EOF sentry
