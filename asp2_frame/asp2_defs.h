/**************************************************************************
* asp2_defs.h
* Definitions for PiscoSpeck2
*
* JLP
* Version 26/08/2015
**************************************************************************/
#ifndef asp2_defs_h_
#define asp2_defs_h_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// Array of subwindow pointers:
#define NCHILDS_MAX 14
// For Display2Mutex:
// 512x512 for Luca Picotti (Nov 2017)
#define NX1_MAX 512
#define NY1_MAX 512

#define DECODE_NAME_MAX 8
/*
* ipanel_decode_index: 0=ShortExp, 1=Modsq, 2=Autocc, 3=LongInt, 4=Quadrant,
* 5=FlattenedAutoc, 6=LuckyImage 7=DirectVector
*/

enum DecodeNameIndex {ShortExp=0, Modsq, Autocc, LongInt, Quadrant,
                      FlattenedAutoc, LuckyImage, DirectVector};

// 5 languages: English, French, Italian, Spanish, German
#define NLANG 5
// Tranlated messages in file:
#define NMAX_MESSAGES 1024

// Processing modes:
#define N_PROC_MODES 9

#define NCHOICES 10

typedef struct {
wxComboBox *combo;
wxString choices[NCHOICES];
wxString label;
int nchoices;
} JLP_ComboBox;

#endif
