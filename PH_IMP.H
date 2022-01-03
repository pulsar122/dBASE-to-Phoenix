/*--------------------------------------------------------------------------*/

EXTERN BYTE ph_path[300];										/* Phoenix Path									*/
EXTERN PhoenixStruct *phoenix_zeiger;
EXTERN BYTE Datentypen[16][30];
EXTERN BYTE ph_name[50],ph_passwort[50];

EXTERN VOID open_phoenix(VOID);
EXTERN WORD handle_login(WORD msg,WORD button,DIALOG_INFO *inf);
EXTERN VOID close_phoenix(VOID);
EXTERN WORD handle_phoenix(WORD msg,WORD button,DIALOG_INFO *inf);
EXTERN VOID init_phoenix_Import(VOID);
EXTERN VOID term_phoenix_Import(VOID);

