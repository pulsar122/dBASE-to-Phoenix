/*--------------------------------------------------------------------------*/
/* Initalisiert Phoenix/BASE																								*/

#include <portab.h>
#include <import.h>
#include <phoenix.h>
#include <string.h>
#include <sys_gem2.h>
#include "db2ph.h"
#include "ph_base.h"
#include "rsc.h"
/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/

/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

LOCAL CHAR short_month [12][4];
LOCAL CHAR long_month [12][20];
LOCAL CHAR *s_month[12];
LOCAL CHAR *l_month[12];

/*--------------------------------------------------------------------------*/
/* ™ffnet eine Phoenixdatenbank																							*/
/* d		: Zeiger auf eine Struktur die der Infos zu Datenbank abgelegt			*/
/*				werden.																														*/
/* path	: Zeiger auf den Path	+ Dateiname																		*/
/* Rckgabe: 																																*/

WORD phoenix_open(PhoenixStruct *d,BYTE *path,BYTE *User,BYTE *Password)
{
	BYTE Name[30];
	BYTE *s;
	WORD i,k,ret;
	WORD offset;
  BASE_INFO info;
	LPBASE_INFO lpinfo;
	TABLE_INFO table;
	LPTABLE_INFO lptable;
	FIELD_INFO field;
	LPFIELD_INFO lpfield_info;

	memset(d,0,sizeof(PhoenixStruct));
	s=strrchr(path,'\\');
	if(s==NULL)
	{
		Note(ALERT_NORM,1,PFAD_FEHLER);
		return FALSE;
	}
	strcpy(Name,s+1);												/* Dateiname ermitteln						*/
	*(s+1)='\0';														/* Dateiname von Path abschneiden */
	s=strrchr(Name,'.');
	if(s==NULL)
	{
		Note(ALERT_NORM,1,DATEINAME_FEHLER);
		return FALSE;
	}
	*s='\0';																	/* Suffix abscheinden							*/
	d->Base=db_open(Name,path,BASE_FLUSH,16L,20,User,Password);
	if(d->Base==NULL)
	{
		status_phoenix(d->Base);
		return FALSE;
	}
	ShowBee();
	lpinfo=&info;
	db_baseinfo(d->Base,lpinfo);
	d->created=lpinfo->data_info.created;
	d->lastuse=lpinfo->data_info.lastuse;
	d->reorg=lpinfo->data_info.reorg;
	offset=20;
	i=0;																		/* Erste Benutzer definierte Tab.	*/
	lptable=&table;
	lpfield_info=&field;
	do
	{
		ret=db_tableinfo(d->Base,i+offset,lptable);
		if(ret==FAILURE)
			break;
		strcpy(&d->Table[i].Name[0],&table.name[0]);	/* Tabellenname						*/
		d->Table[i].CountColumn=table.cols;		/* Anzahl der Spalten (Felder)		*/
		d->Table[i].Size=table.size;					/* Gr”že eines Datensatz					*/
		d->Table[i].recs=table.recs;					/* Anzahl der Eintr„ge						*/
		k=0;
		do
		{
			ret=db_fieldinfo(d->Base,i+offset,k,lpfield_info);
			if(ret==FAILURE)
				break;
			strcpy(&d->Table[i].Column[k].Name[0],&field.name[0]);	/* Feldenname	*/
			d->Table[i].Column[k].Typ=field.type;		/* Feldtyp										*/
			d->Table[i].Column[k].Size=field.size;	/* Feldgr”že									*/
			d->Table[i].Column[k].Offset=field.addr;/* Adresse im Record					*/
			k++;
		}while(1);
		i++;
	}while(1);
	d->CountTable=i;
	ShowArrow();
	return TRUE;
}

/*--------------------------------------------------------------------------*/
/* Schliežt eine Phoenixdatenbank																						*/

WORD phoenix_close(PhoenixStruct *d)
{
	if(d->Base!=NULL)
		db_close(d->Base);
	return TRUE;
}

/*--------------------------------------------------------------------------*/
/* Teste ob ein Datenbankoperation erfolgereich war, wenn nein erscheint ein*/
/* Fehlermeldung																														*/

WORD status_phoenix(LPBASE *base)
{
	char ZStr[100];
	int error;
	
	error=db_status(base);
	if(error==SUCCESS)
		return 1;
	strcpy(ZStr,"[3][");
	strcat(ZStr,HoleText(PHOENIX_BASE,TEXT_01,NULL));
	strcat(ZStr,"|");
	if(error >= DB_NOMEMORY && error <= DB_CDELETEDUPDATE)
		strcat(ZStr,HoleText(PHOENIX_BASE,ERROR_00 + (error - DB_NOMEMORY),NULL));
	else
		strcat(ZStr,HoleText(PHOENIX_BASE,ERROR_40,NULL));
	strcat(ZStr,"][");
	strcat(ZStr,HoleText(PHOENIX_BASE,TEXT_02,NULL));
	strcat(ZStr,"]");
	ShowArrow();
	Alert(ALERT_NORM,1,ZStr);
	return 0;
}

/*--------------------------------------------------------------------------*/
/* Initaliesiert Phoenix/BASE																								*/

VOID init_phoenix(VOID)
{
	WORD i;
	
	
	for(i=0; i<12; i++)
	{
		strcpy(&short_month[i][0],HoleText(PHOENIX_BASE,MONAT_KURZ_01 + i,NULL));
		s_month[i]=&short_month[i][0];
	}
	for(i=0; i<12; i++)
	{
		strcpy(&long_month[i][0],HoleText(PHOENIX_BASE,MONAT_LANG_01 + i,NULL));
		l_month[i]=&long_month[i][0];
	}
		
	init_conv (s_month, l_month, '.', ','); 
	db_init(0);
}
