/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cx_disconnectmsg(msg)
 *
 *  DESCRIPTION
 *      cx_disconnectmsg() is called by cx_main() to deal with the
 *      VT_DISCONNECTED message.
 *
 *  FUNCTIONS CALLED
 *      cfn_reboot, cfn_error, freemsg, sprintf
 *
 *  CALLED BY
 *      cx_main
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

cx_disconnectmsg(REG MSG *msg)  /* a6@(8), a5 */
{
    REG short   myport;         /* d7 */
    REG short   portid;         /* d6 */
    REG CXDATA *cxdata;         /* a4 */

    cxdata = (CXDATA *) MYDATA();
    myport = cxdata->cx_index;

    portid = ((VT_DISCONNECTED *) msg)->portid;

    if (portid != myport) {

        sprintf(cfn_linebuf,
                "%s: SA disconnected for port %d went to port %d",
                cfn_hostaddr, portid, myport);
        cfn_error(cfn_linebuf);
        return;
    }

    cxdata->cx_state = 0;
    cxdata->cx_sadmbox = 0;
    freemsg(msg);

    sprintf(cfn_linebuf,
            "%s: disconnection on cxs port %d, reason %d",
            cfn_hostaddr, myport, ((VT_DISCONNECTED *) msg)->ret_code);
    cfn_error(cfn_linebuf);
    cfn_reboot();
    return;
}
