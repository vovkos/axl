// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_TRANSPORT_H

// #include "axl_exe_Function.h"

namespace axl {
namespace io {

//.............................................................................

class Transport
{
public:
	virtual
	size_t
	transmit (
		const void* p,
		size_t size
		) = 0;

	virtual
	size_t
	receive (
		void* p,
		size_t size
		) = 0;
};

/*

//.............................................................................

class Asyncransport: public Transport
{
	typedef exe::ArgSeqEx_2 <exe::ArgObjPtr <err::Error>, exe::Arg <size_t> > OnSendCompleteArg;
	typedef exe::ArgSeqEx_2 <exe::ArgObjPtr <err::Error>, exe::Arg <size_t> > OnRecvCompleteArg;

	virtual
	bool
	send (
		const void* p,
		size_t size,
		const exe::Function& onComplete // void OnComplete (const err::CError& Error, size_t ActualSize);
		) = 0;

	virtual
	bool
	recv (
		void* p,
		size_t size,
		const exe::Function& onComplete // void OnComplete (const err::CError& Error, size_t ActualSize);
		) = 0;

	size_t
	syncSend (
		const void* p,
		size_t size
		);

	size_t
	syncRecv (
		void* p,
		size_t size
		);
};

*/

//.............................................................................

} // namespace io
} // namespace axl

