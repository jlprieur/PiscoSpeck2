/************************************************************************
* "jlp_raptor_cam1.cpp"
* JLP_RaptorCam1 class
*
* JLP
* Version 11/01/2018
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_raptor_cam1.h"      // RAPTOR_SETTINGS, ...
#include "jlp_raptor_utils.h"     // Copy_RaptorSettings, ...

#ifndef MAXI
#define MAXI(a,b) ((a) > (b)) ? (a) : (b)
#endif // MAXI
#ifndef MINI
#define MINI(a,b) ((a) < (b)) ? (a) : (b)
#endif // MINI

#ifndef FORMAT
// For PIXCI(R) A, CL1, CL2, CL3SD, D, D24, D32,
// D2X, D3X, D3XE, E1, E1DB, E4, E4DB, E4G2, E8, E8CAM, E8DB, e104x4, EB1, EB1-POCL, EB1mini,
// EC1, ECB1, ECB1-34, ECB2, EL1, EL1DB, ELS2,
// SI, SI1, SI2, SI4
    #define FORMAT  "default"	  // as per board's intended camera
#endif

#define DEBUG 0

//---------------------------------------------------------------------------
/*
 *  2.1) Set number of expected PIXCI(R) image boards, from 1 to 4.
 *  The XCLIB Simple 'C' Functions expect that the boards are
 *  identical and operated at the same resolution.
 *
 *  For PIXCI(R) frame grabbers with multiple, functional units,
 *  the XCLIB presents the two halves of the
 *  PIXCI(R) E1DB, E4DB, E4G2-2F, E8CAM, E8DB, e104x4-2f, ECB2, EL1DB, ELS2, SI2, or SV7 frame grabbers,
 *  or the three parts of the PIXCI(R) E4G2-F2B or e104x4-f2b frame grabbers,
 *  or the four quarters of the PIXCI(R) E4G2-4B, e104x4-4b, or SI4 frame grabbers,
 *  as two, three, or four independent PIXCI(R) frame grabbers, respectively.
 *
 */
#if !defined(UNITS)
    #define UNITS	1
#endif
#define UNITSMAP    ((1<<UNITS)-1)  /* shorthand - bitmap of all units */
#if !defined(UNITSOPENMAP)
    #define UNITSOPENMAP UNITSMAP
#endif

/*
 *  2.2) Optionally, set driver configuration parameters.
 *  These are normally left to the default, "".
 *  The actual driver configuration parameters include the
 *  desired PIXCI(R) frame grabbers, but to make configuation easier,
 *  code, below, will automatically add board selection to this.
 */
#if !defined(DRIVERPARMS)
  //#define DRIVERPARMS "-QU 0"   // don't use interrupts
    #define DRIVERPARMS ""	  // default
#endif
// #define DEBUG

/***************************************************************************
* Start dialog with Raptor camera
****************************************************************************/
JLP_RaptorCam1::JLP_RaptorCam1()
{
int status;
wxString error_msg;

not_connected = true;
RaptorSet1.pImageArray = NULL;

status = Cam1_ConnectToCamera();

return;
}
/***************************************************************************
* Release the Raptor camera
****************************************************************************/
JLP_RaptorCam1::~JLP_RaptorCam1()
{
if(!not_connected) Cam1_ShutDown();
}
/*************************************************************************
* Open connection to RAPTOR camera for the first time (if present)
*
**************************************************************************/
int JLP_RaptorCam1::Cam1_ConnectToCamera()
{
int status;
//
// Open the PIXCI(R) frame grabber.
// If this program were to only support a single PIXCI(R)
// frame grabber, the first parameter could be simplified to:
//
//	if (pxd_PIXCIopen("", FORMAT, NULL) < 0)
//	    pxd__mesgFault(1);
//
// But, for the sake of multiple PIXCI(R) frame grabbers
// specify which units are to be used.
//

char driverparms[80];
	driverparms[sizeof(driverparms)-1] = 0;
// this & snprintf: overly conservative - avoids warning messages
	snprintf(driverparms, sizeof(driverparms)-1, "-DM 0x%x %s", UNITSOPENMAP, DRIVERPARMS);

//  if (pxd_PIXCIopen(driverparms, FORMAT, "./Owl640-video-setup.fmt") < 0) {
  if (pxd_PIXCIopen(driverparms, FORMAT, "./Ninox640-video-setup.fmt") < 0) {
	pxd_mesgFault(UNITSMAP);
    wxMessageBox(wxT("Error opening connection to EPIX board"),
                 wxT("Cam1_ConnectToCamera"), wxICON_ERROR);
    not_connected = true;
    return(-1);
    }

not_connected = false;
RaptorSet1.pImageArray = NULL;

//
// Init serial port if not done already
// int pxd_serialConfigure(unitmap, rsvd0, baud, bits, parity, stopbits, rsvd1, rsvd2, rsvd3);
// rsvd reserved: should be 0
// 9600 very bad ! status = pxd_serialConfigure(0x1, 0,  9600, 8, 0, 1, 0, 0, 0);
// From OWL640 Ninox instruction manual:
   check_sum1 = true;
   status = pxd_serialConfigure(0x1, 0, 115200, 8, 0, 1, 0, 0, 0);

   if(status != 0) {
     wxMessageBox(wxT("Fatal error opening serial link to EPIX board"),
                  wxT("Cam1_ConnectToCamera"), wxICON_ERROR);
     exit(-2); // error
   }

// Initialisation of serial communication:
   JLP_Raptor_Serial_Initialize();

// First Initialization:
// Set RaptorSet1.is_initialized to 1 or 0:
  JLP_Raptor_FirstInit();

return(0);
}
/*************************************************************************
* Initialize RAPTOR camera for the first time (called
* when creating RaptorSetupBox)
* Selection of the settings included in RaptorSet
*
* Set RaptorSet1.is_initialized to 1 or 0
*
**************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_FirstInit()
{
int status, width0, height0;
bool errorFlag = false;
wxString buffer, error_msg;
double  scalex, scaley, aspect;

 RaptorSet1.is_initialized = 0;

 if(not_connected) {
   return(-1);
 }

// Head Model
// strcpy(RaptorSet1.HeadModel, "Owl 640");
 strcpy(RaptorSet1.HeadModel, "Ninox 640");

// Initialize EPROM values with the C2PU Ninox values to avoid problems in case of error:
 EPROM_ADC_Cal_0C = 1218;
 EPROM_ADC_Cal_40C = 787;
 EPROM_DAC_Cal_0C = 1680;
 EPROM_DAC_Cal_40C = 2535;

 RaptorSet1.gblXPixels = pxd_imageXdim();
 RaptorSet1.gblYPixels = pxd_imageYdim();
 RaptorSettingsToDefault(&RaptorSet1, RaptorSet1.gblXPixels,
                         RaptorSet1.gblYPixels);

 RaptorSet1.pImageArray = new AWORD[RaptorSet1.gblXPixels * RaptorSet1.gblYPixels];

#if DEBUG
  error_msg.Printf(wxT("gblXPixels=%d gblYPixels=%d xbin=%d ybin=%d, \
                       CoolerOn=%d CoolerSetTemp=%.2f Expo_time_msec=%d DigitalGain=%d"),
                   RaptorSet1.gblXPixels, RaptorSet1.gblXPixels,
                   RaptorSet1.xbin, RaptorSet1.ybin, RaptorSet1.CoolerOn,
                   RaptorSet1.CoolerSetTemp, RaptorSet1.ExpoTime_msec, RaptorSet1.DigitalGain);
  wxMessageBox(error_msg, "JLP_Raptor_FirstInit", wxOK);
#endif

/*****
// Screen size:
 width0 = 400;
 height0 = 300;
 aspect = pxd_imageAspectRatio();
 if (aspect == 0.0) aspect = 1.0;
 scalex = width0 /(double)RaptorSet1.gblXPixels;
 scaley = height0 /((double)RaptorSet1.gblYPixels*aspect);
 scalex = MINI(scalex, scaley);
 width0 = (int)(RaptorSet1.gblXPixels * scalex);
 height0 = (int)(RaptorSet1.gblYPixels * scalex * aspect);

//
// If StrecthDIBits is to be used, some VGA card drivers
// abhor horizontal dimensions which are not a multiple of 4.
// This isn't needed for other rendering methods, but doesn't hurt.
//
 width0 &= ~3;
***/

// Display all error messages if present:
  if(errorFlag) {
     wxMessageBox(error_msg, _T("JLP_Raptor_FirstInit"),
                   wxICON_ERROR);
     status = -1;
    } else {
     RaptorSet1.is_initialized = 1;
// This is the first time cooler is called:
     if(RaptorSet1.CoolerOn) JLP_Raptor_StartCooler();
// Apply all the current settings:
     JLP_Raptor_ApplyBasicSettings();
     status = 0;
    }

return(status);
}
/****************************************************************************
* Shutdown RAPTOR camera
* and take care of cooling system (slow re-heating...)
*
*****************************************************************************/
int JLP_RaptorCam1::Cam1_ShutDown()
{
int status = 0;
bool shutter_available;
int shutter_mode;

  if(RaptorSet1.is_initialized == 0) return(-1);

// Switch off cooler (if used)
  status = JLP_Raptor_StopCooler();

// Shut down system
  status = pxd_PIXCIclose();
  if(status != 0) {
    wxMessageBox(_T("Warning: error shutting down"),_T("JLP_Raptor_Shutdown"),
               wxICON_ERROR);
    status = -3;
    }

// Free allocated memory
  if(RaptorSet1.pImageArray != NULL){
    delete[] RaptorSet1.pImageArray;
    RaptorSet1.pImageArray = NULL;
  }

RaptorSet1.is_initialized = 0;

return(status);
}
/***************************************************************************
* Get RAPTOR_SETTINGS structure
****************************************************************************/
int JLP_RaptorCam1::Get_RaptorSettings(RAPTOR_SETTINGS* RaptorSet0)
{
int status;
// Copy from RaptorSet1 to RaptorSet0
status = Copy_RaptorSettings(RaptorSet1, RaptorSet0);
return(status);
}
/***************************************************************************
* Load RAPTOR_SETTINGS structure
****************************************************************************/
int JLP_RaptorCam1::Load_RaptorSettings(RAPTOR_SETTINGS RaptorSet0)
{
int status;
// Copy from RaptorSet0 to RaptorSet1
status = Copy_RaptorSettings(RaptorSet0, &RaptorSet1);
return(status);
}
/***************************************************************************
* Get JLP_CAMERA_SETTINGS structure
****************************************************************************/
int JLP_RaptorCam1::Cam1_Get_CameraSettings(JLP_CAMERA_SETTINGS* CamSet0)
{
int status;
// Copy RaptorSet1 to CamSet0
status = Copy_RaptorToCameraSettings(RaptorSet1, CamSet0);
return(status);
}
/***************************************************************************
* Get JLP_CAMERA_SETTINGS structure
****************************************************************************/
int JLP_RaptorCam1::Cam1_Load_CameraSettings(JLP_CAMERA_SETTINGS CamSet0)
{
int status;
// Copy CamSet0 to RaptorSet1
status = Copy_CameraToRaptorSettings(CamSet0, &RaptorSet1);
return(status);
}
/*********************************************************************
*
**********************************************************************/
int JLP_RaptorCam1::JLP_Raptor_Serial_Initialize()
{
int status, i, bbit[8], int_value, out_length = 2, nread0, ntry;
long my_options, power_of_two;
unsigned char data_send0[64], data_read0[64];
char out_Hexadecimal[out_length + 1], char_value[3], comments[128];
wxString buffer;

// Get system status:
// Poll this command until the Rx byte, Bit 2 = 1 to indicate that the FPGA has booted successfully
// sprintf(data_send0, "\\x49\\x50\\x19");
data_send0[0] = 0x49;
data_send0[1] = 0x50;
data_send0[2] = 0x19;
// Check sum: 0x49 ^ 0x50 = 0x19
// Note that the check sum byte has been added here but is not required
// at power up as the camera has not yet been set to check sum mode

status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 3, 2, &nread0);
#if DEBUG
// Expected value 0x36 0x50
// At this stage only the status byte is returned.
// Above byte bit 2 = 1 indicates FPGA booted. A value of 0x02 would indicate not booted
 strcpy(comments, "Get system status: expected 0x36 0x50 (check sum, ack, FPGA booted, FPGA_no_reset, FPGA_coms_unabled)");
// Display the answer in the log routine:
 serial_log_command(data_send0, 3, data_read0, nread0, comments);
#endif // DEBUG

/**************
// Set system status (expected answer: 0x50)
// 0x4F 0xYY 0x50
// 0YY=0x52 enable checksum, enable ack, disable FPGA in reset, disable FPGA eprom communication
// 0YY=0x53 enable checksum, enable ack, disable FPGA in reset, enable FPGA eprom communication

for(i = 0; i < 8; i++) bbit[i] = 0;

//YY Bits 7,5,3,2 = Reserved set to 0

//YY Bit 6 = 0 to disable check sum mode
bbit[6] = 0;

//YY Bit 4 = 1 to enable command ack
bbit[4] = 1;

//YY Bit 1 = 1 to disable Hold FPGA in RESET
bbit[1] = 1;

//YY Bit 0 = 1 to enable comms to FPGA EPROM
bbit[0] = 1;
my_options = 0;
power_of_two = 1;
for(i = 0; i < 8; i++) {
      my_options += bbit[i] * power_of_two;
      power_of_two *= 2;
 }
data_send0[1] = my_options;

JLP_DecimalToHexadecimal(my_options, out_Hexadecimal, out_length);
sprintf(char_value, "%c%c", out_Hexadecimal[0], out_Hexadecimal[1]);
// convertion string to long, basis 16:
int_value = strtol(char_value, NULL, 16);
data_send0[1] = int_value;
buffer.Printf(wxT("my_options: %d (or %02X) -> char_value=%s -> int_value=%d (or %02X)"),
              my_options, my_options, char_value, int_value, int_value);
wxMessageBox(buffer, wxT("DEBUGZZ"), wxOK);
***********/


//Single byte will be transmitted from camera when command received.
// YY bits 7,5,3= Reserved
// YY bit 6 = 1 if check sum mode enabled
// YY bit 4 = 1 if command ack enabled
// YY bit 2 = 1 if FPGA booted successfully
//YY bit 1 = 0 if FPGA is held in RESET
//YY bit 0 = 1 if comms is enabled to FPGA EPROM


// Doc exel Communal/Ninox:
// Set system status 13 enable ack, enable EPROM com
// Set system status 53 enable ack, checksum, enable EPROM com
// sprintf(data_send0, "\\x4F\\x53\\x50", 3);
data_send0[0] = 0x4F;
if(check_sum1 == true) data_send0[1] = 0x53;
 else data_send0[1] = 0x13;
data_send0[2] = 0x50;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 3, 1, &nread0);
if(check_sum1 == true)
   strcpy(comments, "Set system status 53 enable ack, checksum, enable EPROM com (expected: 0x50)");
else
   strcpy(comments, "Set system status 13 enable ack, no_checksum, enable EPROM com (expected: 0x50)");
#if DEBUG
 serial_log_command(data_send0, 3, data_read0, nread0, comments);
#endif // DEBUG
 if(data_read0[0] != 0x50) {
   buffer.Printf(wxT("Error, system status has returned >%02X<"), data_read0[0]);
   wxMessageBox(buffer, wxT("JLP_Raptor_serial_Initialize"), wxOK | wxICON_ERROR);
 }

// Get system status again to see if previous command has been applied
data_send0[0] = 0x49;
data_send0[1] = 0x50;
status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 2, 2, &nread0);
strcpy(comments, "Get system status: expected 0x57 0x50 (check sum, ack, FPGA booted, FPGA_no_reset, FPGA_coms_abled)");
// Display the answer in the log routine:
#if DEBUG
 serial_log_command(data_send0, 2, data_read0, nread0, comments);
#endif // DEBUG

// Get manufacters data 1/2
// sprintf(data_send0, "\\x53\\xAE\\x05\\x01\\x00\x00\\x02\\x00\\x50");
data_send0[0] = 0x53;
data_send0[1] = 0xAE;
data_send0[2] = 0x05;
data_send0[3] = 0x01;
data_send0[4] = 0x00;
data_send0[5] = 0x00;
data_send0[6] = 0x02;
data_send0[7] = 0x00;
data_send0[8] = 0x50;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 9, 1, &nread0);
 strcpy(comments, "Get manufacters data 1/2 (expected: 0x50)");
 serial_log_command(data_send0, 9, data_read0, nread0, comments);
 if(data_read0[0] != 0x50) {
   buffer.Printf(wxT("Error: system status has returned >%02X<\n (0x52=check sum error, 0x53=Command received but failed internally in the camera)"),
                      data_read0[0]);
   wxMessageBox(buffer, wxT("JLP_Raptor_serial_Initialize"), wxOK | wxICON_ERROR);
// Prepare dummy input for JLP_Raptor_DecodeSerialNumber()
   nread0 = 0;
  } else {

// Get manufacters data 2/2
// sprintf(data_send0, "\\x53\\xAF\\x12\\x50");
// Example of answer
// 6727170c0e4c61726e65ba040503a706130a50be
// Ninox Calern January 2018:
// 12 27 08 07 0F 4C 41 52 4E 45 C2 04 13 03 90 06 E7 09
data_send0[0] = 0x53;
data_send0[1] = 0xAF;
data_send0[2] = 0x12;
data_send0[3] = 0x50;
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 4, 19, &nread0);
 sprintf(comments, "Get manufacters data 2/2 (expected answer: 19 bytes), status = %d",
         status);
#if DEBUG
 serial_log_command(data_send0, 4, data_read0, nread0, comments);
#endif
  }

 JLP_Raptor_DecodeSerialNumber(data_read0, nread0);

// Set system status 52 (4F 12 50 4D) enable ack, checksum, disable EPROM com
// Set system status 52 enable ack, checksum, disable EPROM com
// Set system status 12 enable ack, no checksum, disable EPROM com
// sprintf(data_send0, "\\x4F\\x12\\x50");
data_send0[0] = 0x4F;
if(check_sum1 == true)
  data_send0[1] = 0x52;
else
  data_send0[1] = 0x12;
data_send0[2] = 0x50;
// Check sum: 0x4F ^ 0x52 ^ 0x50 = 0x4D
 status = JLP_Raptor_SerialSendReceive(data_send0, data_read0, 3, 1, &nread0);
if(check_sum1 == true)
 strcpy(comments, "Set system status, 52 enable ack, checksum, disable EPROM com (expected: 0x50)");
else
 strcpy(comments, "Set system status, 12 enable ack, no_checksum, disable EPROM com (expected: 0x50)");
#if DEBUG
 serial_log_command(data_send0, 3, data_read0, nread0, comments);
#endif

 if(data_read0[0] != 0x50) {
   buffer.Printf(wxT("Error: system status has returned >%02X< \n (0x52=check sum error, 0x53=Command received but failed internally in the camera)"), data_read0[0]);
   wxMessageBox(buffer, wxT("JLP_Raptor_serial_Initialize"), wxOK | wxICON_ERROR);
  }

return(0);
}
/***********************************************************************************
* Ninox Calern January 2018:
* 12 27 08 07 0F 4C 41 52 4E 45 C2 04 13 03 90 06 E7 09 50
*
* When error: ?
* 12 27 08 07 0F 4C 41 52 4E 45 C2 04 13 03 90 06 E7 XX 50
*
***********************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_DecodeSerialNumber(unsigned char *dread0, int nread0)
{
int status = -1;
long serial_number0, date_dd, date_mm, date_yy, int_value;
char char_value[16], build_code[16];
wxString buffer;

// In case of error, load ithe C2PU Ninox values:
 if((nread0 < 19) || (dread0[3] > 12)) {
   EPROM_ADC_Cal_0C = 1218;
   EPROM_ADC_Cal_40C = 787;
   EPROM_DAC_Cal_0C = 1680;
   EPROM_DAC_Cal_40C = 2535;
   return(status);
   }

// Ninox Calern January 2018:
// 12 27 -> 2712 -> 10002
// Serial number : 0,1,2,3 reversed: 2,3,0,1 and converted to int:
 sprintf(char_value, "%02X%02X", dread0[1], dread0[0]);
 char_value[4] = '\0';
// conversion string to long, basis 16:
 serial_number0 = strtol(char_value, NULL, 16);

// Ninox Calern January 2018:
// 08 07 0F -> 08 07 15
// Date : dd=4,5 mm=6,7 yy=8,9 and converted to int:
 sprintf(char_value, "%02X", dread0[2]);
 char_value[2] = '\0';
 date_dd = strtol(char_value, NULL, 16);

 sprintf(char_value, "%02X", dread0[3]);
 char_value[2] = '\0';
 date_mm = strtol(char_value, NULL, 16);

 sprintf(char_value, "%02X", dread0[4]);
 char_value[2] = '\0';
 date_yy = strtol(char_value, NULL, 16);

// Ninox Calern January 2018:
// 4C 41 52 4E 45 -> 4C41524E45 -> LARNE
// Built code=10,11,12,13,14,15,16,17,18,19 and converted to char:
 sprintf(build_code, "%c%c%c%c%c",
         dread0[5], dread0[6], dread0[7], dread0[8], dread0[9]);
 build_code[10] = '\0';

// Ninox Calern January 2018:
// C2 04 -> 04C2 -> 1218
// ADC_cal0deg = 20,21,22,23 inverted and converted to int:
 sprintf(char_value, "%02X%02X", dread0[11], dread0[10]);
 char_value[4] = '\0';
 EPROM_ADC_Cal_0C = strtol(char_value, NULL, 16);

// Ninox Calern January 2018:
// 13 03 -> 03 13 -> 787
// ADC_cal40deg = 24,25,26,27 inverted and converted to int:
 sprintf(char_value, "%02X%02X", dread0[13], dread0[12]);
 char_value[4] = '\0';
 EPROM_ADC_Cal_40C = strtol(char_value, NULL, 16);

// Ninox Calern January 2018:
// 90 06 -> 06 90 -> 1680
// DAC_cal0deg = 28,29,30,31 inverted and converted to int:
 sprintf(char_value, "%02X%02X", dread0[15], dread0[14]);
 char_value[4] = '\0';
 EPROM_DAC_Cal_0C = strtol(char_value, NULL, 16);

// Ninox Calern January 2018:
// E7 09 -> 09 E7 -> 2535
// DAC_cal40deg = 32,33,34,35 inverted and converted to int:
 sprintf(char_value, "%02X%02X", dread0[17], dread0[16]);
 char_value[4] = '\0';
 EPROM_DAC_Cal_40C = strtol(char_value, NULL, 16);

// Display result:
 buffer.Printf(wxT("Raptor serial number: %d Fabrication date: %02d/%02d/%02d \n Build code: %s ADC_cal_0C=%d, ADC_cal_40C=%d \n DAC_cal_0C=%d, DAC_cal_40C=%d"),
             serial_number0, (int)date_dd, (int)date_mm, (int)date_yy, build_code,
             EPROM_ADC_Cal_0C, EPROM_ADC_Cal_40C, EPROM_DAC_Cal_0C, EPROM_DAC_Cal_40C);

wxMessageBox(buffer, wxT("ReadSerialNumber"), wxOK);


return(status);
}
