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

#define _AXL_RE_REGEXSTORAGE_H

#include "axl_re_Pch.h"

namespace axl {
namespace re {

//..............................................................................

enum {
	StorageSignature = '.AFN',
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
	uint32_t m_captureCount;
	uint32_t m_stateCount;
	uint32_t m_switchCaseCount;
	uint32_t m_matchStartStateId;
	uint32_t m_searchStartStateId;

	// followed by m_dataSize bytes:
	// SwitchCaseStorage[m_switchCaseCount]
	// NfaStateStorage[m_stateCount]
};

struct SwitchCaseStorage {
	uint32_t m_startCount;
	uint32_t m_captureCount;
	uint32_t m_matchStartStateId;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct NfaStateStorage {
	uint32_t m_stateKind;
	uint32_t m_nextStateId;

	union {
		uint32_t m_unionData[2];
		uint32_t m_captureId;
		uint32_t m_acceptId;
		utf32_t m_char;
		uint32_t m_anchor;
		uint32_t m_charRangeCount;
		uint32_t m_splitStateId;
		uint32_t m_tailStateId;
		struct {
			uint32_t m_opStateId;
			uint32_t m_reverseStateId;
		};
	};

	// followed by:
	// for NfaStateKind_MatchCharSet: NfaCharRangeStorage[m_charRangeCount]
};

struct NfaCharRangeStorage {
	utf32_t m_charFrom;
	utf32_t m_charTo; // including
};

//..............................................................................

} // namespace re
} // namespace axl
