/******************************************************************************
* asp_camera_dlg.cpp
* Dialog box used to select the camera (None/Andor/Raptor/FitsCube)
*
* Author:      JLP
* Version:     26/11/2017
******************************************************************************/
#include "asp_camera_dlg.h"

//*************************************************************************
enum
{
   ID_CAMERA_OK = 4800,
   ID_CAMERA_CANCEL,
   ID_CAMERA_FITS_CUBE,
   ID_CAMERA_ANDOR,
   ID_CAMERA_RAPTOR
};

BEGIN_EVENT_TABLE(AspCamera_Dlg, wxDialog)
EVT_BUTTON (ID_CAMERA_OK, AspCamera_Dlg::OnOKButton)
EVT_BUTTON (ID_CAMERA_CANCEL, AspCamera_Dlg::OnCancelButton)
EVT_RADIOBUTTON (ID_CAMERA_FITS_CUBE, AspCamera_Dlg::OnSelectCamera)
EVT_RADIOBUTTON (ID_CAMERA_ANDOR, AspCamera_Dlg::OnSelectCamera)
#ifdef USE_RAPTOR
EVT_RADIOBUTTON (ID_CAMERA_RAPTOR, AspCamera_Dlg::OnSelectCamera)
#endif
END_EVENT_TABLE()


/********************************************************************
* Constructor:
********************************************************************/
AspCamera_Dlg::AspCamera_Dlg(wxFrame *parent,
                                   const wxString &title)
        : wxDialog(parent, -1, title, wxPoint(400,100), wxDefaultSize,
                   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
wxBoxSizer *topsizer;
wxFlexGridSizer *fgs1;
int i, nrows, ncols, vgap = 12, hgap = 12;

// Initialization at first radio item:
  iCamera_1 = 0;

// To avoid initialization problems with Windows:
// (An event is sent to "OnSelectCamera"
//  as soon as a Text control is created...)
  initialized = 0;

// Flexible grid sizer:
  nrows = NCAMERA;
  ncols = 1;
  fgs1 = new wxFlexGridSizer(nrows, ncols, vgap, hgap);

// ***************** Language menu ******************************
  CameraRadioButton[0] = new wxRadioButton(this, ID_CAMERA_FITS_CUBE,
                                           _T("FitsCube"),
                                           wxPoint(-1,-1), wxSize(-1,-1),
                                           wxRB_GROUP);
  CameraRadioButton[1] = new wxRadioButton(this, ID_CAMERA_ANDOR, _T("Andor"));
#ifdef RAPTOR
  CameraRadioButton[2] = new wxRadioButton(this, ID_CAMERA_RAPTOR,
                                           _T("Raptor"));
#endif
  topsizer = new wxBoxSizer( wxVERTICAL );


// Sizer surrounded with a rectangle, with a title on top:
  wxStaticBoxSizer *Language_sizer = new wxStaticBoxSizer(wxVERTICAL, this,
                                                          _T(" Camera menu"));

  for(i = 0; i < NCAMERA; i++) fgs1->Add(CameraRadioButton[i]);

  Language_sizer->Add(fgs1, 0, wxALIGN_CENTER|wxALL, 20);
  topsizer->Add(Language_sizer, 0, wxALIGN_CENTER|wxALL, 20);

wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );

//create two buttons that are horizontally unstretchable,
  // with an all-around border with a width of 10 and implicit top alignment
 button_sizer->Add(
    new wxButton(this, ID_CAMERA_OK, _T("OK") ), 0, wxALIGN_LEFT|wxALL, 10);

 button_sizer->Add(
   new wxButton(this, ID_CAMERA_CANCEL, _T("Cancel") ), 0, wxALIGN_CENTER|wxALL, 10);

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
void AspCamera_Dlg::OnOKButton( wxCommandEvent& WXUNUSED(event) )
{
// Close dialog and return status = 0:
  EndModal(0);
}
/**************************************************************************
* Handle "Cancel" button:
**************************************************************************/
void AspCamera_Dlg::OnCancelButton( wxCommandEvent& WXUNUSED(event) )
{
// Close dialog and return status = 1:
  EndModal(1);
}
/**************************************************************************
* Handle Radio Button selection
*
*
* icamera: 0=FitsCube 1=Andor 2=Raptor
**************************************************************************/
void AspCamera_Dlg::OnSelectCamera( wxCommandEvent& event )
{

// First check that all text controls are created:
 if(initialized != 1234) return;

  switch (event.GetId())
  {
   default:
   case ID_CAMERA_FITS_CUBE:
    {
      iCamera_1 = 0;
      break;
    }
   case ID_CAMERA_ANDOR:
    {
      iCamera_1 = 1;
      break;
    }
#ifdef USE_RAPTOR
   case ID_CAMERA_RAPTOR:
    {
      iCamera_1 = 2;
      break;
    }
#endif
  }  // EOF switch
return;
}
