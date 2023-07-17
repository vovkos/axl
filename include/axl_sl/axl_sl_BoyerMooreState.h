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
#include "axl_enc_Unicode.h"

namespace axl {
namespace sl {

//..............................................................................

// states for incremental search

template <typename C>
class BoyerMooreStateBase {
	template <
		typename C,
		bool IsReverse,
		typename State
	>
	friend class BoyerMooreIncrementalAccessorBase;

protected:
	sl::CircularBufferBase<C> m_tail;
	uint64_t m_offset; // for reverse find, it's the (positive) offset from the end

public:
	template <typename T>
	BoyerMooreStateBase(
		const T& find,
		uint64_t offset = 0
	) {
		create(find, offset);
	}

	uint64_t
	getOffset() const {
		return m_offset;
	}

	size_t
	getTailLength() const {
		return m_tail.getDataLength();
	}

	template <typename T>
	bool
	create(
		const T& find,
		uint64_t offset = 0
	) {
		m_offset = offset;
		return m_tail.create(find.getPattern().getCount());
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

protected:
	BoyerMooreStateBase() {}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreStateBase<char> BoyerMooreBinState;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class BoyerMooreTextState: public BoyerMooreStateBase<utf32_t> {
	template <typename Details>
	friend class BoyerMooreTextFindBase;

protected:
	sl::CircularBuffer m_binTail;
	uint64_t m_binOffset;
	enc::DecoderState m_decoderState;
	utf32_t m_prefix;

public:
	template <typename T>
	BoyerMooreTextState(const T& find) {
		create(find, 0, 0);
	}

	template <typename T>
	BoyerMooreTextState(
		const T& find,
		uint64_t charOffset,
		uint64_t binOffset
	) {
		create(find, charOffset, binOffset);
	}

	uint64_t
	getBinOffset() const {
		return m_binOffset;
	}

	size_t
	getBinTailSize() const {
		return m_binTail.getDataSize();
	}

	enc::DecoderState
	getDecoderState() const {
		return m_decoderState;
	}

	utf32_t
	getPrefix() const {
		return m_prefix;
	}

	void
	setPrefix(utf32_t c) {
		m_prefix = c;
	}

	template <typename T>
	bool
	create(const T& find) {
		return create(find, 0, 0);
	}

	template <typename T>
	bool
	create(
		const T& find,
		uint64_t charOffset,
		uint64_t binOffset
	) {
		m_binOffset = binOffset;
		m_decoderState = 0;
		m_prefix = 0;

		return
			BoyerMooreStateBase<utf32_t>::create(find, charOffset) &&
			m_binTail.create(find.getPattern().getCount() * 4); // a codepoint takes up to 4 bytes
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
		m_offset += charOffset;
		m_binOffset += binOffset;
		m_decoderState = decoderState;

		size_t charTailLength = m_tail.getDataLength();
		size_t binTailSize = m_binTail.getDataSize();

		if (charOffset >= charTailLength) {
			charOffset -= charTailLength;
			size_t textCopyLength = textLength - charOffset;
			m_tail.clear();
			size_t result = m_tail.write(text + charOffset, textCopyLength);
			ASSERT(result == textCopyLength);

			ASSERT(binOffset >= binTailSize);
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
		size_t tailLength = m_state->m_tail.getDataLength();
		return i < tailLength ?
			m_state->m_tail[IsReverse ? tailLength - i - 1 : i] :
			*PtrIterator<const C, IsReverse>::inc(m_p, i - tailLength);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class BoyerMooreTextAccessor: public BoyerMooreIncrementalAccessorBase<
	utf32_t,
	false,
	BoyerMooreTextState
> {
	template <typename Details>
	friend class BoyerMooreTextFindBase;

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
