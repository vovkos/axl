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
GetCurrentDir ();

bool
DoesFileExist (const char* pFileName);

bool
EnsureDirExists (const char* pFileName);

rtl::CString
GetFullFilePath (const char* pFileName);

rtl::CString
GetDir (const char* pFilePath);

rtl::CString
GetFileName (const char* pFilePath);

rtl::CString
GetExtension (const char* pFilePath);

rtl::CString
ConcatFilePath (
	rtl::CString* pFilePath,
	const char* pFileName
	);

inline
rtl::CString
ConcatFilePath (
	const char* pDir,
	const char* pFileName
	)
{
	rtl::CString FilePath = pDir;
	return ConcatFilePath (&FilePath, pFileName);
}

rtl::CString
FindFilePath (
	const char* pFileName,
	const char* pFirstDir,
	const rtl::CBoxListT <rtl::CString>* pDirList,
	bool DoFindInCurrentDir = true
	);

//.............................................................................

} // namespace io
} // namespace axl

