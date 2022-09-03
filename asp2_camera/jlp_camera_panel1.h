/************************************************************************
* "jlp_camera_panel1.h"
* Implementation of virtual JLP_CameraPanel class (needed for linking ??)
*
* JLP
* Version 09/10/2017
*************************************************************************/
#ifndef jlp_camera_panel1_h_
#define jlp_camera_panel1_h_

#include "jlp_camera_panel.h"      // JLP_Camera_Panel virtual class

class JLP_CameraPanel1 : public JLP_CameraPanel
{

public:

  JLP_CameraPanel1();
  ~JLP_CameraPanel1(){};

// Virtual routines:
  void ValidateChanges(){};
  void CancelNonValidatedChanges(){};
  void ValidateNewSettings(){};

private:

};

#endif // EOF sentry
