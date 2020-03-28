/* @(#)cvb.c    6.1.1.2 (ESO-IPG) 9/23/93 15:13:40 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.COPYRIGHT  (c)  1993  European Southern Observatory
.IDENTIFIC  cvb.c
.LANGUAGE   C
.AUTHOR     P. Grosbol   ESO/IPG
.KEYWORDS   binary number conversion, IEEE-754, floating point
.PURPOSE    convert array of binary numbers from standard FITS
            format to local binary representation
.VERSION    1.0    1988-Nov-06 :  Creation,  PJG
.VERSION    1.1    1989-Oct-10 :  Byte swap for SP-FP,  PJG
.VERSION    2.0    1990-Feb-04 :  Change calling sequence,  PJG
.VERSION    2.1    1990-Feb-12 :  Change byte-swap algorithm, PJG
.VERSION    2.2    1990-Mar-30 :  Correct union pointers, PJG
.VERSION    2.3    1990-Sep-28 :  Correct IEEE -> VAXG, PJG
.VERSION    2.4    1991-Feb-12 :  Include fp-swap, PJG
.VERSION    2.5    1991-Mar-22 :  Remove a register variable 'v', PJG
.VERSION    2.6    1993-Sep-22 :  Correct IEEE NULL check + bswap, PJG
---------------------------------------------------------------------*/
#include   <math.h>
#include   "jlp_fits0_fmt.h"       /* define types of binary data format */
#include   "jlp_fits0_computer.h"  /* define types of data conversion */

/* 32 and 64 bit IEEE exp. mask
* dummy variable for alignment  */
static struct {
INT4   dummy;
char   cd[4];
char   cf[4];
} FDMSK = {0,{static_cast<char>(0x7F), static_cast<char>(0xF0),
              static_cast<char>(0x00), static_cast<char>(0x00)},
              {static_cast<char>(0x7F), static_cast<char>(0x80),
              static_cast<char>(0x00), static_cast<char>(0x00)}};
static  DFMT  cpu = { INTFMT, FLOATFMT,
                     SWAPSHORT, SWAPLONG, SWAPFLOAT, SWAPDOUBLE
                    };

static  int  samei2;  /* same 2-byte integer format         */
static  int  samei4;  /* same 4-byte integer format         */
static  DFMT efmt;  /* external data format definition    */
static  int  ls0,ls1,ls2,ls3;  /* long integer byte swap order       */
static  int  fs0,fs1,fs2,fs3;  /* 32-bit float byte swap order       */
static  int  ds0,ds1,ds2,ds3;  /* 64-bit double byte swap order      */
static  int  ds4,ds5,ds6,ds7;  /* 64-bit double byte swap order      */

int cvinit();
int cvr4(float *pbuf, int no);

int cvinit()
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PURPOSE      initiate data conversion routines
.RETURN       error code: ok=0, 1=unknown format
.ALGORITM     Analyze internal and external data format definitions
              and setup static varibles to define conversion needed.
--------------------------------------------------------------------*/
{
  int   i, lbo[8], xbo[8], sa[8];
  long  n;

// FITS characterisation:
  efmt.ifmt = TWOS_COMP; efmt.fpfmt = IEEEFLOAT;
  efmt.bos = 12;   efmt.bol = 1234;
  efmt.bof = 1234; efmt.bod = 12345678;

// JLP99, for IBM/PC:
// (defined in fitsfmt.h with computer.h)
/*
  cpu.ifmt = TWOS_COMP; cpu.fpfmt = IEEEFLOAT;
  cpu.bos = 21;   cpu.bol = 4321;
  cpu.bof = 4321; cpu.bod = 87654321;
*/

  samei2 = (cpu.ifmt == efmt.ifmt) && (cpu.bos == efmt.bos);
  samei4 = (cpu.ifmt == efmt.ifmt) && (cpu.bol == efmt.bol);

/*  Setup byte swap pattern for long int        */

  n = cpu.bol;
  for (i=0; i<4; i++) { lbo[3-i] = n%10; n /= 10; }
  n = efmt.bol;
  for (i=0; i<4; i++) { xbo[3-i] = n%10; n /= 10; }
  for (i=0; i<4; i++)
      for (n=0; n<4; n++) if (lbo[i]==xbo[n]) sa[i] = n;
  ls0 = sa[0]; ls1 = sa[1]; ls2 = sa[2]; ls3 = sa[3];

/*  Setup byte swap pattern for float           */

  n = cpu.bof;
  for (i=0; i<4; i++) { lbo[3-i] = n%10; n /= 10; }
  n = efmt.bof;
  for (i=0; i<4; i++) { xbo[3-i] = n%10; n /= 10; }
  for (i=0; i<4; i++)
      for (n=0; n<4; n++) if (lbo[i]==xbo[n]) sa[i] = n;
  fs0 = sa[0]; fs1 = sa[1]; fs2 = sa[2]; fs3 = sa[3];

/*  Setup byte swap pattern for double          */

  n = cpu.bod;
  for (i=0; i<8; i++) { lbo[7-i] = n%10; n /= 10; }
  n = efmt.bod;
  for (i=0; i<8; i++) { xbo[7-i] = n%10; n /= 10; }
  for (i=0; i<8; i++)
      for (n=0; n<8; n++) if (lbo[i]==xbo[n]) sa[i] = n;
  ds0 = sa[0]; ds1 = sa[1]; ds2 = sa[2]; ds3 = sa[3];
  ds4 = sa[4]; ds5 = sa[5]; ds6 = sa[6]; ds7 = sa[7];

  return 0;
}

/* Union for conversion
typedef union {
                UINT1           c[2];       // bytes
                INT2               s;       // 2 byte integer
              } VI2;
*/
int cvi2(VI2 *pbuf, int no, int to)
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PURPOSE      convert 2 byte integer array between different computers
.RETURN       error code: ok=0, 1=cannot convert
.ALGORITM     swap bytes between VAX - nonVAX machines
---------------------------------------------------------------------*/
/*
      VI2      *pbuf;                  // pointer to data array
      int         no;                  // no. of values to convert
      int         to;                  // true if convert to ext.fmt
*/
{
  register  UINT1 byte;
  register  int   n;
  register  VI2   *pv;

  if (no<1 || samei2) return 0;        /* check if conversion needed */

  if (cpu.ifmt!=efmt.ifmt) return 1;      /* no format conversion    */

  if (cpu.bos!=efmt.bos) {                /* byte swap needed !      */
     n = no; pv = pbuf;
     while (n--) {                        /* loop through data array */
       byte = pv->c[0]; pv->c[0] = pv->c[1]; pv->c[1] = byte;
       pv++;
     }
  }

  return 0;
}
/* Union for conversion
typedef union {
                UINT1           c[2];       // bytes
                UINT2               s;       // 2 byte unsigned integer
              } UI2;
*/
int cui2(UI2 *pbuf, int no, int to)
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PURPOSE      convert 2 byte unsigned integer array between different computers
.RETURN       error code: ok=0, 1=cannot convert
.ALGORITM     swap bytes between VAX - nonVAX machines
---------------------------------------------------------------------*/
/*
      UI2      *pbuf;                  // pointer to data array
      int         no;                  // no. of values to convert
      int         to;                  // true if convert to ext.fmt
*/
{
  register UINT1 byte;
  register int   n;
  register UI2   *pv;

  if (no < 1 || samei2) return 0;        /* check if conversion needed */

  if (cpu.ifmt != efmt.ifmt) return 1;      /* no format conversion    */

  if (cpu.bos != efmt.bos) {                /* byte swap needed !      */
     n = no; pv = pbuf;
     while (n--) {                        /* loop through data array */
       byte = pv->c[0]; pv->c[0] = pv->c[1]; pv->c[1] = byte;
       pv++;
     }
  }

  return 0;
}

/* union for conversion
typedef union {
                UINT1                 c[4]; // bytes
                INT4                     i; // 4 byte integer
              } VI4;
*/
int cvi4(VI4 *pbuf, int no, int to)
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PURPOSE      convert 4 byte integer array between different computers
.RETURN       error code: ok=0, 1=cannot convert
.ALGORITM     swap bytes and words between VAX - nonVAX machines
---------------------------------------------------------------------*/
/*
      VI4      *pbuf;                  // pointer to data array
      int         no;                  // no. of values to convert
      int         to;                  // true if convert to ext.fmt
*/
{
  register  UINT1 *pb;
  register  VI4   *pv;
  register  int   n;
  VI4             v;

  if (no<1 || samei4) return 0;        /* check if conversion needed */

  if (cpu.ifmt!=efmt.ifmt) return 1;      /* no format conversion    */

  if (cpu.bol!=efmt.bol) {                /* byte swap needed !      */
    n = no; pv = pbuf; pb = (UINT1 *) pbuf;
    if (to)
      while (n--) {                        /* loop through data array */
        v.c[ls0] = *pb++; v.c[ls1] = *pb++;
        v.c[ls2] = *pb++; v.c[ls3] = *pb++;
        (pv++)->i = v.i;
      }
    else
      while (n--) {                        /* loop through data array */
        v.i = (pv++)->i;
        *pb++ = v.c[ls0]; *pb++ = v.c[ls1];
        *pb++ = v.c[ls2]; *pb++ = v.c[ls3];
      }
  }

  return 0;
}

typedef union {                             /* union for conversion  */
                UINT1* c[4]; /* bytes                 */
                INT4*  i; /* 4 byte integer        */
                REAL4* f; /* 4 byte floating point */
              } VR4;
typedef union {                             /* union for conversion  */
                UINT1  c[4]; /* bytes                 */
                INT4   i; /* 4 byte integer        */
                REAL4  f; /* 4 byte floating point */
              } VVR4;
int cvr4(float *pbuf, int no)
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PURPOSE      convert 4 byte real array between different computers
.RETURN       error code: ok=0, 1=cannot convert
.ALGORITM     swap bytes between VAX - nonVAX machines
              in addition some bit change due to different FP def's.
---------------------------------------------------------------------*/
/*
      VR4      *pbuf;                  // pointer to data array
      int         no;                  // no. of values to convert
*/
{
  register  UINT1 *pb;
  register  VR4   pv;
  register  int   n;
  VVR4            v;
  int             bswap;

  bswap = (cpu.bof != efmt.bof);

  pv.f = pbuf;
  for(n = 0; n < no; n++)
      {
      if (isNULLFLOAT(pv.f[n])) pv.i[n] = 0xFFFFFFFFL;
      }
  if (bswap)
      {                        /* check if byte swap machine */
        pv.f = pbuf; pb = (UINT1 *) pbuf;
        for(n = 0; n < no; n++)
          {             /* loop through data array */
          v.c[fs0] = *pb++; v.c[fs1] = *pb++;
          v.c[fs2] = *pb++; v.c[fs3] = *pb++;
          pv.i[n] = v.i;
          }
      }

  return 0;
}

typedef union {                             /* union for conversion  */
                UINT1 c[8]; /* bytes                 */
                UINT2 s[4]; /* 2 byte integers       */
                INT4  i[2]; /* 4 byte integers       */
                REAL8 d; /* 8 byte floating point */
              } VR8;

int cvr8(VR8 *pbuf, int no, int to)
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PURPOSE      convert 8 byte real array between different computers
.RETURN       error code: ok=0, 1=cannot convert
.ALGORITM     swap bytes between VAX - nonVAX machines
              in addition change some bits due to differences in the
              internal FP definition.
---------------------------------------------------------------------*/
/*
      VR8      *pbuf;                  // pointer to data array
      int         no;                  // no. of values to convert
      int         to;                  // true if convert to ext.fmt
*/
{
  register  UINT1 *pb;
  register  int   n;
  register  VR8   *pv;
  VR8             v;
  int             bswap;
  if (no<1) return 0;                  /* check if conversion needed */
  bswap = (cpu.bod != efmt.bod);

  if (to) {                           /* convert to external format  */
    n = no; pv = pbuf;
    while (n--) {
                 if (isNULLDOUBLE(pv->d))
                        pv->i[0] = 0xFFFFFFFFL, pv->i[1] = 0xFFFFFFFFL;
                }
     if (bswap) {                         /* check if byte swap machine */
        n = no; pv = pbuf; pb = (UINT1 *) pbuf;
        while (n--) {
          v.c[ds0] = *pb++; v.c[ds1] = *pb++;
          v.c[ds2] = *pb++; v.c[ds3] = *pb++;
          v.c[ds4] = *pb++; v.c[ds5] = *pb++;
          v.c[ds6] = *pb++; v.c[ds7] = *pb++;
          pv->i[0] = v.i[0]; (pv++)->i[1] = v.i[1];
        }
     }
  }
  else {                      /* copy from external to internal format */
        n = no; pv = pbuf; pb = (UINT1 *) pbuf;
        while (n--) {
          if ((pv->i[0] & DPXM) == DPXM)
            { /* check NaN + Inf */
            toNULLDOUBLE(pv->d);
            pb += 8;
            }
            else if (bswap)
            {
            v.i[0] = pv->i[0]; v.i[1] = pv->i[1];
            *pb++ = v.c[ds0]; *pb++ = v.c[ds1];
            *pb++ = v.c[ds2]; *pb++ = v.c[ds3];
            *pb++ = v.c[ds4]; *pb++ = v.c[ds5];
            *pb++ = v.c[ds6]; *pb++ = v.c[ds7];
            }
            pv++;
          }
  }

  return 0;
}
