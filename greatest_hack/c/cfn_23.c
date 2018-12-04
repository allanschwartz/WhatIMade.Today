/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_init(arg)
 *
 *  DESCRIPTION
 *	cx_init() is the per-port process initialization code, run as
 *	the entry point for this process.  We initialize our CXDATA
 *	struction, create some mailboxes, open a socket, then return.
 *	Just wait, when events happen, cx_main() will be called with a
 *	message.
 *
 *  FUNCTIONS CALLED
 *	cfn_reboot, cx_fill_ff, cfn_error, idopenskt, mboxcreate, sprintf
 *
 *  CALLED BY
 *	-kernel-
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

cx_init(arg)
    short       arg;		/* a6@(0xa) */

{

    REG CXDATA *cxdata;		/* a5 */
    REG short   index;		/* d7 */
    REG ushort  rc;		/* d6 */

    index = arg;

    cxdata = &cxDATAblk[index];
    SETDATA((ADDRESS) cxdata);

    cxdata->cx_mailboxdepth = 10;
    cxdata->cx_datastate = 0;
    cxdata->cx_index = index;
    cxdata->cx_mailbox1 = mboxcreate(cxdata->cx_mailboxdepth);
    cxdata->cx_mailbox2 = mboxcreate(cxdata->cx_mailboxdepth);
    cxdata->cx_mbid = MYMBID();
    cxdata->cx_sock = index + 0x2710;
    cx_fill_ff(cxdata->cx_databuf, 513);
    cxdata->cx_dataptr = cxdata->cx_databuf;

    rc = idopenskt(cxdata->cx_sock, cxdata->cx_mailbox1);
    if (rc != NoError) {
	sprintf(cfn_linebuf, "%s: cannot open socket %d",
		cfn_hostaddr, cxdata->cx_sock);
	cfn_error(cfn_linebuf);
	cfn_reboot();
    }

}
