/************************************************************************
* "jlp_raptor_cam1_serial.cpp"
* JLP_RaptorCam1 class
*
* JLP
* Version 17/09/2015
*************************************************************************/
#include "jlp_raptor_cam1.h"
#include "jlp_raptor_serial.h"

#include <time.h> // clock_t

// Contained here:
void mySleep(double nTime_ms);
double myClock();

/*************************************************************************
*
*************************************************************************/
int JLP_RaptorCam1::serialWriteReadCmd(int unitopenmap, int unit,
                                       unsigned char* bufin, int insize, unsigned char* bufout,
                                       int outsize, bool bChkSum )
{
/*
	if(g_SerialOK == false)
		return -100;

	bChkSum &= g_bCheckSum;

//	MMThreadGuard g(g_serialLock_[unitopenmap]);
	int nBigLoop=0, ret=0;
	do
	{
		nBigLoop++;

		int ii;
		if(pxd_serialRead( unit, 0, NULL, 0)>0)
		{
			ret = pxd_serialRead( unit, 0, (char*)bufout, outsize);
			if(fidSerial[unitopenmap] && gUseSerialLog[unitopenmap])
			{
				gClockCurrent[unitopenmap] = myClock();
				fprintf(fidSerial[unitopenmap], "Time: %0.8f\t", double(gClockCurrent[unitopenmap] - gClockZero[unitopenmap]) );

				fprintf(fidSerial[unitopenmap], "FB: ");
				for(ii=0; ii<ret; ii++)
					fprintf(fidSerial[unitopenmap], "0x%02X ",bufout[ii]);
				fprintf(fidSerial[unitopenmap], "\n");
			}
		}
		int tick = 0;
		while(pxd_serialWrite(unit, 0, NULL, 0)<insize && tick<100)
		{
			tick++;
			mySleep(1);
		}
		if(tick==100)
			tick=100;

		insize = insize < 256 ? insize : 255;
		unsigned char ucChk = 0;
		for(ii=0;ii<insize;ii++)
		{
			g_ucSerialBuf[ii] = bufin[ii] ;
			ucChk ^= bufin[ii];
		}

		int insize2;
		insize2 = insize;
		if(bChkSum)
		{
			g_ucSerialBuf[insize] = ucChk;
			insize2++;
		}

		//ret = pxd_serialWrite(unit, 0, (char*)bufin, insize);
		ret = pxd_serialWrite(unit, 0, (char*)g_ucSerialBuf, insize2);
		if(ret<0)
			ret = pxd_serialWrite(unit, 0, (char*)g_ucSerialBuf, insize2);

		if(fidSerial[unitopenmap] && gUseSerialLog[unitopenmap])
		{
			gClockCurrent[unitopenmap] = myClock();
			fprintf(fidSerial[unitopenmap], "Time: %0.8f \t", double(gClockCurrent[unitopenmap] - gClockZero[unitopenmap]));

			fprintf(fidSerial[unitopenmap], "TX: ");
			for(ii=0; ii<insize2; ii++)
				fprintf(fidSerial[unitopenmap], "0x%02X ",g_ucSerialBuf[ii]);
			fprintf(fidSerial[unitopenmap], "\n");
		}
		tick=0;
		while(pxd_serialRead( unit, 0, NULL, 0)==0 && tick<100)
		{
			tick++;
			mySleep(1);
		}
		if(tick==100 )
		{
			tick=0;
		}
		int bytes, morebytes;
		bytes = pxd_serialRead( unit, 0, (char*)bufout, outsize);
		mySleep(1);
		morebytes = pxd_serialRead( unit, 0, NULL, 0);

		tick=0;
		if(bytes==0 || morebytes>0)
		{
			while((bytes<1 || (bufout[bytes-2] != 0x50 && bufout[bytes-1] != 0x50) || morebytes>0) && tick<100)
			{
				bytes += pxd_serialRead( unit, 0, (char*)&bufout[bytes], outsize-bytes);
				mySleep(1);
				morebytes = pxd_serialRead( unit, 0, NULL, 0);
				tick++;
			}
		}
		ret = bytes;

		if( (bChkSum && bytes==2) && bufout[0]>0x50 && bufout[0]<=0x55  )
		{
			gClockCurrent[unitopenmap] = myClock();
			fprintf(fidSerial[unitopenmap], "Time: %0.8f \t", double(gClockCurrent[unitopenmap] - gClockZero[unitopenmap]) );
			fprintf(fidSerial[unitopenmap], "CMD ERROR: 0x%02X\n",bufout[0]);
			ret = 0x50 - bufout[0];
		}
		else if( (!bChkSum && bytes>=1)  && bufout[bytes-1]>0x50 && bufout[bytes-1]<=0x55)
		{
			gClockCurrent[unitopenmap] = myClock();
			fprintf(fidSerial[unitopenmap], "Time: %0.8f \t", double(gClockCurrent[unitopenmap] - gClockZero[unitopenmap]) );
			fprintf(fidSerial[unitopenmap], "CMD ERROR: 0x%02X\n",bufout[bytes-1]);
			ret = 0x50 - bufout[bytes-1];
		}
		else if(bChkSum && (bytes>1 && bufout[bytes-1]!=ucChk && bufout[bytes-1]!=0x50))
		{
			gClockCurrent[unitopenmap] = myClock();
			fprintf(fidSerial[unitopenmap], "Time: %0.8f \t", double(gClockCurrent[unitopenmap] - gClockZero[unitopenmap]) );
			fprintf(fidSerial[unitopenmap], "CHKSUM ERROR: 0x%02X, 0x%02X\n",bufout[bytes-1], ucChk);
		}
		if(bChkSum && bytes<2)
		{
			gClockCurrent[unitopenmap] = myClock();
			fprintf(fidSerial[unitopenmap], "Time: %0.8f \t", double(gClockCurrent[unitopenmap] - gClockZero[unitopenmap]) );
			fprintf(fidSerial[unitopenmap], "RESPONSE ERROR: \n");
		}

		if(bChkSum && bytes>0 && bufout[bytes-2]==0x50)
			ret = bytes-1;

		if(fidSerial[unitopenmap] && gUseSerialLog[unitopenmap])
		{
			gClockCurrent[unitopenmap] = myClock();
			fprintf(fidSerial[unitopenmap], "Time: %0.8f \t", double(gClockCurrent[unitopenmap] - gClockZero[unitopenmap]) );
			fprintf(fidSerial[unitopenmap], "RX: ");
			for(ii=0; ii<bytes; ii++)
				fprintf(fidSerial[unitopenmap], "0x%02X ",bufout[ii]);
			fprintf(fidSerial[unitopenmap], "\n");
		}
		if(nBigLoop>1)
			Sleep(100);
	}
	while(ret<0 && nBigLoop<3);

	return ret;
*/
return(0);
}
/**************************************************************************
*
***************************************************************************/
void mySleep(double nTime_ms)
{
#if defined (MMLINUX32) || defined(MMLINUX64)
	Sleep(nTime_ms);
#else

	if(nTime_ms<0.0 )
		return;

	if(nTime_ms!=nTime_ms)
		return;

	double clock_current, clock_start, dSleep;

	clock_start = myClock();

	if(nTime_ms>=200)
	{
#if defined (WIN32) || defined(WIN64)
		Sleep((DWORD)(nTime_ms-100));
#else
		Sleep((int)(nTime_ms-100));
#endif
	}


	clock_current = myClock();

	dSleep = (nTime_ms)/1000.0f;

	while(dSleep > (clock_current - clock_start))
	{
		clock_current = myClock();
	}
	dSleep = clock_current - clock_start;
#endif
}

/**************************************************************************
*
***************************************************************************/
double myClock()
{
#if defined (WIN32) || defined(WIN64)
	bool bAccurate=true;
	double dTime;

	static LARGE_INTEGER m_ticksPerSecond = {0};
	static LARGE_INTEGER m_ticksPerSecondOld = {0};
	static double dLastQPFUpdate = 0.0;
	static double dStartTime = 0.0;
	static clock_t clockStart;
	clock_t clockCurrent;
	LARGE_INTEGER tick;   // A point in time

	// get the high resolution counter's accuracy
	if(m_ticksPerSecond.QuadPart==0)
	{
		QueryPerformanceFrequency(&m_ticksPerSecond);
	}
	// what time is it?

	if(bAccurate)
	{
		QueryPerformanceCounter(&tick);
		dTime = double(tick.QuadPart)/double(m_ticksPerSecond.QuadPart);
	}
	else
	{
		clockCurrent = clock();
		dTime = double(clockCurrent)/double(CLOCKS_PER_SEC);
	}

	return dTime;
#else
	double dTime;

	struct timeval start;

        long seconds, useconds;

        gettimeofday(&start, NULL);

        dTime = (double)start.tv_sec;
	dTime += ((double)start.tv_usec)/1.0e6;

	return dTime;

#endif
}
/***************************************************************************
* Send command to serial port
*
* INPUT:
* char data_send0[64]
****************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SerialSend(unsigned char *data_send0, int nsend0)
{
int status, nsend2, i;
unsigned char data_send2[128], check_sum_byte2;

// To avoid saturating the port: (milliseonds)
Sleep(200);

nsend2 = nsend0;
for(i = 0; i < nsend2; i++) data_send2[i] = data_send0[i];

if(check_sum1 == true) {
   check_sum_byte2 = 0x0;
// XOR
   for(i = 0; i < nsend2; i++) check_sum_byte2 ^= data_send2[i];
   data_send2[nsend2] = check_sum_byte2;
   nsend2++;
}

// Not working:
//status = serialWriteReadCmd(UNITSOPENMAP, 1 , data_send0, nsend0, data_read0, expected_nread0, true);

// Clear serial port
  JLP_Raptor_SerialClearPort();

//
// Format command and send.
// Here we use an example of an ASCII style, \n terminated, command.
// If the camera used binary commands, we would likely construct the
// databuffer byte by byte instead of using sprintf, and we
// would know the length a priori instead of using strlen().
// sprintf(databuffer, "Set Gain=%f\n", 12.345);
    status = pxd_serialWrite(0x1, 0, (char *)data_send2, nsend2);
    if (status < 0) {
    wxMessageBox(wxT("Error sending command to EPIX board"),
                 wxT("Raptor_SerialSend"), wxICON_ERROR);
    }

// DEBUG:
// serial_log_command(data_send2, nsend2, data_send0, 0, "");

return(status);
}
/*******************************************************************************
* Clear serial port
********************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SerialClearPort()
{
int unit = 0x1, tick, insize = 2, outsize = 2, ret;
unsigned char bufout[256];

   if(pxd_serialRead( unit, 0, NULL, 0) > 0) {
			ret = pxd_serialRead( unit, 0, (char*)bufout, outsize);
    }
    tick = 0;
    while((pxd_serialWrite(unit, 0, NULL, 0) < insize) && (tick < 100))
		{
			tick++;
			mySleep(1);
		}

return(0);
}
/***************************************************************************
* Send command to serial port
*
* INPUT:
* char data_send0[64]
****************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SerialSendReceive(unsigned char *data_send0,
                                                 unsigned char *data_read0,
                                                 const int nsend0,
                                                 const int expected_nread0,
                                                 int *nread0)
{
int status;

// Send command:
  status = JLP_Raptor_SerialSend(data_send0, nsend0);

  if(status == 0)
   status = JLP_Raptor_SerialReceive_0(data_read0, expected_nread0, nread0);
  else
    wxMessageBox(wxT("JLP_Raptor_SerialSend/Error sending command to EPIX board"),
                 wxT("JLP_Raptor_SerialSendReceive"), wxICON_ERROR);

// Equivalent:
// status = JLP_Raptor_SerialReceive_1(data_read0, expected_nread0, nread0);

return(status);
}
/*******************************************************************************
* This example reads the response one character at a time for sake of clarity,
* not necessity...
********************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SerialReceive_0(unsigned char *data_read0,
                                         int expected_nread0, int *nread0)
{
int     isafe, status = -1, expected_nread2;
char    c0;
DWORD   time;

// If checksum is enabled, the answer contains the echo of the (sent command) checksum at the end
if(check_sum1 == true)
    expected_nread2 = expected_nread0 + 1;
else
    expected_nread2 = expected_nread0;

//
// Wait for and read response. Don't wait forever, in
// case the camera is not connected or not powered on.
//
    *nread0 = 0;
    time = GetTickCount();  // Windows: current time in milliseconds
// This example reads the response one character at a time for sake of clarity,
// not necessity...
    for (isafe = 0; isafe < 1000; isafe++) {
//
// Another character arrived?
//
        status = pxd_serialRead(0x1, 0, &c0, 1);
        if (status < 0)
            return(-1);     // error
        if (status == 0) {
            Sleep(1);   // be nice to multitasking OS
            continue;   // nothing arrived since last check
        }
//
// Store next character, building up response
//
    data_read0[(*nread0)++] = c0;
//
// Have we read a complete response? The test is camera dependent!
// Here we assume an ASCII response, terminated with a \n .
//
            if((c0 == '\n') || (*nread0 == expected_nread2)) {
                status = 0;
                break;
            }
            //
            // Have we waited long enough to give up?
            //
            if (GetTickCount() - time > 100)
                return(-2);     // error: timeout
        } // EOF infinite loop


//
// Check the response. This test is camera dependent!
//
// ACK : positive acknowledge seen
// NAK : negative acknowledge seen

    if((strstr((char *)data_read0, "ACK"))
       || (strstr((char *)data_read0, "NAK")))
        status = 0;

return(status);
}
/*******************************************************************************
* Wait for and read response. Don't wait forever, in
* case the camera is not connected or not powered on.
* Simpler version than JLP_Raptor_SerialSendReceive__0,
* but do the same (as tested in november 2017):
********************************************************************************/
int JLP_RaptorCam1::JLP_Raptor_SerialReceive_1(unsigned char *data_read0,
                                                   int expected_nread0,
                                                   int *nread0)
{
int     isafe, status = -1, nread1;
unsigned char    *buf_read;
DWORD   time;

//
// Wait for and read response. Don't wait forever, in
// case the camera is not connected or not powered on.
//
    *nread0 = 0;
    time = GetTickCount();  // Windows: current time in milliseconds
// This example reads the response one character at a time for sake of clarity,
// not necessity...
    for (isafe = 0; isafe < 10000; isafe++) {
//
// Read up to as many characters as will fit in the remainder
// of the data buffer.
//
            buf_read = &data_read0[*nread0];
            nread1 = pxd_serialRead(0x1, 0, (char *)buf_read, expected_nread0 - *nread0);
            if (nread1 < 0)
                return(-1);     // error
            if (nread1 == 0) {
                Sleep(1);   // be nice to multitasking OS
                continue;   // nothing arrived since last check
            }
            *nread0 += nread1;
//
// Have we read a complete response? The test is camera dependent!
// Here we assume an ASCII response, terminated with a \n .
//
            if((data_read0[*nread0 - 1] == '\n') || (*nread0 >= expected_nread0)) {
                status = 0;
                break;
            }

//
// Have we waited long enough to give up?
//
            if (GetTickCount() - time > 1000)
                return(-2);     // error: timeout
        } // EOF infinite loop

//
// Check the response. This test is camera dependent!
//
// ACK : positive acknowledge seen
// NAK : negative acknowledge seen

    if((strstr((char *)data_read0, "ACK"))
       || (strstr((char *)data_read0, "NAK")))
        status = 0;


return(status);
}
/***
The above example reads the response one character at a time for sake of clarity,
not necessity. The first half of the ''for'' loop can be replaced with:

        for (;;) {
            //
            // Read up to as many characters as will fit in the remainder
            // of the data buffer.
            //
            r = pxd_serialRead(0x1, 0, &databuffer[dataread], sizeof(databuffer)-dataread);
            if (r < 0)
                return;     // error
            if (r == 0) {
                Sleep(1);   // be nice to multitasking OS
                continue;   // nothing arrived since last check
            }
            dataread += r;
            //
            // Have we read a complete response?
            //
            if (databuffer[dataread-1] == '\n')
                break;
            ...
        }

*****/
/***************************************************************************
* Read register from port
*
****************************************************************************/
int JLP_RaptorCam1::serialReadRaptorRegister1(int unit, unsigned char nReg,
                                              unsigned char* val )
{
/*
	if(!g_SerialOK) return -1;

	int ret;
	unsigned char bufin[]  = {0x53, 0xE0, 0x01, 0xFF, 0x50};
	unsigned char bufin2[] = {0x53, 0xE1, 0x01, 0x50};
	unsigned char buf[256];

	int pos=0;
	bufin[3] = nReg;
	ret = serialWriteReadCmd(UNITSOPENMAP, unit, bufin,  5, buf, 256 );
	ret = serialWriteReadCmd(UNITSOPENMAP, unit, bufin2, 4, buf, 256 );

	if(ret>0)
		pos = ret;
	else if(ret<0)
		pxd_mesgFault(UNITSMAP);

	if(pos>1 && buf[pos-1]==0x50)
	{
		*val = buf[pos-2];
		return pos;
	}
	else
		return -ret;
*/
return(0);
}
/***************************************************************************
* Read register from port
*
****************************************************************************/
int JLP_RaptorCam1::serialReadRaptorRegister2(int unit, unsigned char nReg,
                                              unsigned char* val )
{
/*
	if(!g_SerialOK) return -1;

	int ret;
	unsigned char bufin[]  = {0x53, 0xE0, 0x02, 0xFF, 0x00, 0x50};
	unsigned char bufin2[] = {0x53, 0xE1, 0x01, 0x50};
	unsigned char buf[256];

	int pos=0;
	bufin[3] = nReg;
	ret = serialWriteReadCmd(UNITSOPENMAP, unit, bufin,  6, buf, 256 );
	ret = serialWriteReadCmd(UNITSOPENMAP, unit, bufin2, 4, buf, 256 );


	if(ret>0)
		pos = ret;
	else if(ret<0)
		pxd_mesgFault(UNITSMAP);

	if(pos>1 && buf[pos-1]==0x50)
	{
		*val = buf[pos-2];
		return pos;
	}
	else
		return -ret;
*/
return(0);
}
/**************************************************************************************
*
**************************************************************************************/
int JLP_RaptorCam1::serial_log_command(unsigned char *data_send0, int nsend0,
                                       unsigned char *data_read0, int nread0,
                                       char *comments)
{ // Print for debug:
int i, nsend2;
unsigned char data_send2[128], check_sum_byte2;
char buffer_read0[128], buffer_send0[128], *pc;
wxString buf0, buf1;

// Add check sum if needed:
nsend2 = nsend0;
for(i = 0; i < nsend2; i++) data_send2[i] = data_send0[i];

if(check_sum1 == true) {
   check_sum_byte2 = 0x0;
// XOR
   for(i = 0; i < nsend2; i++) check_sum_byte2 ^= data_send2[i];
   data_send2[nsend2] = check_sum_byte2;
   nsend2++;
}

// Print sended command:
    pc = buffer_send0;
    for(i = 0; i < nsend2; i++) {
      sprintf(pc, " %02X", data_send2[i]);
      pc += 3;
    }
    pc++;
    *pc = '\0';

    pc = buffer_read0;
    for(i = 0; i < nread0; i++) {
      sprintf(pc, " %02X", data_read0[i]);
      pc += 3;
    }
    pc++;
    *pc = '\0';

    buf0.Printf(wxT("%s Command: >%s< (nbytes=%d) \n Answer : >%s< (nbytes=%d)"),
                comments, buffer_send0, nsend0, buffer_read0, nread0);
    buf1.Printf(wxT(" %s"), comments);
    wxMessageBox(buf0, wxT("log_command"), wxOK);

return(0);
}
