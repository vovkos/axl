// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILEFIND_H

#include "axl_rtl_BoxList.h"
#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

rtl::CString
GetFullFilePath (const tchar_t* pFileName);

rtl::CString
GetDirName (const tchar_t* pFilePath);

rtl::CString
GetFileName (const tchar_t* pFilePath);

rtl::CString
GetExtension (const tchar_t* pFilePath);

rtl::CString
ConcatFilePath (
	rtl::CString* pFilePath,
	const tchar_t* pFileName
	);

inline
rtl::CString 
ConcatFilePath (
	const tchar_t* pDirName,
	const tchar_t* pFileName
	)
{
	rtl::CString FilePath = pDirName;
	return ConcatFilePath (&FilePath, pFileName);
}

rtl::CString 
FindFilePath (
	const tchar_t* pFileName,
	const tchar_t* pFirstDirName, 
	const rtl::CBoxListT <rtl::CString>* pDirNameList,
	bool DoFindInCurrentDir = true
	);

//.............................................................................

} // namespace io
} // namespace axl

