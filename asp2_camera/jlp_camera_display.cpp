/************************************************************************
* "jlp_camera_display.cpp"
* JLP_CameraDisplay class
*
* JLP
* Version 07/09/2015
*************************************************************************/
#include "jlp_camera_display.h"

static int MirrorImage_dble(double *ima, int nx, int ny, int MirrorMode);
/*******************************************************************************
* Constructor from AspFrame:
*******************************************************************************/
JLP_CameraDisplay::JLP_CameraDisplay(JLP_CAMERA_SETTINGS CamSet0, const int width0, const int height0)
                                   : wxFrame(NULL, wxID_ANY, wxT("Camera display"),
                                   wxPoint(-1, -1), wxSize(width0, height0),
  wxCAPTION | wxMAXIMIZE_BOX |wxMINIMIZE_BOX | wxSTAY_ON_TOP | wxRESIZE_BORDER
  | wxFRAME_TOOL_WINDOW)
{

// Transform coma into point for numbers:
 setlocale(LC_NUMERIC, "C");

// Smaller fonts (needed for Windows, to have a similar appearance as Linux)
// SetWindowVariant(wxWINDOW_VARIANT_SMALL);

 initialized = 0;

// Initialize temperature message to empty string:
 temperature_msg1 = wxT("");

// Copy Aset0 to private Aset1
 Copy_CameraSettings(CamSet0, &CamSet1);

// nx0_ROI, ny0_ROI will be used for ROI : here they are initialized with dummy values
 nx0_ROI = 0;
 ny0_ROI = 0;

// Setup panel
 DisplayPanelSetup(width0, height0);

 initialized = 1234;

// Load Dummy image to update nx0, ny0, etc
 CameraDisplay_LoadDummyImage();

return;
}
/**********************************************************************
* DisplayPanel setup
* Create DisplayPanel with image panels:
*
***********************************************************************/
void JLP_CameraDisplay::DisplayPanelSetup(const int width0, const int height0)
{
wxBoxSizer *main_vtop_sizer;
wxBoxSizer *hbox_sizer1, *vbox_sizer1;
int width1, height1, should_fit_inside0;
int widths[2];

// Status bar:
// Create a status bar with two fields at the bottom:
 m_StatusBar = CreateStatusBar(2);
// First field has a variable length, second has a fixed length:
 widths[0] = -1;
 widths[1] = 200;
 SetStatusWidths( 2, widths );

 main_vtop_sizer = new wxBoxSizer( wxVERTICAL );

// Create two areas with static text
 Static_Temperature = new wxStaticText(this, wxID_ANY,
                                     wxT("T=10 deg. C (not stabilized yet)"));
 Static_ImageInfo = new wxStaticText(this, wxID_ANY,
                                     wxT("Image max: 0. (no mirror effect)"));
 main_vtop_sizer->Add(Static_Temperature);
 main_vtop_sizer->Add(Static_ImageInfo, 0, wxTOP, 1);

// Size of subpanel:
 width1 = (width0 - 40);
 height1 = (height0 - 80);

vbox_sizer1 = new wxBoxSizer( wxVERTICAL );
hbox_sizer1 = new wxBoxSizer( wxHORIZONTAL );

// JLP2016: full frame should fit in sub-panel:
  should_fit_inside0 = 1;

// New scrollwindow for image display (and NULL for JLP_wxLogbook):
  pChildVisu1 = new JLP_wxImagePanel(this, NULL, m_StatusBar,
                                     20, 20, width1, height1,
                                     should_fit_inside0);

// Set default settings
  pChildVisu1->wxIP_SetLUT("pisco");
  pChildVisu1->wxIP_SetITT_type("MinMax");
//  pChildVisu1->SetITT_linear(is_linear);
  pChildVisu1->wxIP_SetITT_linear(0);

// wxEXPAND | xALL: horizontally strechable with borders all around
    vbox_sizer1->Add(pChildVisu1, 1, wxEXPAND | wxALL);
// 1: vertically strechable
    hbox_sizer1->Add(vbox_sizer1, 1, wxEXPAND);
//
  main_vtop_sizer->Add(hbox_sizer1, 1, wxEXPAND);

this->SetSizerAndFit(main_vtop_sizer);

return;
}
/*********************************************************************
* Display a dummy image to the screen
*
*********************************************************************/
void JLP_CameraDisplay::CameraDisplay_LoadDummyImage()
{
int i, nx0, ny0;
double *dble_image0;

nx0 = CamSet1.nx1;
ny0 = CamSet1.ny1;
dble_image0 = new double[nx0 * ny0];
for(i = 0; i < nx0 * ny0; i++) dble_image0[i] = 0.;

CameraDisplay_LoadImage(dble_image0, nx0, ny0);

return;
}
/*********************************************************************
* Display an image to the screen
*
*********************************************************************/
void JLP_CameraDisplay::CameraDisplay_LoadImage(double *dble_image0, int nx0, int ny0)
{

// Mirror effect:
 if(CamSet1.MirrorMode != 0)
   MirrorImage_dble(dble_image0, nx0, ny0, CamSet1.MirrorMode);

 pChildVisu1->wxIP_LoadImage(dble_image0, nx0, ny0);

// Show ROI on screen:
 DisplayROI();

return;
}
/*********************************************************************
* Load New settings (mainly used to update ROI display)
* nx0_ROI, ny0_ROI : size of ROI (used for dispaying white box)
*
*********************************************************************/
void JLP_CameraDisplay::LoadNewSettings(JLP_CAMERA_SETTINGS CamSet0, const int nx0_ROI_0,
                                       const int ny0_ROI_0)
{

// Copy current settings: CamSet0 to CamSet1:
 Copy_CameraSettings(CamSet0, &CamSet1);

 nx0_ROI = nx0_ROI_0;
 ny0_ROI = ny0_ROI_0;

// Show ROI on screen (useful to update center of image after streaming):
 DisplayROI();

return;
}
/****************************************************************
* Show ROI on screen:
*
*****************************************************************/
void JLP_CameraDisplay::DisplayROI()
{
int status, rr0, gg0, bb0, pen_width0 = 1, filled0 = 0;

double xstart, ystart, xend, yend;

if((nx0_ROI == 0) && (ny0_ROI == 0)) return;

 xstart = CamSet1.xc0 - (nx0_ROI / 2);
 ystart = CamSet1.yc0 - (ny0_ROI / 2);
 xend = xstart + nx0_ROI - 1;
 yend = ystart + ny0_ROI - 1;
// White:
 rr0 = 255; gg0 = 255; bb0 = 255;
 status = pChildVisu1->wxIP_DrawRectangle_UC(xstart, ystart, xend, yend, rr0, gg0, bb0, pen_width0, filled0);

/* DEBUG:
{
wxString buffer;
int nx0, ny0;

nx0 = CamSet1.nx1;
ny0 = CamSet1.ny1;
buffer.Printf(wxT("DisplayROI: nx0,ny0=%d,%d xc0,yc0=%d %d x1,y1= %.1f,%.1f, x2,y2= %.1f,%.1f status = %d"),
              nx0, ny0, CamSet1.xc0, CamSet1.yc0, xstart, ystart, xend, yend, status);
wxLogError(buffer);
}
*/

return;
}
/*********************************************************************
* Mirroring of elementary images
* MirrorMode: 0 if no change
*             1 if vertical mirror
*             2 if horizontal mirror
*             3 if horiz. and vertical mirror
*********************************************************************/
static int MirrorImage_dble(double *ima, int nx, int ny, int MirrorMode)
{
double *tmp;
register int i, j;

 if(MirrorMode < 1 || MirrorMode > 3) return(0);

 tmp = new double[nx * ny];
 for (i = 0; i < nx * ny; i++) tmp[i] = ima[i];

// Vertical mirror:
 if(MirrorMode == 1) {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[i + (ny - 1 - j) * nx];
      }
    }
// Horizontal mirror:
 } else if (MirrorMode == 2) {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[(nx - 1 - i) + j * nx];
      }
    }
// Horizontal and vertical mirror:
 } else {
    for (j = 0; j < ny; j++) {
      for (i = 0; i < nx; i++) {
        ima[i + j * nx] = tmp[(nx - 1 - i) + (ny - 1 - j) * nx];
      }
    }
 }

delete[] tmp;

return(0);
}
