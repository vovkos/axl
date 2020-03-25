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
#include "axl_io_Pcap.h"
#include "axl_io_PcapFilter.h"

namespace axl {
namespace io {

//..............................................................................

bool
Pcap::openDevice(const sl::StringRef& device)
{
	close();

	char errorBuffer[PCAP_ERRBUF_SIZE];
	m_h = ::pcap_create(device.sz(), errorBuffer);
	if (!m_h)
	{
		err::setError(errorBuffer);
		return false;
	}

	return true;
}

bool
Pcap::openLive(
	const sl::StringRef& device,
	size_t snapshotSize,
	bool isPromiscious,
	uint_t timeout
	)
{
	close();

	char errorBuffer[PCAP_ERRBUF_SIZE];
	m_h = ::pcap_open_live(
		device.sz(),
		snapshotSize,
		isPromiscious,
		timeout,
		errorBuffer
		);

	if (!m_h)
	{
		err::setError(errorBuffer);
		return false;
	}

	return true;
}

bool
Pcap::openFile(const sl::StringRef& fileName)
{
	close();

	char errorBuffer[PCAP_ERRBUF_SIZE];
	m_h = ::pcap_open_offline(fileName.sz(), errorBuffer);
	if (!m_h)
	{
		err::setError(errorBuffer);
		return false;
	}

	return true;
}

bool
Pcap::openDead(
	int linkType,
	size_t snapshotSize
	)
{
	close();

	m_h = ::pcap_open_dead(linkType, (int)snapshotSize);
	if (!m_h)
	{
		err::setError("pcap_open_dead failed");
		return false;
	}

	return true;
}

bool
Pcap::activate()
{
	ASSERT(m_h);

	int result = ::pcap_activate(m_h);
	return complete(result == 0);
}

bool
Pcap::setSnapshotSize(size_t size)
{
	ASSERT(m_h);

	int result = ::pcap_set_snaplen(m_h, size);
	return complete(result == 0);
}

bool
Pcap::setBufferSize(size_t size)
{
	ASSERT(m_h);

	int result = ::pcap_set_buffer_size(m_h, size);
	return complete(result == 0);
}

bool
Pcap::setPromiscious(bool isPromiscious)
{
	ASSERT(m_h);

	int result = ::pcap_set_promisc(m_h, isPromiscious);
	return complete(result == 0);
}

bool
Pcap::setTimeout(uint_t timeout)
{
	ASSERT(m_h);

	int result = ::pcap_set_timeout(m_h, timeout);
	return complete(result == 0);
}

bool
Pcap::setFilter(const bpf_program* filter)
{
	ASSERT(m_h);

	int result = ::pcap_setfilter(m_h, (bpf_program*)filter);
	if (result == -1)
	{
		setLastError();
		return false;
	}

	return true;
}

bool
Pcap::setFilter(
	const sl::StringRef& source,
	bool isOptimized,
	uint32_t netMask
	)
{
	ASSERT(m_h);

	PcapFilter filter;

	return
		filter.compile(m_h, source, isOptimized, netMask) &&
		setFilter(&filter);
}

bool
Pcap::getBlockingMode()
{
	ASSERT(m_h);

	char errorBuffer[PCAP_ERRBUF_SIZE];
	int result = ::pcap_getnonblock(m_h, errorBuffer);

	return result == 0;
}

bool
Pcap::setBlockingMode(bool isBlocking)
{
	ASSERT(m_h);

	char errorBuffer[PCAP_ERRBUF_SIZE];
	int result = ::pcap_setnonblock(m_h, !isBlocking, errorBuffer);
	if (result == -1)
	{
		err::setError(errorBuffer);
		return false;
	}

	return true;
}

PcapLoopResult
Pcap::loop(
	size_t count,
	::pcap_handler handler,
	void* context
	)
{
	ASSERT(m_h);

	int result = ::pcap_loop(m_h, count, handler, (u_char*)context);
	if (result == -1)
		setLastError();

	return (PcapLoopResult)result;
}

size_t
Pcap::dispatch(
	size_t count,
	::pcap_handler handler,
	void* context
	)
{
	ASSERT(m_h);

	int result = ::pcap_dispatch(m_h, count, handler, (u_char*)context);
	if (result == -1)
	{
		setLastError();
		return -1;
	}

	return (intptr_t)result; // propagate potential -2
}

size_t
Pcap::read(
	void* p,
	size_t size,
	uint64_t* timestamp
	)
{
	ASSERT(m_h);

	pcap_pkthdr* hdr;
	const uchar_t* data;

	int result = ::pcap_next_ex(m_h, &hdr, &data);
	if (result == -1)
	{
		setLastError();
		return -1;
	}

	if (result != 1) // special values
		return (intptr_t)result;

	size_t copySize = AXL_MIN(hdr->caplen, size);
	memcpy(p, data, copySize);

	if (timestamp)
		*timestamp = sys::getTimestampFromTimeval(&hdr->ts);

	return copySize;
}

size_t
Pcap::write(
	const void* p,
	size_t size
	)
{
	ASSERT(m_h);

#if (_AXL_OS_WIN)
	int result = ::pcap_sendpacket(m_h, (const u_char*) p, (int)size);
#else
	int result = ::pcap_inject(m_h, p, (int)size);
#endif
	if (result == -1)
	{
		setLastError();
		return -1;
	}

#if (_AXL_OS_WIN)
	return size;
#else
	return result;
#endif
}

bool
Pcap::complete(bool result)
{
	if (!result)
		setLastError();

	return result;
}

//..............................................................................

} // namespace io
} // namespace axl
