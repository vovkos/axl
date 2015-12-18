// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILEPATHUTILS_H

#include "axl_sl_BoxList.h"
#include "axl_sl_String.h"

namespace axl {
namespace io {

//.............................................................................

sl::String 
getTempDir ();

sl::String
getCurrentDir ();

sl::String
getExeFilePath ();

sl::String
getExeDir ();

bool
doesFileExist (const char* fileName);

bool
ensureDirExists (const char* fileName);

sl::String
getFullFilePath (const char* fileName);

sl::String
getDir (const char* filePath);

sl::String
getFileName (const char* filePath);

sl::String
getExtension (const char* filePath);

sl::String
concatFilePath (
	sl::String* filePath,
	const char* fileName
	);

inline
sl::String
concatFilePath (
	const char* dir,
	const char* fileName
	)
{
	sl::String filePath = dir;
	return concatFilePath (&filePath, fileName);
}

sl::String
findFilePath (
	const char* fileName,
	const char* firstDir,
	const sl::BoxList <sl::String>* dirList,
	bool doFindInCurrentDir = true
	);

//.............................................................................

} // namespace io
} // namespace axl

