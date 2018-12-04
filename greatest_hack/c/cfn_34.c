/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_psp_reply(psp, bd1, addr, sock)
 *
 *  DESCRIPTION
 *	Sends a PSP reply packet, using the specified IDP socket,
 *	and the specified address.
 *
 *  FUNCTIONS CALLED
 *	blt, cfn_reboot, cfn_sendidpdata, cfn_error, copybuf, sprintf
 *
 *  CALLED BY
 *	cx_xr_packet
 *
 *  ARGUMENTS
 *	~~~~
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

cx_psp_reply(psp, bd1, addr, sock)
    REG PSPPKT *psp;		/* a6@(8), a5 */
    REG BD     *bd1;		/* a6@(c), a4 */
    L1_ADDR     addr;		/* a6@(0x10) */
    ushort      sock;		/* a6@(0x1e) */

{

    REG PSPPKT *psp2;		/* a3 */
    BD         *bd2;		/* a6@(-4) */	/* ~~~ REG */

    psp2 = (PSPPKT *) BUFADDR(bd1);
    blt(psp2->psp_name2, psp->psp_name, 4);
    blt(psp2->psp_name, psp->psp_name2, 4);

    if ((bd2 = copybuf(bd1)) == NULL) {
	sprintf(cfn_linebuf, "%s: out of memory in copybuf", cfn_hostaddr);
	cfn_error(cfn_linebuf);
	cfn_reboot();
    }
    else {
	cfn_sendidpdata(bd2, sock, addr);
    }

}
