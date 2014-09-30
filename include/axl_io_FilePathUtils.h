// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILEPATHUTILS_H

#include "axl_rtl_BoxList.h"
#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

rtl::CString
getCurrentDir ();

bool
doesFileExist (const char* fileName);

bool
ensureDirExists (const char* fileName);

rtl::CString
getFullFilePath (const char* fileName);

rtl::CString
getDir (const char* filePath);

rtl::CString
getFileName (const char* filePath);

rtl::CString
getExtension (const char* filePath);

rtl::CString
concatFilePath (
	rtl::CString* filePath,
	const char* fileName
	);

inline
rtl::CString
concatFilePath (
	const char* dir,
	const char* fileName
	)
{
	rtl::CString filePath = dir;
	return concatFilePath (&filePath, fileName);
}

rtl::CString
findFilePath (
	const char* fileName,
	const char* firstDir,
	const rtl::CBoxListT <rtl::CString>* dirList,
	bool doFindInCurrentDir = true
	);

//.............................................................................

} // namespace io
} // namespace axl

