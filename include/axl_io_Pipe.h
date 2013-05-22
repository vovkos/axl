// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PIPE_H

#include "axl_g_win_Handle.h"
#include "axl_io_Transport.h"
#include "axl_exe_WorkerThread.h"

namespace axl {
namespace io {

//.............................................................................

// although pipe semantics is basically identical to socket semantics
// i decided to split socket and pipes (in prev axl pipe was one of socket protocols)
// the reasons are: 
// - cleaner mapping to underlying OS objects
// - avoiding a layer of polymorphism, be it virtual (like in prev axl) or templated
// - being able to fit any socket address into a fixed-size union

//.............................................................................

class CPipe
{
protected:
	g::win::CFileHandle m_Socket;

protected:
	bool Open (
		const char* pName,
		SECURITY_DESCRIPTOR* pSd
		);

	void 
	Close ();
};

//.............................................................................

class CConnectionPipe;

class CListenerPipe: public CPipe
{
protected:
	rtl::CArrayT <CConnectionPipe*> m_BackLog;
	ref::CPtrT <exe::IFunction> m_OnAccept;
	ref::CPtrT <exe::CWorkerThread> m_WorkerThread;

public:
	bool
	Listen (
		const char* pName,
		SECURITY_DESCRIPTOR* pSecDescr,
		size_t BackLog,
		exe::IFunction* pOnAccept
		);
};

//.............................................................................

class CConnectionPipe: 
	public CPipe,
	public io::ITransport
{
protected:
	ref::CPtrT <exe::IFunction> m_OnConnectComplete;
	ref::CPtrT <exe::IFunction> m_OnDisconnectComplete;	

public:
	bool 
	Connect (
		const char* pAddress,
		uint_t Timeout,
		exe::IFunction* pOnComplete // void OnComplete (err::CError* pError)
		);

	bool 
	Disconnect (
		uint_t Timeout,
		exe::IFunction* pOnComplete // void OnComplete (err::CError* pError)
		);

	bool 
	SyncConnect (
		const char* pAddress,
		uint_t Timeout
		);

	bool 
	SyncDisconnect (uint_t Timeout);

	bool
	IsConnected ();

	virtual
	bool
	Send (
		const void* p,
		size_t Size,
		exe::IFunction* pOnComplete // void OnComplete (size_t ActualSize);
		);

	virtual
	bool
	Recv (
		void* p,
		size_t Size,
		exe::IFunction* pOnComplete // void OnComplete (size_t ActualSize);
		);
};

//.............................................................................

} // namespace io
} // namespace axl
