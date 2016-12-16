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
};

//..............................................................................

} // namespace win
} // namespace io {
} // namespace axl
