/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_atoh_n(str, count)
 *
 *  DESCRIPTION
 *	cx_atoh_n() converts an ascii (string) representation of a
 *	hex number into an int.;
 *
 *  FUNCTIONS CALLED
 *	cfn_ascii_to_hex
 *
 *  CALLED BY
 *	cfn_20, cfn_41
 *
 *  ARGUMENTS
 *	~~~~
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

int         cx_atoh_n(str, count)
    char       *str;		/* a6@(8) */
    short       count;		/* a6@(0xe) */

{
    char        p[16];		/* a6@(-0x10) */
    short       i;		/* a6@(-0x12) */

    for (i = 0; i < count; i++) {
	p[i] = *str++;
    }

    p[i] = 0;
    return cfn_ascii_to_hex(p);
}
