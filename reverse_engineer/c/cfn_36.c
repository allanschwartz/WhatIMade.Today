/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_36(cxdata)
 *
 *  DESCRIPTION
 *
 *
 *  FUNCTIONS CALLED
 *      blt, cfn_str_to_bd, cfn_reboot, cfn_31, cfn_42, 
 *      cfn_sendidpdata, cfn_error, freebuf, sprintf
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
 *      s1, type mismatch
 *
 ********************************************************************/

#include "cfn.h"

BOOL    cfn_36(REG CXDATA cxdata) /* a6@(8), a5 */
{
    REG short   d7;             /* d7 */
    REG BD     *bd;             /* a4 */
    L1_ADDR     addr;           /* a6@(-0xc) */
    char        buf[251];       /* a6@(-0x107) */
    short       s1;             /* a6@(-0x10a) */       /* ~~~ REG */
                /* wrong type or completely unnecessary */

    cxdata->cx_datastate = 0;
    d7 = cxdata->cx_data6;
    blt(buf, cxdata->cx_databuf, d7);
    cxdata->cx_dataptr = cfn_42(&cxdata->cx_databuf[d7], cxdata);
    buf[d7] = 0xff;
    if (d7 < 17) {
        return TRUE;
    }

    if( (bd = cfn_str_to_bd(buf, TRUE)) == NULL ) {
        sprintf(cfn_linebuf, "%s: out of memory in getbuf", cfn_hostaddr);
        cfn_error(cfn_linebuf);
        cfn_reboot();
    }

    s1 = cfn_31(bd, &addr, cxdata->cx_index);
    if (!s1) {
        freebuf(bd);
        return TRUE;
    }

    cfn_sendidpdata(bd, cxdata->cx_sock, addr);
    return TRUE;
}
