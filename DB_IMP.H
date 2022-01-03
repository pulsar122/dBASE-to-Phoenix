/*--------------------------------------------------------------------------*/

EXTERN BYTE db_path[300];										/* dBASE Path										*/
EXTERN DBStruct *dbase_zeiger;

EXTERN VOID open_dbase(VOID);
EXTERN VOID close_dbase(VOID);
EXTERN WORD handle_dbase(WORD msg,WORD button,DIALOG_INFO *inf);
EXTERN VOID hole_typ(BYTE Typ,BYTE *Text);
EXTERN VOID dbase_daten_anzeigen(VOID);
EXTERN VOID init_dBASE_Import(VOID);
EXTERN VOID term_dBASE_Import(VOID);
