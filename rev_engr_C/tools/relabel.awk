#
#	relabel
#
BEGIN   { ln = 0; }
/beq|bge|bgt|bhi|ble|bls|blt|bne|bra/	{ label[$3] = 1; }
	{ line[ln] = $0; ln++; }
END	{ for ( i = 0; i < ln ; i++ ) {
	      lbl = substr(line[i], 0, index(line[i],":")-1)
	      if (label[lbl]) printf ("_")
	      print line[i]
	  }
	  print(":3,$s/^cfn[^ 	]*://")
	}

