/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cx_printpkt(bd)
 *
 *  DESCRIPTION
 *      cx_printpkt() is a debugging routine to print the data in a
 *      buffer.
 *
 *  FUNCTIONS CALLED
 *      putchar
 *
 *  CALLED BY
 *      -no one-
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

cx_printpkt(REG BD *bd)         /* a6@(8), a5 */
{
    REG short   len;            /* d7 */
    REG char   *p;              /* a4 */

    for (; bd; bd = bd->bd_next) {

        len = BUFLENC(bd);
        p = (char *) BUFADDR(bd);

        for (; len; len--) {
            putchar(*p++);
        }
        putchar('|');
    }
    putchar('\n');
}
