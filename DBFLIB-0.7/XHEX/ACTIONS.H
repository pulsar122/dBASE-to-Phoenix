/*
 * this file belongs to 'xhex' and is published under the
 * GNU General Public License (GPL), see COPYING for details.
 *
 * (c) rasca gmelch, berlin 1995, 1996
 */

void mQuit (Widget, void*);
void mLoad (Widget, void*);
void mSaveAs (Widget, void*);

void about (Widget, void*);
void fill_buff (void*);

void mOct (Widget, void*);
void mHex (Widget, void*);
void mDec (Widget, void*);
void mAscii (Widget, void*);
void mDbase (Widget, void*);

#ifdef _POSIX_SOURCE
	char *strdup (const char*);
#endif

