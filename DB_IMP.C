/*--------------------------------------------------------------------------*/
/* dBASE-Datei																															*/

#include <stdio.h>
#include <portab.h>
#include <stdlib.h>
#include <string.h>
#include <sys_gem2.h>
#include "copyrigh.h"
#include "dbase.h"
#include "db_imp.h"
#include "db2ph.h"
#include "drucker.h"
#include "global.h"
#include "make.h"
#include "rsc.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/

EXTERN PARAMETER Para;											/* Programmparameter						*/

/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

BYTE db_path[300];													/* dBASE Path										*/
DBStruct *dbase_zeiger;

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

#define WINDOW_FLAGS		NAME | CLOSER | FULLER | MOVER | SIZER | UPARROW | DNARROW | VSLIDE | LFARROW | RTARROW | HSLIDE | ICONIFIER
#define SLIDER_LEN 40

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

LOCAL VOID init_dbase_struktur(OBJECT *tree);
LOCAL VOID dbase_daten_zeile(DBStruct *d,BYTE *puffer);
LOCAL VOID db_textbeschreibung_export(VOID);
LOCAL VOID db_textbeschreibung_drucken(VOID);
LOCAL WORD handle_dbase_daten(WORD msg,WINDOW_INFO *inf);
LOCAL VOID set_menu_dbase(WORD open);

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

BYTE *slider_dbstruct;

/*--------------------------------------------------------------------------*/
/* ôffnet eine dBASE Datenbank																							*/

VOID open_dbase(VOID)
{
	BYTE Name[30],datei[300], str[40];
	WORD i;
	
	strcpy(Name,"Test");
	HoleText(TEXTE,TEXT_001,str);
	i=FileSelect(Name,db_path,"*.DBF",str,datei);
	if(i)
	{
		dbase_zeiger=(DBStruct *)malloc(sizeof(DBStruct));
		if(dbase_zeiger==NULL)
		{
			Note(ALERT_NORM,1,KEIN_SPEICHER);
			return;
		}
		ShowBee();
		if(dBase_open(dbase_zeiger,datei))
			set_menu_dbase(TRUE);
		else
			close_dbase();
		ShowArrow();
	}
}

/*--------------------------------------------------------------------------*/
/* Close eine dBASE Datenbank																								*/

VOID close_dbase(VOID)
{
	if(dbase_zeiger!=NULL)
	{
		dBase_close(dbase_zeiger);
		free(dbase_zeiger);
		dbase_zeiger=NULL;
		set_menu_dbase(FALSE);
	}
}

/*--------------------------------------------------------------------------*/
/* Auswertung der Meldungen fÅr den Dialog dBASE Struktur										*/

WORD handle_dbase(WORD msg,WORD button,DIALOG_INFO *inf)
{
	switch(msg)
	{
    case SG_START:
    	init_dbase_struktur(inf->tree);
    	DisableObj(dbase_menu_tree,DDRUCKENSTRUKT,0);
		break;
    case SG_UNDO:																/* Undo-Taste betÑtigt			*/
			return SG_CLOSE;
		case SG_HELP:																/* Help-Taste betÑtigt.			*/
 			CallOnlineHelp(HoleText(ANLEITUNGS_TEXTE,HELP_001,NULL));
 		break;
		case SG_END:
			return SG_CLOSE;
		case SG_QUIT:
			if(slider_dbstruct!=NULL)
			{
				free(slider_dbstruct);
				slider_dbstruct=NULL;
			}
			UnLinkSlider(inf->tree,DBSTBOX);
		break;
		case SG_MENU:
			switch(button)
			{
				case DPROGRAMMINFO:
			 		zeige_copyright(); 
				break;
				case DEXPORTSTRUKT:
					db_textbeschreibung_export();
				break;
				case DDRUCKENSTRUKT:
					db_textbeschreibung_drucken();
				break;
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

VOID init_dbase_struktur(OBJECT *tree)
{
	BYTE ZStr[100];
	BYTE *H;
	WORD i;
	
	switch((WORD)dbase_zeiger->typ)
	{
		case DBASEII:
			strcpy(ZStr,"dBASE II");
		break;
		case DBASEIII:
			strcpy(ZStr,"dBASE III");
		break;
		case DBASEIV:
			strcpy(ZStr,"dBASE IV");
		break;
		case DBASEV:
			strcpy(ZStr,"dBASE V");
		break;
		case DBASEIII_MEMO:
			HoleText(TEXTE,TEXT_002,ZStr);
		break;
		case DBASEIV_MEMO:
			HoleText(TEXTE,TEXT_003,ZStr);
		break;
		case DBASEIV_SQL:
			HoleText(TEXTE,TEXT_004,ZStr);
		break;
		case FOXPRO_MEMO:
			HoleText(TEXTE,TEXT_005,ZStr);
		break;
		default:
			HoleText(TEXTE,TEXT_006,ZStr);
		break;
	}
	SetText(tree,DBASETYP,ZStr);
	sprintf(ZStr,"%02d.%02d.%04d",(WORD)dbase_zeiger->Tag,(WORD)dbase_zeiger->Monat,(WORD)dbase_zeiger->Jahr);
	SetText(tree,DBLETZEAENDER,ZStr);
	sprintf(ZStr,"%d",dbase_zeiger->recsize);
	SetText(tree,DBSTGROESSE,ZStr);
	sprintf(ZStr,"%li",dbase_zeiger->recs);
	SetText(tree,DBSTANZAHL,ZStr);
	slider_dbstruct=(BYTE *)malloc(dbase_zeiger->fields*SLIDER_LEN);
	if(slider_dbstruct==NULL)
	{
		Note(ALERT_NORM,1,KEIN_SPEICHER);
		return;
	}
	memset(slider_dbstruct,0,dbase_zeiger->fields*SLIDER_LEN);
	H=slider_dbstruct;
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		strcpy(H,"\t");
		strcat(H,dbase_zeiger->field[i].Name);
		strcat(H,"\t");
		hole_typ(dbase_zeiger->field[i].Typ,ZStr);
		strcat(H,ZStr);
		strcat(H,"\t");
		sprintf(ZStr,"%5i",(WORD)dbase_zeiger->field[i].Size);
		strcat(H,ZStr);
		strcat(H,"\t");
		if(dbase_zeiger->field[i].Nk>0)
			sprintf(ZStr,"%3i",(WORD)dbase_zeiger->field[i].Nk);
		else
			strcpy(ZStr," ");
		strcat(H,ZStr);
		H +=SLIDER_LEN;
	}
	LinkSlider(tree,DBSTUP,DBSTDOWN,DBSTSLIDER,DBSTSLIDERBOX,dbase_zeiger->fields,DBSTBOX,slider_dbstruct,SLIDER_LEN,FALSE);
	SetSliderTab(tree,DBSTBOX,0,TAB_LEFT);
	SetSliderTab(tree,DBSTBOX,14,TAB_LEFT);
	SetSliderTab(tree,DBSTBOX,29,TAB_LEFT);
	SetSliderTab(tree,DBSTBOX,36,TAB_LEFT);
}

/*--------------------------------------------------------------------------*/
/* Kopiert den Datentyp als Klartext in einen String												*/

VOID hole_typ(BYTE Typ,BYTE *Text)
{
	switch(Typ)
	{
		case FIELD_ASCII:
			HoleText(TEXTE,TEXT_007,Text);
		break;
		case FIELD_NUM:
			HoleText(TEXTE,TEXT_008,Text);
		break;
		case FIELD_NUM2:
			HoleText(TEXTE,TEXT_009,Text);
		break;
		case FIELD_OBJEKT:
			HoleText(TEXTE,TEXT_010,Text);
		break;
		case FIELD_LOGIC:
			HoleText(TEXTE,TEXT_011,Text);
		break;
		case FIELD_DATE:
			HoleText(TEXTE,TEXT_012,Text);
		break;
		case FIELD_MEMO:
			HoleText(TEXTE,TEXT_013,Text);
		break;
		case FIELD_PICTURE:
			HoleText(TEXTE,TEXT_014,Text);
		break;
		default:
			HoleText(TEXTE,TEXT_015,Text);
		break;
	}
}

/*--------------------------------------------------------------------------*/
/* Schreibt die dBASE-Strukture in eine Datei																*/

VOID db_textbeschreibung_export(VOID)
{
	BYTE Name[30],datei[300],ZStr[300];
	WORD i;
	FILE *fp;
	
	strcpy(Name,"*.*");
	i=FileSelect(Name,db_path,"*.TXT\0",HoleText(TEXTE,TEXT_016,NULL),datei);
	if(!i)
		return;
	fp=fopen(datei,"wb+");
	if(fp==NULL)
	{
		Note(ALERT_NORM,1,DATEI_ERZEUGEN);
		return;
	}
	HoleText(TEXTE,TEXT_026,ZStr);
	switch((WORD)dbase_zeiger->typ)
	{
		case DBASEII:
			strcat(ZStr,"dBASE II");
		break;
		case DBASEIII:
			strcat(ZStr,"dBASE III");
		break;
		case DBASEIV:
			strcat(ZStr,"dBASE IV");
		break;
		case DBASEV:
			strcat(ZStr,"dBASE V");
		break;
		case DBASEIII_MEMO:
			strcat(ZStr,HoleText(TEXTE,TEXT_002,NULL));
		break;
		case DBASEIV_MEMO:
			strcat(ZStr,HoleText(TEXTE,TEXT_003,NULL));
		break;
		case DBASEIV_SQL:
			strcat(ZStr,HoleText(TEXTE,TEXT_004,NULL));
		break;
		case FOXPRO_MEMO:
			strcat(ZStr,HoleText(TEXTE,TEXT_005,NULL));
		break;
		default:
			strcat(ZStr,HoleText(TEXTE,TEXT_006,NULL));
		break;
	}
	strcat(ZStr,"\r\n");
	fwrite(ZStr,1,strlen(ZStr),fp);
	sprintf(ZStr,"%s%02d.%02d.%04d\r\n",HoleText(TEXTE,TEXT_017,NULL),(WORD)dbase_zeiger->Tag,(WORD)dbase_zeiger->Monat,(WORD)dbase_zeiger->Jahr);
	fwrite(ZStr,1,strlen(ZStr),fp);
	sprintf(ZStr,"%s%d\r\n",HoleText(TEXTE,TEXT_018,NULL),dbase_zeiger->recsize);
	fwrite(ZStr,1,strlen(ZStr),fp);
	sprintf(ZStr,"%s%li\r\n\r\n",HoleText(TEXTE,TEXT_019,NULL),dbase_zeiger->recs);
	fwrite(ZStr,1,strlen(ZStr),fp);
	strcpy(ZStr,HoleText(TEXTE,TEXT_027,NULL));
	strcat(ZStr,"\r\n");
	fwrite(ZStr,1,strlen(ZStr),fp);
	strcpy(ZStr,"---------------------------------------\r\n");
	fwrite(ZStr,1,strlen(ZStr),fp);
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		sprintf(ZStr,"  %-14.14s",dbase_zeiger->field[i].Name);
		hole_typ(dbase_zeiger->field[i].Typ,datei);
		sprintf(&ZStr[strlen(ZStr)],"%-12.12s",datei);
		sprintf(datei,"%5i  ",(WORD)dbase_zeiger->field[i].Size);
		strcat(ZStr,datei);
		if(dbase_zeiger->field[i].Nk>0)
			sprintf(datei,"%3i",(WORD)dbase_zeiger->field[i].Nk);
		else
			datei[0]='\0';
		strcat(ZStr,datei);
		strcat(ZStr,"\r\n");
		fwrite(ZStr,1,strlen(ZStr),fp);
	}
	fclose(fp);
}

/*--------------------------------------------------------------------------*/
/* Druckt die dBASE-Struktur																								*/

VOID db_textbeschreibung_drucken(VOID)
{
	BYTE Name[30],datei[300],ZStr[300];
	WORD i;

/*
	if(do_print_dialog("dBASE Strukture",PDLG_PRINT)==PDLG_OK)
	{
	/*	
		strcpy(ZStr,"Typ                  : ");
		switch((WORD)dbase_zeiger->typ)
		{
			case DBASEII:
				strcat(ZStr,"dBASE II");
			break;
			case DBASEIII:
				strcat(ZStr,"dBASE III");
			break;
			case DBASEIV:
				strcat(ZStr,"dBASE IV");
			break;
			case DBASEV:
				strcat(ZStr,"dBASE V");
			break;
			case DBASEIII_MEMO:
				strcat(ZStr,"dBASE III mit Memfeld");
			break;
			case DBASEIV_MEMO:
				strcat(ZStr,"dBASE IV mit Memfeld");
			break;
			case DBASEIV_SQL:
				strcat(ZStr,"dBASE IV mit SQL-Tabelle");
			break;
			case FOXPRO_MEMO:
				strcat(ZStr,"FoxPro mit Memofeld");
			break;
			default:
				strcat(ZStr,"unbekannte Version");
			break;
		}
		strcat(ZStr,"\r\n");
		fwrite(ZStr,1,strlen(ZStr),fp);
		sprintf(ZStr,"letzte énderung      : %02d.%02d.%04d\r\n",(WORD)dbase_zeiger->Tag,(WORD)dbase_zeiger->Monat,(WORD)dbase_zeiger->Jahr);
		fwrite(ZStr,1,strlen(ZStr),fp);
		sprintf(ZStr,"Datensatzgrîûe       : %d\r\n",dbase_zeiger->recsize);
		fwrite(ZStr,1,strlen(ZStr),fp);
		sprintf(ZStr,"Anzahl der DatensÑtze: %li\r\n\r\n",dbase_zeiger->recs);
		fwrite(ZStr,1,strlen(ZStr),fp);
		strcpy(ZStr,"  Feldname      Feldtyp     LÑnge   Dez\r\n");
		fwrite(ZStr,1,strlen(ZStr),fp);
		strcpy(ZStr,"---------------------------------------\r\n");
		fwrite(ZStr,1,strlen(ZStr),fp);
		for(i=0; i<dbase_zeiger->fields; i++)
		{
			sprintf(ZStr,"  %-14.14s",dbase_zeiger->field[i].Name);
			hole_typ(dbase_zeiger->field[i].Typ,datei);
			sprintf(&ZStr[strlen(ZStr)],"%-12.12s",datei);
			sprintf(datei,"%5i  ",(WORD)dbase_zeiger->field[i].Size);
			strcat(ZStr,datei);
			if(dbase_zeiger->field[i].Nk>0)
				sprintf(datei,"%3i",(WORD)dbase_zeiger->field[i].Nk);
			else
				datei[0]='\0';
			strcat(ZStr,datei);
			strcat(ZStr,"\r\n");
			fwrite(ZStr,1,strlen(ZStr),fp);
		}
		fclose(fp);
	*/
	}

*/
}

/*--------------------------------------------------------------------------*/
/* ôffnet ein Fenster mit den Daten der dBASE Datenbank											*/

VOID dbase_daten_anzeigen(VOID)
{
	BYTE *puffer;
	WORD offset;
	WORD handle;
	ULONG i;
	
	if((handle=GetHandle('DBDA'))!=-1)				/* Fenster schon vorhanden			*/
	{
		TopWindow(handle);											/* Dann nach vorne bringen			*/
		return;
	}
	puffer=(BYTE *)malloc(dbase_zeiger->recsize+50);
	if(puffer==NULL)
	{
		Note(ALERT_NORM,1,KEIN_SPEICHER);
		return;
	}
	ShowRotor();	
	OpenWindow('DBDA',HoleText(TEXTE,TEXT_020,NULL),"",WINDOW_FLAGS,NULL,Para.charw,TRUE,-1,-1,
						 1L,1L,10,10,500,400,NULL,(RPROC)NULL,handle_dbase_daten);
	dBase_move(dbase_zeiger,1);
	dbase_daten_zeile(dbase_zeiger,puffer);
	BeginListUpdate('DBDA');
	LinkList('DBDA',puffer);
	for(i=2; i<=dbase_zeiger->recs; i++)
	{
		dBase_move_plus(dbase_zeiger);
		dbase_daten_zeile(dbase_zeiger,puffer);
		AddToList('DBDA',puffer);
		UpdateRotor();
	}
	EndRotor();
	ShowBee();
	offset=2;
	SetListTab('DBDA',offset,TAB_LEFT);
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		SetListTab('DBDA',offset,TAB_LEFT);
		offset +=(dbase_zeiger->field[i].Size+1);
	}
	EndListUpdate('DBDA');
	CheckWindow('DBDA');
	free(puffer);	
	ShowArrow();
}

/*--------------------------------------------------------------------------*/

VOID dbase_daten_zeile(DBStruct *d,BYTE *puffer)
{
	BYTE ZStr[260];
	WORD i;

	if(dBase_deletet(d))
		strcpy(puffer,"\t*");
	else
		strcpy(puffer,"\t ");
	for(i=0; i<d->fields; i++)
	{
		strcat(puffer,"\t");
		strcat(puffer,"|");
		dBase_read(d,d->field[i].Name,ZStr);
		strcat(puffer,ZStr);
	}
}

/*--------------------------------------------------------------------------*/
/* Handel fÅr die Bearbeitung des dBASE Datenfenster												*/

WORD handle_dbase_daten(WORD msg,WINDOW_INFO *inf)
{
  switch(msg)
  {
    case SG_START:
    break;
		case SG_HELP:																/* Help-Taste betÑtigt.				*/
 			CallOnlineHelp(HoleText(ANLEITUNGS_TEXTE,HELP_002,NULL));
 		break;
  	case SG_END:
    	return SG_CLOSE;
    case SG_KEY:
    	inf->key &=0xbfff;
    	switch(inf->key)
      {
      	case 0x800C:													/* Clr/Home											*/
      		ScrollWindow(inf->handle,SCROLL_HOME);
          return(SG_KEYUSED);
      	case 0x810C:													/* Shift + Clr/Home							*/
      	case 0x820C:													/* Shift + Clr/Home							*/
      		ScrollWindow(inf->handle,SCROLL_SHIFT_HOME);
          return(SG_KEYUSED);
      	case 0x8001:													/* Cursor hoch									*/
      		ScrollWindow(inf->handle,SCROLL_UP);
          return(SG_KEYUSED);
        case 0x8101:													/* Shift Cursor hoch						*/
        case 0x8201:													/* Shift Cursor hoch						*/
      		ScrollWindow(inf->handle,SCROLL_PG_UP);
          return(SG_KEYUSED);
        case 0x8002:													/* Cursor runter								*/
      		ScrollWindow(inf->handle,SCROLL_DOWN);
          return(SG_KEYUSED);
        case 0x8102:													/* Shift Cursor runter					*/
        case 0x8202:													/* Shift Cursor runter					*/
      		ScrollWindow(inf->handle,SCROLL_PG_DOWN);
          return(SG_KEYUSED);
        case 0x8003:													/* Cursor rechts								*/
      		ScrollWindow(inf->handle,SCROLL_RIGHT);
          return(SG_KEYUSED);
        case 0x8103:													/* Shift Cursor rechts					*/
        case 0x8203:													/* Shift Cursor rechts					*/
      		ScrollWindow(inf->handle,SCROLL_PG_RIGHT);
          return(SG_KEYUSED);
        case 0x8004:													/* Cursor links									*/
      		ScrollWindow(inf->handle,SCROLL_LEFT);
          return(SG_KEYUSED);
        case 0x8104:													/* Shift Cursor links						*/
        case 0x8204:													/* Shift Cursor links						*/
      		ScrollWindow(inf->handle,SCROLL_PG_LEFT);
          return(SG_KEYUSED);
      }
      return(SG_KEYCONT);
    case SG_QUIT:
    	ShowBee();
    	DelCompleteList('DBDA');
    	ShowArrow();
    break;
  }
  return(SG_CONT);
}

/*--------------------------------------------------------------------------*/
/* MenÅpunkte entsprechen setzen																						*/
/* open : TRUE, wenn eine dBASE Datenbank geîffnet wurde										*/

VOID set_menu_dbase(WORD open)
{
	menu_ienable(menu_tree,MOEFFNEDBF,!open);
	menu_ienable(menu_tree,MDBCLOSE,open);
	menu_ienable(menu_tree,MDBASESTRUCT,open);
	menu_ienable(menu_tree,MDBASEDATEN,open);
	menu_ienable(menu_tree,MERZEUGEN,open);
	CloseWindowById('DBST');
	CloseWindowById('DBDA');
	test_menu_import();
}

/*--------------------------------------------------------------------------*/
/* dBASE Åber Kommandozeile laden																						*/

WORD open_dbase_kommando(BYTE *Datei)
{
	BYTE *s;
	
	s=strrchr(Datei,'.');
	if(s==NULL)
		return FALSE;
	if(strcmp(s,"DBS")==0)
	{	
		close_dbase();										/* Vorhanden dBASE-Datei schliessen		*/
		dbase_zeiger=(DBStruct *)malloc(sizeof(DBStruct));
		if(dbase_zeiger==NULL)
		{
			Note(ALERT_NORM,1,KEIN_SPEICHER);
			return TRUE;
		}
		ShowBee();
		if(dBase_open(dbase_zeiger,Datei))
			set_menu_dbase(TRUE);
		else
			close_dbase();
		ShowArrow();
		return TRUE;
	}
	return FALSE;
}

/*--------------------------------------------------------------------------*/
/* Init dBASE-Import																												*/

VOID init_dBASE_Import(VOID)
{
	dbase_zeiger=NULL;
	set_menu_dbase(FALSE);
}

/*--------------------------------------------------------------------------*/
/* Beendet dBASE-Import																											*/

VOID term_dBASE_Import(VOID)
{
	close_dbase();
}
