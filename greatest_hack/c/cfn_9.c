#include "cfn.h"

int         cfn_ascii_to_hex(p)
    REG uchar   *p;		/* a6@(8) & a5 */

{
    REG int     sum;		/* d7 */
    REG int     digit;		/* d6 */

    sum = 0;

    for (; *p; p++) {

	switch (*p) {

	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	    case '8':
	    case '9':
		digit = *p - '0';
		break;

	    case 'a':
	    case 'b':
	    case 'c':
	    case 'd':
	    case 'e':
	    case 'f':
		digit = *p - 'a' + 10;
		break;

	    case 'A':
	    case 'B':
	    case 'C':
	    case 'D':
	    case 'E':
	    case 'F':
		digit = *p - 'A' + 10;
		break;

	    default:
		return -1;
	}
	sum <<= 4;
	sum += digit;
    }
    return sum;
}
