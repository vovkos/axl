// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_MAPPING_H

#include "axl_sys_win_Handle.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class Mapping: public sys::win::Handle
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

class UnmapViewOfFile 
{
public:
	void
	operator () (void* p)
	{
		::UnmapViewOfFile (p);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MappedView: public sl::Handle <void*, UnmapViewOfFile>
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
