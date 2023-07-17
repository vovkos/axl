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

#define _AXL_SL_BOYERMOOREFIND_H

#include "axl_sl_BoyerMooreDetails.h"
#include "axl_sl_BoyerMooreState.h"
#include "axl_sl_String.h"

namespace axl {
namespace sl {

//
// Boyer-Moore & Boyer-Moore-Horspool adaptation with added support for:
//
//   - reverse matching
//   - incremental (stream) matching
//   - misc text encodings
//   - case-folded matching
//   - whole-word matching
//

//..............................................................................

template <typename Details0>
class BoyerMooreFindBase {
public:
	typedef Details0 Details;
	typedef typename Details::C C;
	typedef typename Details::SkipTables SkipTables;

	enum {
		IsReverse = Details::IsReverse
	};

protected:
	Array<C> m_pattern;
	SkipTables m_skipTables;

public:
	BoyerMooreFindBase() {}

	BoyerMooreFindBase(
		const C* p,
		size_t length
	) {
		setPattern(p, length);
	}

	bool
	isEmpty() const {
		return m_pattern.isEmpty();
	}

	const Array<C>&
	getPattern() const {
		return m_pattern;
	}

	void
	clear() {
		m_pattern.clear();
		m_skipTables.clear();
	}

	bool
	setPattern(
		const C* p,
		size_t length
	) {
		bool result = IsReverse ?
			m_pattern.copyReverse(p, length) :
			m_pattern.copy(p, length);

		return m_skipTables.build(m_pattern, length, Details::BadSkipTableSize);
	}

	size_t
	find(
		const C* p,
		size_t length
	) const {
		size_t i = findImpl(sl::PtrIterator<const C, IsReverse>(IsReverse ? p + length - 1 : p), length);
		return i < length ? i : -1;
	}

	uint64_t
	find(
		BoyerMooreStateBase<C>* state,
		const C* p,
		size_t length
	) const {
		BoyerMooreIncrementalAccessorBase<C, IsReverse> accessor(state, IsReverse ? p + length - 1 : p);
		size_t fullLength = state->getTailLength() + length;
		size_t i = findImpl(accessor, fullLength);
		if (i + m_pattern.getCount() <= fullLength) {
			uint64_t offset = state->getOffset() + i;
			state->reset(offset + m_pattern.getCount());
			return offset;
		}

		state->template advance<IsReverse>(i, p, length);
		return -1;
	}

protected:
	template <typename Accessor>
	size_t
	findImpl(
		const Accessor& accessor,
		size_t length
	) const {
		size_t last = m_pattern.getCount() - 1;
		size_t i = last;
		while (i < length) {
			intptr_t j = last;
			uchar_t c;
			for (;;) {
				c = accessor[i];
				if (c != m_pattern[j])
					break;

				if (j == 0)
					return i;

				i--;
				j--;
			}

			i += m_skipTables.getSkip(c, j);
		}

		return i - last; // prospective start of match
	}
};

//..............................................................................

struct BoyerMooreTextFindResult {
	size_t m_charOffset;
	size_t m_binOffset;
	size_t m_binEndOffset; // not necessarily the size needed to "properly" encode pattern!

	BoyerMooreTextFindResult() {
		m_charOffset = -1;
		m_binOffset = -1;
		m_binEndOffset = -1;
	}

	bool
	isValid() const {
		return m_charOffset != -1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Details>
class BoyerMooreTextFindBase: public BoyerMooreFindBase<Details> {
public:
	typedef typename Details::Encoding Encoding;
	typedef typename Details::Decoder Decoder;
	typedef typename Details::CaseOp CaseOp;

	typedef enc::Convert<
		enc::Utf32,
		Encoding,
		CaseOp,
		Decoder
	> Convert;

	typedef enc::Locate<Encoding, Decoder> Locate;

	enum {
		IsCaseFolded = Details::IsCaseFolded,
		IsWholeWord  = Details::IsWholeWord,
		IsReverse    = Details::IsReverse,
	};

public:
	BoyerMooreTextFindBase() {}

	BoyerMooreTextFindBase(const sl::StringRef& pattern) {
		setPattern(pattern);
	}

	BoyerMooreTextFindBase(const sl::StringRef_utf32& pattern) {
		setPattern(pattern);
	}

	bool
	setPattern(const sl::StringRef& pattern) {
		char buffer[256];
		sl::String_utf32 pattern_utf32(rc::BufKind_Stack, buffer, sizeof(buffer));
		return
			pattern_utf32.copy(pattern) != -1 &&
			setPattern(pattern_utf32);
	}

	bool
	setPattern(const sl::StringRef_utf32& pattern) {
		size_t length = pattern.getLength();
		bool result = IsReverse ?
			this->m_pattern.copyReverse(pattern.cp(), length) :
			this->m_pattern.copy(pattern.cp(), length);

		if (!result)
			return false;

		if (IsCaseFolded)
			for (size_t i = 0; i < length; i++)
				this->m_pattern[i] = CaseOp()(pattern[i]);

		this->m_skipTables.build(this->m_pattern, length, Details::BadSkipTableSize);
		return true;
	}

	BoyerMooreTextFindResult
	find(
		const void* p,
		size_t size
	) const {
		BoyerMooreTextState state(*this);
		BoyerMooreTextFindResult result = find(&state, p, size);
		if (IsWholeWord && !result.isValid())
			result = eof(&state);
		return result;
	}

	BoyerMooreTextFindResult
	eof(BoyerMooreTextState* state) const {
		ASSERT(IsWholeWord);

		static utf32_t suffix = ' ';
		BoyerMooreTextAccessor accessor(state, &suffix);
		size_t fullLength = state->getTailLength() + 1;
		size_t i = findWholeWordImpl(accessor, fullLength);
		return i + m_pattern.getCount() < fullLength ? // account for suffix
			createFindResult(state, i, NULL, NULL) :
			BoyerMooreTextFindResult();
	}

	BoyerMooreTextFindResult
	find(
		BoyerMooreTextState* state,
		const void* p0,
		size_t size
	) const {
		sl::PtrIterator<const char, IsReverse> p(IsReverse ? (char*)p0 + size - 1 : (char*)p0);
		const char* end = p + size;

		enc::DecoderState decoderState = state->getDecoderState();

		while (p < end) {
			utf32_t buffer[4]; // [Details::DecoderBufferLength]
			typename Convert::Result convertResult = Convert::convert(
				&decoderState,
				buffer,
				buffer + countof(buffer),
				p,
				end
			);

			const char* p2 = convertResult.m_src; // just a short alias
			size_t srcLength = -(p - p2); // respect IsReverse
			size_t dstLength = convertResult.m_dst - buffer;
			size_t fullLength = state->getTailLength() + dstLength;
			size_t i;

			BoyerMooreTextAccessor accessor(state, buffer);

			if (IsWholeWord) {
				i = findWholeWordImpl(accessor, fullLength);
				if (i + m_pattern.getCount() < fullLength) // account for suffix
					return createFindResult(state, i, p, p2);

			} else {
				i = this->findImpl(accessor, fullLength);
				if (i + m_pattern.getCount() <= fullLength)
					return createFindResult(state, i, p, p2);
			}

			size_t binOffset = locateBinOffset(state, i, p, p2); // locate bin-offset of the prospective start of match

			state->advance<IsReverse>(
				i,
				buffer,
				dstLength,
				binOffset,
				p,
				srcLength,
				decoderState
			);

			p = p2;
		}

		return BoyerMooreTextFindResult();
	}

protected:
	size_t
	findWholeWordImpl(
		BoyerMooreTextAccessor& accessor,
		size_t length
	) const {
		size_t patternLength = this->m_pattern.getCount();
		if (!length)
			return 0;

		size_t end = length - 1;
		size_t i = findImpl(accessor, end);
		if (i) // update prefix
			accessor.m_state->m_prefix = accessor[i - 1];

		if (i + patternLength > end)
			return i;

		utf32_t suffix = accessor[i + patternLength];
		if (enc::isWord(accessor.m_state->m_prefix) || enc::isWord(suffix)) { // not a whole word
			size_t last = patternLength - 1; // to calculate skip, use the last character of the pattern
			utf32_t c = accessor[i + last];
			i += this->m_skipTables.getSkip(c, last);
		}

		return i;
	}

	BoyerMooreTextFindResult
	createFindResult(
		BoyerMooreTextState* state,
		size_t i,
		const char* p,
		const char* end
	) const {
		BoyerMooreTextFindResult result;
		result.m_charOffset = state->getOffset() + i;
		result.m_binOffset = state->getBinOffset() + locateBinOffset(state, i, p, end);
		result.m_binEndOffset = state->getBinOffset() + locateBinOffset(state, i + this->m_pattern.getCount(), p, end);
		state->reset(result.m_charOffset, result.m_binOffset);
		return result;
	}

	static
	size_t
	locateBinOffset(
		const BoyerMooreTextState* state,
		size_t i,
		const char* p,
		const char* end
	) {
		enc::ConvertLengthResult result;
		enc::DecoderState decoderState;
		size_t charTailLength = state->m_tail.getDataLength();
		size_t binTailSize = state->m_binTail.getDataSize();

		if (i > charTailLength)
			decoderState = state->getDecoderState(); // well past tail; can skip tail search
		else { // search tail first
			const char* front = state->m_binTail.getFront();
			const char* back = state->m_binTail.getBack();
			decoderState = 0;

			if (front < back) // one continous chunk
				result = IsReverse ?
					Locate::locate(&decoderState, i, back - 1, front - 1) :
					Locate::locate(&decoderState, i, front, back);
			else { // two disjoint chunks
				p = state->m_binTail.getBuffer();
				end = state->m_binTail.getBufferEnd();
				enc::DecoderState decoderState = 0;

				if (IsReverse) {
					result = Locate::locate(&decoderState, i, back - 1, p - 1); // back first
					if (result.m_dstLength < i) // not found; search front now
						result = Locate::locate(&decoderState, i, end - 1, front - 1);
				} else {
					result = Locate::locate(&decoderState, i, front, end); // front first
					if (result.m_dstLength < i) // not found; search back now
						result = Locate::locate(&decoderState, i, p, back);
				}
			}

			if (result.m_dstLength >= i) // found in tail
				return result.m_srcLength;

			// otherwise, move on to the latest data
		}

		// search the latest data

		return binTailSize + Locate::locate(&decoderState, i - charTailLength, p, end).m_srcLength;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreFindBase<BoyerMooreHorspoolBinDetails>        BoyerMooreHorspoolBinFind;
typedef BoyerMooreFindBase<BoyerMooreHorspoolReverseBinDetails> BoyerMooreHorspoolReverseBinFind;
typedef BoyerMooreFindBase<BoyerMooreBinDetails>                BoyerMooreBinFind;
typedef BoyerMooreFindBase<BoyerMooreReverseBinDetails>         BoyerMooreReverseBinFind;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::Details>                    BoyerMooreTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::ReverseDetails>             BoyerMooreReverseTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::WholeWordDetails>           BoyerMooreWholeWordTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::WholeWordReverseDetails>    BoyerMooreWholeWordReverseTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::CaseFoldedDetails>          BoyerMooreCaseFoldedTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::CaseFoldedReverseDetails>   BoyerMooreCaseFoldedReverseTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::CaseFoldedWholeWordDetails> BoyerMooreCaseFoldedWholeWordTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::CaseFoldedWholeWordReverseDetails> BoyerMooreCaseFoldedWholeWordReverseTextFind_utf8;

typedef BoyerMooreTextFind_utf8                    BoyerMooreTextFind;
typedef BoyerMooreReverseTextFind_utf8             BoyerMooreReverseTextFind;
typedef BoyerMooreWholeWordTextFind_utf8           BoyerMooreWholeWordTextFind;
typedef BoyerMooreWholeWordReverseTextFind_utf8    BoyerMooreWholeWordReverseTextFind;
typedef BoyerMooreCaseFoldedTextFind_utf8          BoyerMooreCaseFoldedTextFind;
typedef BoyerMooreCaseFoldedReverseTextFind_utf8   BoyerMooreCaseFoldedReverseTextFind;
typedef BoyerMooreCaseFoldedWholeWordTextFind_utf8 BoyerMooreCaseFoldedWholeWordTextFind;
typedef BoyerMooreCaseFoldedWholeWordReverseTextFind_utf8 BoyerMooreCaseFoldedWholeWordReverseTextFind;

//..............................................................................

} // namespace sl
} // namespace axl
