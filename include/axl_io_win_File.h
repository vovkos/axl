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

#define _AXL_WIN_FILE_H

#include "axl_io_win_FileHandle.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

class File: public FileHandle
{
public:
	bool
	create (
		const sl::StringRef_w& fileName,
		uint_t accessMode,
		uint_t shareMode,
		SECURITY_ATTRIBUTES* secAttr,
		uint_t creationDisposition,
		uint_t flagsAttributes = 0
		);

	uint64_t
	getSize () const;

	bool
	setSize (uint64_t size);

	uint64_t
	getPosition () const;

	bool
	setPosition (uint64_t offset) const;

	bool
	flush ()
	{
		bool_t result = ::FlushFileBuffers (m_h);
		return err::complete (result);
	}

	size_t
	read (
		void* p,
		size_t size
		) const;

	size_t
	write (
		const void* p,
		size_t size
		);

	bool
	read (
		void* p,
		dword_t size,
		dword_t* actualSize,
		OVERLAPPED* overlapped = NULL
		) const
	{
		bool_t result = ::ReadFile (m_h, p, size, actualSize, overlapped);
		return completeAsyncRequest (result, overlapped);
	}

	bool
	write (
		const void* p,
		dword_t size,
		dword_t* actualSize,
		OVERLAPPED* overlapped = NULL
		)
	{
		bool_t result = ::WriteFile (m_h, p, size, actualSize, overlapped);
		return completeAsyncRequest (result, overlapped);
	}

	bool
	ioctl (
		dword_t code,
		const void* inData,
		dword_t inDataSize,
		void* outData,
		dword_t outDataSize,
		dword_t* actualSize,
		OVERLAPPED* overlapped = NULL
		)
	{
		bool_t result = ::DeviceIoControl (m_h, code, (void*) inData, inDataSize, outData, outDataSize, actualSize, overlapped);
		return completeAsyncRequest (result, overlapped);
	}

	bool
	overlappedRead (
		void* p,
		dword_t size,
		dword_t* actualSize
		) const;

	bool
	overlappedWrite (
		const void* p,
		dword_t size,
		dword_t* actualSize
		);

	bool
	overlappedIoctl (
		dword_t code,
		const void* inData,
		dword_t inDataSize,
		void* outData,
		dword_t outDataSize,
		dword_t* actualSize
		);

	bool
	readEx (
		void* p,
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE completionFunc
		) const
	{
		bool_t result = ::ReadFileEx (m_h, p, size, overlapped, completionFunc);
		return completeAsyncRequest (result, overlapped);
	}

	bool
	writeEx (
		const void* p,
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE completionFunc
		)
	{
		bool_t result = ::WriteFileEx (m_h, p, size, overlapped, completionFunc);
		return completeAsyncRequest (result, overlapped);
	}
};

//..............................................................................

} // namespace win
} // namespace io {
} // namespace axl
