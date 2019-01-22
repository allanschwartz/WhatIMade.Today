/*
 *  SYSTBL (initial system process) STRUCTURE
 *
 *  The SYSINIT table is used to form an array of initial system 
 *  process descriptors which the process "parent" uses to spawn 
 *  the boot-up processes.
 */
typedef struct systbl {
    int     (*p_initentry)();
    ADDRESS p_arg;
    char    *p_nam;
    ushort  p_pri;
    short   p_mod;

    int     (*p_mainentry)();
    int (*p_inittest)();
    MREQ    *p_mreq;
} SYSTBL;

# define SUPSHARED  SUPER+SHARESTACK
# define SHRDAGENT  SUPER|SHARESTACK|AGENT

SYSTBL SYSINIT [] = {

    { ed2init,  NULL, "ED2",   2, SUPSHARED, ed2main, NULL, NULL }, 
    { nm2init,  NULL, "ED2NM", 2, SUPSHARED, nm2main, NULL, NULL },
    { idpinit,  NULL, "IDP",   2, SUPSHARED, idpmain, NULL, NULL },
    { errinit,  NULL, "ERROR", 2, SUPSHARED, errmain, NULL, NULL },
    { ripinit,  NULL, "RIP",   2, SUPSHARED, errmain, NULL, NULL },

    /* SM's before PV and PS */
    { sminit,   NULL, "SM",    2, SUPSHARED, smmain,  NULL, NULL },
    { psinit,   NULL, "PS",    2, SUPSHARED, psmain,  NULL, NULL },

    { pepinit,  NULL, "PEP",   2, SUPSHARED, pepmain, NULL, NULL },
    { diskio2,  NULL, "DISKIO",2, SUPER,     NULL,    NULL, NULL },
    { sainit,   NULL, "SA",    2, SHRDAGENT, samain,  NULL, NULL },

    /* PV must initialize after DISKIO2 */
    { ch,       NULL, "CH",    2, SUPER,     NULL,    NULL, NULL },
    { pv,       NULL, "PV",    2, SUPER,     NULL,    NULL, NULL },
    { lbinit,   NULL, "LB",    2, SUPSHARED, lbmain,  NULL, NULL },
    { cfn,      NULL, "CFN",   2, SUPER,     NULL,    NULL, NULL },

    /* Now the blank entries for SM to use */
    { NULL,     NULL,  "        ",  0,  0,   NULL,    NULL, NULL }, 
    { NULL,     NULL,  "        ",  0,  0,   NULL,    NULL, NULL }, 
    { NULL,     NULL,  "        ",  0,  0,   NULL,    NULL, NULL }, 

    { NULL,     NULL,  NULL,        0,  0,   NULL,    NULL, NULL }, 

} ;








