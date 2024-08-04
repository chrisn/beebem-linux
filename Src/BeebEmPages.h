/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 1994  David Alan Gilbert

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

#ifndef BEEBEMPAGES_HEADER
#define BEEBEMPAGES_HEADER

#include <SDL.h>

void Show_Main();
bool InitializeBeebEmGUI(SDL_Surface *screen_ptr);
void DestroyBeebEmGUI();

int UpdateGUIOption(int windows_menu_id, int is_selected);
int GetGUIOption(int windows_menu_id);
int SetGUIOptionCaption(int windows_menu_id, const char *str);

int Open_GTK_File_Selector(char *filename_ptr);
int Save_GTK_File_Selector(char *filename_ptr);

void Update_FDC_Buttons();
void Update_Resolution_Buttons();

void SetNameForDisc(int drive, char *name_ptr);
void SetFullScreenTickbox(int state);

void ClearWindowsBackgroundCacheAndResetSurface();

#endif