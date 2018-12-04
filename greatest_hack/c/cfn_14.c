/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_panic(str)
 *
 *  DESCRIPTION
 *	Serious system error, die
 *
 *  FUNCTIONS CALLED
 *	panic
 *
 *  CALLED BY
 *	cx_connectmsg
 *
 *  ARGUMENTS
 *	~~~
 *
 *  HISTORY
 *	reversed engineering from binary, June/July 1991,
 *	by Allan M. Schwartz
 *
 *  BUGS
 *	This routine should not be called at all in the production software.
 *
 ********************************************************************/

#include "cfn.h"

cfn_panic(str)
    char       *str;		/* a6@(8) */

{

    panic(str);

}
