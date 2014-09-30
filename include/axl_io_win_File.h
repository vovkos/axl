// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_FILE_H

#include "axl_g_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class CFile: public g::win::CFileHandle
{
public:
	bool
	create (
		const wchar_t* fileName,
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
	readEx (
		void* p,
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		) const
	{
		bool_t result = ::ReadFileEx (m_h, p, size, overlapped, pfOnComplete);
		return completeAsyncRequest (result, overlapped);
	}

	bool
	writeEx (
		const void* p,
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		)
	{
		bool_t result = ::WriteFileEx (m_h, p, size, overlapped, pfOnComplete);
		return completeAsyncRequest (result, overlapped);
	}
};

//.............................................................................

} // namespace win
} // namespace io {
} // namespace axl
