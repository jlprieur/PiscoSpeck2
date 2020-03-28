/************************************************************************
* "jlp_raptor_cam1_get.cpp"
* JLP_RaptorCam1 class
*
* JLP
* Version 17/09/2015
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_raptor_cam1.h"      // RAPTOR_SETTINGS, ...

#define DEBUG 0

/************************************************************************
* Display current Raptor settings in a popup window
* for all image acquisition modes
************************************************************************/
int  JLP_RaptorCam1::Cam1_DisplaySettings()
{
wxString buff1, buff2, buff3, buff4, buffer;

  buff1.Printf(wxT("Head model=%s gblXPixels=%d gblYPixels=%d \
xc0=%d yc0=%d xbin=%d ybin=%d nx1=%d ny1=%d\n \
MirrorMode=%d RotationMode=%d\n"),
                   RaptorSet1.HeadModel, RaptorSet1.gblXPixels, RaptorSet1.gblXPixels,
                   RaptorSet1.xc0, RaptorSet1.yc0,
                   RaptorSet1.xbin, RaptorSet1.ybin, RaptorSet1.nx1, RaptorSet1.ny1,
                   RaptorSet1.MirrorMode, RaptorSet1.RotationMode);
  buffer.Append(buff1);
  buff2.Printf(wxT("Expo_time_msec=%d  Integ_time_msec=%d \n \
FrameRate=%d HighGain=%d DigitalGain=%d \n"),
                   RaptorSet1.ExpoTime_msec, RaptorSet1.IntegTime_sec,
                   RaptorSet1.FrameRate, RaptorSet1.HighGain, RaptorSet1.DigitalGain);
  buffer.Append(buff2);
  buff3.Printf(wxT("AcquisitionMode=%d AutoExposure=%d ImageSharpen=%d\n \
 NUC_state=%d "),
                   RaptorSet1.AcquisitionMode, RaptorSet1.AutoExposure, RaptorSet1.ImageSharpen,
                   RaptorSet1.NUC_State);
  buffer.Append(buff3);
  buff3.Printf(wxT("CoolerOn=%d CoolerSetTemp=%.2f FanIsOn=%d ExtTrigger=%d  TriggerDelay=%d\n"),
                   RaptorSet1.CoolerOn, RaptorSet1.CoolerSetTemp, RaptorSet1.FanIsOn,
                   RaptorSet1.ExtTrigger, RaptorSet1.TriggerDelay);
  buffer.Append(buff3);
  buff4.Printf(wxT("CloseShutterWhenExit=%d ShutterMode=%d\n"),
                   RaptorSet1.CloseShutterWhenExit, RaptorSet1.ShutterMode);
  buffer.Append(buff4);
  wxMessageBox(buffer, "Cam1_DisplaySettings", wxOK);

return(0);
}
/**************************************************************************
* Enquire the current temperature to Raptor camera
* Need a short output message since it is displayed on the main form
* of the program
*
* OUTOUT:
* sensor_temp: current sensor temperature value in deg. C
* box_temp: current electronic board temperature value in deg. C
* info_msg: miscellaneous information
**************************************************************************/
int  JLP_RaptorCam1::Cam1_GetTemperature(double *sensor_temp, double *box_temp,
                                         wxString& info_msg)
{
int status;
wxString buffer;

*sensor_temp = 0.;
*box_temp = 0.;
info_msg = wxT("");

status = JLP_Raptor_GetSensorTemperature(sensor_temp, buffer);
status = JLP_Raptor_GetPCBTemperature(box_temp, buffer);
info_msg.Printf(wxT("PCB T=%.2f Sensor T=%.2f (deg. C)"), *box_temp, *sensor_temp);

#if DEBUG
wxMessageBox(buffer, wxT("Get Temperature"), wxOK);
#endif // DEBUG

return(status);
}
/**************************************************************************
* Enquire the current temperature to Raptor camera
* Need a short output message since it is displayed on the main form
* of the program
*
* OUTOUT:
* current_temp: current temperature value in deg. C
* info_msg: miscellaneous information
**************************************************************************/
int  JLP_RaptorCam1::JLP_Raptor_GetSensorTemperature(double *current_temp,
                                                     wxString& info_msg)
{
int status, nread0;
double M_ADC, C_ADC, temp_counts_decimal;
unsigned char data_send0[64], data_read0[64], temper0[16], wwchar;
char comments[128];

*current_temp = 0.;
info_msg = wxT("");

// Y_40 = M_ADC * X_40 + C_ADC
// Y_0 = M_ADC * X_0 + C_ADC
// Y_40 - Y_0 = M_ADC * (X_40 - X_0)
// M_ADC = (Y_40 - Y_0) / (X_40 - X_0)
M_ADC = (40.0 - 0.0) / (EPROM_ADC_Cal_40C - EPROM_ADC_Cal_0C);
// C_ADC = Y_40 - M_ADC * EPROM_ADC_Cal_40C;
C_ADC = 40. - M_ADC * EPROM_ADC_Cal_40C;

info_msg = wxT("");

/*
* Get Sensor Temperature
*
12 bit number returned in 2 bytes
Reg 6E, bits 3..0 = temp bits 11..8
Reg 6F, bits 7..0 = temp bits 7..0
12 bit value to be converted to temperature from ADC calibration values
*
Set and read address 6E, (MSB) 1 byte
0x53 0xE0 0x01 0x6E 0x50
0x53 0xE1 0x01 0x50
Set and read address 6F, (LSB) 1 byte
0x53 0xE0 0x01 0x6F 0x50
0x53 0xE1 0x01 0x50
*/
// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE0\\x01\\x6E\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0x6E;
 data_send0[4] = 0x50;
 status = JLP_Raptor_SerialSend(data_send0, 5);

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get sensor temperature 2/4: read register (expected answer: 0x50)");
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
// Expected answer:
 temper0[0] = data_read0[0];
 temper0[1] = data_read0[1];
 temper0[2] = data_read0[2];

// Commands to serial port:
//sprintf(data_send0, "\\x53\\xE0\\x01\\x6E\\x50",
//        out_Hexadecimal[0], out_Hexadecimal[1]);
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0x6F;
 data_send0[4] = 0x50;
 status = JLP_Raptor_SerialSend(data_send0, 5);

info_msg = wxT("");
// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get sensor temperature 4/4: read register (expected answer: 0x50)");
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 temper0[3] = data_read0[0];
 temper0[4] = data_read0[1];
 temper0[5] = data_read0[2];

// Y_40 = M_ADC * X_40 + C_ADC
// Y_0 = M_ADC * X_0 + C_ADC
// Y_40 - Y_0 = M_ADC * (X_40 - X_0)
// M_ADC = (Y_40 - Y_0) / (X_40 - X_0)

// bits 3,2,1,0 become bits 11 10 9 8 by multiplying it by 256
 wwchar = temper0[0] & 0xFF;
 temp_counts_decimal = wwchar * 256.0 + temper0[3];
 *current_temp = temp_counts_decimal * M_ADC  + C_ADC;
 info_msg.Printf(wxT("T=%.2f "), *current_temp);
#if DEBUG
info_msg.Printf(wxT("T=%.3f M_ADC=%f C_ADC=%f counts=%f %02X %02X %02X %02X %02X %02X"),
                *current_temp, M_ADC, C_ADC, temp_counts_decimal,
                temper0[0], temper0[1], temper0[2], temper0[3],
                temper0[4], temper0[5]);
wxMessageBox(info_msg, wxT("Sensor TEMP"), wxOK);
#endif // DEBUG
return(0);
}
/**************************************************************************
* Enquire the current temperature to Raptor camera
* Need a short output message since it is displayed on the main form
* of the program
*
* OUTOUT:
* current_temp: current temperature value in deg. C
* info_msg: miscellaneous information
**************************************************************************/
int  JLP_RaptorCam1::JLP_Raptor_GetPCBTemperature(double *current_temp,
                                            wxString& info_msg)
{
int status, nread0, isign, nread_max;
unsigned char data_send0[64], data_read0[64], temper0[8], wwchar;
char comments[128];

*current_temp = 0.;
info_msg = wxT("");

/*
* Get sensor PCB temperature
*
12 bit signed number returned in 2 bytes
Reg 70, bits 3..0 = temp bits 11..8
Reg 71, bits 7..0 = temp bits 7..0
Temp bits 11..4 is a signed number that represents the temperature in deg C.
Temp bits 3..0 is a signed number that represents the fractional part of the temp in deg C. i.e. 1/16th of a degC
e.g.
0x7FF = 127.9375 degC
0x7FE = 127.8750 degC
0x000 = 0 degC
:
0x801 = - 127.9375 degC
0x800 = -128.0000 degC
*
Set and read address 70, (MSB) 1 byte
0x53 0xE0 0x01 0x70 0x50
0x53 0xE1 0x01 0x50
Set and read address 71, (LSB) 1 byte
0x53 0xE0 0x01 0x71 0x50
0x53 0xE1 0x01 0x50
*/

// Commands to serial port:
// sprintf(data_send0, "\\x53\\xE0\\x01\\x70\\x50");
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0x70;
 data_send0[4] = 0x50;

// Should return 0x50 0x91
#if DEBUG
 nread_max = 2;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 5, nread_max, &nread0);
 sprintf(comments, "Get PCB temperature 1/4 (expected 0x50)");
 serial_log_command(data_send0, 5, data_read0, nread0, comments);
#else
 status = JLP_Raptor_SerialSend(data_send0, 5);
#endif // DEBUG

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get PCB temperature 0x70 2/4: read register (expected answer: 0x50)");
// Should return 0x01 0x50 0xE3
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 temper0[0] = data_read0[0];
 temper0[1] = data_read0[1];
 temper0[2] = data_read0[2];

// Commands to serial port:
// sprintf(data_send0, "\\x53\\xE0\\x01\\x71\\x50");
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0x71;
 data_send0[4] = 0x50;
// Should return 0x50 0x90
#if DEBUG
 nread_max = 2;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 5, nread_max, &nread0);
 sprintf(comments, "Get PCB temperature 3/4 (expected 0x50)");
 serial_log_command(data_send0, 5, data_read0, nread0, comments);
#else
 status = JLP_Raptor_SerialSend(data_send0, 5);
#endif // DEBUG

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get PCB temperature 0x71 4/4: read register (expected answer: 0x50)");
// Should return 0x93 0x50 0xE3
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 temper0[3] = data_read0[0];
 temper0[4] = data_read0[1];
 temper0[5] = data_read0[2];

//info_msg.Printf(wxT("T=%.2f "), *current_temp);
 wwchar = temper0[0] & 0xFF;
// bit 3 is the sign bit:
 if((wwchar >> 3) == 1) isign = -1;
 else isign = +1;
// bits 3,2,1,0 become bits 11 10 9 8 by multiplying it by 256
 wwchar = temper0[0] & 0x07;
 *current_temp = wwchar * 256 + temper0[3];
 *current_temp *= (double)isign;
// Conversion to degrees by dividing the result by 16:
*current_temp /= 16.;

// ZZ Print temperature as 4 hexa bytes: always 51 92 00 51 93 00 ?
info_msg.Printf(wxT("tt=%.2f %02X %02X %02X %02X %02X %02X"),
                *current_temp,
                temper0[0], temper0[1], temper0[2], temper0[3], temper0[4], temper0[5]);
#if DEBUG
wxMessageBox(info_msg, wxT("ZZ PCB TEMP"), wxOK);
#endif // DEBUG

return(0);
}
/**************************************************************************
*
**************************************************************************/
int JLP_RaptorCam1::JLP_Serial_ReadOutputRegister1(unsigned char *data_read0,
                                                   int *nread0, char *comments0)
{
int i, status, nread_max;
char comments[256];
unsigned char data_send0[64];

*nread0 = 0;
nread_max = 3;
for(i = 0; i < nread_max; i++) data_send0[i] = 0;

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
data_send0[0] = 0x53;
data_send0[1] = 0xE1;
data_send0[2] = 0x01;
data_send0[3] = 0x50;

// nsend=4, expected_nread=3
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 4, nread_max, nread0);
 sprintf(comments, "%s status = %d", comments0, status);
#if DEBUG
 serial_log_command(data_send0, 4, data_read0, *nread0, comments);
#endif // DEBUG

return(status);
}
/**************************************************************************
* Enquire the current exposure time to Raptor camera
*
* OUTOUT:
*   expo_time0: current value of exposure time
**************************************************************************/
int JLP_RaptorCam1::Cam1_GetExpoTime(double *expo_time0)
{
int status, nread0, nread_max;
unsigned char data_send0[64], data_read0[64], expo0[16];
char comments[128];
wxString info_msg;

*expo_time0 = 0.;

/*
Set and read address EE, (MSB) 1 byte
0x53 0xE0 0x01 0xEE 0x50
0x53 0xE1 0x01 0x50
Set address EF,(MIDU) 1 byte
0x53 0xE0 0x01 0xEF 0x50
0x53 0xE1 0x01 0x50
Set address F0, (MIDL)1 byte
0x53 0xE0 0x01 0xF0 0x50
0x53 0xE1 0x01 0x50
Set address F1, (LSB) 1 byte
0x53 0xE0 0x01 0xF1 0x50
0x53 0xE1 0x01 0x50
*** Output:
30 bit value, 4 bytes,
1 count = 1*40MHz period = 25nsecs
2 Upper bits of 0xEE are don’t care’s.
Min Exposure = 500nsec = 20counts
*/
// Commands to serial port:
// sprintf(data_send0, "\\x53\\xE0\\x01\\xEE\\x50");
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0xEE;
 data_send0[4] = 0x50;
#if DEBUG
 nread_max = 2;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 5, nread_max, &nread0);
 sprintf(comments, "Get PCB temperature 1/4 (expected 0x50)");
 serial_log_command(data_send0, 5, data_read0, nread0, comments);
#else
 status = JLP_Raptor_SerialSend(data_send0, 5);
#endif // DEBUG

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get ExpoTime 0xEE 2/8: read register (expected answer: 0x50)");
// Should return 0x01 0x50 0xE3
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 expo0[0] = data_read0[0];
 expo0[1] = data_read0[1];
 expo0[2] = data_read0[2];

// Commands to serial port:
// sprintf(data_send0, "\\x53\\xE0\\x01\\xEF\\x50");
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0xEF;
 data_send0[4] = 0x50;
#if DEBUG
 nread_max = 2;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 5, nread_max, &nread0);
 sprintf(comments, "Get PCB temperature 1/4 (expected 0x50)");
 serial_log_command(data_send0, 5, data_read0, nread0, comments);
#else
 status = JLP_Raptor_SerialSend(data_send0, 5);
#endif // DEBUG

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get ExpoTime 0xEF 4/8: read register (expected answer: 0x50)");
// Should return 0x93 0x50 0xE3
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 expo0[3] = data_read0[0];
 expo0[4] = data_read0[1];
 expo0[5] = data_read0[2];

// Commands to serial port:
// sprintf(data_send0, "\\x53\\xE0\\x01\\xF0\\x50");
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0xF0;
 data_send0[4] = 0x50;
#if DEBUG
 nread_max = 2;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 5, nread_max, &nread0);
 sprintf(comments, "Get PCB temperature 1/4 (expected 0x50)");
 serial_log_command(data_send0, 5, data_read0, nread0, comments);
#else
 status = JLP_Raptor_SerialSend(data_send0, 5);
#endif // DEBUG

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get ExpoTime 0xF0 6/8: read register (expected answer: 0x50)");
// Should return 0x93 0x50 0xE3
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 expo0[6] = data_read0[0];
 expo0[7] = data_read0[1];
 expo0[8] = data_read0[2];

// Commands to serial port:
// sprintf(data_send0, "\\x53\\xE0\\x01\\xF1\\x50");
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0xF1;
 data_send0[4] = 0x50;
#if DEBUG
 nread_max = 2;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 5, nread_max, &nread0);
 sprintf(comments, "Get PCB temperature 1/4 (expected 0x50)");
 serial_log_command(data_send0, 5, data_read0, nread0, comments);
#else
 status = JLP_Raptor_SerialSend(data_send0, 5);
#endif // DEBUG

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get ExpoTime 0xF1 8/8: read register (expected answer: 0x50)");
// Should return 0x93 0x50 0xE3
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 expo0[9] = data_read0[0];
 expo0[10] = data_read0[1];
 expo0[11] = data_read0[2];

 *expo_time0 = ((((expo0[0] * 256) + expo0[3]) * 256) + expo0[6]) * 256 + expo0[9];
// From counts of 25nsec to msec
 *expo_time0 *= (25. / 1000000.);
#if DEBUG
info_msg.Printf(wxT("expo_time=%.2f %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"),
                *expo_time0, expo0[0], expo0[1], expo0[2],
                expo0[3], expo0[4], expo0[5], expo0[6],
                expo0[7], expo0[8], expo0[9], expo0[10], expo0[11]);
wxMessageBox(info_msg, wxT("Inquire ExpoTime"), wxOK);
#endif // DEBUG

return(0);
}
/**************************************************************************
* Enquire the current digital gain to Raptor camera
*
* OUTOUT:
*  digital_gain0: current value of digital_gain
**************************************************************************/
int JLP_RaptorCam1::Cam1_GetDigitalGain(double *digital_gain0)
{
int status, nread0, nread_max;
unsigned char data_send0[64], data_read0[64], gain0[8];
char comments[128];
wxString info_msg;

*digital_gain0 = 0.;
/*
2 bytes returned MM,LL
16bit value = gain*256
Reg. C6 bits 7..0 = gain bits 15..8
Reg. C7 bits 7..0 = level bits 7..0
*
Set and read address C6 (MSB) , 1 byte
0x53 0xE0 0x01 0xC6 0x50
0x53 0xE1 0x01 0x50
Set and read address C7 (LSB) , 1 byte
0x53 0xE0 0x01 0xC7 0x50
0x53 0xE1 0x01 0x50
*/
// Commands to serial port:
// sprintf(data_send0, "\\x53\\xE0\\x01\\xC6\\x50");
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0xC6;
 data_send0[4] = 0x50;
#if DEBUG
 nread_max = 2;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 5, nread_max, &nread0);
 sprintf(comments, "Get DigitalGain 1/4 (expected 0x50)");
 serial_log_command(data_send0, 5, data_read0, nread0, comments);
#else
 status = JLP_Raptor_SerialSend(data_send0, 5);
#endif // DEBUG

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get DigitalGain 0xC0 2/4: read register (expected answer: 0x50)");
// Should return 0x01 0x50 0xE3
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 gain0[0] = data_read0[0];
 gain0[1] = data_read0[1];
 gain0[2] = data_read0[2];

// Commands to serial port:
// sprintf(data_send0, "\\x53\\xE0\\x01\\xC7\\x50");
 data_send0[0] = 0x53;
 data_send0[1] = 0xE0;
 data_send0[2] = 0x01;
 data_send0[3] = 0xC7;
 data_send0[4] = 0x50;
#if DEBUG
 nread_max = 2;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 5, nread_max, &nread0);
 sprintf(comments, "Get DigitalGain 3/4 (expected 0x50)");
 serial_log_command(data_send0, 5, data_read0, nread0, comments);
#else
 status = JLP_Raptor_SerialSend(data_send0, 5);
#endif // DEBUG

// Command to serial port:
//sprintf(data_send0, "\\x53\\xE1\\x01\\x50");
 sprintf(comments, "Get DigitalGain 4/4: read register (expected answer: 0x50)");
// Should return 0x93 0x50 0xE3
 JLP_Serial_ReadOutputRegister1(data_read0, &nread0, comments);
 gain0[3] = data_read0[0];
 gain0[4] = data_read0[1];
 gain0[5] = data_read0[2];

 *digital_gain0 = (gain0[0] * 256) + gain0[3];
//
 *digital_gain0 /= 256.;
#if DEBUG
info_msg.Printf(wxT("digital_gain=%.2f %02X %02X %02X %02X %02X %02X"),
                *digital_gain0, gain0[0], gain0[1], gain0[2],
                gain0[3], gain0[4], expo0[5]);
wxMessageBox(info_msg, wxT("Inquire DigitalGain"), wxOK);
#endif // DEBUG


return(0);
}
/**************************************************************************
* Enquire the current frame rate to Raptor camera
* (used when internal trigger is selected)
*
* OUTOUT:
*   frame_rate0: current value of frame rate
**************************************************************************/
int  JLP_RaptorCam1::JLP_Raptor_GetFrameRate(int *frame_rate0)
{
/*
32 bit value, 4 separate Registers,
1 count = 1*5MHz period = 200nsecs
DD = MSB of 4 byte word
:
:
:
E0 = LSB of 4 byte word;
*
Set and read address DD (MSB), 1 byte
0x53 0xE0 0x01 0xDD 0x50
0x53 0xE1 0x01 0x50
Set and read address DE, 1 byte
0x53 0xE0 0x01 0xDE 0x50
0x53 0xE1 0x01 0x50
Set and read address DF, 1 byte
0x53 0xE0 0x01 0xDF 0x50
0x53 0xE1 0x01 0x50
Set and read address E0 (LSB), 1 byte
0x53 0xE0 0x01 0xE0 0x50
0x53 0xE1 0x01 0x50
*/

return(0);
}
/**************************************************************************
* Enquire the current frame rate to Raptor camera
*
* OUTOUT:
*   horiz_flip0
*   video_inverted0
*   fan_enabled0
*   auto_exposure_enabled0
*   TEC_enabled0
**************************************************************************/
int  JLP_RaptorCam1::JLP_Raptor_GetFPGAStatus(bool *horiz_flip0, bool *video_inverted0,
                                        bool *fan_enabled0, bool *auto_exposure_enabled0,
                                        bool *TEC_enabled0)
{
/*
Set address 00 (MSB)
Read address 00 (MSB), 1 byte
Bit 7 = 1 if horiz flip is enabled (default = 1)
Bit 6 = 1 if video is inverted (default = 0)
Bit 2 = 1 if the fan is enabled (default = 0, note this only applies to the Owl 640 cooled variant)
Bit 1 = 1 if auto exposure is enabled (default = 0)
Bit 0 = 1 if the TEC is enabled (default = 0)
*
Set and read address 00 (MSB), 1 byte
0x53 0xE0 0x01 0x00 0x50
0x53 0xE1 0x01 0x50
*/

return(0);
}
/**************************************************************************
* Enquire the current TEC set point to Raptor camera
*
* OUTOUT:
*   cooler_set_temp0: current value of frame rate
**************************************************************************/
int  JLP_RaptorCam1::JLP_Raptor_GetCoolerSetTemp(double *cooler_set_temp0)
{
/*
Read 12 bit EM word;
2 bytes returned MM,LL
Reg 0xFB, bits 3..0 = set point bits 11..8
Reg 0xFA, bits 7..0 = set point bits 7..0
12 bit value to be converted to temperature from DAC calibration values
*
Set and read address FB (MSB), 1 byte
0x53 0xE0 0x01 0xFB 0x50
0x53 0xE1 0x01 0x50
Set and read address FA (LSB), 1 byte
0x53 0xE0 0x01 0xFA 0x50
0x53 0xE1 0x01 0x50
*/

return(0);
}
/**************************************************************************
* Enquire the current NUC state to Raptor camera
*
* OUTOUT:
*   current_nuc0: current value of NUC state
**************************************************************************/
int  JLP_RaptorCam1::JLP_Raptor_GetNUCState(double *current_nuc0)
{
/*
1 byte returned
Bit7 Bit6 Bit5 Bit4
0 0 0 1 Ramp test pattern
0 0 0 0 offset corrected
0 0 1 0 offset+gain corrected
0 1 0 0 Normal
0 1 1 0 offset +gain + Dark
1 0 0 0 8bit offset /32
1 0 1 0 8bit Dark *2^19
1 1 0 0 8bit gain /128
1 0 0 1 Reserved map
Bit 3,2,1 = Reserved set = 0.
Bit 0 = 1 if Bad Pixel show is enabled
*
Set and read address F9 (MSB), 1 byte
0x53 0xE0 0x01 0xF9 0x50
0x53 0xE1 0x01 0x50
*/
return(0);
}
