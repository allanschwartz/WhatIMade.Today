
/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_panic2(str, arg2)
 *
 *  DESCRIPTION
 *	Serious system error, die
 *
 *  FUNCTIONS CALLED
 *	panic
 *
 *  CALLED BY
 *	-no one-
 *
 *  ARGUMENTS
 *	~~~
 *
 *  HISTORY
 *	reversed engineering from binary, June/July 1991,
 *	by Allan M. Schwartz
 *
 *  BUGS
 *	arg2 in this function is never used
 *	This routine should not be called at all in the production software.
 *
 ********************************************************************/

#include "cfn.h"

cfn_panic2(str, arg2)
    char       *str;		/* a6@(8) */

{

    panic(str);

}
