
#include <stdio.h>

#define PROGNAME	"offset"


main(argc, argv)
    int         argc;
    char      **argv;
{
    extern char *optarg;
    extern int  optind;
    register int c;
    int         errflg = 0;
    char        name[32];
    int         n,
                offset;
    char type;

    while ((c = getopt(argc, argv, "o:")) != EOF) {
	switch (c) {
	    case 'o':		/* offset */
		offset = strtol(optarg,0,16);
		break;
	    case '?':
		errflg++;
	}
    }

    if (argc > optind || errflg) {
	printf("Usage: %s -o offset\n", PROGNAME);
	exit(2);
    }


    while (scanf("%x %c %s", &n, &type, name) == 3 ) {
	printf("%06x (%06x) %c %s\n", offset + n, n, type, name);
    }
    exit (0 );
}
