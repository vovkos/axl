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

#define _AXL_IO_WIN_FILE_H

#include "axl_io_win_Pch.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class File: public sl::Handle<HANDLE, sys::win::CloseHandle, sl::MinusOne<HANDLE> >
{
public:
	bool
	create(
		const sl::StringRef_w& fileName,
		uint_t accessMode,
		uint_t shareMode,
		SECURITY_ATTRIBUTES* secAttr,
		uint_t creationDisposition,
		uint_t flagsAttributes = 0
		);

	bool
	duplicate(
		HANDLE sourceProcess,
		HANDLE sourceHandle,
		HANDLE targetProcess,
		dword_t accessMode,
		bool isInheritable,
		dword_t options
		);

	uint64_t
	getSize() const;

	bool
	setSize(uint64_t size);

	uint64_t
	getPosition() const;

	bool
	setPosition(uint64_t offset) const;

	bool
	flush()
	{
		bool_t result = ::FlushFileBuffers(m_h);
		return err::complete(result);
	}

	bool
	read(
		void* p,
		dword_t size,
		dword_t* actualSize
		) const
	{
		bool_t result = ::ReadFile(m_h, p, size, actualSize, NULL);
		return err::complete(result);
	}

	bool
	write(
		const void* p,
		dword_t size,
		dword_t* actualSize
		)
	{
		bool_t result = ::WriteFile(m_h, p, size, actualSize, NULL);
		return err::complete(result);
	}

	bool
	ioctl(
		dword_t code,
		const void* inData,
		dword_t inDataSize,
		void* outData,
		dword_t outDataSize,
		dword_t* actualSize
		)
	{
		bool_t result = ::DeviceIoControl(m_h, code, (void*)inData, inDataSize, outData, outDataSize, actualSize, NULL);
		return err::complete(result);
	}

	bool
	overlappedRead(
		void* p,
		dword_t size,
		OVERLAPPED* overlapped
		) const
	{
		bool_t result = ::ReadFile(m_h, p, size, NULL, overlapped);
		return completeOverlappedRequest(result);
	}

	bool
	overlappedWrite(
		const void* p,
		dword_t size,
		OVERLAPPED* overlapped
		)
	{
		bool_t result = ::WriteFile(m_h, p, size, NULL, overlapped);
		return completeOverlappedRequest(result);
	}

	bool
	overlappedIoctl(
		dword_t code,
		const void* inData,
		dword_t inDataSize,
		void* outData,
		dword_t outDataSize,
		OVERLAPPED* overlapped
		)
	{
		bool_t result = ::DeviceIoControl(m_h, code, (void*)inData, inDataSize, outData, outDataSize, NULL, overlapped);
		return completeOverlappedRequest(result);
	}

	bool
	readEx(
		void* p,
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE completionFunc
		) const
	{
		bool_t result = ::ReadFileEx(m_h, p, size, overlapped, completionFunc);
		return err::complete(result);
	}

	bool
	writeEx(
		const void* p,
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE completionFunc
		)
	{
		bool_t result = ::WriteFileEx(m_h, p, size, overlapped, completionFunc);
		return err::complete(result);
	}

	size_t
	read(
		void* p,
		size_t size
		) const;

	size_t
	write(
		const void* p,
		size_t size
		);

	size_t
	overlappedRead(
		void* p,
		size_t size
		) const;

	size_t
	overlappedWrite(
		const void* p,
		size_t size
		);

	bool
	overlappedRead(
		void* p,
		dword_t size,
		dword_t* actualSize
		) const;

	bool
	overlappedWrite(
		const void* p,
		dword_t size,
		dword_t* actualSize
		);

	bool
	overlappedIoctl(
		dword_t code,
		const void* inData,
		dword_t inDataSize,
		void* outData,
		dword_t outDataSize,
		dword_t* actualSize
		);

	static
	bool
	completeOverlappedRequest(bool_t result);

	bool
	getOverlappedResult(
		OVERLAPPED* overlapped,
		dword_t* actualSize
		) const;

	size_t
	getOverlappedResult(OVERLAPPED* overlapped) const;
};

//..............................................................................

class StdOverlapped: public OVERLAPPED
{
public:
	sys::NotificationEvent m_completionEvent;

public:
	StdOverlapped()
	{
		memset(this, 0, sizeof(OVERLAPPED));
		hEvent = m_completionEvent.m_event;
	}
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
