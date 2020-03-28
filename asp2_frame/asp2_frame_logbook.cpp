/******************************************************************************
* Name:        gdp_frame_logbook.cpp (AspFrame class)
* same as  "pisco/Gdpisco/gp_frame_logbook.cpp" (AspFrame class)
*          but without "int BinariesSaveMeasurements();"
*
* Purpose:     Logbook utilities
* Author:      JLP
* Version:     03/08/2015
******************************************************************************/
#include "asp2_frame.h"
#include "asp2_frame_id.h"  // Menu identifiers

/* Declared in asp2_frame.h
void OnViewLogbook(wxCommandEvent& event)
void OnSaveLogbook(wxCommandEvent& WXUNUSED(event))
void OnOffsetCorrection(wxCommandEvent& event);
void OnFlatFieldCorrection(wxCommandEvent& event);
int  SaveLogbook(wxString save_filename)
void ShowLogbook()
void HideLogbook()
void ClearLogbook()
void CleanLogbook()
int  WriteToLogbook(wxString str1, bool SaveToFile);
int  AddNewPointToLogbook(int xx, int yy)
*/

/************************************************************************
* Showing/hiding logbook panel
************************************************************************/
void AspFrame::OnViewLogbook(wxCommandEvent& event)
{
  switch (event.GetId())
  {
   case ID_LOGBOOK_SHOW:
     ShowLogbook();
     break;
   case ID_LOGBOOK_HIDE:
     HideLogbook();
     break;
   }
}
/************************************************************************
* Save useful content of logbook to file
************************************************************************/
void AspFrame::OnSaveLogbook(wxCommandEvent& WXUNUSED(event))
{
wxString save_filename;
int status;

// Select name for output logbook file:
wxFileDialog saveFileDialog(this, wxT("Save logbook to file"), wxT(""), wxT(""),
               wxT("Logbook files (*.log;*.txt)|*.log;*.txt"),
               wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

// Stops the timers:
 StopDisplayTimers();
 status = saveFileDialog.ShowModal();
 RestartDisplayTimers();

// If "cancel", exit from here without saving files
 if (status == wxID_CANCEL) return;

save_filename = saveFileDialog.GetFilename();

SaveLogbook(save_filename);

return;
}
/*******************************************************************
* Clear the logbook: erase all its content
********************************************************************/
void AspFrame::OnClearLogbook(wxCommandEvent& event)
{
ClearLogbook();
return;
}
/*******************************************************************
* Clean the logbook: only keep its useful content
********************************************************************/
void AspFrame::OnCleanLogbook(wxCommandEvent& event)
{
CleanLogbook();
}
/************************************************************************
* Save useful content of logbook to file
* Input:
* save_filename: wxString whose value is set in gdp_frame_menu.cpp
************************************************************************/
int AspFrame::SaveLogbook(wxString save_filename)
{
int status = -2;

 if(initialized == 1234) status = jlp_Logbook->SaveLogbook(save_filename);

return(status);
}
/************************************************************************
* Showing logbook panel
************************************************************************/
void AspFrame::ShowLogbook()
{
 if(initialized != 1234) return;

 m_topsizer->Show(jlp_Logbook);
 m_topsizer->Layout();
}
/************************************************************************
* Hiding logbook panel
************************************************************************/
void AspFrame::HideLogbook()
{
 if(initialized != 1234) return;

 m_topsizer->Hide(jlp_Logbook);
 m_topsizer->Layout();
}
/*******************************************************************
* Clear the logbook: erase all its content
********************************************************************/
void AspFrame::ClearLogbook()
{
wxString str1;

 if(initialized != 1234) return;

 jlp_Logbook->Clear();

return;
}
/*******************************************************************
* Clean the logbook: only keep its useful content
********************************************************************/
void AspFrame::CleanLogbook()
{
 if(initialized != 1234) return;
 jlp_Logbook->Clean();
}
/************************************************************************
* Write to logbook
*************************************************************************/
int  AspFrame::WriteToLogbook(wxString str1, bool SaveToFile)
{
int status = -1;

 if(initialized == 1234) status = jlp_Logbook->WriteToLogbook(str1, SaveToFile);

return(status);
}
/************************************************************************
* Add a new point to logbook
*************************************************************************/
int AspFrame::AddNewPointToLogbook(double xx, double yy, double value)
{
int status = -1;
wxString str1;

 if(initialized == 1234) {
  str1.Printf("%.2f %.2f %.4g\n", xx, yy, value);
  status = jlp_Logbook->WriteToLogbook(str1, true);
  }

return(status);
}
