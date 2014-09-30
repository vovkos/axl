#include "pch.h"
#include "axl_rtl_BmhFind.h"

namespace axl {
namespace rtl {

//.............................................................................

BmhFind::BmhFind ()
{
	m_patternSize = 0;
	m_incFindOffset = 0;
	m_incFindTailSize = 0; // IncrementalFind temporary storage size -- always < m_PatternSize
	m_doMatchCase = true;
}

void
BmhFind::clear ()
{
	m_patternSize = 0;
	m_incFindOffset = 0;
	m_incFindTailSize = 0;
	m_doMatchCase = true;
}

bool
BmhFind::setPattern (	
	const void* p,
	size_t size,
	bool doMatchCase
	)
{
	size_t newBufferSize = size * 2;
	
	newBufferSize += 128 - newBufferSize % 128; // 128-byte granularity

	clear ();

	if (!size)
		return true;

	m_buffer.setCount (size * 2);

	m_patternSize = size;
	m_incFindOffset = 0;
	m_incFindTailSize = 0;
	m_doMatchCase = doMatchCase;

	memcpy (m_buffer, p, size);

	rebuildTables ();
	return true;
}

size_t 
BmhFind::find (
	const void* _p,
	size_t size
	)
{
	uchar_t* p;
	uchar_t* end;
	size_t m;

	if (!m_patternSize || size < m_patternSize)
		return -1;

	p = (uchar_t*) _p;
	end = p + size - m_patternSize;
	m = m_patternSize - 1;

	do
	{
		uchar_t y = p [m];

		if (cmpPattern (p) == -1)
			return p - (uchar_t*) _p;

		p += m_nextBadCharTable [y];
	} while (p <= end);

	return -1;
}

size_t
BmhFind::reverseFind (	
	const void* _p,
	size_t size
	)
{
	uchar_t* p;

	if (!m_patternSize || size < m_patternSize)
		return -1;

	p = (uchar_t*) _p + size - m_patternSize;

	do 
	{
		uchar_t y = *p;

		if (reverseCmpPattern (p) == -1)
			return size - (p - (uchar_t*) _p);

		p -= m_prevBadCharTable [y];
	} while (p >= (uchar_t*) _p);

	return -1;
}

size_t
BmhFind::incrementalFind (	
	const void* _p,
	size_t size
	)
{
	uchar_t* p = (uchar_t*) _p;
	size_t testSize = size + m_incFindTailSize;

	size_t i;
	size_t iEnd;
	size_t n;

	if (!m_patternSize)
		return -1;

	if (testSize < m_patternSize)
	{
		// incapsulate tail until BM/BMH is possible

		memcpy (m_buffer + m_patternSize + m_incFindTailSize, _p, size);
		m_incFindTailSize += size;
		return -1;
	}
	
	i = 0;
	iEnd = testSize - m_patternSize;
	n = m_patternSize - m_incFindTailSize - 1;

	do
	{
		uchar_t y = p [n + i];

		if (incrementalCmpPattern (p, i) == -1)
		{
			size_t result = m_incFindOffset + i;
			m_incFindOffset = 0;
			m_incFindTailSize = 0;
			return result;
		}

		i += m_nextBadCharTable [y];
	} while (i <= iEnd);

	if (i < testSize)
		updateIncrementalTail (p, size, testSize - i);
	else 
		m_incFindTailSize = 0;

	m_incFindOffset += i;

	return -1;
}

size_t
BmhFind::reverseIncrementalFind (	
	const void* _p,
	size_t size
	)
{
	uchar_t* p = (uchar_t*) _p;
	size_t testSize = size + m_incFindTailSize;

	size_t i;

	if (!m_patternSize)
		return -1;

	if (testSize < m_patternSize)
	{
		// incapsulate tail until BM/BMH is possible
		
		memcpy (m_buffer + 2 * m_patternSize - m_incFindTailSize - size, _p, size);
		m_incFindTailSize += size;
		m_incFindOffset += size;
		return -1;
	}
	
	i = testSize - m_patternSize;

	for (;;)
	{
		uchar_t y = p [i];

		if (reverseIncrementalCmpPattern (p, size, i) == -1)
		{
			size_t result = m_incFindOffset + size - i;
			m_incFindOffset = 0;
			m_incFindTailSize = 0;
			return result;
		}

		if (i >= m_prevBadCharTable [y])
		{
			i -= m_prevBadCharTable [y];
			continue;
		}

		if (m_prevBadCharTable [y] - i < m_patternSize)
			updateReverseIncrementalTail (p, size, m_patternSize - (m_prevBadCharTable [y] - i));
		else
			m_incFindTailSize = 0;

		break;
	}

	m_incFindOffset += size;

	return -1;
}

void 
BmhFind::rebuildTables ()
{
	size_t i;
	size_t m = m_patternSize - 1;
	uchar_t* p = m_buffer;
	
	for (i = 0; i < 256; i++)
	{
		m_nextBadCharTable [i] = 
		m_prevBadCharTable [i] = m_patternSize;
	}

	if (m_doMatchCase)
	{
		for (i = 0; i < m; i++) 
			m_nextBadCharTable [p [i]] = m - i;

		for (i = m; i > 0; i--) 
			m_prevBadCharTable [p [i]] = i;
	}
	else
	{
		for (i = 0; i < m; i++) 
		{
			m_nextBadCharTable [tolower (p [i])] = 
			m_nextBadCharTable [toupper (p [i])] = m - i;
		}

		for (i = m; i > 0; i--) 
		{
			m_prevBadCharTable [tolower (p [i])] = 
			m_prevBadCharTable [toupper (p [i])] = i;
		}
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the following 4 helpers return offset of a mismatch or -1 if argument matches the pattern

size_t
BmhFind::cmpPattern (uchar_t* _p)
{
	uchar_t* p1 = m_buffer + m_patternSize - 1; 
	uchar_t* p2 = _p + m_patternSize - 1; 

	if (m_doMatchCase)
	{
		for (; p1 >= m_buffer; p1--, p2--)
			if (*p1 != *p2)
				return p1 - m_buffer;
	}
	else
	{
		for (; p1 >= m_buffer; p1--, p2--)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_buffer;
	}

	return -1;
}

size_t
BmhFind::reverseCmpPattern (uchar_t* _p)
{
	uchar_t* p1 = m_buffer;
	uchar_t* p2 = _p;
	uchar_t* end = p1 + m_patternSize;

	if (m_doMatchCase)
	{
		for (; p1 < end; p1++, p2++)
			if (*p1 != *p2)
				return p1 - m_buffer;
	}
	else
	{
		for (; p1 < end; p1++, p2++)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_buffer;
	}

	return -1;
}

size_t
BmhFind::incrementalCmpPattern (	
	uchar_t* _p,
	size_t i
	)
{
	uchar_t* p1;
	uchar_t* p2;

	if (i >= m_incFindTailSize)
		return cmpPattern (_p + i - m_incFindTailSize);

	p1 = m_buffer + m_patternSize - 1; 
	p2 = _p + i + m_patternSize - m_incFindTailSize - 1; 

	if (m_doMatchCase)
	{
		for (; p2 >= _p; p1--, p2--)
			if (*p1 != *p2)
				return p1 - m_buffer;

		p2 = m_buffer + m_patternSize + m_incFindTailSize - 1;

		for (; p1 >= m_buffer; p1--, p2--)
			if (*p1 != *p2)
				return p1 - m_buffer;
	}
	else
	{
		for (; p2 >= _p; p1--, p2--)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_buffer;

		p2 = m_buffer + m_patternSize + m_incFindTailSize - 1;

		for (; p1 >= m_buffer; p1--, p2--)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_buffer;
	}

	return -1;
}

size_t
BmhFind::reverseIncrementalCmpPattern (	
	uchar_t* _p,
	size_t size,
	size_t i
	)
{
	uchar_t* p1;
	uchar_t* end1;

	uchar_t* p2;
	uchar_t* end2;

	if (size >= i + m_patternSize)
		return reverseCmpPattern (_p + i);

	p1 = m_buffer;
	end1 = p1 + m_patternSize;
	
	p2 = _p + i;
	end2 = _p + size;

	if (m_doMatchCase)
	{
		for (; p2 < end2; p1++, p2++)
			if (*p1 != *p2)
				return p1 - m_buffer;

		p2 = m_buffer + 2 * m_patternSize - m_incFindTailSize;
	
		for (; p1 < end1; p1++, p2++)
			if (*p1 != *p2)
				return p1 - m_buffer;
	}
	else
	{
		for (; p2 < end2; p1++, p2++)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_buffer;

		p2 = m_buffer + 2 * m_patternSize - m_incFindTailSize;

		for (; p1 < end1; p1++, p2++)
			if (toupper (*p1) != toupper (*p2))
				return p1 - m_buffer;
	}

	return -1;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the following 2 helpers are used to update the state of incremental-find tail

void
BmhFind::updateIncrementalTail (	
	uchar_t* p,
	size_t size,
	size_t tailSize
	)
{
	size_t leftOver;

	ASSERT (m_incFindTailSize + size >= m_patternSize);
	ASSERT (tailSize < m_patternSize);

	if (size >= tailSize) 
	{
		// shortcut -- fill the tail all from the new data

		memcpy (m_buffer + m_patternSize, p + size - tailSize, tailSize);
		m_incFindTailSize = tailSize;
		return;
	}

	// to fill up the tail here, we need to use some part of old tail...

	leftOver = tailSize - size;
	ASSERT (leftOver < m_incFindTailSize);

	memmove (
		m_buffer + m_patternSize, 
		m_buffer + m_patternSize + m_incFindTailSize - leftOver,
		leftOver
		);

	// ... and new data too

	memcpy (
		m_buffer + m_patternSize + leftOver,
		p, 
		tailSize - leftOver
		);

	m_incFindTailSize = tailSize;
}

void
BmhFind::updateReverseIncrementalTail (	
	uchar_t* p,
	size_t size,
	size_t tailSize
	)
{
	size_t leftOver;

	ASSERT (m_incFindTailSize + size >= m_patternSize);
	ASSERT (tailSize < m_patternSize);

	if (size >= tailSize) 
	{
		// shortcut -- fill the tail all from the new data

		memcpy (m_buffer + 2 * m_patternSize - tailSize, p, tailSize);
		m_incFindTailSize = tailSize;
		return;
	}

	// to fill up the tail here, we need to use some part of old tail...

	leftOver = tailSize - size;
	ASSERT (leftOver < m_incFindTailSize);

	memmove (
		m_buffer + 2 * m_patternSize - leftOver, 
		m_buffer + 2 * m_patternSize - m_incFindTailSize,
		leftOver
		);

	// ... and new data too

	memcpy (
		m_buffer + 2 * m_patternSize - tailSize,
		p, 
		size
		);

	// tail is filled now

	m_incFindTailSize = tailSize;
}

//.............................................................................

} // namespace gc
} // namespace axl
