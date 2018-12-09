/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_31(bd, addrp, port)
 *
 *  DESCRIPTION
 *
 *
 *  FUNCTIONS CALLED
 *      blt, cfn_gethash, cfn_20, cfn_33, strncmp
 *
 *  CALLED BY
 *      cfn_36
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
PSPPKT     *cx_xr_packet();

BOOL        cfn_31(bd, addrp, port)
    REG BD     *bd;             /* a6@(8) & a5 */
    L1_ADDR    *addrp;          /* a6@(0xc) */
    short       port;           /* a6@(0x12) */
{
    REG PSPPKT *psp;            /* a4 */
    REG int     index;          /* d7 */
    char        name[5];        /* a6@(-5) */

    if ((psp = cx_psp_packet(bd, 1)) == NULL)
        return FALSE;

    blt(name, psp->psp_name, 4);

    if (strnmatch(name, "D000", 4)) {
        if ((psp = cx_xr_packet(bd, 0)) == NULL)
            return FALSE;

        cfn_33(psp, bd, port);
        return FALSE;
    }

    blt(name, psp->psp_name, 4);
    name[4] = '\0';

    if (!strnmatch(name, "A0", 2) &&
            !strnmatch(name, "B000", 4) &&
            !strnmatch(name, "B001", 4))
        return FALSE;

    if (name[0] != 'B') {
        index = cfn_gethash(name);
    }
    else {
        switch (name[3]) {
            case '0':
                index = 256;
                break;
            case '1':
                index = 257;
                break;
            default:
                return FALSE;
        }
    }

    if (table[index].t_inuse == 0)
        return FALSE;

    blt(addrp, &table[index].t_addr, sizeof(L1_ADDR));
    return TRUE;
}
