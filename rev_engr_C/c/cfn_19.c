/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_reboot()
 *
 *  DESCRIPTION
 *	Serious system error, reboot this comm. server.
 *
 *  FUNCTIONS CALLED
 *	reboot
 *
 *  CALLED BY
 *	cfn_initworld, cfn_receive, cx_init, cx_listenmsg, cx_connectmsg, 
 *	cx_disconnectmsg, 
 *
 *  ARGUMENTS
 *	none
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

cfn_reboot()
{
    reboot();
}
