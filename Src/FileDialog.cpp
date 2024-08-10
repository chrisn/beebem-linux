/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 1994  Nigel Magnay
Copyright (C) 1997  Mike Wyatt

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

#include "Windows.h"

#include "FileDialog.h"

#ifndef WIN32
#include "gui/gui.h"
#include "BeebEmPages.h"
#include "Main.h"

#include <gtk/gtk.h>
#endif

FileDialog::FileDialog(HWND hwndOwner, LPTSTR result, DWORD resultLength,
                       LPCTSTR initialFolder, LPCTSTR filter)
{
	#ifdef WIN32

	ZeroMemory(&m_ofn, sizeof(m_ofn));

	m_ofn.lStructSize = sizeof(OPENFILENAME);
	m_ofn.hwndOwner = hwndOwner;
	m_ofn.lpstrFilter = filter;
	m_ofn.nFilterIndex = 1;
	m_ofn.lpstrFile = result;
	m_ofn.nMaxFile = resultLength;
	m_ofn.lpstrInitialDir = initialFolder;
	m_ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	#else

	UNREFERENCED_PARAMETER(hwndOwner);
	UNREFERENCED_PARAMETER(resultLength);
	UNREFERENCED_PARAMETER(filter);

	m_pszTitle = nullptr;
	m_pszFileName = result;
	strcpy(m_szInitialFolder, initialFolder);

	size_t Length = strlen(m_szInitialFolder);

	if (Length > 0)
	{
		if (m_szInitialFolder[Length - 1] != '/')
		{
			m_szInitialFolder[Length] = '/';
			m_szInitialFolder[Length + 1] = '\0';
		}
	}

	#endif
}

void FileDialog::SetFilterIndex(DWORD index)
{
	#ifdef WIN32

	m_ofn.nFilterIndex = index;

	#else

	UNREFERENCED_PARAMETER(index);

	#endif
}

void FileDialog::AllowMultiSelect()
{
	#ifdef WIN32

	m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	#endif
}

void FileDialog::SetTitle(LPCTSTR title)
{
	#ifdef WIN32

	m_ofn.lpstrTitle = title;

	#else

	m_pszTitle = title;

	#endif
}

bool FileDialog::Open()
{
	return ShowDialog(true);
}

bool FileDialog::Save()
{
	return ShowDialog(false);
}

DWORD FileDialog::GetFilterIndex() const
{
	#ifdef WIN32

	return m_ofn.nFilterIndex;

	#else

	return 0;

	#endif
}

static char *gtk_file_selector_filename_ptr;
static GtkWidget *filew;
static bool got_file;
bool was_full_screen = false;

// Get the selected filename and print it to the console
static void file_ok_sel(GtkWidget * /* w */, GtkFileSelection *fs)
{
	// g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
	strcpy(gtk_file_selector_filename_ptr, gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));

	got_file = true;

	gtk_widget_destroy(filew);

	if (was_full_screen)
	{
		mainWin->ToggleFullScreen();
	}
}

bool FileDialog::ShowDialog(bool open)
{
	#ifdef WIN32

	if (open)
	{
		return GetOpenFileName(&m_ofn) != 0;
	}
	else
	{
		m_ofn.Flags |= OFN_OVERWRITEPROMPT;

		return GetSaveFileName(&m_ofn) != 0;
	}

	#else

	UNREFERENCED_PARAMETER(open);

	if (m_pszFileName == nullptr)
	{
		return false;
	}

	if (mainWin->IsFullScreen())
	{
		mainWin->ToggleFullScreen();
		was_full_screen = true;
	}

	got_file = false;

	gtk_init(&__argc, &__argv);

	gtk_file_selector_filename_ptr = m_pszFileName;

	filew = gtk_file_selection_new(m_pszTitle != nullptr ? m_pszTitle : "File selection");

	g_signal_connect(G_OBJECT(filew), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filew)->ok_button), "clicked", G_CALLBACK(file_ok_sel), (gpointer)filew);
	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button),
	                         "clicked",
	                         G_CALLBACK(gtk_widget_destroy),
	                         G_OBJECT(filew));

	if (strlen(m_pszFileName) > 0)
	{
		gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew), m_pszFileName);
	}
	else
	{
		gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew), m_szInitialFolder);
	}

	gtk_widget_show(filew);
	gtk_main();

	was_full_screen = false;

	return got_file;

	#endif
}
