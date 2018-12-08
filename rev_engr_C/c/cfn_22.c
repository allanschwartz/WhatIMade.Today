/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cx_psp_cksum(bd)
 *
 *  DESCRIPTION
 *	This routine returns the checksum of the characters in the
 *	buffer, until the end-of-data, or until the second ETX is found.
 *
 *  FUNCTIONS CALLED
 *	-none-
 *
 *  CALLED BY
 *	cfn_32, cfn_33
 *
 *  ARGUMENTS
 *	~~~~
 *
 *  HISTORY
 *	reversed engineering from binary, June/July 1991,
 *	by Allan M. Schwartz
 *
 *  BUGS
 *	All the autos should be class REG.
 *	Variable "pad" is unused.
 *
 ********************************************************************/

#include "cfn.h"

#define ETX 3

uchar        cx_psp_cksum(bd)
    REG BD     *bd;		/* a6@(8) , a5 */

{

    char        val;		/* a6@(-1) */
    char       *str;		/* a6@(-8) */
    short       flag;		/* a6@(-0xa) */
    short	pad;		/* a6@(-0xc) */
    short       len;		/* a6@(-0xe) */

    val = 0;
    flag = 1;
    for (; bd; bd = bd->bd_next) {

	for( len = BUFLENC(bd), str = (char *)BUFADDR(bd); len; len--, str++) {

	    if (flag) {
		flag = 0;
		continue;
	    }

	    if (*str == ETX) {
		val ^= *str;
		return val;
	    }

	    val ^= *str;
	}

    }
    return 0;
}
