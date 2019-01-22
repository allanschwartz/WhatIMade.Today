/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_42(pp, cxdata)
 *
 *  DESCRIPTION
 *      Moves a string from (typically) the middle of the databuf 
 *      to the begginning of the databuf, filling the remainder of the
 *      databuf with "0xff".
 *
 *  FUNCTIONS CALLED
 *      blt, cx_countbytes, cx_fill_ff
 *
 *  CALLED BY
 *      cfn_36, cfn_38, cfn_41
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

char       *cfn_42(
    REG char   *pp,             /* a6@(8), a5 */
    REG CXDATA *cxdata)         /* a6@(0xc), a4 */
{
    char        buf[0x200];     /* a6@(-0x200) */
    REG short   cnt;            /* d7 */

    cnt = cx_countbytes(pp);
    blt(buf, pp, cnt);
    cx_fill_ff(cxdata->cx_databuf, 513);
    blt(cxdata->cx_databuf, buf, cnt);
    return (cxdata->cx_databuf);
}
