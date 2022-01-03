/*
 * this file belongs to 'xhex' and is published under the
 * GNU General Public License (GPL), see COPYING for details.
 *
 * (c) rasca gmelch, berlin 1994, 1995, 1996
 */
 
/* HEX, OCT, DEC, ASCII
 */
#define DEFAULT_MODE ASCII

#ifdef XBASE_SUPPORT
	#define XBASE_DEF_SEP	'\t'
#endif

/* default window size
 */
#define X_SIZE 580
#define Y_SIZE 400

#define VERSION "0.8"

#define ASCII_WIDTH 80
#define   HEX_WIDTH 16
#define   OCT_WIDTH 12
#define   DEC_WIDTH 12

#define BUTTON_BG_COLOR	"grey70"
#define BUTTON_FR_COLOR	"blue"

typedef struct _Program {
	Widget widget_text;
	Widget widget_file;
	Widget widget_menu;
	char *buffer;
	char *file_name;
	int file_size;
	int mode;
	int flag;
	int igndel;
} Program;

enum {
	HEX,
	DEC,
	OCT,
	ASCII,
	DBASE
};

enum {
	INIT,
	RUNNING
};

