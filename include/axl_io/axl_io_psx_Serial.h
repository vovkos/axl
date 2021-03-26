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

#pragma once

#define _AXL_PSX_SERIAL_H

#include "axl_io_psx_File.h"

#if (defined TCGETS2 && defined TCGETS2)
#	define _AXL_IO_PSX_TERMIOS2 1
#	define NCCS2 19 // for x86, amd64, arm32, arm64

struct termios2
{
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_line;
	cc_t c_cc[NCCS2];
	speed_t c_ispeed;
	speed_t c_ospeed;
};

#endif

namespace axl {
namespace io {
namespace psx {

//..............................................................................

class Serial: public File
{
public:
	bool
	open(
		const sl::StringRef& name,
		uint_t openFlags = O_RDWR | O_NOCTTY
		)
	{
		return File::open(name, openFlags, 0);
	}

	bool
	getAttr(termios* attr) const
	{
		int result = ::tcgetattr(m_h, attr);
		return err::complete(result != -1);
	}

	bool
	setAttr(
		const termios* attr,
		int actions = TCSANOW
		)
	{
		int result = ::tcsetattr(m_h, actions, attr);
		return err::complete(result != -1);
	}

#if (_AXL_IO_PSX_TERMIOS2)
	bool
	getAttr(termios2* attr) const
	{
		int result = ::ioctl(m_h, TCGETS2, attr);
		return err::complete(result != -1);
	}

	bool
	setAttr(const termios2* attr)
	{
		int result = ::ioctl(m_h, TCSETS2, attr);
		return err::complete(result != -1);
	}
#endif

	bool
	drain()
	{
		int result = ::tcdrain(m_h);
		return err::complete(result != -1);
	}

	bool
	flush(int queueSelector = TCIOFLUSH)
	{
		int result = ::tcflush(m_h, queueSelector);
		return err::complete(result != -1);
	}

	bool
	flow(int action)
	{
		int result = ::tcflow(m_h, action);
		return err::complete(result != -1);
	}

	uint_t
	getStatusLines() const;

	bool
	setDtr(bool isSet);

	bool
	setRts(bool isSet);

	bool
	setBreakCondition(bool isSet);

	bool
	sendBreak(uint_t duration = 0)
	{
		int result = ::tcsendbreak(m_h, duration);
		return err::complete(result != -1);
	}

#if (!_AXL_OS_DARWIN)
	bool
	wait(uint_t mask)
	{
		int result = ::ioctl(m_h, TIOCMIWAIT, mask);
		return err::complete(result != -1);
	}
#endif
};

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
