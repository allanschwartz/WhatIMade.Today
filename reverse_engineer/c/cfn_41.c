/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_
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

BOOL    cfn_41(REG CXDATA *cxdata)  /* a6@(8) , a5 */
{
    REG char   *pp;             /* a4 */
    short       s1;             /* a6@(-2) */

    pp = cxdata->cx_dataptr;
    while (1) {
        switch (*pp) {

            case -1:
                cxdata->cx_dataptr = pp;
                return TRUE;

            case 1:
                pp = cfn_42(pp, cxdata);
                cxdata->cx_datastate = 1;
                break;

            default:
                switch (cxdata->cx_datastate) {

                    case 1:
                    case 4:
                        cxdata->cx_datastate = 5;
                        break;

                    case 5:
                    case 6:
                        s1 = cx_atoh_n(&cxdata->cx_databuf[1], 2);
                        cxdata->cx_data8 = s1 + 15;
                        cxdata->cx_data6 = s1 + 17;
                        cxdata->cx_datastate = 8;
                        cxdata->cx_dataptr = cxdata->cx_databuf;
                        return TRUE;

                    default:
                        break;
                }
        }
        pp++;
    }
}
