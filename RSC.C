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

OBJECT **tree_addr;

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
OBJECT *texte_tree;
OBJECT *phoenix_base_tree;
OBJECT *Anleitung_tree;
OBJECT **alertmsg_tree;

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
	RSHDR		*rsh;
	
	rsh = *((RSHDR **)(&_GemParBlk.global[7]));					/* Adresse des Resource-Headers Åber global[7/8] holen */

	tree_addr = (OBJECT **)(((UBYTE *)rsh) + rsh->rsh_trindex);	/* Zeiger auf die Objektbaumtabelle holen */

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

	texte_tree=RscAdr(R_TREE,TEXTE);

	phoenix_base_tree=RscAdr(R_TREE,PHOENIX_BASE);

	Anleitung_tree=RscAdr(R_TREE,ANLEITUNGS_TEXTE);

	rsrc_gaddr(R_FRSTR,DBASE_LESE_FEHLER,&alertmsg_tree);
}

/*--------------------------------------------------------------------------*/
/* Text aus der Resource-Datei holen																				*/
/* Baum		: Nummer der Objektbaum, in welchem das Textfeld liegt						*/
/* Nummer	: Nummer des Textobjekt																						*/
/* String	: Adresse auf einen String in welchen der Text kopiert wird.			*/
/* RÅckgabe: Adresse ab der sich der Text befindet													*/

BYTE *HoleText(WORD Baum,WORD Nummer,BYTE *String)
{
	OBJECT *tree;

	switch(Baum)
	{
		case TEXTE:
			tree=texte_tree;
		break;
		case PHOENIX_BASE:
			tree=phoenix_base_tree;
		break;
		case ANLEITUNGS_TEXTE:
			tree=Anleitung_tree;
		break;
	}
	if(String!=NULL)
		strcpy(String, tree[Nummer].ob_spec.free_string);

	return tree[Nummer].ob_spec.free_string;
}

/*--------------------------------------------------------------------------*/
/* Zeigte eine Alertbox, als Text und Button kommt ein String aus der				*/
/* Resource-Datei.																													*/
/* priority	: 'PrioritÑt'																										*/
/* def			: Nummer des Default-Konpfes																		*/
/* index		: Nummer aus der Resoucre-Datei																	*/
/* RÅckgabe	: Nummer des Buttons der betÑtigt wurde.												*/

WORD Note(WORD priority,WORD def,WORD index)
{
	return Alert(priority,def,(BYTE *)alertmsg_tree[index]);
}
