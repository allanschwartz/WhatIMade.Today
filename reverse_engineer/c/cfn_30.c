/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cx_xr_packet(bd, addr, sock)
 *
 *  DESCRIPTION
 *      cx_xr_packet() does the main processing for data packets
 *      from the XRX to the CXS.
 *
 *
 *  FUNCTIONS CALLED
 *      blt, cfn_gethash, cfn_tcreate, cfn_update, cx_psp_packet, 
 *      cx_xr_psp_valid, cx_psp_reply, strncmp
 *
 *  CALLED BY
 *      cx_IDPdatamsg()
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

PSPPKT     *cx_psp_packet();

BOOL        cx_xr_packet(bd, addr, sock)
    REG BD     *bd;             /* a6@(8) & a5 */
    L1_ADDR     addr;           /* a6@(0xc) */
    ushort      sock;           /* a6@(0x18) */
{
    REG int     index;          /* d7 */
    REG PSPPKT *psp;            /* a4 */
    short       flag;           /* a6@(-2) */
    char        name[5];        /* a6@(-9) */

    flag = FALSE;
    if ((psp = cx_psp_packet(bd, 1)) == NULL)
        return FALSE;

    blt(name, psp->psp_name, 4);
    if (strnmatch(name, "D000", 4)) {
        if ((psp = cx_psp_packet(bd, 0)) == NULL)
            return FALSE;

        if (cx_xr_psp_valid(psp, bd, addr) == TRUE) {
            addr.l1_sock = 0x271a;
            cx_psp_reply(psp, bd, addr, sock);
        }
        return FALSE;
    }
    else {

        blt(name, psp->psp_name2, 4);
        name[4] = 0;
        if ((name[0] != 'A') || (name[1] != '0')) {

            if ((name[0] != 'B'))
                return FALSE;

            flag = TRUE;
        }

        if (flag)
            return TRUE;

        if ((index = cfn_gethash(name)) < 0)
            return FALSE;

        if (table[index].t_inuse == 0) {
            cfn_tcreate(index, name, addr);
        }
        else {
            cfn_update(index, addr);
        }
        return TRUE;
    }
}
