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
	create (
		HANDLE hFile,
		SECURITY_ATTRIBUTES* secAttr,
		uint_t pageProtection,
		uint64_t maxSize,
		const wchar_t* name = NULL
		);

	bool 
	open (
		uint_t access,
		bool doInheritHandle,
		const wchar_t* name
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
	view (
		HANDLE hMapping,
		uint_t access,
		uint64_t offset,
		uint32_t size
		);
};

//.............................................................................

} // namespace win
} // namespace io 
} // namespace axl
