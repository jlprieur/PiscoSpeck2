/******************************************************************************
* jlp_utshift_dlg.cpp
* Dialog box used to select the UTShift value
*
* Author:      JLP 
* Version:     28/09/2022
******************************************************************************/
#include "jlp_utshift_dlg.h"

//*************************************************************************
enum
{
   ID_UTSHIFT_OK = 800,
   ID_UTSHIFT_CANCEL,
   ID_UTSHIFT_COMBO,
};

BEGIN_EVENT_TABLE(JLP_UTShift_Dlg, wxDialog)
EVT_BUTTON  (ID_UTSHIFT_OK, JLP_UTShift_Dlg::OnOKButton)
EVT_BUTTON  (ID_UTSHIFT_CANCEL, JLP_UTShift_Dlg::OnCancelButton)
EVT_COMBOBOX (ID_UTSHIFT_COMBO, JLP_UTShift_Dlg::OnSelectCombo)
END_EVENT_TABLE()

/********************************************************************
* Constructor:
********************************************************************/
JLP_UTShift_Dlg::JLP_UTShift_Dlg(wxFrame *parent, 
                                   const wxString &title)
        : wxDialog(parent, -1, title, wxPoint(400,100), wxDefaultSize,
                   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{

// Initialization : 
  UTShift_1 = -1;

// To avoid initialization problems with Windows:
// (An event is sent to "OnSelectUTShift"
//  as soon as a Text control is created...)
  initialized = 0; 

  UTShift_Panel_Setup();

return;
}
/********************************************************************
* 
********************************************************************/
void JLP_UTShift_Dlg::UTShift_Panel_Setup()
{
wxBoxSizer *topSizer;
int wwidth = 100;

// Create top-level sizer     
  topSizer = new wxBoxSizer( wxVERTICAL );

// Create static box and static box sizer 
wxStaticBox* staticBox = new wxStaticBox(this, wxID_ANY, wxT("UT shift (h)"));     
wxStaticBoxSizer* staticSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);     
// "U.T. Shift (h)"
 PscCmb_UTShift.label = wxString("U.T. Shift (h)");
 PscCmb_UTShift.nchoices = 8;
 PscCmb_UTShift.choices[0] = wxT("-4");
 PscCmb_UTShift.choices[1] = wxT("-3");
 PscCmb_UTShift.choices[2] = wxT("-2");
 PscCmb_UTShift.choices[3] = wxT("-1");
 PscCmb_UTShift.choices[4] = wxT("0");
 PscCmb_UTShift.choices[5] = wxT("1");
 PscCmb_UTShift.choices[6] = wxT("2");
 PscCmb_UTShift.choices[7] = wxT("3");
 PscCmb_UTShift.combo = new wxComboBox(this, ID_UTSHIFT_COMBO, wxT(""),
                                       wxDefaultPosition, wxSize(wwidth, 28),
                                       PscCmb_UTShift.nchoices,
                                       PscCmb_UTShift.choices);

// staticSizer->Add(new wxStaticText(this, -1, PscCmb_UTShift.label));
 staticSizer->Add(PscCmb_UTShift.combo, 0, wxALIGN_CENTER|wxALL, 20);

topSizer->Add(staticSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);     

wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );

//Create two buttons that are horizontally unstretchable, 
// with an all-around border with a width of 10 and implicit top alignment
 button_sizer->Add(
    new wxButton(this, ID_UTSHIFT_OK, _T("OK") ), 0, wxALIGN_LEFT|wxALL, 10);

 button_sizer->Add(
   new wxButton(this, ID_UTSHIFT_CANCEL, _T("Cancel") ), 0, wxALIGN_CENTER|wxALL, 10);

//Create a sizer with no border and centered horizontally
  topSizer->Add(button_sizer, 0, wxALIGN_CENTER);

SetSizer(topSizer);     
topSizer->SetSizeHints(this);   // set size hints to honour minimum size

  initialized = 1234; 
 PscCmb_UTShift.combo->SetSelection(UTShift_1 + 4);
}


/**************************************************************************
* Handle "OK" button:
**************************************************************************/
void JLP_UTShift_Dlg::OnOKButton( wxCommandEvent& WXUNUSED(event) )
{
  if(initialized != 1234) return;

// Close dialog and return status = 0:
  EndModal(0); 
}
/**************************************************************************
* Handle "Cancel" button:
**************************************************************************/
void JLP_UTShift_Dlg::OnCancelButton( wxCommandEvent& WXUNUSED(event) )
{
  if(initialized != 1234) return;

// Back to initialization : 
  UTShift_1 = -1;
// Close dialog and return status = 1:
  EndModal(1); 
}
/**************************************************************************
* Handle ComboBox selection	 
*
****************************************************************************/
void JLP_UTShift_Dlg::OnSelectCombo(wxCommandEvent& event)
{
int index;

  if(initialized != 1234) return;

index = PscCmb_UTShift.combo->GetSelection();
UTShift_1 = index - 4;

}
