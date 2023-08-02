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
	sl::CircularBufferBase<C> m_tail;
	uint64_t m_offset; // for reverse find, it's the (positive) offset from the end

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
			size_t result = m_tail.write(p + i, copyLength);
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

class BoyerMooreTextState: public BoyerMooreStateBase<utf32_t> {
protected:
	sl::CircularBuffer m_binTail;
	uint64_t m_binOffset;
	enc::DecoderState m_decoderState;

public:
	utf32_t m_prefix; // freely adjustible

public:
	BoyerMooreTextState() {
		m_binOffset = 0;
		m_prefix = 0;
	}

	BoyerMooreTextState(const sl::StringRef& pattern) {
		create(pattern, 0, 0);
	}

	BoyerMooreTextState(const sl::StringRef_utf32& pattern) {
		create(pattern, 0, 0);
	}

	BoyerMooreTextState(
		const sl::StringRef& pattern,
		uint64_t charOffset,
		uint64_t binOffset
	) {
		create(pattern, charOffset, binOffset);
	}

	BoyerMooreTextState(
		const sl::StringRef_utf32& pattern,
		uint64_t charOffset,
		uint64_t binOffset
	) {
		create(pattern, charOffset, binOffset);
	}

	uint64_t
	getBinOffset() const {
		return m_binOffset;
	}

	size_t
	getBinTailSize() const {
		return m_binTail.getDataSize();
	}

	const char*
	getBinTailFront() const {
		return m_binTail.getFront();
	}

	const char*
	getBinTailBack() const {
		return m_binTail.getBack();
	}

	const char*
	getBinTailBuffer() const {
		return m_binTail.getBuffer();
	}

	const char*
	getBinTailBufferEnd() const {
		return m_binTail.getBufferEnd();
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
		uint64_t charOffset,
		uint64_t binOffset
	) {
		size_t patternLength = enc::Convert<enc::Utf32, enc::Utf8>::calcRequiredLength(pattern.cp(), pattern.getEnd());
		return create(pattern.getLength(), charOffset, binOffset);
	}

	bool
	create(
		const sl::StringRef_utf32& pattern,
		uint64_t charOffset,
		uint64_t binOffset
	) {
		return create(pattern.getLength(), charOffset, binOffset);
	}

	void
	reset() {
		reset(0, 0);
	}

	void
	reset(
		uint64_t charOffset,
		uint64_t binOffset
	) {
		BoyerMooreStateBase<utf32_t>::reset(charOffset);
		m_binTail.clear();
		m_binOffset = binOffset;
		m_decoderState = 0;
		m_prefix = 0;
	}

	template <bool IsReverse>
	void
	advance(
		size_t charOffset,
		const utf32_t* text,
		size_t textLength,
		size_t binOffset,
		const void* bin,
		size_t binSize,
		enc::DecoderState decoderState
	) {
		ASSERT(binOffset >= charOffset);

		m_offset += charOffset;
		m_binOffset += binOffset;
		m_decoderState = decoderState;

		size_t charTailLength = m_tail.getDataLength();
		size_t binTailSize = m_binTail.getDataSize();

		if (binOffset >= binTailSize) {
			ASSERT(charOffset >= charTailLength);

			charOffset -= charTailLength;
			size_t textCopyLength = textLength - charOffset;
			m_tail.clear();
			size_t result = m_tail.write(text + charOffset, textCopyLength);
			ASSERT(result == textCopyLength);

			binOffset -= binTailSize;
			size_t binCopySize = binSize - binOffset;
			m_binTail.clear();
			result = m_binTail.write((char*)bin + binOffset, binCopySize);
			ASSERT(result == binCopySize);
		} else {
			m_tail.drop(charOffset);
			size_t result = m_tail.write(text, textLength); // text is already reversed
			ASSERT(result == textLength);

			if (IsReverse) {
				m_binTail.dropBack(binOffset);
				result = m_binTail.writeFront((char*)bin, binSize);
				ASSERT(result == binSize);
			} else {
				m_binTail.dropFront(binOffset);
				result = m_binTail.writeBack((char*)bin, binSize);
				ASSERT(result == binSize);
			}
		}

		ASSERT(m_binTail.getDataSize() >= m_tail.getDataLength());
	}

protected:
	bool
	create(
		size_t patternLength,
		uint64_t charOffset,
		uint64_t binOffset
	) {
		m_binOffset = binOffset;
		m_decoderState = 0;
		m_prefix = 0;

		return
			BoyerMooreStateBase<utf32_t>::create(patternLength, charOffset) &&
			m_binTail.create(patternLength * 4); // a codepoint takes up to 4 bytes
	}
};

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

class BoyerMooreTextAccessor: public BoyerMooreIncrementalAccessorBase<
	utf32_t,
	false,
	BoyerMooreTextState
> {
public:
	BoyerMooreTextAccessor(
		BoyerMooreTextState* state,
		const utf32_t* p
	):	BoyerMooreIncrementalAccessorBase<
			utf32_t,
			false,
			BoyerMooreTextState
		>(state, p) {}
};

//..............................................................................

} // namespace sl
} // namespace axl
