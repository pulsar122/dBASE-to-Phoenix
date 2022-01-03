/*--------------------------------------------------------------------------*/
/* Routinen zuur Behandlung von KEYTAB																			*/

#include <portab.h>
#include <string.h>
#include <sys_gem2.h>
#include <keytab.h>
#include "db2ph.h"
#include "global.h"
#include "key.h"
#include "rsc.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/


/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

WORD keytab_id_import;
KEYT *keytab;																/* Variable fr Keytab-Cookie		*/

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

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
	if(GetCookie(KEYTAB_COOKIE,(LONG *)&keytab) && keytab->size>=KEYTAB_C)
  		WindowDialog('KEYT',-1,-1,"Importfilter","",FALSE,FALSE,keytab_tree,NULL,0,NULL,handle_keytab);
  else
  {
  	Alert(ALERT_NORM,1,"[3][Keytab ist nicht oder in der|falschen Version vorhanden][[OK]");
  	keytab_id_import=-1;
  }
}

/*--------------------------------------------------------------------------*/
/* Bearbeiten der Reaktion auf den Auswahldialog beim GEDCOM Export					*/

WORD handle_keytab(WORD msg,WORD button,DIALOG_INFO *inf)
{
	BYTE ZStr[200];
	BYTE *s;
	WORD Anzahl,i;
	static WORD key_alt;
	
	switch(msg)
	{
		case SG_HELP:													/* Help-Taste bet„tigt.						*/
 			CallOnlineHelp("GEDCOM Import");
 		break;
 		case SG_START:
 			key_alt=keytab_id_import;
			if(keytab_id_import==-1)
				keytab_id_import=0;
			Anzahl=GetImportCount();
			for(i=0; i<=Anzahl; i++)
			{
				s=GetImportName(i);
				if(s!=NULL && i==keytab_id_import)
				{
					strcpy(ZStr,s);
					break;
				}
			}
			SetText(inf->tree,KEYTABLIST,ZStr);
 		break;
    case SG_UNDO:													/* Undo-Taste bet„tigt						*/
			keytab_id_import=key_alt;						/* alten Zustand wiederherstel		*/
			return SG_CLOSE;
		case SG_END:
  		switch(button)
  		{
  			case KEYTABIMPOK:
					SetConfig("Keytab Import",&keytab_id_import,2);
  			break;
  			case KEYTABIMABBRUCH:
  				keytab_id_import=key_alt;				/* alten Zustand wiederherstel		*/
  			break;
  			case KEYTABLIST:
					Anzahl=GetImportCount();
					ZStr[0]=EOS;
					for(i=0; i<=Anzahl; i++)
					{
						s=GetImportName(i);
						strcat(ZStr,s);
						strcat(ZStr,"|");
					}
					ZStr[strlen(ZStr)-1]=EOS;
					i=Listbox(ZStr,-1,-1,inf->tree,KEYTABLIST);
					if(i!=-1)
						keytab_id_import=i;
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
	if(!GetCookie(KEYTAB_COOKIE,(LONG *)&keytab) && keytab->size<KEYTAB_C)
		keytab_id_import=-1;									/* Keytab ist nicht vorhanden			*/
}

