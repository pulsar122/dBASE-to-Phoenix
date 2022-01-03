/*----------------------------------------------------------------------------*/
/* Ausgabe des Copyright Dialog																								*/

#include <stdio.h>
#include <portab.h>
#include <stdlib.h>
#include <string.h>
#include <sys_gem2.h>
#include "global.h"
#include "rsc.h"

/*----------------------------------------------------------------------------*/
/* EXTERNE VARIABLE																														*/

/*----------------------------------------------------------------------------*/
/* EXTPORTIERTE VARIABLE																											*/

/*----------------------------------------------------------------------------*/
/* DEFINES																																		*/

/*----------------------------------------------------------------------------*/
/* TYPES																																			*/

/*----------------------------------------------------------------------------*/
/* FUNKTIONS																																	*/

LOCAL WORD handle_copyright(WORD msg,WORD button,DIALOG_INFO *inf);

/*----------------------------------------------------------------------------*/
/* LOCALE VARIABLES																														*/

/*----------------------------------------------------------------------------*/
/* Zeigt den Copyright-Dialog an																							*/

VOID zeige_copyright(VOID)
{
	WindowDialog('COPY',-1,-1,"| Programminfo ","",FALSE,FALSE,copy_tree,NULL,0,NULL,handle_copyright);
}

/*----------------------------------------------------------------------------*/
/* Bearbeiten der Reaktion auf den Copyrightdialog														*/

WORD handle_copyright(WORD msg,WORD button,DIALOG_INFO *inf)
{
	switch(msg)
	{
    case SG_UNDO:																/* Undo-Taste bet„tigt				*/
			return SG_CLOSE;
		case SG_HELP:																/* Help-Taste bet„tigt.				*/
 			CallOnlineHelp("Programminfo...");
 		break;
 		case SG_START:
 		break;
		case SG_END:
				return SG_CLOSE;
		case SG_POSX:
			SetObjX(inf->tree,0,button);							/* Wegen SysGem-Bug						*/
		break;
		case SG_POSY:
			SetObjY(inf->tree,0,button);							/* Wegen SysGem-Bug						*/
		break;
	}
	return SG_CONT;
}
