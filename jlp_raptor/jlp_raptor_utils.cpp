/************************************************************************
* "jlp_raptor_utils.h"
*
* JLP
* Version 17/08/2015
*************************************************************************/
#include "jlp_raptor_utils.h"
#include "asp2_rw_config_files.h" // ReadString....

/****************************************************************************
* First initialization of Raptor Settings (needed for before creating JLP_RaptorCam1)
****************************************************************************/
void RaptorSettingsToDefault(RAPTOR_SETTINGS* out_RSet, int nx_full, int ny_full)
{

// RaptorCapabilities RaptorCaps;
// RaptorCapabilities structure
// out_RSet->RaptorCaps;
// Head Model
strcpy(out_RSet->HeadModel, "");
// Detector dimensions
out_RSet->gblXPixels = nx_full;
out_RSet->gblYPixels = ny_full;
// 5 for continuous streaming
out_RSet->AcquisitionMode = 5;
// External trigger: 0=Off
out_RSet->ExtTrigger = 0;
// Trigger delay
out_RSet->TriggerDelay = 0;
// Fan
out_RSet->FanIsOn = 1;
out_RSet->AutoExposure = 0;
out_RSet->ImageSharpen = 0;
out_RSet->CloseShutterWhenExit = 0;
out_RSet->DigitalGain = 25;
out_RSet->HighGain = 0;
// 0:25MHz
out_RSet->FrameRate = 0;
// 3=Offset+Gain+Dark
out_RSet->NUC_State = 3;
// Shutter: -1=unavailable 0=auto 1=open 2=closed
out_RSet->ShutterMode = -1;
// Rotation mode: 0 for 0, 1 for +90, 2 for +180, 3 for +270
out_RSet->RotationMode = 0;
// MirrorMode: 0=none, 1=vertical, 2=horizontal 3=vertical+horizontal
out_RSet->MirrorMode = 0;
// CloseShutterWhenExit (0/1)
out_RSet->CloseShutterWhenExit = 1;
// Center of ROI subwindow
out_RSet->xc0 = out_RSet->gblXPixels/ 2;
out_RSet->yc0 = out_RSet->gblYPixels/ 2;
// Binning factors
out_RSet->xbin = 1;
out_RSet->ybin = 1;
// Size of elementary frames
out_RSet->nx1 = 128;
out_RSet->ny1 = 128;
// Exposure time in msec
out_RSet->ExpoTime_msec = 10;
// Long exposure time in seconds
out_RSet->IntegTime_sec = 500;
// Flag set to true if Cooler is wanted
out_RSet->CoolerOn = 1;
// Cooler setting temperature when Cooler is on
out_RSet->CoolerSetTemp = -15.;

return;
}
/***************************************************************************
* Copy RAPTOR_SETTINGS structure
*
* INPUT:
*  in_RSet
*
* OUTPUT:
*  out_RSet
****************************************************************************/
int Copy_RaptorSettings(RAPTOR_SETTINGS in_RSet, RAPTOR_SETTINGS* out_RSet)
{
strcpy(out_RSet->HeadModel, in_RSet.HeadModel);
out_RSet->gblXPixels = in_RSet.gblXPixels;
out_RSet->gblYPixels = in_RSet.gblYPixels;
out_RSet->is_initialized = in_RSet.is_initialized;
out_RSet->AcquisitionMode = in_RSet.AcquisitionMode;
out_RSet->AutoExposure = in_RSet.AutoExposure;
out_RSet->ExtTrigger = in_RSet.ExtTrigger;
out_RSet->TriggerDelay = in_RSet.TriggerDelay;
out_RSet->FanIsOn = in_RSet.FanIsOn;
out_RSet->HighGain = in_RSet.HighGain;
out_RSet->DigitalGain = in_RSet.DigitalGain;
out_RSet->FrameRate = in_RSet.FrameRate;
out_RSet->ImageSharpen = in_RSet.ImageSharpen;
out_RSet->NUC_State = in_RSet.NUC_State;
out_RSet->ShutterMode = in_RSet.ShutterMode;
out_RSet->MirrorMode = in_RSet.MirrorMode;
out_RSet->RotationMode = in_RSet.RotationMode;
out_RSet->CloseShutterWhenExit = in_RSet.CloseShutterWhenExit;
// JLP2015
// out_RSet->NExposures = in_RSet.NExposures;
out_RSet->xc0 = in_RSet.xc0;
out_RSet->yc0 = in_RSet.yc0;
out_RSet->nx1 = in_RSet.nx1;
out_RSet->ny1 = in_RSet.ny1;
out_RSet->xbin = in_RSet.xbin;
out_RSet->ybin = in_RSet.ybin;
out_RSet->CoolerOn = in_RSet.CoolerOn;
out_RSet->CoolerSetTemp = in_RSet.CoolerSetTemp;
out_RSet->IntegTime_sec = in_RSet.IntegTime_sec;
out_RSet->ExpoTime_msec = in_RSet.ExpoTime_msec;

return(0);
}
/***************************************************************************
* Copy useful RAPTOR_SETTINGS parameters to JLP_CAMERA_SETTINGS
*
* INPUT:
*  in_RSet
*
* OUTPUT:
*  out_CSet
****************************************************************************/
int Copy_RaptorToCameraSettings(RAPTOR_SETTINGS in_RSet,
                                JLP_CAMERA_SETTINGS* out_CSet)
{
strcpy(out_CSet->HeadModel, in_RSet.HeadModel);
out_CSet->gblXPixels = in_RSet.gblXPixels;
out_CSet->gblYPixels = in_RSet.gblYPixels;
out_CSet->is_initialized = in_RSet.is_initialized;
out_CSet->AcquisitionMode = in_RSet.AcquisitionMode;
out_CSet->ShutterMode = in_RSet.ShutterMode;
out_CSet->MirrorMode = in_RSet.MirrorMode;
out_CSet->RotationMode = in_RSet.RotationMode;
out_CSet->CloseShutterWhenExit = in_RSet.CloseShutterWhenExit;
out_CSet->xc0 = in_RSet.xc0;
out_CSet->yc0 = in_RSet.yc0;
out_CSet->nx1 = in_RSet.nx1;
out_CSet->ny1 = in_RSet.ny1;
out_CSet->xbin = in_RSet.xbin;
out_CSet->ybin = in_RSet.ybin;
out_CSet->CoolerOn = in_RSet.CoolerOn;
out_CSet->CoolerSetTemp = in_RSet.CoolerSetTemp;
out_CSet->IntegTime_sec = in_RSet.IntegTime_sec;
out_CSet->ExpoTime_msec = in_RSet.ExpoTime_msec;

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
int Copy_CameraToRaptorSettings(JLP_CAMERA_SETTINGS in_CamSet,
                                RAPTOR_SETTINGS* out_RSet)
{
strcpy(out_RSet->HeadModel, in_CamSet.HeadModel);
out_RSet->gblXPixels = in_CamSet.gblXPixels;
out_RSet->gblYPixels = in_CamSet.gblYPixels;
out_RSet->is_initialized = in_CamSet.is_initialized;
out_RSet->AcquisitionMode = in_CamSet.AcquisitionMode;
out_RSet->ShutterMode = in_CamSet.ShutterMode;
out_RSet->MirrorMode = in_CamSet.MirrorMode;
out_RSet->RotationMode = in_CamSet.RotationMode;
out_RSet->CloseShutterWhenExit = in_CamSet.CloseShutterWhenExit;
out_RSet->xc0 = in_CamSet.xc0;
out_RSet->yc0 = in_CamSet.yc0;
out_RSet->nx1 = in_CamSet.nx1;
out_RSet->ny1 = in_CamSet.ny1;
out_RSet->xbin = in_CamSet.xbin;
out_RSet->ybin = in_CamSet.ybin;
out_RSet->CoolerOn = in_CamSet.CoolerOn;
out_RSet->CoolerSetTemp = in_CamSet.CoolerSetTemp;
out_RSet->IntegTime_sec = in_CamSet.IntegTime_sec;
out_RSet->ExpoTime_msec = in_CamSet.ExpoTime_msec;

return(0);
}
//---------------------------------------------------------------------------
// Save Raptor Settings to File
//---------------------------------------------------------------------------
int SaveRaptorSettingsToFile(char *filename, RAPTOR_SETTINGS RSet1)
{
FILE *fp_out;
wxString buffer;

if((fp_out = fopen(filename,"w"))== NULL) {
  buffer.Printf(_T("Error opening output file >%s< \n"), filename);
  wxMessageBox(buffer, _T("SaveRaptorSettingsToFile/Error"),
               wxOK | wxICON_ERROR);
  return(-1);
  }

// Version 1 (oct 2017)
fprintf(fp_out, "RaptorSettingsV3 Configuration file\n");
fprintf(fp_out, "HeadModel = %s\n", RSet1.HeadModel);
fprintf(fp_out, "gblXPixels = %d\n", RSet1.gblXPixels);
fprintf(fp_out, "gblYPixels = %d\n", RSet1.gblYPixels);
fprintf(fp_out, "AcquisitionMode = %d\n", RSet1.AcquisitionMode);
fprintf(fp_out, "ExtTrigger = %d\n", RSet1.ExtTrigger);
fprintf(fp_out, "ShutterMode = %d\n", RSet1.ShutterMode);
fprintf(fp_out, "MirrorMode = %d\n", RSet1.MirrorMode);
fprintf(fp_out, "RotationMode = %d\n", RSet1.RotationMode);
fprintf(fp_out, "CloseShutterWhenExit = %d\n", RSet1.CloseShutterWhenExit);
// JLP2015
// fprintf(fp_out, "NExposures = %d\n", RSet1.NExposures);
fprintf(fp_out, "xc0 = %d\n", RSet1.xc0);
fprintf(fp_out, "yc0 = %d\n", RSet1.yc0);
fprintf(fp_out, "nx1 = %d\n", RSet1.nx1);
fprintf(fp_out, "ny1 = %d\n", RSet1.ny1);
fprintf(fp_out, "xbin = %d\n", RSet1.xbin);
fprintf(fp_out, "ybin = %d\n", RSet1.ybin);
fprintf(fp_out, "CoolerOn = %d\n", RSet1.CoolerOn);
fprintf(fp_out, "CoolerSetTemp = %.2f\n", RSet1.CoolerSetTemp);
fprintf(fp_out, "IntegTime_sec = %d\n", RSet1.IntegTime_sec);
fprintf(fp_out, "ExpoTime_msec = %d\n", RSet1.ExpoTime_msec);

fclose(fp_out);

return(0);
}
//---------------------------------------------------------------------------
// Load Raptor Settings from File
//---------------------------------------------------------------------------
int LoadRaptorSettingsFromFile(char *filename, RAPTOR_SETTINGS *RSet1)
{
FILE *fp_in;
wxString err_message;
char buffer[128];

if((fp_in = fopen(filename,"r"))== NULL) {
  err_message.Printf(_T("Error opening input configuration file >%s< \n"),
                     filename);
  wxMessageBox(err_message, "LoadRaptorSettingsFromFile/Error",
               wxOK | wxICON_ERROR);
  return(-1);
  }
// Read the first line to see if it is a good file:
  fgets(buffer, 128, fp_in);
// Version 1 (feb 2011): RaptorSettingsV1
// Version 2 (aug 2015): RaptorSettingsV2 (I added "BaselineClamp")
  if(strncmp(buffer, "RaptorSettingsV", 14)) {
   wxMessageBox(_T("Invalid format!"), _T("LoadRaptorSettingsFromFile/Error"),
                wxOK | wxICON_ERROR);
   fclose(fp_in);
   return(-2);
   }
fclose(fp_in);

ReadStringFromConfigFile(filename, "HeadModel", RSet1->HeadModel);
ReadIntFromConfigFile(filename, "gblXPixels", &RSet1->gblXPixels);
ReadIntFromConfigFile(filename, "gblYPixels", &RSet1->gblYPixels);
ReadIntFromConfigFile(filename, "AcquisitionMode", &RSet1->AcquisitionMode);
ReadIntFromConfigFile(filename, "ExtTrigger", &RSet1->ExtTrigger);
ReadIntFromConfigFile(filename, "ShutterMode", &RSet1->ShutterMode);
ReadIntFromConfigFile(filename, "MirrorMode", &RSet1->MirrorMode);
ReadIntFromConfigFile(filename, "RotationMode", &RSet1->RotationMode);
ReadIntFromConfigFile(filename, "CloseShutterWhenExit", &RSet1->CloseShutterWhenExit);
ReadIntFromConfigFile(filename, "xc0", &RSet1->xc0);
ReadIntFromConfigFile(filename, "yc0", &RSet1->yc0);
ReadIntFromConfigFile(filename, "nx1", &RSet1->nx1);
ReadIntFromConfigFile(filename, "ny1", &RSet1->ny1);
ReadIntFromConfigFile(filename, "xbin", &RSet1->xbin);
ReadIntFromConfigFile(filename, "ybin", &RSet1->ybin);
ReadIntFromConfigFile(filename, "CoolerOn", &RSet1->CoolerOn);
ReadDoubleFromConfigFile(filename, "CoolerSetTemp", &RSet1->CoolerSetTemp);
ReadIntFromConfigFile(filename, "IntegTime_sec", &RSet1->IntegTime_sec);
ReadIntFromConfigFile(filename, "ExpoTime_msec", &RSet1->ExpoTime_msec);

/*
buf0.Printf(wxT("exp_time=%d"), RSet1->ExpoTime_msec);
wxMessageBox(buf0, wxT("LoadNewvalues"), wxOK);
*/
return(0);
}
/***********************************************************************
* Write information specific to Raptor camera to FITS descriptors
*
***********************************************************************/
int Write_RaptorSettingsToFITSDescriptors(DESCRIPTORS *descrip,
                                         RAPTOR_SETTINGS RaptorSet1)
{
int k, rot_angle;

 // Information about the RAPTOR camera and its settings:

// Head model: (e.g. DV885)
// sprintf(buf,"RAPTOR0  = \'Head model: %20.40s\' //", RaptorSet1.HeadModel);
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "RAPTOR0 ");
 sprintf((descrip->descr[k]).cvalue, "Head model: %20.40s", RaptorSet1.HeadModel);
 (descrip->ndescr)++;

// Geometrical parameters:
 rot_angle = RaptorSet1.RotationMode * 90;
// sprintf(buf,"RAPTOR1  = \'xbin=%d ybin=%d xc=%d, yc=%d, rot=%d deg, mirror=%d\' //",
//         Aset.xbin, Aset.ybin, Aset.xc0, Aset.yc0, rot_angle,
//         Aset.MirrorMode);
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "RAPTOR1 ");
 sprintf((descrip->descr[k]).cvalue, "xbin=%d ybin=%d xc=%d, yc=%d, rot=%d deg, mirror=%d",
         RaptorSet1.xbin, RaptorSet1.ybin, RaptorSet1.xc0, RaptorSet1.yc0, rot_angle, RaptorSet1.MirrorMode);
 (descrip->ndescr)++;

 /*
// Other parameters:
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "RAPTOR3 ");
 if(RaptorSet1.CoolerOn){
 sprintf((descrip->descr[k]).cvalue,"Exp=%d ms (EM: mode=%d G=%d, Cooler on: T=%d degC)",
         RaptorSet1.ExposureTime, RaptorSet1.EMGainMode, RaptorSet1.EMGainValue,
         RaptorSet1.CoolerTemp);
 } else {
 sprintf((descrip->descr[k]).cvalue,"Exp=%d ms (EM: mode=%d G=%d, Cooler off)",
         RaptorSet1.KineticExpoTime_msec, RaptorSet1.EMGainMode, RaptorSet1.EMGainValue);
 }
 (descrip->ndescr)++;

// Other parameters:
 k = descrip->ndescr;
 strcpy((descrip->descr[k]).keyword, "RAPTOR4 ");
 sprintf((descrip->descr[k]).cvalue, "FrameTransfer=%d BaselineClamp=%d CloseShutterWhenExit=%d AcqMode=%d",
         RaptorSet1.FrameTransferMode, RaptorSet1.BaselineClamp, RaptorSet1.CloseShutterWhenExit,
         RaptorSet1.acquisitionMode);
 (descrip->ndescr)++;
*/
return(0);
}
/*******************************************************************************
*
* INPUT:
* out_length_in_bytes : output length in bytes
********************************************************************************/
int JLP_DecimalToHexadecimal(long int in_decimal,
                             char *out_Hexadecimal, int out_length_in_bytes)
{
long int remainder, quotient;
int i, j, k, max_length = 16;
int HexaNumber[max_length];
wxString buffer;

for(i = 0; i < max_length; i++) HexaNumber[i] = 48;

 quotient = in_decimal;

i = 0;
 while(quotient!=0) {
         remainder = quotient % 16;
// For values of remainder less than 10, the appropriate ASCII code is 48 + remainder:
/*
0 => 48 + 0 => '0'
1 => 48 + 1 => '1'
2 => 48 + 2 => '2'
3 => 48 + 3 => '3'
4 => 48 + 4 => '4'
5 => 48 + 5 => '5'
6 => 48 + 6 => '6'
7 => 48 + 7 => '7'
8 => 48 + 8 => '8'
9 => 48 + 9 => '9'
*/

// For values of remainder larger than 10, the appropriate ASCII code is 55 + remainder:
/*
10 => 55 + 10 => 'A'
11 => 55 + 11 => 'B'
12 => 55 + 12 => 'C'
13 => 55 + 13 => 'D'
14 => 55 + 14 => 'E'
15 => 55 + 15 => 'F'
*/
//To convert integer into character
  if( remainder < 10)
     remainder = remainder + 48;
    else
     remainder = remainder + 55;

    HexaNumber[i++]= remainder;
    if(i == out_length_in_bytes) break;

    quotient = quotient / 16;
  }

// Initialize output strings with zeros:
 for(k = 0; k < out_length_in_bytes - 1; k++)  {
    out_Hexadecimal[k] = '0';
  }
 k = out_length_in_bytes - i;
 for(j = i - 1; j >= 0; j--)  {
    out_Hexadecimal[k++] = HexaNumber[j];
    if(k == out_length_in_bytes) break;
  }
  out_Hexadecimal[out_length_in_bytes] = '\0';
/*
 buffer.Printf(wxT("Hexadecimal value of decimal number %ld: %s out_length_in_bytes=%d"),
               in_decimal, out_Hexadecimal, out_length_in_bytes);
 wxMessageBox(buffer, wxT("Hexadecimal"), wxOK);
*/
 return 0;
}
