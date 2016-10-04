// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILEPATHUTILS_H

#include "axl_io_Pch.h"

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
doesFileExist (const sl::StringRef& fileName);

bool
ensureDirExists (const sl::StringRef& fileName);

bool
isDir (const sl::StringRef& fileName);

sl::String
getFullFilePath (const sl::StringRef& fileName);

sl::String
getDir (const sl::StringRef& filePath);

sl::String
getFileName (const sl::StringRef& filePath);

sl::String
getExtension (const sl::StringRef& filePath);

sl::String
concatFilePath (
	sl::String* filePath,
	const sl::StringRef& fileName
	);

inline
sl::String
concatFilePath (
	const sl::StringRef& dir,
	const sl::StringRef& fileName
	)
{
	sl::String filePath = dir;
	return concatFilePath (&filePath, fileName);
}

sl::String
findFilePath (
	const sl::StringRef& fileName,
	const sl::StringRef& firstDir,
	const sl::BoxList <sl::String>* dirList,
	bool doFindInCurrentDir = true
	);

//.............................................................................

} // namespace io
} // namespace axl

