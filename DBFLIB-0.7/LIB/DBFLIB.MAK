# Microsoft Visual C++ generated build script - Do not modify

PROJ = DBFLIB
DEBUG = 0
PROGTYPE = 1
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG 
R_RCDEFINES = /dNDEBUG 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = H:\SRC\DBFL~1IK.6\LIB\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = DBF.C       
FIRSTCPP =             
RC = rc
CFLAGS_D_WDLL = /nologo /G2 /Za /Gy /W3 /Zi /ALw /Od /D "_DEBUG" /D "WINDLL" /U "_WINDLL" /FR /GD /Fd"DBFLIB.PDB"
CFLAGS_R_WDLL = /nologo /Gs /G2 /W3 /ALw /D "NDEBUG" /D "WINDLL" /U "_WINDLL" /FR /GD 
LFLAGS_D_WDLL = /NOLOGO /NOD /NOE /PACKC:61440 /NOPACKF /ALIGN:16 /ONERROR:NOEXE /CO /MAP:FULL
LFLAGS_R_WDLL = /NOLOGO /NOD /NOE /PACKC:61440 /NOPACKF /ALIGN:16 /ONERROR:NOEXE /MAP:FULL
LIBS_D_WDLL = libw ldllcew commdlg.lib olecli.lib olesvr.lib shell.lib 
LIBS_R_WDLL = libw ldllcew commdlg.lib  oldnames
RCFLAGS = /nologo 
RESFLAGS = /nologo 
RUNFLAGS = 
DEFFILE = DBFLIB.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WDLL)
LFLAGS = $(LFLAGS_D_WDLL)
LIBS = $(LIBS_D_WDLL)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WDLL)
LFLAGS = $(LFLAGS_R_WDLL)
LIBS = $(LIBS_R_WDLL)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = DBF.SBR \
		DBFDLL.SBR


DBF_DEP = h:\src\dbfl~1ik.6\lib\dbf.h


DBFDLL_DEP = h:\src\dbfl~1ik.6\lib\dbf.h \
	h:\src\dbfl~1ik.6\lib\dbfdll.h


all:	$(PROJ).DLL $(PROJ).BSC

DBF.OBJ:	DBF.C $(DBF_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c DBF.C

DBFDLL.OBJ:	DBFDLL.C $(DBFDLL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DBFDLL.C


$(PROJ).DLL::	DBF.OBJ DBFDLL.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
DBF.OBJ +
DBFDLL.OBJ +
$(OBJS_EXT)
$(PROJ).DLL
$(MAPFILE)
c:\msvc\lib\+
g:\wapps\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) $@
	implib /nowep $(PROJ).LIB $(PROJ).DLL


run: $(PROJ).DLL
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
