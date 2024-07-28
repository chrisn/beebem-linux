/* Fake windows stuff in here.
 */

#ifndef _FAKE_MS_WINDOWS_H_
#define _FAKE_MS_WINDOWS_H_

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include <SDL.h>
#include "Log.h"
#include "Sdl.h"

#include "BeebEmPages.h"

/* NOTE: We cannot define WIN32 and fake our way through the BeebEm code..
 *       This would break the SDL library - as if it's header was ever included
 *       after WIN32 being defined, then we get the Windows header instead!
 *
 *	 So all WIN32 definitions have been removed.
 */


#define MAX_PATH 1024


/* Command line args (variables in main.cpp)
 */
extern int __argc;
extern char **__argv;

#define stricmp strcasecmp

typedef Uint64 __int64;

typedef struct{
	Uint16 lowpart;
	Uint16 highpart;
}LARGE_INTEGER;

typedef Uint8* PBYTE;

typedef float FLOAT;


// this is its correct value.
#define MF_BYCOMMAND 0x00



#define MOVEFILE_REPLACE_EXISTING 	0x00000001
#define MOVEFILE_COPY_ALLOWED       	0x00000002


// cregistry.h
typedef int HKEY;
typedef char* LPSTR;
typedef char* LPTSTR;

// This is not right, but we dont use it anyway.
#define HKEY_CURRENT_USER 		0

// Not sure what this is, but its not a pointer.
typedef int PTR;

typedef void* PVOID;
typedef const char* LPCTSTR;

typedef char CHAR;
typedef Uint8 BYTE;
typedef Uint16 WORD;
typedef Uint32 DWORD;
typedef unsigned int UINT;
typedef int INT;

typedef Uint32 COLORREF;

// Menus
#define MF_CHECKED 		1
#define MF_UNCHECKED 		0
#define	MF_ENABLED		0x0000	
#define MF_GRAYED		1

/*
#ifndef BOOL
#	define BOOL Uint8
#	define FALSE false
#	define TRUE true
#endif
*/

#ifndef BOOL
#	define BOOL int
#	define FALSE 0
#	define TRUE 1
#endif


// beebwin
typedef int HMENU;
typedef int HDC;
typedef int HWND;
typedef int JOYCAPS;
typedef int HGDIOBJ;
typedef int bmiData;
typedef int HBITMAP;
typedef void* LPDIRECTDRAW;
typedef void* LPDIRECTDRAW2;
typedef void* LPDIRECTDRAWSURFACE;
typedef void* LPDIRECTDRAWSURFACE2;
typedef int HRESULT;
typedef void* LPDIRECTDRAWCLIPPER;

typedef void* LPDIRECT3D9;
typedef void* LPDIRECT3DDEVICE9;
typedef void* LPDIRECT3DVERTEXBUFFER9;
typedef void* LPDIRECT3DTEXTURE9;

typedef int D3DVECTOR;
typedef int D3DCOLOR;
typedef int D3DMATRIX;

typedef int POINT;
typedef void* WNDPROC;

// Text to speech
typedef void ISpVoice;
typedef void ISpObjectToken;

typedef unsigned int ULONG_PTR;
// beebsound
typedef void* LPDIRECTSOUND;
typedef void* LPDIRECTSOUNDBUFFER;

// serial
typedef int HINSTANCE;

// Econet
typedef int SOCKET;
constexpr SOCKET INVALID_SOCKET = -1;
constexpr int SOCKET_ERROR = -1;

typedef char WCHAR;
typedef void CLSID;
typedef int WPARAM;

// Time
struct SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};

void GetLocalTime(SYSTEMTIME* pTime);

// --- Windows message box:

// Buttons:
#define MB_OK 			0x00000000L
#define MB_OKCANCEL 		0x00000001L
#define MB_ABORTRETRYIGNORE 	0x00000002L
#define MB_YESNOCANCEL 		0x00000003L
#define MB_YESNO 		0x00000004L
#define MB_RETRYCANCEL 		0x00000005L

// Icons:
#define MB_ICONHAND 		0x00000010L
#define MB_ICONSTOP             MB_ICONHAND

#define MB_ICONQUESTION 	0x00000020L
#define MB_ICONEXCLAMATION 	0x00000030L
#define MB_ICONWARNING 		MB_ICONEXCLAMATION
#define MB_ICONASTERISK 	0x00000040L
#define MB_ICONINFORMATION      MB_ICONASTERISK

#define MB_USERICON		0x00000080L
#define MB_ICONWARNING          MB_ICONEXCLAMATION
#define MB_ICONERROR            MB_ICONHAND

// Return values:
#define IDOK 			1
#define IDCANCEL 		2
#define IDABORT 		3
#define IDRETRY			4
#define IDIGNORE		5
#define IDYES 			6
#define IDNO			7



//' DEFAULT BUTTON
// 
//DEFINE MB_DEFBUTTON1 &H0
//DEFINE MB_DEFBUTTON2 &H100
//DEFINE MB_DEFBUTTON3 &H200
//DEFINE MB_DEFBUTTON4 &H300



// ----------


// Errors
//#define ERROR_SUCCESS 0

int 	MessageBox(HWND hwnd, const char *message_p, const char *title_p, int type);
void 	SetWindowText(HWND hwnd, const char *title_p);
void 	Sleep(Uint32 ticks);
DWORD 	GetTickCount(void);
DWORD	CheckMenuItem(HMENU hmenu, UINT uIDCheckItem, UINT uCheck);
BOOL	ModifyMenu(HMENU hMnu, UINT uPosition, UINT uFlags, PTR uIDNewItem, LPCTSTR lpNewItem);
BOOL	MoveFileEx(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD dwFlags);
BOOL	EnableMenuItem(HMENU hMenu,UINT uIDEnableItem,UINT uEnable);
UINT GetMenuState(HMENU hMenu, UINT uId, UINT uFlags);

UINT GetPrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName,
 INT nDefault, LPCTSTR lpFileName);

DWORD GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault,
 LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);




#ifndef MAKEWORD
#define MAKEWORD(b1, b2) ((WORD)(((BYTE)(b1)) | ((WORD)((BYTE)(b2))) << 8))
#endif

#ifndef LOBYTE
#define LOBYTE(w) ((BYTE)(w))
#endif

#ifndef HIBYTE
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif



#endif
