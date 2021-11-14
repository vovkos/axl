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

#define _AXL_RE_REGEXSTATE_H

#include "axl_re_Nfa.h"
#include "axl_re_Dfa.h"

namespace axl {
namespace re {

class Regex;
class RegexState;

//..............................................................................

enum RegexExecFlag {
	RegexExecFlag_Nfa             = 0x0001, // DFA by default
	RegexExecFlag_Stream          = 0x0002,
	RegexExecFlag_Multiline       = 0x0004,
	RegexExecFlag_NoCapture       = 0x0008, // capture by default
	RegexExecFlag_AnchorDataBegin = 0x0100,
	RegexExecFlag_AnchorDataEnd   = 0x0200,
	RegexExecFlag_ExactMatch      = RegexExecFlag_AnchorDataBegin | RegexExecFlag_AnchorDataEnd,
};

//..............................................................................

class RegexMatch {
	friend class RegexState;
	friend class RegexStateImpl;

protected:
	size_t m_offset;
	size_t m_endOffset;
	enc::CharCodec* m_charCodec;
	const char* m_p;
	mutable sl::StringRef m_text; // cache

public:
	RegexMatch();

	size_t
	getOffset() const {
		return m_offset;
	}

	size_t
	getEndOffset() const {
		return m_endOffset;
	}

	size_t
	getSize() const {
		return m_endOffset - m_offset;
	}

	const char*
	p() const {
		return m_p;
	}

	sl::StringRef
	getText() const  {
		ASSERT(m_p);
 		return !m_text.isEmpty() ? m_text : cacheText();
	}

protected:
	sl::StringRef
	cacheText() const;
};

//..............................................................................

class RegexStateImpl {
	friend class RegexState;

protected:
	enum {
		DecodeBufferSize = 64
	};

	enum CharFlag {
		CharFlag_AlphaNum = 0x01,
		CharFlag_NewLine  = 0x02,
	};

	struct Nfa {
		NfaStateSet m_consumingStateSetTable[2];
		NfaStateSet m_nonConsumingStateSetTable[2];
		size_t m_consumingStateSetIdx;
		size_t m_nonConsumingStateSetIdx;
		size_t m_lastAcceptStateId;
		bool m_isEmpty;

		Nfa();

		void
		reset(RegexStateImpl* parent);

		bool
		exec(
			RegexStateImpl* parent,
			const void* p,
			size_t size
		);

		bool
		eof(RegexStateImpl* parent);

		void
		addState(const NfaState* state);

		void
		advanceNonConsumingStates(
			RegexStateImpl* parent,
			uint32_t anchors
		);

		void
		advanceConsumingStates(
			RegexStateImpl* parent,
			utf32_t c
		);
	};

	struct Dfa {
		const DfaState* m_state;

		Dfa() {
			m_state = NULL;
		}

		void
		reset(RegexStateImpl* parent);

		bool
		exec(
			RegexStateImpl* parent,
			const void* p,
			size_t size
		);

		bool
		eof(RegexStateImpl* parent);

		void
		gotoState(
			RegexStateImpl* parent,
			const DfaState* state
		);
	};

	struct CapturePos {
		size_t m_offset;
		size_t m_endOffset;

		CapturePos() {
			m_offset = -1;
			m_endOffset = -1;
		}
	};

protected:
	Regex* m_regex;
	enc::CodePointDecoder m_decoder;
	const void* m_lastExecBuffer;
	size_t m_lastExecOffset;
	size_t m_lastExecSize;
	uint_t m_execFlags;
	uint_t m_prevCharFlags;
	size_t m_offset;
	size_t m_matchAcceptId;
	RegexMatch m_match;
	sl::BoxList<RegexMatch> m_subMatchList;
	sl::Array<RegexMatch*> m_subMatchArray;
	sl::Array<CapturePos> m_capturePosArray;

	union {
		void* m_fsm;
		Nfa* m_nfa;
		Dfa* m_dfa;
	};

public: // only constructible by RegexState
	RegexStateImpl() {
		initialize();
	}

	RegexStateImpl(const RegexStateImpl& src) {
		initialize();
		copy(src);
	}

	~RegexStateImpl() {
		freeFsm();
	}

	RegexStateImpl&
	operator = (const RegexStateImpl& src) {
		copy(src);
		return *this;
	}

	void
	freeFsm();

	void
	initialize();

	void
	initialize(
		uint_t execFlags,
		enc::CharCodec* codec
	);

	void
	postInitialize(Regex* regex);

	void
	reset(size_t offset = 0);

	void
	copy(const RegexStateImpl& src);

	uint32_t
	calcAnchors(utf32_t c);

	void
	openCapture(size_t captureId);

	void
	closeCapture(size_t captureId);

	void
	accept(size_t switchCaseIdx);

	bool
	finalize(bool isEof);

	bool
	exec(
		const void* p,
		size_t size
	);

protected:
	void
	createSubMatches(
		size_t baseCaptureId,
		size_t captureCount
	);
};

//..............................................................................

class RegexState {
	friend class Regex;
	friend class RegexMatch;

protected:
	rc::Buf<RegexStateImpl> m_p;

public:
	RegexState() {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	RegexState(RegexState&& src) {
		m_p.move(std::move(src.m_p));
	}
#endif

	RegexState(const RegexState& src) {
		m_p.copy(src.m_p);
	}

	RegexState(
		uint32_t flags,
		enc::CharCodec* codec
	) {
		initialize(flags, codec);
	}

	RegexState(
		uint32_t flags,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		initialize(flags, codecKind);
	}

	operator bool () const {
		return m_p;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	RegexState&
	operator = (RegexState&& src) {
		m_p.move(std::move(src.m_p));
		return *this;
	}
#endif

	RegexState&
	operator = (const RegexState& src) {
		m_p.copy(src.m_p);
		return *this;
	}

	const Regex*
	getRegex() const {
		ASSERT(m_p);
		return m_p->m_regex;
	}

	enc::CharCodec*
	getCodec() const {
		ASSERT(m_p);
		return m_p->m_decoder.getCharCodec();
	}

	uint_t
	getExecFlags() const {
		ASSERT(m_p);
		return m_p->m_execFlags;
	}

	bool
	isMatch() const {
		ASSERT(m_p);
		return m_p->m_matchAcceptId != -1;
	}

	bool
	getPreMatchOffset() const {
		ASSERT(m_p);
		return m_p->m_match.m_offset;
	}

	size_t
	getMatchSwitchCaseId() const {
		ASSERT(m_p);
		return m_p->m_matchAcceptId;
	}

	const RegexMatch*
	getMatch() const {
		ASSERT(m_p);
		return isMatch() ? &m_p->m_match : NULL;
	}

	size_t
	getSubMatchCount() const {
		ASSERT(m_p);
		return m_p->m_subMatchArray.getCount();
	}

	const RegexMatch*
	getSubMatch(size_t i) const {
		ASSERT(m_p);
		return i < m_p->m_subMatchArray.getCount() ? m_p->m_subMatchArray[i] : NULL;
	}

	void
	initialize(
		uint_t execFlags,
		enc::CharCodec* codec
	);

	void
	initialize(
		uint_t execFlags = 0,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		initialize(execFlags, enc::getCharCodec(codecKind));
	}

	void
	reset(size_t offset = 0) {
		ASSERT(m_p && m_p.isExclusive());
		m_p->reset();
	}

protected:
	void
	initialize(Regex* regex);

	void
	postInitialize(Regex* regex) {
		ASSERT(m_p && m_p.isExclusive());
		return m_p->postInitialize(regex);
	}

	bool
	exec(
		const void* p,
		size_t size
	) {
		ASSERT(m_p && m_p.isExclusive());
		return m_p->exec(p, size);
	}

	bool
	eof() {
		ASSERT(m_p && m_p.isExclusive());
		return (m_p->m_execFlags & RegexExecFlag_Nfa) ?
			m_p->m_nfa->eof(m_p) :
			m_p->m_dfa->eof(m_p);
	}
};

//..............................................................................

} // namespace re
} // namespace axl
