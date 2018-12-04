/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_countbytes(p)
 *
 *  DESCRIPTION
 *	cx_countbytes() counts the number of bytes in the
 *	string, before a "0xff" is encountered.
 *
 *  FUNCTIONS CALLED
 *	-none-
 *
 *  CALLED BY
 *	cfn_str_to_bd, cfn_39, cfn_42, cfn_43
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

short       cx_countbytes(p)
    REG uchar  *p;		/* a5 */

{
    REG short   k;		/* d7 */

    for (k = 0; *p++ != 0xff; k++) {
	;
    }

    return k;
}
