/*--------------------------------------------------------------------------*/
/* Header fÅr Keytab																												*/

#ifndef _key.h_
#define _key.h_

#include <keytab.h>

EXTERN keytab_id_import;										/* Keytab-Import-Filter					*/
EXTERN keytab_id_export;										/* Keytab-Export-Filter					*/
EXTERN KEYT *keytab;												/* Variable fÅr Keytab-Cookie		*/

EXTERN VOID import_keytab(VOID);
EXTERN VOID init_keytab(VOID);

#endif
