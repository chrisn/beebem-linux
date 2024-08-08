/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 2023 Chris Needham

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

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "FileUtils.h"
#include "StringUtils.h"

/****************************************************************************/

const char DIR_SEPARATOR = '\\';

/****************************************************************************/

bool FileExists(const char* PathName)
{
	#ifdef WIN32

	DWORD dwAttrib = GetFileAttributes(PathName);

	return dwAttrib != INVALID_FILE_ATTRIBUTES &&
	       !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

	#else

	struct stat st;
	
	if (stat(PathName, &st) == -1)
	{
		return false;
	}

	return S_ISREG(st.st_mode);

	#endif
}

/****************************************************************************/

bool FolderExists(const char* PathName)
{
	#ifdef WIN32

	DWORD dwAttrib = GetFileAttributes(PathName);

	return dwAttrib != INVALID_FILE_ATTRIBUTES &&
	       (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0;

	#else

	struct stat st;
	
	if (stat(PathName, &st) == -1)
	{
		return false;
	}

	return S_ISDIR(st.st_mode);

	#endif
}

/****************************************************************************/

std::string AppendPath(const std::string& BasePath, const std::string& Path)
{
	std::string PathName(BasePath);

	if (!PathName.empty())
	{
		char LastChar = PathName[PathName.size() - 1];

		if (LastChar != '\\' && LastChar != '/')
		{
			PathName.append(1, DIR_SEPARATOR);
		}
	}

	PathName += Path;

	return PathName;
}

/****************************************************************************/

bool HasFileExt(const char* FileName, const char* Ext)
{
	const size_t ExtLen = strlen(Ext);
	const size_t FileNameLen = strlen(FileName);

	return FileNameLen >= ExtLen &&
	       StrCaseCmp(FileName + FileNameLen - ExtLen, Ext) == 0;
}

/****************************************************************************/

std::string ReplaceFileExt(const std::string& FileName, const char* Ext)
{
	size_t index = FileName.find_last_of(".");

	if (index == std::string::npos)
	{
		return FileName;
	}

	std::string NewFileName(FileName, 0, index);

	NewFileName += Ext;

	return NewFileName;
}

/****************************************************************************/

void GetPathFromFileName(const char* FileName, char* Path, size_t Size)
{
	Path[0] = '\0';

	const char* Pos = strrchr(FileName, DIR_SEPARATOR);

	if (Pos != nullptr)
	{
		size_t PathLength = Pos - FileName;

		if (PathLength < Size)
		{
			strncpy(Path, FileName, PathLength);
			Path[PathLength] = '\0';
		}
	}
}

/****************************************************************************/

void MakeFileName(char* Path, size_t /* Size */, const char* DirName, const char* FileName, ...)
{
	va_list args;
	va_start(args, FileName);

	strcpy(Path, DirName);

	size_t Len = strlen(Path);

	if (Path[Len] != DIR_SEPARATOR)
	{
		Path[Len++] = DIR_SEPARATOR;
	}

	vsprintf(&Path[Len], FileName, args);

	va_end(args);
}

/****************************************************************************/

void MakePreferredPath(char* PathName)
{
	for (size_t i = 0; i < strlen(PathName); ++i)
	{
		if (PathName[i] == '/')
		{
			PathName[i] = DIR_SEPARATOR;
		}
	}
}

/****************************************************************************/
