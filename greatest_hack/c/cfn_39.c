/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_39(cxdata)
 *
 *  DESCRIPTION
 *
 *
 *  FUNCTIONS CALLED
 *	cx_countbytes
 *
 *  CALLED BY
 *	cfn_SAdatamsg
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

#define ETX	3

BOOL        cfn_39(cxdata)

    REG CXDATA *cxdata;		/* a5 */

{

    REG short   cnt;		/* d7 */
    REG char   *pp;		/* a4 */

    pp = cxdata->cx_dataptr;
    cnt = cx_countbytes(pp);
    if (cnt <= cxdata->cx_data8) {
	cxdata->cx_datastate = 0xc;
	return TRUE;
    }

    if (pp[cxdata->cx_data8] == ETX) {
	if (cx_countbytes(pp) >= 2) {
	    cxdata->cx_datastate = 0xb;
	    return TRUE;
	}

	cxdata->cx_dataptr = pp;
	cxdata->cx_datastate = 9;
	return TRUE;
    }

    cxdata->cx_datastate = 2;
    return TRUE;

}
