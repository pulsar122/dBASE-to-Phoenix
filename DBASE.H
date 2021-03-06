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
	BYTE Res[2];
																				/* Ab dBASE IV 											*/
	UCHAR transaction;  /* 1 byte, mark for not ended transactions 						*/	UCHAR crypted;      /* 1 byte, mark for cryption (dBASE IV) 							*/	UCHAR net[12];      /* 12 bytes, reserved for dBASE IV+5.0 network version */	UCHAR mdx;          /* 1 byte, 1 = MDX is there, 0 = no MDX-file 					*/	UCHAR code;			/* foxpro, dBASE IV2.0, 5.0 save here information about the code page */	UCHAR reserved2[2]; /* 2 bytes, reserved */}dBASEIII_Header;

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
	UWORD recsize;															/* Gr��e eines Datensatz			*/
	LONG recs;																	/* Anzahl der Datens�tze			*/
	BYTE Tag,Monat;															/* letzte �nderung						*/
	WORD Jahr;
	WORD modi;																	/* Flag f�r "ge�ndert" 				*/
	WORD f;																			/* Dateihandle								*/
	LONG fpos;																	/* Datei-Zeiger								*/
	BYTE *buffer;																/* Zeiger auf Datensatzbuffer */
}DBStruct;

EXTERN WORD dBase_open(DBStruct *d,BYTE *file);
EXTERN VOID dBase_close(DBStruct *d);
EXTERN WORD dBase_move(DBStruct *d,LONG pos);
EXTERN WORD dBase_move_plus(DBStruct *d);
EXTERN VOID dBase_read(DBStruct *d,BYTE *fname,BYTE *Wert);
EXTERN VOID dBase_read_typ(DBStruct *d,BYTE *fname,BYTE *Wert);
EXTERN WORD dBase_deletet(DBStruct *d);
