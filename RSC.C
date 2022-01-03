/*--------------------------------------------------------------------------*/
/* Aufprogramm von db2ph																										*/

#include <portab.h>
#include <import.h>
#include <phoenix.h>
#include <sys_gem2.h>
#include "db2ph.h"
#include "rsc.h"
#include "version.h"

/*--------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																													*/

/*--------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																										*/

OBJECT *menu_tree;
OBJECT *dbase_menu_tree;
OBJECT *phoenix_menu_tree;
OBJECT *copy_tree;
OBJECT *main_tree;
OBJECT *login_tree;
OBJECT *dbase_tree;
OBJECT *phoenix_tree;
OBJECT *keytab_tree;
OBJECT *icon_tree;
OBJECT *wdialog_tree;

/*--------------------------------------------------------------------------*/
/* DEFINES																																	*/

/*--------------------------------------------------------------------------*/
/* TYPES																																		*/

/*--------------------------------------------------------------------------*/
/* FUNKTIONS																																*/

/*--------------------------------------------------------------------------*/
/* LOCALE VARIABLES																													*/

/*--------------------------------------------------------------------------*/
/* Resourceen init																													*/

VOID init_rsc(VOID)
{
	BYTE ZStr[30];
	UWORD Version;
	
	menu_tree=RscAdr(R_TREE,MENU);

	dbase_menu_tree=RscAdr(R_TREE,DBASESTRUKMENU);

	phoenix_menu_tree=RscAdr(R_TREE,PHOENIXSTRUKMENU);
	
	copy_tree=RscAdr(R_TREE,COPYRIGHT);
	NewDialog(copy_tree);
  SetText(copy_tree,CVERSION,VERSION);	/* Versionsnummer eintragen					*/
  SetText(copy_tree,CDATUM,__DATE__);		/* Versionsdatum eintragen					*/
  SetText(copy_tree,CSYSGEM,SysGemVerStr());	/* SysGem Version eintragen		*/
	Version=db_lib_version();
	sprintf(ZStr,"Phoenix/BASE: %X.%X",Version>>8,Version & 0xff);
  SetText(copy_tree,CPHOENIX,ZStr);			/* Phoenix/BASE eintragen						*/

	main_tree=RscAdr(R_TREE,MAIN);
	NewDialog(main_tree);
	
	login_tree=RscAdr(R_TREE,LOGIN);
	NewDialog(login_tree);

	dbase_tree=RscAdr(R_TREE,DBASESTRUKTUR);
	NewDialog(dbase_tree);

	phoenix_tree=RscAdr(R_TREE,PHOENIXSTRUKT);
	NewDialog(phoenix_tree);

	keytab_tree=RscAdr(R_TREE,KEYTAB);
	NewDialog(keytab_tree);
	
	wdialog_tree=RscAdr(R_TREE,PRN_SUB);
	icon_tree=RscAdr(R_TREE,PRN_ICON);
}
