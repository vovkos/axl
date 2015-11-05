// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILETRANSPORT_H

#include "axl_io_Transport.h"
#include "axl_io_File.h"
#include "axl_sl_Handle.h"

namespace axl {
namespace io {

//.............................................................................

class Fileransport: public Transport
{
protected:
	File* m_transmitFile;
	File* m_receiveFile;

public:
	Fileransport ()
	{
		m_transmitFile = NULL;
		m_receiveFile = NULL;
	}

	void
	attach (File* file)
	{
		m_transmitFile = file;
		m_receiveFile = file;
	}

	void
	attach (
		File* transmitFile,
		File* receiveFile
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

