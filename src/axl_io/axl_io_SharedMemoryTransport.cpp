#include "pch.h"
#include "axl_io_SharedMemoryTransport.h"
#include "axl_rtl_String.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

CSharedMemoryTransportBase::CSharedMemoryTransportBase ()
{
	m_Flags = 0;
	m_pHdr = NULL;
	m_pData = NULL;
	m_MappingSize = 0;
	m_PendingReqCount = 0;
}

bool
CSharedMemoryTransportBase::Open (
	const char* pFileName,
	const char* pReadEventName,
	const char* pWriteEventName,
	uint_t Flags
	)
{
	Close ();

	bool Result = m_File.Open (pFileName, io::EFileFlag_ShareWrite);
	if (!Result)
		return false;

	Result = EnsureMappingSize (ESharedMemoryTransport_DefMappingSize);
	if (!Result)
	{
		m_File.Close ();
		return false;
	}

	if (Flags & ESharedMemoryTransportFlag_Create)
	{
		m_pHdr->m_Signature = ESharedMemoryTransport_FileSignature;
		m_pHdr->m_Lock = 0;
		m_pHdr->m_State = ESharedMemoryTransportState_MasterConnected;
		m_pHdr->m_ReadOffset = 0;
		m_pHdr->m_WriteOffset = 0;
		m_pHdr->m_EndOffset = 0;
		m_pHdr->m_DataSize = 0;

#if (_AXL_ENV == AXL_ENV_WIN)
		Result =
			m_ReadEvent.Create (NULL, false, false, rtl::CString_utf16 (pReadEventName)) &&
			m_WriteEvent.Create (NULL, false, false, rtl::CString_utf16 (pWriteEventName));
#elif (_AXL_ENV == AXL_ENV_POSIX)
		Result = m_ReadEvent.Open (pReadEventName, O_CREAT);
		if (Result)
		{
			m_ReadEventName = pReadEventName;

			Result = m_WriteEvent.Open (pWriteEventName, O_CREAT);
			if (Result)
				m_WriteEventName = pWriteEventName;
		}
#endif
	}
	else
	{
		if (m_pHdr->m_Signature != ESharedMemoryTransport_FileSignature)
		{
			err::SetError (err::EStatus_InvalidParameter);
			return false;
		}

		mt::AtomicLock (&m_pHdr->m_Lock);

		if (m_pHdr->m_State != ESharedMemoryTransportState_MasterConnected ||
			m_pHdr->m_ReadOffset != 0)
		{
			err::SetError (err::EStatus_InvalidDeviceState);
			return false;
		}

		m_pHdr->m_State = ESharedMemoryTransportState_SlaveConnected;

		mt::AtomicUnlock (&m_pHdr->m_Lock);

#if (_AXL_ENV == AXL_ENV_WIN)
		Result =
			m_ReadEvent.Open (EVENT_ALL_ACCESS, false, rtl::CString_utf16 (pReadEventName)) &&
			m_WriteEvent.Open (EVENT_ALL_ACCESS, false, rtl::CString_utf16 (pWriteEventName));
#elif (_AXL_ENV == AXL_ENV_POSIX)
		Result =
			m_ReadEvent.Open (pReadEventName, 0) &&
			m_WriteEvent.Open (pWriteEventName, 0);
#endif
	}

	if (!Result)
	{
		Close ();
		return false;
	}

	m_Flags = Flags;
	return true;
}

void
CSharedMemoryTransportBase::Close ()
{
	if (!IsOpen ())
		return;

	Disconnect ();

	m_File.Close ();
	m_ReadEvent.Close ();
	m_WriteEvent.Close ();
	m_pHdr = NULL;
	m_pData = NULL;
	m_MappingSize = 0;
	m_PendingReqCount = 0;

#if (_AXL_ENV == AXL_ENV_POSIX)
	if (!m_ReadEventName.IsEmpty ())
	{
		mt::psx::CSem::Unlink (m_ReadEventName);
		m_ReadEventName.Clear ();
	}

	if (!m_WriteEventName.IsEmpty ())
	{
		mt::psx::CSem::Unlink (m_WriteEventName);
		m_WriteEventName.Clear ();
	}
#endif
}

void
CSharedMemoryTransportBase::Disconnect ()
{
	mt::AtomicLock (&m_pHdr->m_Lock);
	m_pHdr->m_State = ESharedMemoryTransportState_Disconnected;

#if (_AXL_ENV == AXL_ENV_WIN)
	m_ReadEvent.Signal ();
	m_WriteEvent.Signal ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	m_ReadEvent.Post ();
	m_WriteEvent.Post ();
#endif

	mt::AtomicUnlock (&m_pHdr->m_Lock);
}

bool
CSharedMemoryTransportBase::EnsureMappingSize (size_t Size)
{
	if (Size <= m_MappingSize)
		return true;

	void* p = m_File.View (0, Size);
	if (!p)
		return false;

	m_pHdr = (TSharedMemoryTransportHdr*) p;
	m_pData = (char*) (m_pHdr + 1);
	m_MappingSize = Size;
	return true;
}

//.............................................................................

rtl::CArrayT <char>
CSharedMemoryReader::Read ()
{
	rtl::CArrayT <char> Buffer;
	Read (&Buffer);
	return Buffer;
}

size_t
CSharedMemoryReader::Read (rtl::CArrayT <char>* pBuffer)
{
	ASSERT (IsOpen ());

	mt::AtomicLock (&m_pHdr->m_Lock);

	// if buffer is empty, then wait until we have any data

	while (
		!m_pHdr->m_DataSize &&
		m_pHdr->m_State != ESharedMemoryTransportState_Disconnected)
	{
		mt::AtomicUnlock (&m_pHdr->m_Lock);
		m_WriteEvent.Wait ();
		mt::AtomicLock (&m_pHdr->m_Lock);
	}

	if (m_pHdr->m_State == ESharedMemoryTransportState_Disconnected)
	{
		mt::AtomicUnlock (&m_pHdr->m_Lock);
		err::SetError (err::EStatus_InvalidDeviceState);
		return -1;
	}

	size_t ReadOffset = m_pHdr->m_ReadOffset;
	size_t WriteOffset = m_pHdr->m_WriteOffset;
	size_t EndOffset = m_pHdr->m_EndOffset;
	mt::AtomicUnlock (&m_pHdr->m_Lock);

	bool Result = EnsureOffsetMapped (EndOffset);
	if (!Result)
		return -1;

	size_t ReadSize = 0;

	if (m_Flags & ESharedMemoryTransportFlag_Message)
	{
		TSharedMemoryTransportMessageHdr* pMsgHdr = (TSharedMemoryTransportMessageHdr*) (m_pData + ReadOffset);
		ReadSize = pMsgHdr->m_Size;
		size_t ReadEndOffset = ReadOffset + sizeof (TSharedMemoryTransportMessageHdr) + pMsgHdr->m_Size;

		if (pMsgHdr->m_Signature != ESharedMemoryTransport_MessageSignature || ReadEndOffset > EndOffset)
		{
			err::SetError (err::EStatus_InvalidParameter);
			return -1;
		}

		pBuffer->Copy ((const char*) (pMsgHdr + 1), ReadSize);

		mt::AtomicLock (&m_pHdr->m_Lock);
		ASSERT (ReadEndOffset <= m_pHdr->m_EndOffset);

		m_pHdr->m_ReadOffset = m_pHdr->m_EndOffset != m_pHdr->m_WriteOffset && ReadEndOffset >= m_pHdr->m_EndOffset ?
			0 : ReadEndOffset; // wrap : no wrap
	}
	else
	{
		if (ReadOffset < WriteOffset)
		{
			ReadSize = WriteOffset - ReadOffset;
			pBuffer->Copy (m_pData + ReadOffset, ReadSize);
		}
		else
		{
			size_t Size1 = EndOffset - ReadOffset;
			size_t Size2 = WriteOffset;
			ReadSize = Size1 + Size2;

			pBuffer->SetCount (ReadSize);

			if (Size1)
				memcpy (*pBuffer, m_pData + ReadOffset, Size1);

			if (Size2)
				memcpy (*pBuffer + Size1, m_pData + 1, Size2);
		}

		mt::AtomicLock (&m_pHdr->m_Lock);
		m_pHdr->m_ReadOffset = WriteOffset;
	}

	m_pHdr->m_DataSize -= ReadSize;
	mt::AtomicUnlock (&m_pHdr->m_Lock);

#if (_AXL_ENV == AXL_ENV_WIN)
	m_ReadEvent.Signal ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	m_ReadEvent.Post ();
#endif

	return ReadSize;
}

//.............................................................................

bool
CSharedMemoryWriter::Open (
	const char* pFileName,
	const char* pReadEventName,
	const char* pWriteEventName,
	uint_t Flags,
	size_t SizeLimitHint
	)
{
	Close ();
	m_SizeLimitHint = SizeLimitHint;
	return CSharedMemoryTransportBase::Open (pFileName, pReadEventName, pWriteEventName, Flags);
}

size_t
CSharedMemoryWriter::Write (
	const void* const* pBlockArray,
	const size_t* pSizeArray,
	size_t Count
	)
{
	ASSERT (IsOpen ());

	size_t ChainSize = 0;

	for (size_t i = 0; i < Count; i++)
		ChainSize += pSizeArray [i];

	if (!ChainSize)
		return 0;

	size_t WriteSize = ChainSize;

	if (m_Flags & ESharedMemoryTransportFlag_Message)
		WriteSize += sizeof (TSharedMemoryTransportMessageHdr);

	mt::CScopeLock ScopeLock (&m_WriteLock); // ensure atomic write

	mt::AtomicLock (&m_pHdr->m_Lock);

	// if buffer is wrapped, then wait until we have enough space

	while (
		m_pHdr->m_WriteOffset <= m_pHdr->m_ReadOffset && m_pHdr->m_DataSize && // wrapped
		m_pHdr->m_WriteOffset + WriteSize > m_pHdr->m_ReadOffset &&
		m_pHdr->m_State != ESharedMemoryTransportState_Disconnected
		)
	{
		mt::AtomicUnlock (&m_pHdr->m_Lock);
		m_ReadEvent.Wait ();
		mt::AtomicLock (&m_pHdr->m_Lock);
	}

	if (m_pHdr->m_State == ESharedMemoryTransportState_Disconnected)
	{
		mt::AtomicUnlock (&m_pHdr->m_Lock);
		err::SetError (err::EStatus_InvalidDeviceState);
		return -1;
	}

	size_t ReadOffset = m_pHdr->m_ReadOffset;
	size_t WriteOffset = m_pHdr->m_WriteOffset;
	size_t DataSize = m_pHdr->m_DataSize;

	mt::AtomicUnlock (&m_pHdr->m_Lock);

	size_t WriteEndOffset = WriteOffset + WriteSize;

	EnsureOffsetMapped (WriteEndOffset);

	if (m_Flags & ESharedMemoryTransportFlag_Message)
	{
		TSharedMemoryTransportMessageHdr* pMsgHdr = (TSharedMemoryTransportMessageHdr*) (m_pData + WriteOffset);
		pMsgHdr->m_Signature = ESharedMemoryTransport_MessageSignature;
		pMsgHdr->m_Size = ChainSize;
		CopyWriteChain (pMsgHdr + 1, pBlockArray, pSizeArray, Count);
	}
	else
	{
		CopyWriteChain (m_pData + WriteOffset, pBlockArray, pSizeArray, Count);
	}

	mt::AtomicLock (&m_pHdr->m_Lock);

	if (m_pHdr->m_WriteOffset <= m_pHdr->m_ReadOffset && m_pHdr->m_DataSize) // wrapped
	{
		m_pHdr->m_WriteOffset = WriteEndOffset;
	}
	else
	{
		m_pHdr->m_WriteOffset = m_pHdr->m_DataSize > m_SizeLimitHint ? 0 : WriteEndOffset;
		m_pHdr->m_EndOffset = WriteEndOffset;
	}

	m_pHdr->m_DataSize += ChainSize;
	mt::AtomicUnlock (&m_pHdr->m_Lock);

#if (_AXL_ENV == AXL_ENV_WIN)
	m_WriteEvent.Signal ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	m_WriteEvent.Post ();
#endif

	return ChainSize;
}

void
CSharedMemoryWriter::CopyWriteChain (
	void* _pDst,
	const void* const* pBlockArray,
	const size_t* pSizeArray,
	size_t Count
	)
{
	char* pDst = (char*) _pDst;

	for (size_t i = 0; i < Count; i++)
	{
		const void* pSrc = pBlockArray [i];
		size_t Size = pSizeArray [i];

		memcpy (pDst, pSrc, Size);
		pDst += Size;
	}
}

//.............................................................................

} // namespace axl {
} // namespace io {
