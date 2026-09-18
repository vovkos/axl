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

#include "axl_cry_Pch.h"

#if (_AXL_OS_WIN)
#	include <wincrypt.h> // needs windows.h, hence after axl_cry_Pch.h
#	undef X509_NAME      // ...which re-defines the macro (see axl_cry_Pch.h)
#endif
