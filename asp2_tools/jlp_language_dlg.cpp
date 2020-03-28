/******************************************************************************
* jlp_language_dlg.cpp
* Dialog box used to select the language of the messages
*
* Author:      JLP 
* Version:     28/08/2015
******************************************************************************/
#include "jlp_language_dlg.h"

//*************************************************************************
enum
{
   ID_LANG_OK = 800,
   ID_LANG_CANCEL,
   ID_LANG_EN,
   ID_LANG_FR,
   ID_LANG_IT,
   ID_LANG_SP,
   ID_LANG_DE
};

BEGIN_EVENT_TABLE(JLP_Language_Dlg, wxDialog)
EVT_BUTTON  (ID_LANG_OK, JLP_Language_Dlg::OnOKButton)
EVT_BUTTON  (ID_LANG_CANCEL, JLP_Language_Dlg::OnCancelButton)
EVT_RADIOBUTTON (ID_LANG_EN, JLP_Language_Dlg::OnSelectLanguage)
EVT_RADIOBUTTON (ID_LANG_FR, JLP_Language_Dlg::OnSelectLanguage)
EVT_RADIOBUTTON (ID_LANG_IT, JLP_Language_Dlg::OnSelectLanguage)
EVT_RADIOBUTTON (ID_LANG_SP, JLP_Language_Dlg::OnSelectLanguage)
EVT_RADIOBUTTON (ID_LANG_DE, JLP_Language_Dlg::OnSelectLanguage)
END_EVENT_TABLE()


/********************************************************************
* Constructor:
********************************************************************/
JLP_Language_Dlg::JLP_Language_Dlg(wxFrame *parent, 
                                   const wxString &title)
        : wxDialog(parent, -1, title, wxPoint(400,100), wxDefaultSize,
                   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
wxBoxSizer *topsizer;
wxFlexGridSizer *fgs1;
int i, nrows, ncols, vgap = 12, hgap = 12;

// Initialization at first radio item: 
  iLang_1 = 0;

// To avoid initialization problems with Windows:
// (An event is sent to "OnSelectLanguage"
//  as soon as a Text control is created...)
  initialized = 0; 

// Flexible grid sizer:
  nrows = NLANG;
  ncols = 1;
  fgs1 = new wxFlexGridSizer(nrows, ncols, vgap, hgap);

// ***************** Language menu ******************************
  LangRadioButton[0] = new wxRadioButton(this, ID_LANG_EN, _T("English"),
                                        wxPoint(-1,-1), wxSize(-1,-1),
                                        wxRB_GROUP);
  LangRadioButton[1] = new wxRadioButton(this, ID_LANG_FR, 
                                         _T("Fran\u00E7ais")); 
  LangRadioButton[2] = new wxRadioButton(this, ID_LANG_IT, 
                                         _T("Italiano")); 
  LangRadioButton[3] = new wxRadioButton(this, ID_LANG_SP, 
                                         _T("Espa\u00F1ol")); 
  LangRadioButton[4] = new wxRadioButton(this, ID_LANG_DE, 
                                         _T("Deutsch")); 
// ccedil: 00E7
// ntilde: 00F1
  topsizer = new wxBoxSizer( wxVERTICAL );


// Sizer surrounded with a rectangle, with a title on top:
  wxStaticBoxSizer *Language_sizer = new wxStaticBoxSizer(wxVERTICAL, this, 
                                   _T(" Menu language"));
    
  for(i = 0; i < NLANG; i++) fgs1->Add(LangRadioButton[i]); 

  Language_sizer->Add(fgs1, 0, wxALIGN_CENTER|wxALL, 20);
  topsizer->Add(Language_sizer, 0, wxALIGN_CENTER|wxALL, 20);

wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );

//create two buttons that are horizontally unstretchable, 
  // with an all-around border with a width of 10 and implicit top alignment
 button_sizer->Add(
    new wxButton(this, ID_LANG_OK, _T("OK") ), 0, wxALIGN_LEFT|wxALL, 10);

 button_sizer->Add(
   new wxButton(this, ID_LANG_CANCEL, _T("Cancel") ), 0, wxALIGN_CENTER|wxALL, 10);

  //create a sizer with no border and centered horizontally
  topsizer->Add(button_sizer, 0, wxALIGN_CENTER);

  SetSizer(topsizer);      // use the sizer for layout

  topsizer->SetSizeHints( this );   // set size hints to honour minimum size

  initialized = 1234; 
return;
}
/**************************************************************************
* Handle "OK" button:
**************************************************************************/
void JLP_Language_Dlg::OnOKButton( wxCommandEvent& WXUNUSED(event) )
{
// Close dialog and return status = 0:
  EndModal(0); 
}
/**************************************************************************
* Handle "Cancel" button:
**************************************************************************/
void JLP_Language_Dlg::OnCancelButton( wxCommandEvent& WXUNUSED(event) )
{
// Close dialog and return status = 1:
  EndModal(1); 
}
/**************************************************************************
* Handle Radio Button selection	 
*
*
* iLang: 0=English 1=French 2=Italian 3=Spanish 4=German
**************************************************************************/
void JLP_Language_Dlg::OnSelectLanguage( wxCommandEvent& event )
{

// First check that all text controls are created:
 if(initialized != 1234) return;

  switch (event.GetId())
  {
   case ID_LANG_EN:
    {
      iLang_1 = 0;
      break;
    }
   case ID_LANG_FR:
    {
      iLang_1 = 1;
      break;
    }
   case ID_LANG_IT:
    {
      iLang_1 = 2;
      break;
    }
   case ID_LANG_SP:
    {
      iLang_1 = 3;
      break;
    }
   case ID_LANG_DE:
    {
      iLang_1 = 4;
      break;
    }
  }  // EOF switch
return;
}
