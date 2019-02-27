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

#define _AXL_IO_FILEENUMERATOR_H

#include "axl_io_Pch.h"

namespace axl {
namespace io {

//..............................................................................

#if (_AXL_OS_WIN)

class CloseFileEnumeratorHandle
{
public:
	void
	operator () (HANDLE h)
	{
		::FindClose(h);
	}
};

typedef sl::Handle<HANDLE, CloseFileEnumeratorHandle, sl::MinusOne<HANDLE> > FileEnumeratorHandle;

#elif (_AXL_OS_POSIX)

class CloseFileEnumeratorHandle
{
public:
	void
	operator () (DIR* h)
	{
		::closedir(h);
	}
};

typedef sl::Handle<DIR*, CloseFileEnumeratorHandle> FileEnumeratorHandle;

#endif

class FileEnumerator: public FileEnumeratorHandle
{
protected:
	sl::String m_nextFileName;

public:
	bool
	openDir(const sl::StringRef& dir);

	bool
	hasNextFile()
	{
		return !m_nextFileName.isEmpty();
	}

	sl::String
	getNextFileName();
};

//..............................................................................

} // namespace io
} // namespace axl
