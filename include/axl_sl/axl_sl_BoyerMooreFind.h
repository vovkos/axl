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

#include "axl_sl_Array.h"
#include "axl_sl_String.h"
#include "axl_enc_CharCodec.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class BoyerMooreIncrementalContext
{
public:
	Array<T> m_tail;
	utf32_t m_prefix;

public:
	BoyerMooreIncrementalContext()
	{
		m_prefix = ' ';
	}

	void
	reset()
	{
		m_tail.clear();
		m_prefix = ' ';
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum BoyerMooreFlag
{
	BoyerMooreFlag_Reverse  = 0x01,
	BoyerMooreFlag_Horspool = 0x02, // don't use good-skip table
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class BoyerMooreFindBase
{
public:
	typedef BoyerMooreIncrementalContext<T> IncrementalContext;

protected:
	Array<T> m_pattern;
	Array<size_t, ArrayDetails<size_t> > m_badSkipTable;
	Array<size_t, ArrayDetails<size_t> > m_goodSkipTable;
	uint_t m_flags;

public:
	BoyerMooreFindBase()
	{
		m_flags = 0;
	}

	bool
	isEmpty()
	{
		return m_pattern.isEmpty();
	}

	Array<T>
	getPattern()
	{
		return m_pattern;
	}

	uint_t
	getFlags()
	{
		return m_flags;
	}

	void
	clear()
	{
		m_pattern.clear();
		m_badSkipTable.clear();
		m_goodSkipTable.clear();
		m_flags = 0;
	}

protected:
	// wikipedia implementation adaptation

	bool
	isPrefix(intptr_t pos)
	{
		intptr_t suffixSize = m_pattern.getCount() - pos;

		for (intptr_t i = 0, j = pos; i < suffixSize; i++, j++)
			if (m_pattern[i] != m_pattern[j])
				return false;

		return true;
	}

	size_t
	calcSuffixSize(intptr_t pos)
	{
		intptr_t i = 0;
		intptr_t j = pos;
		intptr_t k = m_pattern.getCount() - 1;

		while (i < pos && m_pattern[j] == m_pattern[k])
		{
			i++;
			j--;
			k--;
		}

		return i;
	}

	bool
	buildGoodSkipTable()
	{
		intptr_t patternSize = m_pattern.getCount();

		bool result = m_goodSkipTable.setCount(patternSize);
		if (!result)
			return false;

		if (!patternSize)
			return true;

		intptr_t lastPrefixPos = patternSize - 1;
		for (intptr_t i = patternSize - 1; i >= 0; i--)
		{
			if (isPrefix(i + 1))
				lastPrefixPos = i + 1;

			m_goodSkipTable[i] = lastPrefixPos + patternSize - 1 - i;
		}

		for (intptr_t i = 0; i < patternSize - 1; i++)
		{
			intptr_t suffixSize = calcSuffixSize(i);
			if (m_pattern[i - suffixSize] != m_pattern[patternSize - 1 - suffixSize])
				m_goodSkipTable[patternSize - 1 - suffixSize] = patternSize - 1 - i + suffixSize;
		}

		return true;
	}
};

//..............................................................................

class BinaryBoyerMooreFind: public BoyerMooreFindBase<uchar_t>
{
public:
	BinaryBoyerMooreFind()
	{
	}

	BinaryBoyerMooreFind(
		const void* p,
		size_t size,
		uint_t flags = 0
		)
	{
		setPattern(p, size, flags);
	}

	bool
	setPattern(
		const void* p,
		size_t size,
		uint_t flags = 0
		);

	size_t
	find(
		const void* p,
		size_t size
		);

	size_t
	find(
		IncrementalContext* incrementalContext,
		size_t offset,
		const void* p,
		size_t size
		);

protected:
	void
	buildBadSkipTable();

	template <typename Accessor>
	size_t
	findImpl(
		const Accessor& accessor,
		size_t size
		);
};

//..............................................................................

enum TextBoyerMooreFlag
{
	TextBoyerMooreFlag_CaseInsensitive = 0x10,
	TextBoyerMooreFlag_WholeWord       = 0x20,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class TextBoyerMooreFind: public BoyerMooreFindBase<utf32_t>
{
public:
	enum Def
	{
		Def_BadSkipTableSize = 256, // any number will do, actually (code points are hashed)
	};

protected:
	Array<utf32_t> m_buffer;

public:
	bool
	setPattern(
		size_t badSkipTableSize,
		enc::CharCodec* codec,
		const void* p,
		size_t size,
		uint_t flags = 0
		);

	bool
	setPattern(
		size_t badSkipTableSize,
		enc::CharCodecKind codecKind,
		const void* p,
		size_t size,
		uint_t flags = 0
		)
	{
		return setPattern(badSkipTableSize, enc::getCharCodec(codecKind), p, size, flags);
	}

	bool
	setPattern(
		enc::CharCodec* codec,
		const void* p,
		size_t size,
		uint_t flags = 0
		)
	{
		return setPattern(Def_BadSkipTableSize, codec, p, size, flags);
	}

	bool
	setPattern(
		enc::CharCodecKind codecKind,
		const void* p,
		size_t size,
		uint_t flags = 0
		)
	{
		return setPattern(Def_BadSkipTableSize, codecKind, p, size, flags);
	}

	bool
	setPattern(
		const sl::StringRef& pattern,
		uint_t flags = 0
		)
	{
		return setPattern(
			Def_BadSkipTableSize,
			enc::CharCodecKind_Utf8,
			pattern.cp(),
			pattern.getLength(),
			flags
			);
	}

	size_t
	find(
		enc::CharCodec* codec,
		const void* p,
		size_t size
		);

	size_t
	find(
		enc::CharCodecKind codecKind,
		const void* p,
		size_t size
		)
	{
		return find(enc::getCharCodec(codecKind), p, size);
	}

	size_t
	find(const sl::StringRef& string)
	{
		return find(enc::CharCodecKind_Utf8, string.cp(), string.getLength());
	}

	size_t
	find(
		IncrementalContext* incrementalContext,
		enc::CharCodec* codec,
		size_t offset,
		const void* p,
		size_t size
		);

	size_t
	find(
		IncrementalContext* incrementalContext,
		enc::CharCodecKind codecKind,
		size_t offset,
		const void* p,
		size_t size
		)
	{
		return find(incrementalContext, enc::getCharCodec(codecKind), offset, p, size);
	}

	size_t
	find(
		IncrementalContext* incrementalContext,
		size_t offset,
		const sl::StringRef& string
		)
	{
		return find(incrementalContext, enc::CharCodecKind_Utf8, offset, string.cp(), string.getLength());
	}

protected:
	bool
	buildBadSkipTable(size_t tableSize);

	template <typename Accessor>
	size_t
	findImpl(
		const Accessor& accessor,
		size_t end,
		size_t size
		);
};

//..............................................................................

} // namespace sl
} // namespace axl
