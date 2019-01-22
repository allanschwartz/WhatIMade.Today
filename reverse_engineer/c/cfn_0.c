/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *      cfn_main()
 *
 *  DESCRIPTION
 *      cfn_main() is the main entry in the CFN module.  It is
 *      called as the main or entry function to the "CFN" process at
 *      boot time, from the SYSINIT table.
 *
 *      This function initializes things, and creates 4 child
 *      processes, "CXS0", "CXS1", "CXS2" and "CXS3".  These processes
 *      do the actual protocol processing.
 *
 *      Finally, cfn_main() calls (and never returns from)
 *      cfn_receive() where it handles alarms and connections
 *      requests.
 *
 *  FUNCTIONS CALLED
 *      cfn_initworld, cfn_reboot, cfn_receive, cfn_error, 
 *      procreate, prorun, sprintf
 *
 *  CALLED BY
 *      SYSINIT
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

cfn_main()
{
    REG short   i;              /* d7 */
    REG short   rc;             /* d6 */
    REG PCB    *pcb;            /* a5 */
    char        pname[8];       /* a6(-8) */

    cfn_initworld();

    for (i = 0; i < 4; i++) {

        sprintf(pname, "CXS%d", i);

        pcb = procreate(cx_init, i, pname, 2, SUPER + SHARESTACK, cx_main);

        if (pcb == NULL) {

            sprintf(cfn_linebuf, "%s: cannot create process %s",
                    cfn_hostaddr, pname);
            cfn_error(cfn_linebuf);
            cfn_reboot();
        }

        rc = prorun(pcb);

        if (rc != NoError) {

            sprintf(cfn_linebuf, "%s:cannot run process %s",
                    cfn_hostaddr, pname);
            cfn_error(cfn_linebuf);
            cfn_reboot();
        }
    }
    cfn_receive();
}
