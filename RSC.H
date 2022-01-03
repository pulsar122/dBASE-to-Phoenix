/*--------------------------------------------------------------------------*/

EXTERN OBJECT	**tree_addr;

EXTERN OBJECT *menu_tree;
EXTERN OBJECT *dbase_menu_tree;
EXTERN OBJECT *phoenix_menu_tree;
EXTERN OBJECT *copy_tree;
EXTERN OBJECT *main_tree;
EXTERN OBJECT *login_tree;
EXTERN OBJECT *dbase_tree;
EXTERN OBJECT *phoenix_tree;
EXTERN OBJECT *keytab_tree;
EXTERN OBJECT *icon_tree;
EXTERN OBJECT *wdialog_tree;

VOID init_rsc(VOID);
BYTE *HoleText(WORD Baum,WORD Nummer,BYTE *String);
WORD Note(WORD priority,WORD def,WORD index);
