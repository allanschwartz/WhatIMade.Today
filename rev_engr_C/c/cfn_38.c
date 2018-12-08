/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_
 *
 *  DESCRIPTION
 *
 *
 *  FUNCTIONS CALLED
 *
 *  CALLED BY
 *
 *
 *  ARGUMENTS
 *	none
 *
 *  HISTORY
 *	reversed engineering from binary, June/July 1991,
 *	by Allan M. Schwartz
 *
 *  BUGS
 *	none known
 *
 ********************************************************************/

#include "cfn.h"

         BOOL    cfn_38(cxdata)
         REG CXDATA *cxdata;     a6@(8)
                                 a5
         {
         REG char *pp;           a4;
         short s1;               a6@(-2)
         ---

         cxdata
         pp = cxdata->cx_dataptr;

         s1 = 0;

         pp = cfn_42( pp, cxdata )

         ( s1 == 1 )

         pp = cfn_42( pp, cxdata )

         cxdata->cx_datastate = 3

         s1 = 1;

         (cxdata->cx_datastate == 0)

         (cxdata->cx_datastate == 2)

         pp = cfn_42( pp, cxdata )

         cxdata->cx_datastate = 1;
         }

         pp++

         (*pp == 0xff)

         (*pp == 1)

         (cxdata->cx_datastate == 1)

         (cxdata->cx_datastate == 3)

         cxdata->cx_datastate = 4;

         cxdata->cx_dataptr = pp;

         return  cfn_41( cxdata );

         (cxdata->cx_datastate == 0)

         (cxdata->cx_datastate == 2)

         cxdata->cx_datastate = 0xd;

         cxdata->cx_dataptr = pp;
         return TRUE;

         }
