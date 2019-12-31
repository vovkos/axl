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

#define _AXL_WIN_NAMEDPIPE_H

#include "axl_io_win_File.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

class NamedPipe: public File
{
public:
	bool
	create(
		const sl::StringRef_w& name,
		uint_t openMode,
		uint_t pipeMode,
		dword_t maxInstanceCount,
		dword_t txBufferSize,
		dword_t rxBufferSize,
		uint_t timeout,
		SECURITY_ATTRIBUTES* secAttr
		);

	bool
	open(
		const sl::StringRef_w& name,
		uint_t access = GENERIC_READ | GENERIC_WRITE,
		uint_t flags = FILE_FLAG_OVERLAPPED
		);

	bool
	connect()
	{
		bool_t result = ::ConnectNamedPipe(m_h, NULL);
		return err::complete(result);
	}

	bool
	overlappedConnect(OVERLAPPED* overlapped)
	{
		bool_t result = ::ConnectNamedPipe(m_h, overlapped);
		return completeOverlappedRequest(result);
	}

	bool
	overlappedConnect();
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
