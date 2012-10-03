// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_TRANSPORT_H

#include "axl_exe_Function.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

[uuid ("47832043-6148-40cb-958a-5e1c51b36c21")]
struct ITransport: obj::IRoot
{
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

