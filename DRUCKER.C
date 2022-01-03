/*--------------------------------------------------------------------------*/
/* Alles Rund um den Drucker																								*/

#include <stdio.h>
#include <portab.h>
#include <stdlib.h>
#include <string.h>
#include <sys_gem2.h>
#include <aes.h>
#include <tos.h>
#include "rsc.h"
#include "drucker.h"
#include "db2ph.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/

/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

WORD wdialog;

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

#define	PRINT_FLAGS	 /* PDLG_ALWAYS_ORIENT + PDLG_ALWAYS_COPIES + PDLG_EVENODD*/

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

typedef struct							/* Dialogbeschreibung fÅr create_sub_dialogs() 	*/
{
	PDLG_INIT	init_dlg;
	PDLG_HNDL	do_dlg;
	PDLG_RESET reset_dlg;
	WORD icon_index;
/*	OBJECT *tree_index;*/
} SIMPLE_SUB;

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

LOCAL LONG cdecl pinit_special(PRN_SETTINGS *settings, PDLG_SUB *sub_dialog);
LOCAL LONG cdecl psub_special(PRN_SETTINGS *settings,PDLG_SUB *sub_dialog,WORD exit_obj);
LOCAL LONG cdecl preset_special( PRN_SETTINGS *settings, PDLG_SUB *sub_dialog);

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

LOCAL PRN_DIALOG *gprn_dialog;
LOCAL PRN_SETTINGS *gprn_settings;

SIMPLE_SUB	my_subs_for_pdlg[2] =		/* Feld mit Dialogbeschreib. fÅr create_sub_dialogs() */
{
	  pinit_special, psub_special, preset_special, WD_01, /*wdialog_tree ,*/
	  pinit_special, psub_special, preset_special, WD_01/*, wdialog_tree*/
};

/*--------------------------------------------------------------------------*/ 
/* Unterdialog initialisieren																								*/
/* Funktionsergebnis:	0: Fehler 1: alles in Ordnung													*/
/* settings:	 Druckereinstellung																						*/
/* sub_dialog: Zeiger auf die Unterdialog-Struktur													*/

LONG cdecl pinit_special(PRN_SETTINGS *settings, PDLG_SUB *sub_dialog)
{
	WORD offset;
	OBJECT *tree;

	tree = sub_dialog->tree;									/* Zeiger auf den Objektbaum 		*/
	offset = sub_dialog->index_offset;				/* Offset fÅr die Objektindizes */

	return 1;
}

/*--------------------------------------------------------------------------*/ 
/* Buttons im Unterdialog behandeln																					*/
/* Funktionsergebnis:	0: Fehler 1: alles in Ordnung	PDLG_BUT_DEV: GerÑt wechseln			*/
/* settings:    			Druckereinstellung																		*/
/* sub_dialog:				Zeiger auf die Unterdialog-Struktur										*/
/* exit_obj:					Objektnummer																					*/

LONG cdecl psub_special(PRN_SETTINGS *settings,PDLG_SUB *sub_dialog,WORD exit_obj )
{
	WORD offset;
	OBJECT *tree;

	tree = sub_dialog->tree;									/* Zeiger auf den Objektbaum 		*/
	offset = sub_dialog->index_offset;				/* Offset fÅr die Objektindizes */

	switch ( exit_obj - sub_dialog->index_offset )
	{
/*	
		case	MYSP_PUSH_ME1:												/* Beispielbutton */
		{
			/* der einzige "aktive" Button in diesem Unterdialog */
			/* invertiert den Status eines Icons */
		
			tree[PUSH_ME_ICON1 + offset].ob_state ^= SELECTED;	/* Selektion eines Icons Ñndern */
			redraw_obj( sub_dialog, PUSH_ME_ICON1 + offset );
			
			obj_DESELECTED( tree, exit_obj );						/* Button wieder normal */
			redraw_obj( sub_dialog, exit_obj );
			break;
		}
*/
	}
	return 1;
}

/*--------------------------------------------------------------------------*/ 
/* Felder im Unterdialog auslesen bevor der Unterdialog gewechselt wird			*/
/* Funktionsergebnis:	0: Fehler 1: alles in Ordnung													*/
/* settings:					Druckereinstellung																		*/
/* sub_dialog:				Zeiger auf die Unterdialog-Struktur										*/

LONG cdecl preset_special(PRN_SETTINGS *settings, PDLG_SUB *sub_dialog)
{
	OBJECT	*tree;
	int16		offset;
	
	tree = sub_dialog->tree;
	offset = sub_dialog->index_offset;
	
	/* Objektstati auswerten */

	return 1;
}

/*--------------------------------------------------------------------------*/ 
/* Liste fÅr die Unterdialoge aufbauen																			*/
/* Funktionsresultat:	Zeiger auf Liste																			*/
/*	tree_addr:				Feld mit Objektbaumadressen														*/
/*	create:																																	*/
/*	no_subs:					Anzahl der Unterdialoge																*/

PDLG_SUB	*create_sub_dialogs(OBJECT **tree_addr,SIMPLE_SUB *create, 
					WORD no_subs)
{
	PDLG_SUB *sub_dialogs;
	WORD	i;

	sub_dialogs = 0L;
	for(i=0; i<no_subs; i++)
	{
		PDLG_SUB	*sub;

		sub=malloc(sizeof(PDLG_SUB ));
		if(sub)
		{
			sub->next = 0L;													/* Zeiger auf den Nachfolger 	*/

			sub->option_flags = 0;
			sub->sub_id = -1;												/* Kennung des Unterdialogs (wird von pdlg_add_sub_dialogs() gesetzt) */
			sub->sub_icon = icon_tree[create->icon_index];
			sub->sub_tree = create->tree_index;			/* Zeiger auf den Objektbaum des Unterdialogs */

			sub->dialog = 0L;												/* Zeiger auf die Struktur des Fensterdialogs oder 0L */
			sub->tree = 0L;													/* Zeiger auf den zusammengesetzen Objektbaum */
			sub->index_offset = 0;									/* Indexverschiebung des Unterdialogs */
			sub->reserved1 = 0;
			sub->reserved2 = 0;

			sub->init_dlg = create->init_dlg;				/* Initialisierungsfunktion 	*/
			sub->do_dlg = create->do_dlg;						/* Behandlungsfunktion 				*/
			sub->reset_dlg = create->reset_dlg;			/* ZurÅcksetzfunktion 				*/
			sub->reserved3 = 0;
			
			sub->next = sub_dialogs;
			sub_dialogs = sub;
		}
		create++;																	/* Zeiger auf die nÑchste Beschreibung */
	}
	return sub_dialogs;	
}

/*--------------------------------------------------------------------------*/ 
/* Gespeicherte Druckereinstellung zurÅckliefern														*/
/* Funktionsresultat:	Zeiger auf Druckereinstellung oder 0L									*/
/* prn_dialog				:	Zeiger auf Verwaltungsstruktur												*/
/*										(wenn dauerhaft geîffnet) oder 0L											*/

PRN_SETTINGS	*read_psettings(PRN_DIALOG *prn_dialog)
{
	PRN_DIALOG		*p;
	PRN_SETTINGS	*settings;

	settings = 0L;

	if(prn_dialog == 0L )										/* noch nicht offen? 							*/
	{
		p = pdlg_create( 0 );									/* kurzzeitig îffnen 							*/
		if(p==0L) 
			return 0L;
		prn_dialog = p;
	}
	else
		p = 0L;

#if 0	
	
	Wenn das Programm die Einstellungen in einer Datei gespeichert hÑtte,
	sollte der folgende Code benutzt werden:
	
	settings = read_settings_from_file();		/* Einstellungen aus programmeigener Datei holen */
	if(settings)
	{
		if(pdlg_validate_settings(prn_dialog,settings) == 0 )	/* schwerwiegender Fehler? */
		{
			Mfree(settings);
			settings = 0L;
		}		
	}
#endif
	if(settings==0L)
	{
		settings=Malloc(pdlg_get_setsize());	/* Speicherbereich anfordern 			*/
		if(settings)
			pdlg_dflt_settings(prn_dialog,settings);	/* und initialisieren 			*/
	}
	if(p)																		/* nur kurzzeitig geîffnet? 			*/
		pdlg_delete( p );
	return settings;
}

/*--------------------------------------------------------------------------*/ 
/* Druckereinstellung speichern																							*/
/* Funktionsresultat:	1: alles in Ordnung																		*/
/* settings					:	Druckereinstellung																		*/

WORD save_psettings(PRN_SETTINGS *settings)
{
	if(settings)
	{
		#if 0
	
		Wenn das Programm die Einstellungen in einer Datei speichern wÅrde,
		sollte der folgende Code benutzt werden:
		
		write_settings_to_file( settings );		/* Druckereinstellung speichern 	*/
		
		#endif
		Mfree( settings );										/* Speicher freigeben 						*/
		return 1;
	}
	return 0 ;
}

/*--------------------------------------------------------------------------*/
/* Druckdialog anzeigen																											*/
/* Funktionsresultat:	0, PDLG_CANCEL oder PDLG_OK														*/
/* document_name		:	Dokumentenname																				*/
/* kind							:	PDLG_PREFS: Einstelldialog PDLG_PRINT: Druckdialog		*/

WORD do_print_dialog(BYTE *document_name,WORD kind)
{
	WORD	button;

	gprn_dialog=pdlg_create(PDLG_3D);	 /* Speicher anfordern, Treiber scannen */
	if(gprn_dialog)
	{
		sub_dialogs = create_sub_dialogs( tree_addr, my_subs_for_pdlg, 1 );
		if(sub_dialogs)
			pdlg_add_sub_dialogs(gprn_dialog, sub_dialogs );	/* Unterdialoge hinzufÅgen */
		button=pdlg_do(gprn_dialog,gprn_settings,document_name,PRINT_FLAGS + kind);
		pdlg_delete(gprn_dialog);							/* Speicher freigeben 						*/
		gprn_dialog=0L;
		return button;
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
/* Init Drucker																															*/

VOID init_drucker(VOID)
{
	WORD ag1,ag2,ag3,ag4;

	wdialog=FALSE;
	if(appl_find("?AGI")==0)								/* appl_getinfo() vorhanden? 			*/
	{
		if(appl_getinfo(7,&ag1,&ag2,&ag3,&ag4))	/* Unterfunktion 7 aufrufen 		*/
		{
			if((ag1 & 0x17 )==0x17)							/* wdlg_xx()/lbox_xx()/fnts_xx()/pdlg_xx() vorhanden? */
				wdialog=TRUE;
		}	
	}
	if(wdialog)
	{
		gprn_settings=read_psettings(0L);			/* Druckereinstellung einlesen		*/
	}
}
