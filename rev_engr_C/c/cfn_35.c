/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	cfn_
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

         cx_SAdatamsg(msg)

         MSG *msg;               a6@(8)

         {

         REG CXDATA *cxdata;     /*a5*/
         REG BD *bd;             /*a4*/
         REG ushort myport;      /*d7*/
         REG ushort portid;      /*d6*/
         REG short flag;         /*d5*/


         cxdata = (CXDATA *)MYDATA();
         bd = msg->m_bufdesc;
         myport = cxdata->cx_index;
         portid = msg->0x14;
         flag = 1;
         if( myport == portid )
         goto L_0x86:
         else {

	     sprintf(cfn_linebuf, "%s: SA data for port %d went to port %d",
		     cfn_hostaddr, portid, myport )
	     cfn_error(cfn_linebuf);
	     freemsg(msg);		/* ~~~ WTF */
	     return;
         }

         L_0x6e:
         {
	     freemsg(msg);
	     cfn_37(cxdata);
	     return;
         {

         if(cfn_43(bd, cxdata)!=1)

         goto L_0x6e;

         freemsg(msg);

         while( flag )
         {
         switch( cxdata->cx_datastate )

         {

         case 0: case 2:
             flag = cfn_38(cxdata);
             break;

         case 1: case 3:
             cxdata->cx_datastate = 4;
             return;

         case 8:
             flag = cfn_39(cxdata);
             break;

         case 9:
             cxdata->cx_datastate = 0xa;
             return;

         case 10:
             flag = cfn_40(cxdata);
             break;

         case 11:
             flag = cfn_36(cxdata);
             break;
         
         case 2: 
             goto L_0xbe;

         case 3:
             goto L_0xca;

         case 4: case 6:
             flag = cfn_41(cxdata)
             break;

         case 5:
             cxdata->cx_datastate = 6;
             return;

         case 6:
             goto L_0xf4;

         case 7: case 12:
             cxdata->cx_datastate = 8
             return;

         case 12:
             goto L_0x106;

         case 13:
         default:
             goto L_0x7a;
         }

         }

         return;
}

"

