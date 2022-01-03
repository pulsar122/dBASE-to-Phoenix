/*
 * mini demo for the libdbf.a, converts a dbf to an ascii flat file
 * rasca, berlin 1996
 */

#include <stdlib.h> /* free() */
#include "dbf.h"    /* dbOpen(), dbStrNext(), dbClose() */

int main (int argc, char **argv) {
	int db;
	UCHAR *s;

	if (argc > 1) {
		if ((db = dbOpen (argv[1], DB_READ | DB_IGNORE_MEMO)) > 0) {
			while ((s = dbTrStrNext(db, ';')) != NULL) {
				printf ("%s\n", s);
				free (s);
			}
			dbClose (db);
		}
	} else {
		printf ("Usage: mini <xbase_file>\n");
	}
	return (0);
}

