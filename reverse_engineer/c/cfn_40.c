/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_40(cxdata)
 *
 *  DESCRIPTION
 *
 *
 *  FUNCTIONS CALLED
 *      -none-
 *
 *  CALLED BY
 *      cfn_SAdatamsg
 *
 *  ARGUMENTS
 *      none
 *
 *  HISTORY
 *      reversed engineering from binary, June/July 1991,
 *      by Allan M. Schwartz
 *
 *  BUGS
 *      This function always returns TRUE.
 *
 ********************************************************************/

#include "cfn.h"

BOOL    cfn_40(REG CXDATA *cxdata)      /* a5 */
{
    REG char   *pp;                     /* a4 */

    pp = cxdata->cx_dataptr;
    pp++;
    if (*pp != -1) {
        cxdata->cx_datastate = 0xb;
        return TRUE;
    }
    return TRUE;
}
