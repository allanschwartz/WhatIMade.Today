/*********************************************************************
 *
 *  $Header:$
 *
 *  NAME
 *	unstrip
 *
 *  DESCRIPTION
 *	add a namelist back into a stripped object
 *
 *  CONTENTS
 *
 *	line	Function Declarations
 *	----    ---------------------------------------------------------
 *	  38    main(argc, argv)
 *	 125    unstrip(symfile, objfile)
 *	 175    match_type(type)
 *	 198    patch_objfile(file)
 *	 238    syserr(msg)
 *	 246    fatal(reason)
 *
 *  HISTORY
 *
 *
 *  BUGS
 *
 *
 *  TODO
 *
 ********************************************************************/

# include <stdio.h>
# include <a.out.h>
# include <stab.h>
# include <string.h>

extern void exit();
extern int  errno;
int         verbose = {0};
char       *progname;


main(argc, argv)
    int         argc;
    char      **argv;
{
    char       *rindex();
    extern char *optarg;
    extern int  optind;
    register int c, errflg = {0};
    char       *ofname = {NULL};
    char       *ifname = {NULL};
    FILE       *objfile = {NULL};
    FILE       *symfile = {NULL};


    /* Initialization */
    if ((progname = rindex(*argv, '/')) == NULL)
	progname = *argv;
    else
	progname++;

    /* Argument Processing */
    while ((c = getopt(argc, argv, "vo:")) != EOF) {
	switch (c) {
	    case 'v':
		verbose = 1;
		break;
	    case 'o':		/* object file name */
		if ((ofname = optarg) == NULL) 
		    fatal("Missing object file name\n");
		break;
	    case '?':
		errflg++;
	}
    }

    for (;optind < argc; optind++) {

	/* process file name */
	if (ifname == NULL) {
	    ifname = argv[optind];
	}
	else {
	    fprintf(stderr,
		    "Duplicate input filenames: keeping %s, ignoring %s\n",
		    ifname, argv[optind]);
	}
    }

    if (errflg) {
	printf("Usage: %s -v -o objfile symbol_file\n", progname);
	exit(2);
    }

    /* Open output (object) file */
    if (ofname == NULL)
	fatal("missing object file name");
    if ((objfile = fopen(ofname, "r+")) == NULL) 
	syserr(ofname);

    /* Open input (namelist) file */
    if (ifname == NULL)
	fatal("missing namelist file name");
    if ((symfile = fopen(ifname, "r")) == NULL) 
	syserr(ifname);
    
    unstrip(symfile, objfile);

    return 0;
}


#define MAX_SYMBOLS	2000
struct nlist namelist[MAX_SYMBOLS];
struct nlist *p_namelist = namelist;
struct nlist *p_end_namelist = &namelist[MAX_SYMBOLS - 1];

#define STR_SPACE_SIZE	MAX_SYMBOLS*16
char        string_space[STR_SPACE_SIZE] = {0};
char       *p_str_space = &string_space[4];
char       *p_end_str_space = &string_space[STR_SPACE_SIZE - 1];


unstrip(symfile, objfile)
    FILE       *symfile;
    FILE       *objfile;
{
    char        buf[512], name[64], type[16];
    long        address;
    int         len, rc;

    /*
     * read symbol table one line at a time, into a global structure
     */
    while (fgets(buf, sizeof( buf ), symfile) != NULL) {
	if (buf[0] == '\0' || buf[0] == '#')
	    continue;		/* these are not the droids you want... */

	rc = sscanf(buf, "%x %s %s", &address, type, name);
	if (rc != 3) {
	    printf("unrecognized line \"%s\" ignored.\n", buf);
	    continue;
	}
	if( verbose)
	    printf( "address = 0x%06x, type = %c, name = %s\n", 
	    address, type[0], name);

	/* add this name to the string space */
	len = strlen(name);
	if (p_str_space + len + 1 > p_end_str_space)
	    fatal("out of string space");

	(void)strcpy(p_str_space, name);
	p_namelist->n_un.n_strx = p_str_space - string_space;

	p_namelist->n_type = match_type(type[0]) | N_EXT;
	p_namelist->n_desc = N_GSYM;
	p_namelist->n_value = address;
	p_namelist++;
	if (p_namelist > p_end_namelist) {
	    fatal("out of namelist space");
	}
	p_str_space += (len + 1);
    }

    /*
     * now glue this table to the object file
     */
    patch_objfile( objfile );
}


/* lets figure out what kind of symbol this is */
match_type(type)
    char        type;
{
    int n_type;

    switch (type) {
	case 'T':
	case 't':
	    n_type = N_TEXT;
	    break;
	case 'D':
	case 'd':
	    n_type = N_DATA;
	    break;
	case 'B':
	case 'b':
	    n_type = N_BSS;
	    break;
	default:
	    printf("type %c unrecognized\n", type);
	    n_type = N_ABS;
    }
    return n_type;
}


patch_objfile(file)
    FILE       *file;
{
    struct exec a_out;
    long        stringoffset,
                namelistoffset;
    int         len,
                rc;

    /* read existing a.out header */
    (void) fseek(file, 0L, 0);
    rc = fread((char *) &a_out, sizeof a_out, 1, file);
    if (rc == 0)
	syserr("fread a.out");


    /* write namelist */
    a_out.a_syms = ((char *) p_namelist - (char *) &namelist[0]);
    namelistoffset = N_SYMOFF(a_out);
    (void) fseek(file, namelistoffset, 0);
    rc = fwrite((char *) &namelist[0], (int) a_out.a_syms, 1, file);
    if (rc == 0)
	syserr("fwrite namelist");

    /* write string table */
    len = p_str_space - string_space;
    stringoffset = N_STROFF(a_out);
    {
	int        *p;
	p = (int *)&string_space[0];
	*p = len;
    }

    (void) fseek(file, stringoffset, 0);
    rc = fwrite(&string_space[0], len, 1, file);
    if (rc == 0)
	syserr("fwrite string_space");

    /* write a.out header */
    (void) fseek(file, 0L, 0);
    rc = fwrite((char *) &a_out, sizeof a_out, 1, file);
    if (rc == 0)
	syserr("fwrite a.out");

}


syserr(msg)
    char *msg;
{
    perror(msg);
    fatal("system call error");
}


fatal(reason)
    char *reason;
{
    printf("%s: FATAL %s\n", progname, reason);
    exit(1);
}
