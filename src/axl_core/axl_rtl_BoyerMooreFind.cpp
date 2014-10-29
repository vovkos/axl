#include "pch.h"
#include "axl_rtl_BoyerMooreFind.h"
#include "axl_rtl_BoyerMooreAccessor.h"

namespace axl {
namespace rtl {

//.............................................................................

bool
BinaryBoyerMooreFind::setPattern (
	const void* p,
	size_t size, 
	uint_t flags
	)
{
	bool result = (flags & Flag_Reverse) ?
		m_pattern.copyReverse ((const char*) p, size) :
		m_pattern.copy ((const char*) p, size);

	if (!result)
		return false;

	m_flags = flags;

	buildBadSkipTable ();

	return !(flags & Flag_Horspool) ? buildGoodSkipTable () : true;
}

void
BinaryBoyerMooreFind::buildBadSkipTable ()
{
	size_t patternSize = m_pattern.getCount ();

	m_badSkipTable.setCount (256);

	for (size_t i = 0; i < 256; i++)
		m_badSkipTable [i] = patternSize;

	size_t last = patternSize - 1;
	for (size_t i = 0, j = last; i < last; i++, j--)
		m_badSkipTable [(uchar_t) m_pattern [i]] = j;
}

size_t 
BinaryBoyerMooreFind::find (
	const void* p, 
	size_t size
	)
{
	if (size < m_pattern.getCount ())
		return -1;

	return (m_flags & Flag_Reverse) ? 
		findImpl (BinaryBoyerMooreReverseAccessor ((const char*) p + size - 1), size) : 
		findImpl (BinaryBoyerMooreAccessor ((const char*) p), size);
}

size_t 
BinaryBoyerMooreFind::find (
	IncrementalContext* incrementalContext,
	const void* p, 
	size_t size
	)
{
	size_t fullSize = size + incrementalContext->m_tail.getCount ();
	if (fullSize < m_pattern.getCount ())
	{
		if (m_flags & Flag_Reverse)
			incrementalContext->m_tail.appendReverse ((const char*) p, size);
		else
			incrementalContext->m_tail.append ((const char*) p, size);

		return -1;
	}

	size_t result = (m_flags & Flag_Reverse) ? 
		findImpl (BinaryBoyerMooreIncrementalReverseAccessor ((const char*) p + size - 1, incrementalContext), fullSize) : 
		findImpl (BinaryBoyerMooreIncrementalAccessor ((const char*) p, incrementalContext), fullSize);

	if (result != -1)
	{
		result += incrementalContext->m_offset;
		incrementalContext->reset ();
		return result;
	}

	return -1;
}

template <typename Accessor>
size_t 
BinaryBoyerMooreFind::findImpl (
	const Accessor& accessor, 
	size_t size
	)
{
	size_t patternSize = m_pattern.getCount ();
	ASSERT (patternSize && size >= patternSize);

	size_t last = patternSize - 1;

	size_t i = last;

	if (m_flags & Flag_Horspool)
		while (i < size)
		{
			intptr_t j = last;
		
			uchar_t c;

			for (;;)
			{
				c = accessor.getChar (i);

				if (c != m_pattern [j])
					break;

				if (j == 0)
					return i;

				i--;
				j--;
			}

			i += m_badSkipTable [c];
		}
	else
		while (i < size)
		{
			intptr_t j = last;
		
			uchar_t c;

			for (;;)
			{
				c = accessor.getChar (i);

				if (c != m_pattern [j])
					break;

				if (j == 0)
					return i;

				i--;
				j--;
			}

			size_t badSkip = m_badSkipTable [c];
			size_t goodSkip = m_goodSkipTable [j];
		
			i += AXL_MAX (badSkip, goodSkip);
		}

	ASSERT (i - size <= last);
	accessor.saveTail (i - last, last - (i - size));

	return -1;
}

//.............................................................................

bool
TextBoyerMooreFind::setPattern (
	size_t badSkipTableSize,
	CharCodec* codec,
	const void* p, 
	size_t size,
	uint_t flags
	)
{
	size_t length = codec->decodeToUtf32 (&m_pattern, p, size);

	if (flags & Flag_CaseInsensitive)
		for (size_t i = 0; i < length; i++)
			m_pattern [i] = utfToCaseFold (m_pattern [i]);

	if (flags & Flag_Reverse)
		m_pattern.reverse ();
	
	m_flags = flags;

	bool result = buildBadSkipTable (badSkipTableSize);
	if (!result)
		return false;

	return !(flags & Flag_Horspool) ? buildGoodSkipTable () : true;
}

bool
TextBoyerMooreFind::buildBadSkipTable (size_t tableSize)
{
	size_t patternSize = m_pattern.getCount ();

	bool result = m_badSkipTable.setCount (tableSize);
	if (!result)
		return false;

	for (size_t i = 0; i < tableSize; i++)
		m_badSkipTable [i] = patternSize;

	size_t last = patternSize - 1;
	
	if (m_flags & Flag_CaseInsensitive)
		for (size_t i = 0, j = last; i < last; i++, j--)
		{
			uint32_t c = utfToLower (m_pattern [i]);
			m_badSkipTable [c % tableSize] = j;
		}
	else
		for (size_t i = 0, j = last; i < last; i++, j--)
		{
			uint32_t c = m_pattern [i];
			m_badSkipTable [c % tableSize] = j;
		}
	
	return true;
}

size_t 
TextBoyerMooreFind::find (
	CharCodec* codec,
	const void* p, 
	size_t size
	)
{
	char buffer [256];
	rtl::Array <utf32_t> text (ref::BufKind_Stack, buffer, sizeof (buffer));
	
	size_t length = codec->decodeToUtf32 (&text, p, size);
	if (length == -1)
		return -1;

	if (length < m_pattern.getCount ())
		return -1;

	return (m_flags & Flag_CaseInsensitive) ? 
		(m_flags & Flag_Reverse) ? 
			findImpl (TextBoyerMooreCaseFoldReverseAccessor (text + length - 1), length) : 
			findImpl (TextBoyerMooreCaseFoldAccessor (text), length) :
		(m_flags & Flag_Reverse) ? 
			findImpl (TextBoyerMooreReverseAccessor (text + length - 1), length) : 
			findImpl (TextBoyerMooreAccessor (text), length);

	return 0;
}

size_t 
TextBoyerMooreFind::find (
	IncrementalContext* incrementalContext,
	CharCodec* codec,
	const void* p, 
	size_t size
	)
{
	char buffer [256];
	rtl::Array <utf32_t> text (ref::BufKind_Stack, buffer, sizeof (buffer));
	
	size_t length = codec->decodeToUtf32 (&text, p, size);
	if (length == -1)
		return -1;

	size_t fullLength = length + incrementalContext->m_tail.getCount ();
	if (fullLength < m_pattern.getCount ())
	{
		if (m_flags & Flag_Reverse)
			incrementalContext->m_tail.appendReverse (text, length);
		else
			incrementalContext->m_tail.append (text, length);

		return -1;
	}

	size_t result = (m_flags & Flag_CaseInsensitive) ? 
		(m_flags & Flag_Reverse) ? 
			findImpl (TextBoyerMooreCaseFoldIncrementalReverseAccessor (text + length - 1, incrementalContext), fullLength) : 
			findImpl (TextBoyerMooreCaseFoldIncrementalAccessor (text, incrementalContext), fullLength) :
		(m_flags & Flag_Reverse) ? 
			findImpl (TextBoyerMooreIncrementalReverseAccessor (text + length - 1, incrementalContext), fullLength) : 
			findImpl (TextBoyerMooreIncrementalAccessor (text, incrementalContext), fullLength);

	if (result != -1)
	{
		result += incrementalContext->m_offset;
		incrementalContext->reset ();
		return result;
	}

	return -1;
}

template <typename Accessor>
size_t 
TextBoyerMooreFind::findImpl (
	const Accessor& accessor, 
	size_t size
	)
{
	size_t badSkipTableSize = m_badSkipTable.getCount ();
	size_t patternSize = m_pattern.getCount ();
	ASSERT (patternSize && size >= patternSize);

	size_t last = patternSize - 1;

	size_t i = last;

	if (m_flags & Flag_Horspool)
		while (i < size)
		{
			intptr_t j = last;
		
			uint32_t c;

			for (;;)
			{
				c = accessor.getChar (i);

				if (c != m_pattern [j])
					break;

				if (j == 0)
					return i;

				i--;
				j--;
			}

			i += m_badSkipTable [c % badSkipTableSize];
		}
	else
		while (i < size)
		{
			intptr_t j = last;
		
			uint32_t c;

			for (;;)
			{
				c = accessor.getChar (i);

				if (c != m_pattern [j])
					break;

				if (j == 0)
					return i;

				i--;
				j--;
			}

			size_t badSkip = m_badSkipTable [c % badSkipTableSize];
			size_t goodSkip = m_goodSkipTable [j];
		
			i += AXL_MAX (badSkip, goodSkip);
		}

	ASSERT (i - size <= last);
	accessor.saveTail (i - last, last - (i - size));

	return -1;
}

//.............................................................................

} // namespace rtl
} // namespace axl
