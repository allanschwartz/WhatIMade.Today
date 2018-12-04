/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_xr_psp_valid(psp, bd, addr)
 *
 *  DESCRIPTION
 *	Look for magic packets "UZ" or "UX" and special
 *	names "B00x", in which case we update the reserved
 *	name/address lookup table entries.
 *
 *  FUNCTIONS CALLED
 *	cfn_update, cx_psp_cksum, strncmp
 *
 *  CALLED BY
 *	cx_xr_packet
 *
 *  ARGUMENTS
 *	none
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

BOOL        cx_xr_psp_valid(psp, bd, addr)

    REG PSPPKT *psp;		/* a6@(8) & a5 */
    REG BD     *bd;		/* a6@(0xc) & a4 */
    L1_ADDR     addr;		/* a6@(0x10) */

{

    REG char   *ba;		/* a3 */
    REG char   *name;		/* a2 */
    uchar       sum;		/* a6@(-1) */

    ba = psp->cx_save_ba;
    name = &psp->psp_name2;
    sum = cx_psp_cksum(bd);

    if ((ba[0] == 'U') && (ba[1] == 'Z')
	    && (sum == psp->cx_cksum)
	    && (name[0] == 'B')) {

	addr.l1_sock = 0x271a;
	cfn_update(256, addr);
	cfn_update(257, addr);
	addr.l1_sock = 0x271b;
	cfn_update(258, addr);
	return TRUE;
    }

    if ((ba[0] == 'U') && (ba[1] == 'X')
	    && (sum == psp->cx_cksum)) {

	addr.l1_sock = 0x271a;

	if (strnmatch(name, "B000", 4)) {
	    cfn_update(256, addr);
	    return TRUE;
	}

	if (strnmatch(name, "B001", 4)) {
	    cfn_update(257, addr);
	    return TRUE;
	}

	if (strnmatch(name, "B002", 4)) {
	    addr.l1_sock = 0x271a;
	    cfn_update(258, addr);
	    return TRUE;
	}
    }
    return FALSE;
}
