/************************************************************************
* "jlp_raptor_cam1_set.cpp"
* JLP_RaptorCam1 class
*
* JLP
* Version 17/09/2015
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_raptor_cam1.h"      // RAPTOR_SETTINGS, ...
#include "jlp_raptor_utils.h"     // Copy_RaptorSettings, ...

#define DEBUG 0

/****************************************************************************
*
****************************************************************************/
int JLP_RaptorCam1::Cam1_SaveCameraSettingsToFile(char *filename)
{
 int status;
 status = SaveRaptorSettingsToFile(filename, RaptorSet1);
 return(status);
}
/****************************************************************************
*
****************************************************************************/
int JLP_RaptorCam1::Cam1_LoadCameraSettingsFromFile(char *filename)
{
 int status;
 status = LoadRaptorSettingsFromFile(filename, &RaptorSet1);
 return(status);
}
/****************************************************************************
*
****************************************************************************/
int JLP_RaptorCam1::Cam1_Write_CameraSettingsToFITSDescriptors(DESCRIPTORS *descrip)
{
 int status;
 status = Write_RaptorSettingsToFITSDescriptors(descrip, RaptorSet1);
 return(status);
}
/****************************************************************************
*
* Shutter config. is made up of TTL level, shutter mode and open close times
* Shutter mode: 0=automatic 1=open 2=close -1=NoShutterAvailable
*
*  ShutterMode = RaptorSet1.ShutterMode;
*
****************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SetShutter(int ShutterMode, bool& ShutterAvailable)
{
int status = - 1;
// For Raptor cameras, shutter is not available
ShutterAvailable = false;
if(ShutterMode == -1) status = 0;
return(status);
}
/***************************************************************************
* Set default values for JLP_Raptor_Settings
*
* WARNING: SHOULD NOT CHANGE gblXPixels, ... !
*
* Size of subwindow, center of subwindow, bin factor, exposure time
* Acquisition mode, EMGainValue, number of exposures
*
****************************************************************************/
int JLP_RaptorCam1::Cam1_SettingsToDefault(int nx_full, int ny_full)
{
// Full format:
RaptorSet1.gblXPixels = nx_full;
RaptorSet1.gblYPixels = ny_full;

// 5 for continuous streaming
RaptorSet1.AcquisitionMode = 5;
// External trigger: 0=Off
RaptorSet1.ExtTrigger = 0;
// Trigger delay
RaptorSet1.TriggerDelay = 0;
RaptorSet1.ImageSharpen = 0;
RaptorSet1.CloseShutterWhenExit = 0;
RaptorSet1.DigitalGain = 24;
// 0:25MHz
RaptorSet1.FrameRate = 0;
// 3=Offset+Gain+Dark
RaptorSet1.NUC_State = 3;
// Shutter: -1=unavailable 0=auto 1=open 2=closed
RaptorSet1.ShutterMode = -1;
// Rotation mode: 0 for 0, 1 for +90, 2 for +180, 3 for +270
RaptorSet1.RotationMode = 0;
// MirrorMode: 0=none, 1=vertical, 2=horizontal 3=vertical+horizontal
RaptorSet1.MirrorMode = 0;
// CloseShutterWhenExit (0/1)
RaptorSet1.CloseShutterWhenExit = 0;
// Center of ROI subwindow
RaptorSet1.xc0 = RaptorSet1.gblXPixels/ 2;
RaptorSet1.yc0 = RaptorSet1.gblYPixels/ 2;
// Binning factors
RaptorSet1.xbin = 1;
RaptorSet1.ybin = 1;
// Size of elementary frames
RaptorSet1.nx1 = 128;
RaptorSet1.ny1 = 128;
// Exposure time in msec
RaptorSet1.ExpoTime_msec = 10;
RaptorSet1.AutoExposure = 0;
// Long exposure time in seconds
RaptorSet1.IntegTime_sec = 500;
// Flag set to true if Cooler is wanted
RaptorSet1.CoolerOn = 1;
// Cooler setting temperature when Cooler is on
RaptorSet1.CoolerSetTemp = 10.;
// Fan
RaptorSet1.FanIsOn = 1;

return(0);
}
/************************************************************************
* Start Cooler
************************************************************************/
int  JLP_RaptorCam1::JLP_Raptor_StartCooler()
{
int status;
wxString err_msg;
// Flag set to true if Cooler is wanted
 RaptorSet1.CoolerOn = 1;
// Set cooler on by selecting the proper FPGA options:
 JLP_Raptor_Set_FPGA_Options();
 status = Cam1_SetCoolerTemp(err_msg);
 if(status != 0) wxMessageBox(err_msg, wxT("JLP_Raptor_StartCooler"), wxICON_ERROR);
return(status);
}
/************************************************************************
* Stop Cooler
************************************************************************/
int  JLP_RaptorCam1::JLP_Raptor_StopCooler()
{
// Flag set to true if Cooler is wanted
RaptorSet1.CoolerOn = 0;
// Set cooler off by selecting the proper FPGA options:
 JLP_Raptor_Set_FPGA_Options();
return(0);
}
/*********************************************************************
*
0x53 0xE0 0x02 0xEE 0xY1 0x50
0x53 0xE0 0x02 0xEF 0xY2 0x50
0x53 0xE0 0x02 0xF0 0xY3 0x50
0x53 0xE0 0x02 0xF1 0xY4 0x50
*
30 bit value, 4 separate commands,
1 count = 1*40MHz period = 25nsecs
Y1 = xxMMMMMM of 4 byte word
::
Y4 = LLLLLLLL of 4 byte word
Exposure updated on LSB write
Min Exposure = 500nsec = 20counts
Max Exposure = (2^30)*25ns ≈ 26.8secs
**********************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SetExpoTimeOrAutoExpo()
{
double clock_freq_MHz, clock_period_sec, expo_msec;
int status, out_length = 8, int_value;
long clock_counts;
char out_Hexadecimal[out_length + 1], char_value[3];
unsigned char data_send0[64];
wxString buf0;

// Set AutoExpo on/off:
JLP_Raptor_Set_FPGA_Options();

// If AutoExposure return from here:
if(RaptorSet1.AutoExposure == 1) return(0);


clock_freq_MHz = 40.;

clock_period_sec = 1. / (clock_freq_MHz * 1.E06);

// Check if exposure time is between 1 and 25000 msec
if( RaptorSet1.ExpoTime_msec < 1.) RaptorSet1.ExpoTime_msec = 1;
if( RaptorSet1.ExpoTime_msec > 25000) RaptorSet1.ExpoTime_msec = 25000;
expo_msec = RaptorSet1.ExpoTime_msec;

clock_counts = (long)(expo_msec / (1000. * clock_period_sec));

JLP_DecimalToHexadecimal(clock_counts, out_Hexadecimal, out_length);

// DEBUG:
#if DEBUG
buf0.Printf(wxT("Expo time msec = %f clock_counts=%d hexa=%s"),
            expo_msec, clock_counts, out_Hexadecimal);
wxMessageBox(buf0, wxT("SetExpotimeOrAutoExpo"), wxOK);
#endif // DEBUG

// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE0\\x02\\xEE\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xEE;
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE0\\x02\\xEF\\x%c%c\\x50",
//        out_Hexadecimal[2], out_Hexadecimal[3]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xEF;
sprintf(char_value, "%c%c", out_Hexadecimal[2], out_Hexadecimal[3]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE0\\x02\\xEF\\x%c%c\\x50",
//        out_Hexadecimal[4], out_Hexadecimal[5]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xF0;
sprintf(char_value, "%c%c", out_Hexadecimal[4], out_Hexadecimal[5]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE0\\x02\\xF1\\x%c%c\\x50",
//        out_Hexadecimal[6], out_Hexadecimal[7]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xF1;
sprintf(char_value, "%c%c", out_Hexadecimal[6], out_Hexadecimal[7]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

return(0);
}
/*********************************************************************
*
0x53 0xE0 0x02 0xDD 0xY1 0x50
0x53 0xE0 0x02 0xDE 0xY2 0x50
0x53 0xE0 0x02 0xDF 0xY3 0x50
0x53 0xE0 0x02 0xE0 0xY4 0x50
*
32 bit value, 4 separate commands,
1 count = 1*40MHz period = 25nsecs
Y1 = MSB of 4 byte word
Y4 = LSB of 4 byte word;
Frame rate updated on LSB write
**********************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SetFrameRate()
{
double clock_freq_MHz, frame_rate;
int out_length = 8, int_value, status;
long clock_counts;
char out_Hexadecimal[out_length + 1],  char_value[3];
unsigned char data_send0[64];
wxString buf0;

// If external trigger, exit from here:
if(RaptorSet1.ExtTrigger != 0) return(0);

clock_freq_MHz = 40.;

/* Frame rate :
* 0: 25 Hz
* 1: 30 Hz
* 2: 50 Hz
* 3: 60 Hz
* 4: 90 Hz
* 5: 120 Hz
*/
 switch(RaptorSet1.FrameRate) {
   default:
   case 0:
     frame_rate = 25.;
     break;
   case 1:
     frame_rate = 30.;
     break;
   case 2:
     frame_rate = 50.;
     break;
   case 3:
     frame_rate = 60.;
     break;
   case 4:
     frame_rate = 90.;
     break;
   case 5:
     frame_rate = 120.;
     break;
 }

clock_counts = (long)(clock_freq_MHz * 1.E06 / frame_rate);

JLP_DecimalToHexadecimal(clock_counts, out_Hexadecimal, out_length);

// DEBUG:
#if DEBUG
buf0.Printf(wxT("Frame rate = %f clock_counts=%d hexa=%s"),
            frame_rate, clock_counts, out_Hexadecimal);
wxMessageBox(buf0, wxT("SetFrameRate"), wxOK);
#endif // DEBUG

// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE0\\x02\\xDD\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xDD;
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE0\\x02\\xDE\\x%c%c\\x50",
//        out_Hexadecimal[2], out_Hexadecimal[3]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xDE;
sprintf(char_value, "%c%c", out_Hexadecimal[2], out_Hexadecimal[3]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE0\\x02\\xDF\\x%c%c\\x50",
//        out_Hexadecimal[4], out_Hexadecimal[5]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xDF;
sprintf(char_value, "%c%c", out_Hexadecimal[4], out_Hexadecimal[5]);
// convertion string to long, basis 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE0\\x02\\xE0\\x%c%c\\x50",
//        out_Hexadecimal[6], out_Hexadecimal[7]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xE0;
sprintf(char_value, "%c%c", out_Hexadecimal[6], out_Hexadecimal[7]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

return(0);
}
/************************************************************************
* Modify cooler temperature
*
0x53 0xE0 0x02 0xFB 0xMM 0x50
0x53 0xE0 0x02 0xFA 0xLL 0x50
*
12 bit DAC value, LSB = LL byte, Lower nibble of MM = MSBs
Reg 0xFB, bits 3..0 = set point bits 11..8
Reg 0xFA, bits 7..0 = set point bits 7..0
12 bit value to be converted to temperature from DAC calibration values
************************************************************************/
int JLP_RaptorCam1::Cam1_SetCoolerTemp(wxString& error_msg)
{
// For Owl640: TECSetPoint = 15.0;
// For Ninox640: TECSetPoint = -15.0;
double M_ADC, C_ADC;
int out_length = 4, int_value, status;
long temp_counts_decimal;
char out_Hexadecimal[out_length + 1], char_value[3];
unsigned char data_send0[64];
wxString buff;

// If Cooler not set, exit from here:
if(RaptorSet1.CoolerOn != 1) return(0);

// Y_temp = M_ADC * X_counts + C_ADC
// X_counts = (Y_temp - C_ADC) / M_ADC

// Y_40 = M_ADC * X_40 + C_ADC
// Y_0 = M_ADC * X_0 + C_ADC
// Y_40 - Y_0 = M_ADC * (X_40 - X_0)
// M_ADC = (Y_40 - Y_0) / (X_40 - X_0)
M_ADC = (40.0 - 0.0) / (EPROM_ADC_Cal_40C - EPROM_ADC_Cal_0C);
// C_ADC = Y_40 - M_ADC * EPROM_ADC_Cal_40C;
C_ADC = 40. - M_ADC * EPROM_ADC_Cal_40C;

temp_counts_decimal = (long)((RaptorSet1.CoolerSetTemp - C_ADC) / M_ADC);

#if DEBUG
 buff.Printf(wxT("Temperature=%.2f M_ADC=%.4f C_ADC=%.4f counts=%d"),
             RaptorSet1.CoolerSetTemp, M_ADC, C_ADC, (int)temp_counts_decimal);
 wxMessageBox(buff, wxT("JLP_Raptor_SetCoolerTemp"), wxOK);
#endif // DEBUG

JLP_DecimalToHexadecimal(temp_counts_decimal, out_Hexadecimal, out_length);

// Commands to serial port:
// 0x53 0xE0 0x02 0xFB 0xMM 0x50
// 0x53 0xE0 0x02 0xFA 0xLL 0x50
//sprintf(data_send0, "\\x53\\xE0\\x02\\xFB\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xFB;
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE0\\x02\\xFA\\x%c%c\\x50",
//        out_Hexadecimal[2], out_Hexadecimal[3]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xFA;
sprintf(char_value, "%c%c", out_Hexadecimal[2], out_Hexadecimal[3]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

return(0);
}
/****************************************************************************
* Set the digital gain to the output register
*
0x53 0xE0 0x02 0xC6 0xMM 0x50
0x53 0xE0 0x02 0xC7 0xLL 0x50
*
16bit value = gain*256
MM bits 7..0 = gain bits 15..8
LL bits 7..0 = level bits 7..0
Data updated on write to LSBs
*****************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SetHighOrDigitalGain(wxString& err_msg)
{
int out_length = 4, int_value, status;
long gain_decimal;
char out_Hexadecimal[out_length + 1], char_value[3];
unsigned char data_send0[64];
wxString buf0;

// Set HighGain on/off
JLP_Raptor_SetHighGainAndTriggerMode();

// If High gain, exit from here:
if(RaptorSet1.HighGain == 1) return(0);

// RaptorSet1.DigitalGain in the range 0 48
gain_decimal = (long)(RaptorSet1.DigitalGain * 256);

JLP_DecimalToHexadecimal(gain_decimal, out_Hexadecimal, out_length);
// DEBUG:
#if DEBUG
buf0.Printf(wxT("DigitalGain=%d gain_decimal=%d hexa=%s"),
            RaptorSet1.DigitalGain, gain_decimal, out_Hexadecimal);
wxMessageBox(buf0, wxT("JLP_Raptor_SetDigitalGain"), wxOK);
#endif // DEBUG

// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE0\\x02\\xC6\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xC6;
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, basis 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE0\\x02\\xC7\\x%c%c\\x50",
//        out_Hexadecimal[2], out_Hexadecimal[3]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xC7;
sprintf(char_value, "%c%c", out_Hexadecimal[2], out_Hexadecimal[3]);
// convertion string to long, basis 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

return(0);
}
/*********************************************************************
*
0x53 0xE2 0x02 0xE9 0xY1 0x50
0x53 0xE2 0x02 0xEA 0xY2 0x50
0x53 0xE2 0x02 0xEB 0xY3 0x50
0x53 0xE2 0x02 0xEC 0xY4 0x50
*
30 bit value, 4 separate commands,
1 count = 1*40MHz period = 25nsecs
Y1 = xxMMMMMM of 4 byte word
::
Y4 = LLLLLLLL of 4 byte word
Trig Delay updated on LSB write
**********************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SetTriggerDelay()
{
int out_length = 8, int_value, status;
long tdelay_decimal;
double clock_freq_MHz;
char out_Hexadecimal[out_length + 1], char_value[3];
unsigned char data_send0[64];
wxString buf0;

// If internal trigger, exit from here:
if(RaptorSet1.ExtTrigger == 0) return(0);

clock_freq_MHz = 40.;
if(RaptorSet1.TriggerDelay == 0)
  tdelay_decimal = 0.;
else
  tdelay_decimal = (long)(clock_freq_MHz * 1.E06 / RaptorSet1.TriggerDelay);

JLP_DecimalToHexadecimal(tdelay_decimal, out_Hexadecimal, out_length);
// DEBUG:
#if DEBUG
buf0.Printf(wxT("TriggerDelay=%d tdelay_decimal=%d hexa=%s"),
            RaptorSet1.TriggerDelay, tdelay_decimal, out_Hexadecimal);
wxMessageBox(buf0, wxT("JLP_Raptor_SetTriggerDelay"), wxOK);
#endif // DEBUG

// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE2\\x02\\xE9\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE2;
data_send0[2] = 0x02;
data_send0[3] = 0xE9;
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE2\\x02\\xEA\\x%c%c\\x50",
//        out_Hexadecimal[2], out_Hexadecimal[3]);
data_send0[0] = 0x53;
data_send0[1] = 0xE2;
data_send0[2] = 0x02;
data_send0[3] = 0xEA;
sprintf(char_value, "%c%c", out_Hexadecimal[2], out_Hexadecimal[3]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE2\\x02\\xEB\\x%c%c\\x50",
//        out_Hexadecimal[4], out_Hexadecimal[5]);
data_send0[0] = 0x53;
data_send0[1] = 0xE2;
data_send0[2] = 0x02;
data_send0[3] = 0xEB;
sprintf(char_value, "%c%c", out_Hexadecimal[4], out_Hexadecimal[5]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

//sprintf(data_send0, "\\x53\\xE2\\x02\\xEC\\x%c%c\\x50",
//        out_Hexadecimal[6], out_Hexadecimal[7]);
data_send0[0] = 0x53;
data_send0[1] = 0xE2;
data_send0[2] = 0x02;
data_send0[3] = 0xEC;
sprintf(char_value, "%c%c", out_Hexadecimal[6], out_Hexadecimal[7]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

return(0);
}
/**************************************************************************
* Set Gain/Trigger mode of Raptor camera
*
0x53 0xE0 0x02 0xF2 0xYY 0x50
*
YY Bit 7 = 0, Reserved (default = 0)
YY Bit 6 = 1 to enable Ext trig (default = 0)
YY Bit 5 = 0 for –ve edge trig (default = 0)
YY Bit 4 = 1 to enable High Gain Trigger mode 2 (default = 0)
YY Bit 3 = 0, Reserved (default = 0)
YY Bit 2 = 0 for low gain, 1 for high gain(default = 0)
YY Bit 1 = 0 for low gain, 1 for high gain(default = 0)
YY Bit 0 = 0, Reserved (default = 0)
Note that bits 2 and 1 should be set to the same value.
*
**************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SetHighGainAndTriggerMode()
{
int i, out_length = 2, bit0[8], int_value, i_2, status;
long options_decimal;
char out_Hexadecimal[out_length + 1], char_value[3];
unsigned char data_send0[64];
wxString buf0;

for(i = 0; i < 8; i++) bit0[i] = 0;

// Low Gain;
if(RaptorSet1.HighGain == 0 ) {
 bit0[1] = 1;
 bit0[2] = 1;
 }

// Internal/External trigger;
if(RaptorSet1.ExtTrigger > 0) bit0[6] = 1;

// High gain trigger mode 2 not enabled:
bit0[4] = 0;

options_decimal = bit0[0];
i_2 = 1;
for(i = 1; i < 8; i++) {
 i_2 *= 2;
 options_decimal += i_2 * bit0[i];
 }

JLP_DecimalToHexadecimal(options_decimal, out_Hexadecimal, out_length);
// DEBUG:
#if DEBUG
buf0.Printf(wxT("HighGain=%d ext_trigger=%d options_decimal=%d hexa=%s"),
            RaptorSet1.HighGain, RaptorSet1.ExtTrigger, options_decimal, out_Hexadecimal);
wxMessageBox(buf0, wxT("JLP_Raptor_SetHighGainTriggerMode"), wxOK);
#endif // DEBUG

// Commands to serial port:
// 0x53 0xE0 0x02 0xF2 0xYY 0x50
//sprintf(data_send0, "\\x53\\xE0\\x02\\xF2\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xF2;
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

return(0);
}
/*********************************************************************
* Set FPGA CTRL register
* used to set cooler, auto_exposure, fan on/off
**********************************************************************/
int JLP_RaptorCam1::JLP_Raptor_Set_FPGA_Options()
{
int out_length = 2, bit0, bit1, bit2, int_value, status;
long options_decimal;
char out_Hexadecimal[out_length + 1], char_value[3];
unsigned char data_send0[64];
wxString buf0;

if(RaptorSet1.CoolerOn) bit0 = 1;
 else bit0 = 0;
if(RaptorSet1.AutoExposure) bit1 = 1;
 else bit1 = 0;

// Fan is on:
if(RaptorSet1.FanIsOn != 0)
 bit2 = 1;
else
 bit2 = 0;

// Video inverted if bit6==1
// Horizontal flip if bit7==1

options_decimal = (long)(bit0 + bit1 * 2 + bit2 * 4);

JLP_DecimalToHexadecimal(options_decimal, out_Hexadecimal, out_length);
// DEBUG:
#if DEBUG
buf0.Printf(wxT("CoolerOn=%d Fan=%d auto_exposure=%d options_decimal=%d hexa=%s"),
            RaptorSet1.CoolerOn, RaptorSet1.FanIsOn,
            RaptorSet1.AutoExposure, options_decimal, out_Hexadecimal);
wxMessageBox(buf0, wxT("JLP_Raptor_Set_FPGA_Options"), wxOK);
#endif // DEBUG

// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE0\\x02\\x00\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0x00;
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

return(0);
}
/*********************************************************************
* Set FPGA CTRL register
**********************************************************************/
int JLP_RaptorCam1::JLP_Raptor_Set_NUC_State()
{
int out_length = 2, bbit[8], i, int_value, status;
long nuc_decimal, power_of_two;
char out_Hexadecimal[out_length + 1], char_value[3];
unsigned char data_send0[64];
wxString buf0;

for(i = 0; i < 8; i++) bbit[i] = 0;

// To enable Bad Pixel show in any mode:
 bbit[0] = 1;


/* NUC State:
* 0: Offset corr.
* 1: Offset+Gain corr.
* 2: Normal
* 3: Offset+Gain+Dark
* 4: 8 bit Offset / 32
* 5: 8 bit Dark * 2^19
* 6: 8 bit Gain / 128
* 7: Reserved map
* 8: Ramp Test Pattern
*/
switch(RaptorSet1.NUC_State) {
// 0: Offset corr.
  default:
  case 0:
    break;
// 1: Offset+Gain corr.
  case 1:
    bbit[5] = 1;
    break;
// 2: Normal
  case 2:
    bbit[6] = 1;
    break;
// 3: Offset+Gain+Dark
  case 3:
    bbit[5] = 1;
    bbit[6] = 1;
    break;
// 4: 8 bit Offset / 32
  case 4:
    bbit[7] = 1;
    break;
// 5: 8 bit Dark * 2^19
  case 5:
    bbit[5] = 1;
    bbit[7] = 1;
    break;
// 6: 8 bit Gain / 128
  case 6:
    bbit[6] = 1;
    bbit[7] = 1;
    break;
// 7: Reserved map
  case 7:
    bbit[4] = 1;
    bbit[7] = 1;
    break;
// 8: Ramp Test Pattern
  case 8:
    bbit[4] = 1;
    break;
 }

nuc_decimal = 0;
power_of_two = 1;
for(i = 0; i < 8; i++) {
      nuc_decimal += bbit[i] * power_of_two;
      power_of_two *= 2;
 }

JLP_DecimalToHexadecimal(nuc_decimal, out_Hexadecimal, out_length);
// DEBUG:
#if DEBUG
buf0.Printf(wxT("NUC_state=%d nuc_decimal=%d hexa=%s"),
            RaptorSet1.NUC_State, nuc_decimal, out_Hexadecimal);
wxMessageBox(buf0, wxT("JLP_Raptor_Set_NUC_State"), wxOK);
#endif // DEBUG

// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE0\\x02\\xF9\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xF9;
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, bas 16:
int_value = strtol(char_value, NULL, 16);
data_send0[4] = int_value;
data_send0[5] = 0x50;
status = JLP_Raptor_SerialSend(data_send0, 6);

return(0);
}
/*********************************************************************
* Set/Unset ImageSharpen mode
* Set Convolve reg.
* 0x53 0xE0 0x02 0xFD 0xYY 0x50
* YY = 0x43 to enable image sharpening
* YY = 0x22 to provide normal image data
**********************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SetImageSharpen()
{
int status;
unsigned char data_send0[64];

// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE0\\x02\\xFD\\x%c%c\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
data_send0[0] = 0x53;
data_send0[1] = 0xE0;
data_send0[2] = 0x02;
data_send0[3] = 0xFD;
if(RaptorSet1.ImageSharpen == 1)
 data_send0[4] = 0x43;
else
 data_send0[4] = 0x22;
data_send0[5] = 0x50;

status = JLP_Raptor_SerialSend(data_send0, 6);

return(status);
}
