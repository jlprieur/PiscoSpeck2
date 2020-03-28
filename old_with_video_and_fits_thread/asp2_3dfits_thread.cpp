/*****************************************************************************
* asp2_3dfits_thread.cpp
*
* JLP_3DFits_Thread
*
* JLP version 06/09/2015
*****************************************************************************/
#include "asp2_3dfits_thread.h"        // JLP_3DFitsThread()...
#include "asp2_frame_id.h"         // ID_3DFITS_THREAD_DONE

/*
#define DEBUG
*/

/****************************************************************************
* JLP_3DFitsThread
* Constructor
****************************************************************************/
JLP_3DFitsThread::JLP_3DFitsThread(AspFrame *asp_frame)
                                  : wxThread(wxTHREAD_DETACHED)
{
 m_aspframe = asp_frame;
 inFITSFile1 = NULL;

// Dummy values for nx1, ny1, nz1:
 nx1 = 8;
 ny1 = 8;
 nz1 = 2;
 nz_read = 0;
 nz_total = nz1;
 ImageCube1 = NULL;
 cube_to_be_loaded = false;

}
/****************************************************************************
* Setup before new processing
*
* INPUT:
*   nz0 : number of planes in an elementary data cube
****************************************************************************/
int JLP_3DFitsThread::Setup(char *inFITS_filename0,  int *nx0, int *ny0,
                            const int nz0)
{
int status;

// Delete old object:
 if(inFITSFile1 != NULL) {
  delete inFITSFile1;
  inFITSFile1 = NULL;
 }

// Create new object:
 inFITSFile1 = new JLP_3D_inFITSFile(inFITS_filename0);

// If error, display an error message:
 if(!inFITSFile1->IsOpen()) {
   wxMessageBox(wxT("Error opening FITS cube"), wxT("JLP_3DFitsThread/Setup"),
                wxOK | wxICON_ERROR);
   delete inFITSFile1;
   inFITSFile1 = NULL;
   if(ImageCube1 != NULL) delete[] ImageCube1;
   ImageCube1 = NULL;
   status = -1;
 } else {
   nx1 = inFITSFile1->Nx();
   ny1 = inFITSFile1->Ny();
   nz1 = nz0;
   nz_read = 0;
   nz_total = inFITSFile1->Nz_Total();
   if(ImageCube1 != NULL) delete[] ImageCube1;
   ImageCube1 = new AWORD[nx1 * ny1 * nz1];
   status = 0;
 }

// Copy to output values before exiting from this routine:
 *nx0 = nx1;
 *ny0 = ny1;

 cube_to_be_loaded = false;

return(status);
}
/****************************************************************************
* Called when the thread exits - whether it terminates normally or is
* stopped with Delete() (but not when it is Kill()ed!)
*
****************************************************************************/
void JLP_3DFitsThread::OnExit()
{
 if(inFITSFile1 != NULL) delete inFITSFile1;
 inFITSFile1 = NULL;
 if(ImageCube1 != NULL) delete[] ImageCube1;
 ImageCube1 = NULL;
}

/****************************************************************************
* Thread execution starts here
*
****************************************************************************/
wxThread::ExitCode JLP_3DFitsThread::Entry()
{
#ifdef DEBUG
long iloop = 0;
wxString buffer;
#endif

  while( !TestDestroy()) {
#ifdef DEBUG
// Display immediately after testing if it is still alive !
    buffer.Printf(_T("JLP_3DFitsThread (iloop = %ld)."), iloop);
    wxLogMessage(buffer);
    iloop++;
#endif
// Sleep or work...
    if(cube_to_be_loaded) {
      LoadCube();
      } else {
      Sleep(10);
      }
  }

return (wxThread::ExitCode)0;  // Success
}
/****************************************************************************
* Start loading the cubes
*
****************************************************************************/
void JLP_3DFitsThread::StartLoadingCubeForProcessing()
{
cube_to_be_loaded = true;
}
/****************************************************************************
* Load cube
*
****************************************************************************/
void JLP_3DFitsThread::LoadCube()
{
char err_messg[128];
wxThreadEvent  *CubeIsLoadedEvent;
wxString buffer;

if(inFITSFile1 != NULL && ImageCube1 != NULL) {
// If file not fully read, read a new cube, otherwise, simply send the last cube a few times
// (to have a similar behaviour as the andor thread: the process should be stopped
// by asp2_process.cpp when receiving the last event from the decode thread)
  if(nz_read <= nz_total - nz1) {
    inFITSFile1->Read_aword_cube_from_3DFitsFile(ImageCube1, nx1, ny1, nz1,
                                                 err_messg);
    nz_read += nz1;
    }
  } else {
  buffer.Printf(wxT("Fatal error: unitialized thread !\n"));
  wxMessageBox(buffer, wxT("JLP_3DFitsThread::LoadCube"), wxOK | wxICON_ERROR);
  exit(-1);
  }

// Create new event: can be used only once since it is deleted by wxQueueEvent
CubeIsLoadedEvent = new wxThreadEvent(wxEVT_THREAD, ID_3DFITS_THREAD_DONE);

// Send event to AspFrame:
  wxQueueEvent( (wxFrame*)m_aspframe, CubeIsLoadedEvent);

cube_to_be_loaded = false;
}
/****************************************************************************
* Retrieve results
*
****************************************************************************/
int JLP_3DFitsThread::GetResults(AWORD *ImageCube0, const int nx0,
                                const int ny0, const int nz0)
{
wxString buffer;
int i;

 if(ImageCube1 == NULL || inFITSFile1 == NULL) {
  wxMessageBox(wxT("Fatal error: file nor opened !\n"),
               wxT("JLP_3DFitsThread::GetResults"), wxOK | wxICON_ERROR);
  exit(-1);
  }


if(nx0 != nx1 || ny0 != ny1 || nz0 != nz1) {
  buffer.Printf(wxT("Incompatible size: nx0,ny0=%d,%d nx1,ny1=%d,%d !\n"),
                nx0, ny0, nx1, ny1),
  wxMessageBox(buffer, wxT("JLP_3DFitsThread::GetResults/Error"), wxOK | wxICON_ERROR);
  exit(-1);
  }


if(cube_to_be_loaded) {
  wxLogError(wxT("3DFitsThread/Warning can't get results: cube not fully loaded yet!\n"));
  return(-1);
  }

for(i = 0; i < nx0 * ny0 * nz0; i++) ImageCube0[i] = ImageCube1[i];

return(0);
}
