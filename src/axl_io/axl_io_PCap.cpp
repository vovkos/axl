#include "pch.h"
#include "axl_io_PCap.h"
#include "axl_err_Error.h"
#include "axl_sys_Time.h"

namespace axl {
namespace io {

//.............................................................................

bool
PCap::openDevice (
	const char* device,
	size_t snapshotSize,
	bool isPromiscious,
	uint_t readTimeout
	)
{
	close ();

	char errorBuffer [PCAP_ERRBUF_SIZE];
	m_h = pcap_open_live (
		device,
		snapshotSize,
		isPromiscious,
		readTimeout,
		errorBuffer
		);

	if (!m_h)
	{
		err::setStringError (errorBuffer);
		return false;
	}

	return true;
}

bool
PCap::openFile (const char* fileName)
{
	close ();

	char errorBuffer [PCAP_ERRBUF_SIZE];
	m_h = pcap_open_offline (fileName, errorBuffer);
	if (!m_h)
	{
		err::setStringError (errorBuffer);
		return false;
	}

	return true;
}

bool
PCap::setFilter (const char* filter)
{
	bpf_program program;

	int result = pcap_compile (m_h, &program, (char*) filter, true, 0);
	if (result == -1)
	{
		err::setStringError (pcap_geterr (m_h));
		return false;
	}

	result = pcap_setfilter (m_h, &program);
	pcap_freecode (&program);
	return result == 0;
}

bool
PCap::setBlockingMode (bool isBlocking)
{
	char errorBuffer [PCAP_ERRBUF_SIZE];
	int result = pcap_setnonblock (m_h, !isBlocking, errorBuffer);
	if (result == -1)
	{
		err::setStringError (errorBuffer);
		return false;
	}

	return true;
}

size_t
PCap::read (
	void* p,
	size_t size,
	uint64_t* timestamp
	)
{
	pcap_pkthdr* hdr;
	const uchar_t* data;

	int result = pcap_next_ex (m_h, &hdr, &data);
	if (result == -1)
	{
		err::setStringError (pcap_geterr (m_h));
		return -1;
	}

	if (result != 1) // special values
		return result != -2 ? result : 0; // -2 means EOF

	size_t copySize = AXL_MIN (hdr->caplen, size);
	memcpy (p, data, copySize);

	if (timestamp)
		*timestamp = sys::getTimestampFromTimeval (&hdr->ts);

	return copySize;
}

size_t
PCap::write (
	const void* p,
	size_t size
	)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	int result = pcap_sendpacket (m_h, (const u_char*) p, (int) size);
#else
	int result = pcap_inject (m_h, p, (int) size);
#endif
	if (result == -1)
	{
		err::setStringError (pcap_geterr (m_h));
		return -1;
	}

	return size;
}

//.............................................................................

} // namespace io
} // namespace axl
