/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_tcreate(index, name, addr)
 *
 *  DESCRIPTION
 *	cfn_tcreate() specifies one entry in the name/address lookup table.
 *
 *  FUNCTIONS CALLED
 *	blt, cfn_update
 *
 *  CALLED BY
 *	cfn_tableinit, cfn_30
 *
 *  ARGUMENTS
 *	~~~
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

cfn_tcreate(index, name, addr)
    REG int     index;		/* a6@(8), d7 */
    char       *name;		/* a6@(0xc) */
    L1_ADDR     addr;		/* a6@(0x10) */
{

    blt(&table[index], name, 4);
    table[index].t_inuse = 1;
    cfn_update(index, addr);
}
