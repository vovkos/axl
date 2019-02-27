//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_IO_FILEPATHUTILS_H

#include "axl_io_Pch.h"

namespace axl {
namespace io {

//..............................................................................

sl::String
getTempDir();

sl::String
getCurrentDir();

bool
setCurrentDir(const sl::StringRef& dir);

sl::String
getExeFilePath();

sl::String
getExeDir();

bool
doesFileExist(const sl::StringRef& fileName);

bool
ensureDirExists(const sl::StringRef& fileName);

bool
isDir(const sl::StringRef& fileName);

sl::String
getFullFilePath(const sl::StringRef& fileName);

sl::String
getDir(const sl::StringRef& filePath);

sl::String
getFileName(const sl::StringRef& filePath);

sl::String
getExtension(const sl::StringRef& filePath);

sl::String
concatFilePath(
	sl::String* filePath,
	const sl::StringRef& fileName
	);

inline
sl::String
concatFilePath(
	const sl::StringRef& dir,
	const sl::StringRef& fileName
	)
{
	sl::String filePath = dir;
	return concatFilePath(&filePath, fileName);
}

sl::String
findFilePath(
	const sl::StringRef& fileName,
	const sl::StringRef& firstDir,
	const sl::BoxList<sl::String>* dirList,
	bool doFindInCurrentDir = true
	);

inline
sl::String
findFilePath(
	const sl::StringRef& fileName,
	const sl::BoxList<sl::String>* dirList,
	bool doFindInCurrentDir = true
	)
{
	return findFilePath(fileName, NULL, dirList, doFindInCurrentDir);
}

bool
getSymbolicLinkTarget(
	sl::String* targetName,
	const sl::StringRef& linkName
	);

//..............................................................................

} // namespace io
} // namespace axl
