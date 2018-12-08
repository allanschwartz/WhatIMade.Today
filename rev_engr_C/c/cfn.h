
#define TRUE	1
#define FALSE 	0
#define REG	register
#define ETX	3

#define strnmatch(a,b,n) strncmp(a,b,n)==0
#define strmatch(a,b,n) strcmp(a,b)==0

#define MCFN_ALARM 0x64		/* message type */
#define uchar	unsigned char


#include "../kernel/kernel.h"
#include "../global/addstructs.h"
#include "../global/mtypes.h"
#include "../global/util.h"
#include "../idp/idlevel2.h"
#include "../pi/pi.h"
#include "cfn_structs.h"
#include "cfn_vars.h"
#include "cfn_externs.h"
