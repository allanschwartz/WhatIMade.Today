/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cx_psp_packet(bd, flag)
 *
 *  DESCRIPTION
 *
 *
 *  FUNCTIONS CALLED
 *
 *  CALLED BY
 *
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


PSPPKT *     cx_psp_packet(a_bd, flag)
    BD         *a_bd;           /* a6@(8)        */
    short       flag;           /* a6@(0xe)      */

{
    REG BD     *bd;             /* a5            */
    short       cnt;            /* a6@(-2)       */
    short       len;            /* a6@(-6)       */
    char       *ba;             /* a6@(-0xa)     */
    long        k;              /* a6@(-0xe)     */

    bd = a_bd;

    cnt = 0;

    for (; bd; bd = bd->bd_next) {
        len = BUFLENC(bd);
        ba = (char *)BUFADDR(bd);

        for (; len; ba++, len--, cnt++) {

            if (cnt < 15) {

                cx_ID_data[cnt] = *ba;
            }
            else {
                if (cnt == 15) {

                    if (flag == 1)
                        return( cx_ID_data );

                    cx_save_ba = ba;

                    k = cfn_atoh_n(&cx_ID_data[1], 2);

                    continue;
                }

                else if (cnt >= 15) {

                    if (cnt < k + 15)
                        continue;
                }
                else if (cnt <= k + 15)
                    continue;

                if (cnt > k + 17)
                    continue;

                *(&cfn_d6 + k + 17 - cnt) = *ba;
            }

        }                       /* end for ( ; len; ) */

    }                           /* end for ( ; bd; ) */

    return cx_ID_data;
}
