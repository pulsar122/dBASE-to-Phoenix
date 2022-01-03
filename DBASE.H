/*--------------------------------------------------------------------------*/


#define DBASEII 					0x02
#define DBASEIII					0X03
#define DBASEIV						0X04
#define DBASEV						0X05
#define DBASEIII_MEMO			0X83
#define DBASEIV_MEMO			0X8B
#define DBASEIV_SQL				0X8E
#define FOXPRO_MEMO				0xf5

#define DBASEII_HEADSIZE 	520

#define FIELD_BINARY	'B'
#define FIELD_ASCII		'C'
#define FIELD_DATE		'D'
#define FIELD_NUM2		'F'
#define FIELD_OBJEKT	'G'
#define	FIELD_LOGIC		'L'
#define FIELD_NUM			'N'
#define FIELD_MEMO		'M'
#define FIELD_PICTURE	'P'

#define MAX_FIELDS	100

typedef struct
{
	BYTE Version;
	WORD Records;
	BYTE Tag;
	BYTE Monat;
	BYTE Jahr;
	WORD Recsize;
}dBASEII_Header;

typedef struct
{
	BYTE Name[11];
	BYTE Typ;
	BYTE Fieldsize;
	WORD Adr;
	BYTE Nk;
}dBASEII_FIELD;

typedef struct
{
	BYTE Version;
	BYTE Jahr;
	BYTE Monat;
	BYTE Tag;
	ULONG Records;
	WORD Headersize;
	WORD Recsize;
	BYTE Res[20];
}dBASEIII_Header;

typedef struct
{
	BYTE Name[11];
	BYTE Typ;
	LONG Adr;
	BYTE Fieldsize;
	BYTE Nk;
	BYTE Res[14];
}dBASEIII_FIELD;

typedef struct
{
	BYTE Name[11];
	BYTE Typ;
	BYTE Size;
	BYTE Nk;
	LONG offset;
}FIELD;

typedef struct
{
	WORD typ;																		/* Type der Datenbank					*/
	WORD fields;																/* Anzahl der Felder					*/
	FIELD field[MAX_FIELDS];										/* Felderliste								*/
	WORD headersize;
	UWORD recsize;															/* Grîûe eines Datensatz			*/
	ULONG recs;																	/* Anzahl der DatensÑtze			*/
	BYTE Tag,Monat;															/* letzte énderung						*/
	WORD Jahr;
	WORD modi;																	/* Flag fÅr "geÑndert" 				*/
	FILE *f;																		/* Dateihandle								*/
	ULONG fpos;																	/* Datei-Zeiger								*/
	BYTE *buffer;																/* Zeiger auf Datensatzbuffer */
}DBStruct;

EXTERN WORD dBase_open(DBStruct *d,BYTE *file);
EXTERN VOID dBase_close(DBStruct *d);
EXTERN WORD dBase_move(DBStruct *d,ULONG pos);
EXTERN WORD dBase_move_plus(DBStruct *d);
EXTERN VOID dBase_read(DBStruct *d,BYTE *fname,BYTE *Wert);
EXTERN VOID dBase_read_typ(DBStruct *d,BYTE *fname,BYTE *Wert);
EXTERN WORD dBase_deletet(DBStruct *d);
