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

#define _AXL_SL_BMHFIND_H

#include "axl_sl_Array.h"

namespace axl {
namespace sl {

// Boyer-Moore-Horspool search (second table omitted)
// later it can be upgraded to a complete Boyer-Moore ;)

// the problem with upgrade is the need to carefully port non-trivial building of second table
// for ReverseFind and IncrementalFind... which i dont have neither time nor mood for

//..............................................................................

class BmhFind
{
protected:
	Array<uchar_t> m_buffer; // work size of buffer is 2 * m_PatternSize
	size_t m_patternSize;
	size_t m_incFindOffset;
	size_t m_incFindTailSize; // IncrementalFind temporary storage size -- always < m_PatternSize
	size_t m_nextBadCharTable[256];
	size_t m_prevBadCharTable[256];
	bool m_doMatchCase;

public:
	BmhFind();

	void
	clear();

	bool
	isEmpty()
	{
		return m_patternSize == 0;
	}

	size_t
	getPatternSize()
	{
		return m_patternSize;
	}

	const void*
	getPattern()
	{
		return m_buffer;
	}

	bool
	setPattern(
		const void* p,
		size_t size,
		bool doMatchCase = true
		);

	size_t
	find(
		const void* p,
		size_t size
		);

	size_t
	reverseFind(
		const void* p,
		size_t size
		);

	size_t
	incrementalFind(
		const void* p,
		size_t size
		);

	size_t
	reverseIncrementalFind(
		const void* p,
		size_t size
		);

	void
	resetIncrementalFind(size_t incFindOffset = 0)
	{
		m_incFindOffset = incFindOffset;
		m_incFindTailSize = 0;
	}

	size_t
	getIncrementalFindOffset()
	{
		return m_incFindOffset;
	}

	size_t
	getIncrementalFindTailSize()
	{
		return m_incFindTailSize;
	}

protected:
	void
	rebuildTables();

	size_t
	cmpPattern(uchar_t* _p);

	size_t
	reverseCmpPattern(uchar_t* _p);

	size_t
	incrementalCmpPattern(
		uchar_t* _p,
		size_t i
		);

	size_t
	reverseIncrementalCmpPattern(
		uchar_t* _p,
		size_t size,
		size_t i
		);

	void
	updateIncrementalTail(
		uchar_t* p,
		size_t size,
		size_t tailSize
		);

	void
	updateReverseIncrementalTail(
		uchar_t* p,
		size_t size,
		size_t tailSize
		);
};

//..............................................................................

} // namespace sl
} // namespace axl
