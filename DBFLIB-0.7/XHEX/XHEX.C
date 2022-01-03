/*
 * this file belongs to 'xhex' and is published under the
 * GNU General Public License (GPL), see COPYING for details.
 *
 * (c) rasca gmelch, berlin 1994, 1995, 1996
 */

#include <stdio.h>
#include <getopt.h>
#include <libsx.h>
#include "xhex.h"
#include "actions.h"
#include "msg.h"

void usage (char *pname) {
	fprintf (stderr, "Usage: %s [-?XODAB] [-F file]\n", pname);
	fprintf (stderr, " -X: force hex mode\n");
	fprintf (stderr, " -O: force octal mode\n");
	fprintf (stderr, " -D: force decimal mode\n");
	fprintf (stderr, " -A: force ascii mode\n");
#ifdef XBASE_SUPPORT
	fprintf (stderr, " -B: force dBase mode\n");
	fprintf (stderr, " -I: ignore deleted dBase records\n");
#endif
	exit (1);
}

int init_display (int argc, char **argv, Program *program) {
	static Widget w[7];
	static Widget m[5];
	static Widget twin;
	int bg_color, frame_color;

	if ((argc > 1) && (*argv[1] == '-') && (*(argv[1]+1) == '?'))
		return (-1);
	if ((argc = OpenDisplay(argc, argv)) == FALSE)
		return(0);

	w[0] = MakeMenu(M_FILE);
	w[1] = MakeMenuItem(w[0], M_LOAD,	mLoad,   program);
	w[2] = MakeMenuItem(w[0], M_SAVE,	mSaveAs, program);
	w[3] = MakeMenuItem(w[0], M_QUIT,	mQuit,	 program);

	w[4] = MakeMenu("Format");
	/* the items */
	m[0] = MakeMenuItem(w[4], M_HEX,	mHex,	program);
	m[1] = MakeMenuItem(w[4], M_DEC,	mDec,	program);
	m[2] = MakeMenuItem(w[4], M_OCT,	mOct,	program);
	m[3] = MakeMenuItem(w[4], "8bit ASCII",		mAscii, program);
#ifdef XBASE_SUPPORT
	m[4] = MakeMenuItem(w[4], "xBase",	mDbase,	program);
#endif
	w[5] = MakeButton(B_ABOUT,	about,	program);

	w[6] = MakeLabel(L_NO_FILE);
	twin = MakeTextWidget(NULL, TRUE, FALSE, X_SIZE, Y_SIZE);

	program->widget_file = w[6];
	program->widget_text = twin;
	program->widget_menu = m[program->mode];
	SetMenuItemChecked(m[program->mode], TRUE);

	SetWidgetPos(w[4], PLACE_RIGHT, w[0], NO_CARE, NULL);
	SetWidgetPos(w[5], PLACE_RIGHT, w[4], NO_CARE, NULL);
	SetWidgetPos(w[6], PLACE_RIGHT, w[5], NO_CARE, NULL);
	SetWidgetPos(twin, PLACE_UNDER, w[0], PLACE_UNDER, w[4]);
	ShowDisplay();

	bg_color	= GetNamedColor(BUTTON_BG_COLOR);
	frame_color = GetNamedColor(BUTTON_FR_COLOR);
	SetBgColor(w[0], bg_color);
	SetBgColor(w[4], bg_color);
	SetBgColor(w[5], bg_color);
	SetBgColor(w[6], bg_color);
	SetBorderColor(w[0], frame_color);
	SetBorderColor(w[4], frame_color);
	SetBorderColor(w[5], frame_color);
	SetBorderColor(w[6], frame_color);
	return (argc);
}


int main(int argc, char *argv[]) {
	Program program;
	int c;

	program.widget_file = NULL;
	program.widget_text = NULL;
	program.widget_menu = NULL;
	program.buffer = NULL;
	program.file_name = NULL;
	program.file_size = 0;
	program.mode = DEFAULT_MODE;
	program.flag = INIT;
	program.igndel = FALSE;

	if ((argc = init_display (argc, argv, &program)) == FALSE) {
		fprintf (stderr, "error in 'OpenDisplay()': argc: %d\n", argc);
		exit (1);
	}
	if (argc == -1)
		usage(argv[0]);
	while ((c = getopt(argc, argv, "?hF:XODABI")) != -1) {
		switch (c) {
			case 'F':	program.file_name = strdup (optarg);
						break;
			case 'X':	program.mode = HEX;
						break;
			case 'O':	program.mode = OCT;
						break;
			case 'D':	program.mode = DEC;
						break;
			case 'A':	program.mode = ASCII;
						break;
#ifdef XBASE_SUPPORT
			case 'B':	program.mode = DBASE;
						break;
			case 'I':	program.igndel = TRUE;
						break;
#endif
			case 'h':
			case '?':	usage(argv[0]);
						break;
			default:	break;
		}
	}

	if (program.file_name) {
		mLoad(NULL, &program);
	}
	program.flag = RUNNING;
	MainLoop();
	return (0);
}

