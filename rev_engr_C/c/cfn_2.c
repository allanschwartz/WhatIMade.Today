/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_receive()
 *
 *  DESCRIPTION
 *      cfn_receive() is where the "CFN" process waits to receive a
 *      message.  Also, the 2 second time-out message is send from 
 *      this function, and then resent every time it is received.
 *
 *  FUNCTIONS CALLED
 *      breceive, cfn_reboot, cfn_set_alarm, cfn_send2sock, cfn_error, 
 *      freemsg, getalarm, idopenskt, mboxcreate, sendmsg, sprintf
 *
 *  CALLED BY
 *      cfn_main
 *
 *  ARGUMENTS
 *      none
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

cfn_receive()
{
    REG short   i;              /* d7 */
    REG int     rc;             /* d6 */
    MSG        *msg;            /* a6@(-4) */
    MBID        mbid;           /* a6@(-8) */
    ushort      sock;           /* a6@(-0xa) */

    cfn_amsg = getalarm();

    sock = 0x271b;
    rc = idopenskt(sock, mboxcreate(1));
    if (rc != 0) {
        sprintf(cfn_linebuf, "%s: cannot open iamhere socket %d",
                cfn_hostaddr, sock);
        cfn_error(cfn_linebuf);
        cfn_reboot();
    }

    cfn_send2sock(sock);

do_set_alarm:
    /*
     * prime the timer ... set an ALARM
     */
    cfn_set_alarm();

    while (1) {
        rc = breceive(&msg, &mbid);
        if (rc == 0) {
            sprintf(cfn_linebuf, "%s: breceive returns %d!", cfn_hostaddr, rc);
            cfn_error(cfn_linebuf);
            continue;
        }

        switch (msg->m_type) {
            case MCFN_IAMHERE:
                cfn_send2sock(sock);
                /*
                 * msg == cfn_amsg, a statically allocated alarm message, don't
                 * free it here
                 */
                goto do_set_alarm;      /* set the alarm again */
            case ML2TLISTEN:
                i = ((VT_MSG *) msg)->portid;
                rc = sendmsg(msg, cxDATAblk[i].cx_mbid);
                if (rc != NoError) {
                    sprintf(cfn_linebuf, "%s: sendmsg returns %d: rebooting!",
                            cfn_hostaddr, rc);
                    cfn_error(cfn_linebuf);
                    freemsg(msg);       /* ~~~WTF? */
                    cfn_reboot();
                }
                break;
            default:
                freemsg(msg);
                break;
        }
    }
}
