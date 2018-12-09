/********************************************************************* *
 *  $Header:$
 *
 *  NAME
 *      cfn_sendidpdata(bd, src_socket, addr)
 *
 *  DESCRIPTION
 *      cfn_sendidpdata() sends an IDP packet to the specified destination
 *      address.
 *
 *  FUNCTIONS CALLED
 *      cfn_reboot, cfn_error, freemsg, getmaxmsg, sendmsg, sprintf
 *
 *  CALLED BY
 *      cfn_34, cfn_36, cfn_send2sock, cfn_error
 *
 *  ARGUMENTS
 *      ~~~
 *
 *  HISTORY
 *      reversed engineering from binary, June/July 1991,
 *      by Allan M. Schwartz
 *
 *  BUGS
 *      If there are no messages, this routine will be called recursively.
 *
 ********************************************************************/

#include "cfn.h"

cfn_sendidpdata(bd, src_socket, addr)
    BD         *bd;             /* a6@(8) */
    short       src_socket;     /* a6@(0xe) */
    L1_ADDR     addr;           /* a6@(0x10) */
{
    REG short   rc;             /* d7 */
    REG MSG    *msg;            /* a5 */

    if ((msg = getmaxmsg()) == NULL) {
        sprintf(cfn_linebuf, "%s: out of memory in getmsg", 
            cfn_hostaddr);
        cfn_error(cfn_linebuf);
        cfn_reboot();
    }

    MOVEL1ADDR(((IDL2_SDATAMSG *) msg)->idsd_sadd, cfn_attnet);

    ((IDL2_SDATAMSG *) msg)->idsd_sadd.l1_sock = src_socket;

    MOVEL1ADDR(((IDL2_SDATAMSG *) msg)->idsd_dadd, addr);

    ((IDL2_SDATAMSG *) msg)->idsd_ptype = 0x63; /* protocol type */
    msg->m_bufdes = bd;
    msg->m_prio = NORMAL;
    msg->m_type = MIDSDATA;     /* IDP send data */

    rc = sendmsg(msg, idu2nmbox);

    if (rc != NoError) {
        freemsg(msg);
    }
}
