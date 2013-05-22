// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILEPATHUTILS_H

#include "axl_rtl_BoxList.h"
#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

bool 
DoesFileExist (const char* pFileName);

rtl::CString
GetFullFilePath (const char* pFileName);

rtl::CString
GetDirName (const char* pFilePath);

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
	const char* pDirName,
	const char* pFileName
	)
{
	rtl::CString FilePath = pDirName;
	return ConcatFilePath (&FilePath, pFileName);
}

rtl::CString 
FindFilePath (
	const char* pFileName,
	const char* pFirstDirName, 
	const rtl::CBoxListT <rtl::CString>* pDirNameList,
	bool DoFindInCurrentDir = true
	);

rtl::CString
CreateTempFileName ();

//.............................................................................

} // namespace io
} // namespace axl

