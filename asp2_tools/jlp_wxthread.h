/*****************************************************************************
* jlp_wxthread.cpp
*
* From wxWidgets thread sample
* by Guilhem Lavaux, Vadim Zeitlin version of 06/16/98
*
* JLP version 16/08/2015
*
*****************************************************************************/
#ifndef _jlp_wxthread_
#define _jlp_wxthread_

// ===========================================================================
// Declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// Headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if !wxUSE_THREADS
    #error "This sample requires thread support!"
#endif // wxUSE_THREADS

#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"

// ----------------------------------------------------------------------------
// Private classes
// ----------------------------------------------------------------------------

class MyThread;
WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);

// ----------------------------------------------------------------------------
// the application object
// ----------------------------------------------------------------------------

class MyApp : public wxApp
{
public:
    MyApp();
    virtual ~MyApp(){};
    virtual bool OnInit();

// critical section protects access to all of the fields below
    wxCriticalSection m_critsect;

// all the threads currently alive - as soon as the thread terminates, it's
// removed from the array
    wxArrayThread m_threads;

// semaphore used to wait for the threads to exit, see MyFrame::OnQuit()
    wxSemaphore m_semAllDone;

// indicates that we're shutting down and all threads should exit
    bool m_shuttingDown;
};

// ----------------------------------------------------------------------------
// the main application frame
// ----------------------------------------------------------------------------

class MyFrame : public wxFrame,
                private wxLog
{
public:
    // ctor
    MyFrame(const wxString& title);
    virtual ~MyFrame();

    // accessors for MyWorkerThread (called in its context!)
    bool Cancelled();

protected:
    virtual void DoLogRecord(wxLogLevel level,
                             const wxString& msg,
                             const wxLogRecordInfo& info);

private:
    // event handlers
    // --------------

    void OnQuit(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);

    void OnStartThread(wxCommandEvent& event);
    void OnStartThreads(wxCommandEvent& event);
    void OnStopThread(wxCommandEvent& event);
    void OnPauseThread(wxCommandEvent& event);
    void OnResumeThread(wxCommandEvent& event);

    void OnStartWorker(wxCommandEvent& event);
    void OnExecMain(wxCommandEvent& event);
    void OnStartGUIThread(wxCommandEvent& event);

    void OnShowCPUs(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnIdle(wxIdleEvent &event);
    void OnWorkerEvent(wxThreadEvent& event);
    void OnUpdateWorker(wxUpdateUIEvent& event);


    // logging helper
    void DoLogLine(wxTextCtrl *text,
                   const wxString& timestr,
                   const wxString& threadstr,
                   const wxString& msg);


    // thread helper functions
    // -----------------------

    // helper function - creates a new thread (but doesn't run it)
    MyThread *CreateThread();

    // update display in our status bar: called during idle handling
    void UpdateThreadStatus();


    // internal variables
    // ------------------

    // just some place to put our messages in
    wxTextCtrl *m_txtctrl;

    // old log target, we replace it with one using m_txtctrl during this
    // frame life time
    wxLog *m_oldLogger;

    // the array of pending messages to be displayed and the critical section
    // protecting it
    wxArrayString m_messages;
    wxCriticalSection m_csMessages;

    // remember the number of running threads and total number of threads
    size_t m_nRunning,
           m_nCount;

    // the progress dialog which we show while worker thread is running
    wxProgressDialog *m_dlgProgress;

    // was the worker thread cancelled by user?
    bool m_cancelled;
    wxCriticalSection m_csCancelled;        // protects m_cancelled

    wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// ID for the menu commands
enum
{
    THREAD_QUIT  = wxID_EXIT,
    THREAD_ABOUT = wxID_ABOUT,
    THREAD_TEXT          = 101,
    THREAD_CLEAR,
    THREAD_START_THREAD  = 201,
    THREAD_START_THREADS,
    THREAD_STOP_THREAD,
    THREAD_PAUSE_THREAD,
    THREAD_RESUME_THREAD,

    THREAD_START_WORKER,
    THREAD_EXEC_MAIN,
    THREAD_START_GUI_THREAD,

    THREAD_SHOWCPUS,

    WORKER_EVENT = wxID_HIGHEST+1,   // this one gets sent from MyWorkerThread
    GUITHREAD_EVENT                  // this one gets sent from MyGUIThread
};

// ----------------------------------------------------------------------------
// a simple thread
// ----------------------------------------------------------------------------

class MyThread : public wxThread
{
public:
    MyThread();
    virtual ~MyThread();

    // thread execution starts here
    virtual void *Entry();

public:
    unsigned m_count;
};

// ----------------------------------------------------------------------------
// a worker thread
// ----------------------------------------------------------------------------

class MyWorkerThread : public wxThread
{
public:
    MyWorkerThread(MyFrame *frame);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

public:
    MyFrame *m_frame;
    unsigned m_count;
};

// ----------------------------------------------------------------------------
// a thread which executes GUI calls using wxMutexGuiEnter/Leave
// ----------------------------------------------------------------------------

#define GUITHREAD_BMP_SIZE          300
#define GUITHREAD_NUM_UPDATES       50
class MyImageDialog;

class MyGUIThread : public wxThread
{
public:
    MyGUIThread(MyImageDialog *dlg) : wxThread(wxTHREAD_JOINABLE)
    {
        m_dlg = dlg;
    }

    virtual ExitCode Entry();

private:
    MyImageDialog *m_dlg;
};

// ----------------------------------------------------------------------------
// an helper dialog used by MyFrame::OnStartGUIThread
// ----------------------------------------------------------------------------

class MyImageDialog: public wxDialog
{
public:
    // ctor
    MyImageDialog(wxFrame *frame);
    ~MyImageDialog();

    // stuff used by MyGUIThread:
    wxBitmap m_bmp;    // the bitmap drawn by MyGUIThread
    wxCriticalSection m_csBmp;        // protects m_bmp

private:
    void OnGUIThreadEvent(wxThreadEvent& event);
    void OnPaint(wxPaintEvent&);

    MyGUIThread m_thread;
    int m_nCurrentProgress;

    wxDECLARE_EVENT_TABLE();
};

#endif  // EOF sentry
