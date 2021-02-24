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

#include "axl_sys_CaPch.h"

#if (_AXL_OS_LINUX)
#	include "axl_io_MappedFile.h"
#	include "axl_mem_Block.h"
#	include <link.h>
#elif (_AXL_OS_DARWIN)
#	include "axl_sec_StaticCode.h"
#	include "axl_cf_Dictionary.h"
#endif
