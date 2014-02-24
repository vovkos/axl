// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_MAPPING_H

#include "axl_g_win_Handle.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class CMapping: public g::win::CHandle
{
public:
	bool
	Create (
		HANDLE hFile,
		SECURITY_ATTRIBUTES* pSecAttr,
		uint_t PageProtection,
		uint64_t MaxSize,
		const wchar_t* pName = NULL
		);

	bool 
	Open (
		uint_t Access,
		bool DoInheritHandle,
		const wchar_t* pName
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

class CMappedView: public rtl::CHandleT <void*, CUnmapViewOfFile>
{
public:
	void* 
	View (
		HANDLE hMapping,
		uint_t Access,
		uint64_t Offset,
		uint32_t Size
		);
};

//.............................................................................

} // namespace win
} // namespace io 
} // namespace axl
