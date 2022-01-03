/*--------------------------------------------------------------------------*/
/* dBASE Datenbank einlesen																									*/

#include <portab.h>
#include <import.h>
#include <nkcc.h>
#include <phoenix.h>
#include <stdlib.h>
#include <string.h>
#include <sys_gem2.h>
#include "copyrigh.h"
#include "db2ph.h"
#include "global.h"
#include "ph_base.h"
#include "ph_imp.h"
#include "make.h"
#include "main.h"
#include "rsc.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/

/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

BYTE ph_path[300];													/* Phoenix Path									*/
PhoenixStruct *phoenix_zeiger;
BYTE Datentypen[16][30];

/*= {"Text","Zahl","Langzahl","Flieûkomma",
										"Flieûkomma/Text","Datum","Zeit","Datum+Zeit",
										"Byte-Strom","Wort-Strom","Long-Strom","Grafik",
										"externe Datei","Datensatzadresse","Blob"};
*/
BYTE ph_name[50],ph_passwort[50];

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

#define WINDOW_FLAGS		NAME | CLOSER | FULLER | MOVER | SIZER | UPARROW | DNARROW | VSLIDE | LFARROW | RTARROW | HSLIDE | ICONIFIER

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

LOCAL VOID verdeckt(OBJECT *tree,LONG taste);
LOCAL VOID init_phoenix_table(OBJECT *tree,WORD table);
LOCAL VOID textbeschreibung(VOID);
LOCAL VOID set_menu_phoenix(WORD open);

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

BYTE *slider_phstruct;


/*--------------------------------------------------------------------------*/
/* ôffnet eine dBASE Datenbank																							*/

VOID open_phoenix(VOID)
{
	BYTE Name[30],datei[300];
	WORD i;
	
	strcpy(Name,"*.*");
	i=FileSelect(Name,ph_path,"*.DAT\0*.IND\0",HoleText(TEXTE,TEXT_022,NULL),datei);
	if(i)
	{
		xWindowDialog('PHLO',-1,-1,HoleText(TEXTE,TEXT_023,NULL),"",FALSE,TRUE,login_tree,NULL,-2,NULL,handle_login);
   	if(ph_passwort[0]==255)
   		return;
		phoenix_zeiger=(PhoenixStruct *)malloc(sizeof(PhoenixStruct));
		if(phoenix_zeiger==NULL)
			return;
		if(phoenix_open(phoenix_zeiger,datei,ph_name,ph_passwort))
			set_menu_phoenix(TRUE);
		else
			close_phoenix();
	}
}

/*--------------------------------------------------------------------------*/
/* Auswertung der Meldungen fÅr den Dialog Login														*/

WORD handle_login(WORD msg,WORD button,DIALOG_INFO *inf)
{
	WORD i;

	switch(msg)
	{
    case SG_START:
    	ClearEditFields(inf->tree);
    	ph_name[0]='\0';
    	ph_passwort[0]='\0';
		break;
    case SG_UNDO:																/* Undo-Taste betÑtigt			*/
    	ph_passwort[0]=255;
			return SG_CLOSE;
		case SG_HELP:																/* Help-Taste betÑtigt			*/
 			CallOnlineHelp(HoleText(ANLEITUNGS_TEXTE,HELP_005,NULL));
 		break;
		case SG_END:
			if(button==LOK)
				GetText(inf->tree,LBENUTZER,ph_name);
			else
	    	ph_passwort[0]=255;
			return SG_CLOSE;
		case SG_EDKEY:
			if(inf->edit_field==LPASSWORT)
			{
				verdeckt(inf->tree,inf->denied);
				i=(WORD)'*';
				return (( i << 8 ) | SG_TAKEKEY );
			}
		case SG_POSX:
			SetObjX(inf->tree,0,button);							/* Wegen SysGem-Bug					*/
		break;
		case SG_POSY:
			SetObjY(inf->tree,0,button);							/* Wegen SysGem-Bug					*/
		break;
	}
	return SG_CONT;
}

/*--------------------------------------------------------------------------*/
/* Verdeckte Eingabe																												*/

VOID verdeckt(OBJECT *tree,LONG taste)
{
	WORD key;
	LONG len;
	
	key=nkc_gconv((WORD)taste);
	key &=0xbfff;
	switch(key)
	{
		case NK_ESC | NKF_FUNC:
			ph_passwort[0]='\0';
			SetText(tree,LPASSWORT,"");
			RedrawObj(tree,LPASSWORT,1,0,UPD_STATE);
		break;
		case NK_BS | NKF_FUNC:
			len=strlen(ph_passwort);
			ph_passwort[len-1]='\0';
		break;
		default:
			key &=0xff;
			ph_passwort[len]=(BYTE)key;
			ph_passwort[len+1]='\0';
		break;
	}
}

/*--------------------------------------------------------------------------*/
/* Schlieût eine Phoenix Datenbank																					*/

VOID close_phoenix(VOID)
{
	if(phoenix_zeiger!=NULL)
	{
		phoenix_close(phoenix_zeiger);
		free(phoenix_zeiger);
		phoenix_zeiger=NULL;
		set_menu_phoenix(FALSE);
	}
}


/*--------------------------------------------------------------------------*/
/* Auswertung der Meldungen fÅr den Dialog Phoenix Struktur									*/

#define PHOENIX_MAX		60

BYTE *slider_phoenix_struct;

WORD handle_phoenix(WORD msg,WORD button,DIALOG_INFO *inf)
{
  BYTE ZStr[MAX_TABLENAME*MAX_TABLE + 1];
  WORD i;
  FORMAT format;

	switch(msg)
	{
    case SG_START:
		  build_format(TYPE_TIMESTAMP, "DD.MM.YYYY HH:MI:SS", format);
		  bin2str(TYPE_TIMESTAMP,&phoenix_zeiger->created,ZStr);
		  str2format(TYPE_TIMESTAMP,ZStr,format);
		  SetText(inf->tree,PHSTCREATED,ZStr);
		  bin2str(TYPE_TIMESTAMP,&phoenix_zeiger->lastuse,ZStr);
		  str2format(TYPE_TIMESTAMP,ZStr,format);
		  SetText(inf->tree,PHSTLASTUSE,ZStr);
		  SetText(inf->tree,PHSTTABELLE,&phoenix_zeiger->Table[0].Name[0]);
		  init_phoenix_table(inf->tree,0);
		break;
    case SG_UNDO:																/* Undo-Taste betÑtigt			*/
			return SG_CLOSE;
		case SG_HELP:																/* Help-Taste betÑtigt.			*/
 			CallOnlineHelp(HoleText(ANLEITUNGS_TEXTE,HELP_006,NULL));
 		break;
		case SG_END:
			switch(button)
			{
			}
			return SG_CLOSE;
		case SG_TOUCH:
			switch(button)
			{
				case PHSTTABELLE:
					ZStr[0]='\0';
					for(i=0; i<phoenix_zeiger->CountTable; i++)
					{
						strcat(ZStr,&phoenix_zeiger->Table[i].Name[0]);
						strcat(ZStr,"|");
					}
					ZStr[strlen(ZStr)-1]='\0';
					i=Listbox(ZStr,-1,-1,inf->tree,PHSTTABELLE);
					if(i!=-1)
					{
					  init_phoenix_table(inf->tree,i);
						RedrawObj(inf->tree,PHSTTABELLENBOX,8,0,UPD_STATE);
						RedrawSliderBox(inf->tree,PHSTBOX);
					}
				break;
			}
		break;
		case SG_QUIT:
			UnLinkSlider(inf->tree,PHSTBOX);
			free(slider_phoenix_struct);
			slider_phoenix_struct=NULL;
		break;
		case SG_MENU:
			switch(button)
			{
				case PPROGRAMMINFO:
			 		zeige_copyright(); 
				break;
				case PEXPORTSTRUKT:
					textbeschreibung();
				break;
/*
				case DDRUCKENSTRUKT:
					db_textbeschreibung_drucken();
				break;
*/	
			}
		break;
		case SG_POSX:
			SetObjX(inf->tree,0,button);							/* Wegen SysGem-Bug					*/
		break;
		case SG_POSY:
			SetObjY(inf->tree,0,button);							/* Wegen SysGem-Bug					*/
		break;
	}
	return SG_CONT;
}


/*--------------------------------------------------------------------------*/
/* Tabellendaten eintragen																									*/

VOID init_phoenix_table(OBJECT *tree,WORD table)
{
	BYTE ZStr[100];
	BYTE *H;
	WORD i;
	
	ltoa(phoenix_zeiger->Table[table].recs,ZStr,10);
	SetText(tree,PHSTRECS,ZStr);
	UnLinkSlider(tree,PHSTBOX);
	if(slider_phoenix_struct!=NULL)
		free(slider_phoenix_struct);
	slider_phoenix_struct=(BYTE *)malloc(phoenix_zeiger->Table[table].CountColumn*PHOENIX_MAX);
	H=slider_phoenix_struct;
	for(i=1; i<phoenix_zeiger->Table[table].CountColumn; i++)
	{
		strcpy(H,"\t");
		strcat(H,&phoenix_zeiger->Table[table].Column[i].Name[0]);
		strcat(H,"\t");
		if(phoenix_zeiger->Table[table].Column[i].Typ>=0 && phoenix_zeiger->Table[table].Column[i].Typ<=14)
			strcat(H,&Datentypen[phoenix_zeiger->Table[table].Column[i].Typ][0]);
		else
			strcat(H,HoleText(TEXTE,TEXT_015,NULL));
		strcat(H,"\t");
		if(phoenix_zeiger->Table[table].Column[i].Typ==TYPE_CHAR) /* Sonderbehandlung bei Zeichenkette */
			sprintf(ZStr,"%7li",phoenix_zeiger->Table[table].Column[i].Size-1);
		else
			sprintf(ZStr,"%7li",phoenix_zeiger->Table[table].Column[i].Size);
		strcat(H,ZStr);
		H +=PHOENIX_MAX;
	}
	LinkSlider(tree,PHSTUP,PHSTDOWN,PHSTSLIDER,PHSTSLIDERBOX,
						 phoenix_zeiger->Table[table].CountColumn-1,PHSTBOX,
						 slider_phoenix_struct,PHOENIX_MAX,FALSE);
	SetSliderTab(tree,PHSTBOX,0,TAB_LEFT);
	SetSliderTab(tree,PHSTBOX,25,TAB_LEFT);
	SetSliderTab(tree,PHSTBOX,43,TAB_LEFT);
}

/*--------------------------------------------------------------------------*/
/* Schreibt die Strukture in eine Datei																			*/

VOID textbeschreibung(VOID)
{
	BYTE Name[30],datei[300],ZStr[300];
	WORD i,j;
	FILE *fp;
  BASE_INFO info;
	LPBASE_INFO lpinfo;
	
	strcpy(Name,"*.*");
	i=FileSelect(Name,ph_path,"*.TXT\0",HoleText(TEXTE,TEXT_046,NULL),datei);
	if(!i)
		return;
	fp=fopen(datei,"wb+");
	if(fp==NULL)
	{
		Note(ALERT_NORM,1,DATEI_ERZEUGEN);
		return;
	}
	lpinfo=&info;
	db_baseinfo(phoenix_zeiger->Base,lpinfo);
	sprintf(ZStr,"Datenbank: %s\r\n\r\n",lpinfo->basename);
	fwrite(ZStr,1,strlen(ZStr),fp);
	for(i=0; i<phoenix_zeiger->CountTable; i++)
	{
		strcpy(ZStr,&phoenix_zeiger->Table[i].Name[0]);
		strcat(ZStr,"\r\n");
		fwrite(ZStr,1,strlen(ZStr),fp);
		for(j=1; j<phoenix_zeiger->Table[i].CountColumn; j++)
		{
			sprintf(ZStr,"  %-30.30s%-18.18s",&phoenix_zeiger->Table[i].Column[j].Name[0],
			                                &Datentypen[phoenix_zeiger->Table[i].Column[j].Typ][0]);
			if(phoenix_zeiger->Table[i].Column[j].Typ==TYPE_CHAR)
				sprintf(&ZStr[strlen(ZStr)],"(%li)",phoenix_zeiger->Table[i].Column[j].Size-1);
			strcat(ZStr,"\r\n");
			fwrite(ZStr,1,strlen(ZStr),fp);
		}
		strcpy(ZStr,"-------------------------------------------------------\r\n");
		fwrite(ZStr,1,strlen(ZStr),fp);
	}
	fclose(fp);
}

/*--------------------------------------------------------------------------*/
/* MenÅpunkte entsprechen setzen																						*/
/* open : TRUE, wenn eine Phoenix Datenbank geîffnet wurde									*/

VOID set_menu_phoenix(WORD open)
{
	menu_ienable(menu_tree,MOEFFNEPH,!open);
	menu_ienable(menu_tree,MPHCLOSE,open);
	menu_ienable(menu_tree,MPHSTRUKTUR,open);
	CloseWindowById('PHST');
	test_menu_import();
}

/*--------------------------------------------------------------------------*/
/* Init Phoenix-Import																											*/

VOID init_phoenix_Import(VOID)
{
	WORD i;

	phoenix_zeiger=NULL;
	slider_phoenix_struct=NULL;
	set_menu_phoenix(FALSE);

	for(i=0; i<15; i++)
		HoleText(TEXTE,TEXT_025+i,&Datentypen[i][0]);

}

/*--------------------------------------------------------------------------*/
/* Beendet Phoenix-Import																										*/

VOID term_phoenix_Import(VOID)
{
	close_phoenix();
}
