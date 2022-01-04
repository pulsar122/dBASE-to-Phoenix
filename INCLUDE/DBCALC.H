/*****************************************************************************
 *
 * Module : DBCALC.H
 * Author : Jürgen Geiß
 *
 * Creation date    : 01.07.89
 * Last modification: 14.11.92
 *
 *
 * Description: This module defines the database calculation routines.
 *
 * History:
 * 14.11.92: HPVOID used as data type of parameter buffer in db_excute
 * 31.10.92: Far pointer definitions for DLL implementation added
 * 26.10.92: db_compile and db_execute have INT results
 * 25.10.92: Module header added
 * 01.07.89: Creation of body
 *****************************************************************************/

#ifndef _DBCALC_H
#define _DBCALC_H

/****** DEFINES **************************************************************/

/****** TYPES ****************************************************************/

/****** VARIABLES ************************************************************/

/****** FUNCTIONS ************************************************************/

GLOBAL INT WINAPI db_compile _((LPBASE base, SHORT table, LPSYSCALC syscalc, LPSHORT line, LPSHORT col, LPFORMAT form));
GLOBAL INT WINAPI db_execute _((LPBASE base, SHORT table, LPCALCCODE calccode, HPVOID buffer, LONG count, LPFORMAT form));

#endif /* _DBCALC_H */

