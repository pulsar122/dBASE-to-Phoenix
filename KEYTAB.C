/*--------------------------------------------------------------------------*/
/* Routinen zuur Behandlung von KEYTAB																			*/

#include <portab.h>
#include <string.h>
#include <sys_gem2.h>
#include "db2ph.h"
#include "global.h"
#include "key.h"
#include "rsc.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/


/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

WORD keytab_id_import;
WORD keytab_id_export;
KEYT *keytab;																/* Variable fr Keytab-Cookie		*/

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

#define KEYTAB_C 		52

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

LOCAL WORD handle_keytab(WORD msg,WORD button,DIALOG_INFO *inf);

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

/*--------------------------------------------------------------------------*/
/* Importfilter ausw„hlten																									*/

VOID import_keytab(VOID)
{
	keytab = Akt_getKeyTab ();
	if( keytab != NULL && keytab->size >= KEYTAB_C)
  		WindowDialog('KEYT',-1,-1,HoleText(TEXTE,TEXT_021,NULL),"",FALSE,FALSE,keytab_tree,NULL,0,NULL,handle_keytab);
  else
  {
  	Note(ALERT_NORM,1,KEYTAB_NICHT_DA);
  	keytab_id_import=-1;
  }
}

/*--------------------------------------------------------------------------*/
/* Bearbeiten der Reaktion auf den Auswahldialog beim GEDCOM Export					*/

WORD handle_keytab(WORD msg,WORD button,DIALOG_INFO *inf)
{
	BYTE ZStr[500];
	BYTE *s;
	WORD Anzahl,i;
	static WORD key_alt_import, key_alt_export;
	
	switch(msg)
	{
		case SG_HELP:													/* Help-Taste bet„tigt.						*/
 			CallOnlineHelp("");
 		break;
 		case SG_START:
 			key_alt_import=keytab_id_import;		/* Name des Importfilter ermitteln*/
			if(keytab_id_import==-1)
				keytab_id_import=0;
			Anzahl=Akt_getImpMaxNr();
			for(i=0; i<=Anzahl; i++)
			{
				s=Akt_getImpNameFromNr(i);
				if(s!=NULL && i==keytab_id_import)
				{
					strcpy(ZStr,s);
					break;
				}
			}
			SetText(inf->tree, KEYTABLIST_IMPORT, ZStr);

 			key_alt_export=keytab_id_export;		/* Name des Exportfilter ermitteln*/
			if(keytab_id_export==-1)
				keytab_id_export=0;
			Anzahl=Akt_getExpMaxNr();
			for(i=0; i<=Anzahl; i++)
			{
				s=Akt_getExpNameFromNr(i);
				if(s!=NULL && i==keytab_id_export)
				{
					strcpy(ZStr,s);
					break;
				}
			}
			SetText(inf->tree,KEYTABLIST_EXPORT,ZStr);

 		break;
    case SG_UNDO:													/* Undo-Taste bet„tigt						*/
			keytab_id_import=key_alt_import;		/* alten Zustand wiederherstellen	*/
			keytab_id_export=key_alt_export;		/* alten Zustand wiederherstellen	*/
			return SG_CLOSE;
		case SG_END:
  		switch(button)
  		{
  			case KEYTAB_OK:
					SetConfig("Keytab Import",&keytab_id_import,2);
					SetConfig("Keytab Export",&keytab_id_export,2);
  			break;
  			case KEYTAB_SETZEN:
  				key_alt_import = keytab_id_import;		/* Neue Einstellung merken	*/
					SetConfig("Keytab Import",&keytab_id_import,2);

  				key_alt_export = keytab_id_export;		/* Neue Einstellung merken	*/
					SetConfig("Keytab Export",&keytab_id_export,2);
  				return SG_CONT;

  			case KEYTAB_ABBRUCH:
  				keytab_id_import=key_alt_import;/* alten Zustand wiederherstellen	*/
					keytab_id_export=key_alt_export;/* alten Zustand wiederherstellen	*/
  			break;

  			case KEYTABLIST_IMPORT:
					Anzahl=Akt_getImpMaxNr();
					ZStr[0]=EOS;
					for(i=0; i<=Anzahl; i++)
					{
						s=Akt_getImpNameFromNr(i);
						strcat(ZStr,s);
						strcat(ZStr,"|");
					}
					ZStr[strlen(ZStr)-1]=EOS;
					i=Listbox(ZStr,-1,-1,inf->tree,KEYTABLIST_IMPORT);
					if(i!=-1)
						keytab_id_import=i;
					return SG_CONT;

  			case KEYTABLIST_EXPORT:
					Anzahl=Akt_getExpMaxNr();
					ZStr[0]=EOS;
					for(i=0; i<=Anzahl; i++)
					{
						s=Akt_getExpNameFromNr(i);
						strcat(ZStr,s);
						strcat(ZStr,"|");
					}
					ZStr[strlen(ZStr)-1]=EOS;
					i=Listbox(ZStr,-1,-1,inf->tree,KEYTABLIST_EXPORT);
					if(i!=-1)
						keytab_id_export=i;
					return SG_CONT;
  		}
			return SG_CLOSE;
		case SG_POSX:
			SetObjX(inf->tree,0,button);				/* Wegen SysGem-Bug								*/
		break;
		case SG_POSY:
			SetObjY(inf->tree,0,button);				/* Wegen SysGem-Bug								*/
		break;
	}
	return SG_CONT;
}

/*--------------------------------------------------------------------------*/
/* Keytab init																															*/

VOID init_keytab(VOID)
{
	if(!GetConfig("Keytab Import",&keytab_id_import))
	{
		keytab_id_import=4;
		SetConfig("Keytab Import",&keytab_id_import,2);
	}
	if(!GetConfig("Keytab Export",&keytab_id_export))
	{
		keytab_id_export=4;
		SetConfig("Keytab Export",&keytab_id_export,2);
	}
	keytab = Akt_getKeyTab ();
	if( keytab == NULL )
	{
		keytab_id_import=-1;									/* Keytab ist nicht vorhanden			*/
		keytab_id_export=-1;									/* Keytab ist nicht vorhanden			*/
	}
}
