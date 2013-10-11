// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_TRANSPORT_H

// #include "axl_exe_Function.h"

namespace axl {
namespace io {

//.............................................................................

class CTransport
{
public:
	virtual
	size_t
	Transmit (
		const void* p,
		size_t Size
		) = 0;

	virtual
	size_t
	Receive (
		void* p,
		size_t Size
		) = 0;
};

/*

//.............................................................................

class CAsyncTransport: public CTransport
{
	typedef exe::CArgSeqExT_2 <exe::CArgObjPtrT <err::CError>, exe::CArgT <size_t> > COnSendCompleteArg;
	typedef exe::CArgSeqExT_2 <exe::CArgObjPtrT <err::CError>, exe::CArgT <size_t> > COnRecvCompleteArg;

	virtual
	bool
	Send (
		const void* p,
		size_t Size,
		const exe::CFunction& OnComplete // void OnComplete (const err::CError& Error, size_t ActualSize);
		) = 0;

	virtual
	bool
	Recv (
		void* p,
		size_t Size,
		const exe::CFunction& OnComplete // void OnComplete (const err::CError& Error, size_t ActualSize);
		) = 0;

	size_t
	SyncSend (
		const void* p,
		size_t Size
		);

	size_t
	SyncRecv (
		void* p,
		size_t Size
		);
};

*/

//.............................................................................

} // namespace io
} // namespace axl

