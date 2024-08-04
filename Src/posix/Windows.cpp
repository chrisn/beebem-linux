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

#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include "log.h"
#include "Windows.h"
#include "BeebEmPages.h"
#include "FileUtils.h"

#include "gui/gui.h"
#include "Sdl.h"

// Fake windows stuff:

int _vscprintf(const char *format, va_list pargs)
{
	int retval;
	va_list argcopy;
	va_copy(argcopy, pargs);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return retval;
}

/* Fake windows MessageBox
 */
int MessageBox(HWND hwnd, const char *message_p, const char *title_p, int type)
{
	int ret = 1, icon_type=0;
	HWND tmp_hwnd;

	// [TODO] Add support for default button, BeebEm doesn't use it anyway.
	int selected = 1;

	switch (type & 0xf0)
	{
	case MB_ICONHAND:
		icon_type = EG_MESSAGEBOX_STOP;
		break;
	case MB_ICONQUESTION:
		icon_type = EG_MESSAGEBOX_QUESTION;
		break;
	case MB_ICONEXCLAMATION:
		icon_type = EG_MESSAGEBOX_STOP;
		break;
	case MB_ICONASTERISK:
		icon_type = EG_MESSAGEBOX_INFORMATION;
		break;
	case MB_USERICON:
		icon_type = EG_MESSAGEBOX_STOP;
		break;
	default:
		icon_type = EG_MESSAGEBOX_INFORMATION;
		break;
	}

	switch (type & 0xf)
	{
	case MB_OKCANCEL:
		if (EG_MessageBox(screen_ptr, icon_type, title_p, message_p, "OK", "CANCEL", NULL, NULL, selected) == 1)
			ret = IDOK;
		else
			ret = IDCANCEL;
		break;
	case MB_ABORTRETRYIGNORE:
		ret = EG_MessageBox(screen_ptr, icon_type, title_p, message_p, "Abort", "Retry", "Ignore", NULL, selected);
		switch (ret){
			case 2: ret = IDRETRY; break;
			case 3: ret = IDIGNORE; break;
			case 1: default: ret = IDABORT; break;
		}
		break;
	case MB_YESNOCANCEL:
		ret = EG_MessageBox(screen_ptr, icon_type, title_p, message_p, "Yes", "No", "Cancel", NULL, selected);
		switch (ret){
			case 2: ret = IDNO; break;
			case 3: ret = IDCANCEL; break;
			case 1: default: ret = IDYES; break;
		}
		break;
	case MB_YESNO:
		if (EG_MessageBox(screen_ptr, icon_type, title_p, message_p, "Yes", "No", NULL, NULL, selected) == 1)
			ret = IDYES;
		else
			ret = IDNO;
		break;
	case MB_RETRYCANCEL:
		if (EG_MessageBox(screen_ptr, icon_type, title_p, message_p, "Retry", "Cancel", NULL, NULL, selected) == 1)
			ret = IDRETRY;
		else
			ret = IDCANCEL;
		break;

	case MB_OK:
	default:
		EG_MessageBox(screen_ptr, icon_type, title_p, message_p, "OK", NULL, NULL, NULL, 1);
		ret = IDOK;
		break;
	}

	return ret;
}

void SetWindowText(HWND hwnd, const char *pszTitle)
{
	SetWindowTitle(pszTitle);
}

void Sleep(DWORD ticks)
{
	SaferSleep( (unsigned int) ticks);
}

DWORD GetTickCount(void)
{
	return(SDL_GetTicks());
}

BOOL ModifyMenu(HMENU hMnu, UINT uPosition, UINT uFlags, PTR uIDNewItem, LPCTSTR lpNewItem)
{
	int v;

	// Only supports changing the title.
	//SetGUIOptionCaption(int windows_menu_id, const char *str)

	v = SetGUIOptionCaption( (int) uPosition, (const char*) lpNewItem);

/*
Return Value

    If the function succeeds, the return value is nonzero.

    If the function fails, the return value is zero. To get extended error information, call GetLastError.
*/

	if (v==EG_TRUE) return 1;
	else return 0;
}

UINT GetMenuState(HMENU hMenu, UINT uId, UINT uFlags)
{
	int v;

//	printf("GetMenuState\n");

	v = GetGUIOption(uId);

	if (v == EG_TRUE) v = MF_CHECKED;
	if (v == EG_FALSE) v = MF_UNCHECKED;

	return(v);

/*
Return Value

    If the specified item does not exist, the return value is -1.

    If the menu item opens a submenu, the low-order byte of the return value contains the menu flags associated with the item, and the high-order byte contains the number of items in the submenu opened by the item.

    Otherwise, the return value is a mask (Bitwise OR) of the menu flags. Following are the menu flags associated with the menu item.

    MF_CHECKED	A check mark is placed next to the item (for drop-down menus, submenus, and shortcut menus only).
    MF_DISABLED	The item is disabled.
    MF_GRAYED	The item is disabled and grayed.
    MF_HILITE	The item is highlighted.
    MF_MENUBARBREAK	This is the same as the MF_MENUBREAK flag, except for drop-down menus, submenus, and shortcut menus, where the new column is separated from the old column by a vertical line.
    MF_MENUBREAK	The item is placed on a new line (for menu bars) or in a new column (for drop-down menus, submenus, and shortcut menus) without separating columns.
    MF_OWNERDRAW	The item is owner-drawn.
    MF_POPUP	Menu item is a submenu.
    MF_SEPARATOR	There is a horizontal dividing line (for drop-down menus, submenus, and shortcut menus only).



Remarks

    Note  The GetMenuState function has been superseded by the GetMenuItemInfo. You can still use GetMenuState, however, if you do not need any of the extended features of GetMenuItemInfo.

    In addition, it is possible to test an item for a flag value of MF_ENABLED, MF_STRING, MF_UNCHECKED, or MF_UNHILITE. However, since these values equate to zero you must use an expression to test for them.
    Flag 	Expression to test for the flag
    MF_ENABLED	! (Flag&(MF_DISABLED | MF_GRAYED))
    MF_STRING	! (Flag&(MF_BITMAP | MF_OWNERDRAW))
    MF_UNCHECKED	! (Flag&MF_CHECKED)
    MF_UNHILITE	! (Flag&HILITE)


*/
}

DWORD CheckMenuItem(HMENU /* hMenu */, UINT uIDCheckItem, UINT uCheck)
{
	//printf("Asked to set %d to %d\n", uIDCheckItem, uCheck);

	if (uCheck == MF_CHECKED)
		return( UpdateGUIOption(uIDCheckItem, 1) );
	else
		return( UpdateGUIOption(uIDCheckItem, 0) );
}

BOOL CheckMenuRadioItem(HMENU /* hMenu */, UINT FirstID, UINT LastID, UINT SelectedID, UINT /* Flags */)
{
	for (UINT MenuItemID = FirstID; MenuItemID < LastID; MenuItemID++)
	{
		UpdateGUIOption(MenuItemID, MenuItemID == SelectedID);
	}
}

BOOL MoveFileEx(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD dwFlags)
{
	printf("MoveFileEx\n");
	return(FALSE);
}

BOOL EnableMenuItem(HMENU hMenu,UINT uIDEnableItem,UINT uEnable)
{
	return(TRUE);
}

void GetLocalTime(SYSTEMTIME* pTime)
{
	time_t Time;
	time(&Time);
	struct tm *pTM = localtime(&Time);

	pTime->wYear = pTM->tm_year + 1900;
	pTime->wMonth = pTM->tm_mon + 1;
	pTime->wDayOfWeek = pTM->tm_wday;
	pTime->wDay = pTM->tm_mday;
	pTime->wHour = pTM->tm_hour;
	pTime->wMinute = pTM->tm_min;
	pTime->wSecond = pTM->tm_sec;
	pTime->wMilliseconds = 0;
}

int ioctlsocket(SOCKET Socket, long Cmd, unsigned long* pArg)
{
	return ioctl(Socket, Cmd, pArg);
}

void ZeroMemory(PVOID Destination, SIZE_T Length)
{
	memset(Destination, 0, Length);
}

UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
{
	// TODO
	return 0;
}

BOOL KillTimer(HWND hWnd, UINT_PTR nIDEvent)
{
	// TODO
	return FALSE;
}

BOOL PathIsRelative(LPCSTR pszPath)
{
	return pszPath[0] != '/';
}

BOOL PathCanonicalize(LPSTR pszBuf, LPCSTR pszPath)
{
	// TODO
	strcpy(pszBuf, pszPath);
}

int SHCreateDirectoryEx(HWND /* hWnd */, LPCSTR pszPath, const void * /* psa */)
{
	int Result = mkdir(pszPath, S_IRWXU | S_IRWXG | S_IRWXO);

	return Result == 0 ? 0 : errno;
}

DWORD GetFullPathName(LPCSTR pszFileName, DWORD BufferLength, LPSTR pszBuffer, LPSTR *pszFilePart)
{
	// TODO
	return 0;
}


void InitializeCriticalSection(CRITICAL_SECTION* pCriticalSection)
{
}

void DeleteCriticalSection(CRITICAL_SECTION* pCriticalSection)
{
}

void EnterCriticalSection(CRITICAL_SECTION* pCriticalSection)
{
}

void LeaveCriticalSection(CRITICAL_SECTION* pCriticalSection)
{
}

BOOL GetWindowRect(HWND hWnd, RECT* pRect)
{
	ZeroMemory(pRect, sizeof(RECT));

	return TRUE;
}

BOOL MessageBeep(UINT uType)
{
	return TRUE;
}

void _splitpath(const char *path,
                char *drive,
                char *dir,
                char *fname,
                char *ext)
{
	// TODO
}

void _makepath(char *path,
               const char *drive,
               const char *dir,
               const char *fname,
               const char *ext)
{
	AppendPath(path, drive);
	AppendPath(path, dir);
	AppendPath(path, fname);
	strcat(path, ext);
}
