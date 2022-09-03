/****************************************************************************
* Name: asp2_frame_id.h  (AspFrame class for PiscoSpeck2)
* Purpose: display and process FITS images obtained with PISCO and PISCO2
*
* list of ID used by AspFrame class
*
* JLP
* Version 22/02/2016
****************************************************************************/
#ifndef _asp2_frame_id_h_
#define _asp2_frame_id_h_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//---------------------------------------------------------------------
enum
{
  ID_QUIT        = wxID_EXIT,

// File:
  ID_LOAD_RESULTS2 = 1000,
  ID_LOAD_BISPECTRUM,
  ID_SAVE_TO_3DFITS,
  ID_SAVE_RESULTS2,

// Toolbar buttons:
  ID_START_PROCESS,
  ID_STOP_PROCESS,
  ID_START_STREAM,
  ID_STOP_STREAM,
  ID_SAVE_RESULTS,
  ID_STATISTICS,
  ID_BISP_RESTORE,
  ID_ERASE_ALL,

// Menu/Notebook:
  ID_NOTEBOOK,
  ID_APANEL_PAGE,
  ID_PROCESS_PAGE,
  ID_DISPLAY_PAGE,

// Camera display timer:
  ID_CAMDISP_TIMER,

// Context menu
  ID_CONTEXT_HELP,

// Display menu
  ID_DISPMENU_DEFAULT,
  ID_DISPMENU_OFFSET,
  ID_DISPMENU_FFIELD,
  ID_DISPMENU_UNRES_AUTOC,
  ID_DISPMENU_UNRES_MODSQ,
  ID_DISPMENU_OPT4,
  ID_DISPMENU_OPT5,
  ID_DISPMENU_OPT6,
  ID_DISPMENU_OPT7,
  ID_DISPMENU_OPT8,
  ID_DISPMENU_OPT9,
  ID_DISPMENU_OPT10,

// ITT: LIN, LOG
  ID_ITT_LIN,
  ID_ITT_LOG,

// LUT: RAIN1, RAIN2, SAW, GRAY, CUR, PISCO
  ID_LUT_NONE,
  ID_LUT_RAIN1,
  ID_LUT_RAIN2,
  ID_LUT_SAW,
  ID_LUT_GRAY,
  ID_LUT_CUR,
  ID_LUT_PISCO,

// Process menu
  ID_PROCMENU_BISP_RESTORE,

// Logbook menu
  ID_LOGBOOK_SHOW,
  ID_LOGBOOK_HIDE,
  ID_LOGBOOK_CLEAR,
  ID_LOGBOOK_CLEAN,
  ID_LOGBOOK_SAVE,

// Threads:
  ID_CAMERA_THREAD,
  ID_DECODE_THREAD,
  ID_DECODE_THREAD_DONE,
  ID_CAMERA_THREAD_DONE,
  ID_CAMERA_THREAD_STREAM,

// Help:
  ID_ABOUT = wxID_ABOUT,
  ID_HELP  = wxID_HELP
};

// Declare new types of event, to be used by our wxThread class:

#endif
