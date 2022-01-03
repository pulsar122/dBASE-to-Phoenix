/*--------------------------------------------------------------------------*/
/* dBASE Datenbank einlesen																									*/

#include <portab.h>
#include <import.h>
#include <phoenix.h>
#include <stdlib.h>
#include <string.h>
#include <sys_gem2.h>
#include "dbase.h"
#include "db_imp.h"
#include "db2ph.h"
#include "global.h"
#include "ph_base.h"
#include "ph_imp.h"
#include "main.h"
#include "rsc.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/

/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

#define SLIDER_LEN_DBASE			12
#define SLIDER_LEN_PHOENIX		(MAX_FIELDNAME+2)
#define SLIDER_LEN_IMPORT			(MAX_FIELDNAME+10)

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

typedef struct
{
	BYTE anz;
	BYTE Name[MAX_FIELDNAME+1];							/* Spalte- bzw. Feldname					*/
}PHOENIX_SPALTEN;

typedef struct
{
	BYTE Name[MAX_TABLENAME+1];							/* Tabellenname										*/
	PHOENIX_SPALTEN Column[MAX_COLUMN];
}PHOENIX_LISTE;

typedef struct
{
	WORD ph_tabelle;
	WORD ph_col;
	WORD db_col;
}IMPORT_FIELD;

typedef struct
{
	IMPORT_FIELD list[MAX_FIELDS];
	WORD Anzahl;
}IMPORT_LIST;

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

LOCAL VOID init_main_dbase(OBJECT *tree);
LOCAL VOID dbase_import(OBJECT *tree);
LOCAL VOID dbase_import_alle(OBJECT *tree);
LOCAL WORD dbase_pos(WORD pos);
LOCAL VOID init_main_phoenix(OBJECT *tree,WORD table);
LOCAL VOID test_phoenix_button(OBJECT *tree);
LOCAL VOID phoenix_import(OBJECT *tree);
LOCAL VOID phoenix_import_alle(OBJECT *tree);
LOCAL WORD phoenix_pos(WORD Table,WORD pos);
LOCAL VOID init_main_import(OBJECT *tree);
LOCAL VOID sg_slider_import(OBJECT *tree,WORD item);
LOCAL VOID import_phoenix(OBJECT *tree);
LOCAL VOID import_dbase(OBJECT *tree);
LOCAL VOID import_phoenix_alle(OBJECT *tree);
LOCAL VOID import_dbase_alle(OBJECT *tree);
LOCAL WORD import(VOID);

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

BYTE *slider_dbase;
WORD dbase_tabelle[MAX_FIELDS];

BYTE *slider_phoenix;
PHOENIX_LISTE phoenix_daten[MAX_TABLE];
WORD anz_tabelle;

BYTE *slider_import;
IMPORT_LIST import_list;

/*--------------------------------------------------------------------------*/
/* Testet den MenÅpunkt Import																							*/

VOID test_menu_import(VOID)
{
	WORD i,k;

	if(dbase_zeiger!=NULL && phoenix_zeiger!=NULL)
	{
		menu_ienable(menu_tree,MIMPORT,TRUE);
		for(i=0; i<dbase_zeiger->fields; i++)
		{
			if(dbase_zeiger->field[i].Typ==FIELD_OBJEKT ||
				 dbase_zeiger->field[i].Typ==FIELD_MEMO ||
				 dbase_zeiger->field[i].Typ==FIELD_PICTURE)
				dbase_tabelle[i]=FALSE;
			else
				dbase_tabelle[i]=TRUE;
		}
		for(i=0; i<phoenix_zeiger->CountTable; i++)
		{
			strcpy(phoenix_daten[i].Name,phoenix_zeiger->Table[i].Name);
			for(k=0; k<phoenix_zeiger->Table[i].CountColumn; k++)
			{
				strcpy(phoenix_daten[i].Column[k].Name,phoenix_zeiger->Table[i].Column[k].Name);
				phoenix_daten[i].Column[k].anz=TRUE;
			}
		}
		memset(&import_list,-1,sizeof(IMPORT_LIST));
		import_list.Anzahl=0;
	}
	else
	{
		menu_ienable(menu_tree,MIMPORT,FALSE);
		CloseWindowById('IMPO');
	}
}

/*--------------------------------------------------------------------------*/
/* Auswertung der Meldungen fÅr den Dialog Main															*/

WORD handle_import(WORD msg,WORD button,DIALOG_INFO *inf)
{
  BYTE ZStr[MAX_TABLENAME*MAX_TABLE + 1];
  WORD i,k;

	switch(msg)
	{
    case SG_START:
    	ZStr[0]='\0';															/* Damit die Elemente gesetz*/
			LinkSlider(inf->tree,MIMUP,MIMDOWN,MIMSLIDER,MIMSLIDERBOX,1,MIM,ZStr,SLIDER_LEN_IMPORT,FALSE);
			UnLinkSlider(inf->tree,MIM);
    	init_main_dbase(inf->tree);
    	init_main_phoenix(inf->tree,0);
    	init_main_import(inf->tree);
		 	DisableObj(inf->tree,MIMPORT_PH,0);
		 	DisableObj(inf->tree,MIMPORT_DB,0);
		break;
    case SG_UNDO:																/* Undo-Taste betÑtigt			*/
			return SG_CLOSE;
		case SG_HELP:																/* Help-Taste betÑtigt.			*/
 			CallOnlineHelp(HoleText(ANLEITUNGS_TEXTE,HELP_004,NULL));
 		break;
		case SG_END:
			switch(button)
			{
				case MPH_IMPORT_ALLE:
				 phoenix_import_alle(inf->tree);
				break;
				case MPH_IMPORT:
					phoenix_import(inf->tree);
				break;
				case MIMPORT_PH:
					import_phoenix(inf->tree);
				break;
				case MDB_IMPORT_ALLE:
				 dbase_import_alle(inf->tree);
				break;
				case MDB_IMPORT:
				 dbase_import(inf->tree);
				break;
				case MIMPORT_DB:
					import_dbase(inf->tree);
				break;
				case MIMPORT_PH_ALLE:
					import_phoenix_alle(inf->tree);
				break;
				case MIMPORT_DB_ALLE:
					import_dbase_alle(inf->tree);
				break;
				case MDOIMPORT:
					if(import())
						return SG_CLOSE;
				break;
				case MABBRUCH:
					return SG_CLOSE;
			}
		case SG_TOUCH:
			switch(button)
			{
				case MPHTABELLEN:
					ZStr[0]='\0';
					for(i=0; i<phoenix_zeiger->CountTable; i++)
					{
						strcat(ZStr,&phoenix_zeiger->Table[i].Name[0]);
						strcat(ZStr,"|");
					}
					ZStr[strlen(ZStr)-1]='\0';
					i=Listbox(ZStr,-1,-1,inf->tree,MPHTABELLEN);
					if(i!=-1)
					{
					  init_main_phoenix(inf->tree,i);
						RedrawSliderBox(inf->tree,MPH);
					}
				break;
			}
		break;
		case SG_SLIDER:
			i=GetSelectSldItem(inf->tree,inf->box);
			switch(inf->box)
			{
				case MPH:
					SelectSldItem(inf->tree,inf->box,button,TRUE);
					if(i!=button)													/* Neuer Eintrag?						*/
					{
						k=phoenix_pos(anz_tabelle,button);
						SetText(inf->tree,MPHTYP,
							&Datentypen[phoenix_zeiger->Table[anz_tabelle].Column[k].Typ][0]);
						sprintf(ZStr,"%li",phoenix_zeiger->Table[anz_tabelle].Column[k].Size);
						SetText(inf->tree,MPHLAENGE,ZStr);
					}
					else
					{
						SetText(inf->tree,MPHTYP,"");
						SetText(inf->tree,MPHLAENGE,"");
					}
					RedrawObj(inf->tree,MPHTYP,1,0,UPD_STATE);
					RedrawObj(inf->tree,MPHLAENGE,1,0,UPD_STATE);
					test_phoenix_button(inf->tree);
				break;
				case MIM:
					sg_slider_import(inf->tree,button);
				break;
				case MDB:
					SelectSldItem(inf->tree,inf->box,button,TRUE);
					if(i!=button)													/* Neuer Eintrag?						*/
					{
						EnableObj(inf->tree,MDB_IMPORT,TRUE);
						k=dbase_pos(button);
						hole_typ(dbase_zeiger->field[k].Typ,ZStr);
						SetText(inf->tree,MDBTYP,ZStr);
						sprintf(ZStr,"%i",(WORD)dbase_zeiger->field[k].Size);
						SetText(inf->tree,MDBLAENGE,ZStr);
					}
					else
					{
						DisableObj(inf->tree,MDB_IMPORT,TRUE);
						SetText(inf->tree,MDBTYP,"");
						SetText(inf->tree,MDBLAENGE,"");
					}
					RedrawObj(inf->tree,MDBTYP,1,0,UPD_STATE);
					RedrawObj(inf->tree,MDBLAENGE,1,0,UPD_STATE);
				break;
			}
		break;
		case SG_EMPTYSLDLINE:
			switch(inf->box)
			{
				case MPH:
					DeSelSldItem(inf->tree,inf->box,TRUE);
					SetText(inf->tree,MPHTYP,"");
					SetText(inf->tree,MPHLAENGE,"");
					DisableObj(inf->tree,MPH_IMPORT,TRUE);
				break;
				case MIM:
					DeSelSldItem(inf->tree,inf->box,TRUE);
					DisableObj(inf->tree,MIMPORT_PH,TRUE);
					DisableObj(inf->tree,MIMPORT_DB,TRUE);
					SetText(inf->tree,MPHITYP,"");
					SetText(inf->tree,MPHILAENGE,"");
					SetText(inf->tree,MDBITYP,"");
					SetText(inf->tree,MDBILAENGE,"");
					RedrawObj(inf->tree,MIFELDERBOX,2,0,UPD_STATE);
				break;
				case MDB:
					DeSelSldItem(inf->tree,inf->box,TRUE);
					SetText(inf->tree,MDBTYP,"");
					SetText(inf->tree,MDBLAENGE,"");
					DisableObj(inf->tree,MDB_IMPORT,TRUE);
				break;
			}
		break;
		case SG_QUIT:
			if(slider_dbase!=NULL)
			{
				free(slider_dbase);
				slider_dbase=NULL;
			}
			UnLinkSlider(inf->tree,MDB);
			if(slider_phoenix!=NULL)
			{
				free(slider_phoenix);
				slider_phoenix=NULL;
			}
			UnLinkSlider(inf->tree,MPH);
			if(slider_import!=NULL)
			{
				free(slider_import);
				slider_import=NULL;
			}
			UnLinkSlider(inf->tree,MIM);
		break;
		case SG_DRAGDROP:
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
/* dBASE Sliderbox init																											*/

VOID init_main_dbase(OBJECT *tree)
{
	BYTE *H;
	WORD i,k;
	
	UnLinkSlider(tree,MDB);
	if(slider_dbase!=NULL)
	{
		free(slider_dbase);
		slider_dbase=NULL;
	}
	slider_dbase=(BYTE *)malloc(dbase_zeiger->fields*SLIDER_LEN_DBASE);
	if(slider_dbase==NULL)
	{
		Note(ALERT_NORM,1,KEIN_SPEICHER);
		return;
	}
	H=slider_dbase;
	for(i=k=0; i<dbase_zeiger->fields; i++)
	{
		if(dbase_tabelle[i])												/* Feld anzeigen						*/
		{
			sprintf(H,"%-10s",dbase_zeiger->field[i].Name);
			H +=SLIDER_LEN_DBASE;
			k++;
		}
	}
	LinkSlider(tree,MDBUP,MDBDOWN,MDBSLIDER,MDBSLIDERBOX,k,MDB,slider_dbase,SLIDER_LEN_DBASE,FALSE);
	SetText(tree,MDBTYP,"");
	RedrawObj(tree,MDBTYP,1,0,UPD_STATE);
	SetText(tree,MDBLAENGE,"");
	RedrawObj(tree,MDBLAENGE,1,0,UPD_STATE);
 	DisableObj(tree,MDB_IMPORT,TRUE);
 	if(GetSliderItems(tree,MDB)>0)
 		EnableObj(tree,MDB_IMPORT_ALLE,TRUE);
 	else
 		DisableObj(tree,MDB_IMPORT_ALLE,TRUE);
	test_phoenix_button(tree);
}

/*--------------------------------------------------------------------------*/
/* dBASE -> Import																													*/

VOID dbase_import(OBJECT *tree)
{
	WORD i,k;
	WORD item;
	
	item=GetSelectSldItem(tree,MDB);
	i=GetSelectSldItem(tree,MIM);
	if(i==-1)																/* Noch kein Eintrag angewÑhlt		*/
	{
		if(import_list.Anzahl==dbase_zeiger->fields)
			return;
		i=dbase_pos(item);
		import_list.list[import_list.Anzahl].db_col=i;
		dbase_tabelle[i]=0;
		import_list.Anzahl++;
	}
	else
	{
		k=dbase_pos(item);
		if(import_list.list[i].db_col!=-1)
			dbase_tabelle[import_list.list[i].db_col]=1;
		dbase_tabelle[k]=0;
		import_list.list[i].db_col=k;
	}
	init_main_dbase(tree);
	init_main_import(tree);
	RedrawSliderBox(tree,MDB);
	RedrawSliderBox(tree,MIM);
}

/*--------------------------------------------------------------------------*/
/* dBASE -> Import	alle Datenfelder																				*/

VOID dbase_import_alle(OBJECT *tree)
{
	WORD i,k;
	
	
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		if(import_list.list[i].db_col==-1)		/* Noch nicht belegt.							*/
		{
			for(k=0; k<dbase_zeiger->fields; k++)
			{
				if(dbase_tabelle[k])
				{
					import_list.list[i].db_col=k;
					dbase_tabelle[k]=0;
					if(import_list.list[i].ph_tabelle==-1)
						import_list.Anzahl++;
					break;
				}
			}
		}
	}
	init_main_dbase(tree);
	init_main_import(tree);
	RedrawSliderBox(tree,MDB);
	RedrawSliderBox(tree,MIM);
}

/*--------------------------------------------------------------------------*/

WORD dbase_pos(WORD pos)
{
	WORD i,k;
	
	k=0;
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		if(dbase_tabelle[i])
		{
			if(k==pos)
				return i;
			k++;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
/* Phoenix Sliderbox init																										*/

VOID init_main_phoenix(OBJECT *tree,WORD table)
{
	BYTE *H;
	WORD i,k;
	LONG len;
	
	anz_tabelle=table;														/* Tabelle die angezeigt wir*/
	SetText(tree,MPHTABELLEN,phoenix_daten[table].Name);
	UnLinkSlider(tree,MPH);
	if(slider_phoenix!=NULL)
	{
		free(slider_phoenix);
		slider_phoenix=NULL;
	}
	len=phoenix_zeiger->Table[table].CountColumn*SLIDER_LEN_PHOENIX;
	slider_phoenix=(BYTE *)malloc(len);
	if(slider_phoenix==NULL)
	{
		Note(ALERT_NORM,1,KEIN_SPEICHER);
		return;
	}
	H=slider_phoenix;
	for(k=0,i=1; i<phoenix_zeiger->Table[table].CountColumn; i++)
	{
		if(phoenix_daten[table].Column[i].anz)			/* Feld anzeigen						*/
		{
			strcpy(H,phoenix_daten[table].Column[i].Name);
			H +=SLIDER_LEN_PHOENIX;
			k++;
		}
	}
	LinkSlider(tree,MPHUP,MPHDOWN,MPHVSLIDER,MPHVSLIDERBOX,k,MPH,slider_phoenix,SLIDER_LEN_PHOENIX,FALSE);
	LinkHorSlider(tree,MPH,MPHLEFT,MPHRIGHT,MPHHSLIDER,MPHHSLIDERBOX);
	SetText(tree,MPHTYP,"");
	RedrawObj(tree,MPHTYP,1,0,UPD_STATE);
	SetText(tree,MPHLAENGE,"");
	RedrawObj(tree,MPHLAENGE,1,0,UPD_STATE);
 	DisableObj(tree,MPH_IMPORT,TRUE);
 	for(i=k=0; i<dbase_zeiger->fields; i++)				/* Noch Datenfelder frei		*/
	{
		if(import_list.list[i].ph_tabelle==-1)
		{
			k=1;																			/* Noch ein Datenfeld frei 	*/
			break;
		}
	}
 	if(GetSliderItems(tree,MPH)>0 && k)
 	{
			EnableObj(tree,MPH_IMPORT_ALLE,TRUE);
 	}
 	else
 		DisableObj(tree,MPH_IMPORT_ALLE,TRUE);
	test_phoenix_button(tree);
}

/*--------------------------------------------------------------------------*/
/* Test die Buttons ob sie Selektierbar sein dÅrfen oder nicht							*/

VOID test_phoenix_button(OBJECT *tree)
{
	if((import_list.Anzahl==dbase_zeiger->fields &&
		 GetSelectSldItem(tree,MIM)==-1) || GetSelectSldItem(tree,MPH)==-1)
 		DisableObj(tree,MPH_IMPORT,TRUE);
 	else
 		EnableObj(tree,MPH_IMPORT,TRUE);
}

/*--------------------------------------------------------------------------*/
/* Phoenix -> Import																												*/

VOID phoenix_import(OBJECT *tree)
{
	WORD i,k;
	WORD item;
	
	item=GetSelectSldItem(tree,MPH);
	i=GetSelectSldItem(tree,MIM);
	if(i==-1)																/* Noch kein Eintrag angewÑhlt		*/
	{
		if(import_list.Anzahl==dbase_zeiger->fields)
			return;
		i=phoenix_pos(anz_tabelle,item);			/* Position ermitteln							*/
		import_list.list[import_list.Anzahl].ph_tabelle=anz_tabelle;
		import_list.list[import_list.Anzahl].ph_col=i;
		phoenix_daten[anz_tabelle].Column[i].anz=0;	/* nicht mehr anzeigen			*/
		import_list.Anzahl++;
	}
	else
	{
		k=phoenix_pos(anz_tabelle,item);
		if(import_list.list[i].ph_tabelle!=-1)
			phoenix_daten[import_list.list[i].ph_tabelle].Column[import_list.list[i].ph_col].anz=1;
		phoenix_daten[anz_tabelle].Column[k].anz=0;	/* nicht mehr anzeigen			*/
		import_list.list[i].ph_tabelle=anz_tabelle;
		import_list.list[i].ph_col=k;
	}
	init_main_phoenix(tree,anz_tabelle);
	init_main_import(tree);
	RedrawSliderBox(tree,MPH);
	RedrawSliderBox(tree,MIM);
}

/*--------------------------------------------------------------------------*/
/* Phoenix -> Import alle Datenfelder																				*/

VOID phoenix_import_alle(OBJECT *tree)
{
	WORD i,k;
	
	
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		if(import_list.list[i].ph_tabelle==-1)/* Noch nicht belegt.							*/
		{
			for(k=1; k<phoenix_zeiger->Table[anz_tabelle].CountColumn; k++)
			{
				if(phoenix_daten[anz_tabelle].Column[k].anz)
				{
					import_list.list[i].ph_tabelle=anz_tabelle;
					import_list.list[i].ph_col=k;
					phoenix_daten[anz_tabelle].Column[k].anz=0;
					if(import_list.list[i].db_col==-1)
						import_list.Anzahl++;
					break;
				}
			}
		}
	}
	init_main_phoenix(tree,anz_tabelle);
	init_main_import(tree);
	RedrawSliderBox(tree,MPH);
	RedrawSliderBox(tree,MIM);
}

/*--------------------------------------------------------------------------*/

WORD phoenix_pos(WORD Table,WORD pos)
{
	WORD i,k;
	
	k=0;
	for(i=1; i<phoenix_zeiger->Table[Table].CountColumn; i++)
	{
		if(phoenix_daten[Table].Column[i].anz)
		{
			if(k==pos)
				return i;
			k++;
		}
	}
	return 0;
}

/*--------------------------------------------------------------------------*/

VOID init_main_import(OBJECT *tree)
{
	BYTE *H;
	WORD i;

	UnLinkSlider(tree,MIM);
	if(slider_import!=NULL)
	{
		free(slider_import);
		slider_import=NULL;
	}
	slider_import=(BYTE *)malloc(import_list.Anzahl*SLIDER_LEN_IMPORT);
	if(slider_import==NULL)
	{
		Note(ALERT_NORM,1,KEIN_SPEICHER);
		return;
	}
	H=slider_import;
	for(i=0; i<import_list.Anzahl; i++)
	{
		strcpy(H,"\t");
		if(import_list.list[i].ph_tabelle!=-1)
			strncat(H,phoenix_zeiger->Table[import_list.list[i].ph_tabelle].Column[import_list.list[i].ph_col].Name,14);
		else
			strcat(H," ");
		strcat(H,"\t");
		if(import_list.list[i].db_col!=-1)
			strncat(H,dbase_zeiger->field[import_list.list[i].db_col].Name,10);
		H +=SLIDER_LEN_IMPORT;
	}
	LinkSlider(tree,MIMUP,MIMDOWN,MIMSLIDER,MIMSLIDERBOX,import_list.Anzahl,MIM,slider_import,SLIDER_LEN_IMPORT,FALSE);
	SetSliderTab(tree,MIM,0,TAB_LEFT);
	SetSliderTab(tree,MIM,15,TAB_LEFT);
	DisableObj(tree,MIMPORT_PH,TRUE);
	DisableObj(tree,MIMPORT_DB,TRUE);
	SetText(tree,MPHITYP,"");
	SetText(tree,MPHILAENGE,"");
	SetText(tree,MDBITYP,"");
	SetText(tree,MDBILAENGE,"");
	RedrawObj(tree,MIFELDERBOX,2,0,UPD_STATE);
 	if(GetSliderItems(tree,MIM)>0)
 	{
 		EnableObj(tree,MIMPORT_DB_ALLE,TRUE);
 		EnableObj(tree,MIMPORT_PH_ALLE,TRUE);
 	}
 	else
 	{
 		DisableObj(tree,MIMPORT_DB_ALLE,TRUE);
 		DisableObj(tree,MIMPORT_PH_ALLE,TRUE);
 	}
	test_phoenix_button(tree);
}

/*--------------------------------------------------------------------------*/

VOID sg_slider_import(OBJECT *tree,WORD item)
{
	BYTE ZStr[40];
	WORD i;
	
	i=GetSelectSldItem(tree,MIM);
	SelectSldItem(tree,MIM,item,TRUE);
	if(item==i)
	{
		DisableObj(tree,MIMPORT_PH,TRUE);
		DisableObj(tree,MIMPORT_DB,TRUE);
		SetText(tree,MPHITYP,"");
		SetText(tree,MPHILAENGE,"");
		SetText(tree,MDBITYP,"");
		SetText(tree,MDBILAENGE,"");
	}
	else
	{
		if(import_list.list[item].ph_tabelle!=-1)
		{
			EnableObj(tree,MIMPORT_PH,TRUE);
			SetText(tree,MPHITYP,
				Datentypen[phoenix_zeiger->Table[import_list.list[item].ph_tabelle].Column[import_list.list[item].ph_col].Typ]);
			sprintf(ZStr,"%li",
				phoenix_zeiger->Table[import_list.list[item].ph_tabelle].Column[import_list.list[item].ph_col].Size);
			SetText(tree,MPHILAENGE,ZStr);
		}
		else
			DisableObj(tree,MIMPORT_PH,TRUE);
		if(import_list.list[item].db_col!=-1)
		{
			EnableObj(tree,MIMPORT_DB,TRUE);
			hole_typ(dbase_zeiger->field[import_list.list[item].db_col].Typ,ZStr);
			SetText(tree,MDBITYP,ZStr);
			sprintf(ZStr,"%i",(WORD)dbase_zeiger->field[import_list.list[item].db_col].Size);
			SetText(tree,MDBILAENGE,ZStr);
		}
		else
			DisableObj(tree,MIMPORT_DB,TRUE);
	}
	RedrawObj(tree,MIFELDERBOX,2,0,UPD_STATE);
	test_phoenix_button(tree);
}

/*--------------------------------------------------------------------------*/

VOID import_phoenix(OBJECT *tree)
{
	WORD k,item;
	
	item=GetSelectSldItem(tree,MIM);
	phoenix_daten[import_list.list[item].ph_tabelle].Column[import_list.list[item].ph_col].anz=1;
	if(anz_tabelle==import_list.list[item].ph_tabelle)
	{
		init_main_phoenix(tree,anz_tabelle);
		RedrawSliderBox(tree,MPH);
	}
	import_list.list[item].ph_tabelle=-1;
	import_list.list[item].ph_col=-1;
	if(import_list.list[item].db_col==-1)
	{
		for(k=item; k<import_list.Anzahl; k++)
			import_list.list[k]=import_list.list[k+1];
		import_list.Anzahl--;
	}
	init_main_import(tree);
	RedrawSliderBox(tree,MIM);
}

/*--------------------------------------------------------------------------*/

VOID import_dbase(OBJECT *tree)
{
	WORD k,item;
	
	item=GetSelectSldItem(tree,MIM);
	dbase_tabelle[import_list.list[item].db_col]=1;
	init_main_dbase(tree);
	RedrawSliderBox(tree,MDB);
	import_list.list[item].db_col=-1;
	if(import_list.list[item].ph_tabelle==-1)
	{
		for(k=item; k<import_list.Anzahl; k++)
			import_list.list[k]=import_list.list[k+1];
		import_list.Anzahl--;
	}
	init_main_import(tree);
	RedrawSliderBox(tree,MIM);
}

/*--------------------------------------------------------------------------*/
/* Import -> Phoenix alle																										*/

VOID import_phoenix_alle(OBJECT *tree)
{
	WORD i,k,j;
	
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		if(import_list.list[i].ph_tabelle!=-1)
		{
			phoenix_daten[import_list.list[i].ph_tabelle].Column[import_list.list[i].ph_col].anz=1;
			import_list.list[i].ph_tabelle=-1;
			import_list.list[i].ph_col=-1;
		}
	}
	for(j=0; j<import_list.Anzahl; j++)
	{
		for(i=0; i<import_list.Anzahl; i++)
		{
			if(import_list.list[i].db_col==-1)
			{
				for(k=i; k<import_list.Anzahl; k++)
					import_list.list[k]=import_list.list[k+1];
			}
		}
	}
	import_list.Anzahl=0;
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		if(import_list.list[i].db_col!=-1)
			import_list.Anzahl++;
	}
	init_main_phoenix(tree,anz_tabelle);
	init_main_import(tree);
	RedrawSliderBox(tree,MPH);
	RedrawSliderBox(tree,MIM);
}

/*--------------------------------------------------------------------------*/
/* Import -> dBASE alle																											*/

VOID import_dbase_alle(OBJECT *tree)
{
	WORD i,k,j;
	
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		if(import_list.list[i].db_col!=-1)
		{
			dbase_tabelle[import_list.list[i].db_col]=1;
			import_list.list[i].db_col=-1;
		}
	}
	for(j=0; j<import_list.Anzahl; j++)
	{
		for(i=0; i<import_list.Anzahl; i++)
		{
			if(import_list.list[i].ph_tabelle==-1)
			{
				for(k=i; k<import_list.Anzahl; k++)
					import_list.list[k]=import_list.list[k+1];
			}
		}
	}
	import_list.Anzahl=0;
	for(i=0; i<dbase_zeiger->fields; i++)
	{
		if(import_list.list[i].ph_tabelle!=-1)
			import_list.Anzahl++;
	}
	init_main_dbase(tree);
	init_main_import(tree);
	RedrawSliderBox(tree,MDB);
	RedrawSliderBox(tree,MIM);
}

/*--------------------------------------------------------------------------*/
/* Importieren der Daten 																										*/

WORD import(VOID)
{
	BYTE ZStr[300];
	BYTE *Record;
	WORD i,k,j;
	WORD Anzahl_Table,status;
	WORD table[MAX_TABLE];
	LONG Size;
	
	if(import_list.Anzahl==0)
		return TRUE;
	Anzahl_Table=0;
	memset(table,-1,sizeof(table));
	memset(ZStr,0,sizeof(ZStr));
	for(i=0; i<import_list.Anzahl; i++)			/* Tabellen ermitteln							*/
	{
		if(import_list.list[i].ph_tabelle==-1 || import_list.list[i].db_col==-1)
		{
			Note(ALERT_NORM,1,UNVOLLSTAENDIG);
			return FALSE;
		}
		if(ZStr[import_list.list[i].ph_tabelle]==0)
		{
			ZStr[import_list.list[i].ph_tabelle]=1;
			table[Anzahl_Table]=import_list.list[i].ph_tabelle;
			Anzahl_Table++;
		}
	}
	Size=0;
	for(i=0; i<Anzahl_Table; i++)						/* Grîûter Datensatz ermitteln		*/
	{
		if(phoenix_zeiger->Table[table[i]].Size>Size)
			Size=phoenix_zeiger->Table[table[i]].Size;
	}
	Record=(BYTE *)malloc(Size);
	if(Record==NULL)
	{
		Note(ALERT_NORM,1,KEIN_SPEICHER);
		return TRUE;
	}
	ShowRotor();
	dBase_move(dbase_zeiger,1);							/* An die erste Position					*/
	for(i=0; i<dbase_zeiger->recs; i++)			/* Alle DatensÑtze durchgehen			*/
	{
		if(!dBase_deletet(dbase_zeiger))
		{
			for(k=0; k<Anzahl_Table; k++)					/* Alle gewÅnschten Tabellen durch*/
			{
				if(!db_fillnull(phoenix_zeiger->Base,table[k]+20,Record))
				{
					EndRotor();
					Note(ALERT_NORM,1,FEHLER_NULL_SETZEN);
					free(Record);
					return TRUE;
				}
				for(j=0; j<import_list.Anzahl; j++)	/* Alle Definitionen durchgehen	*/
				{
					if(import_list.list[j].ph_tabelle==table[k])/* gewÅnschte Tabelle	*/
					{
						dBase_read_typ(dbase_zeiger,
								dbase_zeiger->field[import_list.list[j].db_col].Name,ZStr);
						if(!db_setfield(phoenix_zeiger->Base,table[k]+20,import_list.list[j].ph_col,
								Record,ZStr))
						{
							EndRotor();
							Note(ALERT_NORM,1,FEHLER_EINFUEGEN_FELD);
							free(Record);
							return TRUE;
						}
					}
				}
				if(!db_insert(phoenix_zeiger->Base,table[k]+20,Record,&status))
				{
					EndRotor();
					status_phoenix(phoenix_zeiger->Base);
					free(Record);
					return TRUE;
				}
			}
		}
		dBase_move_plus(dbase_zeiger);				/* NÑchster Datensatz							*/
		UpdateRotor();
	}
	db_flush(phoenix_zeiger->Base,TRUE,TRUE);
	EndRotor();
	free(Record);
	return TRUE;
}

/*--------------------------------------------------------------------------*/

VOID init_make(VOID)
{
	slider_dbase=NULL;
	slider_phoenix=NULL;
	slider_import=NULL;
}
