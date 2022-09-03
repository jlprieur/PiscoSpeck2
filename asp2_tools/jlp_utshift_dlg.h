/******************************************************************************
* jlp_utshift_dlg.h
* To select the utshift value 
*
* Author:  JLP 
* Version: 05/08/2015
******************************************************************************/
#ifndef jlp_utshift_dlg_h    // sentry 
#define jlp_utshift_dlg_h

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "asp2_defs.h" // JLP_ComboBox

/********************************************************************
* Class JLP_UTShift_Dlg
*********************************************************************/

class JLP_UTShift_Dlg: public wxDialog
{
public:

// Constructor:
   JLP_UTShift_Dlg(wxFrame *parent, const wxString &title);
// Destructor: 
   ~JLP_UTShift_Dlg(){
       };

// in "jlp_utshift_dlg.cpp"
   void UTShift_Panel_Setup();
// Handling events:
   void OnOKButton( wxCommandEvent &event );
   void OnCancelButton( wxCommandEvent &event );
   void OnSelectCombo( wxCommandEvent &event );

// Accessors:
   int RetrieveData(int *UTShift_2) {
       *UTShift_2 = UTShift_1; 
       return(0);
       }

protected:

private:
   JLP_ComboBox PscCmb_UTShift;
   int UTShift_1;
   int initialized;

   DECLARE_EVENT_TABLE()
};

#endif               // EOF sentry
