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

#include "axl_io_HidPch.h"

namespace axl {
namespace io {

class HidReport;
class HidStandaloneReport;
class HidDb;

//..............................................................................

class HidReportSerializer {
public:
	static
	size_t
	saveReportDecodeInfo(
		sl::Array<char>* buffer,
		const HidReport& report
	);

	static
	size_t
	loadReportDecodeInfo(
		HidStandaloneReport* report,
		const HidDb* db,
		const void* p,
		size_t size
	);
};

//..............................................................................

} // namespace io
} // namespace axl
