/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 2006  David Eggleston

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.
****************************************************************/

// Fake windows stuff in here.

#ifndef FAKE_MS_WINDOWS_H
#define FAKE_MS_WINDOWS_H

/* NOTE: We cannot define WIN32 and fake our way through the BeebEm code..
 *       This would break the SDL library - as if it's header was ever included
 *       after WIN32 being defined, then we get the Windows header instead!
 *
 *	 So all WIN32 definitions have been removed.
 */

#include <stdarg.h>

#define MAX_PATH 1024

/* Command line args (variables in main.cpp)
 */
extern int __argc;
extern char **__argv;

#define stricmp strcasecmp

typedef long __int64;

typedef struct {
	unsigned short lowpart;
	unsigned short highpart;
} LARGE_INTEGER;

typedef unsigned char* PBYTE;

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
#define HKEY_CURRENT_USER 0

// Not sure what this is, but its not a pointer.
typedef int PTR;

typedef void* PVOID;
typedef const char* LPCTSTR;
typedef const char* LPCSTR;

typedef char CHAR;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned int UINT_PTR;
typedef int INT;
typedef int INT_PTR;
typedef long LONG;

typedef unsigned int COLORREF;
typedef unsigned long SIZE_T;

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
typedef void* HANDLE;
typedef void* HACCEL;
typedef void* HMENU;
typedef void* HDC;
typedef void* HWND;
typedef void* HMONITOR;
typedef int JOYCAPS;
typedef void* HGDIOBJ;
typedef void* HBITMAP;
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

typedef void* WNDPROC;
typedef int LRESULT;

// Text to speech
typedef void ISpVoice;
typedef void ISpObjectToken;

typedef unsigned int ULONG_PTR;
// beebsound
typedef void* LPDIRECTSOUND;
typedef void* LPDIRECTSOUNDBUFFER;

// serial
typedef void* HINSTANCE;

// Econet
typedef int SOCKET;
constexpr SOCKET INVALID_SOCKET = -1;
constexpr int SOCKET_ERROR = -1;
typedef struct sockaddr SOCKADDR;

#define S_ADDR(s) (s).sin_addr.s_addr

#define IN_ADDR(addr) (addr).s_addr

// Win32: #define IN_ADDR(addr) (addr).S_un.S_addr

int WSAGetLastError();

typedef char WCHAR;
typedef void CLSID;
typedef int WPARAM;
typedef int LPARAM;
typedef void* HHOOK;

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

typedef void* TIMERPROC;

void SetWindowText(HWND hwnd, const char *pszTitle);
void Sleep(DWORD Milliseconds);
DWORD GetTickCount();
DWORD CheckMenuItem(HMENU hmenu, UINT uIDCheckItem, UINT uCheck);
BOOL ModifyMenu(HMENU hMnu, UINT uPosition, UINT uFlags, PTR uIDNewItem, LPCTSTR lpNewItem);
BOOL MoveFileEx(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD dwFlags);
BOOL EnableMenuItem(HMENU hMenu,UINT uIDEnableItem,UINT uEnable);
UINT GetMenuState(HMENU hMenu, UINT uId, UINT uFlags);

UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
BOOL KillTimer(HWND hWnd, UINT_PTR nIDEvent);

BOOL PathIsRelative(LPCSTR pszPath);

void _splitpath(
   const char *path,
   char *drive,
   char *dir,
   char *fname,
   char *ext
);

void _makepath(
   char *path,
   const char *drive,
   const char *dir,
   const char *fname,
   const char *ext
);

constexpr int _MAX_DRIVE = 3;
constexpr int _MAX_DIR = MAX_PATH;

BOOL PathIsRelative(LPCSTR pszPathName);

BOOL PathCanonicalize(LPSTR pszBuf, LPCSTR pszPath);

int SHCreateDirectoryEx(HWND hWnd, LPCSTR pszPath, const void *psa);

DWORD GetFullPathName(LPCSTR pszFileName, DWORD BufferLength, LPSTR pszBuffer, LPSTR *pszFilePart);

DWORD GetCurrentThreadId();

#define CALLBACK

int _stricmp(
   const char *string1,
   const char *string2
);

DWORD GetLastError();

BOOL PostMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT;

typedef struct tagPOINT {
	LONG x;
	LONG y;
} POINT;

typedef struct tagMSG {
	HWND   hwnd;
	UINT   message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD  time;
	POINT  pt;
	DWORD  lPrivate;
} MSG;

#ifndef MAKEWORD
#define MAKEWORD(b1, b2) ((WORD)(((BYTE)(b1)) | ((WORD)((BYTE)(b2))) << 8))
#endif

#ifndef LOBYTE
#define LOBYTE(w) ((BYTE)(w))
#endif

#ifndef HIBYTE
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif

#define _countof(x) (sizeof(x) / sizeof(x[0]))

void ZeroMemory(PVOID Destination, SIZE_T Length);

int _vscprintf(const char* format, va_list pargs);

constexpr int ERROR_SUCCESS = 0;

#define UNREFERENCED_PARAMETER(x) (void)x

constexpr int WM_APP = 0x8000;

BOOL CheckMenuRadioItem(HMENU hMenu, UINT FirstID, UINT LastID, UINT SelectedID, UINT Flags);

#define __stdcall

constexpr unsigned char NOPARITY = 0;
constexpr unsigned char ODDPARITY = 1;
constexpr unsigned char EVENPARITY = 2;

constexpr DWORD MS_CTS_ON = 0x0010;

int ioctlsocket(SOCKET Socket, long Cmd, unsigned long* pArg);

typedef int CRITICAL_SECTION;

void InitializeCriticalSection(CRITICAL_SECTION* pCriticalSection);
void DeleteCriticalSection(CRITICAL_SECTION* pCriticalSection);

void EnterCriticalSection(CRITICAL_SECTION* pCriticalSection);
void LeaveCriticalSection(CRITICAL_SECTION* pCriticalSection);

BOOL GetWindowRect(HWND hWnd, RECT* pRect);

constexpr DWORD SPF_PURGEBEFORESPEAK = 1;
constexpr DWORD SPF_NLP_SPEAK_PUNC   = 2;

BOOL MessageBeep(UINT uType);

#endif
