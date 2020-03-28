/******************************************************************************
* jlp_language_dlg.h
* To select the language of the messages 
*
* Author:  JLP 
* Version: 05/08/2015
******************************************************************************/
#ifndef jlp_language_dlg_h    // sentry 
#define jlp_language_dlg_h

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/filename.h"

#define NLANG 5

/********************************************************************
* Class JLP_Language_Dlg
*********************************************************************/

class JLP_Language_Dlg: public wxDialog
{
public:

// Constructor:
   JLP_Language_Dlg(wxFrame *parent, const wxString &title);
// Destructor: 
   ~JLP_Language_Dlg(){
       };

// Handling events:
   void OnOKButton( wxCommandEvent &event );
   void OnCancelButton( wxCommandEvent &event );
   void OnSelectLanguage( wxCommandEvent &event );

// Accessors:
   int RetrieveData(int *iLang_2) {
       *iLang_2 = iLang_1; 
       return(0);
       }

protected:
   bool DataIsOK() {
       if(iLang_1 < 0 || iLang_1 >= NLANG ) return(false); 
       return(true);
       }

private:
   wxRadioButton *LangRadioButton[NLANG];
   int iLang_1;
   int initialized;

   DECLARE_EVENT_TABLE()
};

#endif               // EOF sentry
