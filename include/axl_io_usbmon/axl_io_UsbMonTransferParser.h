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

#define _AXL_IO_USBMONTRANSFERPARSER_H

#include "axl_io_UsbMonTransfer.h"

namespace axl {
namespace io {

//..............................................................................

enum UsbMonTransferParserState {
	UsbMonTransferParserState_IncompleteHeader = 0,
	UsbMonTransferParserState_CompleteHeader,
	UsbMonTransferParserState_IncompleteData,
	UsbMonTransferParserState_CompleteData,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
    typename T,
	typename H
>
class UsbMonTransferParserBase {
protected:
	UsbMonTransferParserState m_state;
	UsbMonTransferHdr m_hiHdr;

	union {
		H m_loHdr;
		char m_buffer[1];
	};

	size_t m_offset; // offset in the original stream

public:
	UsbMonTransferParserBase() {
		memset(this, 0, sizeof(UsbMonTransferParserBase));
	}

	UsbMonTransferParserState
	getState() const {
		return m_state;
	}

	const UsbMonTransferHdr* getTransferHdr() const {
		ASSERT(m_state >= UsbMonTransferParserState_CompleteHeader);
		return &m_hiHdr;
	}

	void
	reset() {
		m_state = UsbMonTransferParserState_IncompleteHeader;
		m_offset = 0;
	}

	size_t
	parse(
	 	const void* p,
	 	size_t size
	);

protected:
	size_t
	parseData(size_t size);

	const char*
	buffer(
		size_t targetSize,
	 	const char* p,
		const char* end
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
    typename T,
	typename B
>
size_t
UsbMonTransferParserBase<T, B>::parse(
 	const void* p,
 	size_t size
) {
	switch (m_state) {
	case UsbMonTransferParserState_IncompleteHeader:
		return static_cast<T*>(this)->parseHeader(p, size);

	case UsbMonTransferParserState_CompleteHeader:
		if (m_hiHdr.m_captureSize)
			return parseData(size);

		reset();
		return static_cast<T*>(this)->parseHeader(p, size);

	case UsbMonTransferParserState_IncompleteData:
		return parseData(size);

	case UsbMonTransferParserState_CompleteData:
		reset();
		return static_cast<T*>(this)->parseHeader(p, size);

	default:
		ASSERT(false);
		return err::fail<size_t>(-1, err::SystemErrorCode_InvalidDeviceState);
	}
}

template <
    typename T,
	typename B
>
size_t
UsbMonTransferParserBase<T, B>::parseData(size_t size) {
	size_t leftover = m_hiHdr.m_captureSize - m_offset;
	size_t taken;

	if (size < leftover) {
		m_state = UsbMonTransferParserState_IncompleteData;
		taken = size;
	} else {
		m_state = UsbMonTransferParserState_CompleteData;
		taken = leftover;
	}

	m_offset += taken;
	return taken;
}

template <
    typename T,
	typename B
>
const char*
UsbMonTransferParserBase<T, B>::buffer(
	size_t targetSize,
	const char* p,
	const char* end
) {
	if (m_offset >= targetSize)
		return p;

	size_t available = end - p;
	size_t leftover = targetSize - m_offset;
	size_t taken = AXL_MIN(leftover, available);
	memcpy(m_buffer + m_offset, p, taken);
	m_offset += taken;
	return p + taken;
}

//..............................................................................

} // namespace io
} // namespace axl
