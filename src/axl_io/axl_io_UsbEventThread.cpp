//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_io_UsbEventThread.h"
#include "axl_io_UsbContext.h"

namespace axl {
namespace io {

//..............................................................................

bool
UsbEventThread::start (libusb_context* context)
{
	stop ();

	m_context = context;
	m_isTerminating = false;

	return ThreadImpl <UsbEventThread>::start ();
}

void
UsbEventThread::threadFunc ()
{
	UsbContext context;
	context.attach (m_context);

	while (!m_isTerminating)
		context.handleEvents (TimerGranularity);

	context.detach ();
}

//..............................................................................

} // namespace io
} // namespace axl
