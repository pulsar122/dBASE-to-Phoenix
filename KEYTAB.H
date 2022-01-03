/******************************************************************************/
/*																										*/
/*									KeyTab - Zeichensatzumwandlungen							*/
/*																										*/
/* Include-Datei zur Bibliothek, die die Anbindung an KEYTAB						*/
/* von Pure-C und GNU-C aus erledigt														*/
/*																										*/
/* Dieses Programm ist incl. des Sources Freeware!										*/
/*																										*/
/* Include-File of the library, which implements the connection to KEYTAB		*/
/* from Pure-C and GNU-C																		*/
/*																										*/
/* This Program is incl. the Source Freeware!											*/
/*																										*/
/* Copyright (c)	1997-2001 by Thomas Much, thomas@snailshell.de					*/
/* 					     2002 by Martin ElsÑsser, Martin_Elsaesser@ll.maus.de	*/
/******************************************************************************/

#ifndef __KEYTAB_LIB__
#define __KEYTAB_LIB__

/******************************************************************************/
/*																										*/
/* Globale Datentypen																			*/
/*																										*/
/*	Global Typedefs																				*/
/*																										*/
/******************************************************************************/

/* In KEYTAB.H evtl. definiert - in KEYTAB.H perhaps defined */
#ifndef KEYTAB_H
#define KEYTAB_H
	
	typedef struct
	{
		long  magic;
		long  size;
		short cdecl (*GetExpMaxNr)(void);
		long  resvd0;
		short cdecl (*GetExpNrFromId)(short id);
		char  cdecl (*CharAtari2X)(short nr, char chr);
		short cdecl (*GetImpMaxNr)(void);
		long  resvd1;
		short cdecl (*GetImpNrFromId)(short id);
		char  cdecl (*CharX2Atari)(short nr, char ichr);
		void  cdecl (*BlockAtari2X)(short nr, long len, char *chrs);
		void  cdecl (*BlockX2Atari)(short nr, long len, char *chrs);
		char *cdecl (*GetExpNameFromNr)(short nr);
		char *cdecl (*GetImpNameFromNr)(short nr);
		char *cdecl (*GetExpShortNameFromNr)(short nr);
		char *cdecl (*GetImpShortNameFromNr)(short nr);
		short cdecl (*GetRelease)(void);
		short cdecl (*GetExpIdFromNr)(short nr);
		short cdecl (*GetImpIdFromNr)(short nr);
		short cdecl (*GetExpNrFromName)(char *name);
		short cdecl (*GetImpNrFromName)(char *name);
		long  cdecl (*GetEuro)(void);
		short cdecl (*CharX2Unicode)(short nr, char chr);
		char  cdecl (*CharUnicode2X)(short nr, short unicode);
		void  cdecl (*BlockX2Unicode)( short *unicode, short nr, char *x_chrs, long len );
		void  cdecl (*BlockUnicode2X)( char *x_chrs, short nr, short *unicode, long len );
		short cdecl (*GetExpMinNr)(void);
		short cdecl (*GetImpMinNr)(void);
		long  cdecl (*CharXUtf2Unicode)( short nr, char *string, long len, short *read_chars );
		char *cdecl (*CharUnicode2XUtf)( short nr, long lunicode, char *result, short *used_chars );
		void cdecl (*BlockXUtf2Unicode)( short *unicode, long *uni_len, short nr, char *string, long len );
		void cdecl (*BlockUnicode2XUtf)( char *string, long *str_len, short nr, short *unicode, long len );
		void cdecl (*BlockXUtf2U2XUtf)( char *dest, long *dest_len, short dest_nr, char *source, long src_len, short src_nr );
		void cdecl (*BlockXUtf2XUtf)( char *dest, long *dest_len, short dest_nr, char *source, long src_len, short src_nr );
	} KEYT;
	
#endif

/******************************************************************************/
/*																										*/
/* Funktions-Prototypen																			*/
/*																										*/
/* Prototypes of the Functions																*/
/*																										*/
/******************************************************************************/

/* Funktionen rund um den CookieJar - Functions around the CookieJar   */
short Ash_getcookie( long cookie, void *value );
KEYT *Akt_getKeyTab( void );
short Akt_getRelease( void );

/******************************************************************************/

/* Auskunfts-Funktionen Åber Filter - Info-Functions about the Filters */
short Akt_getExpMaxNr( void );
short Akt_getImpMaxNr( void );
char *Akt_getExpNameFromNr( short nr );
char *Akt_getImpNameFromNr( short nr );
char *Akt_getExpShortNameFromNr( short nr );
char *Akt_getImpShortNameFromNr( short nr );
short Akt_getExpNrFromId( short eid );
short Akt_getImpNrFromId( short iid );
short Akt_getExpIdFromNr( short enr );
short Akt_getImpIdFromNr( short inr );
short Akt_getExpNrFromName( char *name );
short Akt_getImpNrFromName( char *name );

/******************************************************************************/

/* Konvertieren von Zeichen und Strings - Conversion of Chars and Strings */
char Akt_CharAtari2X( short nr, char ch );
char Akt_CharX2Atari( short nr, char ch );
short Akt_CharX2Unicode( short nr, char ch );
char Akt_CharUnicode2X( short nr, short unicode );
long Akt_CharXUtf2Unicode( short nr, char *string, long len, short *read_chars );
char *Akt_CharUnicode2XUtf( short nr, long lunicode, char *result, short *used_chars );

char *Akt_BlockAtari2X( char *dest_string, short nr, char *source_string, long len );
char *Akt_BlockX2Atari( char *dest_string, short nr, char *source_string, long len );
short *Akt_BlockX2Unicode( short *unicode, short nr, char *x_chrs, long len );
char *Akt_BlockUnicode2X( char *x_chrs, short nr, short *unicode, long len );
short *Akt_BlockXUtf2Unicode( short *unicode, long *uni_len, short nr, char *x_chrs, long len );
char *Akt_BlockUnicode2XUtf( char *x_chrs, long *str_len, short nr, short *unicode, long len );
char *Akt_BlockXUtf2U2XUtf( char *dest, long *dest_len, short dest_nr, char *source, long src_len, short src_nr );
char *Akt_BlockXUtf2XUtf( char *dest, long *dest_len, short dest_nr, char *source, long src_len, short src_nr );

char *Akt_StringAtari2X( char *dest_string, short nr, char *source_string );
char *Akt_StringX2Atari( char *dest_string, short nr, char *source_string );
short *Akt_StringX2Unicode( short *unicode, short nr, char *string );
short *Akt_StringXUtf2Unicode( short *unicode, long *uni_len, short nr, char *string );
char *Akt_StringXUtf2U2XUtf( char *dest, long *dest_len, short dest_nr, char *source, short src_nr );
char *Akt_StringXUtf2XUtf( char *dest, long *dest_len, short dest_nr, char *source, short src_nr );

/******************************************************************************/

/* Das Euro-Zeichen und Infos Åber Euro - The Euro-Sign and Infos about Euro */
long Akt_getEuro( void );

/******************************************************************************/

#endif
