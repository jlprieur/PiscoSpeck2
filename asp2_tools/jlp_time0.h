/**********************************************************************
*  jlp_time0.h
*
*  JLP
* Version 05-08-2015
**********************************************************************/

#ifndef jlp_time0_h_
#define jlp_time0_h_

int JLP_TIME_MSEC(char *string, int *istat);
int JLP_CTIME(char *string, int *istat);
int JLP_TIMER(float *cpu_time);
int jlp_time_microsec(char *full_date, int *istat);
int jlp_local_time(char *buffer);
int jlp_italian_date(char *cdate, char *ctime, int utime_offset);
int jlp_english_date(char *cdate, char *ctime, int utime_offset);
int jlp_french_date(char *cdate, char *ctime, int utime_offset);

#endif
