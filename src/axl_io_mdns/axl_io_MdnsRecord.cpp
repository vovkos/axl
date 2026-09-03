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
#include "axl_io_MdnsRecord.h"

namespace axl {
namespace io {

//..............................................................................

size_t
MdnsRecordParser::parseTxt(sl::Array<mdns_record_txt_t>* array) const {
	mdns_record_txt_t buffer[TxtBufferCount];

	array->clear();

	size_t offset = m_dataOffset;
	size_t length = m_dataLength;

	while (length) {
		size_t finalOffset;

		size_t count = ::mdns_record_parse_txt_ex(
			m_packet,
			m_packetSize,
			offset,
			length,
			buffer,
			countof(buffer),
			&finalOffset
		);

		bool result = array->append(buffer, count) != -1;
		if (!result)
			return -1;

		length -= finalOffset - offset;
		offset = finalOffset;
	}

	return array->getCount();
}

//..............................................................................

} // namespace io
} // namespace axl
