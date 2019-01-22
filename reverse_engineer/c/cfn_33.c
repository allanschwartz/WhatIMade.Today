/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_33(psp, bd, port)
 *
 *  DESCRIPTION
 *      ~~~~
 *
 *  FUNCTIONS CALLED
 *      blt, cx_psp_cksum, cfn_ascii_to_hex
 *
 *  CALLED BY
 *      cfn_31
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


cfn_33(psp, bd, port)
    REG PSPPKT *psp;            /* a6@(8) & a5 */
    REG BD     *bd;             /* a6@(0xc) & a4 */
    short       port;           /* a6@(0x12) */
{
    REG char   *ba;             /* a3 */
    char        name[5];        /* a6@(-5) */
    uchar       sum;            /* a6@(-6) */
    int         rc;             /* a6@(-0xc) */

    ba = psp->psp_save_ba;
    sum = cx_psp_cksum(bd);
    if (ba[0] != 'U' || ba[1] != 'E' ||
            psp->psp_cksum != sum)
        return;

    blt(name, psp->psp_name2, 4);
    name[4] = '\0';

    rc = cfn_ascii_to_hex(name);
    if (rc < 0xc000 || rc > 0xcfff)
        return;

    if (cx_pn_index > 12)
        return;

    if (cx_port_active[port])
        return;

    blt(&cx_port_names[cx_pn_index], name, 4);
    cx_port_active[port] = 1;
    cx_pn_index += 4;
}
