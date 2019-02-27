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

#define _AXL_SL_CIRCULARBUFFER_H

#include "axl_sl_Array.h"

namespace axl {
namespace sl {

//..............................................................................

class CircularBuffer
{
protected:
	sl::Array<char> m_buffer;
	char* m_reader;
	char* m_writer;
	size_t m_dataSize;

public:
	CircularBuffer();

	bool
	isValid() const;

	bool
	isEmpty() const
	{
		return m_dataSize == 0;
	}

	bool
	isFull() const
	{
		ASSERT(isValid());
		return m_dataSize == m_buffer.getCount();
	}

	size_t
	getBufferSize() const
	{
		return m_buffer.getCount();
	}

	bool
	setBufferSize(size_t size);

	size_t
	getDataSize() const
	{
		return m_dataSize;
	}

	void
	clear();

	size_t
	readAll(sl::Array<char>* buffer);

	sl::Array<char>
	readAll()
	{
		sl::Array<char> buffer;
		readAll(&buffer);
		return buffer;
	}

	size_t
	read(
		void* p,
		size_t size
		);

	size_t
	write(
		const void* p,
		size_t size
		);

	size_t
	drop(size_t size);
};

//..............................................................................

} // namespace sl
} // namespace axl
