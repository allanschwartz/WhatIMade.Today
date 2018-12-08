/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_update(index, addr)
 *
 *  DESCRIPTION
 *	cfn_update() is used to respecify an address lookup table
 *	entry.
 *
 *  FUNCTIONS CALLED
 *	none
 *
 *  CALLED BY
 *	cfn_tcreate, cfn_30, cfn_32
 *
 *
 *  ARGUMENTS
 *	~~~~
 *
 *  HISTORY
 *	reversed engineering from binary, June/July 1991,
 *	by Allan M. Schwartz
 *
 *  BUGS
 *	This is rather inefficient code.
 *
 ********************************************************************/
#include "cfn.h"

cfn_update(index, addr)
    int         index;		/* a6@(8) */
    L1_ADDR     addr;		/* a6@(0xc) */

{

    table[index].t_addr.l1_net = addr.l1_net;
    table[index].t_addr.l1_host.ha_word1 = addr.l1_host.ha_word1;
    table[index].t_addr.l1_host.ha_word2 = addr.l1_host.ha_word2;
    table[index].t_addr.l1_host.ha_word3 = addr.l1_host.ha_word3;
    table[index].t_addr.l1_sock = addr.l1_sock;

}
