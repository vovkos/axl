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

rtl::String
getCurrentDir ();

bool
doesFileExist (const char* fileName);

bool
ensureDirExists (const char* fileName);

rtl::String
getFullFilePath (const char* fileName);

rtl::String
getDir (const char* filePath);

rtl::String
getFileName (const char* filePath);

rtl::String
getExtension (const char* filePath);

rtl::String
concatFilePath (
	rtl::String* filePath,
	const char* fileName
	);

inline
rtl::String
concatFilePath (
	const char* dir,
	const char* fileName
	)
{
	rtl::String filePath = dir;
	return concatFilePath (&filePath, fileName);
}

rtl::String
findFilePath (
	const char* fileName,
	const char* firstDir,
	const rtl::BoxList <rtl::String>* dirList,
	bool doFindInCurrentDir = true
	);

//.............................................................................

} // namespace io
} // namespace axl

