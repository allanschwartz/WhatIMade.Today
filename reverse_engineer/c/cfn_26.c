/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cx_connectmsg(msg)
 *
 *  DESCRIPTION
 *      cx_connectmsg() is called by cx_main() to deal with the VT_CONNECT
 *      message.  It sends a CHANGEPARM message.
 *
 *  FUNCTIONS CALLED
 *      blt, cfn_panic, cfn_reboot, cfn_error, getbuf, sendmsg, sprintf
 *
 *  CALLED BY
 *      cx_main
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

cx_connectmsg(REG MSG *msg)     /* a6@(8), a5 */
{
    REG CXDATA *cxdata;         /* a4 */
    REG BD     *bd;             /* a3 */
    REG short   myport;         /* d7 *//* should be ushort */
    REG short   portid;         /* d6 *//* should be ushort */
    REG short   rc;             /* d5 */
    static short paramdata[] = {0x2800, 0x1300, 0x1c02, 0x2102, 0x1900};

    cxdata = (CXDATA *) MYDATA();
    myport = cxdata->cx_index;
    portid = ((VT_CONNECT *) msg)->portid;

    if (myport != portid) {

        sprintf(cfn_linebuf,
                "%s: SA connected for port %d went to port %d",
                cfn_hostaddr, portid, myport);
        cfn_error(cfn_linebuf);
        cfn_reboot();
    }
    else {

        cxdata->cx_state = 1;
        cxdata->cx_sadmbox = ((VT_CONNECT *) msg)->dmbox;

        if ((bd = getbuf(sizeof paramdata)) == NULL)
            cfn_panic("cx_connectmsg: getbuf returns NULL");

        blt(BUFADDR(bd), paramdata, sizeof paramdata);

        ((VT_CHANGEPARM *) msg)->portid = myport;
        msg->m_bufdes = bd;
        msg->m_prio = NORMAL;
        msg->m_type = MT2LCHANGEPARM;

        if ((rc = sendmsg(msg, cxdata->cx_sadmbox)) != NoError) {

            sprintf(cfn_linebuf,
                    "%s: port %d change parameters sendmsg fails with %d",
                    cfn_hostaddr, myport, rc);
            cfn_error(cfn_linebuf);
        }
    }
}
