/******************************************************************************
* Name:        asp_language (AspFrame class)
* Purpose:     handling menu events of Psc1Frame class
*
* Author:      JLP
* Version:     18/07/2015
******************************************************************************/
#include "asp2_frame.h"
#include "asp2_frame_id.h"  // Menu identifiers
#include "asp2_frame_language.h"  // Unicode/html conversion array

/*
#define DEBUG
*/
static int read_messages_file(char *in_fname,
                               wxString sstr[NLANG][NMAX_MESSAGES]);
static int string_html_to_unicode(char *str_html, wxString &str_unicode);
static int decode_html_to_unicode(char *html_item, wxString &unicode_item);
/************************************************************************
* Load Messages contained in PiscoSpeck2_messages.txt and
* save them to private array Str0
* iLang: 0=English 1=French 2=Italian 3=Spanish 4=German
*************************************************************************/
int AspFrame::LoadMenuMessages()
{
int status;
char fname[128];
wxString buffer;

strcpy(fname, "PiscoSpeck2_messages.txt");

status = read_messages_file(fname, Str0);

if(status) {
  buffer.Printf(wxT("Error loading file %s"), fname);
  wxMessageBox(buffer, wxT("LoadmenuMessages"), wxOK | wxICON_ERROR);
  }


return(status);
}
/***************************************************************************
* Open the messages file and save content to sstr array
***************************************************************************/
static int read_messages_file(char *in_fname,
                               wxString sstr[NLANG][NMAX_MESSAGES])
{
int i, k, index, nval;
char buffer[80], *pc;
wxString msg_unicode;
FILE *fp_in;

for(k = 0; k < NLANG; k++)
  for(i = 0; i < NMAX_MESSAGES; i++)
     sstr[k][i] = wxT("");

if((fp_in = fopen(in_fname, "r")) == NULL) {
  fprintf(stderr, "read_messages_file/Error opening input file >%s<\n",
          in_fname);
  return(-1);
  }

k = -1;
while(!feof(fp_in)) {

  if(fgets(buffer, 80, fp_in) != NULL) {

// Check if line with index: */
    if(!strncmp(buffer, "% i=", 4))  {
      nval = sscanf(&buffer[4], "%d", &index);
#ifdef DEBUG
printf("OK0/nval=%d index=%d\n", nval, index);
#endif
      if(nval == 1) k = 0;
    } else if(buffer[0] == '%')  {
      k = -1;
    } else if(k >= 0) {
// Removes CR and \r at the end of the buffer:
      pc = buffer;
      while(*pc && *pc != '\r' && *pc != '\n') pc++;
      *pc = '\0';
      string_html_to_unicode(buffer, msg_unicode);
      sstr[k][index] = msg_unicode;
      if(k < NLANG - 1) k++;
      else k = -1;
    }
  } // EOF fgets != NULL

} // EOF while

fclose(fp_in);

// Fill array with English items if missing translations in other languages:
for(i = 0; i < NMAX_MESSAGES; i++) {
   if(!sstr[0][i].IsEmpty()) {
   for(k = 1; k < NLANG; k++)
      if(sstr[k][i].IsEmpty()) sstr[k][i] = sstr[0][i];
   }
}

return(0);
}
/**************************************************************************
*
**************************************************************************/
static int string_html_to_unicode(char *str_html, wxString &str_unicode)
{
char buffer[80], buff_begin[80], buff_end[80], html_item[80];
wxString unicode_item;
char *pc;

str_unicode = wxT("");

strcpy(buffer, str_html);

pc = buffer;
while(*pc) {
  strcpy(buff_begin, buffer);
  pc = buff_begin;
// Look for begin of item ("&")
  while(*pc && *pc != '&') pc++;
  if(*pc == '&') {
  strcpy(html_item, pc);
  *pc ='\0';
  str_unicode.Append(buff_begin);
  } else {
   break;
  }
// Look for end of item (";")
  pc = html_item;
  while(*pc && *pc != ';') pc++;
  if(*pc == ';') {
    pc++;
    if(*pc) {
    strcpy(buff_end, pc);
    *pc = '\0';
    decode_html_to_unicode(html_item, unicode_item);
    str_unicode.Append(unicode_item);
    strcpy(buffer, buff_end);
    pc = buffer;
    } else {
    break;
    }
  } else {
  break;
  }
} // EOF while
str_unicode.Append(buffer);

#ifdef DEBUG
wxPrintf("UNICODE/DDEBUG/%s\n", str_unicode);
// Next is not working... why ?
printf("UNICODE/DDEBUG2/%s\n", (const char*)str_unicode.wc_str());
#endif

return(0);
}
/**************************************************************************
*
* INPUT:
*
* html_item: e.g. "&ecirc;"
**************************************************************************/
static int decode_html_to_unicode(char *html_item, wxString &unicode_item)
{
char *pc, html0[80];
int i, status = -1;

unicode_item = wxT("");

// Remove first character ('&')
strcpy(html0, &html_item[1]);
pc = html0;
// Remove last character (';')
while(*pc && *pc != ';') pc++;
if(*pc != ';') {
  fprintf(stderr, "decode_html_to_unicode/Error: html item =>%s<\n", html_item);
  return(-1);
  }
*pc = '\0';

// Look for unicode string:
 for(i = 0; i < N_UNICODE; i++) {
   if(!strcmp(html0, html_item1[i])) {
     unicode_item = unicode_item1[i];
     status = 0;
     break;
     }
   }

#ifdef DEBUG
printf("decode_html_to_unicode/OK4/status=%d unicode=>%s<\n",
        status, (const char*)unicode_item.wc_str());
// Next is not working... why ?
("decode_html_to_unicode/OK4: unicode_item=%s< (status=%d)\n",
          unicode_item, status);
#endif

return(status);
}
