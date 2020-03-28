// jlp_fits0_fmt.h
/**********************************************************************/
/* @(#)fitsfmt.h        6.1.1.1 (ESO-IPG) 7/16/93 20:24:04 */
/* @(#)fitscvb.h        6.1.1.1 (ESO-IPG) 7/16/93 20:24:03 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.COPYRIGHT    (c)  1991   European Southern Observatory
.LANGUAGE     C
.IDENT        fitscvb.h
.AUTHOR       P.Grosbol   ESO/IPG
.KEYWORDS     data representation and type definitions
.PURPOSE      define computers with different data formats
.VERSION      12/07/2010 (JLP)
--------------------------------------------------------------------*/
#ifndef     _FITSFMT
#define     _FITSFMT     1

#define     FITS         1             /* FITS format file          */
#define     IHAP         2             /* IHAP format file          */

#define     FITSLR    2880         /* Logical record size of FITS   */

#include    "asp2_typedef.h"         /* Definition of INT4, etc  */
/*** Now defined in asp2_typedef.h
typedef     char            INT1;
typedef     unsigned char   UINT1;
typedef     short           INT2;
typedef     unsigned short  UINT2;
typedef     long            INT4;
typedef     unsigned long   UINT4;
typedef     float           REAL4;
typedef     double          REAL8;
*/

#ifdef vms
#define osfphname(x,y)  OSY_TRNLOG(x,y,64,&no)
#endif

typedef  struct {
                  int            ifmt;   /* integer format          */
                  int           fpfmt;   /* floating point format   */
                  INT4            bos;   /* byte order for short    */
                  INT4            bol;   /* byte order for long     */
                  INT4            bof;   /* byte order for float    */
                  INT4            bod;   /* byte order for double   */
                } DFMT;

#define FPXM   (*(INT4 *)FDMSK.cf)         /* 32 bit IEEE exp. mask  */
#define DPXM   (*(INT4 *)FDMSK.cd)         /* 64 bit IEEE exp. mask  */


#endif
