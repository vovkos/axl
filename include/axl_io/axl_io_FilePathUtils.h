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
getHomeDir();

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

inline
bool
isAbsolutePath(const sl::StringRef& fileName) {
#if (_AXL_OS_WIN)
	return !fileName.isEmpty() && (
		fileName[0] == '/' ||
		fileName[0] == '\\' ||
		fileName.getLength() >= 2 && fileName[1] == ':'
	);
#else
	return fileName.isPrefix('/');
#endif
}

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
) {
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
) {
	return findFilePath(fileName, NULL, dirList, doFindInCurrentDir);
}

bool
isSymbolicLink(const sl::StringRef& fileName);

bool
getSymbolicLinkTarget(
	sl::String* targetName,
	const sl::StringRef& linkName
);

inline
sl::String
getSymbolicLinkTarget(const sl::StringRef& linkName) {
	sl::String targetName;
	getSymbolicLinkTarget(&targetName, linkName);
	return targetName;
}

bool
matchWildcard(
	const sl::StringRef& string0,
	const sl::StringRef& wildcard
);

//..............................................................................

#if (_AXL_OS_WIN)

namespace win {

sl::String_w
getWindowsDir();

sl::String_w
getSystemDir();

} // namespace win

#endif


} // namespace io
} // namespace axl
