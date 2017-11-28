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

#include "pch.h"
#include "axl_sl_CircularBuffer.h"

namespace axl {
namespace sl {

//..............................................................................

bool
CircularBuffer::setBufferSize (size_t size)
{
	ASSERT (size);

	char buffer [256];
	sl::Array <char> data (ref::BufKind_Stack, buffer, sizeof (buffer));
	size_t dataSize = readAll (&data);

	bool result = m_buffer.setCount (size);
	if (!result)
		return false;

	m_reader = m_buffer;
	m_writer = m_buffer;
	write (data, dataSize);
	return true;
}

size_t
CircularBuffer::readAll (sl::Array <char>* buffer)
{	
	size_t dataSize = getDataSize ();
	bool result = buffer->setCount (dataSize);
	if (!result)
		return false;

	if (m_reader <= m_writer)
	{
		memcpy (*buffer, m_reader, m_writer - m_reader);
	}
	else 
	{
		size_t tailSize = m_buffer.getEnd () - m_reader;
		ASSERT (tailSize);

		memcpy (*buffer, m_reader, tailSize);
		memcpy (*buffer + tailSize, m_buffer, m_writer - m_buffer);
	}

	m_reader = m_buffer;
	m_writer = m_buffer;
	return dataSize;
}

size_t
CircularBuffer::read (
	void* p,
	size_t size
	)
{
	size_t copySize;

	if (m_reader <= m_writer)
	{
		copySize = m_writer - m_reader;
		if (copySize > size)
			copySize = size;

		memcpy (p, m_reader, copySize);
		m_reader += copySize;
	}
	else 
	{
		size_t tailSize = m_buffer.getEnd () - m_reader;
		ASSERT (tailSize);

		if (tailSize >= size)
		{
			memcpy (p, m_reader, size);
			m_reader += size;
			copySize = size;
		}
		else
		{
			memcpy (p, m_reader, tailSize);
			p = (char*) p + tailSize;
			size -= tailSize;

			size_t headSize = m_writer - m_buffer;
			if (headSize > size)
				headSize = size;

			memcpy (p, m_buffer, headSize);
			m_reader = m_buffer + headSize;
			copySize = tailSize + headSize;
		}
	}

	if (m_reader == m_buffer.getEnd ())
		m_reader = m_buffer;

	if (m_reader == m_writer)
	{
		m_reader = m_buffer;
		m_writer = m_buffer;
	}

	return copySize;
}

size_t
CircularBuffer::write (
	const void* p,
	size_t size
	)
{
	size_t copySize;

	if (m_writer < m_reader)
	{
		copySize = m_reader - m_writer;
		if (copySize > size)
			copySize = size;

		memcpy (m_writer, p, copySize);
		m_writer += copySize;
	}
	else
	{
		size_t tailSize = m_buffer.getEnd () - m_writer;
		if (tailSize >= size)
		{
			memcpy (m_writer, p, size);
			m_writer += size;
			copySize = size;
		}
		else
		{
			if (tailSize)
			{
				memcpy (m_writer, p, tailSize);
				p = (char*) p + tailSize;
				size -= tailSize;
			}

			size_t headSize = m_reader - m_buffer;
			if (headSize > size)
				headSize = size;

			memcpy (m_buffer, p, headSize);
			m_writer = m_buffer + headSize;
			copySize = tailSize + headSize;
		}
	}

	return copySize;
}

//..............................................................................

} // namespace err
} // namespace axl
