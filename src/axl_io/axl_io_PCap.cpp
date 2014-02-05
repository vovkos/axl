#include "pch.h"
#include "axl_io_PCap.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

bool
CPCap::Open (
	const char* pDevice,
	size_t SnapshotSize,
	bool IsPromiscious,
	uint_t ReadTimeout
	)
{
	Close ();

	char ErrorBuffer [PCAP_ERRBUF_SIZE];
	m_h = pcap_open_live (
		pDevice,
		SnapshotSize,
		IsPromiscious,
		ReadTimeout,
		ErrorBuffer
		);

	if (!m_h)
	{
		err::SetStringError (ErrorBuffer);
		return false;
	}

	return true;
}

bool
CPCap::SetFilter (const char* pFilter)
{
	bpf_program Program;

	int Result = pcap_compile (m_h, &Program, (char*) pFilter, true, 0);
	if (Result == -1)
	{
		err::SetStringError (pcap_geterr (m_h));
		return false;
	}

	Result = pcap_setfilter (m_h, &Program);
	pcap_freecode (&Program);
	return Result == 0;
}

bool
CPCap::SetBlockingMode (bool IsBlocking)
{
	char ErrorBuffer [PCAP_ERRBUF_SIZE];
	int Result = pcap_setnonblock (m_h, !IsBlocking, ErrorBuffer);
	if (Result == -1)
	{
		err::SetStringError (ErrorBuffer);
		return false;
	}

	return true;
}

size_t
CPCap::Read (
	void* p,
	size_t Size
	)
{
	pcap_pkthdr* pHdr;
	const uchar_t* pData;

	int Result = pcap_next_ex (m_h, &pHdr, &pData);
	if (Result == -1)
	{
		err::SetStringError (pcap_geterr (m_h));
		return -1;
	}

	if (Result != 1) // special values
		return Result;

	size_t CopySize = AXL_MIN (pHdr->caplen, Size);
	memcpy (p, pData, CopySize);
	return CopySize;
}

size_t
CPCap::Write (
	const void* p,
	size_t Size
	)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	int Result = pcap_sendpacket (m_h, (const u_char*) p, (int) Size);
#else
	int Result = pcap_inject (m_h, p, (int) Size);
#endif
	if (Result == -1)
	{
		err::SetStringError (pcap_geterr (m_h));
		return -1;
	}

	return Size;
}

//.............................................................................

} // namespace io
} // namespace axl
