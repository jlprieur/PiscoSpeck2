/***************************************************************************
* asp2_rw_config_files
*
* JLP
* Version 17/08/2015
***************************************************************************/
#ifndef asp2_rw_config_files_
#define asp2_rw_config_files_

#include "asp2_typedef.h"

/* Routines accessible to all modules */

 int Init_PSET(PROCESS_SETTINGS& Pset2);
 int Copy_PSET(const PROCESS_SETTINGS Pset1, PROCESS_SETTINGS& Pset2);

 int ReadBoolFromConfigFile(char *filename, const char *keyword, bool *bvalue);
 int ReadIntFromConfigFile(char *filename, const char *keyword, int *ivalue);
 int ReadDoubleFromConfigFile(char *filename, const char *keyword, double *dvalue);
 int ReadStringFromConfigFile(char *filename, const char *keyword, char *cvalue);

#endif
