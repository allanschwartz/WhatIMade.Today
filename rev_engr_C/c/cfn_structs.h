#define TENTRY struct tentry
struct tentry
{
    char t_name[5];				/* 0 */
    char t_inuse;				/* 5 */
    L1_ADDR t_addr;				/* 6 */
}; 


#define CXDATA struct cxdata
struct cxdata
{
    short cx_mailboxdepth;	/* == 10 */	/* 0 */
    short cx_state;		/* 0 | 1 | 2 */	/* 2 */
	/* 0 
	 * 1 CONNECTED
	 * 2 DISCONNECTED
	 */
    short cx_index;				/* 4 */
    short cx_data6;				/* 6 */
    short cx_data8;				/* 8 */
    short cx_datastate;				/* 0xa */
    ushort cx_sock;				/* 0xc */
    MBID cx_sacmbox;	/* from listenmsg */	/* 0xe */
    MBID cx_sadmbox;	/* from connect msg */	/* 0x12 */
    MBID cx_mailbox1;				/* 0x16 */
    MBID cx_mbid;				/* 0x1a */
    MBID cx_mailbox2;				/* 0x1e */
    char cx_databuf[513];			/* 0x22 */
    char cx_pad;				/* 0x223 */
    char *cx_dataptr;				/* 0x224 */

};


#define PSPPKT struct psppkt
struct psppkt
{
    char byte0;					/* 0 */
    char byte1;					/* 1 */
    char byte2;					/* 2 */
    char psp_name[4];				/* 3 */
    char psp_name2[4];				/* 7 */
    char byteb;					/* 0xb */
    char bytec;					/* 0xc */
    char byted;					/* 0xd */
    char bytee;					/* 0xe */
    char bytef;					/* 0xf */
    char *psp_save_ba;				/* 0x10 */
    char byte14;				/* 0x14 */
    uchar psp_cksum;				/* 0x15 */
};
