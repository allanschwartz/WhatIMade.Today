/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_37(cxdata)
 *
 *  DESCRIPTION
 *
 *
 *  FUNCTIONS CALLED
 *      cs_setbytes_ff
 *
 *  CALLED BY
 *      cfn_SAdatamsg, cfn_43
 *
 *  ARGUMENTS
 *      ~~~
 *
 *  HISTORY
 *      reversed engineering from binary, June/July 1991,
 *      by Allan M. Schwartz
 *
 *  BUGS
 *      none known
 *
 ********************************************************************/

#include "cfn.h"

cfn_37(REG CXDATA *cxdata)         /* a6@(8), a5 */
{
    cx_fill_ff(cxdata->cx_databuf, 513);
    cxdata->cx_dataptr = cxdata->cx_databuf;
    cxdata->cx_datastate = 0;
}
