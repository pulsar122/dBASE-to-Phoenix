/*
 * this file belongs to 'xhex' and is published under the
 * GNU General Public License (GPL), see COPYING for details.
 *
 * (c) rasca gmelch, berlin 1994, 1995, 1996
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#include <libsx.h>
#include "xhex.h"
#include "actions.h"
#include "msg.h"
#include "freq.h"
#ifdef XBASE_SUPPORT
#include "dbf.h"
#endif

void switch_to_mode (int mode, Widget menu_item, void *data) {
	Program *prog = (Program *) data;

	if (prog->widget_menu != menu_item) {
		SetMenuItemChecked(prog->widget_menu, FALSE);
		SetMenuItemChecked(menu_item, TRUE);
		prog->widget_menu = menu_item;
	}
	prog->mode = mode;
	fill_buff(data);
}


int isumlaut (int c) {
	switch (c) {
	case 'ö': case 'Ö': case 'ü': case 'Ü': case 'Ä': case 'ä': case 'ß':
				return (1);
				break;
	default:	return (0);
	}
}

void mHex (Widget w, void *data) {
	Program *prog = (Program *) data;
	if (prog->mode != HEX)
		switch_to_mode (HEX, w, data);
}

void mOct (Widget w, void *data) {
	Program *prog = (Program *) data;
	if (prog->mode != OCT)
		switch_to_mode (OCT, w, data);
}

void mDec (Widget w, void *data) {
	Program *prog = (Program *) data;
	if (prog->mode != DEC)
		switch_to_mode (DEC, w, data);
}

void mAscii (Widget w, void *data) {
	Program *prog = (Program *) data;
	if (prog->mode != ASCII)
		switch_to_mode (ASCII, w, data);
}

#ifdef XBASE_SUPPORT
void mDbase (Widget w, void *data) {
	Program *prog = (Program *) data;
	if (prog->mode != DBASE)
		switch_to_mode (DBASE, w, data);
}

char *dbase_to_tab (char *fname, int igndel) {
	char *txt, *str;
	UCHAR *fldn;
	int handle;
	int base, len;
	int max, i;
	int line_len;

	base = handle = len = 0;

	handle = dbOpen (fname, DB_READ | DB_IGNORE_MEMO);
	if (handle > 0) {
		dbSetShowDel (handle, !igndel);

		line_len = 0;
		max = dbGetNumOfFields (handle);
		for (i = 0; i < max; i++) {
			if ((len = dbGetFieldLength (handle, i+1)) < MAX_NAME_LEN) {
				line_len += MAX_NAME_LEN;
			} else {
				line_len += len;
			}
		}
		txt = (char *) malloc (line_len + max +1);
		for (i = 0; i < max; i++) {
			fldn = dbGetExFieldName (handle, i+1);
			#ifdef DEBUG
			if (fldn == NULL)
				fprintf (stderr,"null pointer!!!\n");
			#endif
			len = strlen((const char *)fldn);
			#ifdef DEBUG
			fprintf (stderr, "*** field length = %d\n", len);
			#endif
			memcpy (txt+base, fldn, len);
			base += len;
			if (i < max-1) {
				memcpy (txt+base, " ", 2);
			} else {
				memcpy (txt+base, "\n", 2);
			}
			base++;
			free (fldn);
		}
		while ((str = (char *) dbStrNext (handle, '|')) != NULL) {
			if (*str != '\0') {
				len = strlen (str) +1;
#ifdef DEBUG2
				fprintf (stderr, "dbase_to_tab(): %d\n", len);
#endif
				txt = (char *) realloc (txt, base + len + 2);
				memcpy (txt+base, str, len);
				*(txt+base+len-1) = '\n';
				base += len;
			}
			free (str);
		}
		*(txt+base) = '\0';
		dbClose (handle);
#ifdef DEBUG2
				fprintf (stderr, "dbase_to_tab(): base = %d\n", base);
#endif
#ifdef DEBUG2
		fprintf (stderr, "dbase_to_tab(): base: %d\n", base);
		fprintf (stderr, "dbase_to_tab(): strlen: %d\n", strlen (txt));
#endif
		return (txt);
	} else {
		len = strlen (MSG_NO_DBASE_FILE);
		txt = (char *) malloc (len+2);
		strncpy (txt, MSG_NO_DBASE_FILE, len+1);
		return (txt);
	}
}
#endif

char *to_ascii (FILE *fp, int size) {
	int input = ASCII_WIDTH;
	int pos = 0;
	int i, c;
	char *txt, *top;
	u_char line[ASCII_WIDTH];
	
	txt = (char*) malloc(size + (size/ASCII_WIDTH)*2); /* oops */
	if (txt == NULL) {
		fprintf(stderr, "to_ascii(): can't alloc enough memory!");
		exit (2);
	}
	top = txt;
	while (input > ASCII_WIDTH -1) {
		input = fread (line, 1, ASCII_WIDTH, fp);
		if (input > 0) {
			for (i = 0; i < input; i++) {
				pos++;
				c = (int) line[i];
				if (line[i] == '\n') {
					pos = 0;
					*txt = line[i];
				} else if ((line[i] == '\r') || (line[i] == '\f')) {
					txt--;
				} else if (isprint(c) || (line[i] == '\t')
							|| isumlaut(c)) {
					*txt = line[i];
				} else {
					*txt = '.';
				}
				txt++;
			if (pos >= ASCII_WIDTH) {
				pos = 0;
				*txt = '\n';
				txt++;
			}
			}
		}
	}
	*txt = '\0';
	return (top);
}

char *to_oct (FILE *fp, int size) {
	int inlen = OCT_WIDTH;
	u_char line[OCT_WIDTH];
	char *txt, *top;
	int i, lnum = 0, c;

	txt = (char*) malloc ((size*4)+((size/OCT_WIDTH)*(13+3+OCT_WIDTH+1))+2);
	top = txt;
	if (txt == NULL) {
		fprintf(stderr, "to_oct(): can't alloc enough memory!");
		exit (2);
	}
	while (inlen > OCT_WIDTH-1) {
		inlen = fread (line, 1, OCT_WIDTH, fp);
		if (inlen > 0) {
			sprintf (txt, "%08d:   ", lnum);
			lnum += OCT_WIDTH;
			txt += 12;
			for (i=0; i < inlen; i++) {
				sprintf (txt, "%03o ", line[i]);
				txt += 4;
			}
			for (i=inlen; i < OCT_WIDTH; i++) {
				sprintf (txt, "    ");
				txt += 4;
			}
			*(txt++) = ' ';
			*(txt++) = ' ';
			for (i=0; i < inlen; i++) {
				c = (int) line[i];
				if (isprint(c) || isumlaut(c)) {
					*txt = line[i];
				} else {
					*txt = '.';
				}
				txt++;
			}
			*txt = '\n';
			txt++;
		}
	}
	*txt = '\0';
	return (top);
}

char *to_dec (FILE *fp, int size) {
	int inlen = DEC_WIDTH ;
	u_char line[DEC_WIDTH];
	char *txt, *top;
	int i, lnum = 0, c;

	txt = (char*) malloc ((size*4)+((size/DEC_WIDTH)*(13+3+DEC_WIDTH+1))+2);
	top = txt;
	if (txt == NULL) {
		fprintf(stderr, "to_oct(): can't alloc enough memory!");
		exit (2);
	}
	while (inlen > DEC_WIDTH-1) {
		inlen = fread (line, 1, DEC_WIDTH, fp);
		if (inlen > 0) {
			sprintf (txt, "%08d:   ", lnum);
			lnum += DEC_WIDTH;
			txt += 12;
			for (i=0; i < inlen; i++) {
				sprintf (txt, "%03d ", line[i]);
				txt += 4;
			}
			for (i=inlen; i < DEC_WIDTH; i++) {
				sprintf (txt, "    ");
				txt += 4;
			}
			*(txt++) = ' ';
			*(txt++) = ' ';
			for (i=0; i < inlen; i++) {
				c = (int) line[i];
				if (isprint(c) || isumlaut(c)) {
					*txt = line[i];
				} else {
					*txt = '.';
				}
				txt++;
			}
			*txt = '\n';
			txt++;
		}
	}
	*txt = '\0';
	return (top);
}

char *to_hex (FILE *fp, int size) {
	int inlen = HEX_WIDTH;
	u_char line[HEX_WIDTH];
	char *txt;
	char *top;
	int i, lnum = 0, c;

	txt = (char*) malloc ((size*3)+((size/HEX_WIDTH)*(13+3+HEX_WIDTH+1))+2);
	top = txt;
	if (txt == NULL) {
		fprintf(stderr, "to_hex(): can't alloc enough memory!");
		exit (2);
	}
	while (inlen > HEX_WIDTH-1) {
		inlen = fread (line, 1, HEX_WIDTH, fp);
		if (inlen > 0) {
			sprintf (txt, "%08d:   ", lnum);
			lnum += HEX_WIDTH;
			txt += 12;
			for (i=0; i < inlen; i++) {
				sprintf (txt, "%02X ", line[i]);
				txt += 3;
			}
			for (i = inlen; i < HEX_WIDTH; i++) {
				sprintf (txt, "   ");
				txt += 3;
			}
			*(txt++) = ' ';
			*(txt++) = ' ';
			for (i=0; i < inlen; i++) {
				c = (int) line[i];
				if (isprint(c) || isumlaut(c)) {
					*txt = line[i];
				} else {
					*txt = '.';
				}
				txt++;
			}
			*txt = '\n';
			txt++;
		}
	}
	*txt = '\0';
	return (top);
}


void fill_buff (void *data) {
	Program *prog = (Program *) data;
	FILE *fp;
	char *fname;
	char *buff;
	int size, len;
	int mode;

	fname = prog->file_name;
	size = prog->file_size;
	mode = prog->mode;

	if (fname) {
		SetLabel(prog->widget_file, MSG_PLEASE_WAIT);
		SyncDisplay();
		fp = fopen(fname, "r");
		if (fp != NULL) {
			switch (mode) {
				case HEX:	buff = to_hex (fp, size);
							fclose (fp);
							break;
				case OCT:	buff = to_oct (fp, size);
							fclose (fp);
							break;
				case DEC:	buff = to_dec (fp, size);
							fclose (fp);
							break;
				case ASCII:	buff = to_ascii (fp, size);
							fclose (fp);
							break;
#ifdef XBASE_SUPPORT
				case DBASE:	fclose (fp);
							buff = dbase_to_tab (fname, prog->igndel);
							break;
#endif
				default:	len = strlen (MSG_UNKNOWN_MODE);
							buff = (char*) malloc (len+2);
							strncpy (buff, MSG_UNKNOWN_MODE, len+1);
							fclose (fp);
							break;
			}
		} else {
			len = strlen (MSG_CANT_OPEN_FILE);
			buff = (char*) malloc (len+2);
			strncpy (buff, MSG_CANT_OPEN_FILE, len+1);
			perror(fname);
		}
		prog->buffer = buff;
		SetTextWidgetText(prog->widget_text, buff, FALSE);
		SetLabel(prog->widget_file, fname);
		free(buff);
#ifdef DEBUG2
		fprintf (stderr, "fill_buff(): ended\n");
#endif
	}
}


void mLoad (Widget w, void *data) {
	Program *prog = (Program *) data;
	char *fname = prog->file_name;
	struct stat info;

	if (prog->flag != INIT) {
		fname = GetFile(NULL);
	}
	if (fname) {
		stat (fname, &info);
		if (info.st_size > 0) {
			prog->file_size = info.st_size;
			if (prog->flag != INIT)
				free(prog->file_name);
			prog->file_name = strdup(fname);
			free (fname);
			fill_buff (prog);
		} else {
			GetYesNo("\nfile is empty!\n\n");
		}
	}
}

void mSaveAs (Widget w, void *data) {
	Program *prog = (Program *) data;
	char *fname;
	char *txt;
	FILE *fp;

	if (prog->file_name == NULL)
		return;

	fname = GetFile(NULL);
	if (fname) {
		fp = fopen (fname, "w");
		if (fp != NULL) {
			txt = GetTextWidgetText(prog->widget_text);
			if ((txt == NULL) || (txt == '\0')) {
				GetYesNo("buffer is empty!");
			} else {
				while (*txt != '\0') {
					fwrite(txt, 1, 1, fp);
					txt++;
				}
			}
			fclose(fp);
		} else {
			perror (fname);
			GetYesNo("error: can't save file ..");
		}
	}
}


void about(Widget w, void *data) {
	Program *prog = (Program *) data;
	char text[512];

	sprintf (text,	"\nX hexdump -=<#|#>=- version " VERSION "\n"
					" published under the 'GNU GPL'\n\n"
					"(c) rasca gmelch,   berlin 1995\n"
					"rasca@marie.physik.tu-berlin.de\n\n"
					MSG_FILE ": %s\n"
					MSG_SIZE ": %d bytes\n\n",
					prog->file_name, prog->file_size);
	GetYesNo(text);
}
 

void mQuit(Widget w, void *data) {
	/* clean up */
	exit(0);
}

