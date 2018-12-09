/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_print1_tentry(i)
 *
 *  DESCRIPTION
 *      cfn_print1_tentry() prints a single structure in the name/address
 *      lookup table.
 *
 *  FUNCTIONS CALLED
 *
 *  CALLED BY
 *
 *
 *  ARGUMENTS
 *      none
 *
 *  HISTORY
 *      reversed engineering from binary, June/July 1991,
 *      by Allan M. Schwartz
 *
 *  BUGS
 *      Very inefficient. Should have used a pointer to a TENTRY.
 *
 ********************************************************************/

#include "cfn.h"

cfn_print1_tentry(int i)        /* a6@(8) */
{
    TENTRY      tent;           /* a6@(-0x12) */

    tent = table[i];

    printf("table[%d]:", i);
    printf("name:%s", tent.t_name);
    printf("host:%s", display_xaddr(&tent.t_addr.l1_host));
    printf("socket:%d", tent.t_addr.l1_sock);
}
