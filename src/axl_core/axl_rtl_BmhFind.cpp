#include "pch.h"
#include "axl_rtl_BmhFind.h"

namespace axl {
namespace rtl {

//.............................................................................

CBmhFind::CBmhFind ()
{
	m_PatternSize = 0;
	m_IncFindOffset = 0;
	m_IncFindTailSize = 0; // IncrementalFind temporary storage size -- always < m_PatternSize
	m_DoMatchCase = true;
}

void
CBmhFind::Clear ()
{
	m_PatternSize = 0;
	m_IncFindOffset = 0;
	m_IncFindTailSize = 0;
	m_DoMatchCase = true;
}

bool
CBmhFind::SetPattern (	
	const void* p,
	size_t Size,
	bool DoMatchCase
	)
{
	size_t NewBufferSize = Size * 2;
	
	NewBufferSize += 128 - NewBufferSize % 128; // 128-byte granularity

	Clear ();

	if (!Size)
		return true;

	m_Buffer.SetCount (Size * 2);

	m_PatternSize = Size;
	m_IncFindOffset = 0;
	m_IncFindTailSize = 0;
	m_DoMatchCase = DoMatchCase;

	memcpy (m_Buffer, p, Size);

	RebuildTables ();
	return true;
}

size_t 
CBmhFind::Find (
	const void* _p,
	size_t Size
	)
{
	uchar_t* p;
	uchar_t* pEnd;
	size_t m;

	if (!CanFind () || Size < m_PatternSize)
		return -1;

	p = (uchar_t*) _p;
	pEnd = p + Size - m_PatternSize;
	m = m_PatternSize - 1;

	do
	{
		uchar_t y = p [m];

		if (CmpPattern (p) == -1)
			return p - (uchar_t*) _p;

		p += m_NextBadCharTable [y];
	} while (p <= pEnd);

	return -1;
}

size_t
CBmhFind::ReverseFind (	
	const void* _p,
	size_t Size
	)
{
	uchar_t* p;

	if (!CanFind () || Size < m_PatternSize)
		return -1;

	p = (uchar_t*) _p + Size - m_PatternSize;

	do 
	{
		uchar_t y = *p;

		if (ReverseCmpPattern (p) == -1)
			return Size - (p - (uchar_t*) _p);

		p -= m_PrevBadCharTable [y];
	} while (p >= (uchar_t*) _p);

	return -1;
}

size_t
CBmhFind::IncrementalFind (	
	const void* _p,
	size_t Size
	)
{
	uchar_t* p = (uchar_t*) _p;
	size_t TestSize = Size + m_IncFindTailSize;

	size_t i;
	size_t iEnd;
	size_t n;

	if (!CanFind ())
		return -1;

	if (TestSize < m_PatternSize)
	{
		// incapsulate tail until BM/BMH is possible

		memcpy (m_Buffer + m_PatternSize + m_IncFindTailSize, _p, Size);
		m_IncFindTailSize += Size;
		return -1;
	}
	
	i = 0;
	iEnd = TestSize - m_PatternSize;
	n = m_PatternSize - m_IncFindTailSize - 1;

	do
	{
		uchar_t y = p [n + i];

		if (IncrementalCmpPattern (p, i) == -1)
		{
			size_t Result = m_IncFindOffset + i;
			m_IncFindOffset = 0;
			m_IncFindTailSize = 0;
			return Result;
		}

		i += m_NextBadCharTable [y];
	} while (i <= iEnd);

	if (i < TestSize)
		UpdateIncrementalTail (p, Size, TestSize - i);
	else 
		m_IncFindTailSize = 0;

	m_IncFindOffset += i;

	return -1;
}

size_t
CBmhFind::ReverseIncrementalFind (	
	const void* _p,
	size_t Size
	)
{
	uchar_t* p = (uchar_t*) _p;
	size_t TestSize = Size + m_IncFindTailSize;

	size_t i;

	if (!CanFind ())
		return -1;

	if (TestSize < m_PatternSize)
	{
		// incapsulate tail until BM/BMH is possible
		
		memcpy (m_Buffer + 2 * m_PatternSize - m_IncFindTailSize - Size, _p, Size);
		m_IncFindTailSize += Size;
		m_IncFindOffset += Size;
		return -1;
	}
	
	i = TestSize - m_PatternSize;

	for (;;)
	{
		uchar_t y = p [i];

		if (ReverseIncrementalCmpPattern (p, Size, i) == -1)
		{
			size_t Result = m_IncFindOffset + Size - i;
			m_IncFindOffset = 0;
			m_IncFindTailSize = 0;
			return Result;
		}

		if (i >= m_PrevBadCharTable [y])
		{
			i -= m_PrevBadCharTable [y];
			continue;
		}

		if (m_PrevBadCharTable [y] - i < m_PatternSize)
			UpdateReverseIncrementalTail (p, Size, m_PatternSize - (m_PrevBadCharTable [y] - i));
		else
			m_IncFindTailSize = 0;

		break;
	}

	m_IncFindOffset += Size;

	return -1;
}

void 
CBmhFind::RebuildTables ()
{
	size_t i;
	size_t m = m_PatternSize - 1;
	uchar_t* p = m_Buffer;
	
	for (i = 0; i < 256; i++)
	{
		m_NextBadCharTable [i] = 
		m_PrevBadCharTable [i] = m_PatternSize;
	}

	if (m_DoMatchCase)
	{
		for (i = 0; i < m; i++) 
			m_NextBadCharTable [p [i]] = m - i;

		for (i = m; i > 0; i--) 
			m_PrevBadCharTable [p [i]] = i;
	}
	else
	{
		for (i = 0; i < m; i++) 
		{
			m_NextBadCharTable [tolower (p [i])] = 
			m_NextBadCharTable [toupper (p [i])] = m - i;
		}

		for (i = m; i > 0; i--) 
		{
			m_PrevBadCharTable [tolower (p [i])] = 
			m_PrevBadCharTable [toupper (p [i])] = i;
		}
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the following 4 helpers return offset of a mismatch or -1 if argument matches the pattern

size_t
CBmhFind::CmpPattern (uchar_t* _p)
{
	uchar_t* p1 = m_Buffer + m_PatternSize - 1; 
	uchar_t* p2 = _p + m_PatternSize - 1; 

	if (m_DoMatchCase)
	{
		for (; p1 >= m_Buffer; p1--, p2--)
			if (*p1 != *p2)
				return p1 - m_Buffer;
	}
	else
	{
		for (; p1 >= m_Buffer; p1--, p2--)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_Buffer;
	}

	return -1;
}

size_t
CBmhFind::ReverseCmpPattern (uchar_t* _p)
{
	uchar_t* p1 = m_Buffer;
	uchar_t* p2 = _p;
	uchar_t* pEnd = p1 + m_PatternSize;

	if (m_DoMatchCase)
	{
		for (; p1 < pEnd; p1++, p2++)
			if (*p1 != *p2)
				return p1 - m_Buffer;
	}
	else
	{
		for (; p1 < pEnd; p1++, p2++)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_Buffer;
	}

	return -1;
}

size_t
CBmhFind::IncrementalCmpPattern (	
	uchar_t* _p,
	size_t i
	)
{
	uchar_t* p1;
	uchar_t* p2;

	if (i >= m_IncFindTailSize)
		return CmpPattern (_p + i - m_IncFindTailSize);

	p1 = m_Buffer + m_PatternSize - 1; 
	p2 = _p + i + m_PatternSize - m_IncFindTailSize - 1; 

	if (m_DoMatchCase)
	{
		for (; p2 >= _p; p1--, p2--)
			if (*p1 != *p2)
				return p1 - m_Buffer;

		p2 = m_Buffer + m_PatternSize + m_IncFindTailSize - 1;

		for (; p1 >= m_Buffer; p1--, p2--)
			if (*p1 != *p2)
				return p1 - m_Buffer;
	}
	else
	{
		for (; p2 >= _p; p1--, p2--)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_Buffer;

		p2 = m_Buffer + m_PatternSize + m_IncFindTailSize - 1;

		for (; p1 >= m_Buffer; p1--, p2--)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_Buffer;
	}

	return -1;
}

size_t
CBmhFind::ReverseIncrementalCmpPattern (	
	uchar_t* _p,
	size_t Size,
	size_t i
	)
{
	uchar_t* p1;
	uchar_t* pEnd1;

	uchar_t* p2;
	uchar_t* pEnd2;

	if (Size >= i + m_PatternSize)
		return ReverseCmpPattern (_p + i);

	p1 = m_Buffer;
	pEnd1 = p1 + m_PatternSize;
	
	p2 = _p + i;
	pEnd2 = _p + Size;

	if (m_DoMatchCase)
	{
		for (; p2 < pEnd2; p1++, p2++)
			if (*p1 != *p2)
				return p1 - m_Buffer;

		p2 = m_Buffer + 2 * m_PatternSize - m_IncFindTailSize;
	
		for (; p1 < pEnd1; p1++, p2++)
			if (*p1 != *p2)
				return p1 - m_Buffer;
	}
	else
	{
		for (; p2 < pEnd2; p1++, p2++)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_Buffer;

		p2 = m_Buffer + 2 * m_PatternSize - m_IncFindTailSize;

		for (; p1 < pEnd1; p1++, p2++)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_Buffer;
	}

	return -1;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the following 2 helpers are used to update the state of incremental-find tail

void
CBmhFind::UpdateIncrementalTail (	
	uchar_t* p,
	size_t Size,
	size_t TailSize
	)
{
	size_t LeftOver;

	ASSERT (m_IncFindTailSize + Size >= m_PatternSize);
	ASSERT (TailSize < m_PatternSize);

	if (Size >= TailSize) 
	{
		// shortcut -- fill the tail all from the new data

		memcpy (m_Buffer + m_PatternSize, p + Size - TailSize, TailSize);
		m_IncFindTailSize = TailSize;
		return;
	}

	// to fill up the tail here, we need to use some part of old tail...

	LeftOver = TailSize - Size;
	ASSERT (LeftOver < m_IncFindTailSize);

	memmove (
		m_Buffer + m_PatternSize, 
		m_Buffer + m_PatternSize + m_IncFindTailSize - LeftOver,
		LeftOver
		);

	// ... and new data too

	memcpy (
		m_Buffer + m_PatternSize + LeftOver,
		p, 
		TailSize - LeftOver
		);

	m_IncFindTailSize = TailSize;
}

void
CBmhFind::UpdateReverseIncrementalTail (	
	uchar_t* p,
	size_t Size,
	size_t TailSize
	)
{
	size_t LeftOver;

	ASSERT (m_IncFindTailSize + Size >= m_PatternSize);
	ASSERT (TailSize < m_PatternSize);

	if (Size >= TailSize) 
	{
		// shortcut -- fill the tail all from the new data

		memcpy (m_Buffer + 2 * m_PatternSize - TailSize, p, TailSize);
		m_IncFindTailSize = TailSize;
		return;
	}

	// to fill up the tail here, we need to use some part of old tail...

	LeftOver = TailSize - Size;
	ASSERT (LeftOver < m_IncFindTailSize);

	memmove (
		m_Buffer + 2 * m_PatternSize - LeftOver, 
		m_Buffer + 2 * m_PatternSize - m_IncFindTailSize,
		LeftOver
		);

	// ... and new data too

	memcpy (
		m_Buffer + 2 * m_PatternSize - TailSize,
		p, 
		Size
		);

	// tail is filled now

	m_IncFindTailSize = TailSize;
}

//.............................................................................

} // namespace gc
} // namespace axl
