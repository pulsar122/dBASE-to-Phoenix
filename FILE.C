/*--------------------------------------------------------------------------*/
/* Verschiedene Dateifunktionen																							*/

#include <portab.h>
#include <tos.h>

/*--------------------------------------------------------------------------*/
/* Testet ob die Datei vorhanden ist																				*/

WORD file_exist (const UBYTE *filename)
{
	XATTR	attr;
	LONG	ret;

	ret = Fxattr(0, (UBYTE *)filename, &attr);
	if (ret != 0)
	{
		DTA	dta, *old_dta;

		old_dta = (DTA *)Fgetdta ();
		Fsetdta (&dta);
		ret = Fsfirst ((UBYTE*)filename, 0x17);
		Fsetdta (old_dta);
	}
	return (ret == 0);
}
