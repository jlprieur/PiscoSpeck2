/****************************************************************************
* PiscoSpeck2.h
* Application project for PiscoSpeck2.cpp (AspFrame class)
* Purpose: display and process FITS images obtained with PISCO and PISCO2
*
* JLP
* Version 05/09/2015
****************************************************************************/
#ifndef _raptor_speck2_h_
#define _raptor_speck2_h_

// For compilers that support precompilation, includes "wx/wx.h".
// #include "wx/wxprec.h"

#include "wx/wx.h"
#include "wx/thread.h"
#include <wx/app.h> // To declare wxGetApp()

//----------------------------------------------------------------------
// Asp2_App : the application object (JLP's version for threads)
//----------------------------------------------------------------------

class Asp2_App: public wxApp
{
public:
   Asp2_App();
   virtual ~Asp2_App(){};
   virtual bool OnInit();

// critical section protects access to all of the fields below
    wxCriticalSection m_critsect;

// semaphore used to wait for the threads to exit, see AspFrame::OnQuit()
    wxSemaphore m_sem_andor_done, m_sem_decode_done, m_sem_3dfits_done;

// indicates that we're shutting down and threads should exit
    bool m_andor_shuttingDown, m_decode_shuttingDown, m_3dfits_shuttingDown;

};

#endif
