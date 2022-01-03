/*--------------------------------------------------------------------------*/
/* Initalisiert Phoenix/BASE																								*/

#include <portab.h>
#include <import.h>
#include <phoenix.h>
#include <string.h>
#include <sys_gem2.h>
#include "ph_base.h"

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

LOCAL CHAR *short_month [12] = 
{ 
	"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", 
	"Jul", "Aug", "Sep", "Okt", "Nov", "Dez" 
};

LOCAL CHAR *long_month [12] = 
{ 
	"Januar", "Februar", "Marz", "April", "Mai", "Juni", "Juli", 
	"August", "September", "Oktober", "November", "Dezember" 
};
 
/*--------------------------------------------------------------------------*/
/* ôffnet eine Phoenixdatenbank																							*/
/* d		: Zeiger auf eine Struktur die der Infos zu Datenbank abgelegt			*/
/*				werden.																														*/
/* path	: Zeiger auf den Path	+ Dateiname																		*/
/* RÅckgabe: 																																*/

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
		Alert(ALERT_NORM,1,"[3][Path angabe nicht korrekt.][[Mist]");
		return FALSE;
	}
	strcpy(Name,s+1);												/* Dateiname ermitteln						*/
	*(s+1)='\0';														/* Dateiname von Path abschneiden */
	s=strrchr(Name,'.');
	if(s==NULL)
	{
		Alert(ALERT_NORM,1,"[3][Datei angabe nicht korrekt.][[Mist]");
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
		d->Table[i].Size=table.size;					/* Grîûe eines Datensatz					*/
		d->Table[i].recs=table.recs;					/* Anzahl der EintrÑge						*/
		k=0;
		do
		{
			ret=db_fieldinfo(d->Base,i+offset,k,lpfield_info);
			if(ret==FAILURE)
				break;
			strcpy(&d->Table[i].Column[k].Name[0],&field.name[0]);	/* Feldenname	*/
			d->Table[i].Column[k].Typ=field.type;		/* Feldtyp										*/
			d->Table[i].Column[k].Size=field.size;	/* Feldgrîûe									*/
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
/* Schlieût eine Phoenixdatenbank																						*/

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
	strcpy(ZStr,"[3][Phoenix/BASE Error|");
	switch(error)
	{
		case DB_NOMEMORY:
			strcat(ZStr,"Zuwenig Speicher, um Operatiom auszufÅhren.");
		break;
		case DB_TNOCREATE:
			strcat(ZStr,"Indexdatei konnte nicht erstellt werden.");
		break;
		case DB_TNOOPEN:
			strcat(ZStr,"Indexdatei konnte nicht geîffnet werden.");
		break;
		case DB_TNOCLOSE:
			strcat(ZStr,"Indexdatei konnte nicht geschlossen werden.");
		break;
		case DB_TRDPAGE:
			strcat(ZStr,"SchlÅsselseite konnte nicht gelesen werden.");
		break;
		case DB_TWRPAGE:
			strcat(ZStr,"SchlÅsselseite konnte nicht geschrieben werden.");
		break;
		case DB_TNOKEY:
			strcat(ZStr,"SchlÅsselseite konnte nicht gefunden werden");
		break;
		case DB_DNOCREATE:
			strcat(ZStr,"Datendatei konnte nicht angelegt werden");
		break;
		case DB_DNOOPEN:
			strcat(ZStr,"Datendatei konnte nicht geîffnet werden");
		break;
		case DB_DNOCLOSE:
			strcat(ZStr,"Datendatei konnte nicht geschlossen werden");
		break;
		case DB_DNOTCLOSED:
			strcat(ZStr,"Datendatei wurde nicht ordnungsgemÑ· geschlossen");
		break;
		case DB_DVERSION:
			strcat(ZStr,"Versionsnummer nicht kompatibel");
		break;
		case DB_DINSERT:
			strcat(ZStr,"Datensatz konnte nicht eingefÅgt werden");
		break;
		case DB_DDELETE:
			strcat(ZStr,"Datensatz konnte nicht gelîscht werden");
		break;
		case DB_DUDELETE:
			strcat(ZStr,"Datensatz konnte nicht zurÅckgeholt werden");
		break;
		case DB_DUPDATE:
			strcat(ZStr,"Datensatz konnte nicht geÑndert werden");
		break;
		case DB_DREAD:
			strcat(ZStr,"Daten konnten nicht gelesen werden");
		break;
		case DB_DWRITE:
			strcat(ZStr,"Daten konnten nicht geschrieben werden");
		break;
		case DB_DNOLOCK:
			strcat(ZStr,"Datenbank konnte nicht gesperrt werden");
		break;
		case DB_DNOUNLOCK:
			strcat(ZStr,"Datenbank konnte nicht freigegeben werden");
		break;
		case DB_CDELETED:
			strcat(ZStr,"Zugriff auf einen gelîschten Datensatz");
		break;
		case DB_CNOTABLE:
			strcat(ZStr,"Tabelle existiert nicht");
		break;
		case DB_CNOCOLUMN:
			strcat(ZStr,"Spalte existiert nicht");
		break;
		case DB_CNOINDEX:
			strcat(ZStr,"Index existiert nicht");
		break;
		case DB_CNULLKEY:
			strcat(ZStr,"SchlÅsseleintrag leer");
		break;
		case DB_CNOTUNIQUE:
			strcat(ZStr,"SchlÅssel nicht eindeutig");
		break;
		case DB_CNOACCESS:
			strcat(ZStr,"Zugriff fÅr Benutzer nicht erlaubt");
		break;
		case DB_CRECLOCKED:
			strcat(ZStr,"Datensatz bereits durch einen anderen Benutzer|gesperrt");
		break;
		case DB_CLOCK_ERR:
			strcat(ZStr,"Datensatz konnte nicht gesperrt werden");
		break;
		case DB_CFREE_ERR:
			strcat(ZStr,"Datensatz konnte nicht freigegeben werden");
		break;
		case DB_CPASSWORD:
			strcat(ZStr,"Passwort falsch");
		break;
		case DB_CCREATEDD:
			strcat(ZStr,"Fehler beim Anlegen des Data-Dictionaries");
		break;
		case DB_CREADDD:
			strcat(ZStr,"Fehler beim Lesen des Data-Dictionaries");
		break;
		case DB_CINVALID:
			strcat(ZStr,"UngÅltige Datensatzadresse");
		break;
		case DB_CNULLCOL:
			strcat(ZStr,"Spalte hat den Wert NULL");
		break;
		case DB_CNOINSERT:
			strcat(ZStr,"EinfÅgen verstî·t gegen IntegritÑtsbedingung");
		break;
		case DB_CNODELETE:
			strcat(ZStr,"Lîschen verstî·t gegen IntegritÑtsbedingung");
		break;
		case DB_CNOUPDATE:
			strcat(ZStr,"éndern verstî·t gegen IntegritÑtsbedingung");
		break;
		case DB_CUPDATED:
			strcat(ZStr,"Datensatz inzwischen von anderem Benutzer geÑndert");
		break;
		case DB_CDELETEDUPDATE:
			strcat(ZStr,"Datensatz inzwischen von anderem Benutzer gelîscht");
		break;
		default:
			strcat(ZStr,"Unbekannter Fehler");
		break;
	}
	strcat(ZStr," ][[Abbruch]");
	Alert(ALERT_NORM,1,ZStr);
	return 0;
}

/*--------------------------------------------------------------------------*/
/* Initaliesiert Phoenix/BASE																								*/

VOID init_phoenix(VOID)
{
	init_conv (short_month, long_month, '.', ','); 
	db_init(0);
}
