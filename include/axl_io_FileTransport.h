// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	CFile* m_transmitFile;
	CFile* m_receiveFile;

public:
	CFileTransport ()
	{
		m_transmitFile = NULL;
		m_receiveFile = NULL;
	}

	void
	attach (CFile* file)
	{
		m_transmitFile = file;
		m_receiveFile = file;
	}

	void
	attach (
		CFile* transmitFile,
		CFile* receiveFile
		)
	{
		m_transmitFile = transmitFile;
		m_receiveFile = receiveFile;
	}

	void
	detach ()
	{
		m_transmitFile = NULL;
		m_receiveFile = NULL;
	}

	virtual
	size_t
	transmit (
		const void* p,
		size_t size
		)
	{
		ASSERT (m_transmitFile);
		return m_transmitFile->write (p, size);
	}

	virtual
	size_t
	receive (
		void* p,
		size_t size
		)
	{
		ASSERT (m_receiveFile);
		return m_receiveFile->read (p, size);
	}
};

//.............................................................................

} // namespace io
} // namespace axl

