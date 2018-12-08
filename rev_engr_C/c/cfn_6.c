/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_error(txt)
 *
 *  DESCRIPTION
 *	cfn_error() broadcasts an Error packet to the Xerox.
 *
 *  FUNCTIONS CALLED
 *	cfn_str_to_bd, cfn_reboot, cfn_sendidpdata, cfn_fmt_txt_pkt, clear
 *
 *  CALLED BY
 *	cfn_initworld, cfn_receive, cx_init, cx_listenmsg,
 *	cx_connectmsg, cx_disconnectmsg, cx_IDPdatamsg, cfn_set_alarm,
 *	cfn_34, cfn_SAdatamsg, cfn_36, cfn_send2sock, cfn_sendidpdata,
 *	cfn_main
 *
 *  ARGUMENTS
 *	~~~~
 *
 *  HISTORY
 *	reversed engineering from binary, June/July 1991,
 *	by Allan M. Schwartz
 *
 *  BUGS
 *	If out of messages, a recursive loop will form.
 *
 ********************************************************************/


#include "cfn.h"

cfn_error(txt)
    int         txt;		/* a6@(8) */

{
    REG BD     *bd;		/* a5 */
    char        buf[250 + 1];	/* a6@(-0xfb) */
    L1_ADDR     addr;		/* a6@(-0x108) */

    clear(buf, 251);

    cfn_fmt_txt_pkt(buf, cnstnt_pkt2, txt);

    if ((bd = cfn_str_to_bd(buf, 0)) == NULL)
	cfn_reboot();

    MOVEL1ADDR(addr, table[0x101].t_addr);
    cfn_sendidpdata(bd, cfn_src_sock, addr);
}
