/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_set_alarm
 *
 *  DESCRIPTION
 *	cfn_set_alarm sets a 120 second timeout.
 *
 *  FUNCTIONS CALLED
 *	setalarm, sprintf, cfn_reboot, cfn_error 
 *
 *  CALLED BY
 *	cfn_receive
 *
 *  ARGUMENTS
 *	none
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

cfn_set_alarm()
{
    REG short   rc;		/* d7 */
    REG AMSG   *amsg;		/* a5 */

    amsg = cfn_amsg;

    ((MSG *)amsg)->m_bufdes = NULL;
    ((MSG *)amsg)->m_prio = NORMAL;
    ((MSG *)amsg)->m_type = MCFN_IAMHERE;
    amsg->a_timer = 120000;	/* msecs */

    rc = setalarm(amsg);

    if (rc != NoError) {
	sprintf(cfn_linebuf, "%s: could not set iamhere alarm (%d)",
		cfn_hostaddr, rc);
	cfn_error(cfn_linebuf);
	cfn_reboot();
    }
}
