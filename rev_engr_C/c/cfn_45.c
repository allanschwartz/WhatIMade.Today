/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_fill_ff(p, cnt)
 *
 *  DESCRIPTION
 *	cx_fill_ff() fills the string pointed to by "p" with
 *	0xff bytes, for a length of "cnt".
 *
 *  FUNCTIONS CALLED
 *	-none-
 *
 *  CALLED BY
 *	cx_init, cfn_37, cfn_42
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

cx_fill_ff(p, cnt)
    REG uchar  *p;		/* a5 */
    short       cnt;		/* a6@(0xe) */

{

    REG short   k;

    for (k = 0; k < cnt; k++) {
	*p++ = 0xff;
    }

}
