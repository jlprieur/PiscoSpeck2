/******************************************************************************
* Name:        asp2_frame_menu (AspFrame class)
* Purpose:     handling menu events of AspFrame clas (see Filespeck2.cpp)
* Author:      JLP
* Version:     28/09/2015
******************************************************************************/
#include "asp2_frame.h"
#include "asp2_frame_id.h"  // Menu identifiers
#include "asp_display_panel.h" // JLP_AspDispPanel class

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #define USE_XPM
#endif

BEGIN_EVENT_TABLE(AspFrame, wxFrame)
// Toolbar buttons:
   EVT_TOOL  (ID_START_PROCESS, AspFrame::OnStartProcessing)
   EVT_TOOL  (ID_STOP_PROCESS, AspFrame::OnStopProcessing)
   EVT_TOOL  (ID_START_STREAM, AspFrame::OnStartStreaming)
   EVT_TOOL  (ID_STOP_STREAM, AspFrame::OnStopStreaming)
   EVT_TOOL  (ID_SAVE_RESULTS, AspFrame::OnSaveProcessingResults)
   EVT_TOOL  (ID_STATISTICS, AspFrame::OnShowStatistics)
   EVT_TOOL  (ID_BISP_RESTORE, AspFrame::OnBispRestore)
   EVT_TOOL  (ID_ERASE_ALL, AspFrame::OnEraseAll)

// Notebook:
   EVT_BOOKCTRL_PAGE_CHANGING(ID_NOTEBOOK, AspFrame::OnPageChanging)
   EVT_BOOKCTRL_PAGE_CHANGED(ID_NOTEBOOK,  AspFrame::OnPageChanged)

// Menu items:
// File:
   EVT_MENU(ID_LOAD_RESULTS2,   AspFrame::OnLoadProcessingResults)
   EVT_MENU(ID_LOAD_BISPECTRUM, AspFrame::OnLoadProcessingResults)
   EVT_MENU(ID_SAVE_TO_3DFITS,  AspFrame::OnSaveFramesTo3DFits)
   EVT_MENU(ID_SAVE_RESULTS2,   AspFrame::OnSaveProcessingResults)
   EVT_MENU(ID_QUIT,            AspFrame::OnQuit)

// LUT:
/* lut_type: 'l'=log rainbow1 'r'=rainbow2 's'=saw 'g'=gray 'c'=curves
*           'p'=pisco
* RAIN1, RAIN2, SAW, GRAY, CUR, PISCO, REV
*/
   EVT_MENU(ID_LUT_RAIN1, AspFrame::OnChangeLUT)
   EVT_MENU(ID_LUT_RAIN2, AspFrame::OnChangeLUT)
   EVT_MENU(ID_LUT_SAW, AspFrame::OnChangeLUT)
   EVT_MENU(ID_LUT_GRAY, AspFrame::OnChangeLUT)
   EVT_MENU(ID_LUT_CUR, AspFrame::OnChangeLUT)
   EVT_MENU(ID_LUT_PISCO, AspFrame::OnChangeLUT)

/*
* ITT:
*         "Lin" (Linear scale)
*         "Log" (Linear scale)
* LIN, LOG
*/
   EVT_MENU(ID_ITT_LIN, AspFrame::OnChangeITT)
   EVT_MENU(ID_ITT_LOG, AspFrame::OnChangeITT)

// Andor cooler timer (for JLP_CameraDisplay)
   EVT_TIMER(ID_CAMDISP_TIMER, AspFrame::OnCameraDisplay1Timer)

// Menu/display
   EVT_MENU(ID_DISPMENU_DEFAULT, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_OFFSET, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_FFIELD, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_UNRES_AUTOC, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_UNRES_MODSQ, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_OPT4, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_OPT5, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_OPT6, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_OPT7, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_OPT8, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_OPT9, AspFrame::OnSelectDisplayOption)
   EVT_MENU(ID_DISPMENU_OPT10, AspFrame::OnSelectDisplayOption)

// Menu/Process
   EVT_MENU(ID_PROCMENU_BISP_RESTORE, AspFrame::OnBispRestore)

// Menu/Logbook
   EVT_MENU(ID_LOGBOOK_SHOW, AspFrame::OnViewLogbook)
   EVT_MENU(ID_LOGBOOK_HIDE, AspFrame::OnViewLogbook)
   EVT_MENU(ID_LOGBOOK_CLEAN, AspFrame::OnCleanLogbook)
   EVT_MENU(ID_LOGBOOK_CLEAR, AspFrame::OnClearLogbook)
   EVT_MENU(ID_LOGBOOK_SAVE, AspFrame::OnSaveLogbook)

// Miscellaneous:
   EVT_MENU(ID_CONTEXT_HELP,   AspFrame::OnContextHelp)
   EVT_MENU(ID_ABOUT,          AspFrame::OnAbout)
   EVT_MENU(ID_HELP,           AspFrame::OnHelp)

   EVT_IDLE(AspFrame::OnIdle)

// Event used to stop the threads before exiting:
   EVT_CLOSE(AspFrame::OnClose)

   EVT_THREAD(ID_DECODE_THREAD_DONE, AspFrame::OnReceiveDoneEventFromDecode)
   EVT_THREAD(ID_CAMERA_THREAD_DONE, AspFrame::OnReceiveDoneEventFromCamera)
   EVT_THREAD(ID_CAMERA_THREAD_STREAM, AspFrame::OnReceiveStreamingEventFromCamera)

END_EVENT_TABLE()

/********************************************************************
* Create and setup the toolbar on top of main frame
********************************************************************/
void AspFrame::CreateMyToolbar()
{
// Set up toolbar
    enum
    {
        Tool_new,
        Tool_open,
        Tool_copy,
        Tool_cut,
        Tool_paste,
        Tool_print,
        Tool_stream_start,
        Tool_stream_stop,
        Tool_process_start,
        Tool_process_stop,
        Tool_save,
        Tool_stats,
        Tool_restore,
        Tool_erase,
        Tool_help,
        Tool_Max
    };

    wxBitmap toolBarBitmaps[Tool_Max];

// JLP2016:
#ifdef WIN32
#if USE_XPM_BITMAPS
    #define INIT_TOOL_BMP(bmp) \
        toolBarBitmaps[Tool_##bmp] = wxBitmap(bmp##_xpm)
#else // !USE_XPM_BITMAPS
    #define INIT_TOOL_BMP(bmp) \
        toolBarBitmaps[Tool_##bmp] = wxBITMAP(bmp)
#endif // USE_XPM_BITMAPS/!USE_XPM_BITMAPS
#else
// JLP2016, for Linux:
// Conversion to "bitmaps/myname.bmp"
    #define JLP_TOOLA(myname) "bitmaps/" #myname ".bmp"
    #define INIT_TOOL_BMP(myname) \
        toolBarBitmaps[Tool_##myname] = \
                   wxBitmap(wxT(JLP_TOOLA(myname)), wxBITMAP_TYPE_BMP)
#endif

    INIT_TOOL_BMP(new);
    INIT_TOOL_BMP(open);
    INIT_TOOL_BMP(save);
    INIT_TOOL_BMP(copy);
    INIT_TOOL_BMP(cut);
    INIT_TOOL_BMP(paste);
    INIT_TOOL_BMP(print);
    INIT_TOOL_BMP(stream_start);
    INIT_TOOL_BMP(stream_stop);
    INIT_TOOL_BMP(process_start);
    INIT_TOOL_BMP(process_stop);
    INIT_TOOL_BMP(stats);
    INIT_TOOL_BMP(restore);
    INIT_TOOL_BMP(erase);
    INIT_TOOL_BMP(help);
    int w = toolBarBitmaps[Tool_new].GetWidth(),
        h = toolBarBitmaps[Tool_new].GetHeight();

        w = (3*w) / 2;
        h = (3*h) / 2;

        for ( size_t n = Tool_new; n < WXSIZEOF(toolBarBitmaps); n++ )
        {
            toolBarBitmaps[n] =
                wxBitmap(toolBarBitmaps[n].ConvertToImage().Scale(w, h));
        }

// Create toolbar:
 m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition,
                           wxDefaultSize, wxTB_HORIZONTAL);
 // No text, since it reduces vertical size!            // | wxTB_TEXT);
 m_toolbar->SetMargins(4, 4);

// this call is actually unnecessary as the toolbar will adjust its tools
// size to fit the biggest icon used anyhow but it doesn't hurt neither
 m_toolbar->SetToolBitmapSize(wxSize(w, h));

// Create buttons:
/*
 main_vtop_sizer = new wxBoxSizer( wxVERTICAL );
 hsizer0 = new wxBoxSizer(wxHORIZONTAL);
*/
// i=120 "ANDOR not connected: please enter 3D FITS file"
 DispStatic1 = new wxStaticText(m_toolbar, wxID_ANY, Str0[iLang][120]);

// i=121 "Start visu"
// i=122 "Start streaming"
 m_toolbar->AddTool(ID_START_STREAM, Str0[iLang][121],
                    toolBarBitmaps[Tool_stream_start],
                    Str0[iLang][121]);
// i=123 "Stop visu"
// i=124 "Stop streaming"
 m_toolbar->AddTool(ID_STOP_STREAM, Str0[iLang][123],
                    toolBarBitmaps[Tool_stream_stop],
                    Str0[iLang][124]);

// i=125 "Start proc"
// i=126 "Start processing"
 m_toolbar->AddTool(ID_START_PROCESS, Str0[iLang][125],
                    toolBarBitmaps[Tool_process_start],
                    Str0[iLang][126]);
// i=127 "Stop proc"
// i=128 "Stop processing"
 m_toolbar->AddTool(ID_STOP_PROCESS, Str0[iLang][127],
                    toolBarBitmaps[Tool_process_stop],
                    Str0[iLang][128]);
// i=129 "Statistics"
// i=130 "Display statistics"
 m_toolbar->AddTool(ID_STATISTICS, Str0[iLang][129],
                    toolBarBitmaps[Tool_stats],
                    Str0[iLang][130]);
// i=131 "Restore"
// i=132 "Restore image"
 m_toolbar->AddTool(ID_BISP_RESTORE, Str0[iLang][131],
                    toolBarBitmaps[Tool_restore],
                    Str0[iLang][132]);


// m_toolbar->AddControl(SaveResultsButton, wxT("Save results"));
 m_toolbar->AddSeparator();
// i=133 "Save"
// i=134 "Save results"
 m_toolbar->AddTool(ID_SAVE_RESULTS, Str0[iLang][133], toolBarBitmaps[Tool_save],
                    Str0[iLang][134]);
// i=135 "Erase"
// i=136 "Erase all"
 m_toolbar->AddTool(ID_ERASE_ALL, Str0[iLang][135], toolBarBitmaps[Tool_erase],
                    Str0[iLang][136]);

// Space:
 m_toolbar->AddControl(new wxStaticText(m_toolbar, wxID_ANY, wxT("    ")));
// Label:
 m_toolbar->AddControl(DispStatic1);

// wxImage::AddHandler( new wxPNGHandler );
// wxBitmap save0(wxT("exit.png"), wxBITMAP_TYPE_PNG);
// m_toolbar->AddTool(ID_SAVE_RESULTS, save0, wxT("Save results"),
//                    wxITEM_CHECK);

/*
 wxImage::AddHandler( new wxJPEGHandler );
 wxBitmap exit(wxT("exit.jpg"), wxBITMAP_TYPE_JPEG);
 m_toolbar->AddTool(wxID_EXIT, exit, wxT("Exit application"));
*/

 m_toolbar->Realize();

// Associate this toolbar to the main frame: in this way, it is nicely
// incorporated on top of the frame, and is not counted
// in the client area:
 SetToolBar(m_toolbar);

}
/********************************************************************
* Setup the menu on top of main frame
********************************************************************/
void AspFrame::Gdp_SetupMenu()
{

// i=140 "Program to process speckle data from Andor or 3D-Fits files"
 SetHelpText(Str0[iLang][140]);

 menu_bar = new wxMenuBar;

// ***************** File menu **********************************
 menuFile = new wxMenu;

 menuFileOpen = new wxMenu;
// i=141 "Old results without bispectrum"
// i=142 "LongInt, Autoc, Modsq"
 menuFileOpen->Append(ID_LOAD_RESULTS2, Str0[iLang][141], Str0[iLang][142]);
// i=143 "Old results, with bispectrum"
// i=144 "LongInt, Autoc, Modsq, Bispectrum"
 menuFileOpen->Append(ID_LOAD_BISPECTRUM, Str0[iLang][143], Str0[iLang][144]);
// i=149 "Open"
// i=150 "Reading data from files"
 menuFile->Append(wxID_ANY, Str0[iLang][149], menuFileOpen, Str0[iLang][150]);
 menuFileSave = new wxMenu;
// i=151 "Processing results"
// i=152 "Save processing results to FITS files"
 menuFileSave->Append( ID_SAVE_RESULTS2, Str0[iLang][151], Str0[iLang][152]);
// i=153 "Elementary frames to 3DFITS"
// i=154 "Save elementary frames to 3D FITS files"
 menuFileSave->Append( ID_SAVE_TO_3DFITS, Str0[iLang][153], Str0[iLang][154]);
 menuFile->AppendSeparator();
// i=155 "Save"
// i=156 "Saving data to files"
 menuFile->Append(wxID_ANY, Str0[iLang][155], menuFileSave, Str0[iLang][156]);
 menuFile->AppendSeparator();

// i=157 "Exit"
// i=158 "Quit program"
 menuFile->Append(ID_QUIT, Str0[iLang][157], Str0[iLang][158]);
// i=159 "File"
 menu_bar->Append(menuFile, Str0[iLang][159]);

// ***************** LUT submenu ******************************
/* lut_type: 'l'=log rainbow1 'r'=rainbow2 's'=saw 'g'=gray 'c'=curves
*           'p'=pisco
* RAIN1, RAIN2, SAW, GRAY, CUR, PISCO, REV
*/
  menuLUT = new wxMenu;
  menuLUT->Append(ID_LUT_NONE, _T("Default"), wxT(" "),
                  wxITEM_RADIO);
  menuLUT->Append(ID_LUT_RAIN1, _T("Rainbow1"), wxT("Colour Look Up Table"),
                  wxITEM_RADIO);
  menuLUT->Append(ID_LUT_RAIN2, _T("Rainbow2"), wxT("Colour Look Up Table"),
                  wxITEM_RADIO);
  menuLUT->Append(ID_LUT_PISCO, _T("Pisco_like"), wxT("Colour Look Up Table"),
                  wxITEM_RADIO);
  menuLUT->Append(ID_LUT_CUR, _T("For Curves"), wxT("Colour Look Up Table"),
                  wxITEM_RADIO);
  menuLUT->Append(ID_LUT_SAW, _T("Gray Saw"), wxT("B&W Look Up Table"),
                  wxITEM_RADIO);
  menuLUT->Append(ID_LUT_GRAY, _T("Gray Linear"), wxT("B&W Look Up Table"),
                  wxITEM_RADIO);
  menu_bar->Append(menuLUT, _T("LUT"));

// ***************** ITT submenu ******************************
/*
* ITT_1:
*         "Lin" (Linear scale)
* LIN, LOG
*/
  menuITT = new wxMenu;
  menuITT->Append(ID_ITT_LIN, _T("Linear"), _T("Linear scale"),
                  wxITEM_RADIO);
  menuITT->Append(ID_ITT_LOG, _T("Logarithmic"), _T("Log. scale"),
                  wxITEM_RADIO);
  menu_bar->Append(menuITT, _T("ITT"));

// ***************** Display menu ******************************
  menuDisplay = new wxMenu;
/****************************************************************
* Display Option (to select the image to be displayed in the "model" window
* ModelDisplayOption:
* -1=none, 0=Offset, 1=FlatField, 2=UnresAutoc, 3=UnresModsq
* 4=Quadrant, 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector 8=PhotModsq,
* 9=Bispectrum, 10=RestoredImage
*/
 menuDisplay->AppendRadioItem(ID_DISPMENU_DEFAULT, wxT("Default"),
                              wxT("Default frame (e.g., short exposure)"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_OFFSET, wxT("Offset"),
                              wxT("Frame used for offset correction"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_FFIELD, wxT("Flat field"),
                               wxT("Frame used for flat field correction"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_UNRES_AUTOC, wxT("Unres. autoc"),
                               wxT("Autocorrelation of an unresolved star"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_UNRES_MODSQ, wxT("Unres. modsq"),
                               wxT("Power spectrum of an unresolved star"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_OPT4, wxT("Quadrant"),
                               wxT("Restricted triple correlation (red is brighter)"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_OPT5, wxT("Flattened autoc"),
                               wxT("Autoc minus unresolved autoc"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_OPT6, wxT("Lucky image"),
                               wxT("Image obtained with 'Lucky Imaging'"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_OPT7, wxT("Direct vector"),
                               wxT("Image obtained with Bagnuolo's method"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_OPT8, wxT("Photon modsq"),
                               wxT("Power spectrum of photon response"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_OPT9, wxT("Bispectrum"),
                               wxT("2D-slice of the 4D-bispectrum"));
 menuDisplay->AppendRadioItem(ID_DISPMENU_OPT10, wxT("Restored image"),
                               wxT("Restored image from the bispectrum"));
 menu_bar->Append(menuDisplay, _T("Display"));

// ***************** Processing menu ******************************
 menuProcessing = new wxMenu;
 menuProcessing->Append( ID_PROCMENU_BISP_RESTORE, _T("Restore image from bispectrum"),
                       wxT("Phase restoration from bispectrum and deconvolution"));
 menu_bar->Append(menuProcessing, _T("Processing"));

// ***************** Logbook menu ******************************
  menuLog = new wxMenu;
// i=50 "Show logbook"
// i=51 "Display the logbook window"
  menuLog->Append( ID_LOGBOOK_SHOW, Str0[iLang][50],
                   Str0[iLang][51], wxITEM_RADIO);
// i=52 "Hide logbook"
// i=53 "Hide the logbook window"
  menuLog->Append( ID_LOGBOOK_HIDE, Str0[iLang][52],
                   Str0[iLang][53], wxITEM_RADIO);
// i=54 "Clear the logbook"
// i=55 "Clear the logbook content"
  menuLog->Append( ID_LOGBOOK_CLEAR, Str0[iLang][54],
                   Str0[iLang][55]);
// i=56 "Clean the logbook"
// i=57 "Clean the logbook content"
  menuLog->Append( ID_LOGBOOK_CLEAN, Str0[iLang][56],
                   Str0[iLang][57]);
// i=58 "Save cleaned logbook"
// i=59 "Save a selection from the logbook content"
  menuLog->Append( ID_LOGBOOK_SAVE, Str0[iLang][58],
                   Str0[iLang][59]);
// i=60 "Logbook"
  menu_bar->Append(menuLog, Str0[iLang][60]);

// ***************** Help menu ******************************
  menuHelp = new wxMenu;
// i=91 "Help"
  menuHelp->Append( ID_HELP, Str0[iLang][90], Str0[iLang][91]);
// i=92 "&Context help...\tCtrl-H"
// i=93 "Get context help for a control"
  menuHelp->Append(ID_CONTEXT_HELP, Str0[iLang][92],
                     Str0[iLang][93]);
// i=94 "&About..."
  menuHelp->Append( ID_ABOUT, Str0[iLang][94], Str0[iLang][95]);
// i=96 "Help"
  menu_bar->Append(menuHelp, Str0[iLang][96]);

  SetMenuBar(menu_bar);

 Gdp_ResetAllOptionsOfImageMenu();

return;
}
/******************************************************************
* Reset options to default (called for first initialisation
* and after loading a new FITS file)
******************************************************************/
void AspFrame::Gdp_ResetAllOptionsOfImageMenu()
{
int iPage;

// Select graphic panel:
iPage = 2;
m_notebook->SetSelection(iPage);

return;
}
/*********************************************************
* Uncheck all display settings
* (Called when a new active ipanel is selected)
**********************************************************/
void AspFrame::InitDisplaySettings(int itt_is_linear0, char *lut_type0)
{
char c0;
/* lut_type: 'l'=log rainbow1 'r'=rainbow2 's'=saw 'g'=gray 'c'=curves
*           'p'=pisco
*/
  c0 = lut_type0[0];
  switch(c0) {
    case 'l' :
     menuLUT->Check(ID_LUT_RAIN1, true);
     break;
    case 'r' :
     menuLUT->Check(ID_LUT_RAIN2, true);
     break;
    case 's' :
     menuLUT->Check(ID_LUT_SAW, true);
     break;
    case 'g' :
     menuLUT->Check(ID_LUT_GRAY, true);
     break;
    case 'c' :
     menuLUT->Check(ID_LUT_CUR, true);
     break;
    case 'p' :
     menuLUT->Check(ID_LUT_PISCO, true);
     break;
  }

 if(itt_is_linear0 == 1)
   menuITT->Check(ID_ITT_LIN, true);
 else
   menuITT->Check(ID_ITT_LOG, true);

}
