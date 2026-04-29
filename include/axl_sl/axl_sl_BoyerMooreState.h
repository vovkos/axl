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

#include "axl_sl_CircularBuffer.h"
#include "axl_sl_String.h"

namespace axl {
namespace sl {

//..............................................................................

// states for incremental search

template <typename C>
class BoyerMooreStateBase {
protected:
	uint64_t m_offset; // for reverse find, it's the (positive) offset from the end
	sl::CircularBufferBase<C> m_tail;

public:
	BoyerMooreStateBase() {
		m_offset = 0;
	}

	BoyerMooreStateBase(
		size_t patternLength,
		uint64_t offset = 0
	) {
		create(patternLength, offset);
	}

	uint64_t
	getOffset() const {
		return m_offset;
	}

	size_t
	getPatternLength() const {
		return m_tail.getBufferLength();
	}

	size_t
	getTailLength() const {
		return m_tail.getDataLength();
	}

	size_t
	peekTail(sl::Array<C>* buffer) {
		return m_tail.peek(buffer);
	}

	C
	getTailChar(size_t i) const {
		return m_tail[i];
	}

	bool
	create(
		size_t patternLength,
		uint64_t offset = 0
	) {
		m_offset = offset;
		return m_tail.create(patternLength);
	}

	void
	reset(uint64_t offset = 0) {
		m_offset = offset;
		m_tail.clear();
	}

	template <bool IsReverse>
	void
	advance(
		size_t i,
		const C* p,
		size_t length
	) {
		m_offset += i;

		size_t tailLength = m_tail.getDataLength();
		if (i >= tailLength) {
			i -= tailLength;
			size_t copyLength = length - i;
			m_tail.clear();
			size_t result = m_tail.write(
				IsReverse ?
					p + length - copyLength - i :
					p + i,
				copyLength
			);
			ASSERT(result == copyLength);
		} else if (IsReverse) {
			m_tail.dropBack(i);
			size_t result = m_tail.writeFront(p, length);
			ASSERT(result == length);
		} else {
			m_tail.dropFront(i);
			size_t result = m_tail.writeBack(p, length);
			ASSERT(result == length);
		}
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreStateBase<char> BoyerMooreBinState;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename CU>
class BoyerMooreTextStateBase: public BoyerMooreStateBase<utf32_t> {
protected:
	uint64_t m_cuOffset;
	sl::CircularBufferBase<CU> m_cuTail; // decoder state before cu tail is always 0
	enc::DecoderState m_decoderState;

public:
	utf32_t m_prefix; // freely adjustible

public:
	BoyerMooreTextStateBase() {
		m_cuOffset = 0;
		m_decoderState = 0;
		m_prefix = 0;
	}

	BoyerMooreTextStateBase(const sl::StringRef& pattern) {
		create(pattern, 0, 0);
	}

	BoyerMooreTextStateBase(const sl::StringRef_utf32& pattern) {
		create(pattern, 0, 0);
	}

	BoyerMooreTextStateBase(
		const sl::StringRef& pattern,
		uint64_t cpOffset,
		uint64_t cuOffset
	) {
		create(pattern, cpOffset, cuOffset);
	}

	BoyerMooreTextStateBase(
		const sl::StringRef_utf32& pattern,
		uint64_t cpOffset,
		uint64_t cuOffset
	) {
		create(pattern, cpOffset, cuOffset);
	}

	uint64_t
	getCuOffset() const {
		return m_cuOffset;
	}

	size_t
	getCuTailLength() const {
		return m_cuTail.getDataLength();
	}

	const CU*
	getCuTailFront() const {
		return m_cuTail.getFront();
	}

	const CU*
	getCuTailBack() const {
		return m_cuTail.getBack();
	}

	const CU*
	getCuTailBuffer() const {
		return m_cuTail.getBuffer();
	}

	const CU*
	getCuTailBufferEnd() const {
		return m_cuTail.getBufferEnd();
	}

	enc::DecoderState
	getDecoderState() const {
		return m_decoderState;
	}

	bool
	create(const sl::StringRef& pattern) {
		return create(pattern, 0, 0);
	}

	bool
	create(const sl::StringRef_utf32& pattern) {
		return create(pattern, 0, 0);
	}

	bool
	create(
		const sl::StringRef& pattern,
		uint64_t cpOffset,
		uint64_t cuOffset
	) {
		size_t patternLength = enc::Convert<enc::Utf32, enc::Utf8>::calcRequiredLength(pattern.cp(), pattern.getEnd());
		return create(pattern.getLength(), cpOffset, cuOffset);
	}

	bool
	create(
		const sl::StringRef_utf32& pattern,
		uint64_t cpOffset,
		uint64_t cuOffset
	) {
		return create(pattern.getLength(), cpOffset, cuOffset);
	}

	void
	reset() {
		reset(0, 0);
	}

	void
	reset(
		uint64_t cpOffset,
		uint64_t cuOffset
	) {
		BoyerMooreStateBase<utf32_t>::reset(cpOffset);
		m_cuTail.clear();
		m_cuOffset = cuOffset;
		m_decoderState = 0;
		m_prefix = 0;
	}

	template <bool IsReverse>
	void
	advance(
		size_t cpOffset,
		const utf32_t* text,
		size_t textLength,
		size_t cuOffset,
		const CU* cu,
		size_t cuLength,
		enc::DecoderState decoderState
	) {
		m_offset += cpOffset;
		m_cuOffset += cuOffset;
		m_decoderState = decoderState;

		// important -- process cp tail and cu tail independently

		size_t cpTailLength = m_tail.getDataLength();
		if (cpOffset >= cpTailLength) {
			cpOffset -= cpTailLength;
			size_t textCopyLength = textLength - cpOffset;
			m_tail.clear();
			size_t result = m_tail.write(text + cpOffset, textCopyLength);
			ASSERT(result == textCopyLength);
		} else {
			m_tail.drop(cpOffset);
			size_t result = m_tail.write(text, textLength); // text is already reversed
			ASSERT(result == textLength);
		}

		size_t cuTailLength = m_cuTail.getDataLength();
		if (cuOffset >= cuTailLength) {
			cuOffset -= cuTailLength;
			size_t cuCopyLength = cuLength - cuOffset;
			m_cuTail.clear();
			size_t result = m_cuTail.write(
				IsReverse ?
					cu + cuLength - cuCopyLength - cuOffset :
					cu + cuOffset,
				cuCopyLength
			);
			ASSERT(result == cuCopyLength);
		} else if (IsReverse) {
			m_cuTail.dropBack(cuOffset);
			size_t result = m_cuTail.writeFront(cu, cuLength);
			ASSERT(result == cuLength);
		} else {
			m_cuTail.dropFront(cuOffset);
			size_t result = m_cuTail.writeBack(cu, cuLength);
			ASSERT(result == cuLength);
		}
	}

protected:
	bool
	create(
		size_t patternLength,
		uint64_t cpOffset,
		uint64_t cuOffset
	) {
		m_cuOffset = cuOffset;
		m_decoderState = 0;
		m_prefix = 0;

		return
			BoyerMooreStateBase<utf32_t>::create(patternLength, cpOffset) &&
			m_cuTail.create(patternLength * sizeof(utf32_t) / sizeof(CU)); // a codepoint takes no more than 4 bytes
	}
};

//..............................................................................

typedef BoyerMooreTextStateBase<char> BoyerMooreTextState_latin1;
typedef BoyerMooreTextStateBase<utf8_t> BoyerMooreTextState_utf8;
typedef BoyerMooreTextStateBase<utf16_t> BoyerMooreTextState_utf16;
typedef BoyerMooreTextStateBase<utf32_t> BoyerMooreTextState_utf32;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreTextState_utf8 BoyerMooreTextState;

//..............................................................................

template <
	typename C,
	bool IsReverse,
	typename State = BoyerMooreStateBase<C>
>
class BoyerMooreIncrementalAccessorBase {
protected:
	State* m_state;
	const C* m_p;

public:
	BoyerMooreIncrementalAccessorBase(
		State* state,
		const C* p
	) {
		m_state = state;
		m_p = p;
	}

	C
	operator [] (size_t i) const {
		size_t tailLength = m_state->getTailLength();
		return i < tailLength ?
			m_state->getTailChar(IsReverse ? tailLength - i - 1 : i) :
			*PtrIterator<const C, IsReverse>::inc(m_p, i - tailLength);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename CU>
class BoyerMooreTextAccessorBase: public BoyerMooreIncrementalAccessorBase<
	utf32_t,
	false,
	BoyerMooreTextStateBase<CU>
> {
public:
	BoyerMooreTextAccessorBase(
		BoyerMooreTextStateBase<CU>* state,
		const utf32_t* p
	):	BoyerMooreIncrementalAccessorBase<
			utf32_t,
			false,
			BoyerMooreTextStateBase<CU>
		>(state, p) {}
};

//..............................................................................

} // namespace sl
} // namespace axl
