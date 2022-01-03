/*
Database: RAETSEL

Kreuzwortr„tseldaten
  Frage                TEXT  (66)    MANDATORY, INDEX, PRIMARY
  Buchstabenanzahl     WORD          
  Antworten            TEXT  (200)   MANDATORY
----------------------------------------------------------------------
*/



#include <portab.h>
#include <import.h>
#include <phoenix.h>
#include <stdio.h>
#include <string.h>
#include <sys_gem2.h>
#include "ph_base.h"


/* PHOENIX System C-Interface */
/* Database: RAETSEL */

#define TBL_Kreuzwortraetseldaten 20

typedef struct
{
  LONG      DbAddress;             /*     0 */
  BYTE      Frage [68];            /*     4 */
  WORD      Buchstabenanzahl;      /*    72 */
  BYTE      Antworten [202];       /*    74 */
} Kreuzwortratseldaten;

LOCAL VOID loesche(BYTE *Str);

/*--------------------------------------------------------------------------*/

VOID joerg(VOID)
{
	BYTE Daten[400];
	LONG i;
	LONG step;
	LONG error;
	LONG rec;
	FILE *rep;
	CURSOR *cursor;
	PhoenixStruct *Zeiger;
	Kreuzwortratseldaten Datensatz;
	
	Zeiger=(PhoenixStruct *)malloc(sizeof(PhoenixStruct));
	if(Zeiger==NULL)
		return;
	if(phoenix_open(Zeiger,"x:\\phoenix\\daten\\rate_mal\\raetsel.ind","",""))
/*	X:\PHOENIX\DATEN\RATE_MAL\RAETSEL.IND*/
	{
		rep=fopen("x:\\phoenix\\daten\\rate_mal\\test.asc","wb");
		if(rep!=NULL)
		{
			ShowBee();
		  cursor=db_newcursor (Zeiger->Base);
		  if(cursor!=NULL)
		  {
		    if(db_initcursor(Zeiger->Base, TBL_Kreuzwortraetseldaten,1,ASCENDING,cursor))
		    {
		    	rec=Zeiger->Table[0].recs;
		    	ShowStatus("Datenbank exportieren",NULL,0L,rec);
		    	i=0;
		    	step=1L;
		      while(db_movecursor(Zeiger->Base,cursor,step))
		      {
		      	i++;
			    	ShowStatus(NULL,NULL,i,rec);
		        if(db_read(Zeiger->Base,TBL_Kreuzwortraetseldaten, &Datensatz, cursor, 0L,FALSE))
		        {
		        	loesche(&Datensatz.Frage[0]);
		        	loesche(&Datensatz.Antworten[0]);
		        	sprintf(&Daten[0],"%s\r\n%i\r\n%s\r\n",Datensatz.Frage,Datensatz.Buchstabenanzahl,Datensatz.Antworten);
		        	error=fwrite(&Daten[0],1,strlen(Daten),rep);
		        	if(error!=strlen(Daten))
		        	{
		        		Alert(ALERT_NORM,1,"[3][Fehler beim Schreiben!][[OK]");
		        		break;
		        	}
		        }
		        else
		          status_phoenix(Zeiger->Base);
		      }
		     EndStatus();
		    }
		  }
		  fclose(rep);
		}
		phoenix_close(Zeiger);
	}
	free(Zeiger);
	ShowArrow();
}

/*--------------------------------------------------------------------------*/

VOID loesche(BYTE *Str)
{
	BYTE *s;
	BYTE ZStr[300];
	WORD i;
	
	i=0;
	s=Str;
	strcpy(ZStr,s);
	while(*s!='\0')
	{
		if(*s!='\r' && *s!='\n')
			ZStr[i++]=*s;
		s++;
	}
	ZStr[i]='\0';
	strcpy(Str,ZStr);
}