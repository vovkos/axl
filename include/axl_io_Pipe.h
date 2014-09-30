// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	g::win::CFileHandle m_socket;

protected:
	bool open (
		const char* name,
		SECURITY_DESCRIPTOR* sd
		);

	void 
	close ();
};

//.............................................................................

class CConnectionPipe;

class CListenerPipe: public CPipe
{
protected:
	rtl::CArrayT <CConnectionPipe*> m_backLog;
	exe::CFunction m_onAccept;
	ref::CPtrT <exe::CWorkerThread> m_workerThread;

public:
	bool
	listen (
		const char* name,
		SECURITY_DESCRIPTOR* secDescr,
		size_t backLog,
		const exe::CFunction& onAccept
		);
};

//.............................................................................

class CConnectionPipe: public CPipe
{
protected:
	exe::CFunction m_onConnectComplete;
	exe::CFunction m_onDisconnectComplete;	

public:
	bool 
	connect (
		const char* address,
		uint_t timeout,
		const exe::CFunction& onComplete // void OnComplete (err::CError* pError)
		);

	bool 
	disconnect (
		uint_t timeout,
		const exe::CFunction& onComplete // void OnComplete (err::CError* pError)
		);

	bool 
	syncConnect (
		const char* address,
		uint_t timeout
		);

	bool 
	syncDisconnect (uint_t timeout);

	bool
	isConnected ();

	virtual
	bool
	send (
		const void* p,
		size_t size,
		const exe::CFunction& onComplete // void OnComplete (size_t ActualSize);
		);

	virtual
	bool
	recv (
		void* p,
		size_t size,
		const exe::CFunction& onComplete // void OnComplete (size_t ActualSize);
		);
};

//.............................................................................

} // namespace io
} // namespace axl
