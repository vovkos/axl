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

#define _AXL_WIN_MAPPING_H

#include "axl_io_win_Pch.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

class Mapping: public sys::win::Handle
{
public:
	bool
	create(
		HANDLE hFile,
		SECURITY_ATTRIBUTES* secAttr,
		uint_t pageProtection,
		uint64_t maxSize,
		const sl::StringRef_w& name = NULL
		);

	bool
	open(
		uint_t access,
		bool doInheritHandle,
		const sl::StringRef_w& name
		);
};

//..............................................................................

class UnmapViewOfFile
{
public:
	void
	operator () (void* p)
	{
		::UnmapViewOfFile(p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MappedView: public sl::Handle<void*, UnmapViewOfFile>
{
public:
	void*
	view(
		HANDLE hMapping,
		uint_t access,
		uint64_t offset,
		uint32_t size
		);
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
