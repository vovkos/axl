// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_MAPPING_H

#include "axl_win_File.h"

namespace axl {
namespace win {

//.............................................................................

class CMapping: public CHandle
{
public:
	bool
	Create (
		HANDLE hFile,
		SECURITY_ATTRIBUTES* pSecAttr,
		ulong_t PageProtection,
		uint64_t MaxSize,
		const tchar_t* pName
		);

	bool 
	Open (
		ulong_t Access,
		bool DoInheritHandle,
		const tchar_t* pName
		);
};

//.............................................................................

class CUnmapViewOfFile 
{
public:
	void
	operator () (void* p)
	{
		::UnmapViewOfFile (p);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMappedView: public rtl::CHandleT <void*, NULL, CUnmapViewOfFile>
{
public:
	void* 
	View (
		HANDLE hMapping,
		ulong_t Access,
		uint64_t Offset,
		ulong_t Size
		);
};

//.............................................................................

} // namespace win
} // namespace axl
