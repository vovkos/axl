// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_BMHFIND_H

#include "axl_rtl_Array.h"

namespace axl {
namespace rtl {

// Boyer-Moore-Horspool search (second table omitted)
// later it can be upgraded to a complete Boyer-Moore ;)

// the problem with upgrade is the need to carefully port non-trivial building of second table
// for ReverseFind and IncrementalFind... which i dont have neither time nor mood for

//.............................................................................

class CBmhFind
{
protected:	
	CArrayT <uchar_t> m_Buffer; // work size of buffer is 2 * m_PatternSize
	size_t m_PatternSize;
	size_t m_IncFindOffset;
	size_t m_IncFindTailSize; // IncrementalFind temporary storage size -- always < m_PatternSize
	size_t m_NextBadCharTable [256];
	size_t m_PrevBadCharTable [256];
	bool m_DoMatchCase;

public:
	CBmhFind ();

	void
	Clear ();

	bool 
	CanFind ()
	{
		return m_PatternSize != 0;
	}

	size_t 
	GetPatternSize ()
	{
		return m_PatternSize;
	}

	const void* 
	GetPattern ()
	{
		return m_Buffer;
	}

	bool
	SetPattern (
		const void* p, 
		size_t Size, 
		bool DoMatchCase = true
		);

	size_t 
	Find (
		const void* p, 
		size_t Size
		);

	size_t 
	ReverseFind (
		const void* p, 
		size_t Size
		);

	size_t 
	IncrementalFind (
		const void* p, 
		size_t Size
		);

	size_t 
	ReverseIncrementalFind (
		const void* p, 
		size_t Size
		);

	void ResetIncrementalFind (size_t IncFindOffset = 0)
	{
		m_IncFindOffset = IncFindOffset;
		m_IncFindTailSize = 0;
	}

protected:
	void 
	RebuildTables ();

	size_t
	CmpPattern (uchar_t* _p);

	size_t
	ReverseCmpPattern (uchar_t* _p);

	size_t
	IncrementalCmpPattern (	
		uchar_t* _p,
		size_t i
		);

	size_t
	ReverseIncrementalCmpPattern (	
		uchar_t* _p,
		size_t Size,
		size_t i
		);

	void
	UpdateIncrementalTail (	
		uchar_t* p,
		size_t Size,
		size_t TailSize
		);

	void
	UpdateReverseIncrementalTail (	
		uchar_t* p,
		size_t Size,
		size_t TailSize
		);
};

//.............................................................................

} // namespace rtl
} // namespace axl
