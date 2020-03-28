/****************************************************************************
* jlp_Atmcd32d_linux.cpp
* Dummy Andor library to replace Windows one
*
* JLP
* Version 28/08/2015
*****************************************************************************/
#include "asp2_typedef.h"  // BYTE
#include "jlp_Atmcd32d_linux.h"


unsigned int AbortAcquisition(void){return(0);}
unsigned int CancelWait(void){return(0);}
unsigned int CoolerOFF(void){return(0);}
unsigned int CoolerON(void){return(0);}
unsigned int DemosaicImage(WORD* _input, WORD* _red, WORD* _green, WORD* _blue, ColorDemosaicInfo* _info){return(0);}
unsigned int EnableKeepCleans(int iMode){return(0);}
unsigned int FreeInternalMemory(void){return(0);}
unsigned int GetAcquiredData(at_32 * array, unsigned long size){return(0);}
unsigned int GetAcquiredData16(WORD* array, unsigned long size){return(0);}
unsigned int GetAcquiredFloatData(float* array, unsigned long size){return(0);}
unsigned int GetAcquisitionProgress(long* acc, long* series){return(0);}
unsigned int GetAcquisitionTimings(float* exposure, float* accumulate, float* kinetic){return(0);}
unsigned int GetAdjustedRingExposureTimes(int _inumTimes, float *_fptimes){return(0);}
unsigned int GetAllDMAData(at_32* array, unsigned long size){return(0);}
unsigned int GetAmpDesc(int index, char* name, int len){return(0);}
unsigned int GetAmpMaxSpeed(int index, float* speed){return(0);}
unsigned int GetAvailableCameras(long* totalCameras){return(0);}
unsigned int GetBackground(at_32* array, unsigned long size){return(0);}
unsigned int GetBitDepth(int channel, int* depth){return(0);}
unsigned int GetCameraEventStatus(DWORD *cam_status){return(0);}
unsigned int GetCameraHandle(long cameraIndex, long* cameraHandle){return(0);}
unsigned int GetCameraInformation(int index, long *information){return(0);}
unsigned int GetCameraSerialNumber(int* number){return(0);}
unsigned int GetCapabilities(AndorCapabilities* caps){return(0);}
unsigned int GetControllerCardModel(char* controllerCardModel){return(0);}
unsigned int GetCurrentCamera(long* cameraHandle){return(0);}
unsigned int GetCYMGShift(int * _iXshift, int * _iYshift){return(0);}
unsigned int GetDDGIOCFrequency(double* frequency){return(0);}
unsigned int GetDDGIOCNumber(unsigned long* number_pulses){return(0);}
unsigned int GetDDGIOCPulses(int* pulses){return(0);}

// DDG Lite functions
unsigned int GetDDGLiteGlobalControlByte(unsigned char * _byte){return(0);}
unsigned int GetDDGLiteControlByte(AT_DDGLiteChannelId _channel, unsigned char * _byte){return(0);}
unsigned int GetDDGLiteInitialDelay(AT_DDGLiteChannelId _channel, float * _f_delay){return(0);}
unsigned int GetDDGLitePulseWidth(AT_DDGLiteChannelId _channel, float * _f_width){return(0);}
unsigned int GetDDGLiteInterPulseDelay(AT_DDGLiteChannelId _channel, float * _f_delay){return(0);}
unsigned int GetDDGLitePulsesPerExposure(AT_DDGLiteChannelId _channel, at_u32 * _ui32_pulses){return(0);}

unsigned int GetDDGPulse(double width, double resolution, double* Delay, double *Width){return(0);}
unsigned int GetDetector(int* xpixels, int* ypixels){return(0);}
unsigned int GetDICameraInfo(void *info){return(0);}
unsigned int GetEMCCDGain(int* gain){return(0);}
unsigned int GetEMGainRange(int* low, int* high){return(0);}
unsigned int GetFastestRecommendedVSSpeed(int *index, float* speed){return(0);}
unsigned int GetFIFOUsage(int* FIFOusage){return(0);}
unsigned int GetFilterMode(int* mode){return(0);}
unsigned int GetFKExposureTime(float* time){return(0);}
unsigned int GetFKVShiftSpeed(int index, int* speed){return(0);}
unsigned int GetFKVShiftSpeedF(int index, float* speed){return(0);}
unsigned int GetHardwareVersion(unsigned int* PCB, unsigned int* Decode, unsigned int* dummy1, unsigned int* dummy2, unsigned int* CameraFirmwareVersion, unsigned int* CameraFirmwareBuild){return(0);}
unsigned int GetHeadModel(char* name){return(0);}
unsigned int GetHorizontalSpeed(int index, int* speed){return(0);}
unsigned int GetHSSpeed(int channel, int type, int index, float* speed){return(0);}
unsigned int GetHVflag(int *bFlag){return(0);}
unsigned int GetID(int devNum, int* id){return(0);}
unsigned int GetImageFlip(int* iHFlip, int* iVFlip){return(0);}
unsigned int GetImageRotate(int* iRotate){return(0);}
unsigned int GetImages (long first, long last, at_32* array, unsigned long size, long* validfirst, long* validlast){return(0);}
unsigned int GetImages16 (long first, long last, WORD* array, unsigned long size, long* validfirst, long* validlast){return(0);}
unsigned int GetImagesPerDMA(unsigned long* images){return(0);}
unsigned int GetIRQ(int* IRQ){return(0);}
unsigned int GetKeepCleanTime(float *KeepCleanTime){return(0);}
unsigned int GetMaximumBinning(int ReadMode, int HorzVert, int* MaxBinning){return(0);}
unsigned int GetMaximumExposure(float* MaxExp){return(0);}
unsigned int GetMCPGain(int iNum, int *iGain, float *fPhotoepc){return(0);}
unsigned int GetMCPGainRange(int * _i_low, int * _i_high){return(0);}
unsigned int GetMCPVoltage(int *iVoltage){return(0);}
unsigned int GetMinimumImageLength(int* MinImageLength){return(0);}
unsigned int GetMostRecentColorImage16 (unsigned long size, int algorithm, WORD* red, WORD* green, WORD* blue){return(0);}
unsigned int GetMostRecentImage (at_32* array, unsigned long size){return(0);}
unsigned int GetMostRecentImage16 (WORD* array, unsigned long size){return(0);}
unsigned int GetMSTimingsData(SYSTEMTIME *TimeOfStart,float *_pfDifferences, int _inoOfimages){return(0);}
unsigned int GetMSTimingsEnabled(void){return(0);}
unsigned int GetNewData(at_32* array, unsigned long size){return(0);}
unsigned int GetNewData16(WORD* array, unsigned long size){return(0);}
unsigned int GetNewData8(unsigned char* array, unsigned long size){return(0);}
unsigned int GetNewFloatData(float* array, unsigned long size){return(0);}
unsigned int GetNumberADChannels(int* channels){return(0);}
unsigned int GetNumberAmp(int* amp){return(0);}
unsigned int GetNumberAvailableImages (at_32* first, at_32* last){return(0);}
unsigned int GetNumberDevices(int* numDevs){return(0);}
unsigned int GetNumberFKVShiftSpeeds(int* number){return(0);}
unsigned int GetNumberHorizontalSpeeds(int* number){return(0);}
unsigned int GetNumberHSSpeeds(int channel, int type, int* speeds){return(0);}
unsigned int GetNumberNewImages (long* first, long* last){return(0);}
unsigned int GetNumberPreAmpGains(int* noGains){return(0);}
unsigned int GetNumberRingExposureTimes(int *_ipnumTimes){return(0);}
unsigned int GetNumberVerticalSpeeds(int* number){return(0);}
unsigned int GetNumberVSAmplitudes(int* number){return(0);}
unsigned int GetNumberVSSpeeds(int* speeds){return(0);}
unsigned int GetOldestImage (at_32* array, unsigned long size){return(0);}
unsigned int GetOldestImage16 (WORD* array, unsigned long size){return(0);}
unsigned int GetPhysicalDMAAddress(unsigned long* Address1, unsigned long* Address2){return(0);}
unsigned int GetPixelSize(float* xSize, float* ySize){return(0);}
unsigned int GetPreAmpGain(int index, float* gain){return(0);}
unsigned int GetReadOutTime(float *ReadoutTime){return(0);}
unsigned int GetRegisterDump(int* mode){return(0);}
unsigned int GetRingExposureRange(float *_fpMin, float *_fpMax){return(0);}
unsigned int GetSizeOfCircularBuffer (long* index){return(0);}
unsigned int GetSlotBusDeviceFunction(DWORD *dwSlot, DWORD *dwBus, DWORD *dwDevice, DWORD *dwFunction){return(0);}
unsigned int GetSoftwareVersion(unsigned int* eprom, unsigned int* coffile, unsigned int* vxdrev, unsigned int* vxdver, unsigned int* dllrev, unsigned int* dllver){return(0);}
unsigned int GetSpoolProgress(long* index){return(0);}
unsigned int GetStatus(int* status){return(0);}
unsigned int GetTemperature(int* temperature){return(0);}
unsigned int GetTemperatureF(float* temperature){return(0);}
unsigned int GetTemperatureRange(int* mintemp,int* maxtemp){return(0);}
unsigned int GetTemperatureStatus(float *SensorTemp, float *TargetTemp, float *AmbientTemp, float *CoolerVolts){return(0);}
unsigned int GetTotalNumberImagesAcquired (long* index){return(0);}
unsigned int GetVersionInfo(AT_VersionInfoId _id, char * _sz_versionInfo, at_u32 _ui32_bufferLen){return(0);}
unsigned int GetVerticalSpeed(int index, int* speed){return(0);}
unsigned int GetVirtualDMAAddress(void** Address1, void** Address2){return(0);}
unsigned int GetVSSpeed(int index, float* speed){return(0);}
unsigned int GPIBReceive(int id, short address, char* text, int size){return(0);}
unsigned int GPIBSend(int id, short address, char* text){return(0);}
unsigned int I2CBurstRead(BYTE i2cAddress, long nBytes, BYTE* data){return(0);}
unsigned int I2CBurstWrite(BYTE i2cAddress, long nBytes, BYTE* data){return(0);}
unsigned int I2CRead(BYTE deviceID, BYTE intAddress, BYTE* pdata){return(0);}
unsigned int I2CReset(void){return(0);}
unsigned int I2CWrite(BYTE deviceID, BYTE intAddress, BYTE data){return(0);}
unsigned int IdAndorDll(void){return(0);}
unsigned int InAuxPort(int port, int* state){return(0);}
unsigned int Initialize(char * dir){return(0);} // read ini file to get head and card
unsigned int InitializeDevice(char * dir){return(0);}
unsigned int IsCoolerOn(int * _iCoolerStatus){return(0);}
unsigned int IsInternalMechanicalShutter(int *InternalShutter){return(0);}
unsigned int IsPreAmpGainAvailable(int channel, int amplifier, int index, int pa, int* status){return(0);}
unsigned int IsTriggerModeAvailable(int _itriggerMode){return(0);}
unsigned int Merge(const at_32* array, long nOrder, long nPoint, long nPixel, float* coeff, long fit, long hbin, at_32* output, float* start, float* step){return(0);}
unsigned int OutAuxPort(int port, int state){return(0);}
unsigned int PrepareAcquisition(void){return(0);}
unsigned int SaveAsBmp(char* path, char* palette, long ymin, long ymax){return(0);}
unsigned int SaveAsCommentedSif(char* path, char* comment){return(0);}
unsigned int SaveAsEDF(char* _szPath, int _iMode){return(0);}
unsigned int SaveAsFITS(char* szFileTitle, int type){return(0);}
unsigned int SaveAsRaw(char* szFileTitle, int type){return(0);}
unsigned int SaveAsSif(char* path){return(0);}
unsigned int SaveAsSPC(char* path){return(0);}
unsigned int SaveAsTiff(char* path, char* palette, int position, int type){return(0);}
unsigned int SaveAsTiffEx(char* path, char* palette, int position, int type, int _mode){return(0);}
unsigned int SaveEEPROMToFile(char *cFileName){return(0);}
unsigned int SaveToClipBoard(char* palette){return(0);}
unsigned int SelectDevice(int devNum){return(0);}
unsigned int SendSoftwareTrigger(void){return(0);}
unsigned int SetAccumulationCycleTime(float time){return(0);}
unsigned int SetAcqStatusEvent(HANDLE event){return(0);}
unsigned int SetAcquisitionMode(int mode){return(0);}
unsigned int SetAcquisitionType(int type){return(0);}
unsigned int SetADChannel(int channel){return(0);}
unsigned int SetAdvancedTriggerModeState(int _istate){return(0);}
unsigned int SetBackground(at_32* array, unsigned long size){return(0);}
unsigned int SetBaselineClamp(int state){return(0);}
unsigned int SetBaselineOffset(int offset){return(0);}
unsigned int SetCameraStatusEnable(DWORD Enable){return(0);}
unsigned int SetComplexImage(int numAreas, int* areas){return(0);}
unsigned int SetCoolerMode(int mode){return(0);}
unsigned int SetCropMode(int active, int cropheight, int reserved){return(0);}
unsigned int SetCurrentCamera(long cameraHandle){return(0);}
unsigned int SetCustomTrackHBin(int bin){return(0);}
unsigned int SetDataType(int type){return(0);}
unsigned int SetDDGAddress(BYTE t0, BYTE t1, BYTE t2, BYTE tt, BYTE address){return(0);}
unsigned int SetDDGGain(int gain){return(0);}
unsigned int SetDDGGateStep(double step){return(0);}
unsigned int SetDDGInsertionDelay(int state){return(0);}
unsigned int SetDDGIntelligate(int state){return(0);}
unsigned int SetDDGIOC(int state){return(0);}
unsigned int SetDDGIOCFrequency(double frequency){return(0);}
unsigned int SetDDGIOCNumber(unsigned long number_pulses){return(0);}

// DDG Lite functions
unsigned int SetDDGLiteGlobalControlByte(unsigned char _byte){return(0);}
unsigned int SetDDGLiteControlByte(AT_DDGLiteChannelId _channel, unsigned char _byte){return(0);}
unsigned int SetDDGLiteInitialDelay(AT_DDGLiteChannelId _channel, float _f_delay){return(0);}
unsigned int SetDDGLitePulseWidth(AT_DDGLiteChannelId _channel, float _f_width){return(0);}
unsigned int SetDDGLiteInterPulseDelay(AT_DDGLiteChannelId _channel, float _f_delay){return(0);}
unsigned int SetDDGLitePulsesPerExposure(AT_DDGLiteChannelId _channel, at_u32 _ui32_pulses){return(0);}

unsigned int SetDDGTimes(double t0, double t1, double t2){return(0);}
unsigned int SetDDGTriggerMode(int mode){return(0);}
unsigned int SetDDGVariableGateStep(int mode, double p1, double p2){return(0);}
unsigned int SetDelayGenerator(int board, short address, int type){return(0);}
unsigned int SetDMAParameters(int MaxImagesPerDMA, float SecondsPerDMA){return(0);}
unsigned int SetDriverEvent(HANDLE event){return(0);}
unsigned int SetEMAdvanced(int state){return(0);}
unsigned int SetEMCCDGain(int gain){return(0);}
unsigned int SetEMClockCompensation(int EMClockCompensationFlag){return(0);}
unsigned int SetEMGainMode(int mode){return(0);}
unsigned int SetExposureTime(float time){return(0);}
unsigned int SetFanMode(int mode){return(0);}
unsigned int SetFastExtTrigger(int mode){return(0);}
unsigned int SetFastKinetics(int exposedRows, int seriesLength, float time, int mode, int hbin, int vbin){return(0);}
unsigned int SetFastKineticsEx(int exposedRows, int seriesLength, float time, int mode, int hbin, int vbin, int offset){return(0);}
unsigned int SetFilterMode(int mode){return(0);}
unsigned int SetFilterParameters(int width, float sensitivity, int range, float accept, int smooth, int noise){return(0);}
unsigned int SetFKVShiftSpeed(int index){return(0);}
unsigned int SetFPDP(int state){return(0);}
unsigned int SetFrameTransferMode(int mode){return(0);}
unsigned int SetFullImage(int hbin, int vbin){return(0);}
unsigned int SetFVBHBin(int bin){return(0);}
unsigned int SetGain(int gain){return(0);}
unsigned int SetGate(float delay, float width, float step){return(0);}
unsigned int SetGateMode(int gatemode){return(0);}
unsigned int SetHighCapacity(int state){return(0);}
unsigned int SetHorizontalSpeed(int index){return(0);}
unsigned int SetHSSpeed(int type, int index){return(0);}
unsigned int SetImage(int hbin, int vbin, int hstart, int hend, int vstart, int vend){return(0);}
unsigned int SetImageFlip(int iHFlip, int iVFlip){return(0);}
unsigned int SetImageRotate(int iRotate){return(0);}
unsigned int SetIsolatedCropMode (int active, int cropheight, int cropwidth, int vbin, int hbin){return(0);}
unsigned int SetKineticCycleTime(float time){return(0);}
unsigned int SetMCPGain(int gain){return(0);}
unsigned int SetMCPGating(int gating){return(0);}
unsigned int SetMessageWindow(HWND wnd){return(0);}
unsigned int SetMultiTrack(int number, int height, int offset,int* bottom,int* gap){return(0);}
unsigned int SetMultiTrackHBin(int bin){return(0);}
unsigned int SetMultiTrackHRange(int _iStart, int _iEnd){return(0);}
unsigned int SetNextAddress(at_32* data, long lowAdd, long highAdd, long len, long physical){return(0);}
unsigned int SetNextAddress16(at_32* data, long lowAdd, long highAdd, long len, long physical){return(0);}
unsigned int SetNumberAccumulations(int number){return(0);}
unsigned int SetNumberKinetics(int number){return(0);}
unsigned int SetNumberPrescans(int _i_number){return(0);}
unsigned int SetOutputAmplifier(int type){return(0);}
unsigned int SetPCIMode(int mode,int value){return(0);}
unsigned int SetPhotonCounting(int state){return(0);}
unsigned int SetPhotonCountingThreshold(long min, long max){return(0);}
unsigned int SetPixelMode(int bitdepth, int colormode){return(0);}
unsigned int SetPreAmpGain(int index){return(0);}
unsigned int SetRandomTracks(int numTracks, int* areas){return(0);}
unsigned int SetReadMode(int mode){return(0);}
unsigned int SetRegisterDump(int mode){return(0);}
unsigned int SetRingExposureTimes(int numTimes, float *times){return(0);}
unsigned int SetSaturationEvent(HANDLE event){return(0);}
unsigned int SetShutter(int type, int mode, int closingtime, int openingtime){return(0);}
unsigned int SetShutterEx(int type, int mode, int closingtime, int openingtime, int ext_mode){return(0);}
unsigned int SetShutters(int type, int mode, int closingtime, int openingtime, int ext_type, int ext_mode, int dummy1, int dummy2){return(0);}
unsigned int SetSifComment(char* comment){return(0);}
unsigned int SetSingleTrack(int centre, int height){return(0);}
unsigned int SetSingleTrackHBin(int bin){return(0);}
unsigned int SetSpool(int active, int method, char* path, int framebuffersize){return(0);}
unsigned int SetStorageMode(long mode){return(0);}
unsigned int SetTemperature(int temperature){return(0);}
unsigned int SetTriggerMode(int mode){return(0);}
unsigned int SetUserEvent(HANDLE event){return(0);}
unsigned int SetUSGenomics(long width, long height){return(0);}
unsigned int SetVerticalRowBuffer(int rows){return(0);}
unsigned int SetVerticalSpeed(int index){return(0);}
unsigned int SetVirtualChip(int state){return(0);}
unsigned int SetVSAmplitude(int index){return(0);}
unsigned int SetVSSpeed(int index){return(0);}
unsigned int ShutDown(void){return(0);}
unsigned int StartAcquisition(void){return(0);}
unsigned int UnMapPhysicalAddress(void){return(0);}
unsigned int WaitForAcquisition(void){return(0);}
unsigned int WaitForAcquisitionByHandle(long cameraHandle){return(0);}
unsigned int WaitForAcquisitionByHandleTimeOut(long cameraHandle, int _iTimeOutMs){return(0);}
unsigned int WaitForAcquisitionTimeOut(int _iTimeOutMs){return(0);}
unsigned int WhiteBalance(WORD* _wRed, WORD* _wGreen, WORD* _wBlue, float *_fRelR, float *_fRelB, WhiteBalanceInfo *_info){return(0);}
