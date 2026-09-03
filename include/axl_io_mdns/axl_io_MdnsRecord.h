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

#define _AXL_IO_MDNSRECORD_H

#include "axl_io_MdnsPch.h"

namespace axl {
namespace io {

//..............................................................................

// mdns_string_t and sl::StringRef are essentially the same thing

inline
sl::StringRef
mdns2Axl(const mdns_string_t& string) {
	return sl::StringRef(string.str, string.length);
}

inline
bool
mdns2Axl(
	sl::String* string_axl,
	const mdns_string_t& string_mdns
) {
	return string_axl->copy(string_mdns.str, string_mdns.length) != -1;
}

inline
mdns_string_t
axl2Mdns(const sl::StringRef& string) {
	return mdns_string_t { string.cp(), string.getLength() };
}

//..............................................................................

// a record to be sent out; adds nothing to mdns_record_t, so it slices cleanly
// into the mdns answer/announce/goodbye API

class MdnsRecord: public mdns_record_t {
public:
	MdnsRecord() {
		memset((mdns_record_t*)this, 0, sizeof(mdns_record_t));
		rclass = MDNS_CLASS_IN;
	}

	void
	setPtr(
		const sl::StringRef& name,
		const sl::StringRef& target,
		uint_t ttl
	) {
		setup(name, MDNS_RECORDTYPE_PTR, ttl);
		data.ptr.name = axl2Mdns(target);
	}

	void
	setSrv(
		const sl::StringRef& name,
		const sl::StringRef& target,
		uint_t priority,
		uint_t weight,
		uint_t port,
		uint_t ttl
	);

	void
	setA(
		const sl::StringRef& name,
		const sockaddr_in* addr,
		uint_t ttl
	) {
		setup(name, MDNS_RECORDTYPE_A, ttl);
		data.a.addr = *addr;
	}

	void
	setAaaa(
		const sl::StringRef& name,
		const sockaddr_in6* addr,
		uint_t ttl
	) {
		setup(name, MDNS_RECORDTYPE_AAAA, ttl);
		data.aaaa.addr = *addr;
	}

	void
	setTxt(
		const sl::StringRef& name,
		const sl::StringRef& key,
		const sl::StringRef& value,
		uint_t ttl
	);

	void
	setCacheFlush(bool isCacheFlush = true) {
		if (isCacheFlush)
			rclass |= MDNS_CACHE_FLUSH;
		else
			rclass &= ~MDNS_CACHE_FLUSH;
	}

protected:
	void
	setup(
		const sl::StringRef& name,
		mdns_record_type_t recordType,
		uint_t ttl
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
MdnsRecord::setSrv(
	const sl::StringRef& name,
	const sl::StringRef& target,
	uint_t priority,
	uint_t weight,
	uint_t port,
	uint_t ttl
) {
	setup(name, MDNS_RECORDTYPE_SRV, ttl);
	data.srv.name = axl2Mdns(target);
	data.srv.priority = (uint16_t)priority;
	data.srv.weight = (uint16_t)weight;
	data.srv.port = (uint16_t)port;
}

inline
void
MdnsRecord::setTxt(
	const sl::StringRef& name,
	const sl::StringRef& key,
	const sl::StringRef& value,
	uint_t ttl
) {
	setup(name, MDNS_RECORDTYPE_TXT, ttl);
	data.txt.key = axl2Mdns(key);
	data.txt.value = axl2Mdns(value);
}

inline
void
MdnsRecord::setup(
	const sl::StringRef& name,
	mdns_record_type_t recordType,
	uint_t ttl
) {
	memset(&data, 0, sizeof(data));
	this->name = axl2Mdns(name);
	this->type = recordType;
	this->ttl = ttl;
}

//..............................................................................

// name of mdns_record_srv_t returned by mdns_record_parse_srv points to
// a scratch buffer -- we need to lift it to heap

struct MdnsRecordSrv: mdns_record_srv_t {
	sl::String m_name;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// parses one record out of a received packet; the packet must outlive the parser
// (mdns_string_extract needs it -- DNS names are compressed with back-pointers)

class MdnsRecordParser {
protected:
	enum {
		MaxNameLength  = 255,  // RFC 1035 caps an FQDN at 255
		TxtBufferCount = 16,   // TXT entries decoded per pass, on the stack
	};

protected:
	const void* m_packet;
	size_t m_packetSize;
	size_t m_nameOffset;
	size_t m_nameLength;
	size_t m_dataOffset;
	size_t m_dataLength;

public:
	MdnsRecordParser() {
		memset(this, 0, sizeof(MdnsRecordParser));
	}

	MdnsRecordParser(
		const void* packet,
		size_t packetSize,
		size_t nameOffset,
		size_t nameLength,
		size_t dataOffset,
		size_t dataLength
	) {
		setup(packet, packetSize, nameOffset, nameLength, dataOffset, dataLength);
	}

	void
	setup(
		const void* packet,
		size_t packetSize,
		size_t nameOffset,
		size_t nameLength,
		size_t dataOffset,
		size_t dataLength
	);

	// the record owner name, e.g. "_http._tcp.local."

	size_t
	getName(sl::String* name) const {
		return extractString(name, m_nameOffset);
	}

	sl::String
	getName() const;

	// parsing of individual record types

	size_t
	parsePtr(sl::String* name) const;

	sl::String
	parsePtr() const;

	size_t
	parseSrv(MdnsRecordSrv* srv) const;

	size_t
	parseA(SockAddr* addr) const;

	size_t
	parseAaaa(SockAddr* addr) const;

	size_t
	parseTxt(sl::Array<mdns_record_txt_t>* array) const;

	// common string extraction

	size_t
	extractString(
		sl::String* string,
		size_t offset
	) const;

	sl::String
	extractString(size_t offset) const {
		sl::String string;
		extractString(&string, offset);
		return string;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
MdnsRecordParser::setup(
	const void* packet,
	size_t packetSize,
	size_t nameOffset,
	size_t nameLength,
	size_t dataOffset,
	size_t dataLength
) {
	m_packet = packet;
	m_packetSize = packetSize;
	m_nameOffset = nameOffset;
	m_nameLength = nameLength;
	m_dataOffset = dataOffset;
	m_dataLength = dataLength;
}

inline
sl::String
MdnsRecordParser::getName() const {
	sl::String name;
	getName(&name);
	return name;
}

inline
sl::String
MdnsRecordParser::parsePtr() const {
	sl::String name;
	parsePtr(&name);
	return name;
}

inline
size_t
MdnsRecordParser::extractString(
	sl::String* string,
	size_t offset
) const {
	char buffer[MaxNameLength + 1];

	mdns_string_t result = ::mdns_string_extract(
		m_packet,
		m_packetSize,
		&offset,
		buffer,
		sizeof(buffer)
	);

	return string->copy(result.str, result.length);
}

inline
size_t
MdnsRecordParser::parsePtr(sl::String* name) const {
	char buffer[MaxNameLength + 1];

	mdns_string_t result = ::mdns_record_parse_ptr(
		m_packet,
		m_packetSize,
		m_dataOffset,
		m_dataLength,
		buffer,
		sizeof(buffer)
	);

	return name->copy(result.str, result.length);
}

inline
size_t
MdnsRecordParser::parseSrv(MdnsRecordSrv* srv_o) const {
	char buffer[MaxNameLength + 1];

	mdns_record_srv_t srv = ::mdns_record_parse_srv(
		m_packet,
		m_packetSize,
		m_dataOffset,
		m_dataLength,
		buffer,
		sizeof(buffer)
	);

	srv_o->priority = srv.priority;
	srv_o->weight = srv.weight;
	srv_o->port = srv.port;
	size_t result = mdns2Axl(&srv_o->m_name, srv.name);
	if (result == -1)
		return -1;

	srv_o->name = axl2Mdns(srv_o->m_name);
	return result;
}

inline
size_t
MdnsRecordParser::parseA(SockAddr* addr) const {
	if (m_dataLength != sizeof(in_addr))
		return err::fail<size_t>(-1, "malformed mDNS A record");

	::mdns_record_parse_a(m_packet, m_packetSize, m_dataOffset, m_dataLength, &addr->m_addr_ip4);
	return m_dataLength;
}

inline
size_t
MdnsRecordParser::parseAaaa(SockAddr* addr) const {
	if (m_dataLength != sizeof(in6_addr))
		return err::fail<size_t>(-1, "malformed mDNS AAAA record");

	::mdns_record_parse_aaaa(m_packet, m_packetSize, m_dataOffset, m_dataLength, &addr->m_addr_ip6);
	return m_dataLength;
}

//..............................................................................

} // namespace io
} // namespace axl
