/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_gethash(str)
 *
 *  DESCRIPTION
 *      cfn_gethash() is called to hash an ascii hex value into a an 
 *      index in the range of 0 to 255. This is used in address lookup.
 *
 *  FUNCTIONS CALLED
 *      cfn_ascii_to_hex
 *
 *  CALLED BY
 *      cfn_30, cfn_31
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

int     cfn_gethash(char *str)  /* a6@(8) */
{
    REG int     val;            /* d7 */

    if ((val = cfn_ascii_to_hex(str)) == -1) {
        return -1;
    }

    return (val & 0xff);
}
