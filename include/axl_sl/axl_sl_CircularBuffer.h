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

// a classic circular buffer with support for reading and writing at both ends

template <typename C>
class CircularBufferBase {
protected:
	sl::Array<C> m_buffer;
	size_t m_front;
	size_t m_back;
	size_t m_length;

public:
	CircularBufferBase() {
		m_front = 0;
		m_back = 0;
		m_length = 0;
	}

	C&
	operator [] (size_t i) {
		return *locate(i);
	}

	const C&
	operator [] (size_t i) const {
		return *locate(i);
	}

#if (_AXL_DEBUG)
	bool
	isValid() const {
		if (m_buffer.isEmpty()) // uninitialized
			return
				m_buffer == NULL &&
				m_front == 0 &&
				m_back == 0 &&
				m_length == 0;

		return
			m_length <= m_buffer.getCount() &&
			m_front < m_buffer.getCount() &&
			m_back < m_buffer.getCount() &&	(
				m_front < m_back ? m_length == m_back - m_front :
				m_front > m_back ? m_length == m_buffer.getCount() - (m_front - m_back) :
				m_length == 0 ?
					m_front == 0 && m_back == 0 :
					m_length == m_buffer.getCount()
			);
	}
#endif

	bool
	isEmpty() const {
		return m_length == 0;
	}

	bool
	isFull() const {
		return m_length == m_buffer.getCount();
	}

	size_t
	getBufferLength() const {
		return m_buffer.getCount();
	}

	size_t
	getDataLength() const {
		return m_length;
	}

	const C*
	getBuffer() const {
		return m_buffer;
	}

	const C*
	getBufferEnd() const {
		return m_buffer.getEnd();
	}

	const C*
	getFront() const {
		return m_buffer + m_front;
	}

	const C*
	getBack() const {
		return m_buffer + m_back;
	}

	const C*
	locate(size_t i) const {
		ASSERT(i < m_length);
		return m_buffer + (m_front + i) % m_buffer.getCount();
	}

	C*
	locate(size_t i) {
		return (C*)((const CircularBufferBase*)this)->locate(i);
	}

	void
	clear() {
		m_front = 0;
		m_back = 0;
		m_length = 0;
	}

	bool
	create(size_t length) {
		ASSERT(length);

		bool result = m_buffer.setCount(length);
		if (!result)
			return false;

		m_front = 0;
		m_back = 0;
		m_length = 0;
		return true;
	}

	bool
	setBufferLength(size_t length) {
		char buffer[256];
		sl::Array<C> data(rc::BufKind_Stack, buffer, sizeof(buffer));
		size_t dataLength = readAll(&data);

		bool result = create(length);
		if (!result)
			return false;

		write(data, dataLength);
		return true;
	}

	size_t
	readAll(sl::Array<C>* buffer) {
		size_t length = m_length;
		bool result = buffer->setCount(length);
		if (!result)
			return -1;

		copyFrom(m_front, *buffer, length);
		clear();
		return length;
	}

	sl::Array<C>
	readAll() {
		sl::Array<C> buffer;
		readAll(&buffer);
		return buffer;
	}

	size_t
	read(
		C* p,
		size_t length
	) {
		return readFront(p, length);
	}

	size_t
	write(
		const C* p,
		size_t length
	) {
		return writeBack(p, length);
	}

	size_t
	drop(size_t length) {
		return dropFront(length);
	}

	size_t
	readFront(
		C* p,
		size_t length
	) {
		if (length > m_length)
			length = m_length;

		if (!length)
			return 0;

		copyFrom(m_front, p, length);
		m_front = (m_front + length) % m_buffer.getCount();
		m_length -= length;

		ASSERT(isValid());
		return length;
	}

	size_t
	readBack(
		C* p,
		size_t length
	) {
		if (length > m_length)
			length = m_length;

		m_back = (m_back - length + m_buffer.getCount()) % m_buffer.getCount();
		copyFrom(m_back, p, length);
		m_length -= length;

		ASSERT(isValid());
		return length;
	}

	size_t
	writeFront(
		const C* p,
		size_t length
	) {
		size_t leftover = m_buffer.getCount() - m_length;
		if (length > leftover)
			length = leftover;

		m_front = (m_front - length + m_buffer.getCount()) % m_buffer.getCount();
		copyTo(m_front, p, length);
		m_length += length;

		ASSERT(isValid());
		return length;
	}

	size_t
	writeBack(
		const C* p,
		size_t length
	) {
		size_t leftover = m_buffer.getCount() - m_length;
		if (length > leftover)
			length = leftover;

		copyTo(m_back, p, length);
		m_back = (m_back + length) % m_buffer.getCount();
		m_length += length;

		ASSERT(isValid());
		return length;
	}

	size_t
	dropFront(size_t length) {
		if (length > m_length)
			length = m_length;

		m_front = (m_front + length) % m_buffer.getCount();
		m_length -= length;

		ASSERT(isValid());
		return length;
	}

	size_t
	dropBack(size_t length) {
		if (length > m_length)
			length = m_length;

		m_back = (m_back - length + m_buffer.getCount()) % m_buffer.getCount();
		m_length -= length;

		ASSERT(isValid());
		return length;
	}

protected:
	void
	copyFrom(
		size_t i,
		C* p,
		size_t length
	) {
		if (i + length <= m_buffer.getCount())
			sl::ArrayDetails<C>::copy(p, m_buffer + i, length);
		else {
			size_t tail = m_buffer.getCount() - i;
			ASSERT(tail);

			sl::ArrayDetails<C>::copy(p, m_buffer + i, tail);
			sl::ArrayDetails<C>::copy(p + tail, m_buffer, length - tail);
		}
	}

	void
	copyTo(
		size_t i,
		const C* p,
		size_t length
	) {
		if (i + length <= m_buffer.getCount())
			sl::ArrayDetails<C>::copy(m_buffer + i, p, length);
		else {
			size_t tail = m_buffer.getCount() - i;
			ASSERT(tail);

			sl::ArrayDetails<C>::copy(m_buffer + i, p, tail);
			sl::ArrayDetails<C>::copy(m_buffer, p + tail, length - tail);
		}
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the most common kind of circular buffer for binary blobs

class CircularBuffer: public CircularBufferBase<char> {
public:
	// naming consistency aliases -- for byte buffers, we use the term "size"

	size_t
	getDataSize() const {
		return getDataLength();
	}

	size_t
	getBufferSize() const {
		return getBufferLength();
	}

	bool
	setBufferSize(size_t size) {
		return setBufferLength(size);
	}

	size_t
	read(
		void* p,
		size_t length
	) {
		return CircularBufferBase<char>::read((char*)p, length);
	}

	size_t
	write(
		const void* p,
		size_t length
	) {
		return CircularBufferBase<char>::write((char*)p, length);
	}

	size_t
	readFront(
		void* p,
		size_t length
	) {
		return CircularBufferBase<char>::readFront((char*)p, length);

	}

	size_t
	readBack(
		void* p,
		size_t length
	) {
		return CircularBufferBase<char>::readBack((char*)p, length);
	}

	size_t
	writeFront(
		const void* p,
		size_t length
	) {
		return CircularBufferBase<char>::writeFront((char*)p, length);
	}

	size_t
	writeBack(
		const void* p,
		size_t length
	) {
		return CircularBufferBase<char>::writeBack((char*)p, length);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
