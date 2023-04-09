/**********************************************************************
*  jlp_time0.h
*
* Contains:
* int JLP_CTIME(string,istat)
* int JLP_TIMER(float *cpu_time)
* int jlp_time_microsec(char *full_date, int *istat)
* int jlp_local_time(char *buffer);
* int jlp_italian_date(char *cdate, char *ctime, int utime_offset);
* int jlp_french_date(char *cdate, char *ctime, int utime_offset);
*
*  JLP
* Version 05-08-2015
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>

// LINUX
#ifdef LINUX
#include <sys/time.h>
// WIN32 API
#else
#include <windows.h>
#endif

#include "jlp_time0.h"
#include "jlp_string.h"

static int convert_to_utime(int *hh, int *mm, int *ss, int ut_offset);

// For Windows:
#ifndef LINUX
int gettimeofday(struct timeval* p, void* tz);

int gettimeofday(struct timeval* p, void* tz) {
    ULARGE_INTEGER ul; // As specified on MSDN.
    FILETIME ft;

    // Returns a 64-bit value representing the number of
    // 100-nanosecond intervals since January 1, 1601 (UTC).
    GetSystemTimeAsFileTime(&ft);

    // Fill ULARGE_INTEGER low and high parts.
    ul.LowPart = ft.dwLowDateTime;
    ul.HighPart = ft.dwHighDateTime;
    // Convert to microseconds.
    ul.QuadPart /= 10ULL;
    // Remove Windows to UNIX Epoch delta.
    ul.QuadPart -= 11644473600000000ULL;
    // Modulo to retrieve the microseconds.
    p->tv_usec = (long) (ul.QuadPart % 1000000LL);
    // Divide to retrieve the seconds.
    p->tv_sec = (long) (ul.QuadPart / 1000000LL);

    return 0;
}
#endif

/*****************************************************************
* JLP_TIME_MSEC  to get the time in micro-seconds since
* Epoch 1970 01 01 00:00:00 +0000 (UTC):
******************************************************************/
int JLP_TIME_MSEC(char *string, int *istat)
{
 char str[80];
 struct timeval tt;
  JLP_CTIME(str, istat);
  str[19] = '\0';
  gettimeofday(&tt, NULL);
  sprintf(string, "%s [%ld.%06ld sec]", str, tt.tv_sec, tt.tv_usec);
  return(0);
}
/*****************************************************************
* JLP_CTIME  to get the date and time:
******************************************************************/
int JLP_CTIME(char *string, int *istat)
{
time_t current_time;
char *p;

  *istat=1;
  time(&current_time);
  if( (p=ctime(&current_time)) != NULL) {
       *istat=0;
       strncpy(string, p, 20);
       string[19] = '\0';
   }
return(0);
}
/*****************************************************************
* JLP_TIMER to compute the cpu time
*
* Usage:
* First call to initialize the timer
* Second call to read the value of the elapsed time
******************************************************************/
 int JLP_TIMER(float *cpu_time)
 {
   *cpu_time = (float)clock() / CLOCKS_PER_SEC;
   return(0);
 }
/*****************************************************************
* jlp_time_microsec  to get the time in micro-seconds since
* Epoch 1970 01 01 00:00:00 +0000 (UTC):
*
* OUTPUT:
* full_date[64]: date and full micro-second time
******************************************************************/
int jlp_time_microsec(char *full_date, int *istat)
{
 char str[80];
// LINUX
#ifdef LINUX
  struct timeval tt;
  gettimeofday(&tt, NULL);
  JLP_CTIME(str, istat);
  jlp_string_compact(str, 80);
  sprintf(full_date, "%s [%ld:%06ld]", str, tt.tv_sec, tt.tv_usec);
// WIN32 API
#else
  SYSTEMTIME st;
  GetSystemTime(&st);
  jlp_local_time(str);
  jlp_string_compact(str, 80);
  sprintf(full_date, "%s [%02d.%03d sec]", str, st.wSecond, st.wMilliseconds);
#endif

return(0);
}
/*****************************************************************
* Routine to get local time and date (in English)
*
* Example of output:
*     "Wed Jun 19 12:05:34 1994"
******************************************************************/
int jlp_local_time(char *cdate)
{
struct tm *ptr;
time_t lt;
lt = time(NULL);
ptr = localtime(&lt);
strcpy(cdate, asctime(ptr));
return(0);
}
/*******************************************************************
* Conversion from local time to Universal Time
*******************************************************************/
static int convert_to_utime(int *hh, int *mm, int *ss, int ut_offset)
{
double dtime, dhh, dmm, dss;
dhh = (double)*hh;
dmm = (double)*mm;
dss = (double)*ss;
// Local time in hours:
dtime = dhh + dmm/60. + dss/3600.;
dtime += (double)ut_offset;
if(dtime < 0.) dtime += 24.;
*hh = (int)dtime;
// Conversion the difference to minutes
dtime = (dtime - (double)(*hh)) * 60.;
*mm = (int)dtime;
// Conversion the difference to seconds
dtime = (dtime - (double)(*mm)) * 60.;
*ss = (int)dtime;
return(0);
}
/*******************************************************************
* Routine to get local time and date (in English)
*
* Example of output:
*     "Wed Jun 19 12:05:34 1994"
*     cdate="Wednesday 19 June 1994"  ctime="12:05:04 (U.T.)"
********************************************************************/
int jlp_english_date(char *cdate, char *ctime, int utime_offset)
{
char buffer[80], time[20], year[20], day[10], month[10];
char date_it[80], month_it[20];
int hh, mm, ss;

jlp_local_time(buffer);

strncpy(time, &buffer[11], 8);
time[8] = '\0';
sscanf(time, "%2d:%2d:%2d", &hh, &mm, &ss);
// Conversion to Universal Time
convert_to_utime(&hh, &mm, &ss, utime_offset);
// Copy time to output string:
sprintf(ctime, "%02d:%02d:%02d (U.T.)", hh, mm, ss);

strncpy(year, &buffer[20], 4);
year[4] = '\0';
strncpy(day, &buffer[8], 2);
day[2] = '\0';
strncpy(month, &buffer[4], 3);
month[3] = '\0';

// Translation to English:
  if(!strncmp(month, "Jan", 3)){
  strcpy(month_it, "January");
  } else if(!strncmp(month, "Feb", 3)) {
  strcpy(month_it, "February");
  } else if(!strncmp(month, "Mar", 3)) {
  strcpy(month_it, "March");
  } else if(!strncmp(month, "Apr", 3)) {
  strcpy(month_it, "April");
  } else if(!strncmp(month, "May", 3)) {
  strcpy(month_it, "May");
  } else if(!strncmp(month, "Jun", 3)) {
  strcpy(month_it, "June");
  } else if(!strncmp(month, "Jul", 3)) {
  strcpy(month_it, "July");
  } else if(!strncmp(month, "Aug", 3)) {
  strcpy(month_it, "August");
  } else if(!strncmp(month, "Sep", 3)) {
  strcpy(month_it, "September");
  } else if(!strncmp(month, "Oct", 3)) {
  strcpy(month_it, "October");
  } else if(!strncmp(month, "Nov", 3)) {
  strcpy(month_it, "November");
  } else if(!strncmp(month, "Dec", 3)) {
  strcpy(month_it, "December");
  }

sprintf(date_it, "%s %s %s", day, month_it, year);

// Translation to italian:
if(!strncmp(buffer,"Mon",3)){
sprintf(cdate, "Monday %s", date_it);
} else if(!strncmp(buffer,"Tue",3)) {
sprintf(cdate, "Tuesday %s", date_it);
} else if(!strncmp(buffer,"Wed",3)) {
sprintf(cdate, "Wedneday %s", date_it);
} else if(!strncmp(buffer,"Thu",3)) {
sprintf(cdate, "Thursday %s", date_it);
} else if(!strncmp(buffer,"Fri",3)) {
sprintf(cdate, "Friday %s", date_it);
} else if(!strncmp(buffer,"Sat",3)) {
sprintf(cdate, "Saturday %s", date_it);
} else if(!strncmp(buffer,"Sun",3)) {
sprintf(cdate, "Sunday %s", date_it);
}

return(0);
}
/*******************************************************************
* Routine to get local time and date (in Italian)
*
* Example of output:
*     "Wed Jun 19 12:05:34 1994"
*     cdate="Giovedi 19 Giugno 1994"  ctime="12:05:04 (T.U.)"
********************************************************************/
int jlp_italian_date(char *cdate, char *ctime, int utime_offset)
{
char buffer[80], time[20], year[20], day[10], month[10];
char date_it[80], month_it[20];
int hh, mm, ss;

jlp_local_time(buffer);

strncpy(time, &buffer[11], 8);
time[8] = '\0';
sscanf(time, "%2d:%2d:%2d", &hh, &mm, &ss);
// Conversion to Universal Time
convert_to_utime(&hh, &mm, &ss, utime_offset);
// Copy time to output string:
// Marco wants extra blanks:
sprintf(ctime, "   %02d:%02d:%02d (T.U.)", hh, mm, ss);

strncpy(year, &buffer[20], 4);
year[4] = '\0';
strncpy(day, &buffer[8], 2);
day[2] = '\0';
strncpy(month, &buffer[4], 3);
month[3] = '\0';

// Translation to italian:
  if(!strncmp(month, "Jan", 3)){
  strcpy(month_it, "Gennaio");
  } else if(!strncmp(month, "Feb", 3)) {
  strcpy(month_it, "Febbraio");
  } else if(!strncmp(month, "Mar", 3)) {
  strcpy(month_it, "Marzo");
  } else if(!strncmp(month, "Apr", 3)) {
  strcpy(month_it, "Aprile");
  } else if(!strncmp(month, "May", 3)) {
  strcpy(month_it, "Maggio");
  } else if(!strncmp(month, "Jun", 3)) {
  strcpy(month_it, "Giugno");
  } else if(!strncmp(month, "Jul", 3)) {
  strcpy(month_it, "Luglio");
  } else if(!strncmp(month, "Aug", 3)) {
  strcpy(month_it, "Agosto");
  } else if(!strncmp(month, "Sep", 3)) {
  strcpy(month_it, "Settembre");
  } else if(!strncmp(month, "Oct", 3)) {
  strcpy(month_it, "Ottobre");
  } else if(!strncmp(month, "Nov", 3)) {
  strcpy(month_it, "Novembre");
  } else if(!strncmp(month, "Dec", 3)) {
  strcpy(month_it, "Dicembre");
  }

sprintf(date_it, "%s %s %s", day, month_it, year);

// Translation to italian:
if(!strncmp(buffer,"Mon",3)){
sprintf(cdate, "Lunedì %s", date_it);
} else if(!strncmp(buffer,"Tue",3)) {
sprintf(cdate, "Martedì %s", date_it);
} else if(!strncmp(buffer,"Wed",3)) {
sprintf(cdate, "Mercoledì %s", date_it);
} else if(!strncmp(buffer,"Thu",3)) {
sprintf(cdate, "Giovedì %s", date_it);
} else if(!strncmp(buffer,"Fri",3)) {
sprintf(cdate, "Venerdì %s", date_it);
} else if(!strncmp(buffer,"Sat",3)) {
sprintf(cdate, "Sabato %s", date_it);
} else if(!strncmp(buffer,"Sun",3)) {
sprintf(cdate, "Domenica %s", date_it);
}

return(0);
}
/*******************************************************************
* Routine to get local time and date (in French)
*
* Example of output:
*     "Wed Jun 19 12:05:34 1994"
*     cdate="Jeudi 19 Juin 1994"  ctime="12:05:04 (T.U.)"
********************************************************************/
int jlp_french_date(char *cdate, char *ctime, int utime_offset)
{
char buffer[80], time[20], year[20], day[10], month[10];
char date_it[80], month_it[20];
int hh, mm, ss;

jlp_local_time(buffer);

strncpy(time, &buffer[11], 8);
time[8] = '\0';
sscanf(time, "%2d:%2d:%2d", &hh, &mm, &ss);
// Conversion to Universal Time
convert_to_utime(&hh, &mm, &ss, utime_offset);
// Copy time to output string:
sprintf(ctime, "%02d:%02d:%02d (T.U.)", hh, mm, ss);

strncpy(year, &buffer[20], 4);
year[4] = '\0';
strncpy(day, &buffer[8], 2);
day[2] = '\0';
strncpy(month, &buffer[4], 3);
month[3] = '\0';

// Translation to French:
  if(!strncmp(month, "Jan", 3)){
  strcpy(month_it, "Janvier");
  } else if(!strncmp(month, "Feb", 3)) {
  strcpy(month_it, "Février");
  } else if(!strncmp(month, "Mar", 3)) {
  strcpy(month_it, "Mars");
  } else if(!strncmp(month, "Apr", 3)) {
  strcpy(month_it, "Avril");
  } else if(!strncmp(month, "May", 3)) {
  strcpy(month_it, "Mai");
  } else if(!strncmp(month, "Jun", 3)) {
  strcpy(month_it, "Juin");
  } else if(!strncmp(month, "Jul", 3)) {
  strcpy(month_it, "Juillet");
  } else if(!strncmp(month, "Aug", 3)) {
  strcpy(month_it, "Août");
  } else if(!strncmp(month, "Sep", 3)) {
  strcpy(month_it, "Septembre");
  } else if(!strncmp(month, "Oct", 3)) {
  strcpy(month_it, "Octobre");
  } else if(!strncmp(month, "Nov", 3)) {
  strcpy(month_it, "Novembre");
  } else if(!strncmp(month, "Dec", 3)) {
  strcpy(month_it, "Décembre");
  }

sprintf(date_it, "%s %s %s", day, month_it, year);

// Translation to French:
if(!strncmp(buffer,"Mon",3)){
sprintf(cdate, "Lundi %s", date_it);
} else if(!strncmp(buffer,"Tue",3)) {
sprintf(cdate, "Mardi %s", date_it);
} else if(!strncmp(buffer,"Wed",3)) {
sprintf(cdate, "Mercredi %s", date_it);
} else if(!strncmp(buffer,"Thu",3)) {
sprintf(cdate, "Jeudi %s", date_it);
} else if(!strncmp(buffer,"Fri",3)) {
sprintf(cdate, "Vendredi %s", date_it);
} else if(!strncmp(buffer,"Sat",3)) {
sprintf(cdate, "Samedi %s", date_it);
} else if(!strncmp(buffer,"Sun",3)) {
sprintf(cdate, "Dimanche %s", date_it);
}

return(0);
}
