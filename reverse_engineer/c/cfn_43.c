/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_43( bd, cxdata )
 *
 *  DESCRIPTION
 *      cfn_43() copies the packet data in a buffer to the end of
 *      the databuf.  Returns TRUE on success, FALSE if there is
 *      insufficient room in the databuf.
 *
 *  FUNCTIONS CALLED
 *      blt, buflen, cfn_37, cx_countbytes
 *
 *  CALLED BY
 *      cfn_SAdatamsg
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

BOOL        cfn_43(
    REG BD     *bd,             /* a6@(8), a5 */
    REG CXDATA *cxdata)         /* a6@(0xc), a4 */
{
    REG short   bdlen;          /* d7 */
    REG short   len;            /* d6 */
    REG char   *dptr;           /* a3 */

    bdlen = buflen(bd);

    dptr = &cxdata->cx_databuf[cx_countbytes(cxdata->cx_databuf)];

    if (bdlen + dptr < &cxdata->cx_databuf[513]) {

        for (; bd; bd = bd->bd_next) {
            len = BUFLENC(bd);
            blt(dptr, BUFADDR(bd), len);
            dptr += len;
        }
        return TRUE;
    }

    cfn_37(cxdata);
    return FALSE;
}
