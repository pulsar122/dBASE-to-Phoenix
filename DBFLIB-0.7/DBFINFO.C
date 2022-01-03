/*
 * just for testing ..
 * dump some informations about a xbase file to stdout
 *
 * rasca, berlin 1995, 1996
 */
#include "dbf.h"

int main (int argc, char *argv[]) {
	int db;
	int i, max;

	if (argc < 2) {
		printf ("Usage: %s <file.dbf>\n", argv[0]);
		return (1);
	}
	if ((db = dbOpen (argv[1], DB_READ)) > 0 ) {
		dbDumpHeader (db);
		max = dbGetNumOfFields(db);
		printf ("         Fieldname: Fieldtype\n");
		for (i = 0; i < max; i++)
			printf ("      %12s: %c (length: %3d, dec: %d)\n",
				dbGetFieldName(db,i+1), dbGetFieldType(db,i+1),
				dbGetFieldLength(db,i+1), dbGetFieldDec(db, i+1));
		dbClose (db);
	}
	return (0);
}

