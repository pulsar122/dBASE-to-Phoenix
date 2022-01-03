/*
 * hexdump 0.7a
 * this program is published under the GNU GPL, see COPYING for details ...
 *
 * rasca gmelch, berlin 1994, 1995, 1996
 * (rasca@marie.physik.tu-berlin.de)
 */

#include <stdio.h>
#if defined(linux) || defined(unix)
	#include <unistd.h>
	#include <getopt.h>
#else
	#include "getopt.h"
#endif
#include <ctype.h>
#include <stdlib.h>

#define LENGTH 16

enum {
	OCT,
	HEX,
	DEC,
	BIN
};
int Mode = HEX;
int Verbose =1;

int hexdump (unsigned char *line, int len, int inlen) {
	unsigned int i;

	for (i = 0; i < len; i++, line++) {
		if (i < inlen)
			printf ("%02X ", *line);
		else
			printf ("   ");
	}
	return (1);
}

int octdump (unsigned char *line, int len, int inlen) {
	unsigned int i;

	for (i = 0; i < len; i++, line++) {
		if (i < inlen)
			printf ("%03o ", *line);
		else
			printf ("    ");
	}
	return (1);
}

int decdump (unsigned char *line, int len, int inlen) {
	unsigned int i;

	for (i = 0; i < len; i++, line++) {
		if (i < inlen)
			printf ("%03d ", *line);
		else
			printf ("    ");
	}
	return (1);
}

int bindump (unsigned char *line, int len, int inlen) {
	int i;
	char s[10];

	s[8] = ' ';
	s[9] = '\0';

	for (i = 0; i < len; i++, line++) {
		if (i < inlen) {
			if (*line & 0x80) s[0] = '1';
			else s[0] = '0';
			if (*line & 0x40) s[1] = '1';
			else s[1] = '0';
			if (*line & 0x20) s[2] = '1';
			else s[2] = '0';
			if (*line & 0x10) s[3] = '1';
			else s[3] = '0';
			if (*line & 0x08) s[4] = '1';
			else s[4] = '0';
			if (*line & 0x04) s[5] = '1';
			else s[5] = '0';
			if (*line & 0x02) s[6] = '1';
			else s[6] = '0';
			if (*line & 0x01) s[7] = '1';
			else s[7] = '0';
			printf (s);
		} else {
			printf ("         ");
		}
	}
	return (1);
}

void ascii (unsigned char *line, int len) {
	int i;

	printf ("  ");
	for (i = 0; i < len; i++, line++) {
		if (isprint(*line) == 0) {
			putchar ('.');
		} else {
			putchar (*line);
		}
	}
}

void dump(char *line, unsigned int lnum, int len, int inlen) {
	if (Verbose)
		printf ("%08d:  ", lnum); 
	if (Mode == HEX)
		hexdump (line, len, inlen);
	if (Mode == OCT)
		octdump (line, len, inlen);
	if (Mode == DEC)
		decdump (line, len, inlen);
	if (Mode == BIN)
		bindump (line, len, inlen);
	if (Verbose)
		ascii (line, inlen);
	printf ("\n");
}

void usage(char *pn) {
	fprintf (stderr, "Usage: %s [-hodxb] [-w#] [files]\n", pn);
	fprintf (stderr, " o: octal\n");
	fprintf (stderr, " d: decimal\n");
	fprintf (stderr, " x: hex\n");
	fprintf (stderr, " b: binary\n");
	fprintf (stderr, " q: do not show address and ascii values\n");
	exit (1);
}

int main(int argc, char *argv[]) {
	char *pn;
	FILE *fp;
	unsigned char *line;
	unsigned int lnum = 0;
	int c, inlen;
	int width = 0;

	pn = argv[0];

	while ((c = getopt(argc, argv, "?bdhoxqw:")) != -1) {
		switch(c) {
			case 'd':	Mode = DEC;
						break;
			case 'o':	Mode = OCT;
						break;
			case 'x':	Mode = HEX;
						break;
			case 'b':	Mode = BIN;
						break;
			case 'w':	width = atoi(optarg);
						break;
			case 'q':	Verbose = 0;
						break;
			case '?':
			case 'h':
			default:	usage(pn);
						break;
		}
	}
	if (width == 0) {
		switch (Mode) {
			case DEC:	width = 12;
						break;
			case OCT:	width = 12;
						break;
			case BIN:	width = 6;
						break;
			case HEX:
			default:	width = LENGTH;
						break;
		}
	}
	line = (char *) malloc(width);
	if ((argc - optind) <= 0) {
		/* read from stdin */
		lnum = 0;
		inlen = width;
		while (inlen > width-1) {
			inlen = fread (line, 1, width, stdin);
			dump (line, lnum, width, inlen);
			lnum += width;
		}
	} else {
		/* read from files */
		while ((argc - optind)  > 0) {
			fp = fopen (argv[optind], "rb");
			if (fp != NULL) {
				lnum = 0;
				inlen = width;
				while (inlen > width-1) {
					inlen = fread (line, 1, width, fp);
					if (inlen > 0)
						dump (line, lnum, width, inlen);
					lnum += width;
				}
				fclose(fp);
			} else {
				perror (argv[optind]);
			}
			optind ++;
		}
	}
	return (0);	
}

