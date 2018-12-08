/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_IDPdatamsg(msg)
 *
 *  DESCRIPTION
 *	cx_IDPdatamsg() is called by cx_main() to deal with the
 *	IDL2_RDATA message.  This is the data path from the
 *	xrx to cxs.
 *
 *  FUNCTIONS CALLED
 *	cfn_30, cfn_error, freemsg, sendmsg, sprintf
 *
 *  CALLED BY
 *	cx_main
 *
 *  ARGUMENTS
 *	none
 *
 *  HISTORY
 *	reversed engineering from binary, June/July 1991,
 *	by Allan M. Schwartz
 *
 *  BUGS
 *	As noted in the declarations below, some autos should be
 *	REG class.
 *
 ********************************************************************/

#include "cfn.h"

cx_IDPdatamsg(msg)
    MSG        *msg;		/* a6@(8) *//* ~~~ REG */

{

    REG CXDATA *cxdata;		/* a5 */
    REG BD     *bd;		/* a4 */
    REG short   myport;		/* d7 *//* ~~~ ushort */
    REG short   rc;		/* d6 */
    ushort      mysock;		/* a6@(-2) *//* ~~~ REG */

    cxdata = (CXDATA *) MYDATA();
    myport = cxdata->cx_index;
    bd = msg->m_bufdes;
    mysock = cxdata->cx_sock;

    if (((IDL2_RDATAMSG *) msg)->idrd_ptype != 0x63) {
	freemsg(msg);
	return;
    }

    /*
     * process the (Xrx) packet
     */
    rc = cx_xr_packet(bd, ((IDL2_RDATAMSG *) msg)->idrd_sadd, mysock);

    if (rc == 0) {
	freemsg(msg);
	return;
    }

    /*
     * send the data to the (CXS) serial port
     */
    msg->m_prio = 2;
    msg->m_type = MT2LDATA;
    ((VT_DATA *) msg)->portid = myport;
    ((VT_DATA *) msg)->eom = 0;
    ((VT_DATA *) msg)->reason = 0;
    ((VT_DATA *) msg)->delivery = VT_DATA_NORMAL;
    if ((rc = sendmsg(msg, cxdata->cx_sadmbox)) != NoError) {

	freemsg(msg);
	sprintf(cfn_linebuf,
		"%s: data for port %d, sendmsg fails on %d",
		cfn_hostaddr, myport, rc);
	cfn_error(cfn_linebuf);

    }

}
