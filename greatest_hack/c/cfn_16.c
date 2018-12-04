/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_dumptable()
 *
 *  DESCRIPTION
 *	cfn_dumptable() prints the in-use elements in the name/address
 *	lookup table.
 *
 *  FUNCTIONS CALLED
 *	display_xaddr, printf
 *
 *  CALLED BY
 *	-no one-
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

cfn_dumptable()
{

    REG int     i;		/* d7 */
    TENTRY      tent;		/* a6@(-0x12) */

    for (i = 0; i < 0x103; i++) {
	tent = table[i];
	if (tent.t_inuse) {
	    printf("name:%s", tent.t_name);
	    printf("host:%s", display_xaddr(&tent.t_addr.l1_host));
	    printf("socket:%s", tent.t_addr.l1_sock);
	}

    }

}
