#
#	m	f

/^./	{ caller[$1] = caller[$1]  ", " $2
	  called[$2] = called[$2]  ", " $1
	}

END	{ for (i in caller)
	    printf ( "%s ->:	%s\n", i, caller[i] )
	  for (i in called)
	    printf ( "%s <-:	%s\n", i, called[i] )
	}

