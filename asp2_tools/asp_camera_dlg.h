/******************************************************************************
* asp_camera_dlg.h
* Dialog box used to select the camera (none/Andor/Raptor/FitsCube)
*
* Author:  JLP
* Version: 05/08/2015
******************************************************************************/
#ifndef asp_camera_dlg_h    // sentry
#define asp_camera_dlg_h

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/filename.h"

// icamera = 0 : FitsCube
// icamera = 1 : Andor
// icamera = 2 : Raptor
#ifdef USE_RAPTOR
#define NCAMERA 3
#else
#define NCAMERA 2
#endif

/********************************************************************
* Class AspCamera_Dlg
*********************************************************************/

class AspCamera_Dlg: public wxDialog
{
public:

// Constructor:
   AspCamera_Dlg(wxFrame *parent, const wxString &title);
// Destructor:
   ~AspCamera_Dlg(){
       };

// Handling events:
   void OnOKButton( wxCommandEvent &event );
   void OnCancelButton( wxCommandEvent &event );
   void OnSelectCamera( wxCommandEvent &event );

// Accessors:
   int RetrieveData(int *iCamera_2) {
       *iCamera_2 = iCamera_1;
       return(0);
       }

protected:
   bool DataIsOK() {
       if(iCamera_1 < 0 || iCamera_1 >= NCAMERA ) return(false);
       return(true);
       }

private:
   wxRadioButton *CameraRadioButton[NCAMERA];
   int iCamera_1;
   int initialized;

   DECLARE_EVENT_TABLE()
};

#endif               // EOF sentry
