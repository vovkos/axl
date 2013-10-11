// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILETRANSPORT_H

#include "axl_io_Transport.h"
#include "axl_io_File.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace io {

//.............................................................................

class CFileTransport: public CTransport
{
protected:
	CFile* m_pTransmitFile;
	CFile* m_pReceiveFile;

public:
	CFileTransport ()
	{
		m_pTransmitFile = NULL;
		m_pReceiveFile = NULL;
	}

	void
	Attach (CFile* pFile)
	{
		m_pTransmitFile = pFile;
		m_pReceiveFile = pFile;
	}

	void
	Attach (
		CFile* pTransmitFile,
		CFile* pReceiveFile
		)
	{
		m_pTransmitFile = pTransmitFile;
		m_pReceiveFile = pReceiveFile;
	}

	void
	Detach ()
	{
		m_pTransmitFile = NULL;
		m_pReceiveFile = NULL;
	}

	virtual
	size_t
	Transmit (
		const void* p,
		size_t Size
		)
	{
		ASSERT (m_pTransmitFile);
		return m_pTransmitFile->Write (p, Size);
	}

	virtual
	size_t
	Receive (
		void* p,
		size_t Size
		)
	{
		ASSERT (m_pReceiveFile);
		return m_pReceiveFile->Read (p, Size);
	}
};

//.............................................................................

} // namespace io
} // namespace axl

