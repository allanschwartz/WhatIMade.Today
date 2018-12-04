/*
 * CFN data space
 */

AMSG *cfn_amsg = 0;
char cnstnt_pkt1[]="00B002D0001U10UY";
char cnstnt_pkt2[]="00B001D0001U10UF";

/*
 * CFN bss space
 */
char cfn_linebuf[80];
ushort cfn_src_sock;

TENTRY table[18*259];
short cx_pn_index;
char cx_port_names[17];
uchar cx_port_active[4];	/* Booleans */

L1_ADDR cfn_brdcst;

char *cfn_hostaddr;

struct cxdata cxDATAblk[4];

L1_ADDR cfn_attnet;


