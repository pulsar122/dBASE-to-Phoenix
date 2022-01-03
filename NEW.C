/*--------------------------------------------------------------------------*/
/* neue Phoenix Datenbank erstellen 																				*/

#include <portab.h>
#include <import.h>
#include <phoenix.h>
#include <stdlib.h>
#include <string.h>
#include <sys_gem2.h>
#include "dbase.h"
#include "db_imp.h"
#include "db2ph.h"
#include "file.h"
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

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

typedef struct
{
	BYTE Name[MAX_FIELDNAME+1];							/* Columnname											*/
	WORD Type;															/* Datentyp fÅr Phoenix						*/
	LONG Size;															/* Grîûe fÅr Phoenix							*/
	LONG Adr;																/* Adresse im Datenpuffer					*/
}COLUMNS;

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

/*--------------------------------------------------------------------------*/
/* Legt einen dBase-Tabelle in eine Phoenixdatenbank an. Dabei kann es sich	*/
/* um eine bestehende Datenbank handeln oder es wird eine neue erzeugt.			*/

VOID phoenix_neu(VOID)
{
	BYTE Name[30],datei[300],ZStr[300];
	BYTE *Record,*s;
	WORD i,k;
	WORD status;
	WORD Columns;
	WORD new_table_nr;
	LONG RecSize;
	COLUMNS spalten[MAX_FIELDS+1];
	SYSTABLE neue_tabelle;
	SYSCOLUMN neue_spalte;
	SYSINDEX neuer_index;
	PhoenixStruct *da;
	
	strcpy(Name,"*.*");
	i=FileSelect(Name,ph_path,"*.DAT\0","Phoenix Datenbank îffnen",datei);
	if(!i)
		return;
	s=strrchr(Name,'.');
	if(s!=NULL)														/* Suffix abscheiden								*/
		*s='\0';
	s=strrchr(datei,'\\');
	if(s!=NULL)														/* Datei vom Pfad abscheinden				*/
		*(s+1)='\0';
	da=(PhoenixStruct *)malloc(sizeof(PhoenixStruct));
	if(da==NULL)
	{
		Alert(ALERT_NORM,1,"[3][Keine Speicher mehr frei.][[Mist]");
		return;
	}
	strcpy(ZStr,datei);
	strcat(ZStr,Name);
	strcat(ZStr,".dat");
	if(!file_exist(ZStr))
	{
		da->Base=db_create(Name,datei,0,16L,16L);
		if(da->Base==NULL)
		{
			status_phoenix(da->Base);
			free(da);
			return;
		}
		da->CountTable=0;										/* Noch keine Anwendertabelle				*/
		ph_name[0]='\0';
		ph_passwort[0]='\0';
	}
	else
	{
		xWindowDialog('PHLO',-1,-1,"Datenbank îffnen","",FALSE,TRUE,login_tree,NULL,-2,NULL,handle_login);
 		if(ph_passwort[0]==255)
			return;
		if(!phoenix_open(da,ZStr,ph_name,ph_passwort))
		{
			free(da);
			return;
		}
	}
	memset(spalten,0,sizeof(spalten));
	strcpy(spalten[0].Name,"DBAdresse");		/* Datensatzadresse fÅr Phoenix		*/
	spalten[0].Type=TYPE_DBADDRESS;
	spalten[0].Size=sizeof(LONG);
	spalten[0].Adr=0;
	for(k=1,i=0; i<=dbase_zeiger->fields; i++)
	{
		strcpy(spalten[k].Name,dbase_zeiger->field[i].Name);
		switch((WORD)dbase_zeiger->field[i].Typ)
		{
			case FIELD_ASCII:
				spalten[k].Type=TYPE_CHAR;
				spalten[k].Size=dbase_zeiger->field[i].Size+1;

				spalten[k].Adr +=spalten[k-1].Size;
				if( (spalten[k].Adr%2)!=0)
					spalten[k].Adr++;
				spalten[k].Adr +=spalten[k-1].Adr;
				k++;
			break;
			case FIELD_NUM:
			case FIELD_NUM2:
				if(dbase_zeiger->field[i].Nk==0)	/* ganzzahliges Feld							*/
				{
					spalten[k].Type=TYPE_LONG;
					spalten[k].Size=sizeof(LONG);
				}
				else															/* Flieûkommazahl									*/
				{
					spalten[k].Type=TYPE_FLOAT;
					spalten[k].Size=sizeof(DOUBLE);
				}
				spalten[k].Adr +=spalten[k-1].Size;
				if( (spalten[k].Adr%2)!=0)
					spalten[k].Adr++;
				spalten[k].Adr +=spalten[k-1].Adr;
				k++;
			break;
			case FIELD_OBJEKT:
			break;
			case FIELD_LOGIC:
				spalten[k].Type=TYPE_CHAR;
				spalten[k].Size=2;

				spalten[k].Adr +=spalten[k-1].Size;
				if( (spalten[k].Adr%2)!=0)
					spalten[k].Adr++;
				spalten[k].Adr +=spalten[k-1].Adr;
				k++;
			break;
			case FIELD_DATE:
				spalten[k].Type=TYPE_DATE;
				spalten[k].Size=sizeof(DATE);

				spalten[k].Adr +=spalten[k-1].Size;
				if( (spalten[k].Adr%2)!=0)
					spalten[k].Adr++;
				spalten[k].Adr +=spalten[k-1].Adr;
				k++;
			break;
			case FIELD_MEMO:
			break;
			case FIELD_PICTURE:
			break;
		}
	}
	RecSize=0L;															/* Recordgrîûe ermitteln					*/
	RecSize +=spalten[k-1].Size;
	if((RecSize%2)!=0)
		RecSize++;
	RecSize +=spalten[k-1].Adr;
	new_table_nr=da->CountTable+20;					/* Tabellennummer									*/
	Columns=k;															/* Anzahl der Felder							*/
	neue_tabelle.address=0L;								/* neue Tabelle ablegen						*/
	neue_tabelle.table=new_table_nr;
	strcpy(neue_tabelle.name,Name);
	neue_tabelle.recs=0L;
	neue_tabelle.cols=Columns;
	neue_tabelle.indexes=1;
	neue_tabelle.size=RecSize;
	neue_tabelle.color=1;
	neue_tabelle.icon=0;
	neue_tabelle.children=0;
	neue_tabelle.parents=0;
	neue_tabelle.flags=GRANT_ALL;
	if(!db_insert(da->Base,SYS_TABLE,&neue_tabelle,&status))
	{
		status_phoenix(da->Base);
		phoenix_close(da);
		free(da);
		return;
	}
	neue_spalte.address=0L;
	neue_spalte.table=new_table_nr;
	neue_spalte.flags=GRANT_ALL;
	for(i=0; i<Columns; i++)								/* neue Spalten anlegen						*/
	{
		neue_spalte.number=i;
		strcpy(neue_spalte.name,spalten[i].Name);
		neue_spalte.type=spalten[i].Type;
		neue_spalte.addr=spalten[i].Adr;
		neue_spalte.size=spalten[i].Size;
		neue_spalte.format=spalten[i].Type;
		if(!db_insert(da->Base,SYS_COLUMN,&neue_spalte,&status))
		{
			status_phoenix(da->Base);
			phoenix_close(da);
			free(da);
			return;
		}
	}
	neuer_index.address=0L;									/* Index fÅr die neue Tabelle			*/
	neuer_index.table=new_table_nr;
	neuer_index.number=0;
	neuer_index.name[0]='\0';
	neuer_index.type=TYPE_DBADDRESS;
	neuer_index.root=0L;
	neuer_index.num_keys=0L;
	neuer_index.inxcols.size=0L;
	neuer_index.inxcols.cols[0].col=0;
	neuer_index.inxcols.cols[0].len=0;
	neuer_index.flags=0;
	if(!db_insert(da->Base,SYS_INDEX,&neuer_index,&status))
	{
		status_phoenix(da->Base);
		phoenix_close(da);
		free(da);
		return;
	}
	phoenix_close(da);											/* Datenbak schlieûen							*/
	Record=(BYTE *)malloc(RecSize);					/* Recordbuffer										*/
	if(Record==NULL)
	{
		Alert(ALERT_NORM,1,"[3][Keine Speicher mehr frei.][[Mist]");
		free(da);
		return;
	}
	if(!phoenix_open(da,ZStr,ph_name,ph_passwort))
		return;
	dBase_move(dbase_zeiger,1);							/* An die erste Position					*/
	ShowBee();
	ShowStatus("DatensÑtze einfÅgen...",NULL,0,dbase_zeiger->recs);
	for(i=0; i<dbase_zeiger->recs; i++)			/* Alle DatensÑtze								*/
	{
		if(i%5==0)
			ShowStatus(NULL,NULL,i,dbase_zeiger->recs);
		if(!dBase_deletet(dbase_zeiger))
		{
			memset(Record,0,RecSize);						/* Datenpuffer lîschen						*/
			for(k=1; k<Columns; k++)
				dBase_read_typ(dbase_zeiger,spalten[k].Name,Record+spalten[k].Adr);
			if(!db_insert(da->Base,new_table_nr,Record,&status))
			{
				ShowArrow();
				status_phoenix(da->Base);
				phoenix_close(da);
				free(Record);
				free(da);
				return;
			}
		}
		dBase_move_plus(dbase_zeiger);				/* NÑchster Datensatz							*/
	}
	ShowArrow();
	phoenix_close(da);
	free(Record);
	free(da);
}

