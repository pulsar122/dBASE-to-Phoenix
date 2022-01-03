/*--------------------------------------------------------------------------*/
/* Routine zum Bearbeiten von dBase-Dateien																	*/

#include <stdio.h>
#include <portab.h>
#include <ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys_gem2.h>
#include <import.h>				/* Wegen des Datentyp DATE */
#include <phoenix.h>			/* Wegen des Datentyp DATE */

#include "db2ph.h"
#include "dbase.h"
#include "key.h"
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

LOCAL WORD Feldnummer(DBStruct *d,BYTE *fname);
LOCAL VOID flip_word(BYTE *adr);
LOCAL VOID flip_long(WORD *adr);

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

/*--------------------------------------------------------------------------*/
/* ôffnen eine dBase-Datenbank und initalisiert die Datenbankstruckture			*/
/* d		: Zeiger auf die Datenbankstrukture																	*/
/* file	:	Zeiger auf den kompletten Filenamen mit Pfad											*/
/* RÅckgabe: TRUE = Alles OK																								*/
/*					 FALSE = Datei konnte nicht geîffnet/gelesen werden.						*/
/*					 -1 = Datenformat nicht korrekt																	*/

WORD dBase_open(DBStruct *d,BYTE *file)
{
	BYTE typ;
	WORD i;
	LONG ret;
	LONG offset;
	LONG len;
	dBASEII_Header HeadII;
	dBASEII_FIELD FieldII;
	dBASEIII_Header HeadIII;
	dBASEIII_FIELD FieldIII;
	
	memset(d,0,sizeof(DBStruct));
	ret=Fopen(file,FO_RW);
	if( ret < 0 )														/* Fehler beim îffnen aufgetreten	*/
		return FALSE;
	d->f=(WORD)ret;													/* Datehandle holen								*/
	ret=Fread(d->f,1,&typ);									/* Type der Datenbank lesen				*/
	if(ret!=1)
	{
		ShowArrow();
		Note(ALERT_NORM,1,DBASE_LESE_FEHLER);
		Fclose(d->f);
		return FALSE;
	}
	switch((WORD)typ)												/* Typ der Datenbank auswerten		*/
	{
		case DBASEII:
		case DBASEIII:
		case DBASEIV:
		case DBASEV:
		case DBASEIII_MEMO:
		case DBASEIV_MEMO:
		case DBASEIV_SQL:
		case FOXPRO_MEMO:
			d->typ=(WORD)typ;
		break;
		default:
			ShowArrow();
			Note(ALERT_NORM,1,DBASE_FORMAT);
			Fclose(d->f);
			return -1;
	}
	Fseek(0,d->f,0);												/* Wieder an den Anfang						*/
	if(d->typ==DBASEII)											/* dBASEII												*/
	{
		len=sizeof(dBASEII_Header);
		ret=Fread(d->f,len,&HeadII);					/* Header lesen										*/
		if(ret!=len)
		{
			ShowArrow();
			Note(ALERT_NORM,1,DBASE_LESE_FEHLER);
			Fclose(d->f);
			return FALSE;
		}
		d->headersize=521;										/* Grîûe des Header								*/
		d->recsize=HeadII.Recsize;						/* Recordgrîûe										*/
		d->recs=HeadII.Records;								/* Anzahl der DatensÑtze					*/
		d->Tag=HeadII.Tag;										/* Letzte énderung vermerken			*/
		d->Monat=HeadII.Monat;
		d->Jahr=(WORD)HeadII.Jahr;
		offset=1;
		len=sizeof(dBASEII_FIELD);
		for(i=0; i<31; i++)										/* Max 32 Felder erlaubt					*/
		{
			ret=Fread(d->f,len,&FieldII);	
			if(FieldII.Name[0]==0x0d)						/* Letztes Feld?									*/
				break;
			if(ret!=len)
			{
				ShowArrow();
				Note(ALERT_NORM,1,DBASE_LESE_FEHLER);
				Fclose(d->f);
				return FALSE;
			}
			strcpy(&(d->field[i].Name[0]),&FieldII.Name[0]);
			d->field[i].Typ=FieldII.Typ;
			d->field[i].Size=FieldII.Fieldsize;
			d->field[i].Nk=FieldII.Nk;
			d->field[i].offset=offset;
			offset +=FieldII.Fieldsize;
		}
		d->fields=i;													/* Azhal der Felder								*/
	}
	else																		/* >= dBASEIII + Memofelder				*/
	{
		len=sizeof(dBASEIII_Header);
		ret=Fread(d->f,len,&HeadIII);					/* Header lesen										*/
		if(ret!=len)
		{
			ShowArrow();
			Note(ALERT_NORM,1,DBASE_LESE_FEHLER);
			Fclose(d->f);
			return FALSE;
		}
		flip_word((BYTE *)&HeadIII.Headersize);
		d->headersize=HeadIII.Headersize;			/* Grîûe des Header								*/
		flip_word((BYTE *)&HeadIII.Recsize);
		d->recsize=HeadIII.Recsize;						/* Recordgrîûe										*/
		flip_long((WORD *)&HeadIII.Records);
		d->recs=HeadIII.Records;							/* Anzahl der DatensÑtze					*/
		d->Tag=HeadIII.Tag;										/* Letzte énderung vermerken			*/
		d->Monat=HeadIII.Monat;
		d->Jahr=(WORD)HeadIII.Jahr + 1900;
		d->fields=(d->headersize - 0x20) / 0x20;	/* Anzahl der Felder					*/
		offset=1;
		len=sizeof(dBASEIII_FIELD);
		for(i=0; i<d->fields; i++)						/* Max 128 Felder erlaubt					*/
		{
			ret=Fread(d->f,len,&FieldIII);	
			if(FieldIII.Name[0]==0x0d)					/* Letztes Feld?									*/
				break;
			if(ret!=len)
			{
				ShowArrow();
				Note(ALERT_NORM,1,DBASE_LESE_FEHLER);
				Fclose(d->f);
				return FALSE;
			}
			strcpy(&d->field[i].Name[0],&FieldIII.Name[0]);
			d->field[i].Typ=FieldIII.Typ;
			d->field[i].Size=FieldIII.Fieldsize;
			d->field[i].Nk=FieldIII.Nk;
			d->field[i].offset=offset;
			offset +=FieldIII.Fieldsize;
		}
		d->buffer=(BYTE *)malloc(d->recsize+2);
		if(d->buffer==NULL)
		{
			ShowArrow();
			Note(ALERT_NORM,1,KEIN_SPEICHER);
			Fclose(d->f);
			return FALSE;
		}
	 dBase_move(d,1);
	}
	return TRUE;
}

/*--------------------------------------------------------------------------*/
/* Datei schlieûen																													*/
/* d		: Zeiger auf die Datenbankstrukture																	*/

VOID dBase_close(DBStruct *d)
{
	if(d->modi)
	{
	}
	Fclose(d->f);
	free(d->buffer);
}

/*--------------------------------------------------------------------------*/
/* Satzzeiger bewegen																												*/
/* d		: Zeiger auf die Datenbankstrukture																	*/
/* pos	:	Position auf die der Satzzeiger bewegt werden soll								*/
/* RÅckgabe: TRUE = Alles OK																								*/
/*					 FALSE = Zeiger konnte nicht gesetzt werden.										*/

WORD dBase_move(DBStruct *d,LONG pos)
{
	LONG ret;
	
	if(pos>d->recs || pos<1)
		return FALSE;
	if(Fseek(d->headersize+(pos-1)*d->recsize,d->f,0)<0)
		return FALSE;
	ret=Fread(d->f,d->recsize,d->buffer);
	if(ret!=d->recsize)
		return FALSE;
	d->fpos=pos;
	if(keytab_id_import!=-1)
		Akt_BlockX2Atari ( d->buffer, keytab_id_import, d->buffer, d->recsize );
	return TRUE;
}

/*--------------------------------------------------------------------------*/
/* Satzzeiger um eins weiter bewegen																				*/
/* d		: Zeiger auf die Datenbankstrukture																	*/
/* RÅckgabe: TRUE wenn der Satzzeiger bewegt werden konnte, sonst FALSE			*/

WORD dBase_move_plus(DBStruct *d)
{
	LONG ret;

	if(d->fpos>=d->recs)
		return FALSE;
	d->fpos++;
	ret=Fread(d->f,d->recsize,d->buffer);
	if(ret!=d->recsize)
	{
		dBase_move(d,d->fpos-1);
		return FALSE;
	}
	if(keytab_id_import!=-1)
		Akt_BlockX2Atari ( d->buffer, keytab_id_import, d->buffer, d->recsize );
	return TRUE;
}

/*--------------------------------------------------------------------------*/
/* Feld auslesen																														*/
/* d		: Zeiger auf die Datenbankstrukture																	*/
/* fname: Feldname des Inhalt ausgelesen werden soll												*/
/* Wert : Zeiger auf einen String in dem der Wert zurÅckgeben wird					*/

VOID dBase_read(DBStruct *d,BYTE *fname,BYTE *Wert)
{
	WORD FNr;

	if(d->fpos>d->recs || d->fpos<1)
		*Wert='\0';
	else
	{
		FNr=Feldnummer(d,fname);
		if(FNr==-1)
			*Wert='\0';
		else
		{
			memmove(Wert,&d->buffer[d->field[FNr].offset],d->field[FNr].Size);
			*(Wert+d->field[FNr].Size)='\0';
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Feld auslesen, RÅckgabe allerdings im C-Format														*/
/* d		: Zeiger auf die Datenbankstrukture																	*/
/* fname: Feldname des Inhalt ausgelesen werden soll												*/
/* Wert : Zeiger auf einen String in dem der Wert zurÅckgeben wird					*/

VOID dBase_read_typ(DBStruct *d,BYTE *fname,BYTE *Wert)
{
	BYTE ZStr[260];
	WORD FNr;
	LONG len;
	DOUBLE f;
	DATE datum;
	
	FNr=Feldnummer(d,fname);
	if(FNr==-1)
	{
		*Wert=0;
		return;
	}
	dBase_read(d,fname,ZStr);
	len=strlen(ZStr);
	switch(d->field[FNr].Typ)
	{
		case FIELD_BINARY:
			*Wert=0;
		break;
		case FIELD_ASCII:
			len--;															/* NULL Zeichen nicht beachten		*/
			while(ZStr[len]==' ')								/* Leerzeichen am Ende abschneiden*/
				ZStr[len--]='\0';
			strcpy((BYTE *)Wert,ZStr);
		break;
		case FIELD_DATE:
			datum.day=(BYTE)atoi(&ZStr[6]);
			ZStr[6]='\0';
			datum.month=(BYTE)atoi(&ZStr[4]);
			ZStr[4]='\0';
			datum.year=atoi(&ZStr[0]);
			memcpy(Wert,&datum,sizeof(DATE));
		break;
		case FIELD_NUM:
		case FIELD_NUM2:
			if(d->field[FNr].Nk==0)							/* Ganzzahlig											*/
			{
				len=strtol(ZStr,NULL,10);
				memcpy(Wert,&len,sizeof(LONG));
			}
			else
			{
				f=strtod(ZStr,NULL);
				memcpy(Wert,&f,sizeof(DOUBLE));
			}
		break;
		case FIELD_OBJEKT:
			*Wert=0;
		break;
		case FIELD_LOGIC:
			strcpy((BYTE *)Wert,ZStr);
		break;
		case FIELD_MEMO:
		case FIELD_PICTURE:
			*Wert=0;
		break;
	}
}

/*
/*--------------------------------------------------------------------------*/
/* Neu dBASE-Datei anlegen																									*/
/* d		: Zeiger auf die Datenbankstrukture																	*/
/* name	: Dateiname + Pfad																									*/
/* RÅckgabe: TRUE=Datei konnte erfolgreich angelegt werden.									*/
/*					 FALSE=Fehler beim erzeugen																			*/

WORD dBase_create(DBStruct *f,BYTE *name)
{
	BYTE x[2];
	WORD i,o;
	WORD ta,mo,ja,wt;
	dBASEIII_Header Header;
	dBASEIII_FIELD Field;
	struct date Datum;

	if(file_exist(name))
	{
		i=Note(ALERT_NORM,2,DBASE_DATEI_VORHANDEN);
		if(i==2)
			return FALSE;
	}
	f->f=fopen(name,"rb+");
	if(f->f==NULL)
	{
		Note(ALERT_NORM,2,DATEI_ERZEUGEN);
		return FALSE;
	}
  memset(Header,0,sizeof(dBASEIII_Header));
  Header.Version=DBASEIII;
  getdate(Datum);
  Header.Jahr=Datum.da_year-1900;
  Header.Monat=Datum.da_mon;
  Header.Tag=Datum.da_day;
  Header.Headersize=felder*$20+$21;
  f->rsize=1;
  for(i=1; i<=felder; i++)
  {
    if(feld[i].typ==FIELD_DATE)
    	feld[i].size=8;
    if(feld[i].typ==FIELD_LOGIC)
    	feld[i].size=1;
    f->rsize +=feld[i].size;
  }
  Header.Recsize=f->rsize;
  blockwrite(datei,Header,SizeOf(Header));
  hdsize=header.hdsize+1;
  o=0;
  for(i=1; i<=felder; i++)
  {
  	fillchar(XFeld,sizeof(XFeld),0);
    move(feld[i].name[1],XFeld.name,length(feld[i].name));
    XFeld.typ:=feld[i].typ;
    XFeld.size:=feld[i].size;
    if feld[i].typ='N' then XFeld.nk:=feld[i].nk;
    blockwrite(datei,XFeld,SizeOf(XFeld));
    feld[i].off:=o;
    o +=feld[i].size;
  }
  f->modi=false;
  DbEOF:=true;
  getmem(buff,rsize);
  f->recs=0;
  f->FPos=1;
  x[1]:=^M; x[2]:=^Z;
  blockwrite(datei,x,2);
}
*/

/*--------------------------------------------------------------------------*/
/* Leeren Datensatz anhÑngen																								*/

WORD dBase_append(DBStruct *d)
{
	LONG ret;

	d->recs++;
	memset(d->buffer,' ',d->recsize);
	d->buffer[d->recsize]=0x1a;
	ret=Fseek(d->headersize+(d->recs-1)*d->recsize-1,d->f,0);
	if(ret < 0)
		return FALSE;
	ret=Fwrite(d->f,d->recsize+1,d->buffer);
	if(ret < 0 || ret!=d->recsize+1)
		return FALSE;
	d->fpos=d->recs;
	d->modi=TRUE;
	return TRUE;
}

/*--------------------------------------------------------------------------*/
/* Ist Datensatz gelîscht?																									*/
/* d		: Zeiger auf die Datenbankstrukture																	*/
/* RÅckgabe: TRUE = Datensatz ist gelîscht, sonst FALSE											*/

WORD dBase_deletet(DBStruct *d)
{
	if(d->fpos<1 && d->fpos>d->recs)
		return TRUE;
	if(d->buffer[0]==' ')
		return FALSE;
	return TRUE;
}

/*--------------------------------------------------------------------------*/
/* Ermittelt die Feldnummer	zu einem Feldname																*/
/* d		: Zeiger auf die Datenbankstrukture																	*/
/* fname: Feldname des Inhalt ausgelsen werden soll													*/
/* RÅckgabe: Feldnummer oder -1 falls keine Feld gefunden wurde							*/

WORD Feldnummer(DBStruct *d,BYTE *fname)
{
	WORD i;
	
	i=0;
	while(i<d->fields && strcmp(&d->field[i].Name[0],fname)!=0)
		i++;
	if(i==d->fields)
		return -1;
	return i;
}

/*--------------------------------------------------------------------------*/

VOID flip_word(BYTE *adr)
{
	BYTE c;
	
	c=adr[0];
	adr[0]=adr[1];
	adr[1]=c;
}

/*--------------------------------------------------------------------------*/

VOID flip_long(WORD *adr)
{
	WORD  i;
	
	i=adr[0];
	adr[0]=adr[1];
	adr[1]=i;
	flip_word((BYTE *)&adr[0]);
	flip_word((BYTE *)&adr[1]);
}
