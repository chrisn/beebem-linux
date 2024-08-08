/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 2001  Richard Gellman
Copyright (C) 2004  Mike Wyatt

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

#include <string>

#include "TapeControlDialog.h"
#include "Main.h"
#include "Resource.h"
#include "Serial.h"

// Tape control dialog box variables
std::vector<TapeMapEntry> TapeMap;
bool TapeControlEnabled = false;

#ifdef WIN32

static HWND hwndTapeControl;
static HWND hwndMap;

static void TapeControlRecord();
static void UpdateState(HWND hwndDlg);

static INT_PTR CALLBACK TapeControlDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

void TapeControlOpenDialog(HINSTANCE hinst, HWND /* hwndMain */)
{
	TapeControlEnabled = true;

	if (!IsWindow(hwndTapeControl))
	{
		hwndTapeControl = CreateDialog(hinst, MAKEINTRESOURCE(IDD_TAPECONTROL),
		                               NULL, TapeControlDlgProc);
		hCurrentDialog = hwndTapeControl;
		ShowWindow(hwndTapeControl, SW_SHOW);

		hwndMap = GetDlgItem(hwndTapeControl, IDC_TAPE_CONTROL_MAP);
		SendMessage(hwndMap, WM_SETFONT, (WPARAM)GetStockObject(ANSI_FIXED_FONT),
		            (LPARAM)MAKELPARAM(FALSE,0));

		int Time = SerialGetTapeClock();
		TapeControlAddMapLines();
		TapeControlUpdateCounter(Time);
	}
}

#endif

void TapeControlCloseDialog()
{
	#ifdef WIN32

	DestroyWindow(hwndTapeControl);
	hwndTapeControl = nullptr;
	hwndMap = nullptr;
	TapeControlEnabled = false;
	hCurrentDialog = nullptr;

	#endif
}

void TapeControlAddMapLines()
{
	#ifdef WIN32

	SendMessage(hwndMap, LB_RESETCONTENT, 0, 0);

	for (const TapeMapEntry& line : TapeMap)
	{
		SendMessage(hwndMap, LB_ADDSTRING, 0, (LPARAM)line.desc.c_str());
	}

	UpdateState(hwndTapeControl);

	#endif
}

void TapeControlUpdateCounter(int tape_time)
{
	#ifdef WIN32

	if (TapeControlEnabled)
	{
		size_t i = 0;

		while (i < TapeMap.size() && TapeMap[i].time <= tape_time)
			i++;

		if (i > 0)
			i--;

		SendMessage(hwndMap, LB_SETCURSEL, (WPARAM)i, 0);
	}

	#else

	UNREFERENCED_PARAMETER(tape_time);

	#endif
}

#ifdef WIN32

static void EnableDlgItem(HWND hDlg, UINT nIDDlgItem, bool Enable)
{
	EnableWindow(GetDlgItem(hDlg, nIDDlgItem), Enable);
}

static bool IsDlgItemChecked(HWND hDlg, UINT nIDDlgItem)
{
	return SendDlgItemMessage(hDlg, nIDDlgItem, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

static void SetDlgItemChecked(HWND hDlg, UINT nIDDlgItem, bool Checked)
{
	SendDlgItemMessage(hDlg, nIDDlgItem, BM_SETCHECK, Checked ? BST_CHECKED : BST_UNCHECKED, 0);
}

static void UpdateState(HWND hwndDlg)
{
	SetFocus(hwndDlg);

	SerialTapeState State = SerialGetTapeState();

	UINT nIDCheckButton;

	switch (State)
	{
		case SerialTapeState::Playing:
			nIDCheckButton = IDC_PLAYING;

			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, false);
			break;

		case SerialTapeState::Recording:
			nIDCheckButton = IDC_RECORDING;

			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, false);
			break;

		case SerialTapeState::Stopped:
			nIDCheckButton = IDC_STOPPED;

			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, true);
			break;

		case SerialTapeState::NoTape:
		default:
			nIDCheckButton = IDC_STOPPED;

			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, true);
			break;
	}

	CheckRadioButton(hwndDlg,
	                 IDC_PLAYING,
	                 IDC_STOPPED,
	                 nIDCheckButton);
}

INT_PTR CALLBACK TapeControlDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM /* lParam */)
{
	switch (message)
	{
		case WM_INITDIALOG:
			SetDlgItemText(hwndDlg, IDC_TAPE_FILENAME, TapeFileName);
			UpdateState(hwndDlg);
			return TRUE;

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				hCurrentDialog = nullptr;
			}
			else
			{
				hCurrentDialog = hwndTapeControl;
			}
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_TAPE_CONTROL_MAP:
					if (HIWORD(wParam) == LBN_SELCHANGE)
					{
						LRESULT s = SendMessage(hwndMap, LB_GETCURSEL, 0, 0);

						if (s != LB_ERR && s >= 0 && s < (int)TapeMap.size())
						{
							SetTapePosition(TapeMap[s].time);
						}
					}
					return FALSE;

				case IDC_TAPE_CONTROL_PLAY:
					SerialStopTapeRecording(true);
					SerialPlayTape();
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_STOP:
					SerialStopTapeRecording(true);
					SerialStopTape();
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_EJECT:
					SerialStopTapeRecording(false);
					SerialEjectTape();
					SetDlgItemText(hwndDlg, IDC_TAPE_FILENAME, "");
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_REWIND:
					RewindTape();
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_LOAD_TAPE:
					mainWin->LoadTape();
					return TRUE;

				case IDC_TAPE_CONTROL_RECORD:
					TapeControlRecord();
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_UNLOCK: {
					bool Unlock = IsDlgItemChecked(hwndDlg, IDC_TAPE_CONTROL_UNLOCK);
					mainWin->SetUnlockTape(Unlock);
					return TRUE;
				}

				case IDCANCEL:
					TapeControlCloseDialog();
					return TRUE;
			}
	}

	return FALSE;
}

#endif

#ifdef WIN32

static void TapeControlRecord()
{
	if (!TapeState.Recording)
	{
		// Query for new file name
		char FileName[MAX_PATH];
		FileName[0] = '\0';

		if (mainWin->NewTapeImage(FileName, sizeof(FileName)))
		{
			CloseTape();

			// Create file
			if (!SerialRecordTape(FileName))
			{
				mainWin->Report(MessageType::Error,
				                "Error creating tape file:\n  %s", FileName);
			}
		}
	}

	UpdateState(hwndTapeControl);
}

#endif

void TapeControlCloseTape()
{
	#ifdef WIN32

	SendMessage(hwndMap, LB_RESETCONTENT, 0, 0);
	UpdateState(hwndTapeControl);

	#endif
}

void TapeControlSetFileName(const char *FileName)
{
	#ifdef WIN32

	SetDlgItemText(hwndTapeControl, IDC_TAPE_FILENAME, FileName);

	#else

	UNREFERENCED_PARAMETER(FileName);

	#endif
}

void TapeControlSetUnlock(bool Unlock)
{
	#ifdef WIN32

	SetDlgItemChecked(hwndTapeControl, IDC_TAPE_CONTROL_UNLOCK, Unlock);

	#else

	UNREFERENCED_PARAMETER(Unlock);

	#endif
}
