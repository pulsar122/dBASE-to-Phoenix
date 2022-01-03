/*--------------------------------------------------------------------------*/
/* Erstell aus einer Phoenix-Datenbank entsprechende Papyrus-Tabellen 			*/

#include <portab.h>
#include <import.h>
#include <phoenix.h>
#include <stdlib.h>
#include <string.h>
#include <sys_gem2.h>

#include "dbase.h"
#include "ph_base.h"
#include "ph_imp.h"
#include "db2ph.h"
#include "key.h"
#include "rsc.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/

/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

#define DATE_FORMAT					"DD-MM-YYYY"
#define TIME_FORMAT					"HH:MI:SS.mmmmmm"
#define TIMESTAMP_FORMAT		"DD MM YYYY HH:MI:SS.mmmmmm"

#define TYPE_CHARNUMBER			2000


/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

typedef struct
{
	BYTE Name[MAX_FIELDNAME+1];							/* Columnname											*/
	WORD Type;															/* Datentyp fr Phoenix						*/
	LONG Size;															/* Gr”že fr Phoenix							*/
	LONG Adr;																/* Adresse im Datenpuffer					*/
}COLUMNS;

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

LOCAL WORD papCreate ( BYTE *datei );
LOCAL VOID papCreateDbtabdef ( BYTE *Name );
LOCAL VOID papCloseDbtabdef ( VOID );
LOCAL VOID papCreateFileddef ( BYTE *Name, WORD Typ );
LOCAL VOID papOpenDbTab ( VOID );
LOCAL VOID papCloseDbTab ( VOID );
LOCAL VOID papOpenDbSatz ( VOID );
LOCAL VOID papCloseDbSatz ( VOID );
LOCAL VOID papWriteText ( BYTE* text );
LOCAL VOID papClose ( VOID );

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

LOCAL WORD Utf_8;

/*--------------------------------------------------------------------------*/

VOID PhoenixToPapyrus(VOID)
{
	BYTE Name[30],datei[300],ZStr[300];
	BYTE Path[300];
	BYTE *Record, *memo, *s;
	WORD fehler_vor;
	WORD Tabelle;
	WORD i,k;
	WORD Columns;
	WORD spalten[MAX_FIELDS+1];
	LONG len;
	LONG l;
	FILE *fehler;
	PhoenixStruct *da;
	LPCURSOR cursor;
	FORMAT fdate, ftime, ftimestamp;
	
	if ( keytab == NULL )
		;

	Utf_8 = Akt_getExpNrFromName ( "UTF-8" );

	strcpy(Name,"*.*");
	i=FileSelect(Name,ph_path,"*.DAT\0",HoleText(TEXTE,TEXT_022,NULL),datei);
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
		Note(ALERT_NORM,1,KEIN_SPEICHER);
		return;
	}
	strcpy(ZStr,datei);
	strcat(ZStr,Name);
	strcat(ZStr,".dat");
	if(!file_exist(ZStr))
	{
		Note(ALERT_NORM,1,DATEI_NICHT_DA);
		free(da);
		return;
		
	}
	else
	{
		ShowArrow();
		xWindowDialog('PHLO',-1,-1,HoleText(TEXTE,TEXT_023,NULL),"",FALSE,TRUE,login_tree,NULL,-2,NULL,handle_login);
 		if(ph_passwort[0]==255)
			return;
		if(!phoenix_open(da,ZStr,ph_name,ph_passwort))
		{
			free(da);
			return;
		}
	}
	strcpy(Name,"*.*");
	i=FileSelect(Name,Path,"*.ph\0",HoleText(TEXTE,TEXT_031,NULL),datei);
	if(!i)
	{
		phoenix_close(da);
		free(da);
		return;
	}
  build_format(TYPE_DATE, DATE_FORMAT, fdate);
  build_format(TYPE_TIME, TIME_FORMAT, ftime);
  build_format(TYPE_TIMESTAMP, TIMESTAMP_FORMAT, ftimestamp);
	cursor = db_newcursor(da->Base);
	SplitFilename(datei, Path, NULL);
	ShowBee();
	fehler_vor = 0;											/* Z„hlen der aufgetretenen Fehler		*/
	for(Tabelle = 0; 	Tabelle < da->CountTable; Tabelle++)	/* Alle Tabellen du*/
	{
		strcpy(datei, Path);							/* Ausgew„hlten Pfad holen						*/
		strcat(datei, da->Table[Tabelle].Name);	/* Tabellenname anh„ngen				*/
		strcat(datei, ".txt");						/* Zu erst fr eventuelle Fehler			*/

		Columns = 0;
		ShowStatus(da->Table[Tabelle].Name, "Spalten testen", 0L, da->Table[Tabelle].CountColumn, 0);

		/* Startet bei 1, da der Typ TYPE_DBADDRESS nur intern benutzt wird.		*/
		for( k = 1; k < da->Table[Tabelle].CountColumn; k++)
		{
			ShowStatus(NULL, NULL, (LONG) k, da->Table[Tabelle].CountColumn, 0);
			switch(da->Table[Tabelle].Column[k].Typ)
			{
				case TYPE_CHAR:
				case TYPE_WORD:
/*
				case TYPE_LONG:
				case TYPE_DATE:
				case TYPE_TIME:
				case TYPE_TIMESTAMP:
				case TYPE_EXTERN:
*/
					spalten[Columns]=k;
					Columns++;
				break;
				default:
					fehler = fopen(datei , "a");
					if( fehler != NULL )
					{
						fehler_vor++;
						strcpy(ZStr, da->Table[Tabelle].Name);
						strcat(ZStr, ": \"");
						strcat(ZStr, da->Table[Tabelle].Column[k].Name);
						strcat(ZStr, "\" konnte nicht exportiert werden\n");
						fwrite(ZStr, 1, strlen(ZStr), fehler);
						fclose ( fehler );
					}
				break;
			}
		}
		if(Columns)												/* Verwertbare Spalte vorhanden?			*/
		{
			strcpy(datei, Path);						/* Ausgew„hlten Pfad holen						*/
			strcat(datei, da->Table[Tabelle].Name);	/* Tabellenname anh„ngen			*/
			strcat(datei, ".pb");
			
			if( papCreate ( datei ) )				/* PapyrusBase konnte erzeugt werden	*/
			{
				papCreateDbtabdef ( da->Table[Tabelle].Name );
				for(k = 0; k < Columns; k++)	/* Alle Felder anlegen								*/
				{
					ShowStatus(NULL, "dBASE Tabelle init", (LONG) k, Columns, 0);
					switch(da->Table[Tabelle].Column[spalten[k]].Typ)
					{
						case TYPE_CHAR:
							papCreateFileddef ( da->Table[Tabelle].Column[spalten[k]].Name, TYPE_CHARNUMBER );
						break;
						case TYPE_WORD:
							papCreateFileddef ( da->Table[Tabelle].Column[spalten[k]].Name, TYPE_CHARNUMBER );
						break;
						case TYPE_LONG:
/*
							dbInitField(db_handle, k + 1, da->Table[Tabelle].Column[spalten[k]].Name,
							            FT_NUMBER, 11, 0);
*/
						break;
						case TYPE_FLOAT:
						break;
						case TYPE_CFLOAT:
						break;
						case TYPE_DATE:
/*
							dbInitField(db_handle, k + 1, da->Table[Tabelle].Column[spalten[k]].Name,
							            FT_DATE, 8, 0);
*/
						break;
						case TYPE_TIME:
/*
							dbInitField(db_handle, k + 1, da->Table[Tabelle].Column[spalten[k]].Name,
							            FT_CHARS, sizeof(TIME_FORMAT), 0);
*/
						break;
						case TYPE_TIMESTAMP:
/*
							dbInitField(db_handle, k + 1, da->Table[Tabelle].Column[spalten[k]].Name,
							            FT_CHARS, sizeof(TIMESTAMP_FORMAT), 0);
*/
						break;
						case TYPE_VARBYTE:
						break;
						case TYPE_VARWORD:
						break;
						case TYPE_VARLONG:
						break;
						case TYPE_PICTURE:
						break;
						case TYPE_EXTERN:
/*
							if( da->Table[Tabelle].Column[spalten[k]].Size > 254 )
							{
								dbInitField(db_handle, k + 1, da->Table[Tabelle].Column[spalten[k]].Name,
							            FT_MEMO, 10, 0);
							}
							else
							{
								len = da->Table[Tabelle].Column[spalten[k]].Size;
								dbInitField(db_handle, k + 1, da->Table[Tabelle].Column[spalten[k]].Name,
							            FT_CHARS, len, 0);
							}
*/
						break;
						case TYPE_DBADDRESS:
						break;
						case TYPE_BLOB:
						break;
					}
				}
				papCloseDbtabdef ();
				Record = malloc(da->Table[Tabelle].Size);
				memo   = malloc(da->Table[Tabelle].Size);
				if( Record != NULL)
				{
					papOpenDbTab ();
					db_initcursor(da->Base, Tabelle + 20, 0, ASCENDING, cursor);
					ShowStatus(NULL, "Datensatz konvertieren", 0, da->Table[Tabelle].recs, 0);
					for(l = 0; l <da->Table[Tabelle].recs; l++)
					{
						if(db_read(da->Base, Tabelle + 20, Record, cursor, 0L, FALSE))
						{
							ShowStatus(NULL, NULL, l, da->Table[Tabelle].recs, 0);
							papOpenDbSatz ();
							for(k = 0; k <= Columns; k++)	/* Alle Felder konvertieren			*/
							{
								switch(da->Table[Tabelle].Column[spalten[k]].Typ)
								{
									case TYPE_CHAR:
										len = da->Table[Tabelle].Column[spalten[k]].Size;
										strncpy(memo,
														Record + da->Table[Tabelle].Column[spalten[k]].Offset,
														len);
										memo[len+1] = '\0';
										papWriteText ( memo );
									break;
									case TYPE_WORD:
									  bin2str(TYPE_WORD,Record + da->Table[Tabelle].Column[spalten[k]].Offset,
									  				ZStr);
									  papWriteText ( ZStr );
									break;
/*
									case TYPE_LONG:
									  bin2str(TYPE_LONG,Record + da->Table[Tabelle].Column[spalten[k]].Offset,
									  				ZStr);
										dbChangeField(db_handle, rnr, k + 1, ZStr);
									break;
									case TYPE_DATE:
									  bin2str(TYPE_DATE,Record + da->Table[Tabelle].Column[spalten[k]].Offset,
									  				ZStr);
										if( strlen( ZStr ) > 0 )
										{
											str2format(TYPE_DATE, ZStr, fdate);
											dbChangeField(db_handle, rnr, k + 1, ZStr);
										}
									break;
									case TYPE_TIME:
									  bin2str(TYPE_TIME,Record + da->Table[Tabelle].Column[spalten[k]].Offset,
									  				ZStr);
										str2format(TYPE_TIME, ZStr, ftime);
										dbChangeField(db_handle, rnr, k + 1, ZStr);
									break;
									case TYPE_TIMESTAMP:
									  bin2str(TYPE_TIMESTAMP,Record + da->Table[Tabelle].Column[spalten[k]].Offset,
									  				ZStr);
										str2format(TYPE_TIMESTAMP, ZStr, ftimestamp);
										dbChangeField(db_handle, rnr, k + 1, ZStr);
									break;
									case TYPE_EXTERN:
										len = da->Table[Tabelle].Column[spalten[k]].Size;
										strncpy(memo,
														Record + da->Table[Tabelle].Column[spalten[k]].Offset,
														len);
										memo[len+1] = '\0';
										dbChangeField(db_handle, rnr, k + 1, memo);
									break;
*/
								}
							}
							papCloseDbSatz ();
						}
						else
							;
						db_movecursor(da->Base, cursor, 1);
					}
					papCloseDbTab ();
					free(memo);
					free(Record);
				}
				else
					Note(ALERT_NORM,1,KEIN_SPEICHER);
				papClose ();
			}
			else
				Note(ALERT_NORM,1,DBASE_ERZEUGEN);
		}
	}
	EndStatus();
	db_freecursor(da->Base, cursor);
	ShowArrow();
	phoenix_close(da);
	free(da);
	if( fehler_vor )										/* Wurde eine Fehlerdatei geschrieben	*/
		Note(ALERT_NORM,1,ALLE_SPALTEN);

}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

#define PAP_END1			"</PAPBASE>\n"

LOCAL FILE *pap;


WORD papCreate ( BYTE *datei )
{
	BYTE ZStr[100];
	
	pap = fopen(datei , "wb+");	
	if ( pap == NULL )
		return FALSE;
	strcpy ( ZStr, "<?xml version=\"1.0\" standalone=\"yes\"?>\n" );
	strcat ( ZStr, "<PAPBASE VERSION=\"8.21\">\n" );
	strcat ( ZStr, "<DB_OPTIONS INDEX=\"1\" LANG=\"de\"/>\n" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
	return TRUE;
}

/*--------------------------------------------------------------------------*/

VOID papCreateDbtabdef ( BYTE *Name )
{
	BYTE ZStr[100];
	
	strcpy ( ZStr, "<DBTABDEF ID=\"1\" NAME=\"" );
	strcat ( ZStr, Name );
	strcat ( ZStr, "\">\n" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
}

/*--------------------------------------------------------------------------*/

VOID papCloseDbtabdef ( VOID )
{
	BYTE ZStr[100];
	
	strcpy ( ZStr, "</DBTABDEF>\n" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
}


/*--------------------------------------------------------------------------*/

VOID papCreateFileddef ( BYTE *Name, WORD Typ )
{
	BYTE ZStr[200];
	
	strcpy ( ZStr, "<FIELDDEF NAME=\"" );
	strcat ( ZStr, Name );
	strcat ( ZStr, "\" TYPE=\"" );
	switch ( Typ )
	{
		case TYPE_CHARNUMBER:
			strcat ( ZStr, "STD" );
		break;
		case TYPE_CHAR:
			strcat ( ZStr, "STRING" );
		break;
	}
	strcat ( ZStr, "\"></FIELDDEF>\n" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
}

/*--------------------------------------------------------------------------*/

VOID papOpenDbTab ( VOID )
{
	BYTE ZStr[100];
	
	strcpy ( ZStr, "<DBTAB ID=\"1\">\n" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
}


/*--------------------------------------------------------------------------*/

VOID papCloseDbTab ( VOID )
{
	BYTE ZStr[100];
	
	strcpy ( ZStr, "</DBTAB>\n" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
}


/*--------------------------------------------------------------------------*/

VOID papOpenDbSatz ( VOID )
{
	BYTE ZStr[100];
	
	strcpy ( ZStr, "<R>" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
}


/*--------------------------------------------------------------------------*/

VOID papCloseDbSatz ( VOID )
{
	BYTE ZStr[100];
	
	strcpy ( ZStr, "</R>\n" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
}

/*--------------------------------------------------------------------------*/

VOID papWriteText ( BYTE* text )
{
	BYTE ZStr[10];
	BYTE *s;
	LONG len, dest_len;

	len = strlen ( text );
	
	s = malloc ( len * 2 + 10 );
	if ( len != NULL )
	{
		
		Akt_BlockXUtf2XUtf ( s, &dest_len, Utf_8, text, len, 0 );
			
		fwrite ( s, 1, dest_len, pap);
		free ( s );
	}
	strcpy ( ZStr, "\n" );
	fwrite ( ZStr, 1, strlen ( ZStr ), pap);
}

/*--------------------------------------------------------------------------*/

VOID papClose ( VOID )
{
	fwrite ( PAP_END1, 1, strlen ( PAP_END1 ), pap);
	fclose ( pap );
}
