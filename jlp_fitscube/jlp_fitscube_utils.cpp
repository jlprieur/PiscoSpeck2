/************************************************************************
* "jlp_fitscube_utils.h"
*
* JLP
* Version 17/08/2015
*************************************************************************/
#include "jlp_fitscube_utils.h"
#include "asp2_rw_config_files.h" // ReadString....

/****************************************************************************
* First initialization of FitsCube Settings (needed for before creating JLP_FitsCubeCam1)
****************************************************************************/
void FitsCubeSettingsToDefault(FITSCUBE_SETTINGS* out_RSet, int nx_full, int ny_full)
{

// FitsCubeCapabilities FitsCubeCaps;
// FitsCubeCapabilities structure
// out_RSet->FitsCubeCaps;
// Head Model
strcpy(out_RSet->Filename1, "");
// Detector dimensions
out_RSet->gblXPixels = nx_full;
out_RSet->gblYPixels = ny_full;
// Rotation mode: 0 for 0, 1 for +90, 2 for +180, 3 for +270
out_RSet->RotationMode = 0;
// MirrorMode: 0=none, 1=vertical, 2=horizontal 3=vertical+horizontal
out_RSet->MirrorMode = 0;
// Center of ROI subwindow
out_RSet->xc0 = out_RSet->gblXPixels/ 2;
out_RSet->yc0 = out_RSet->gblYPixels/ 2;
// Binning factors
out_RSet->xbin = 1;
out_RSet->ybin = 1;
// Size of elementary frames
out_RSet->nx1 = 128;
out_RSet->ny1 = 128;
out_RSet->nz1 = 50;
// Size of Fits cube:
out_RSet->nz_total = 0;
return;
}
/***************************************************************************
* Copy FITSCUBE_SETTINGS structure
*
* INPUT:
*  in_RSet
*
* OUTPUT:
*  out_RSet
****************************************************************************/
int Copy_FitsCubeSettings(FITSCUBE_SETTINGS in_RSet, FITSCUBE_SETTINGS* out_RSet)
{
strcpy(out_RSet->Filename1, in_RSet.Filename1);
out_RSet->gblXPixels = in_RSet.gblXPixels;
out_RSet->gblYPixels = in_RSet.gblYPixels;
out_RSet->is_initialized = in_RSet.is_initialized;
out_RSet->MirrorMode = in_RSet.MirrorMode;
out_RSet->RotationMode = in_RSet.RotationMode;
// JLP2015
// out_RSet->NExposures = in_RSet.NExposures;
out_RSet->xc0 = in_RSet.xc0;
out_RSet->yc0 = in_RSet.yc0;
out_RSet->nx1 = in_RSet.nx1;
out_RSet->ny1 = in_RSet.ny1;
out_RSet->nz1 = in_RSet.nz1;
out_RSet->nz_total = in_RSet.nz_total;
out_RSet->xbin = in_RSet.xbin;
out_RSet->ybin = in_RSet.ybin;

return(0);
}
/***************************************************************************
* Copy useful FITSCUBE_SETTINGS parameters to JLP_CAMERA_SETTINGS
*
* INPUT:
*  in_RSet
*
* OUTPUT:
*  out_CSet
****************************************************************************/
int Copy_FitsCubeToCameraSettings(FITSCUBE_SETTINGS in_RSet,
                                JLP_CAMERA_SETTINGS* out_CSet)
{
strcpy(out_CSet->HeadModel, in_RSet.Filename1);
out_CSet->gblXPixels = in_RSet.gblXPixels;
out_CSet->gblYPixels = in_RSet.gblYPixels;
out_CSet->is_initialized = in_RSet.is_initialized;
out_CSet->MirrorMode = in_RSet.MirrorMode;
out_CSet->RotationMode = in_RSet.RotationMode;
out_CSet->xc0 = in_RSet.xc0;
out_CSet->yc0 = in_RSet.yc0;
out_CSet->nx1 = in_RSet.nx1;
out_CSet->ny1 = in_RSet.ny1;
out_CSet->xbin = in_RSet.xbin;
out_CSet->ybin = in_RSet.ybin;

return(0);
}
/***************************************************************************
* Copy JLP_CAMERA_SETTINGS to some of the ANDOR_SETTINGS parameters
*
* INPUT:
*  in_CSet
*
* OUTPUT:
*  out_RSet
****************************************************************************/
int Copy_CameraToFitsCubeSettings(JLP_CAMERA_SETTINGS in_CamSet,
                                FITSCUBE_SETTINGS* out_RSet)
{
strcpy(out_RSet->Filename1, in_CamSet.HeadModel);
out_RSet->gblXPixels = in_CamSet.gblXPixels;
out_RSet->gblYPixels = in_CamSet.gblYPixels;
out_RSet->is_initialized = in_CamSet.is_initialized;
out_RSet->MirrorMode = in_CamSet.MirrorMode;
out_RSet->RotationMode = in_CamSet.RotationMode;
out_RSet->xc0 = in_CamSet.xc0;
out_RSet->yc0 = in_CamSet.yc0;
out_RSet->nx1 = in_CamSet.nx1;
out_RSet->ny1 = in_CamSet.ny1;
out_RSet->xbin = in_CamSet.xbin;
out_RSet->ybin = in_CamSet.ybin;

return(0);
}
//---------------------------------------------------------------------------
// Save FitsCube Settings to File
//---------------------------------------------------------------------------
int SaveFitsCubeSettingsToFile(char *filename, FITSCUBE_SETTINGS RSet1)
{
FILE *fp_out;
wxString buffer;

if((fp_out = fopen(filename,"w"))== NULL) {
  buffer.Printf(_T("Error opening output file >%s< \n"), filename);
  wxMessageBox(buffer, _T("SaveFitsCubeSettingsToFile/Error"),
               wxOK | wxICON_ERROR);
  return(-1);
  }

// Version 1 (oct 2017)
fprintf(fp_out, "FitsCubeSettingsV3 Configuration file\n");
fprintf(fp_out, "Filename1 = %s\n", RSet1.Filename1);
fprintf(fp_out, "gblXPixels = %d\n", RSet1.gblXPixels);
fprintf(fp_out, "gblYPixels = %d\n", RSet1.gblYPixels);
fprintf(fp_out, "MirrorMode = %d\n", RSet1.MirrorMode);
fprintf(fp_out, "RotationMode = %d\n", RSet1.RotationMode);
fprintf(fp_out, "xc0 = %d\n", RSet1.xc0);
fprintf(fp_out, "yc0 = %d\n", RSet1.yc0);
fprintf(fp_out, "nx1 = %d\n", RSet1.nx1);
fprintf(fp_out, "ny1 = %d\n", RSet1.ny1);
fprintf(fp_out, "xbin = %d\n", RSet1.xbin);
fprintf(fp_out, "ybin = %d\n", RSet1.ybin);

fclose(fp_out);

return(0);
}
//---------------------------------------------------------------------------
// Load FitsCube Settings from File
//---------------------------------------------------------------------------
int LoadFitsCubeSettingsFromFile(char *filename, FITSCUBE_SETTINGS *RSet1)
{
FILE *fp_in;
wxString err_message;
char buffer[128];

if((fp_in = fopen(filename,"r"))== NULL) {
  err_message.Printf(_T("Error opening input configuration file >%s< \n"),
                     filename);
  wxMessageBox(err_message, "LoadFitsCubeSettingsFromFile/Error",
               wxOK | wxICON_ERROR);
  return(-1);
  }
// Read the first line to see if it is a good file:
  fgets(buffer, 128, fp_in);
// Version 1 (feb 2011): FitsCubeSettingsV1
// Version 2 (aug 2015): FitsCubeSettingsV2 (I added "BaselineClamp")
  if(strncmp(buffer, "FitsCubeSettingsV", 14)) {
   wxMessageBox(_T("Invalid format!"), _T("LoadFitsCubeSettingsFromFile/Error"),
                wxOK | wxICON_ERROR);
   fclose(fp_in);
   return(-2);
   }
fclose(fp_in);

ReadStringFromConfigFile(filename, "Filenamel", RSet1->Filename1);
ReadIntFromConfigFile(filename, "gblXPixels", &RSet1->gblXPixels);
ReadIntFromConfigFile(filename, "gblYPixels", &RSet1->gblYPixels);
ReadIntFromConfigFile(filename, "MirrorMode", &RSet1->MirrorMode);
ReadIntFromConfigFile(filename, "RotationMode", &RSet1->RotationMode);
ReadIntFromConfigFile(filename, "xc0", &RSet1->xc0);
ReadIntFromConfigFile(filename, "yc0", &RSet1->yc0);
ReadIntFromConfigFile(filename, "nx1", &RSet1->nx1);
ReadIntFromConfigFile(filename, "ny1", &RSet1->ny1);
ReadIntFromConfigFile(filename, "xbin", &RSet1->xbin);
ReadIntFromConfigFile(filename, "ybin", &RSet1->ybin);

// Display new values:
//  DisplayNewSettings();

return(0);
}
/***********************************************************************
* Write information specific to FitsCube camera to FITS descriptors
*
***********************************************************************/
int Write_FitsCubeSettingsToFITSDescriptors(DESCRIPTORS *descrip,
                                         FITSCUBE_SETTINGS FitsCubeSet1)
{
int k, rot_angle;

 // Information about the FITSCUBE camera and its settings:

// Head model: (e.g. DV885)
// sprintf(buf,"FITSCUBE0  = \'Head model: %20.40s\' //", FitsCubeSet1.HeadModel);
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "FITSCUBE0 ");
 sprintf((descrip->descr[k]).cvalue, "Filename1: %20.40s", FitsCubeSet1.Filename1);
 (descrip->ndescr)++;

// Geometrical parameters:
 rot_angle = FitsCubeSet1.RotationMode * 90;
// sprintf(buf,"FITSCUBE1  = \'xbin=%d ybin=%d xc=%d, yc=%d, rot=%d deg, mirror=%d\' //",
//         Aset.xbin, Aset.ybin, Aset.xc0, Aset.yc0, rot_angle,
//         Aset.MirrorMode);
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "FITSCUBE1 ");
 sprintf((descrip->descr[k]).cvalue, "xbin=%d ybin=%d xc=%d, yc=%d, rot=%d deg, mirror=%d",
         FitsCubeSet1.xbin, FitsCubeSet1.ybin, FitsCubeSet1.xc0, FitsCubeSet1.yc0, rot_angle, FitsCubeSet1.MirrorMode);
 (descrip->ndescr)++;

return(0);
}
