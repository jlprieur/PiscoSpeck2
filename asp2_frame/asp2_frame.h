/****************************************************************************
* Name: asp2_frame.h (AspFrame class for PiscoSpeck2)
* Purpose: display and process FITS images obtained with PISCO and PISCO2
*
* JLP
* Version 07/10/2017
****************************************************************************/
#ifndef _asp2_frame_
#define _asp2_frame_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/tglbtn.h"
#include "wx/bookctrl.h"
#include "wx/imaglist.h"
#include "wx/cshelp.h"
#include "wx/thread.h"
#include "wx/event.h"  // wxThreadEvent

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#if USE_XPM_BITMAPS
    #include "bitmaps/new.xpm"
    #include "bitmaps/open.xpm"
    #include "bitmaps/save.xpm"
    #include "bitmaps/copy.xpm"
    #include "bitmaps/cut.xpm"
    #include "bitmaps/preview.xpm"  // paste XPM
    #include "bitmaps/print.xpm"
    #include "bitmaps/help.xpm"
#endif // USE_XPM_BITMAPS


#include "jlp_gdev_wxwid.h"   // JLP_GDev_wxWID
#include "jlp_wxlogbook.h"    // JLP_wxLogbook class
#include "jlp_wx_ipanel.h"    // JLP_wxImagePanel class

#include "jlp_camera_panel.h"        // JLP_CameraPanel class
#include "jlp_speckprocess_panel.h"  // JLP_SpeckProcessPanel class
#include "asp_display_panel.h"      // JLP_AspDisp_Panel class
#include "asp2_defs.h"               // NCHILDS_MAX,...
#include "jlp_camera_thread.h"       // JLP_CameraThread(),...
#include "asp2_decode_thread.h"      // JLP_DecodeThread(),...
#include "jlp_camera_display.h"      // JLP_CameraDisplay(),...


// To avoid "loop" in compilation:
class JLP_CameraDisplayMutex;
class JLP_Display2Mutex;
class JLP_AndorCam1;

//----------------------------------------------------------------------
// class definitions
//----------------------------------------------------------------------

class AspFrame: public wxFrame
{
public:

// In "asp2_main.cpp":
    AspFrame(const wxChar *title, int width0, int height0);
    ~AspFrame(){ Main_ShutDown();};

    void Main_ShutDown();
    void Main_Init();
    void UpdateDisplaySetup();
    void NotebookSetup(int width0, int height0);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void EnableStopVideo(bool flag);
    void SetText_to_StatusBar(wxString text, const int icol);
    int LoadNewPointFromCursor(double xx, double yy, double value);
    void CameraPanelSetup();
    void DisplayPanelSetup(int width0, int height0);
    int Get_DisplayPanel_decode_idx(int *decode_index, int *nchild0);
    void SpeckProcessPanelSetup();
    void OnIdle(wxIdleEvent &event);
    void OnPageChanging(wxBookCtrlEvent &event);
    void OnPageChanged(wxBookCtrlEvent &event);
    void OpenCameraConnection();
    void CreateCameraThread();
    void SelectLanguageSetup();
    void SelectCameraSetup();
//    void PromptForCameraType();

#if wxUSE_TOOLTIPS
    void OnToggleTooltips(wxCommandEvent& event);
#endif // wxUSE_TOOLTIPS

    void OnContextHelp(wxCommandEvent& event);

// In "PiscoSpeck2.cpp" since it uses wxGetApp():
    void DeleteAllThreads();
// OnClose, stop the threads:
    void OnClose(wxCloseEvent &event);

// In "asp2_main_utils.cpp":
    void Main_UpdateMessageBar(JLP_Decode *Decod2);
    void OnCameraDisplay1Timer(wxTimerEvent& event);
    void OnDecodeDisplay2Timer(wxTimerEvent& event);
    void RefreshDisplayWhenDecodeEvent();

// In "asp2_process.cpp"
    void UpdateToolbarLabel(wxString filename0);
    void OnStartProcessing(wxCommandEvent& event);
    void OnStopProcessing(wxCommandEvent& event);
    void OnStartStreaming(wxCommandEvent& event);
    void OnStopStreaming(wxCommandEvent& event);
    void StopProcessing();
    void StopStreaming();
    void OnReceiveDoneEventFromDecode(wxThreadEvent &event);
    void OnReceiveDoneEventFromCamera(wxThreadEvent &event);
    void OnReceiveStreamingEventFromCamera(wxThreadEvent &event);
    int StartProcessing();
    void StopIsRequired();
    int StartStreaming();
    void CreateDecodeThread();
    void OnEraseAll(wxCommandEvent &event);
    int EraseAllAfterProcessing();
    void EraseAllWhenProcessing();

// In "asp2_menu_rwfits.cpp":
    void OnSaveFramesTo3DFits(wxCommandEvent& event);
    void OnLoadProcessingResults(wxCommandEvent& event);
    void OnSaveProcessingResults(wxCommandEvent& event);
    int SaveProcessingResults();
    int SaveFramesTo3DFits();
    int PromptForComments(wxString &comments0);

// In "asp2_frame_menu.cpp":
    void CreateMyToolbar();
    void Gdp_SetupMenu();
    void Gdp_ResetAllOptionsOfImageMenu();
    void InitDisplaySettings(int itt_is_linear0, char *lut_type0);

// In "asp2_frame_menu_onlick.cpp":
    void OnShowStatistics(wxCommandEvent& event);
    void OnBispRestore(wxCommandEvent& event);
    void OnSelectDisplayOption(wxCommandEvent& event);
    void OnChangeLUT(wxCommandEvent& event);
    void OnChangeITT(wxCommandEvent& event);

// in "gdp_logbook.cpp":
    void OnViewLogbook(wxCommandEvent& event);
    void OnSaveLogbook(wxCommandEvent& event);
    void OnClearLogbook(wxCommandEvent& event);
    void OnCleanLogbook(wxCommandEvent& event);
    int SaveLogbook(wxString save_filename);
    void ShowLogbook();
    void HideLogbook();
    void ClearLogbook();
    void CleanLogbook();
    int WriteToLogbook(wxString str1, bool SaveToFile);
    int AddNewPointToLogbook(double xx, double yy, double value);

// In asp2_language.cpp
    int LoadMenuMessages();

// Camera display1 timer:
    void StopDisplayTimers(){
      if(m_camera_display1_timer != NULL) m_camera_display1_timer->Stop();
      };
    void RestartDisplayTimers(){
      if(m_camera_display1_timer != NULL)
        m_camera_display1_timer->Start(display1_timer_ms_delay, wxTIMER_CONTINUOUS);
      };


protected:

private:
  wxStatusBar *m_StatusBar;
  int initialized, camera_type1;
  int nx1, ny1, nz_processed, nz_sent;
  bool input_processed_data1;
  bool new_data_is_needed, Cube_is_loaded[2], Cube_is_processed;
  JLP_Camera1 *m_cam1;
  JLP_Decode *jlp_decode1;
  PROCESS_SETTINGS Pset1;
  AWORD *ImageCube[2];

// Toolbar:
  wxToolBar *m_toolbar;
  wxStaticText *DispStatic1;

// Menus:
  wxMenuBar *menu_bar;
  wxMenu *menuFile, *menuFileOpen, *menuFileSave, *menuDisplay;
  wxMenu *menuLog, *menuProcessing, *menuHelp;
  wxMenu *menuLUT, *menuITT;
  wxBoxSizer  *m_topsizer;
  wxString m_filename1, m_full_filename1;
  int m_iframe, m_nframes;

// Messages in 5 languages:
  int iLang;
  wxString Str0[NLANG][NMAX_MESSAGES];
  bool small_version1;

  wxTimer *m_camera_display1_timer;
  int display1_timer_ms_delay;
  int cooler_counter, cooler_counter_max;

// Notebook:
  wxBookCtrl *m_notebook;
  int i_NotebookPage;

// Logbook:
  wxString    m_Logbook;
  JLP_wxLogbook *jlp_Logbook;

// DisplayPanel:
  wxPanel *m_DisplayPanel;
  JLP_CameraDisplayMutex *m_CameraDisplayMutex;
  JLP_Display2Mutex *m_Display2Mutex;
  JLP_AspDispPanel *jlp_aspdisp_panel;
/*
* decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*/
  double *child_image1[NCHILDS_MAX];
  int nchild_image1, nx1_image1, ny1_image1;

// CameraPanel:
  wxPanel *m_CameraPanel;
  JLP_CameraPanel *jlp_camera_panel;

// Camera Display:
  JLP_CameraDisplay *m_CameraDisplay;
  double *short_exposure1;

// ProcessingPanel:
  wxPanel *m_SpeckProcessPanel;
  JLP_SpeckProcessPanel *jlp_speckprocess_panel;

// Critical section protects access to all of the fields below
  wxCriticalSection m_critsect;

// Threads:
  JLP_DecodeThread *m_decode_thread;
  JLP_CameraThread *m_camera_thread;

// old log target, using m_txtctrl
   wxLog *m_oldLogger;

  DECLARE_EVENT_TABLE()
};

#endif
