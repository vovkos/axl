// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_TRANSPORT_H

#include "axl_exe_Function.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

struct ITransport: obj::IRoot
{
	// {A79E97FE-C6F7-4DD8-A900-D9360B9D42CE}
	AXL_OBJ_INTERFACE (
		ITransport,
		0xa79e97fe, 0xc6f7, 0x4dd8, 0xa9, 0x00, 0xd9, 0x36, 0x0b, 0x9d, 0x42, 0xce
		)
		
	typedef exe::CArgSeqExT_2 <exe::CArgObjPtrT <err::CError>, exe::CArgT <size_t> > COnSendCompleteArg;
	typedef exe::CArgSeqExT_2 <exe::CArgObjPtrT <err::CError>, exe::CArgT <size_t> > COnRecvCompleteArg;

	virtual
	bool
	Send (
		const void* p,
		size_t Size,
		exe::IFunction* pOnComplete // void OnComplete (const err::CError& Error, size_t ActualSize);
		) = 0;

	virtual
	bool
	Recv (
		void* p,
		size_t Size,
		exe::IFunction* pOnComplete // void OnComplete (const err::CError& Error, size_t ActualSize);
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

//.............................................................................

} // namespace io
} // namespace axl

