/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_send2sock(sock)
 *
 *  DESCRIPTION
 *	cfn_send2sock() sends the global data in "cx_port_names" to the
 *	specified socket ( socket 10011 the "i-am_here" socket), at the
 *	address in table entry 0x102 (which is the broadcast address).
 *	This is only done once, at the top of cfn_receive().
 *
 *  FUNCTIONS CALLED
 *	cfn_str_to_bd, cfn_reboot, cfn_sendidpdata, cfn_error, 
 *	cfn_fmt_txt_pkt, clear, sprintf
 *
 *  CALLED BY
 *	cfn_receive
 *
 *  ARGUMENTS
 *	~~~~
 *
 *  HISTORY
 *	reversed engineering from binary, June/July 1991, 
 *      by Allan M. Schwartz
 *
 *  BUGS
 *	none known
 *
 ********************************************************************/

#include "cfn.h"

cfn_send2sock(sock)
    short       sock;		/* a6@(0xa) */

{
    REG BD     *bd;		/* a5 */
    char        buf[36];	/* a6@(-0x24) */
    L1_ADDR     addr;		/* a6@(-0x30) */


    clear(buf, 36);
    cfn_fmt_txt_pkt(buf, cnstnt_pkt1, cx_port_names);

    MOVEL1ADDR(addr, table[0x102].t_addr);

    if(( bd = cfn_str_to_bd(buf, 0) ) == NULL) {
	sprintf(cfn_linebuf, "%s: out of memory in getbuf", cfn_hostaddr);
	cfn_error(cfn_linebuf);
	cfn_reboot();
    }

    cfn_sendidpdata(bd, sock, addr);
    clear(cx_port_names, 17);
    clear(cx_port_active, 4);
    cx_pn_index = 0;
}
