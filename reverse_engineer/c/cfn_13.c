/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_str_to_bd
 *
 *  DESCRIPTION
 *      cfn_str_to_bd() allocates a buffer, with preallocated
 *      space for a 0x3a length protocol header, and copies
 *      a string into this buffer.
 *
 *  FUNCTIONS CALLED
 *      blt, cx_countbytes, getbuf, strlen, unprependbuf
 *
 *  CALLED BY
 *      cfn_36, cfn_send2sock, cfn_error
 *
 *  ARGUMENTS
 *      ~~~~
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

BD         *cfn_str_to_bd(
    char       *str,            /* a6@(8) */
    short       flag)           /* a6@(0xe) */
{
    REG short   len;            /* d7 */
    REG BD     *bd;             /* a5 */

    if (flag) {
        len = cx_countbytes(str);
    }
    else {
        len = strlen(str);
    }

    if ((bd = getbuf(len + 0x3a)) == NULL)
        return bd;

    if ((bd = unprependbuf(bd, 0x3a)) == NULL)
        return bd;

    if (len == 0)
        return bd;

    blt(bd->bd_address, str, len);
    return bd;
}
