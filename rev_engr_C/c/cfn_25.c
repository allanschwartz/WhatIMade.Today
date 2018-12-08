/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_listenmsg(msg)
 *
 *  DESCRIPTION
 *	cx_listenmsg() is called by cx_main() to deal with the VT_LISTEN
 *	message.  It returns a VT_CONNECT message.
 *
 *  FUNCTIONS CALLED
 *	cfn_reboot, cfn_error, freemsg, sendmsg, sprintf
 *
 *  CALLED BY
 *	cx_main
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

cx_listenmsg(msg)
    REG MSG    *msg;		/* a6@(8) & a5 */

{

    REG CXDATA *cxdata;		/* a4 */
    REG short   myport;		/* d7 */
    REG short   dst_port;	/* d6 */
    REG short   rc;		/* d5 */

    cxdata = (CXDATA *) MYDATA();
    myport = cxdata->cx_index;
    dst_port = ((VT_MSG *) msg)->portid;

    if (myport != dst_port) {

	sprintf(cfn_linebuf,
		"%s: SA listener for port %d went to port %d",
		cfn_hostaddr, dst_port, myport);
	cfn_error(cfn_linebuf);
	freemsg(msg);
	cfn_reboot();
    }
    else {
	cxdata->cx_state = 2;
	cxdata->cx_sacmbox = ((VT_LISTEN *) msg)->cmbox;
	/*
	 * return a VT_CONNECT message
	 */
	msg->m_prio = 2;
	msg->m_type = 0x700;
	((VT_CONNECT *) msg)->portid = myport;
	((VT_CONNECT *) msg)->cmbox = cxdata->cx_mbid;
	((VT_CONNECT *) msg)->dmbox = cxdata->cx_mailbox2;

	if ((rc = sendmsg(msg, cxdata->cx_sacmbox)) != NoError) {

	    sprintf(cfn_linebuf,
		    "%s: port %d connect request for sendmsg fails with %d",
		    cfn_hostaddr, myport, rc);

	    cfn_error(cfn_linebuf);
	}
    }

}
