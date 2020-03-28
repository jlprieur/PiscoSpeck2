/************************************************************************
* "jlp_andor_utils.h"
*
* JLP
* Version 17/08/2015
*************************************************************************/
#include "jlp_andor_utils.h"
#include "asp2_rw_config_files.h" // ReadString....

/****************************************************************************
* First initialization of Andor Settings (needed for before creating JLP_AndorCam1)
****************************************************************************/
void AndorSettingsToDefault(ANDOR_SETTINGS* out_ASet, int nx_full, int ny_full)
{

// AndorCapabilities AndorCaps;
// AndorCapabilities structure
// out_ASet->AndorCaps;
// Head Model
strcpy(out_ASet->HeadModel, "DU897_BVF");
// Detector dimensions
out_ASet->gblXPixels = nx_full;
out_ASet->gblYPixels = ny_full;
// Analog-Digital converter channel
out_ASet->AD_Channel = 0;
// Vertical shift speed
out_ASet->VShiftSpeed = 1;
// Vertical shift voltage
out_ASet->VShiftVoltage = 3;
// Horizontal shift pre-ampli gain
out_ASet->HShiftPreAmpGain = 0;
// Horizontal shift speed
out_ASet->HShiftSpeed = 1;
// 5 for continuous streaming
out_ASet->AcquisitionMode = 5;
// 4 for imaging mode
out_ASet->readMode = 4;
// 0 for internal, 10 for software
out_ASet->TriggerMode = 0;
// Shutter: 0=auto 1=open 2=closed
out_ASet->ShutterMode = 1;
// Rotation mode: 0 for 0, 1 for +90, 2 for +180, 3 for +270
out_ASet->RotationMode = 0;
// MirrorMode: 0=none, 1=vertical, 2=horizontal 3=vertical+horizontal
out_ASet->MirrorMode = 0;
// EMCCD gain mode (0=[0,255] or 1=[0,4096])
out_ASet->EMGainMode = 1;
// EMCCD gain allowed mode (0=[0,255] or 1=[0,4096])
out_ASet->EMGainBasicMode = 1;
// Value of EMCCD gain
out_ASet->EMGainValue = 3300;
// Smallest value of EMCCD gain
out_ASet->EMGainLowValue = 0;
// Largest value of EMCCD gain
out_ASet->EMGainHighValue = 200;
// Frame transfer Mode (0/1)
out_ASet->FrameTransferMode = 1;
// Baseline Clamp (0/1)
out_ASet->BaselineClamp = 0;
// CloseShutterWhenExit (0/1)
out_ASet->CloseShutterWhenExit = 1;
// Center of subwindow
out_ASet->xc0 = out_ASet->gblXPixels / 2;
out_ASet->yc0 = out_ASet->gblYPixels / 2;
// Binning factors
out_ASet->xbin = 1;
out_ASet->ybin = 1;
// Size of elementary frames
out_ASet->nx1 = 128;
out_ASet->ny1 = 128;
// Long exposure time in seconds
out_ASet->IntegTime_sec = 10;
// Kinetic: exposure time in milliseconds
out_ASet->KineticExpoTime_msec = 10;
// Kinetic: clock time in milliseconds
out_ASet->KineticClockTime_msec = 11;
// Kinetic: cycle time in milliseconds
out_ASet->KineticCycleTime_msec = 12;
// Kinetic: number of exposures/cycle (for accumulation)
out_ASet->KineticCycleNExposures = 1;
// Kinetic: number of cycles (for accumulation)
out_ASet->KineticNCycles = 100;
// Flag set to true if Cooler is wanted
out_ASet->CoolerOn = 1;
// Cooler temperature when Cooler is on
out_ASet->CoolerSetTemp = 10.;
// WORD *pImageArray : main image buffer read from card
out_ASet->pImageArray = NULL;
out_ASet->ImageSize = 0;
return;
}
/***************************************************************************
* Copy ANDOR_SETTINGS structure
*
* INPUT:
*  in_ASet
*
* OUTPUT:
*  out_ASet
****************************************************************************/
int Copy_AndorSettings(ANDOR_SETTINGS in_ASet, ANDOR_SETTINGS* out_ASet)
{
out_ASet->AndorCaps = in_ASet.AndorCaps;
strcpy(out_ASet->HeadModel, in_ASet.HeadModel);
out_ASet->gblXPixels = in_ASet.gblXPixels;
out_ASet->gblYPixels = in_ASet.gblYPixels;
out_ASet->AD_Channel = in_ASet.AD_Channel;
out_ASet->VShiftSpeed = in_ASet.VShiftSpeed;
out_ASet->VShiftVoltage = in_ASet.VShiftVoltage;
out_ASet->HShiftPreAmpGain = in_ASet.HShiftPreAmpGain;
out_ASet->HShiftSpeed = in_ASet.HShiftSpeed;
out_ASet->is_initialized = in_ASet.is_initialized;
out_ASet->AcquisitionMode = in_ASet.AcquisitionMode;
out_ASet->readMode = in_ASet.readMode;
out_ASet->TriggerMode = in_ASet.TriggerMode;
out_ASet->ShutterMode = in_ASet.ShutterMode;
out_ASet->MirrorMode = in_ASet.MirrorMode;
out_ASet->RotationMode = in_ASet.RotationMode;
out_ASet->EMGainMode = in_ASet.EMGainMode;
out_ASet->EMGainBasicMode = in_ASet.EMGainBasicMode;
out_ASet->EMGainValue = in_ASet.EMGainValue;
out_ASet->EMGainLowValue = in_ASet.EMGainLowValue;
out_ASet->EMGainHighValue = in_ASet.EMGainHighValue;
out_ASet->FrameTransferMode = in_ASet.FrameTransferMode;
out_ASet->BaselineClamp = in_ASet.BaselineClamp;
out_ASet->CloseShutterWhenExit = in_ASet.CloseShutterWhenExit;
// JLP2015
// out_ASet->NExposures = in_ASet.NExposures;
out_ASet->xc0 = in_ASet.xc0;
out_ASet->yc0 = in_ASet.yc0;
out_ASet->nx1 = in_ASet.nx1;
out_ASet->ny1 = in_ASet.ny1;
out_ASet->xbin = in_ASet.xbin;
out_ASet->ybin = in_ASet.ybin;
out_ASet->CoolerOn = in_ASet.CoolerOn;
out_ASet->CoolerSetTemp = in_ASet.CoolerSetTemp;
out_ASet->IntegTime_sec = in_ASet.IntegTime_sec;
out_ASet->KineticExpoTime_msec = in_ASet.KineticExpoTime_msec;
out_ASet->KineticClockTime_msec = in_ASet.KineticClockTime_msec;
out_ASet->KineticCycleTime_msec = in_ASet.KineticCycleTime_msec;
out_ASet->KineticCycleNExposures = in_ASet.KineticCycleNExposures;
out_ASet->KineticNCycles = in_ASet.KineticNCycles;
out_ASet->pImageArray = in_ASet.pImageArray;

return(0);
}
/***************************************************************************
* Copy useful ANDOR_SETTINGS parameters to JLP_CAMERA_SETTINGS
*
* INPUT:
*  in_ASet
*
* OUTPUT:
*  out_CSet
****************************************************************************/
int Copy_AndorToCameraSettings(ANDOR_SETTINGS in_ASet, JLP_CAMERA_SETTINGS* out_CSet)
{
strcpy(out_CSet->HeadModel, in_ASet.HeadModel);
out_CSet->gblXPixels = in_ASet.gblXPixels;
out_CSet->gblYPixels = in_ASet.gblYPixels;
out_CSet->is_initialized = in_ASet.is_initialized;
out_CSet->AcquisitionMode = in_ASet.AcquisitionMode;
out_CSet->ShutterMode = in_ASet.ShutterMode;
out_CSet->MirrorMode = in_ASet.MirrorMode;
out_CSet->RotationMode = in_ASet.RotationMode;
out_CSet->CloseShutterWhenExit = in_ASet.CloseShutterWhenExit;
out_CSet->xc0 = in_ASet.xc0;
out_CSet->yc0 = in_ASet.yc0;
out_CSet->nx1 = in_ASet.nx1;
out_CSet->ny1 = in_ASet.ny1;
out_CSet->xbin = in_ASet.xbin;
out_CSet->ybin = in_ASet.ybin;
out_CSet->CoolerOn = in_ASet.CoolerOn;
out_CSet->CoolerSetTemp = in_ASet.CoolerSetTemp;
out_CSet->IntegTime_sec = in_ASet.IntegTime_sec;
out_CSet->ExpoTime_msec = in_ASet.KineticExpoTime_msec;

return(0);
}
/***************************************************************************
* Copy JLP_CAMERA_SETTINGS to some of the ANDOR_SETTINGS parameters
*
* INPUT:
*  in_CSet
*
* OUTPUT:
*  out_ASet
****************************************************************************/
int Copy_CameraToAndorSettings(JLP_CAMERA_SETTINGS in_CamSet, ANDOR_SETTINGS* out_ASet)
{
strcpy(out_ASet->HeadModel, in_CamSet.HeadModel);
out_ASet->gblXPixels = in_CamSet.gblXPixels;
out_ASet->gblYPixels = in_CamSet.gblYPixels;
out_ASet->is_initialized = in_CamSet.is_initialized;
out_ASet->AcquisitionMode = in_CamSet.AcquisitionMode;
out_ASet->ShutterMode = in_CamSet.ShutterMode;
out_ASet->MirrorMode = in_CamSet.MirrorMode;
out_ASet->RotationMode = in_CamSet.RotationMode;
out_ASet->CloseShutterWhenExit = in_CamSet.CloseShutterWhenExit;
out_ASet->xc0 = in_CamSet.xc0;
out_ASet->yc0 = in_CamSet.yc0;
out_ASet->nx1 = in_CamSet.nx1;
out_ASet->ny1 = in_CamSet.ny1;
out_ASet->xbin = in_CamSet.xbin;
out_ASet->ybin = in_CamSet.ybin;
out_ASet->CoolerOn = in_CamSet.CoolerOn;
out_ASet->CoolerSetTemp = in_CamSet.CoolerSetTemp;
out_ASet->IntegTime_sec = in_CamSet.IntegTime_sec;
out_ASet->KineticExpoTime_msec = in_CamSet.ExpoTime_msec;

return(0);
}
//---------------------------------------------------------------------------
// Save Andor Settings to File
//---------------------------------------------------------------------------
int SaveAndorSettingsToFile(char *filename, ANDOR_SETTINGS ASet1)
{
FILE *fp_out;
wxString buffer;

if((fp_out = fopen(filename,"w"))== NULL) {
  buffer.Printf(_T("Error opening output file >%s< \n"), filename);
  wxMessageBox(buffer, _T("SaveAndorSettingsToFile/Error"),
               wxOK | wxICON_ERROR);
  return(-1);
  }

// Version 1 (feb 2015)
// Version 2 (aug 2015) with BaselineClamp and without NExposures
// Version 3 (aug 2015) with CloseShutterOnExit
// Version 4 (oct 2017) with HeadModel
fprintf(fp_out, "AndorSettingsV3 Configuration file\n");
fprintf(fp_out, "HeadModel = %s\n", ASet1.HeadModel);
fprintf(fp_out, "gblXPixels = %d\n", ASet1.gblXPixels);
fprintf(fp_out, "gblYPixels = %d\n", ASet1.gblYPixels);
fprintf(fp_out, "AD_Channel = %d\n", ASet1.AD_Channel);
fprintf(fp_out, "VShiftSpeed = %d\n", ASet1.VShiftSpeed);
fprintf(fp_out, "VShiftVoltage = %d\n", ASet1.VShiftVoltage);
fprintf(fp_out, "HShiftPreAmpGain = %d\n", ASet1.HShiftPreAmpGain);
fprintf(fp_out, "HShiftSpeed = %d\n", ASet1.HShiftSpeed);
fprintf(fp_out, "AcquisitionMode = %d\n", ASet1.AcquisitionMode);
fprintf(fp_out, "TriggerMode = %d\n", ASet1.TriggerMode);
fprintf(fp_out, "ShutterMode = %d\n", ASet1.ShutterMode);
fprintf(fp_out, "MirrorMode = %d\n", ASet1.MirrorMode);
fprintf(fp_out, "RotationMode = %d\n", ASet1.RotationMode);
fprintf(fp_out, "EMGainMode = %d\n", ASet1.EMGainMode);
fprintf(fp_out, "EMGainValue = %d\n", ASet1.EMGainValue);
fprintf(fp_out, "FrameTransferMode = %d\n", ASet1.FrameTransferMode);
fprintf(fp_out, "BaselineClamp = %d\n", ASet1.BaselineClamp);
fprintf(fp_out, "CloseShutterWhenExit = %d\n", ASet1.CloseShutterWhenExit);
// JLP2015
// fprintf(fp_out, "NExposures = %d\n", ASet1.NExposures);
fprintf(fp_out, "xc0 = %d\n", ASet1.xc0);
fprintf(fp_out, "yc0 = %d\n", ASet1.yc0);
fprintf(fp_out, "nx1 = %d\n", ASet1.nx1);
fprintf(fp_out, "ny1 = %d\n", ASet1.ny1);
fprintf(fp_out, "xbin = %d\n", ASet1.xbin);
fprintf(fp_out, "ybin = %d\n", ASet1.ybin);
fprintf(fp_out, "CoolerOn = %d\n", (int)ASet1.CoolerOn);
fprintf(fp_out, "CoolerSetTemp = %.2f\n", ASet1.CoolerSetTemp);
fprintf(fp_out, "IntegTime_sec = %d\n", ASet1.IntegTime_sec);
fprintf(fp_out, "KineticExpoTime_msec = %d\n", ASet1.KineticExpoTime_msec);
fprintf(fp_out, "KineticClockTime_msec = %d\n", ASet1.KineticClockTime_msec);
fprintf(fp_out, "KineticCycleTime_msec = %d\n", ASet1.KineticCycleTime_msec);
fprintf(fp_out, "KineticCycleNExposures = %d\n", ASet1.KineticCycleNExposures);
fprintf(fp_out, "KineticNCycles = %d\n", ASet1.KineticNCycles);

fclose(fp_out);

return(0);
}
//---------------------------------------------------------------------------
// Load Andor Settings from File
//---------------------------------------------------------------------------
int LoadAndorSettingsFromFile(char *filename, ANDOR_SETTINGS *ASet1)
{
FILE *fp_in;
wxString err_message;
char buffer[128];

if((fp_in = fopen(filename,"r"))== NULL) {
  err_message.Printf(_T("Error opening input configuration file >%s< \n"),
                     filename);
  wxMessageBox(err_message, "LoadAndorSettingsFromFile/Error",
               wxOK | wxICON_ERROR);
  return(-1);
  }
// Read the first line to see if it is a good file:
  fgets(buffer, 128, fp_in);
// Version 1 (feb 2011): AndorSettingsV1
// Version 2 (aug 2015): AndorSettingsV2 (I added "BaselineClamp")
  if(strncmp(buffer, "AndorSettingsV", 14)) {
   wxMessageBox(_T("Invalid format!"), _T("LoadAndorSettingsFromFile/Error"),
                wxOK | wxICON_ERROR);
   fclose(fp_in);
   return(-2);
   }
fclose(fp_in);

ReadStringFromConfigFile(filename, "HeadModel", ASet1->HeadModel);
ReadIntFromConfigFile(filename, "gblXPixels", &ASet1->gblXPixels);
ReadIntFromConfigFile(filename, "gblYPixels", &ASet1->gblYPixels);
ReadIntFromConfigFile(filename, "AD_Channel", &ASet1->AD_Channel);
ReadIntFromConfigFile(filename, "VShiftSpeed", &ASet1->VShiftSpeed);
ReadIntFromConfigFile(filename, "VShiftVoltage", &ASet1->VShiftVoltage);
ReadIntFromConfigFile(filename, "HShiftPreAmpGain", &ASet1->HShiftPreAmpGain);
ReadIntFromConfigFile(filename, "HShiftSpeed", &ASet1->HShiftSpeed);
ReadIntFromConfigFile(filename, "AcquisitionMode", &ASet1->AcquisitionMode);
ReadIntFromConfigFile(filename, "TriggerMode", &ASet1->TriggerMode);
ReadIntFromConfigFile(filename, "ShutterMode", &ASet1->ShutterMode);
ReadIntFromConfigFile(filename, "MirrorMode", &ASet1->MirrorMode);
ReadIntFromConfigFile(filename, "RotationMode", &ASet1->RotationMode);
ReadIntFromConfigFile(filename, "EMGainMode", &ASet1->EMGainMode);
ReadIntFromConfigFile(filename, "EMGainValue", &ASet1->EMGainValue);
ReadIntFromConfigFile(filename, "FrameTransferMode", &ASet1->FrameTransferMode);
ReadIntFromConfigFile(filename, "BaselineClamp", &ASet1->BaselineClamp);
ReadIntFromConfigFile(filename, "CloseShutterWhenExit", &ASet1->CloseShutterWhenExit);
// JLP2015
// ReadIntFromConfigFile(filename, "NExposures", &ASet1->NExposures);
ReadIntFromConfigFile(filename, "xc0", &ASet1->xc0);
ReadIntFromConfigFile(filename, "yc0", &ASet1->yc0);
ReadIntFromConfigFile(filename, "nx1", &ASet1->nx1);
ReadIntFromConfigFile(filename, "ny1", &ASet1->ny1);
ReadIntFromConfigFile(filename, "xbin", &ASet1->xbin);
ReadIntFromConfigFile(filename, "ybin", &ASet1->ybin);
ReadIntFromConfigFile(filename, "CoolerOn", &ASet1->CoolerOn);
ReadDoubleFromConfigFile(filename, "CoolerSetTemp", &ASet1->CoolerSetTemp);
ReadIntFromConfigFile(filename, "IntegTime_sec", &ASet1->IntegTime_sec);
ReadIntFromConfigFile(filename, "KineticExpoTime_msec", &ASet1->KineticExpoTime_msec);
ReadIntFromConfigFile(filename, "KineticClockTime_msec", &ASet1->KineticClockTime_msec);
ReadIntFromConfigFile(filename, "KineticCycleTime_msec", &ASet1->KineticCycleTime_msec);
ReadIntFromConfigFile(filename, "KineticCycleNExposures", &ASet1->KineticCycleNExposures);
ReadIntFromConfigFile(filename, "KineticNCycles", &ASet1->KineticNCycles);

// Display new values:
//  DisplayNewSettings();

return(0);
}
/***********************************************************************
* Write information specific to Andor camera to FITS descriptors
*
***********************************************************************/
int Write_AndorSettingsToFITSDescriptors(DESCRIPTORS *descrip,
                                         ANDOR_SETTINGS AndorSet1)
{
int k, rot_angle, conventional_mode;
float hspeed, vspeed;

 // Information about the ANDOR camera and its settings:

// Head model: (e.g. DV885)
// sprintf(buf,"ANDOR0  = \'Head model: %20.40s\' //", AndorSet1.HeadModel);
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "ANDOR0  ");
 sprintf((descrip->descr[k]).cvalue, "Head model: %20.40s", AndorSet1.HeadModel);
 (descrip->ndescr)++;

// Geometrical parameters:
 rot_angle = AndorSet1.RotationMode * 90;
// sprintf(buf,"ANDOR1  = \'xbin=%d ybin=%d xc=%d, yc=%d, rot=%d deg, mirror=%d\' //",
//         Aset.xbin, Aset.ybin, Aset.xc0, Aset.yc0, rot_angle,
//         Aset.MirrorMode);
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "ANDOR1  ");
 sprintf((descrip->descr[k]).cvalue, "xbin=%d ybin=%d xc=%d, yc=%d, rot=%d deg, mirror=%d",
         AndorSet1.xbin, AndorSet1.ybin, AndorSet1.xc0, AndorSet1.yc0, rot_angle, AndorSet1.MirrorMode);
 (descrip->ndescr)++;

// Vertical and horizontal shift parameters:
 conventional_mode = (AndorSet1.EMGainMode == 2) ? 1 : 0;

// Prompt to Andor camera the vertical and horizontal speeds:
 GetHSSpeed(AndorSet1.AD_Channel, conventional_mode, AndorSet1.HShiftSpeed, &hspeed);
 GetVSSpeed(AndorSet1.VShiftSpeed, &vspeed);
// Sometimes those routines do not return the good values
// So I always copy the VShiftSpeed and HShiftSpeed indices...
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "ANDOR2  ");
 sprintf((descrip->descr[k]).cvalue, "VS=%d (%.2f) HS=%d (%.2f) VAmp=%d Preamp=%d AD=%d",
         AndorSet1.VShiftSpeed, vspeed, AndorSet1.HShiftSpeed, hspeed,
         AndorSet1.VShiftVoltage, AndorSet1.HShiftPreAmpGain, AndorSet1.AD_Channel);
 (descrip->ndescr)++;

// Other parameters:
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "ANDOR3  ");
 if(AndorSet1.CoolerOn){
 sprintf((descrip->descr[k]).cvalue,"Exp=%d ms (EM: mode=%d G=%d, Cooler on: T=%.2f degC)",
         AndorSet1.KineticExpoTime_msec, AndorSet1.EMGainMode, AndorSet1.EMGainValue,
         AndorSet1.CoolerSetTemp);
 } else {
 sprintf((descrip->descr[k]).cvalue,"Exp=%d ms (EM: mode=%d G=%d, Cooler off)",
         AndorSet1.KineticExpoTime_msec, AndorSet1.EMGainMode, AndorSet1.EMGainValue);
 }
 (descrip->ndescr)++;

// Other parameters:
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "ANDOR4  ");
 sprintf((descrip->descr[k]).cvalue, "FrameTransfer=%d BaselineClamp=%d CloseShutterWhenExit=%d AcqMode=%d",
         AndorSet1.FrameTransferMode, AndorSet1.BaselineClamp, AndorSet1.CloseShutterWhenExit,
         AndorSet1.AcquisitionMode);
 (descrip->ndescr)++;

return(0);
}

