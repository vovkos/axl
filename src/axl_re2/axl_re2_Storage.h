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

#include "axl_re2_Pch.h"

namespace axl {
namespace re2 {

//..............................................................................

enum {
	StorageSignature    = ':2er',
	SwitchCaseSignature = ':sc\n',
};

enum StorageVersion {
	StorageVersion_1_0_0   = 0x010000,
	StorageVersion_Current = StorageVersion_1_0_0,
};

struct StorageHdr {
	uint32_t m_signature;
	uint32_t m_version;
	uint32_t m_dataSize;
	uint32_t m_regexKind;
	uint32_t m_flags;
	uint32_t m_switchCaseCount;
	uint32_t m_captureCount;

	// followed by one or m_switchCaseCount regexes
};

struct SwitchCaseHdr {
	uint32_t m_signature;
	uint32_t m_length;
	uint32_t m_captureCount;
};

//..............................................................................

} // namespace re2
} // namespace axl
