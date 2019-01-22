/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_tableinit()
 *
 *  DESCRIPTION
 *      cfn_tableinit() initializes the name/address lookup table.
 *
 *  FUNCTIONS CALLED
 *      cfn_tcreate, clear, sprintf
 *
 *  CALLED BY
 *      cfn_initworld
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

cfn_tableinit()
{
    L1_ADDR     addr;           /* a6@(-0xc) */
    short       i;              /* a6@(-0xe)  ~~~ REG?*/
    char        name[4+1];      /* a6@(-0x13) */

    clear(&table[0], 18 * 259);

    MOVEL1ADDR(addr, cfn_brdcst);
    addr.l1_sock = 10010;

    for (i = 0; i < 3; i++) {

        sprintf(name, "B00%d", i);

        if (i == 2) {
            addr.l1_sock = 10011;
        }

        cfn_tcreate(i + 256, name, addr);
    }
}
