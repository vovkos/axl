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

enum RegexStorageKind {
	RegexStorageKind_Nfa = '.AFN',
	RegexStorageKind_Dfa = '.AFD'
};

struct RegexStorageHdr {
	uint32_t m_storageKind;
	uint32_t m_dataSize;
	uint32_t m_regexKind;
	uint32_t m_stateCount;
	uint32_t m_switchCaseCount;
	uint32_t m_captureCount;

	// followed by m_dataSize bytes:
	// RegexSwitchCaseStorage[m_switchCaseCount]
	// for RegexStorageKind_Nfa: NfaStateStorage[m_stateCount]
	// for RegexStorageKind_Dfa: DfaStateStorage[m_stateCount]
};

struct RegexSwitchCaseStorage {
	uint32_t m_baseCaptureId;
	uint32_t m_captureCount;
	uint32_t m_startStateId;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct NfaStateStorage {
	uint32_t m_stateKind;

	union {
		uint32_t m_unionData;
		uint32_t m_captureId;
		uint32_t m_acceptId;
		utf32_t m_char;
		uint32_t m_anchor;
		uint32_t m_charRangeCount;
		uint32_t m_splitStateId;
	};

	uint32_t m_nextStateId;

	// followed by:
	// for NfaStateKind_MatchCharSet: CharRangeStorage[m_charRangeCount]
};

struct CharRangeStorage {
	utf32_t m_charFrom;
	utf32_t m_charTo; // including
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct DfaStateStorage {
	uint32_t m_flags;
	uint32_t m_anchorMask;
	uint32_t m_acceptId;
	uint32_t m_openCaptureCount;
	uint32_t m_closeCaptureCount;
	uint32_t m_charTransitionCount;

	// followed by:
	// uint32_t[m_openCaptureCount]
	// uint32_t[m_closeCaptureCount]
	// uint32_t[m_anchorMask + 1] (anchor transition map)
	// CharTransitionStorage[m_charTransitionCount]
};

struct CharTransitionStorage {
	utf32_t m_charFrom;
	utf32_t m_charTo; // including
	uint32_t m_stateId;
};

//..............................................................................

} // namespace re
} // namespace axl
