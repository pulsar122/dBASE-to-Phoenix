
#define MAX_TABLE		50
#define MAX_COLUMN	100

typedef struct
{
	BYTE Name[MAX_FIELDNAME+1];							/* Spalte- bzw. Feldname					*/
	WORD Typ;																/* Art der Spalte									*/
	LONG Size;															/* Gr”že in Bytes									*/
	LONG Offset;														/* Adresse im Record							*/
}PhoenixField;


typedef struct
{
	BYTE Name[MAX_TABLENAME+1];							/* Tabellenname										*/
	WORD CountColumn;												/* Anzahl der Spalten (Felder)		*/
	PhoenixField Column[MAX_COLUMN];
	LONG	Size;															/* Gr”že eines Datensatz					*/
	LONG recs;															/* Anzahl der Eintr„ge						*/
}PhoenixTable;


typedef struct
{
	TIMESTAMP created;											/* Erstellung der Datenbank				*/
	TIMESTAMP lastuse;											/* Letzte Benutzung der Datenbank	*/
	WORD CountTable;												/* Anzahl der Tabellen						*/
	PhoenixTable Table[MAX_TABLE];
	LPBASE Base;														/* Zeiger auf Datenbankstruktur		*/
	WORD reorg;
}PhoenixStruct;

EXTERN WORD phoenix_open(PhoenixStruct *d,BYTE *path,BYTE *User,BYTE *Password);
EXTERN WORD phoenix_close(PhoenixStruct *d);
EXTERN VOID init_phoenix(VOID);
EXTERN WORD	status_phoenix(LPBASE *base);
