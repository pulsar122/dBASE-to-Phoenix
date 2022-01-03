/*--------------------------------------------------------------------------*/
/* Hauptprogramm von db2ph																									*/

#include <portab.h>
#include <import.h>
#include <phoenix.h>
#include <sys_gem2.h>
#include "copyrigh.h"
#include "db_2_ph.h"
#include "dbase.h"
#include "db_imp.h"
#include "drucker.h"
#include "key.h"
#include "make.h"
#include "ph_2_db.h"
#include "ph_2_pap.h"
#include "ph_base.h"
#include "ph_imp.h"
#include "db2ph.h"
#include "rsc.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/

/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

PARAMETER Para;													/* Programmparameter								*/

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

LOCAL VOID init_programm(VOID);
LOCAL VOID init_SysGem(VOID);
LOCAL VOID kommandozeile(int argc, const char *argv[]);
LOCAL VOID exit_programm(VOID);
LOCAL WORD handle_menue(WORD *msg, WORD item);

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

BYTE home[300];

/*--------------------------------------------------------------------------*/
/* Hauptprogrammm 																													*/

WORD main(int argc, const char *argv[])
{
  if(InitGem("",'DBPH',"dBASE to Phoenix")>0)
  {
  	GetParStruct(&Para);								/* Parameterstruktur ermitteln			*/
  	if(Para.acc_entry==-2)							/* Als Programm gestartet ?					*/
  	{
  		if(LoadResource("DB2PH.RSC",FALSE))
			{
  			ShowRotor();
				init_programm();
				if(argc>1)											/* Komandozeile vorhanden						*/
					kommandozeile(argc,argv);
				EndRotor();
			  SetDeskTopMenu(menu_tree,handle_menue);	/* MenÅ setzen und anzeigen				*/
 				HandleSysGem();
 				exit_programm();
    	}
    	ExitGem();
    }
  }
  return 0;
}

/*--------------------------------------------------------------------------*/
/* Initalisiert die einzelnen Module																				*/

VOID init_programm(VOID)
{
	Pdomain(1);
	init_rsc();														/* Resource init										*/
	init_SysGem();
	ShowMessage(HoleText(TEXTE,TEXT_028,NULL));
  db_path[0]=(BYTE)Dgetdrv()+'A';				/* Aktuelles Laufwerk in Pfad 			*/
  *(db_path+1) = ':';
	Dgetpath(db_path+2,0);								/* Aktueller Dateipath							*/
	strcpy(ph_path,db_path);
	strcpy(home,db_path);									/* INF-Path setzen									*/
	strcat(home,"\\db2ph.inf");
	if(FileExists(home))									/* Konfigdatei vorhanden?						*/
	{
		if(LoadConfig(home)==0L)						/* Konfigdatei laden								*/
		{
			ShowArrow();
	  	Note(ALERT_NORM,1,DEFEKTE_INF);
		}
	}
/*	init_drucker();*/
	init_phoenix();												/* Phoenix/BASE init								*/
	init_phoenix_Import();
	init_dBASE_Import();
	init_make();
	init_keytab();
	menu_ienable(menu_tree,MIMPORT,FALSE);
	if(!wdialog)													/* Kein WDialog vorhanden?					*/
	{
/*		menu_ienable(dbase_menu_tree,DDRUCKENSTRUKT,FALSE);*/
	}
	EndMessage();
}

/*--------------------------------------------------------------------------*/
/* Initialisiert SysGem																											*/

VOID init_SysGem(VOID)
{
	SetLanguage(FALSE);										/* Deutsche Sprache									*/
	SetProgramName("[dB2PH]");
	SetIconifyName("dB2PH");
	SetAlertTitle("|[dB2PH]");
	TellKeyStrokes(TRUE);
	SetMonoEditFrame(TRUE);
	UseRoundButtons(TRUE);
	DialPosXY(TRUE);
	SetReturn(FALSE);
	Enable3D();
	SetOnlineHelp("ST-GUIDE","","*:\\db2ph.HYP");
}

/*--------------------------------------------------------------------------*/

VOID kommandozeile(int argc, const char *argv[])
{
		
	
}

/*--------------------------------------------------------------------------*/
/* Porgramm beenden																													*/

VOID exit_programm(VOID)
{
	SaveConfig(home);											/*	Konfig speichern								*/
	term_dBASE_Import();
	term_phoenix_Import();
}

/*--------------------------------------------------------------------------*/
/* MenÅzeile bearbeiten																											*/

WORD handle_menue(WORD *msg, WORD item)
{
	LONG top;

	switch (item)
  {
  	case -1:														/* Help-Taste betÑtigt							*/
 			CallOnlineHelp(HoleText(ANLEITUNGS_TEXTE,HELP_003,NULL));
  	break;
  	case MPROGRAMMINFO:									/* Copyrightbox darstellen					*/
	 		zeige_copyright(); 
  	break;
  	case MOEFFNEPH:											/* Phoenixdatenbankstruktur îffnen	*/
  		open_phoenix();
  	break;
  	case MPHCLOSE:											/* Phoenixdatenbankstruktur schliesse*/
  		close_phoenix();
  	break;
  	case MOEFFNEDBF:										/* dBAES Datenbankstruktur îffnen		*/
  		open_dbase();
  	break;
  	case MDBCLOSE:											/* dBAES Datenbankstruktur schliesse*/
  		close_dbase();
  	break;
  	case MENDE:													/* Programm beenden									*/
  		return SG_TERM;
  	case MPHSTRUKTUR:										/* Phoenixdatenbankstruktur anzeigen*/
  		WindowDialog('PHST',-1,-1,HoleText(TEXTE,TEXT_029,NULL),"",FALSE,FALSE,phoenix_tree,phoenix_menu_tree,0,NULL,handle_phoenix);
  	break;
  	case MDBASESTRUCT:									/* dBASE Datenbankstruktur anzeigen */
  		WindowDialog('DBST',-1,-1,HoleText(TEXTE,TEXT_030,NULL),"",FALSE,FALSE,dbase_tree,dbase_menu_tree,0,NULL,handle_dbase);
  	break;
  	case MDBASEDATEN:
  		dbase_daten_anzeigen();
  	break;
  	case MIMPORT:												/* Datenimportieren									*/
  		WindowDialog('IMPO',-1,-1,"dBASE to Phoenix","",FALSE,FALSE,main_tree,NULL,0,NULL,handle_import);
  	break;
  	case MERZEUGEN:
  		DbaseToPhoenix();
  	break;
  	case MPHOENIXDBASE:									/* Phoenix nach dBase									*/
  		PhoenixToDbase();
  	break;
  	case MPH_PAP:												/* Phoenix nach PapyrusBase						*/
  		PhoenixToPapyrus ();
  	break;
		case MKEYTAB:												/* Filter einstellen									*/
			import_keytab();
		break;
  	case MFSCHLIESSEN:									/* Oberstes Fenster schlieûen					*/
  		top=GetTopWindowId();							/* Top Window ermitteln								*/
  		if(top!=0L)												/* Gehîrt das Fenster zu mir?					*/
  			CloseWindowById(top);						/* Schlieûmeldung verschicken					*/
  	break;
  	case MFWECHSEL:											/* NÑchstes Fenster										*/
  		CycleWindow(FALSE);
  	break;
  }
  return SG_CONT;
}

