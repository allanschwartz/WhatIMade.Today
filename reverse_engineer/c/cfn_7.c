/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_fmt_txt_pkt(dst, txt1, txt2)
 *
 *  DESCRIPTION
 *      cfn_fmt_txt_pkt() formats a text packet, before it is sent to
 *      either the Error socket or the I-AM-HERE socket on the Xerox.
 *
 *  FUNCTIONS CALLED
 *      blt, cfn_cksum, sprintf, strlen
 *
 *  CALLED BY
 *      cfn_send2sock, cfn_error
 *
 *  ARGUMENTS
 *      ~~~~
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

char    *cfn_fmt_txt_pkt(dst, txt1, txt2)
    REG char   *dst;            /* a6@(8) & a5 */
    REG char   *txt1;           /* a6@(0xc) & a4 */
    REG char   *txt2;           /* a6@(0x10) & a3 */
{
    char        tmp[3];         /* a6@(-3) */
    REG short   len;            /* d7 */

    len = strlen(txt2);
    blt(dst, txt1, 17);

    sprintf(tmp, "%2x", len + 3);
    blt(&dst[1], tmp, 2);
    if (len + 3 <= 15) {
        dst[1] = '0';
    }

    blt(&dst[17], txt2, len);
    dst[len + 17] = 0x16;       /* SYN */
    dst[len + 18] = 3;          /* ETX */
    dst[len + 19] = cfn_cksum(&dst[1]);
    return dst;
}
