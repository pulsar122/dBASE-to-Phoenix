#
# seems to be a little bit buggy, cause tcc tries all
# the time to compile lib/dbfdll.c ..??
#
CFLAGS=-ml -w -1 -N -Ilib

default:	dbfconv.exe dbfinfo.exe

dbfconv.obj	: dbfconv.c
	tcc -c $(CFLAGS) dbfconv.c

dbfinfo.obj	: dbfinfo.c
	tcc -c $(CFLAGS) dbfinfo.c

getopt.obj	: getopt.c
	tcc -c $(CFLAGS) getopt.c

lib\dbf.obj : lib\dbf.c
	tcc -olib\dbf.obj -c $(CFLAGS) lib\dbf.c

dbfinfo.exe	:	dbfinfo.obj lib\dbf.obj
	tcc $(CFLAGS) dbfinfo.obj lib\dbf.obj

dbfconv.exe	:	dbfconv.obj getopt.obj lib\dbf.obj
	tcc $(CFLAGS) dbfconv.obj lib/dbf.obj getopt.obj

clean:
	-del lib\*.obj
	-del *.obj

