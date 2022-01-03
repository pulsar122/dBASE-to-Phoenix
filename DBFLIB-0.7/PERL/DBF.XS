/*
 * published under the GNU Library General Public License
 * rasca, berlin 1996
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#ifdef __cplusplus
}
#endif

#include <dbf.h>

static int
not_here(s)
char *s;
{
    croak("%s not implemented on this architecture", s);
    return -1;
}

static double
constant(name, arg)
char *name;
int arg;
{
    errno = 0;
    switch (*name) {
    case 'A':
	break;
    case 'B':
	break;
    case 'C':
	if (strEQ(name, "CURRENT"))
#ifdef CURRENT
	    return CURRENT;
#else
	    goto not_there;
#endif
	break;
    case 'D':
	if (strEQ(name, "DB_DBASE3"))
#ifdef DB_DBASE3
	    return DB_DBASE3;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DB_FOXPRO"))
#ifdef DB_FOXPRO
	    return DB_FOXPRO;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DB_IGNORE_DEL"))
#ifdef DB_IGNORE_DEL
	    return DB_IGNORE_DEL;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DB_IGNORE_MDX"))
#ifdef DB_IGNORE_MDX
	    return DB_IGNORE_MDX;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DB_IGNORE_MEMO"))
#ifdef DB_IGNORE_MEMO
	    return DB_IGNORE_MEMO;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DB_READ"))
#ifdef DB_READ
	    return DB_READ;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DB_WRITE"))
#ifdef DB_WRITE
	    return DB_WRITE;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DEL_FLAG"))
#ifdef DEL_FLAG
	    return DEL_FLAG;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DF_DD_MM_YYYY"))
#ifdef DF_DD_MM_YYYY
	    return DF_DD_MM_YYYY;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DF_MM_DD_YYYY"))
#ifdef DF_MM_DD_YYYY
	    return DF_MM_DD_YYYY;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DF_YYYYMMDD"))
#ifdef DF_YYYYMMDD
	    return DF_YYYYMMDD;
#else
	    goto not_there;
#endif
	if (strEQ(name, "DF_YYYYMMDD_I"))
#ifdef DF_YYYYMMDD_I
	    return DF_YYYYMMDD_I;
#else
	    goto not_there;
#endif
	break;
    case 'E':
	if (strEQ(name, "EO_FDESC"))
#ifdef EO_FDESC
	    return EO_FDESC;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_ALLOC"))
#ifdef E_ALLOC
	    return E_ALLOC;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_FLD_NR"))
#ifdef E_FLD_NR
	    return E_FLD_NR;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_HANDLE"))
#ifdef E_HANDLE
	    return E_HANDLE;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_HEADER"))
#ifdef E_HEADER
	    return E_HEADER;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_IO"))
#ifdef E_IO
	    return E_IO;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_REC_NR"))
#ifdef E_REC_NR
	    return E_REC_NR;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_UNKNOWN"))
#ifdef E_UNKNOWN
	    return E_UNKNOWN;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_VERSION"))
#ifdef E_VERSION
	    return E_VERSION;
#else
	    goto not_there;
#endif
	if (strEQ(name, "E_WRONG_MODE"))
#ifdef E_WRONG_MODE
	    return E_WRONG_MODE;
#else
	    goto not_there;
#endif
	break;
    case 'F':
	if (strEQ(name, "FT_BINARY"))
#ifdef FT_BINARY
	    return FT_BINARY;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_BOOLEAN"))
#ifdef FT_BOOLEAN
	    return FT_BOOLEAN;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_CHARS"))
#ifdef FT_CHARS
	    return FT_CHARS;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_DATE"))
#ifdef FT_DATE
	    return FT_DATE;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_FLOAT"))
#ifdef FT_FLOAT
	    return FT_FLOAT;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_GENERAL"))
#ifdef FT_GENERAL
	    return FT_GENERAL;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_MEMO"))
#ifdef FT_MEMO
	    return FT_MEMO;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_NUMBER"))
#ifdef FT_NUMBER
	    return FT_NUMBER;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_OBJECT"))
#ifdef FT_OBJECT
	    return FT_OBJECT;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_PICTURE"))
#ifdef FT_PICTURE
	    return FT_PICTURE;
#else
	    goto not_there;
#endif
	if (strEQ(name, "FT_UNKNOWN"))
#ifdef FT_UNKNOWN
	    return FT_UNKNOWN;
#else
	    goto not_there;
#endif
	break;
    case 'G':
	break;
    case 'H':
	break;
    case 'I':
	break;
    case 'J':
	break;
    case 'K':
	break;
    case 'L':
	break;
    case 'M':
	if (strEQ(name, "MEMO_DBASE"))
#ifdef MEMO_DBASE
	    return MEMO_DBASE;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MEMO_FOXPRO"))
#ifdef MEMO_FOXPRO
	    return MEMO_FOXPRO;
#else
	    goto not_there;
#endif
	break;
    case 'N':
	if (strEQ(name, "NOT_DEL_FLAG"))
#ifdef NOT_DEL_FLAG
	    return NOT_DEL_FLAG;
#else
	    goto not_there;
#endif
	break;
    case 'O':
	break;
    case 'P':
	break;
    case 'Q':
	break;
    case 'R':
	break;
    case 'S':
	break;
    case 'T':
	break;
    case 'U':
	break;
    case 'V':
	break;
    case 'W':
	break;
    case 'X':
	if (strEQ(name, "XBASE_MEMO_MASK"))
#ifdef XBASE_MEMO_MASK
	    return XBASE_MEMO_MASK;
#else
	    goto not_there;
#endif
	if (strEQ(name, "XBASE_SQL_MASK"))
#ifdef XBASE_SQL_MASK
	    return XBASE_SQL_MASK;
#else
	    goto not_there;
#endif
	if (strEQ(name, "XBASE_VERSION_MASK"))
#ifdef XBASE_VERSION_MASK
	    return XBASE_VERSION_MASK;
#else
	    goto not_there;
#endif
	break;
    case 'Y':
	break;
    case 'Z':
	break;
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}


MODULE = Dbf		PACKAGE = Dbf
PROTOTYPES: ENABLE

double
constant(name,arg)
	char *		name
	int		arg


int
db_open (name, mode)
	char*	name
	char*	mode
	CODE:
		RETVAL = dbOpen(name, constant(mode));
	OUTPUT:
		RETVAL


int
db_create (path, nof, type)
	char* path
	unsigned short nof
	char* type
	CODE:
		RETVAL = dbCreate(path, nof, constant(type));
	OUTPUT:
		RETVAL


int
db_close (hnd)
	int hnd
	CODE:
		RETVAL = dbClose(hnd);
	OUTPUT:
		RETVAL

unsigned char *
db_tr_str_next (hnd, sep)
	int hnd
	char sep
	CODE:
		RETVAL = dbTrStrNext(hnd, sep);
	OUTPUT:
		RETVAL
	CLEANUP:
		free(RETVAL);

int
db_get_num_of_recs (hnd)
	int hnd
	CODE:
		RETVAL = dbGetNumOfRecs(hnd);
	OUTPUT:
		RETVAL


int
db_get_num_of_fields (hnd)
	int hnd
	CODE:
		RETVAL = dbGetNumOfFields(hnd);
	OUTPUT:
		RETVAL


int
db_init_field (hnd, fnr, name, type, length, dec)
	int hnd
	short fnr
	char* name
	char type
	short length
	short dec
	CODE:
		RETVAL = dbInitField(hnd, fnr, name, type, (char) length, (char) dec);
	OUTPUT:
		RETVAL

int
db_append_blank (hnd)
	int hnd
	CODE:
		RETVAL = dbAppendBlank(hnd);
	OUTPUT:
		RETVAL


unsigned int
db_change_field (hnd, rnr, fnr, value)
	int		hnd
	int		rnr
	short	fnr
	char*	value
	CODE:
		RETVAL = dbChangeField (hnd, rnr, fnr, value);
	OUTPUT:
		RETVAL

int
db_error()
	CODE:
		RETVAL = dberrno;
		dberrno = 0;
	OUTPUT:
		RETVAL

int
db_goto_nr(hnd, num)
	int hnd
	unsigned int num
	CODE:
		RETVAL = dbGotoNr(hnd, num);
	OUTPUT:
		RETVAL

char *
db_get_field_name(hnd, num)
	int hnd
	unsigned short num
	CODE:
		RETVAL = (char *) dbGetFieldName(hnd, num);
	OUTPUT:
		RETVAL


int
db_get_field_length(hnd, num)
	int hnd
	unsigned short num
	CODE:
		RETVAL = dbGetFieldLength(hnd, num);
	OUTPUT:
		RETVAL

char
db_get_field_type(hnd, num)
	int hnd
	unsigned short num
	CODE:
		RETVAL = dbGetFieldType(hnd, num);
	OUTPUT:
		RETVAL


unsigned char *
db_tr_sel_by_name (hnd, fld, go)
	int			hnd
	const char*	fld
	int			go
	CODE:
		RETVAL = dbTrSelByName(hnd, fld, go);
	OUTPUT:
		RETVAL
	CLEANUP:
		free(RETVAL);


unsigned char *
db_tr_sel_by_number (hnd, fld, go)
	int hnd
	int fld
	int go
	CODE:
		RETVAL = dbTrSelByNumber(hnd, fld, go);
	OUTPUT:
		RETVAL
	CLEANUP:
		free(RETVAL);

int
db_wind (hnd, offset)
	int		hnd
	long	offset
	CODE:
		RETVAL = dbWind(hnd, offset);
	OUTPUT:
		RETVAL

int
db_del_record (hnd, rnr)
	int				hnd
	unsigned long	rnr
	CODE:
		RETVAL = dbDelRecord(hnd, rnr);
	OUTPUT:
		RETVAL


int
db_undel_record (hnd, rnr)
	int				hnd
	unsigned long	rnr
	CODE:
		RETVAL = dbUndelRecord(hnd, rnr);
	OUTPUT:
		RETVAL

int
db_is_del (hnd, rnr)
	int				hnd
	unsigned long	rnr
	CODE:
		if (rnr == 0)
			rnr = CURRENT;
		RETVAL = dbIsDeleted(hnd, rnr);
		if (!(RETVAL == 0 || RETVAL == 1)) {
			dberrno = RETVAL;
		}
	OUTPUT:
		RETVAL

long
db_current (hnd)
	int hnd
	CODE:
		RETVAL = dbCurrent(hnd);
	OUTPUT:
		RETVAL

char
db_date_sep ()
	CODE:
		RETVAL = dbDefDateSep();
	OUTPUT:
		RETVAL

int
db_pack(hnd)
	int hnd
	CODE:
		RETVAL = dbPack(hnd);
	OUTPUT:
		RETVAL

int
db_unpack(hnd)
	int hnd
	CODE:
		RETVAL = dbRestore(hnd);
	OUTPUT:
		RETVAL

int
db_type(hnd)
	int hnd
	CODE:
		RETVAL = dbGetType (hnd);
	OUTPUT:
		RETVAL

char *
db_last_update(hnd)
	int hnd
	CODE:
		RETVAL = dbLastUpdate(hnd);
	OUTPUT:
		RETVAL
	CLEANUP:
		free(RETVAL);

int
db_eof(hnd)
	int hnd
	CODE:
		RETVAL = dbEof(hnd);
		if (!(RETVAL == 0 || RETVAL == 1)) {
			dberrno = RETVAL;
		}
	OUTPUT:
		RETVAL

int
db_get_field_dec(hnd, num)
	int hnd
	int num
	CODE:
		RETVAL = dbGetFieldDec(hnd, num);
	OUTPUT:
		RETVAL


char *
db_raw_field (hnd, num)
	int hnd
	int num
	CODE:
		RETVAL = dbRawField(hnd, num);
	OUTPUT:
		RETVAL
	CLEANUP:
		free(RETVAL);


int
db_get_field_num (hnd, name)
	int hnd
	const char* name
	CODE:
		RETVAL = dbGetFieldNum(hnd, name);
	OUTPUT:
		RETVAL

