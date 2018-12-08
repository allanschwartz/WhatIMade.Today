/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_initworld
 *
 *  DESCRIPTION
 *	cfn_initworld() initializes various global variables,
 *	including the name/address table, and opens IDP socket
 *	number 10012 as an error socket.
 *
 *  FUNCTIONS CALLED
 *	cfn_tableinit, cfn_reboot, cfn_error, clear, 
 *	display_xaddr, idopenskt, mboxcreate, sprintf
 *
 *  CALLED BY
 *	cfn_main
 *
 *  ARGUMENTS
 *	none
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

cfn_initworld()
{
    REG short   rc;		/* d7 */

    cfn_brdcst.l1_net = idattnet;
    cfn_brdcst.l1_host.ha_word1 = 0xffff;
    cfn_brdcst.l1_host.ha_word2 = 0xffff;
    cfn_brdcst.l1_host.ha_word3 = 0xffff;

    clear(cxDATAblk, 0x8a0);

    cfn_tableinit();

    cfn_attnet.l1_net = idattnet;
    cfn_attnet.l1_host.ha_word1 = idinrhost.ha_word1;
    cfn_attnet.l1_host.ha_word2 = idinrhost.ha_word2;
    cfn_attnet.l1_host.ha_word3 = idinrhost.ha_word3;

    cfn_hostaddr = display_xaddr(&cfn_attnet.l1_host);

    cfn_src_sock = 10012;

    rc = idopenskt(cfn_src_sock, mboxcreate(1));

    if (rc != NoError) {
	sprintf(cfn_linebuf, "%s: count not open error socket", cfn_hostaddr);
	cfn_error(cfn_linebuf);
	cfn_reboot();
    }
}
