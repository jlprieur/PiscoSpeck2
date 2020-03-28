/************************************************************************
* "jlp_raptor_cam1_tools.cpp"
* JLP_RaptorCam1 class
*
* JLP
* Version 17/09/2015
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "jlp_raptor_cam1.h"      // RAPTOR_SETTINGS, ...
#include "jlp_raptor_serial.h" // ReadSerial

/************************************************************************
*
************************************************************************/
int JLP_RaptorCam1::DisableMicro()
{
 return 0;
}
/************************************************************************
*
************************************************************************/
int JLP_RaptorCam1::SetSystemState(unsigned char nState)
{
/*
	unsigned char bufin[] = {0x4F, 0x00, 0x50, 0x00};
	unsigned char buf[256];
	if(g_bCheckSum)
		bufin[1] = nState | 0x40;
	else
		bufin[1] = nState ;

	bufin[3] = bufin[0] ^ bufin[1] ^ bufin[2] ;

	return serialWriteReadCmd(UNITSOPENMAP, UNITMASK, bufin,  4, buf, 256);
*/
return(0);
}
/************************************************************************
*
************************************************************************/
unsigned char JLP_RaptorCam1::GetSystemState()
{
/*
	unsigned char bufin[] = {0x49, 0x50};
	unsigned char buf[256];
	unsigned char nState;

	int ret = serialWriteReadCmd(UNITSOPENMAP, UNITMASK, bufin,  2, buf, 256);

	nState = 0xFF;

	if(ret>1)
		nState = buf[ret-2];

	if(g_bCheckSum)
		nState &= 0xBF;

	return nState;
*/
return(0);
}
/************************************************************************
*
************************************************************************/
int JLP_RaptorCam1::ConvertTECTempToValue(double dTemp)
{
	int nValue=-1;

	if(EPROM_DAC_Cal_0C > 0 && EPROM_DAC_Cal_40C > 0)
	{
		double m, c;
		m = ((double(EPROM_DAC_Cal_40C) - double(EPROM_DAC_Cal_0C))) / 40.0;
		c = EPROM_DAC_Cal_0C;
		nValue = (unsigned int)(dTemp*m + c);
	}
	return nValue;
}

/************************************************************************
*
************************************************************************/
double JLP_RaptorCam1::ConvertTECValueToTemp(int nValue)
{
	double dValue = -999;
	if(EPROM_DAC_Cal_0C > 0 && EPROM_DAC_Cal_40C > 0)
	{
		double m, c;
		if(EPROM_DAC_Cal_40C - EPROM_DAC_Cal_0C == 0)
			m = 0.0;
		else
			m = 40.0 / ((double(EPROM_DAC_Cal_40C) - double(EPROM_DAC_Cal_0C)));
		c = - m*double(EPROM_DAC_Cal_0C);
		dValue = m*double(nValue) + c;
		dValue = floor(dValue*10 + 0.5) / 10.0;
	}
	return dValue;
}

/************************************************************************
*
************************************************************************/
double JLP_RaptorCam1::GetTECSetPoint()
{
	double dValue=-999.0;

		unsigned int nValue=0;
		unsigned char v1=0, v2=0;

		serialReadRaptorRegister1(UNITMASK, 0xFB, &v1 ) ;
		serialReadRaptorRegister1(UNITMASK, 0xFA, &v2 ) ;

		nValue = (((int)v1&0x0F)<<8) + ((int)v2) ;

		dValue = ConvertTECValueToTemp(nValue);

	return dValue;
}

/************************************************************************
*
************************************************************************/
void JLP_RaptorCam1::SetTECSetPoint(double dValue)
{
		unsigned char bufin1[] = {0x53, 0xE0, 0x02, 0xFB, 0x00, 0x50};
		unsigned char bufin2[] = {0x53, 0xE0, 0x02, 0xFA, 0x00, 0x50};
		unsigned char buf[256];
		unsigned int nValue=0;

		if(EPROM_DAC_Cal_0C > 0 && EPROM_DAC_Cal_40C > 0)
			nValue = ConvertTECTempToValue(dValue);
		else
			return;

		if(nValue < 0 || nValue > 0x0FFF)
			return;

		bufin1[4] = (unsigned char)((nValue&0x0F00)>>8);
		bufin2[4] = (unsigned char)((nValue&0x00FF));

		serialWriteReadCmd(UNITSOPENMAP, UNITMASK, bufin1, 6, buf, 256 );
		serialWriteReadCmd(UNITSOPENMAP, UNITMASK, bufin2, 6, buf, 256 );

}
