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

	void
	clear() {
		m_pattern.clear();
		m_skipTables.clear();
	}

	size_t
	find(
		const C* p,
		size_t length
	) const {
		size_t i = findImpl(sl::PtrIterator<const C, IsReverse>(IsReverse ? p + length - 1 : p), 0, length);
		return i < length ? i : -1;
	}

	uint64_t
	find(
		BoyerMooreStateBase<C>* state,
		const C* p,
		size_t length
	) const {
		ASSERT(state->getPatternLength() == m_pattern.getCount());

		BoyerMooreIncrementalAccessorBase<C, IsReverse> accessor(state, IsReverse ? p + length - 1 : p);
		size_t fullLength = state->getTailLength() + length;
		size_t i = findImpl(accessor, 0, fullLength);
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
		size_t i0,
		size_t length
	) const {
		size_t last = m_pattern.getCount() - 1;
		size_t i = i0 + last;
		while (i < length) {
			intptr_t j = last;
			C c;
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Details>
class BoyerMooreBinFindBase: public BoyerMooreFindBase<Details> {
public:
	BoyerMooreBinFindBase() {}

	BoyerMooreBinFindBase(
		const void* p,
		size_t size
	): BoyerMooreFindBase<Details>((char*)p, size) {}

	bool
	setPattern(
		const void* p,
		size_t size
	) {
		return BoyerMooreFindBase<Details>::setPattern((char*)p, size);
	}

	size_t
	find(
		const void* p,
		size_t size
	) const {
		return BoyerMooreFindBase<Details>::find((char*)p, size);
	}

	uint64_t
	find(
		BoyerMooreBinState* state,
		const void* p,
		size_t size
	) const {
		return BoyerMooreFindBase<Details>::find(state, (char*)p, size);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreBinFindBase<BoyerMooreHorspoolBinDetails>        BoyerMooreHorspoolBinFind;
typedef BoyerMooreBinFindBase<BoyerMooreHorspoolReverseBinDetails> BoyerMooreHorspoolReverseBinFind;
typedef BoyerMooreBinFindBase<BoyerMooreBinDetails>                BoyerMooreBinFind;
typedef BoyerMooreBinFindBase<BoyerMooreReverseBinDetails>         BoyerMooreReverseBinFind;

//..............................................................................

struct BoyerMooreTextFindResult {
	uint64_t m_charOffset;
	uint64_t m_binOffset;
	uint64_t m_binEndOffset;

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

#if (_AXL_DEBUG)
#	define _AXL_SL_BM_DEBUG_PRINT 0
#endif

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
				this->m_pattern[i] = CaseOp()(this->m_pattern[i]);

		this->m_skipTables.build(this->m_pattern, length, Details::BadSkipTableSize);
		return true;
	}

	BoyerMooreTextFindResult
	find(
		const void* p,
		size_t size,
		utf32_t replacementChar = enc::StdChar_Replacement
	) const {
		BoyerMooreTextState state(sl::StringRef_utf32(this->m_pattern, this->m_pattern.getCount()));
		BoyerMooreTextFindResult result = find(&state, p, size, replacementChar);
		if (IsWholeWord && !result.isValid())
			result = eof(&state);
		return result;
	}

	BoyerMooreTextFindResult
	find(
		BoyerMooreTextState* state,
		const void* p0,
		size_t size,
		utf32_t replacementChar = enc::StdChar_Replacement
	) const {
		ASSERT(state->getPatternLength() == this->m_pattern.getCount());

		sl::PtrIterator<const char, IsReverse> p(IsReverse ? (char*)p0 + size - 1 : (char*)p0);
		const char* end = p + size;

		enc::DecoderState decoderState = state->getDecoderState();

#if (_AXL_SL_BM_DEBUG_PRINT)
		sl::Array<utf32_t> tail;
		sl::Array<char> binTail;
#endif

		while (p < end) {
			utf32_t buffer[Details::DecoderBufferLength];

			typename Convert::Result convertResult = Convert::convert(
				&decoderState,
				buffer,
				buffer + countof(buffer),
				p,
				end,
				replacementChar
			);

			const char* p2 = convertResult.m_src; // just a short alias
			size_t srcLength = -(p - p2); // respect IsReverse
			size_t dstLength = convertResult.m_dst - buffer;
			size_t fullLength = state->getTailLength() + dstLength;
			size_t i;

#if (_AXL_SL_BM_DEBUG_PRINT)
			printf(
				"0x%02x, dstLength: %d srcLength: %d pending: %d\n",
				(uchar_t)*p,
				dstLength,
				srcLength,
				Decoder::getPendingLength(decoderState)
			);

			printf("  buf: ");

			for (size_t i = 0; i < dstLength; i++)
				printf("0x%x ", (uint32_t)buffer[i]);

			printf("\n");
#endif
			BoyerMooreTextAccessor accessor(state, buffer);

			if (IsWholeWord) {
				i = findWholeWordImpl(state, accessor, fullLength);
				if (i + this->m_pattern.getCount() < fullLength) // account for suffix
					return createFindResult(state, i, p, p2);

			} else {
				i = this->findImpl(accessor, 0, fullLength);
				if (i + this->m_pattern.getCount() <= fullLength)
					return createFindResult(state, i, p, p2);
			}

			// locate bin-offset of the prospective start of match
			size_t binOffset = locateBinOffset(state, i, p, p2);

			state->advance<IsReverse>(
				i,
				buffer,
				dstLength,
				binOffset,
				IsReverse ? p2 + 1 : (const char*)p,
				srcLength,
				decoderState
			);

#if (_AXL_SL_BM_DEBUG_PRINT)
			printf("  i: %d locate-bin: %d bin-tail: %d post-bin-tail: %d tail: %d\n",
				i,
				binOffset,
				state->getBinTailSize(),
				Decoder::getPendingLength(decoderState),
				state->getTailLength()
			);

			state->peekTail(&tail);
			state->peekBinTail(&binTail);

			printf("  chr-tail: ");
			for (size_t i = 0; i < tail.getCount(); i++)
				printf("0x%x ", (uint32_t)tail[i]);

			printf("\n  bin-tail: ");
			for (size_t i = 0; i < binTail.getCount(); i++)
				printf("0x%x ", (uchar_t)binTail[i]);

			printf("\n");
#endif

 			ASSERT(state->getBinTailSize() >= state->getTailLength());
			p = p2;
		}

		return BoyerMooreTextFindResult();
	}

	BoyerMooreTextFindResult
	eof(BoyerMooreTextState* state) const {
		ASSERT(IsWholeWord);

		static utf32_t suffix = ' ';
		BoyerMooreTextAccessor accessor(state, &suffix);
		size_t fullLength = state->getTailLength() + 1;
		size_t i = findWholeWordImpl(state, accessor, fullLength);
		return i + this->m_pattern.getCount() < fullLength ? // account for suffix
			createFindResult(state, i, NULL, NULL) :
			BoyerMooreTextFindResult();
	}

protected:
	size_t
	findWholeWordImpl(
		BoyerMooreTextState* state,
		const BoyerMooreTextAccessor& accessor,
		size_t length
	) const {
		if (!length)
			return 0;

		size_t patternLength = this->m_pattern.getCount();
		size_t i = 0;

		for (;;) {
			size_t end = length - 1;
			i = this->findImpl(accessor, i, end);
			if (i + patternLength > end)
				break;

			utf32_t prefix = i ? accessor[i - 1] : state->m_prefix;
			utf32_t suffix = accessor[i + patternLength];
			if (!enc::isWord(prefix) && !enc::isWord(suffix)) // a whole word, done!
				return i;

			size_t last = patternLength - 1; // to calculate skip, use the last character of the pattern
			utf32_t c = accessor[i + last];
			size_t skip = this->m_skipTables.getSkip(c, last);
			i += skip;
		}

		if (i) // update prefix
			state->m_prefix = accessor[i - 1];

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
		size_t tailLength = state->getTailLength();
		size_t binTailSize = state->getBinTailSize();

		if (i >= tailLength) { // beyond tail
			enc::DecoderState decoderState = state->getDecoderState();
			result = Locate::locate(&decoderState, i - tailLength, p, end);

			ASSERT((intptr_t)(binTailSize + result.m_srcLength) >= 0);
			return binTailSize + result.m_srcLength;
		}

		enc::DecoderState decoderState = 0;
		const char* front = state->getBinTailFront();
		const char* back = state->getBinTailBack();

		if (front < back) // one continous chunk
			result = IsReverse ?
				Locate::locate(&decoderState, i, back - 1, front - 1) :
				Locate::locate(&decoderState, i, front, back);
		else { // two disjoint chunks
			const char* buffer = state->getBinTailBuffer();
			const char* bufferEnd = state->getBinTailBufferEnd();

			if (IsReverse) {
				result = Locate::locate(&decoderState, i, back - 1, buffer - 1); // back first
				if (result.m_dstLength < i) { // not yet; search front now
					enc::ConvertLengthResult result2 = Locate::locate(&decoderState, i - result.m_dstLength, bufferEnd - 1, front - 1);
					result.m_dstLength += result2.m_dstLength;
					result.m_srcLength += result2.m_srcLength;
				}
			} else {
				result = Locate::locate(&decoderState, i, front, bufferEnd); // front first
				if (result.m_dstLength < i) { // not found; search back now
					enc::ConvertLengthResult result2 = Locate::locate(&decoderState, i - result.m_dstLength, buffer, back);
					result.m_dstLength += result2.m_dstLength;
					result.m_srcLength += result2.m_srcLength;
				}
			}
		}

		if (result.m_dstLength >= i)  { // found in tail
			ASSERT((intptr_t)result.m_srcLength >= 0);
			return result.m_srcLength;
		}

		// not yet, search the latest data
		ASSERT(result.m_srcLength == binTailSize);
		result = Locate::locate(&decoderState, i - tailLength, p, end);

		ASSERT((intptr_t)(binTailSize + result.m_srcLength) >= 0);
		return binTailSize + result.m_srcLength;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::Details>                    BoyerMooreTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::ReverseDetails>             BoyerMooreReverseTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::WholeWordDetails>           BoyerMooreWholeWordTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::WholeWordReverseDetails>    BoyerMooreWholeWordReverseTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::CaseFoldedDetails>          BoyerMooreCaseFoldedTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::CaseFoldedReverseDetails>   BoyerMooreCaseFoldedReverseTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::CaseFoldedWholeWordDetails> BoyerMooreCaseFoldedWholeWordTextFind_utf8;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_utf8::CaseFoldedWholeWordReverseDetails> BoyerMooreCaseFoldedWholeWordReverseTextFind_utf8;

typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_latin1::Details>                    BoyerMooreTextFind_latin1;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_latin1::ReverseDetails>             BoyerMooreReverseTextFind_latin1;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_latin1::WholeWordDetails>           BoyerMooreWholeWordTextFind_latin1;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_latin1::WholeWordReverseDetails>    BoyerMooreWholeWordReverseTextFind_latin1;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_latin1::CaseFoldedDetails>          BoyerMooreCaseFoldedTextFind_latin1;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_latin1::CaseFoldedReverseDetails>   BoyerMooreCaseFoldedReverseTextFind_latin1;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_latin1::CaseFoldedWholeWordDetails> BoyerMooreCaseFoldedWholeWordTextFind_latin1;
typedef BoyerMooreTextFindBase<BoyerMooreTextDetails_latin1::CaseFoldedWholeWordReverseDetails> BoyerMooreCaseFoldedWholeWordReverseTextFind_latin1;

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
