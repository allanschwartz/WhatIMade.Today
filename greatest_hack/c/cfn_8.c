/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_cksum(txt)
 *
 *  DESCRIPTION
 *	cfn_cksum() is called to compute the checksum of a null
 *	terminated string.
 *
 *  FUNCTIONS CALLED
 *	none
 *
 *  CALLED BY
 *	cfn_fmt_txt_pkt
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

uchar       cfn_cksum(txt)
    char       *txt;		/* a6@(8) */

{
    uchar       sum;		/* a6@(-1) */

    for (sum = 0; *txt;) {
	sum ^= *txt++;
    }
    return sum;
}
