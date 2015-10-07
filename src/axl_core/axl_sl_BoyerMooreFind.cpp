#include "pch.h"
#include "axl_sl_BoyerMooreFind.h"
#include "axl_sl_BoyerMooreAccessor.h"

namespace axl {
namespace sl {

//.............................................................................

bool
BinaryBoyerMooreFind::setPattern (
	const void* p,
	size_t size, 
	uint_t flags
	)
{
	if (!size)
	{
		clear ();
		return true;
	}

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
	size_t patternSize = m_pattern.getCount ();
	if (!patternSize)
		return 0;
		
	if (size < patternSize)
		return -1;

	size_t result = (m_flags & Flag_Reverse) ? 
		findImpl (BinaryBoyerMooreReverseAccessor ((const char*) p + size - 1), size) : 
		findImpl (BinaryBoyerMooreAccessor ((const char*) p), size);

	if (result == -1)
		return -1;

	if (m_flags & Flag_Reverse)
		result = size - result - patternSize;

	return result;
}

size_t 
BinaryBoyerMooreFind::find (
	IncrementalContext* incrementalContext,
	size_t offset,
	const void* p, 
	size_t size
	)
{
	size_t patternSize = m_pattern.getCount ();
	if (!patternSize)
		return offset;

	size_t tailSize = incrementalContext->m_tail.getCount ();
	size_t fullSize = size + tailSize;

	if (fullSize < patternSize)
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

	if (result == -1)
		return -1;
	
	incrementalContext->reset ();

	result -= tailSize;

	if (m_flags & Flag_Reverse)
		result = size - result - patternSize;

	return offset + result;
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

	ASSERT (i > last && i - last <= size);

	i -= last;
	accessor.saveTail (i, size - i);

	return -1;
}

//.............................................................................

bool
TextBoyerMooreFind::setPattern (
	size_t badSkipTableSize,
	enc::CharCodec* codec,
	const void* p, 
	size_t size,
	uint_t flags
	)
{
	size_t length = codec->decodeToUtf32 (&m_pattern, p, size);
	if (!length)
	{
		clear ();
		return true;
	}

	if (flags & Flag_CaseInsensitive)
		for (size_t i = 0; i < length; i++)
			m_pattern [i] = enc::utfToCaseFold (m_pattern [i]);

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
			uint32_t c = enc::utfToCaseFold (m_pattern [i]);
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
	enc::CharCodec* codec,
	const void* p, 
	size_t size
	)
{
	size_t patternLength = m_pattern.getCount ();
	if (!patternLength)
		return 0;

	size_t length = codec->decodeToUtf32 (&m_buffer, p, size);
	if (length == -1)
		return -1;

	if (length < patternLength)
		return -1;

	if (m_flags & Flag_WholeWord)
		m_buffer.append (' ');

	size_t result = (m_flags & Flag_CaseInsensitive) ? 
		(m_flags & Flag_Reverse) ? 
			findImpl (TextBoyerMooreCaseFoldReverseAccessor (m_buffer + length - 1), length, length) : 
			findImpl (TextBoyerMooreCaseFoldAccessor (m_buffer), length, length) :
		(m_flags & Flag_Reverse) ? 
			findImpl (TextBoyerMooreReverseAccessor (m_buffer + length - 1), length, length) : 
			findImpl (TextBoyerMooreAccessor (m_buffer), length, length);

	if (result == -1)
		return -1;
	
	if (m_flags & Flag_Reverse)
		result = size - result - patternLength;

	ASSERT (result <= length);
	return codec->calcRequiredBufferSizeToEncodeFromUtf32 (m_buffer, result);
}

size_t 
TextBoyerMooreFind::find (
	IncrementalContext* incrementalContext,
	enc::CharCodec* codec,
	size_t offset,
	const void* p, 
	size_t size
	)
{
	size_t patternLength = m_pattern.getCount ();
	if (!patternLength)
		return offset;

	size_t chunkLength = codec->decodeToUtf32 (&m_buffer, p, size);
	if (chunkLength == -1 || chunkLength == 0)
		return -1;

	size_t tailLength = incrementalContext->m_tail.getCount ();
	size_t fullLength = chunkLength + tailLength;
	size_t end = fullLength;

	if (m_flags & Flag_WholeWord)
		end--;

	if (end < patternLength)
	{
		if (m_flags & Flag_Reverse)
			incrementalContext->m_tail.appendReverse (m_buffer, chunkLength);
		else
			incrementalContext->m_tail.append (m_buffer, chunkLength);

		return -1;
	}

	size_t result = (m_flags & Flag_CaseInsensitive) ? 
		(m_flags & Flag_Reverse) ? 
			findImpl (
				TextBoyerMooreCaseFoldIncrementalReverseAccessor (m_buffer + chunkLength - 1, incrementalContext), 
				end,
				fullLength
				) : 
			findImpl (
				TextBoyerMooreCaseFoldIncrementalAccessor (m_buffer, incrementalContext), 
				end,
				fullLength
				) :
		(m_flags & Flag_Reverse) ? 
			findImpl (
				TextBoyerMooreIncrementalReverseAccessor (m_buffer + chunkLength - 1, incrementalContext), 
				end,
				fullLength
				) : 
			findImpl (
				TextBoyerMooreIncrementalAccessor (m_buffer, incrementalContext), 
				end,
				fullLength
				);

	if (result == -1)
		return -1;

	result -= tailLength;

	if (m_flags & Flag_Reverse)
		result = size - result - patternLength;

	if ((intptr_t) result < 0)
	{
		ASSERT (-result <= tailLength);
		result = -codec->calcRequiredBufferSizeToEncodeFromUtf32 (incrementalContext->m_tail, -result);
	}
	else if (result)
	{	
		ASSERT (result <= chunkLength);
		result = codec->calcRequiredBufferSizeToEncodeFromUtf32 (m_buffer, result);
	}

	incrementalContext->reset ();
	return offset + result;
}

template <typename Accessor>
size_t 
TextBoyerMooreFind::findImpl (
	const Accessor& accessor, 
	size_t end,
	size_t size
	)
{
	size_t badSkipTableSize = m_badSkipTable.getCount ();
	size_t patternSize = m_pattern.getCount ();
	ASSERT (patternSize && end >= patternSize);

	size_t last = patternSize - 1;

	size_t i = last;

	if (m_flags & Flag_Horspool)
		while (i < end)
		{
			intptr_t j = last;
		
			uint32_t c;

			for (;;)
			{
				c = accessor.getChar (i);

				if (c != m_pattern [j])
					break;

				if (j == 0)
				{
					if ((m_flags & Flag_WholeWord) && 
						(!accessor.isDelimChar (i - 1) || !accessor.isDelimChar (i + patternSize)))
					{
						break;
					}

					return i;
				}

				i--;
				j--;
			}

			i += m_badSkipTable [c % badSkipTableSize];
		}
	else
		while (i < end)
		{
			intptr_t j = last;
		
			uint32_t c;

			for (;;)
			{
				c = accessor.getChar (i);

				if (c != m_pattern [j])
					break;

				if (j == 0)
				{
					if ((m_flags & Flag_WholeWord) && 
						(!accessor.isDelimChar (i - 1) || !accessor.isDelimChar (i + patternSize)))
					{
						break;
					}

					return i;
				}

				i--;
				j--;
			}

			size_t badSkip = m_badSkipTable [c % badSkipTableSize];
			size_t goodSkip = m_goodSkipTable [j];
		
			i += AXL_MAX (badSkip, goodSkip);
		}

	ASSERT (i > last && i - last <= size);

	i -= last;
	accessor.saveTail (i, size - i);

	return -1;
}

//.............................................................................

} // namespace sl
} // namespace axl
