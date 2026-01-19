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

//..............................................................................

class StdOverlapped: public OVERLAPPED {
public:
	sys::NotificationEvent m_completionEvent;

public:
	StdOverlapped() {
		memset(this, 0, sizeof(OVERLAPPED));
		hEvent = m_completionEvent.m_event;
	}
};

//.............................................................................

class File: public sl::Handle<HANDLE, sys::win::CloseHandle, sl::MinusOne<HANDLE> > {
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

	dword_t
	getType() const {
		return ::GetFileType(m_h);
	}

	uint64_t
	getSize() const;

	bool
	setSize(uint64_t size);

	uint64_t
	getPosition() const;

	bool
	setPosition(uint64_t offset);

	bool
	flush() {
		bool_t result = ::FlushFileBuffers(m_h);
		return err::complete(result);
	}

	bool
	read(
		void* p,
		dword_t size,
		dword_t* actualSize
	) {
		bool_t result = ::ReadFile(m_h, p, size, actualSize, NULL);
		return err::complete(result);
	}

	bool
	write(
		const void* p,
		dword_t size,
		dword_t* actualSize
	) {
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
	) {
		bool_t result = ::DeviceIoControl(m_h, code, (void*)inData, inDataSize, outData, outDataSize, actualSize, NULL);
		return err::complete(result);
	}

	bool
	overlappedRead(
		void* p,
		dword_t size,
		OVERLAPPED* overlapped
	) {
		bool_t result = ::ReadFile(m_h, p, size, NULL, overlapped);
		return completeOverlappedIo(result);
	}

	bool
	overlappedWrite(
		const void* p,
		dword_t size,
		OVERLAPPED* overlapped
	) {
		bool_t result = ::WriteFile(m_h, p, size, NULL, overlapped);
		return completeOverlappedIo(result);
	}

	bool
	overlappedIoctl(
		dword_t code,
		const void* inData,
		dword_t inDataSize,
		void* outData,
		dword_t outDataSize,
		OVERLAPPED* overlapped
	) {
		bool_t result = ::DeviceIoControl(m_h, code, (void*)inData, inDataSize, outData, outDataSize, NULL, overlapped);
		return completeOverlappedIo(result);
	}

	bool
	readEx(
		void* p,
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE completionFunc
	) {
		bool_t result = ::ReadFileEx(m_h, p, size, overlapped, completionFunc);
		return err::complete(result);
	}

	bool
	writeEx(
		const void* p,
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE completionFunc
	) {
		bool_t result = ::WriteFileEx(m_h, p, size, overlapped, completionFunc);
		return err::complete(result);
	}

	size_t
	read(
		void* p,
		size_t size
	);

	size_t
	write(
		const void* p,
		size_t size
	);

	size_t
	overlappedRead(
		void* p,
		size_t size
	);

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
	);

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
	completeOverlappedIo(bool_t result);

	static
	bool
	isOverlappedIoCompleted(OVERLAPPED* overlapped) {
		return HasOverlappedIoCompleted(overlapped);
	}

	bool
	getOverlappedResult(
		OVERLAPPED* overlapped,
		dword_t* actualSize
	) const;

	size_t
	getOverlappedResult(OVERLAPPED* overlapped) const;

	inline
	bool
	cancelIo(OVERLAPPED* overlapped);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
File::create(
	const sl::StringRef_w& fileName,
	uint_t accessMode,
	uint_t shareMode,
	SECURITY_ATTRIBUTES* secAttr,
	uint_t creationDisposition,
	uint_t flagsAttributes
) {
	close();

	m_h = ::CreateFileW(
		fileName.sz(),
		accessMode,
		shareMode,
		secAttr,
		creationDisposition,
		flagsAttributes,
		NULL
	);

	return err::complete(m_h != INVALID_HANDLE_VALUE);
}

inline
bool
File::duplicate(
	HANDLE sourceProcess,
	HANDLE sourceHandle,
	HANDLE targetProcess,
	dword_t accessMode,
	bool isInheritable,
	dword_t options
) {
	close();

	bool_t result = ::DuplicateHandle(
		sourceProcess,
		sourceHandle,
		targetProcess,
		&m_h,
		accessMode,
		isInheritable,
		options
	);

	return err::complete(result);
}

inline
uint64_t
File::getSize() const {
	ULARGE_INTEGER size;
	size.LowPart = ::GetFileSize(m_h, &size.HighPart);
	if (size.LowPart == INVALID_FILE_SIZE)
		return err::failWithLastSystemError(-1);

	return size.QuadPart;
}

inline
uint64_t
File::getPosition() const {
	LARGE_INTEGER offset = { 0 };
	offset.LowPart = ::SetFilePointer(m_h, 0, &offset.HighPart, FILE_CURRENT);
	if (offset.LowPart == INVALID_FILE_SIZE)
		return err::failWithLastSystemError<uint64_t>(-1);

	return offset.QuadPart;
}

inline
bool
File::setPosition(uint64_t offset0) {
	LARGE_INTEGER offset;
	offset.QuadPart = offset0;
	dword_t result = ::SetFilePointer(m_h, offset.LowPart, &offset.HighPart, FILE_BEGIN);
	return err::complete(result != INVALID_SET_FILE_POINTER);
}

inline
size_t
File::read(
	void* p,
	size_t size
) {
	dword_t actualSize;
	bool result = read(p, (dword_t)size, &actualSize);
	return result ? (size_t)actualSize : -1;
}

inline
size_t
File::write(
	const void* p,
	size_t size
) {
	dword_t actualSize;
	bool result = write(p, (dword_t)size, &actualSize);
	return result ? (size_t)actualSize : -1;
}

inline
size_t
File::overlappedRead(
	void* p,
	size_t size
) {
	dword_t actualSize;
	bool result = overlappedRead(p, (dword_t)size, &actualSize);
	return result ? (size_t)actualSize : -1;
}

inline
size_t
File::overlappedWrite(
	const void* p,
	size_t size
) {
	dword_t actualSize;
	bool result = overlappedWrite(p, (dword_t)size, &actualSize);
	return result ? (size_t)actualSize : -1;
}

inline
bool
File::overlappedRead(
	void* p,
	dword_t size,
	dword_t* actualSize
) {
	StdOverlapped overlapped;
	bool result = overlappedRead(p, size, &overlapped);
	return result ? getOverlappedResult(&overlapped, actualSize) : false;
}

inline
bool
File::overlappedWrite(
	const void* p,
	dword_t size,
	dword_t* actualSize
) {
	StdOverlapped overlapped;
	bool result = overlappedWrite(p, size, &overlapped);
	return result ? getOverlappedResult(&overlapped, actualSize) : false;
}

inline
bool
File::overlappedIoctl(
	dword_t code,
	const void* inData,
	dword_t inDataSize,
	void* outData,
	dword_t outDataSize,
	dword_t* actualSize
) {
	StdOverlapped overlapped;
	bool result = overlappedIoctl(code, inData, inDataSize, outData, outDataSize, &overlapped);
	return result ? getOverlappedResult(&overlapped, actualSize) : false;
}

inline
size_t
File::getOverlappedResult(OVERLAPPED* overlapped) const {
	dword_t actualSize;
	bool result = getOverlappedResult(overlapped, &actualSize);
	return result ? (size_t)actualSize : -1;
}

inline
bool
File::cancelIo(OVERLAPPED* overlapped) {
#if (_WIN32_WINNT >= 0x0600)
	bool_t result = ::CancelIoEx(m_h, overlapped);
#else
	bool_t result = ::CancelIo(m_h);
#endif
	return err::complete(result);
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
