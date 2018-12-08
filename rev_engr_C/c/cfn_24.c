/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_main(msg, mbid)
 *
 *  DESCRIPTION
 *	cx_main() is the main entry for the (shared-stack) child
 *	process.  These processes (one per port) run the
 *	cxs-xrx bridge protocol.
 *	When events happen, cx_main() is called with a
 *	message.
 *
 *  FUNCTIONS CALLED
 *	cx_listenmsg, cx_connectmsg, cx_disconnectmsg, cx_IDPdatamsg,
 *	cfn_SAdatamsg, freemsg
 *
 *  CALLED BY
 *	-kernel-
 *
 *  ARGUMENTS
 *	~~~
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

cx_main(msg, mbid)
    REG MSG    *msg;		/* a6@(8) & a5 */
    REG MBID    mbid;		/* a6@(0xc) & d7 */

{

    REG CXDATA *cxdata;		/* a4 */
    REG short   index;		/* d6 */


    cxdata = (CXDATA *) MYDATA();
    index = cxdata->cx_index;

    switch (msg->m_type) {

	case ML2TLISTEN:	/* 0x70e */
	    cx_listenmsg(msg);
	    break;

	case ML2TCONNECTED:	/* 0x709 */
	    cx_connectmsg(msg);
	    break;

	case ML2TDISCONNECTED:	/* 0x70c */
	    cx_disconnectmsg(msg);
	    break;

	case MIDRDATA:		/* 4 */
	    cx_IDPdatamsg(msg);
	    break;

	case ML2TDATA:		/* 0x70a */
	    cx_SAdatamsg(msg);
	    break;

	default:
	    freemsg(msg);
	    break;
    }

}
