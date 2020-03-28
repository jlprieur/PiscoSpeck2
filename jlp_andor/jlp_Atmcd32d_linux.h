/****************************************************************************
* jlp_Atmcd32d_linux.h
* Dummy Andor library definitions to replace Windows ones
*
* JLP
* Version 28/08/2015
*****************************************************************************/
#ifndef __jlp_atmcd32d_h
#define __jlp_atmcd32d_h

// JLP2017:
// Case: WIN32 API
#ifdef ANDOR_WIN32
#include "Atmcd32d.h"      // Andor functions
#else

#ifdef __cplusplus
extern "C" {
#endif

#define at_32 long
#define at_u32 unsigned long
#define ULONG unsigned long
// Dummy definitions:
#define AWORD unsigned short int
#define DWORD unsigned int
#define HANDLE unsigned long
#define HWND unsigned long
#define SYSTEMTIME long

// ===================================
// Version Information Definitions
// ===================================

// Version Information Enumeration - NOTE: Increment the count constant
// below when this structure is extended
typedef enum {
// Using large numbers to force size to an integer
  AT_SDKVersion          = 0x40000000,
  AT_DeviceDriverVersion = 0x40000001,
} AT_VersionInfoId;

// Count of the number of elements in the Version Information Enumeration
// NOTE: Increment when extending enumeration
#define AT_NoOfVersionInfoIds 2

// Minimum recommended length of the Version Info buffer parameter
#define AT_VERSION_INFO_LEN 80
// Minimum recommended length of the Controller Card Model buffer parameter
#define AT_CONTROLLER_CARD_MODEL_LEN 80
// ===================================

// ===================================
// DDG Lite Definitions
// ===================================

// Channel enumeration
typedef enum {
  // Using large numbers to force size to an integer
  AT_DDGLite_ChannelA = 0x40000000,
  AT_DDGLite_ChannelB = 0x40000001,
  AT_DDGLite_ChannelC = 0x40000002
} AT_DDGLiteChannelId;

// Control byte flags
#define AT_DDGLite_ControlBit_GlobalEnable   0x01

#define AT_DDGLite_ControlBit_ChannelEnable  0x01
#define AT_DDGLite_ControlBit_FreeRun        0x02
#define AT_DDGLite_ControlBit_DisableOnFrame 0x04
#define AT_DDGLite_ControlBit_RestartOnFire  0x08
#define AT_DDGLite_ControlBit_Invert         0x10
// ===================================

typedef struct ANDORCAPS
{
  ULONG ulSize;
  ULONG ulAcqModes;
  ULONG ulReadModes;
  ULONG ulTriggerModes;
  ULONG ulCameraType;
  ULONG ulPixelMode;
  ULONG ulSetFunctions;
  ULONG ulGetFunctions;
  ULONG ulFeatures;
  ULONG ulPCICard;
  ULONG ulEMGainCapability;
  ULONG ulFTReadModes;
} AndorCapabilities;

typedef struct COLORDEMOSAICINFO
{
  int iX; // Number of X pixels. Must be >2.
  int iY; // Number of Y pixels. Must be >2.
  int iAlgorithm;  // Algorithm to demosaic image.
  int iXPhase;   // First pixel in data (Cyan or Yellow/Magenta or Green). For Luca/SurCam = 1.
  int iYPhase;   // First pixel in data (Cyan or Yellow/Magenta or Green). For Luca/SurCam = 0.
  int iBackground;  // Background to remove from raw data when demosaicing. Internal Fixed Baseline Clamp for Luca/SurCam is 512
} ColorDemosaicInfo;

typedef struct WHITEBALANCEINFO
{
  int iSize;  // Structure size
  int iX; // Number of X pixels. Must be >2.
  int iY; // Number of Y pixels. Must be >2.
  int iAlgorithm;  // Algorithm to calculate white balance.
  int iROI_left;   // Region Of Interest from which the White Balance is calculated
  int iROI_right;  // Region Of Interest from which the White Balance is calculated
  int iROI_top;    // Region Of Interest from which the White Balance is calculated
  int iROI_bottom; // Region Of Interest from which the White Balance is calculated
  int iOperation; // 0: do NOT calculate white balance, apply given relative factors to colour fields
                  // 1: calculate white balance, do NOT apply calculated relative factors to colour fields
                  // 2: calculate white balance, apply calculated relative factors to colour fields
} WhiteBalanceInfo;

// JLP2017: just means _std_call
#ifndef WINAPI
#define __std_call
#endif
// The way the name is decorated depends on the language and how the compiler is instructed
// to make the function available, that is, the calling convention. The standard inter-process
// calling convention for Windows used by DLLs is known as the convention.
// It is defined in Windows header files as WINAPI, which is in turn defined
// using the Win32 declarator __stdcall.
unsigned int AbortAcquisition(void);
unsigned int CancelWait(void);
unsigned int CoolerOFF(void);
unsigned int CoolerON(void);
unsigned int DemosaicImage(WORD* _input, WORD* _red, WORD* _green, WORD* _blue, ColorDemosaicInfo* _info);
unsigned int EnableKeepCleans(int iMode);
unsigned int FreeInternalMemory(void);
unsigned int GetAcquiredData(at_32 * array, unsigned long size);
unsigned int GetAcquiredData16(WORD* array, unsigned long size);
unsigned int GetAcquiredFloatData(float* array, unsigned long size);
unsigned int GetAcquisitionProgress(long* acc, long* series);
unsigned int GetAcquisitionTimings(float* exposure, float* accumulate, float* kinetic);
unsigned int GetAdjustedRingExposureTimes(int _inumTimes, float *_fptimes);
unsigned int GetAllDMAData(at_32* array, unsigned long size);
unsigned int GetAmpDesc(int index, char* name, int len);
unsigned int GetAmpMaxSpeed(int index, float* speed);
unsigned int GetAvailableCameras(long* totalCameras);
unsigned int GetBackground(at_32* array, unsigned long size);
unsigned int GetBitDepth(int channel, int* depth);
unsigned int GetCameraEventStatus(DWORD *cam_status);
unsigned int GetCameraHandle(long cameraIndex, long* cameraHandle);
unsigned int GetCameraInformation(int index, long *information);
unsigned int GetCameraSerialNumber(int* number);
unsigned int GetCapabilities(AndorCapabilities* caps);
unsigned int GetControllerCardModel(char* controllerCardModel);
unsigned int GetCurrentCamera(long* cameraHandle);
unsigned int GetCYMGShift(int * _iXshift, int * _iYshift);
unsigned int GetDDGIOCFrequency(double* frequency);
unsigned int GetDDGIOCNumber(unsigned long* number_pulses);
unsigned int GetDDGIOCPulses(int* pulses);

// DDG Lite functions
unsigned int GetDDGLiteGlobalControlByte(unsigned char * _byte);
unsigned int GetDDGLiteControlByte(AT_DDGLiteChannelId _channel, unsigned char * _byte);
unsigned int GetDDGLiteInitialDelay(AT_DDGLiteChannelId _channel, float * _f_delay);
unsigned int GetDDGLitePulseWidth(AT_DDGLiteChannelId _channel, float * _f_width);
unsigned int GetDDGLiteInterPulseDelay(AT_DDGLiteChannelId _channel, float * _f_delay);
unsigned int GetDDGLitePulsesPerExposure(AT_DDGLiteChannelId _channel, at_u32 * _ui32_pulses);

unsigned int GetDDGPulse(double width, double resolution, double* Delay, double *Width);
unsigned int GetDetector(int* xpixels, int* ypixels);
unsigned int GetDICameraInfo(void *info);
unsigned int GetEMCCDGain(int* gain);
unsigned int GetEMGainRange(int* low, int* high);
unsigned int GetFastestRecommendedVSSpeed(int *index, float* speed);
unsigned int GetFIFOUsage(int* FIFOusage);
unsigned int GetFilterMode(int* mode);
unsigned int GetFKExposureTime(float* time);
unsigned int GetFKVShiftSpeed(int index, int* speed);
unsigned int GetFKVShiftSpeedF(int index, float* speed);
unsigned int GetHardwareVersion(unsigned int* PCB, unsigned int* Decode, unsigned int* dummy1, unsigned int* dummy2, unsigned int* CameraFirmwareVersion, unsigned int* CameraFirmwareBuild);
unsigned int GetHeadModel(char* name);
unsigned int GetHorizontalSpeed(int index, int* speed);
unsigned int GetHSSpeed(int channel, int type, int index, float* speed);
unsigned int GetHVflag(int *bFlag);
unsigned int GetID(int devNum, int* id);
unsigned int GetImageFlip(int* iHFlip, int* iVFlip);
unsigned int GetImageRotate(int* iRotate);
unsigned int GetImages (long first, long last, at_32* array, unsigned long size, long* validfirst, long* validlast);
unsigned int GetImages16 (long first, long last, WORD* array, unsigned long size, long* validfirst, long* validlast);
unsigned int GetImagesPerDMA(unsigned long* images);
unsigned int GetIRQ(int* IRQ);
unsigned int GetKeepCleanTime(float *KeepCleanTime);
unsigned int GetMaximumBinning(int ReadMode, int HorzVert, int* MaxBinning);
unsigned int GetMaximumExposure(float* MaxExp);
unsigned int GetMCPGain(int iNum, int *iGain, float *fPhotoepc);
unsigned int GetMCPGainRange(int * _i_low, int * _i_high);
unsigned int GetMCPVoltage(int *iVoltage);
unsigned int GetMinimumImageLength(int* MinImageLength);
unsigned int GetMostRecentColorImage16 (unsigned long size, int algorithm, WORD* red, WORD* green, WORD* blue);
unsigned int GetMostRecentImage (at_32* array, unsigned long size);
unsigned int GetMostRecentImage16 (WORD* array, unsigned long size);
unsigned int GetMSTimingsData(SYSTEMTIME *TimeOfStart,float *_pfDifferences, int _inoOfimages);
unsigned int GetMSTimingsEnabled(void);
unsigned int GetNewData(at_32* array, unsigned long size);
unsigned int GetNewData16(WORD* array, unsigned long size);
unsigned int GetNewData8(unsigned char* array, unsigned long size);
unsigned int GetNewFloatData(float* array, unsigned long size);
unsigned int GetNumberADChannels(int* channels);
unsigned int GetNumberAmp(int* amp);
unsigned int GetNumberAvailableImages (at_32* first, at_32* last);
unsigned int GetNumberDevices(int* numDevs);
unsigned int GetNumberFKVShiftSpeeds(int* number);
unsigned int GetNumberHorizontalSpeeds(int* number);
unsigned int GetNumberHSSpeeds(int channel, int type, int* speeds);
unsigned int GetNumberNewImages (long* first, long* last);
unsigned int GetNumberPreAmpGains(int* noGains);
unsigned int GetNumberRingExposureTimes(int *_ipnumTimes);
unsigned int GetNumberVerticalSpeeds(int* number);
unsigned int GetNumberVSAmplitudes(int* number);
unsigned int GetNumberVSSpeeds(int* speeds);
unsigned int GetOldestImage (at_32* array, unsigned long size);
unsigned int GetOldestImage16 (WORD* array, unsigned long size);
unsigned int GetPhysicalDMAAddress(unsigned long* Address1, unsigned long* Address2);
unsigned int GetPixelSize(float* xSize, float* ySize);
unsigned int GetPreAmpGain(int index, float* gain);
unsigned int GetReadOutTime(float *ReadoutTime);
unsigned int GetRegisterDump(int* mode);
unsigned int GetRingExposureRange(float *_fpMin, float *_fpMax);
unsigned int GetSizeOfCircularBuffer (long* index);
unsigned int GetSlotBusDeviceFunction(DWORD *dwSlot, DWORD *dwBus, DWORD *dwDevice, DWORD *dwFunction);
unsigned int GetSoftwareVersion(unsigned int* eprom, unsigned int* coffile, unsigned int* vxdrev, unsigned int* vxdver, unsigned int* dllrev, unsigned int* dllver);
unsigned int GetSpoolProgress(long* index);
unsigned int GetStatus(int* status);
unsigned int GetTemperature(int* temperature);
unsigned int GetTemperatureF(float* temperature);
unsigned int GetTemperatureRange(int* mintemp,int* maxtemp);
unsigned int GetTemperatureStatus(float *SensorTemp, float *TargetTemp, float *AmbientTemp, float *CoolerVolts);
unsigned int GetTotalNumberImagesAcquired (long* index);
unsigned int GetVersionInfo(AT_VersionInfoId _id, char * _sz_versionInfo, at_u32 _ui32_bufferLen);
unsigned int GetVerticalSpeed(int index, int* speed);
unsigned int GetVirtualDMAAddress(void** Address1, void** Address2);
unsigned int GetVSSpeed(int index, float* speed);
unsigned int GPIBReceive(int id, short address, char* text, int size);
unsigned int GPIBSend(int id, short address, char* text);
unsigned int I2CBurstRead(BYTE i2cAddress, long nBytes, BYTE* data);
unsigned int I2CBurstWrite(BYTE i2cAddress, long nBytes, BYTE* data);
unsigned int I2CRead(BYTE deviceID, BYTE intAddress, BYTE* pdata);
unsigned int I2CReset(void);
unsigned int I2CWrite(BYTE deviceID, BYTE intAddress, BYTE data);
unsigned int IdAndorDll(void);
unsigned int InAuxPort(int port, int* state);
unsigned int Initialize(char * dir); // read ini file to get head and card
unsigned int InitializeDevice(char * dir);
unsigned int IsCoolerOn(int * _iCoolerStatus);
unsigned int IsInternalMechanicalShutter(int *InternalShutter);
unsigned int IsPreAmpGainAvailable(int channel, int amplifier, int index, int pa, int* status);
unsigned int IsTriggerModeAvailable(int _itriggerMode);
unsigned int Merge(const at_32* array, long nOrder, long nPoint, long nPixel, float* coeff, long fit, long hbin, at_32* output, float* start, float* step);
unsigned int OutAuxPort(int port, int state);
unsigned int PrepareAcquisition(void);
unsigned int SaveAsBmp(char* path, char* palette, long ymin, long ymax);
unsigned int SaveAsCommentedSif(char* path, char* comment);
unsigned int SaveAsEDF(char* _szPath, int _iMode);
unsigned int SaveAsFITS(char* szFileTitle, int type);
unsigned int SaveAsRaw(char* szFileTitle, int type);
unsigned int SaveAsSif(char* path);
unsigned int SaveAsSPC(char* path);
unsigned int SaveAsTiff(char* path, char* palette, int position, int type);
unsigned int SaveAsTiffEx(char* path, char* palette, int position, int type, int _mode);
unsigned int SaveEEPROMToFile(char *cFileName);
unsigned int SaveToClipBoard(char* palette);
unsigned int SelectDevice(int devNum);
unsigned int SendSoftwareTrigger(void);
unsigned int SetAccumulationCycleTime(float time);
unsigned int SetAcqStatusEvent(HANDLE event);
unsigned int SetAcquisitionMode(int mode);
unsigned int SetAcquisitionType(int type);
unsigned int SetADChannel(int channel);
unsigned int SetAdvancedTriggerModeState(int _istate);
unsigned int SetBackground(at_32* array, unsigned long size);
unsigned int SetBaselineClamp(int state);
unsigned int SetBaselineOffset(int offset);
unsigned int SetCameraStatusEnable(DWORD Enable);
unsigned int SetComplexImage(int numAreas, int* areas);
unsigned int SetCoolerMode(int mode);
unsigned int SetCropMode(int active, int cropheight, int reserved);
unsigned int SetCurrentCamera(long cameraHandle);
unsigned int SetCustomTrackHBin(int bin);
unsigned int SetDataType(int type);
unsigned int SetDDGAddress(BYTE t0, BYTE t1, BYTE t2, BYTE tt, BYTE address);
unsigned int SetDDGGain(int gain);
unsigned int SetDDGGateStep(double step);
unsigned int SetDDGInsertionDelay(int state);
unsigned int SetDDGIntelligate(int state);
unsigned int SetDDGIOC(int state);
unsigned int SetDDGIOCFrequency(double frequency);
unsigned int SetDDGIOCNumber(unsigned long number_pulses);

// DDG Lite functions
unsigned int SetDDGLiteGlobalControlByte(unsigned char _byte);
unsigned int SetDDGLiteControlByte(AT_DDGLiteChannelId _channel, unsigned char _byte);
unsigned int SetDDGLiteInitialDelay(AT_DDGLiteChannelId _channel, float _f_delay);
unsigned int SetDDGLitePulseWidth(AT_DDGLiteChannelId _channel, float _f_width);
unsigned int SetDDGLiteInterPulseDelay(AT_DDGLiteChannelId _channel, float _f_delay);
unsigned int SetDDGLitePulsesPerExposure(AT_DDGLiteChannelId _channel, at_u32 _ui32_pulses);

unsigned int SetDDGTimes(double t0, double t1, double t2);
unsigned int SetDDGTriggerMode(int mode);
unsigned int SetDDGVariableGateStep(int mode, double p1, double p2);
unsigned int SetDelayGenerator(int board, short address, int type);
unsigned int SetDMAParameters(int MaxImagesPerDMA, float SecondsPerDMA);
unsigned int SetDriverEvent(HANDLE event);
unsigned int SetEMAdvanced(int state);
unsigned int SetEMCCDGain(int gain);
unsigned int SetEMClockCompensation(int EMClockCompensationFlag);
unsigned int SetEMGainMode(int mode);
unsigned int SetExposureTime(float time);
unsigned int SetFanMode(int mode);
unsigned int SetFastExtTrigger(int mode);
unsigned int SetFastKinetics(int exposedRows, int seriesLength, float time, int mode, int hbin, int vbin);
unsigned int SetFastKineticsEx(int exposedRows, int seriesLength, float time, int mode, int hbin, int vbin, int offset);
unsigned int SetFilterMode(int mode);
unsigned int SetFilterParameters(int width, float sensitivity, int range, float accept, int smooth, int noise);
unsigned int SetFKVShiftSpeed(int index);
unsigned int SetFPDP(int state);
unsigned int SetFrameTransferMode(int mode);
unsigned int SetFullImage(int hbin, int vbin);
unsigned int SetFVBHBin(int bin);
unsigned int SetGain(int gain);
unsigned int SetGate(float delay, float width, float step);
unsigned int SetGateMode(int gatemode);
unsigned int SetHighCapacity(int state);
unsigned int SetHorizontalSpeed(int index);
unsigned int SetHSSpeed(int type, int index);
unsigned int SetImage(int hbin, int vbin, int hstart, int hend, int vstart, int vend);
unsigned int SetImageFlip(int iHFlip, int iVFlip);
unsigned int SetImageRotate(int iRotate);
unsigned int SetIsolatedCropMode (int active, int cropheight, int cropwidth, int vbin, int hbin);
unsigned int SetKineticCycleTime(float time);
unsigned int SetMCPGain(int gain);
unsigned int SetMCPGating(int gating);
unsigned int SetMessageWindow(HWND wnd);
unsigned int SetMultiTrack(int number, int height, int offset,int* bottom,int* gap);
unsigned int SetMultiTrackHBin(int bin);
unsigned int SetMultiTrackHRange(int _iStart, int _iEnd);
unsigned int SetNextAddress(at_32* data, long lowAdd, long highAdd, long len, long physical);
unsigned int SetNextAddress16(at_32* data, long lowAdd, long highAdd, long len, long physical);
unsigned int SetNumberAccumulations(int number);
unsigned int SetNumberKinetics(int number);
unsigned int SetNumberPrescans(int _i_number);
unsigned int SetOutputAmplifier(int type);
unsigned int SetPCIMode(int mode,int value);
unsigned int SetPhotonCounting(int state);
unsigned int SetPhotonCountingThreshold(long min, long max);
unsigned int SetPixelMode(int bitdepth, int colormode);
unsigned int SetPreAmpGain(int index);
unsigned int SetRandomTracks(int numTracks, int* areas);
unsigned int SetReadMode(int mode);
unsigned int SetRegisterDump(int mode);
unsigned int SetRingExposureTimes(int numTimes, float *times);
unsigned int SetSaturationEvent(HANDLE event);
unsigned int SetShutter(int type, int mode, int closingtime, int openingtime);
unsigned int SetShutterEx(int type, int mode, int closingtime, int openingtime, int ext_mode);
unsigned int SetShutters(int type, int mode, int closingtime, int openingtime, int ext_type, int ext_mode, int dummy1, int dummy2);
unsigned int SetSifComment(char* comment);
unsigned int SetSingleTrack(int centre, int height);
unsigned int SetSingleTrackHBin(int bin);
unsigned int SetSpool(int active, int method, char* path, int framebuffersize);
unsigned int SetStorageMode(long mode);
unsigned int SetTemperature(int temperature);
unsigned int SetTriggerMode(int mode);
unsigned int SetUserEvent(HANDLE event);
unsigned int SetUSGenomics(long width, long height);
unsigned int SetVerticalRowBuffer(int rows);
unsigned int SetVerticalSpeed(int index);
unsigned int SetVirtualChip(int state);
unsigned int SetVSAmplitude(int index);
unsigned int SetVSSpeed(int index);
unsigned int ShutDown(void);
unsigned int StartAcquisition(void);
unsigned int UnMapPhysicalAddress(void);
unsigned int WaitForAcquisition(void);
unsigned int WaitForAcquisitionByHandle(long cameraHandle);
unsigned int WaitForAcquisitionByHandleTimeOut(long cameraHandle, int _iTimeOutMs);
unsigned int WaitForAcquisitionTimeOut(int _iTimeOutMs);
unsigned int WhiteBalance(WORD* _wRed, WORD* _wGreen, WORD* _wBlue, float *_fRelR, float *_fRelB, WhiteBalanceInfo *_info);

#define DRV_ERROR_CODES 20001
#define DRV_SUCCESS 20002
#define DRV_VXDNOTINSTALLED 20003
#define DRV_ERROR_SCAN 20004
#define DRV_ERROR_CHECK_SUM 20005
#define DRV_ERROR_FILELOAD 20006
#define DRV_UNKNOWN_FUNCTION 20007
#define DRV_ERROR_VXD_INIT 20008
#define DRV_ERROR_ADDRESS 20009
#define DRV_ERROR_PAGELOCK 20010
#define DRV_ERROR_PAGEUNLOCK 20011
#define DRV_ERROR_BOARDTEST 20012
#define DRV_ERROR_ACK 20013
#define DRV_ERROR_UP_FIFO 20014
#define DRV_ERROR_PATTERN 20015

#define DRV_ACQUISITION_ERRORS 20017
#define DRV_ACQ_BUFFER 20018
#define DRV_ACQ_DOWNFIFO_FULL 20019
#define DRV_PROC_UNKONWN_INSTRUCTION 20020
#define DRV_ILLEGAL_OP_CODE 20021
#define DRV_KINETIC_TIME_NOT_MET 20022
#define DRV_ACCUM_TIME_NOT_MET 20023
#define DRV_NO_NEW_DATA 20024
#define DRV_PCI_DMA_FAIL	20025
#define DRV_SPOOLERROR 20026
#define DRV_SPOOLSETUPERROR 20027
#define DRV_FILESIZELIMITERROR 20028
#define DRV_ERROR_FILESAVE 20029

#define DRV_TEMPERATURE_CODES 20033
#define DRV_TEMPERATURE_OFF 20034
#define DRV_TEMPERATURE_NOT_STABILIZED 20035
#define DRV_TEMPERATURE_STABILIZED 20036
#define DRV_TEMPERATURE_NOT_REACHED 20037
#define DRV_TEMPERATURE_OUT_RANGE 20038
#define DRV_TEMPERATURE_NOT_SUPPORTED 20039
#define DRV_TEMPERATURE_DRIFT 20040

#define DRV_TEMP_CODES 20033
#define DRV_TEMP_OFF 20034
#define DRV_TEMP_NOT_STABILIZED 20035
#define DRV_TEMP_STABILIZED 20036
#define DRV_TEMP_NOT_REACHED 20037
#define DRV_TEMP_OUT_RANGE 20038
#define DRV_TEMP_NOT_SUPPORTED 20039
#define DRV_TEMP_DRIFT 20040

#define DRV_GENERAL_ERRORS 20049
#define DRV_INVALID_AUX 20050
#define DRV_COF_NOTLOADED 20051
#define DRV_FPGAPROG 20052
#define DRV_FLEXERROR 20053
#define DRV_GPIBERROR 20054
#define DRV_EEPROMVERSIONERROR 20055

#define DRV_DATATYPE 20064
#define DRV_DRIVER_ERRORS 20065
#define DRV_P1INVALID 20066
#define DRV_P2INVALID 20067
#define DRV_P3INVALID 20068
#define DRV_P4INVALID 20069
#define DRV_INIERROR 20070
#define DRV_COFERROR 20071
#define DRV_ACQUIRING 20072
#define DRV_IDLE 20073
#define DRV_TEMPCYCLE 20074
#define DRV_NOT_INITIALIZED 20075
#define DRV_P5INVALID 20076
#define DRV_P6INVALID 20077
#define DRV_INVALID_MODE 20078
#define DRV_INVALID_FILTER 20079

#define DRV_I2CERRORS 20080
#define DRV_I2CDEVNOTFOUND 20081
#define DRV_I2CTIMEOUT 20082
#define DRV_P7INVALID 20083
#define DRV_USBERROR 20089
#define DRV_IOCERROR 20090
#define DRV_VRMVERSIONERROR 20091
#define DRV_USB_INTERRUPT_ENDPOINT_ERROR 20093
#define DRV_RANDOM_TRACK_ERROR 20094
#define DRV_INVALID_TRIGGER_MODE 20095
#define DRV_LOAD_FIRMWARE_ERROR 20096
#define DRV_DIVIDE_BY_ZERO_ERROR 20097
#define DRV_INVALID_RINGEXPOSURES 20098

#define DRV_ERROR_NOCAMERA 20990
#define DRV_NOT_SUPPORTED 20991
#define DRV_NOT_AVAILABLE 20992

#define DRV_ERROR_MAP 20115
#define DRV_ERROR_UNMAP 20116
#define DRV_ERROR_MDL 20117
#define DRV_ERROR_UNMDL 20118
#define DRV_ERROR_BUFFSIZE 20119
#define DRV_ERROR_NOHANDLE 20121

#define DRV_GATING_NOT_AVAILABLE 20130
#define DRV_FPGA_VOLTAGE_ERROR 20131

#define DRV_MSTIMINGS_ERROR 20156


#define AC_ACQMODE_SINGLE 1
#define AC_ACQMODE_VIDEO 2
#define AC_ACQMODE_ACCUMULATE 4
#define AC_ACQMODE_KINETIC 8
#define AC_ACQMODE_FRAMETRANSFER 16
#define AC_ACQMODE_FASTKINETICS 32

#define AC_READMODE_FULLIMAGE 1
#define AC_READMODE_SUBIMAGE 2
#define AC_READMODE_SINGLETRACK 4
#define AC_READMODE_FVB 8
#define AC_READMODE_MULTITRACK 16
#define AC_READMODE_RANDOMTRACK 32

#define AC_TRIGGERMODE_INTERNAL 1
#define AC_TRIGGERMODE_EXTERNAL 2
#define AC_TRIGGERMODE_EXTERNAL_FVB_EM 4
#define AC_TRIGGERMODE_CONTINUOUS 8
#define AC_TRIGGERMODE_EXTERNALSTART 16
#define AC_TRIGGERMODE_BULB 32

#define AC_CAMERATYPE_PDA 0
#define AC_CAMERATYPE_IXON 1
#define AC_CAMERATYPE_ICCD 2
#define AC_CAMERATYPE_EMCCD 3
#define AC_CAMERATYPE_CCD 4
#define AC_CAMERATYPE_ISTAR 5
#define AC_CAMERATYPE_VIDEO 6
#define AC_CAMERATYPE_IDUS 7
#define AC_CAMERATYPE_NEWTON 8
#define AC_CAMERATYPE_SURCAM 9
#define AC_CAMERATYPE_USBISTAR 10
#define AC_CAMERATYPE_LUCA 11
#define AC_CAMERATYPE_RESERVED 12
#define AC_CAMERATYPE_IKON 13
#define AC_CAMERATYPE_INGAAS 14
#define AC_CAMERATYPE_IVENT 15
#define AC_CAMERATYPE_UNPROGRAMMED 16

#define AC_PIXELMODE_8BIT  1
#define AC_PIXELMODE_14BIT 2
#define AC_PIXELMODE_16BIT 4
#define AC_PIXELMODE_32BIT 8

#define AC_PIXELMODE_MONO 0
#define AC_PIXELMODE_RGB (1 << 16)
#define AC_PIXELMODE_CMY (2 << 16)

#define AC_SETFUNCTION_VREADOUT 1
#define AC_SETFUNCTION_HREADOUT 2
#define AC_SETFUNCTION_TEMPERATURE 4
#define AC_SETFUNCTION_MCPGAIN    8
#define AC_SETFUNCTION_EMCCDGAIN 16
#define AC_SETFUNCTION_BASELINECLAMP 32
#define AC_SETFUNCTION_VSAMPLITUDE 64
#define AC_SETFUNCTION_HIGHCAPACITY 128
#define AC_SETFUNCTION_BASELINEOFFSET 256
#define AC_SETFUNCTION_PREAMPGAIN 512
#define AC_SETFUNCTION_CROPMODE 1024
#define AC_SETFUNCTION_DMAPARAMETERS 2048

// Deprecated for AC_SETFUNCTION_MCPGAIN
#define AC_SETFUNCTION_GAIN 8
#define AC_SETFUNCTION_ICCDGAIN 8

#define AC_GETFUNCTION_TEMPERATURE 1
#define AC_GETFUNCTION_TARGETTEMPERATURE 2
#define AC_GETFUNCTION_TEMPERATURERANGE 4
#define AC_GETFUNCTION_DETECTORSIZE 8
#define AC_GETFUNCTION_GAIN 16
#define AC_GETFUNCTION_ICCDGAIN 16
#define AC_GETFUNCTION_EMCCDGAIN 32

#define AC_FEATURES_POLLING 1
#define AC_FEATURES_EVENTS 2
#define AC_FEATURES_SPOOLING 4
#define AC_FEATURES_SHUTTER 8
#define AC_FEATURES_SHUTTEREX 16
#define AC_FEATURES_EXTERNAL_I2C 32
#define AC_FEATURES_SATURATIONEVENT 64
#define AC_FEATURES_FANCONTROL 128
#define AC_FEATURES_MIDFANCONTROL 256
#define AC_FEATURES_TEMPERATUREDURINGACQUISITION 512
#define AC_FEATURES_KEEPCLEANCONTROL 1024
#define AC_FEATURES_DDGLITE 0x0800

#define AC_EMGAIN_8BIT 1
#define AC_EMGAIN_12BIT 2
#define AC_EMGAIN_LINEAR12 4
#define AC_EMGAIN_REAL12 8

#ifdef __cplusplus
}
#endif

#endif // EOF ANDOR_WIN32 not defined
#endif
