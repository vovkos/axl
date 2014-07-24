#include "pch.h"
#include "axl_io_MappedFileTransport.h"
#include "axl_rtl_String.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

bool 
CMappedFileTransport::Open (
	bool IsWriter,
	const char* pMutexName,
	const char* pFileName,
	const char* pReadEventName,
	const char* pWriteEventName
	)
{
	Close ();

	bool Result = m_File.Open (pFileName);
	if (!Result)
		return false;

	m_pHdr = (TMappedFileTransportHdr*) m_File.View (0, sizeof (TMappedFileTransportHdr), true);

	if (IsWriter)
	{
		m_pHdr->m_Signature = EMappedFileTransport_MessageSignature;
		m_pHdr->m_ReadOffset = 0;
		m_pHdr->m_WriteOffset = 0;
	}
	else if (
		m_pHdr->m_ReadOffset != 0 || 
		m_pHdr->m_Signature != EMappedFileTransport_MessageSignature)
	{
		err::SetError (err::EStatus_InvalidParameter);
		return false;
	}

	Result = 
		m_ReadEvent.m_Event.Create (NULL, false, false, rtl::CString_utf16 (pReadEventName)) &&
		m_WriteEvent.m_Event.Create (NULL, false, false, rtl::CString_utf16 (pWriteEventName));

	if (!Result)
		return false;

	return true;
}

void
CMappedFileTransport::Close ()
{
	if (!IsOpen ())
		return;

	m_File.Close ();
	m_pHdr = NULL;
}

void*
CMappedFileTransport::Read (size_t* pSize)
{
	ASSERT (m_pHdr);
	m_WriteEvent.Wait ();
	
	TMappedFileMessageHdr* pMsgHdr = (TMappedFileMessageHdr*) m_File.View (m_pHdr->m_ReadOffset, sizeof (TMappedFileMessageHdr));
	if (pMsgHdr->m_Signature != EMappedFileTransport_MessageSignature || 
		pMsgHdr->m_Size <= sizeof (TMappedFileMessageHdr))
	{
		err::SetError (err::EStatus_InvalidParameter);
		return false;
	}

	pMsgHdr = (TMappedFileMessageHdr*) m_File.View (m_pHdr->m_ReadOffset, pMsgHdr->m_Size);
	if (pSize)
		*pSize = pMsgHdr->m_Size - sizeof (TMappedFileMessageHdr);

	m_pHdr->m_ReadOffset += pMsgHdr->m_Size;
	if (m_pHdr->m_ReadOffset >= m_pHdr->m_EndOffset)
	{
	}

	m_ReadEvent.Signal ();
	return pMsgHdr + 1;
}

size_t
CMappedFileTransport::Write (
	const void* p,
	size_t Size
	)
{
	ASSERT (m_pHdr);

	uint32_t ReadOffset = m_pHdr->m_ReadOffset;
	uint32_t WriteOffset = m_pHdr->m_WriteOffset;
	uint32_t EndOffset = m_pHdr->m_EndOffset;

	if (WriteOffset < ReadOffset)
	{
	}

	return 0;
}

size_t
CMappedFileTransport::Write (
	size_t BlockCount,
	const void* p,
	size_t Size,
	...
	)
{
	m_WriteEvent.Signal ();
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace io {
