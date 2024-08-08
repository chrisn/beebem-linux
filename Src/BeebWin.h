/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 1994  Nigel Magnay
Copyright (C) 1997  Mike Wyatt
Copyright (C) 1998  Robert Schmidt
Copyright (C) 2001  Richard Gellman
Copyright (C) 2004  Ken Lowe
Copyright (C) 2004  Rob O'Donnell
Copyright (C) 2005  Jon Welch

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

/* Mike Wyatt and NRM's port to win32 - 07/06/1997 */

#ifndef BEEBWIN_HEADER
#define BEEBWIN_HEADER

#include <stdarg.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>

//-- #include <ddraw.h>

#include "DiscType.h"
#include "KeyMap.h"
#include "Model.h"
#include "MonitorType.h"
#include "Port.h"
#include "Preferences.h"
#include "SoundStreamerType.h"
#include "Tube.h"
#include "Video.h"

// Registry defs for disabling windows keys
#define CFG_KEYBOARD_LAYOUT "SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"
#define CFG_SCANCODE_MAP "Scancode Map"

extern const char *WindowTitle;

union EightUChars
{
	unsigned char data[8];
	EightByteType eightbyte;
};

union SixteenUChars
{
	unsigned char data[16];
	EightByteType eightbytes[2];
};

#ifdef WIN32

struct bmiData
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[256];
};

#endif

struct LEDType
{
	bool ShiftLock;
	bool CapsLock;
	bool Motor;
	bool FloppyDisc[2];
	bool HDisc[4];
	bool ShowDisc;
	bool ShowKB;
};

extern LEDType LEDs;

enum class LEDColour
{
	Red,
	Green
};

enum TextToSpeechSearchDirection
{
	TTS_FORWARDS,
	TTS_BACKWARDS
};

enum TextToSpeechSearchType
{
	TTS_CHAR,
	TTS_BLANK,
	TTS_NONBLANK,
	TTS_ENDSENTENCE
};

struct TextToSpeechVoice
{
	std::string Id;
	std::string Description;
};

// A structure for our custom vertex type. We added texture coordinates
struct CUSTOMVERTEX
{
	D3DVECTOR position; // The position
	D3DCOLOR color; // The colour
	FLOAT tu; // The texture coordinates
	FLOAT tv;
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

enum class MessageType
{
	Error,
	Warning,
	Info,
	Question,
	Confirm
};

enum class MessageResult
{
	None,
	Yes,
	No,
	OK,
	Cancel
};

enum class DisplayRendererType
{
	GDI,
	DirectDraw,
	DirectX9
};

enum class JoystickOption
{
	Disabled,
	Joystick,
	AnalogueMouseStick,
	DigitalMouseStick
};

enum class BitmapCaptureFormat
{
	Bmp,
	Jpeg,
	Gif,
	Png
};

enum class BitmapCaptureResolution
{
	Display,
	_1280x1024,
	_640x512,
	_320x256
};

enum class VideoCaptureResolution
{
	Display,
	_640x512,
	_320x256
};

enum class KeyboardMappingType
{
	User,
	Default,
	Logical
};

enum class AMXSizeType
{
	_160x256,
	_320x256,
	_640x256
};

enum class PrinterPortType
{
	File,
	Clipboard,
	Lpt1,
	Lpt2,
	Lpt3,
	Lpt4
};

enum class TimingType
{
	FixedSpeed,
	FixedFPS
};

enum class DirectXFullScreenMode
{
	ScreenResolution,
	_640x480,
	_720x576,
	_800x600,
	_1024x768,
	_1280x720,
	_1280x1024,
	_1280x768,
	_1280x960,
	_1440x900,
	_1600x1200,
	_1920x1080,
	_2560x1440,
	_3840x2160
};

class BeebWin
{
public:
	BeebWin();
	~BeebWin();

	bool Initialise();
	void ApplyPrefs();
	void Shutdown();

	#ifdef WIN32

	static LRESULT CALLBACK WndProc(HWND hWnd,
	                                UINT nMessage,
	                                WPARAM wParam,
	                                LPARAM lParam);

	LRESULT WndProc(UINT nMessage, WPARAM wParam, LPARAM lParam);

	#endif

	void UpdateModelMenu();
	void SetSoundMenu(void);
	void SetPBuff(void);
	void SetImageName(const char *DiscName, int Drive, DiscType Type);
	void SetTapeSpeedMenu();
	void SetUnlockTape(bool Unlock);
	void SetRomMenu(); // LRW  Added for individual ROM/RAM
	void SelectTube(TubeDevice Device);
	void UpdateTubeMenu();
	void SelectFDC();
	void LoadFDC(const char *DLLName, bool Save);
	void UpdateLEDMenu();
	void SetDriveControl(unsigned char value);
	unsigned char GetDriveControl(void);
	void doLED(int sx,bool on);
	void updateLines(HDC hDC, int StartY, int NLines);
	void updateLines(int StartY, int NLines) {
		updateLines(m_hDC, StartY, NLines);
	}

	bool CursorShouldBeHidden(){ return m_HideCursor; }

	void doHorizLine(int Colour, int y, int sx, int width) {
		if (TeletextEnabled) y/=TeletextStyle;
		int d = (y*800)+sx+ScreenAdjust+(TeletextEnabled?36:0);
		if ((d+width)>(500*800)) return;
		if (d<0) return;
		memset(m_screen+d, Colour, width);
	}

	void doInvHorizLine(int Colour, int y, int sx, int width) {
		if (TeletextEnabled) y/=TeletextStyle;
		int d = (y*800)+sx+ScreenAdjust+(TeletextEnabled?36:0);
		char *vaddr;
		if ((d+width)>(500*800)) return;
		if (d<0) return;
		vaddr=m_screen+d;
		for (int n = 0; n < width; n++) *(vaddr+n) ^= Colour;
	}

	void doUHorizLine(int Colour, int y, int sx, int width) {
		if (TeletextEnabled) y /= TeletextStyle;
		if (y > 500) return;
		memset(m_screen + (y * 800) + sx, Colour, width);
	}

	// TODO: set m_screen to video_output->pixels
	EightUChars *GetLinePtr(int y) {
		int d = (y * 800) + ScreenAdjust;
		if (d > MAX_VIDEO_SCAN_LINES * 800)
			return (EightUChars *)(m_screen + MAX_VIDEO_SCAN_LINES * 800);
		return (EightUChars *)(m_screen + d);
	}

	SixteenUChars *GetLinePtr16(int y) {
		int d = (y * 800) + ScreenAdjust;
		if (d > MAX_VIDEO_SCAN_LINES * 800)
			return (SixteenUChars *)(m_screen + MAX_VIDEO_SCAN_LINES * 800);
		return (SixteenUChars *)(m_screen + d);
	}

	HWND GethWnd() { return m_hWnd; }

	void SetModel(Model NewModelType);
	void ResetBeebSystem(Model NewModelType, bool LoadRoms);
	void Break();

	void CreateArmCoPro();
	void DestroyArmCoPro();
	void CreateSprowCoPro();
	void DestroySprowCoPro();

	int StartOfFrame(void);
	bool UpdateTiming();
	void AdjustSpeed(bool up);
	bool ShouldDisplayTiming() const;
	void DisplayTiming();
	void UpdateWindowTitle();
	bool IsWindowMinimized() const;
	void DisplayClientAreaText(HDC hDC);
	void DisplayFDCBoardInfo(HDC hDC, int x, int y);

	#ifdef WIN32
	void InitJoystick();
	void ResetJoystick();
	void ScaleJoystick(unsigned int x, unsigned int y);
	#endif

	void SetMousestickButton(int index, bool button);
	void ScaleMousestick(unsigned int x, unsigned int y);
	void HandleCommand(UINT MenuID);
	void SetAMXPosition(unsigned int x, unsigned int y);
	void ChangeAMXPosition(int deltaX, int deltaY);

	#ifdef WIN32
	void CaptureMouse();
	void ReleaseMouse();
	#endif

	void Activate(bool Active);
	void Focus(bool Focus);
	#ifdef WIN32
	void OnSize(WPARAM ResizeType, int Width, int Height);
	#endif
	bool IsFrozen() const;
	void TogglePause();
	bool IsPaused() const;
	void SetFreezeWhenInactive(bool State);
	void EditRomConfig();
	void OpenUserKeyboardDialog();
	void UserKeyboardDialogClosed();
	void ShowMenu(bool on);
	void HideMenu(bool hide);
	bool IsFullScreen() const { return m_FullScreen; }
	// Member function for SDL to access (+ some code that's still in main.cpp):
	void SetFullScreenToggle(bool fullscreen) { m_FullScreen = fullscreen; }
	void ResetTiming(void);
	int TranslateKey(int vkey, bool keyUp, int &row, int &col);
	void ParseCommandLine(void);
	void CheckForLocalPrefs(const char *path, bool bLoadPrefs);
	bool FindCommandLineFile(char *FileName);
	void HandleCommandLineFile(int Drive, const char *FileName);
	bool CheckUserDataPath(bool Persist);
	bool CopyFiles(const char* SourceFileSpec, const char* DestPath);
	#ifndef WIN32
	int NftwCallback(const char* FileName, const struct stat* pStat);
	#endif
	void SelectUserDataPath(void);
	void StoreUserDataPath(void);
	bool NewTapeImage(char *FileName, int Size);
	const char *GetAppPath() const { return m_AppPath; }
	const char *GetUserDataPath() const { return m_UserDataPath; }
	void GetDataPath(const char *folder, char *path);
	void QuickLoad();
	void QuickSave();
	void LoadUEFState(const char *FileName);
	void SaveUEFState(const char *FileName);
	bool LoadUEFTape(const char *FileName);
	bool LoadCSWTape(const char *FileName);

	void HandleKeyboardTimer();
	void OnCopy();
	void OnPaste();
	void ClearClipboardBuffer();
	void PrintChar(unsigned char Value);
	void CopyPrinterBufferToClipboard();

	void SetBitmapCaptureFormat(BitmapCaptureFormat Format);
	void UpdateBitmapCaptureFormatMenu();
	void SetBitmapCaptureResolution(BitmapCaptureResolution Resolution);
	void UpdateBitmapCaptureResolutionMenu();
	void CaptureBitmapPending(bool autoFilename);

	void DoShiftBreak();
	bool HasKbdCmd() const;
	void SetKeyboardTimer();
	void SetBootDiscTimer();
	void KillBootDiscTimer();

	void SaveBeebEmID(FILE *SUEF);
	void SaveEmuUEF(FILE *SUEF);
	void LoadEmuUEF(FILE *SUEF,int Version);

	#ifdef WIN32
	bool InitClass();
	bool CreateBeebWindow();
	#endif

	void UpdateOptionsMenu();

	#ifdef WIN32
	void DisableRoundedCorners(HWND hWnd);
	#endif

	void FlashWindow();
	void CreateBitmap(void);
	void InitMenu();
	void SetMonitorType(MonitorType Type);
	void UpdateMonitorMenu();

	void ToggleSerial();
	void DisableSerial();
	void ConfigureSerial();
	void UpdateSerialMenu();
	void OnIP232Error(int Error);

	void UpdateEconetMenu();

	void UpdateSFXMenu();

	#ifdef WIN32
	void DisableWindowsKeys();
	bool RebootSystem();
	#endif

	void UpdateDisableKeysMenu();

	void SetDisplayRenderer(DisplayRendererType DisplayRenderer);
	void UpdateDisplayRendererMenu();

	void SetSoundStreamer(SoundStreamerType StreamerType);
	void UpdateSoundStreamerMenu();

	void SetSoundSampleRate(unsigned int SampleRate);
	void UpdateSoundSampleRateMenu();

	void SetSoundVolume(int Volume);
	void UpdateSoundVolumeMenu();

	#if ENABLE_SPEECH
	void EnableSpeech(bool Enable);
	#endif

	void CheckMenuItem(UINT id, bool Checked);
	void CheckMenuRadioItem(UINT FirstID, UINT LastID, UINT SelectedID);
	void EnableMenuItem(UINT id, bool Enabled);

	// DirectX - calls DDraw or DX9 fn
	void InitDX();
	void ResetDX();
	void ReinitDX();
	void ExitDX();
	void UpdateSmoothing();

	void SetMotionBlur(int MotionBlur);
	void UpdateMotionBlurMenu();

	// DirectDraw
	HRESULT InitDirectDraw();
	HRESULT InitSurfaces();
	void CloseSurfaces();

	// DirectX9
	bool InitDX9();
	void CloseDX9();
	HRESULT InitD3DDevice();
	void CloseD3DDevice();
	void RenderDX9();
	void OnDeviceLost();

	void SetWindowSize(int Width, int Height);
	void UpdateWindowSizeMenu();
	void SetDirectXFullScreenMode(DirectXFullScreenMode Mode);
	void TranslateDDSize();
	void UpdateDirectXFullScreenModeMenu();
	void ToggleFullScreen();
	void CalcAspectRatioAdjustment(int DisplayWidth, int DisplayHeight);

	// Timing
	void UpdateSpeedMenu();
	void TranslateTiming();
	void SetRealTimeTarget(double RealTimeTarget);

	void SetKeyboardMapping(KeyboardMappingType KeyboardMapping);
	void UpdateKeyboardMappingMenu();
	void TranslateKeyMapping();
	bool ReadDisc(int Drive, bool bCheckForPrefs);
	bool Load1770DiscImage(const char *FileName, int Drive, DiscType Type);
	bool Load8271DiscImage(const char *FileName, int Drive, int Tracks, DiscType Type);
	void LoadTape();
	bool LoadTape(const char *FileName);

	void SetJoystickOption(JoystickOption Option);
	void UpdateJoystickMenu();

	void RestoreState(void);
	void SaveState(void);
	void NewDiscImage(int Drive);
	void CreateDFSDiscImage(const char *FileName, int Drive, int Heads, int Tracks);
	void EjectDiscImage(int Drive);
	void ExportDiscFiles(int menuId);
	void ImportDiscFiles(int menuId);
	void SelectHardDriveFolder();
	void ToggleWriteProtect(int Drive);
	void SetDiscWriteProtect(int Drive, bool WriteProtect);
	void SetDiscWriteProtects();
	void SetWindowAttributes(bool wasFullScreen);

	void SetAMXSize(AMXSizeType Size);
	void UpdateAMXSizeMenu();
	void TranslateAMX();
	void SetAMXAdjust(int Adjust);
	void UpdateAMXAdjustMenu();

	void SetPrinterPort(PrinterPortType PrinterPort);
	void UpdatePrinterPortMenu();
	bool GetPrinterFileName();
	bool TogglePrinter();
	void TranslatePrinterPort();

	// AVI recording
	void SetVideoCaptureResolution(VideoCaptureResolution Resolution);
	void SetVideoCaptureFrameSkip(int FrameSkip);
	void UpdateVideoCaptureResolutionMenu();
	void UpdateVideoCaptureFrameSkipMenu();
	void UpdateVideoCaptureMenu();
	void CaptureVideo();
	void EndVideo();
	bool IsCapturing() const;

	// Bitmap capture
	void CaptureBitmap(int SourceX,
	                   int SourceY,
	                   int SourceWidth,
	                   int SourceHeight,
	                   bool Teletext);
	bool GetImageFile(char *FileName, int Size);
	bool GetImageEncoderClsid(const WCHAR *mimeType, CLSID *encoderClsid);

	// Debugger
	void OpenDebugWindow();

	bool InitTextToSpeech();
	void TextToSpeechResetState();
	void CloseTextToSpeech();
	bool InitTextToSpeechVoices();
	void InitVoiceMenu();
	int TextToSpeechGetSelectedVoice();
	ISpObjectToken* TextToSpeechGetSelectedVoiceToken();
	void TextToSpeechSetVoice(int Index);
	void TextToSpeechSetVoice(ISpObjectToken* pToken);
	void TextToSpeechSelectVoiceMenuItem(int Index);
	void Speak(const char *text, DWORD flags);
	void SpeakChar(unsigned char c);
	void TextToSpeechClearBuffer();
	void TextToSpeechToggleAutoSpeak();
	void TextToSpeechToggleSpeakPunctuation();
	void TextToSpeechIncreaseRate();
	void TextToSpeechDecreaseRate();
	bool TextToSpeechSearch(TextToSpeechSearchDirection dir,
	                        TextToSpeechSearchType type);
	void TextToSpeechReadChar();
	void TextToSpeechReadWord();
	void TextToSpeechReadLine();
	void TextToSpeechReadSentence();
	void TextToSpeechReadScreen();
	void TextToSpeechKey(WPARAM wParam);

	void InitTextView();
	void CloseTextView();
	void TextView();
	void TextViewSpeechSync();
	void TextViewSetCursorPos(int line, int col);
	void TextViewSyncWithBeebCursor();

	void LoadUserKeyMap(void);
	void SaveUserKeyMap(void);

	MessageResult Report(MessageType type, const char *format, ...);
	MessageResult ReportV(MessageType type, const char *format, va_list args);

	// Preferences
	void LoadPreferences();
	void LoadHardwarePreferences();
	void LoadTubePreferences();
	void LoadWindowPosPreferences(int Version);
	void LoadTimingPreferences(int Version);
	void LoadDisplayPreferences(int Version);
	void LoadSoundPreferences(int Version);
	void LoadInputPreferences(int Version);
	void LoadAMXMousePreferences(int Version);
	void LoadPrinterPreferences(int Version);
	void LoadTextToSpeechPreferences();
	void LoadUIPreferences(int Version);
	void LoadTapePreferences(int Version);
	void LoadSerialPortPreferences(int Version);
	void LoadTeletextAdapterPreferences(int Version);
	void LoadCapturePreferences(int Version);
	void LoadDiskPreferences();
	void LoadUserPortRTCPreferences();
	void LoadDebugPreferences();
	void LoadKeyMapPreferences();
	void LoadAutoSavePreferences();
	void LoadCMOSPreferences();
	void LoadSWRAMPreferences();
	void LoadFilePathPreferences();
	void LoadUserPortBreakoutPreferences();
	void SavePreferences(bool saveAll);

	int FindEnum(const std::string& Value, const char* const* Names, int Default);

	// Timers
	const int TIMER_KEYBOARD       = 1;
	const int TIMER_AUTOBOOT_DELAY = 2;
	const int TIMER_PRINTER        = 3;

	// Main window
	HWND m_hWnd;
	char m_szTitle[256];
	bool m_FullScreen;
	bool m_StartFullScreen;

	// Menu
	HMENU m_hMenu;
	bool m_MenuOn;
	bool m_HideMenuEnabled;
	bool m_DisableMenu;

	// Timing
	bool m_ShowSpeedAndFPS;
	TimingType m_TimingType;
	int m_TimingSpeed;
	double m_RealTimeTarget;
	int m_CyclesPerSec;
	DWORD m_LastTickCount;
	DWORD m_LastStatsTickCount;
	int m_LastTotalCycles;
	int m_LastStatsTotalCycles;
	DWORD m_TickBase;
	int m_CycleBase;
	int m_MinFrameCount;
	DWORD m_LastFPSCount;
	int m_FPSTarget;
	int m_ScreenRefreshCount;
	double m_RelativeSpeed;
	double m_FramesPerSecond;

	// Pause / freeze emulation
	bool m_StartPaused;
	bool m_Paused;
	bool m_WasPaused;
	bool m_FreezeWhenInactive;
	bool m_Frozen;

	// Window size
	int m_XWinSize;
	int m_YWinSize;
	int m_XLastWinSize;
	int m_YLastWinSize;
	int m_XWinPos;
	int m_YWinPos;
	int m_XDXSize;
	int m_YDXSize;
	float m_XRatioAdj;
	float m_YRatioAdj;
	float m_XRatioCrop;
	float m_YRatioCrop;

	// Graphics rendering
	HDC m_hDC;
	HGDIOBJ m_hOldObj;
	HDC m_hDCBitmap;
	HGDIOBJ m_hBitmap;
	#ifdef WIN32
	bmiData m_bmi;
	#endif
	MonitorType m_MonitorType;
	char* m_screen;
	char* m_screen_blur;
	int m_LastStartY;
	int m_LastNLines;
	int m_MotionBlur;
	char m_BlurIntensities[8];
	bool m_MaintainAspectRatio;
	DisplayRendererType m_DisplayRenderer;
	DisplayRendererType m_CurrentDisplayRenderer;
	DirectXFullScreenMode m_DDFullScreenMode;
	LEDColour m_DiscLedColour;

	// DirectX stuff
	bool m_DXInit;
	bool m_DXResetPending;
	bool m_DXDeviceLost;

	// DirectDraw stuff
	HINSTANCE m_hInstDDraw;
	LPDIRECTDRAW m_DD; // DirectDraw object
	LPDIRECTDRAW2 m_DD2; // DirectDraw object
	LPDIRECTDRAWSURFACE m_DDSPrimary; // DirectDraw primary surface
	LPDIRECTDRAWSURFACE2 m_DDS2Primary; // DirectDraw primary surface
	LPDIRECTDRAWSURFACE m_DDSOne; // Offscreen surface 1
	LPDIRECTDRAWSURFACE2 m_DDS2One; // Offscreen surface 1
	bool m_DXSmoothing;
	bool m_DXSmoothMode7Only;
	LPDIRECTDRAWCLIPPER m_Clipper; // clipper for primary

	// Direct3D9 stuff
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DMATRIX m_TextureMatrix;

	unsigned char cols[256];

	// Joystick input
	bool m_JoystickCaptured;
	JOYCAPS m_JoystickCaps;
	JoystickOption m_JoystickOption;

	// Mouse capture
	bool m_HideCursor;
	bool m_CaptureMouse;
	bool m_MouseCaptured;
	POINT m_RelMousePos;

	// Keyboard input
	KeyboardMappingType m_KeyboardMapping;
	bool m_KeyMapAS;
	bool m_KeyMapFunc;
	char m_UserKeyMapPath[MAX_PATH];
	bool m_DisableKeysWindows;
	bool m_DisableKeysBreak;
	bool m_DisableKeysEscape;
	bool m_DisableKeysShortcut;
	bool m_ShiftPressed;
	bool m_ShiftBooted;
	int m_vkeyPressed[256][2][2];

	// File paths
	char m_AppPath[MAX_PATH];
	char m_UserDataPath[MAX_PATH];
	bool m_CustomData;
	char m_DiscPath[MAX_PATH]; // JGH
	bool m_WriteProtectDisc[2];
	bool m_WriteProtectOnLoad;
	char m_CopySourcePath[MAX_PATH];
	char m_CopyDestPath[MAX_PATH];

	// AMX mouse
	AMXSizeType m_AMXSize;
	int m_AMXXSize;
	int m_AMXYSize;
	int m_AMXAdjust;

	// Preferences
	std::string m_PrefsFileName;
	Preferences m_Preferences;
	bool m_AutoSavePrefsCMOS;
	bool m_AutoSavePrefsFolders;
	bool m_AutoSavePrefsAll;
	bool m_AutoSavePrefsChanged;

	// Clipboard
	std::vector<char> m_ClipboardBuffer;
	size_t m_ClipboardLength;
	size_t m_ClipboardIndex;

	// Printer
	std::vector<unsigned char> m_PrinterBuffer;
	bool m_TranslateCRLF;
	PrinterPortType m_PrinterPort;
	std::string m_PrinterFileName;
	std::string m_PrinterDevice;

	// Serial
	std::string m_SerialPort;

	// Command line
	char m_CommandLineFileName1[MAX_PATH];
	char m_CommandLineFileName2[MAX_PATH];
	std::string m_DebugScriptFileName;
	std::string m_DebugLabelsFileName;
	bool m_HasCommandLineModel;
	Model m_CommandLineModel;
	bool m_HasCommandLineTube;
	TubeDevice m_CommandLineTube;

	// Startup key sequence
	std::string m_KbdCmd;
	int m_KbdCmdPos;
	int m_KbdCmdKey;
	bool m_KbdCmdPress;
	int m_KbdCmdDelay;
	int m_KbdCmdLastCycles;
	bool m_KeyboardTimerElapsed;

	// Disc auto-boot
	bool m_NoAutoBoot;
	int m_AutoBootDelay;
	bool m_AutoBootDisc;
	bool m_BootDiscTimerElapsed;

	// ROMs
	bool RomWritePrefs[16];

	// Bitmap capture
	ULONG_PTR m_gdiplusToken;
	bool m_CaptureBitmapPending;
	bool m_CaptureBitmapAutoFilename;
	char m_CaptureFileName[MAX_PATH];
	BitmapCaptureResolution m_BitmapCaptureResolution;
	BitmapCaptureFormat m_BitmapCaptureFormat;

	// Video capture
	#ifdef WIN32
	bmiData m_Avibmi;
	#endif
	HBITMAP m_AviDIB;
	HDC m_AviDC;
	char* m_AviScreen;
	int m_AviFrameSkip;
	int m_AviFrameSkipCount;
	int m_AviFrameCount;
	VideoCaptureResolution m_VideoCaptureResolution;

	// Text to speech
	bool m_TextToSpeechEnabled;
	std::vector<TextToSpeechVoice> m_TextToSpeechVoices;
	HMENU m_hVoiceMenu;
	ISpVoice *m_SpVoice;
	int m_SpeechLine;
	int m_SpeechCol;
	static const int MAX_SPEECH_LINE_LEN = 128;
	static const int MAX_SPEECH_SENTENCE_LEN = 128 * 25;
	static const int MAX_SPEECH_SCREEN_LEN = 128 * 32;
	char m_SpeechText[MAX_SPEECH_LINE_LEN + 1];
	bool m_SpeechSpeakPunctuation;
	bool m_SpeechWriteChar;
	static const int MAX_SPEECH_BUF_LEN = 160;
	char m_SpeechBuf[MAX_SPEECH_BUF_LEN + 1];
	int m_SpeechBufPos;
	int m_SpeechRate;

	// Text view
	HWND m_hTextView;
	bool m_TextViewEnabled;
	WNDPROC m_TextViewPrevWndProc;
	static const int MAX_TEXTVIEW_SCREEN_LEN = 128 * 32;
	char m_TextViewScreen[MAX_TEXTVIEW_SCREEN_LEN + 1];

	// Debug
	bool m_WriteInstructionCounts;
};

class CArm;
class CSprowCoPro;

extern CArm *arm;
extern CSprowCoPro *sprow;

extern const char DefaultBlurIntensities[8];

extern const char* const MachineTypeStr[];
extern const char* const TubeDeviceStr[];

#endif
