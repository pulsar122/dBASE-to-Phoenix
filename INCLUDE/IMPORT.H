/*****************************************************************************
 *
 * Module : IMPORT.H
 * Author : Dieter Geiﬂ
 *
 * Creation date    : 06.03.89
 * Last modification: 27.09.93
 *
 *
 * Description: This module defines the import possibilties for modules.
 *
 * History:
 * 27.09.93: Problems with labs and fabs (non ANSI) fixed
 * 22.09.93: Definition for HINSTANCE_ERROR removed
 * 27.07.93: VER.H/WINNT.H included for definition of SHORT
 * 22.05.93: Warning C4001 disabled because of App Studio using single line comments
 * 18.05.93: Macro INVALID_ATOM added
 * 31.01.93: Warning C4214 disabled because of DDE.H
 * 11.01.93: APIENTRY added
 * 24.11.92: WIN32 capability added
 * 22.11.92: PC GEM capability removed
 * 26.10.92: CALLBACK added
 * 25.10.92: Module header added
 * 06.03.89: Creation of body
 *****************************************************************************/

#ifndef _IMPORT_H
#define _IMPORT_H

#ifdef WIN32
#ifndef _WINDOWS
#define _WINDOWS
#endif
#endif

#ifdef _WINDOWS

#ifdef NULL
#undef NULL
#endif

#define NOCOMM
#define STRICT

#include <windows.h>
#include <windowsx.h>

#ifdef WIN32
#include <winnt.h>
#else
#include <ver.h>
#endif

#define _(params) params

#ifdef WIN32					/* wrong definitions */
#undef FORWARD_WM_HSCROLL
#define FORWARD_WM_HSCROLL(hWnd, hWndCtl, code, pos, fn)			(VOID)(fn)((hWnd), WM_HSCROLL, MAKEWPARAM ((UINT)(code), (UINT)(pos)), (LPARAM)(UINT)(hWndCtl))
#undef FORWARD_WM_VSCROLL
#define FORWARD_WM_VSCROLL(hWnd, hWndCtl, code, pos, fn)			(VOID)(fn)((hWnd), WM_VSCROLL, MAKEWPARAM ((UINT)(code), (UINT)(pos)), (LPARAM)(UINT)(hWndCtl))
#undef FORWARD_WM_MDISETMENU
#define FORWARD_WM_MDISETMENU(hWnd, fRefresh, hmenuFrame, hmenuWindow, fn)	(HMENU)(UINT)(DWORD)(fn)((hWnd), WM_MDISETMENU, (WPARAM)((fRefresh) ? 0 : (hmenuFrame)), (LPARAM)(hmenuWindow))

#define OLDFONTENUMPROC					FONTENUMPROC

#define DlgDirSelect(hDlg, lpString, nIDListBox)	DlgDirSelectEx (hDlg, lpString, sizeof (FULLNAME), nIDListBox)
#define GetTextExtent(hDC, lpString, nCount)		GetTabbedTextExtent (hDC, lpString, nCount, 0, NULL)
#define GetFreeSystemResources(fuSysResource)		100
#define MoveTo(hDC, X, Y)				MoveToEx (hDC, X, Y, NULL)
#define SetBrushOrg(hDC, X, Y)				SetBrushOrgEx (hDC, X, Y, NULL)
#define SetViewportExt(hDC, X, Y)			SetViewportExtEx (hDC, X, Y, NULL)
#define SetViewportOrg(hDC, X, Y)			SetViewportOrgEx (hDC, X, Y, NULL)
#define SetWindowExt(hDC, X, Y)				SetWindowExtEx (hDC, X, Y, NULL)
#define SetWindowOrg(hDC, X, Y)				SetWindowOrgEx (hDC, X, Y, NULL)
#define UnrealizeObject(hgdiobj)

typedef RGBQUAD *LPRGBQUAD;
#else
#define APIENTRY WINAPI
#define INVALID_ATOM	((ATOM)0)
#endif
#else
#define CALLBACK
#define WINAPI
#define APIENTRY
#endif

#include <stdio.h>
#include <string.h>
#include <portab.h>

#if GEMDOS
#include <aes.h>
#include <vdi.h>
#if TURBO_C
#include <tos.h>
#else
#include <osbind.h>
#endif
#define Mavail() (LONG)Malloc (-1L)
#endif

#if MSDOS
#ifdef _WINDOWS
#if TURBO_C
#undef NULL
#define NULL 0					/* like Microsoft's WINDOWS.H */
#endif
#else
#define _(params) params
#ifdef PASCAL
#undef PASCAL
#define PASCAL
#endif
#endif
#endif

#if FLEXOS
#include <gemdos.h>
#include <dosbind.h>
#endif

#if OS2
#define INCL_DOS
#include <os2.h>
#define Mavail() (64 * 1024L)			/* in OS2 there's no way to decide free memory */
#endif

#if UNIX
#include <sys/types.h>
#define Mavail() (64 * 1024L)			/* in UNIX there's no way to decide free memory */
#endif

#if ANSI
#include <stdlib.h>
#else
#define abs(x)      ((x) < 0 ? -(x) : (x))
#define labs(x)     (LONG)abs (x)
#define fabs(x)     (DOUBLE)abs (x)
#endif

#if MS_C
#if (_MSC_VER >= 700)
#pragma warning (disable:4001)			/* nonstandard extension 'single line comment' was used */
#pragma warning (disable:4050)			/* different code attributes */
#pragma warning (disable:4100)			/* unused parameters */
#pragma warning (disable:4131)			/* old style declarator */
#pragma warning (disable:4214)			/* nonstandard extension used : bit field types other than int */
#endif
#endif

/****** DEFINES **************************************************************/

#ifdef GLOBAL
#undef GLOBAL
#endif

#define GLOBAL EXTERN

#if HIGH_C
#ifdef NULL
#undef NULL
#define NULL 0L
#endif
#endif

#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#define odd(i)      ((i) & 1)

/****** TYPES ****************************************************************/

#ifndef _WINDOWS
typedef VOID       FAR *LPVOID;
typedef INT        FAR *LPINT;
typedef LONG       FAR *LPLONG;
typedef CHAR       FAR *LPSTR;
typedef CONST CHAR FAR *LPCSTR;
#endif

typedef CHAR   FAR *LPCHAR;
typedef SHORT  FAR *LPSHORT;
typedef UCHAR  FAR *LPUCHAR;
typedef USHORT FAR *LPUSHORT;
typedef ULONG  FAR *LPULONG;
typedef FLOAT  FAR *LPFLOAT;
typedef DOUBLE FAR *LPDOUBLE;
typedef BOOL   FAR *LPBOOL;

typedef VOID   HUGE *HPVOID;
typedef CHAR   HUGE *HPCHAR;
typedef SHORT  HUGE *HPSHORT;
typedef LONG   HUGE *HPLONG;
typedef UCHAR  HUGE *HPUCHAR;
typedef USHORT HUGE *HPUSHORT;
typedef ULONG  HUGE *HPULONG;
typedef FLOAT  HUGE *HPFLOAT;
typedef DOUBLE HUGE *HPDOUBLE;
typedef BOOL   HUGE *HPBOOL;

/*****************************************************************************/

#endif /* _IMPORT_H */

