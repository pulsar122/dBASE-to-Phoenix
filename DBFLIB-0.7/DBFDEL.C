/*
 * dbfdel 0.1, delete a record in a xbase file
 * published under the GNU General Public License, see COPYING
 *
 * rasca gmelch, berlin 1995, 1996
 * (rasca@marie.physik.tu-berlin.de)
 */

#include <stdlib.h>
#include <string.h>		/* strlen() */
#if defined (linux) || defined (unix)
	#include <getopt.h> /* getopt(), optarg */
#else
	/* MS-DOS */
	#include "getopt.h"
#endif
#include "dbf.h"

#define VERSION "0.1"

void usage (void) {
	fprintf (stderr, " DBFDEL, version %s, published under the GNU GPL\n", VERSION);
	fprintf (stderr, " Usage: dbfdel [-?] -r # [-n #] [-u] file\n");
	fprintf (stderr, " -u   : undelete instead of delete a record\n");
	fprintf (stderr, " -r # : record number to delete / undelte\n");
	fprintf (stderr, " -n # : number of records to delete (default = 1)\n");
	exit (1);
}

int main (int argc, char **argv) {

	int record_nr = 0;
	int undelete = 0;
	int number = 1;
	int c;
	char *file = NULL;
	int db;

	while ((c = getopt(argc, argv, "?hn:r:u")) != -1) {
		switch (c) {
			case '?':	
			case 'h':	usage();
						break;
			case 'u':	undelete = 1;
						break;
			case 'r':   record_nr = atoi (optarg);
						break;
			case 'n':   number = atoi (optarg);
						break;
			default:    usage();
						break;
		}
	}

	if ((argc > 1) && (optind == argc -1)) {
		file = argv[optind++];
	} else {
		usage();
	}
	if (1) {
		db = dbOpen (file, DB_WRITE | DB_IGNORE_MEMO);
	} else {
		db = dbOpen (file, DB_WRITE);
	}
	if (db > 0) {
		while (number) {
			if (undelete) {
				dbUndelRecord(db, record_nr);
			} else {
				dbDelRecord(db, record_nr);
			}
			number--;
			record_nr++;
		}
		dbClose (db);
	} else {
		printf ("error when calling open_dbf()\n");
	}
	return (0);
}

