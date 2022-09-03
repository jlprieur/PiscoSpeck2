/****************************************************************************
* Name: jlp_camera_panel.cpp
* Virtual JLP_CameraPanel class
*
* JLP
* Version 09/10/2017
****************************************************************************/
#include "jlp_camera_panel.h"
#include "asp2_frame.h"
#include "jlp_camera1.h"
#include "jlp_andor_cam1.h"
#include "jlp_andor_panel.h"
#ifdef USE_RAPTOR
#include "jlp_raptor_cam1.h"
#include "jlp_raptor_panel.h"
#endif
#include "jlp_fitscube_panel.h"
// ============================================================================
// implementation
// ============================================================================
/******************************************************************************
* Constructor
*******************************************************************************/
JLP_CameraPanel::JLP_CameraPanel(AspFrame *asp_frame, wxFrame *frame,
                                 JLP_Camera1 **cam0, int *camera_type0,
                                 wxString *str_messg, const int n_messg,
                                 bool small_version, int *panel_wxID,
                                 wxFrame **sub_panel_0)
                                 : wxPanel(frame)
{
int status;
char infits_fname[128];

/* Camera type:
* 0 = Fitscube
* 1 = Andor
* 2 = Raptor
*/
camera_type1 = *camera_type0;
m_cam1 = NULL;
m_andor_cam1 = NULL;
#ifdef USE_RAPTOR
m_raptor_cam1 = NULL;
#endif
m_fitscube_cam1 = NULL;
*panel_wxID = 9802;
*sub_panel_0 = NULL;

// Connect to camera (if possible):
 switch(camera_type1) {
    default:
// FitsCube:
    case 0:
      status = SelectInput3DFitsFile(infits_fname);
      if(status != 0) exit(-1);
      m_fitscube_cam1 = new JLP_FitsCubeCam1(infits_fname);
      if(m_fitscube_cam1->NotConnected() == true) {
       delete m_fitscube_cam1;
       m_fitscube_cam1 = NULL;
       wxMessageBox(wxT("Fatal error: file format not compatible with this program"),
                    wxT("PiscoSpeck2"), wxOK | wxICON_ERROR);
       camera_type1 = -1;
      } else {
       m_cam1 = (JLP_Camera1 *)m_fitscube_cam1;
      }
    break;
// Andor camera:
    case 1:
      m_andor_cam1 = new JLP_AndorCam1();
      if(m_andor_cam1->NotConnected() == true) {
       delete m_andor_cam1;
       m_andor_cam1 = NULL;
       camera_type1 = -1;
      } else {
       m_cam1 = (JLP_Camera1 *)m_andor_cam1;
      }
    break;
#ifdef USE_RAPTOR
// Raptor camera:
    case 2:
      m_raptor_cam1 = new JLP_RaptorCam1();
      if(m_raptor_cam1->NotConnected() == true) {
       delete m_raptor_cam1;
       m_raptor_cam1 = NULL;
       camera_type1 = -1;
      } else {
       m_cam1 = (JLP_Camera1 *)m_raptor_cam1;
      }
    break;
#endif
 }

if((m_cam1 != NULL) && (camera_type1 >= 0))
  m_cam1->Cam1_Get_CameraSettings(&CamSet1);
 else
  exit(-1);

// Output variables:
*cam0 = m_cam1;
*camera_type0 = camera_type1;

m_andor_panel = NULL;
#ifdef USE_RAPTOR
m_raptor_panel = NULL;
#endif
m_fitscube_panel = NULL;
 switch(camera_type1) {
  default:
// Fitscube camera:
  case 0:
    m_fitscube_panel = new JLP_FitsCubePanel(asp_frame, frame, *panel_wxID,
                                             m_fitscube_cam1, &JCamChanges1,
                                             str_messg, n_messg,
                                             small_version);
    *sub_panel_0 = (wxFrame *)m_fitscube_panel;
   break;
// ANDOR camera:
  case 1:
    m_andor_panel = new JLP_AndorPanel(asp_frame, frame, *panel_wxID,
                                       m_andor_cam1, &JCamChanges1,
                                       str_messg, n_messg,
                                       small_version);
    *sub_panel_0 = (wxFrame *)m_andor_panel;
    break;
#ifdef USE_RAPTOR
// Raptor camera:
  case 2:
    m_raptor_panel = new JLP_RaptorPanel(asp_frame, frame, *panel_wxID,
                                         m_raptor_cam1, &JCamChanges1,
                                         str_messg, n_messg,
                                         small_version);
    *sub_panel_0 = (wxFrame *)m_raptor_panel;
    break;
#endif
 }

return;
}
/********************************************************************
* Select 3D FITS file for processing
********************************************************************/
int JLP_CameraPanel::SelectInput3DFitsFile(char *filename0)
{
wxString filename, str1, path, extension;
wxString full_filename, filename1;

// Prompt the user for the FITS filename, if not set by the calling routine:
  filename.Empty();
  filename = wxFileSelector(_T("Select input 3D FITS file to be processed"), _T(""), _T(""),
                      _T("fits|fit|FIT"),
                      _T("FITS files (*.fits;*.fit)|*.fits;*.fit;*.FITS;*.FIT"));

  if (filename.IsEmpty() ) {
    return(-1);
  }
  full_filename = filename;

// Removes the directory name (since the full path is generally too long...)
  wxFileName::SplitPath(full_filename, &path, &filename, &extension);
  filename1 = filename + wxT(".") + extension;

// Copy full filename (with path !) to char array:
 strcpy(filename0, full_filename.c_str());

return(0);
}
/******************************************************************************
*
*******************************************************************************/
void JLP_CameraPanel::CamPanel_Get_CameraSettings(JLP_CAMERA_SETTINGS *CamSet0)
{
  if(m_andor_cam1 != NULL)
    m_andor_cam1->Cam1_Get_CameraSettings(&CamSet1);
#ifdef USE_RAPTOR
  else if(m_raptor_cam1 != NULL)
    m_raptor_cam1->Cam1_Get_CameraSettings(&CamSet1);
#endif
  else if(m_fitscube_cam1 != NULL)
    m_fitscube_cam1->Cam1_Get_CameraSettings(&CamSet1);

  Copy_CameraSettings(CamSet1, CamSet0);

return;
}
/******************************************************************************
*
*******************************************************************************/
void JLP_CameraPanel::CamPanel_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0)
{
  if(m_andor_cam1 != NULL)
    m_andor_cam1->Cam1_Load_CameraSettings(CamSet0);
#ifdef USE_RAPTOR
  else if(m_raptor_cam1 != NULL)
    m_raptor_cam1->Cam1_Load_CameraSettings(CamSet0);
#endif
  else if(m_fitscube_cam1 != NULL)
    m_fitscube_cam1->Cam1_Load_CameraSettings(CamSet0);

Copy_CameraSettings(CamSet0, &CamSet1);
CamPanel_ValidateChanges();

return;
}
/******************************************************************************
*
*******************************************************************************/
void JLP_CameraPanel::CamPanel_ValidateChanges()
{
  if(m_andor_panel != NULL)
    m_andor_panel->CamPanel_ValidateNewSettings();
#ifdef USE_RAPTOR
  else if(m_raptor_panel != NULL)
    m_raptor_panel->CamPanel_ValidateNewSettings();
#endif
  else if(m_fitscube_panel != NULL)
    m_fitscube_panel->CamPanel_ValidateNewSettings();

return;
}
/******************************************************************************
*
*******************************************************************************/
void JLP_CameraPanel::CamPanel_CancelNonValidatedChanges()
{
  if(m_andor_panel != NULL)
    m_andor_panel->CamPanel_CancelNonValidatedChanges();
#ifdef USE_RAPTOR
  else if(m_raptor_panel != NULL)
    m_raptor_panel->CamPanel_CancelNonValidatedChanges();
#endif
  else if(m_fitscube_panel != NULL)
    m_fitscube_panel->CamPanel_CancelNonValidatedChanges();
}
/******************************************************************************
*
*******************************************************************************/
void JLP_CameraPanel::CamPanel_ValidateNewSettings()
{
  if(m_andor_panel != NULL)
    m_andor_panel->CamPanel_ValidateNewSettings();
#ifdef USE_RAPTOR
  else if(m_raptor_panel != NULL)
    m_raptor_panel->CamPanel_ValidateNewSettings();
#endif
  else if(m_fitscube_panel != NULL)
    m_fitscube_panel->CamPanel_ValidateNewSettings();
}
