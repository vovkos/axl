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
	sl::Array <char> m_buffer;
	char* m_reader;
	char* m_writer;

public:
	CircularBuffer ()
	{
		m_reader = 0;
		m_writer = 0;
	}

	size_t 
	getBufferSize ()
	{
		return m_buffer.getCount ();
	}

	bool
	setBufferSize (size_t size);

	size_t 
	getDataSize ()
	{
		return m_reader <= m_writer ? 
			m_writer - m_reader :
			m_buffer.getCount () + m_writer - m_reader;
	}

	size_t
	readAll (sl::Array <char>* buffer);

	sl::Array <char>
	readAll ()
	{
		sl::Array <char> buffer;
		readAll (&buffer);
		return buffer;
	}

	size_t
	read (
		void* p,
		size_t size
		);

	size_t
	write (
		const void* p,
		size_t size
		);
};

//..............................................................................

} // namespace sl
} // namespace axl
