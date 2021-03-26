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
#include "axl_io_Serial.h"
#include "axl_sys_Event.h"

#include "axl_enc_HexEncoding.h"

namespace axl {
namespace io {

//..............................................................................

#if (_AXL_OS_WIN)

bool
Serial::open(
	const sl::StringRef& name,
	uint_t flags
	)
{
	uint_t accessMode =
		(flags & FileFlag_ReadOnly) ? GENERIC_READ :
		(flags & FileFlag_WriteOnly) ? GENERIC_WRITE : GENERIC_READ | GENERIC_WRITE;

	uint_t flagsAttributes = (flags & FileFlag_Asynchronous) ? FILE_FLAG_OVERLAPPED : 0;

	return m_serial.open(name, accessMode, flagsAttributes);
}

bool
Serial::setSettings(
	const SerialSettings* settings,
	uint_t mask
	)
{
	DCB dcb;
	dcb.DCBlength = sizeof(dcb);

	bool result = m_serial.getSettings(&dcb);
	if (!result)
		return false;

	dcb.fBinary = TRUE;

	if (mask & SerialSettingId_BaudRate)
		dcb.BaudRate = settings->m_baudRate;

	if (mask & SerialSettingId_DataBits)
		dcb.ByteSize = settings->m_dataBits;

	if (mask & SerialSettingId_StopBits)
		dcb.StopBits = settings->m_stopBits;

	if (mask & SerialSettingId_Parity)
	{
		dcb.fParity = settings->m_parity != SerialParity_None;
		dcb.Parity = settings->m_parity;
	}

	if (mask & SerialSettingId_FlowControl)
	{
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		dcb.fDsrSensitivity = FALSE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fDtrControl = settings->m_dtr ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
		dcb.fRtsControl = settings->m_rts ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;

		switch (settings->m_flowControl)
		{
		case SerialFlowControl_RtsCts:
			dcb.fOutxCtsFlow = TRUE;
			dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
			break;

		case SerialFlowControl_XonXoff:
			dcb.fOutX = TRUE;
			dcb.fInX = TRUE;
			break;
		}
	}

	if (!(mask & SerialSettingId_ReadInterval))
		return m_serial.setSettings(&dcb);

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout =
		settings->m_readInterval == 0 ? -1 :
		settings->m_readInterval == -1 ? 0 :
		settings->m_readInterval;

	return
		m_serial.setSettings(&dcb) &&
		m_serial.setTimeouts(&timeouts);
}

bool
Serial::getSettings(SerialSettings* settings)
{
	DCB dcb;
	dcb.DCBlength = sizeof(dcb);

	bool result = m_serial.getSettings(&dcb);
	if (!result)
		return false;

	settings->setDcb(&dcb);
	return true;
}

uint_t
Serial::getStatusLines()
{
	uint_t lines = m_serial.getStatusLines();
	return lines != -1 ? (lines & 0xf0) >> 4 : -1;
}

#elif (_AXL_OS_POSIX)

bool
Serial::open(
	const sl::StringRef& name,
	uint_t flags
	)
{
	uint_t posixFlags =
		(flags & FileFlag_ReadOnly) ? O_RDONLY :
		(flags & FileFlag_WriteOnly) ? O_WRONLY : O_RDWR;

	if (flags & FileFlag_Asynchronous)
		posixFlags |= O_NONBLOCK;

	posixFlags |= O_NOCTTY;

	return m_serial.open(name, posixFlags);
}

bool
Serial::setSettings(
	const SerialSettings* settings,
	uint_t mask
	)
{
#if (_AXL_IO_PSX_TERMIOS2)
	termios2 attr;
#else
	termios attr;
#endif

	bool result = m_serial.getAttr(&attr);
	if (!result)
		return false;

	speed_t stdSpeed = -1;

	if (mask & SerialSettingId_BaudRate)
	{
		switch (settings->m_baudRate)
		{
		case 110:
			stdSpeed = B110;
			break;

		case 300:
			stdSpeed = B300;
			break;

		case 600:
			stdSpeed = B600;
			break;

		case 1200:
			stdSpeed = B1200;
			break;

		case 2400:
			stdSpeed = B2400;
			break;

		case 4800:
			stdSpeed = B4800;
			break;

		case 9600:
			stdSpeed = B9600;
			break;

		case 19200:
			stdSpeed = B19200;
			break;

		case 38400:
			stdSpeed = B38400;
			break;

		case 57600:
			stdSpeed = B57600;
			break;

		case 115200:
			stdSpeed = B115200;
			break;

#ifdef B230400
		case 230400:
			stdSpeed = B230400;
			break;
#endif
#ifdef B460800
		case 460800:
			stdSpeed = B460800;
			break;
#endif
#ifdef B500000
		case 500000:
			stdSpeed = B500000;
			break;
#endif
#ifdef B576000
		case 576000:
			stdSpeed = B576000;
			break;
#endif
#ifdef B921600
		case 921600:
			stdSpeed = B921600;
			break;
#endif
#ifdef B1000000
		case 1000000:
			stdSpeed = B1000000;
			break;
#endif
#ifdef B1152000
		case 1152000:
			stdSpeed = B1152000;
			break;
#endif
#ifdef B1500000
		case 1500000:
			stdSpeed = B1500000;
			break;
#endif
#ifdef B2000000
		case 2000000:
			stdSpeed = B2000000;
			break;
#endif
#ifdef B2500000
		case 2500000:
			stdSpeed = B2500000;
			break;
#endif
#ifdef B3000000
		case 3000000:
			stdSpeed = B3000000;
			break;
#endif
#ifdef B3500000
		case 3500000:
			stdSpeed = B3500000;
			break;
#endif
#ifdef B4000000
		case 4000000:
			stdSpeed = B4000000;
			break;
#endif
		default:
			stdSpeed = 0;

#if (_AXL_IO_PSX_TERMIOS2)
			attr.c_cflag &= ~CBAUD;
			attr.c_cflag |= CBAUDEX;
			attr.c_ispeed = settings->m_baudRate;
			attr.c_ospeed = settings->m_baudRate;
#elif (!defined IOSSIOSPEED)
			return err::fail(err::SystemErrorCode_InvalidParameter);
#endif
		}

		if (stdSpeed != 0)
		{
#if (_AXL_IO_PSX_TERMIOS2)
			attr.c_cflag &= ~(CBAUD | CBAUDEX);
			attr.c_cflag |= stdSpeed;
			attr.c_ispeed = stdSpeed;
			attr.c_ospeed = stdSpeed;
#else
			cfsetispeed(&attr, stdSpeed);
			cfsetospeed(&attr, stdSpeed);
#endif
		}
	}

	if (mask & SerialSettingId_DataBits)
	{
		attr.c_cflag &= ~CSIZE;

		switch (settings->m_dataBits)
		{
		case 5:
			attr.c_cflag |= CS5;
			break;

		case 6:
			attr.c_cflag |= CS6;
			break;

		case 7:
			attr.c_cflag |= CS7;
			break;

		case 8:
		default:
			attr.c_cflag |= CS8;
			break;
		}
	}

	if (mask & SerialSettingId_StopBits)
	{
		if (settings->m_stopBits == SerialStopBits_2)
			attr.c_cflag |= CSTOPB;
		else
			attr.c_cflag &= ~CSTOPB;
	}

	if (mask & SerialSettingId_Parity)
	{
		attr.c_iflag &= ~(PARMRK | INPCK);
		attr.c_iflag |= IGNPAR;

		switch (settings->m_parity)
		{
		case SerialParity_None:
			attr.c_cflag &= ~PARENB;
			break;

		case SerialParity_Odd:
			attr.c_cflag |= PARENB | PARODD;
			break;

		case SerialParity_Even:
			attr.c_cflag &= ~PARODD;
			attr.c_cflag |= PARENB;
			break;

#ifdef CMSPAR
		case SerialParity_Mark:
			attr.c_cflag |= PARENB | CMSPAR | PARODD;
			break;

		case SerialParity_Space:
			attr.c_cflag &= ~PARODD;
			attr.c_cflag |= PARENB | CMSPAR;
			break;
#endif

		default:
			attr.c_cflag |= PARENB;
			attr.c_iflag |= PARMRK | INPCK;
			attr.c_iflag &= ~IGNPAR;
		}
	}

	if (mask & SerialSettingId_FlowControl)
	{
		attr.c_cflag &= ~CRTSCTS;
		attr.c_iflag &= ~(IXON | IXOFF | IXANY);

		switch (settings->m_flowControl)
		{
		case SerialFlowControl_RtsCts:
			attr.c_cflag |= CRTSCTS;
			break;

		case SerialFlowControl_XonXoff:
			attr.c_iflag |= IXON | IXOFF | IXANY;
			break;
		}
	}

	// ensure some extra default flags

	attr.c_iflag |= IGNBRK;
	attr.c_iflag &= ~(BRKINT | IGNCR | INLCR | ICRNL | ISTRIP);
	attr.c_oflag = 0;
	attr.c_cflag |= CREAD | CLOCAL;
	attr.c_lflag = 0;

	memset(attr.c_cc, _POSIX_VDISABLE, sizeof(attr.c_cc));
	attr.c_cc[VTIME] = settings->m_readInterval / 100; // milliseconds -> deciseconds
	attr.c_cc[VMIN]  = 1;

	result = m_serial.setAttr(&attr);
	if (!result)
		return false;

#ifdef IOSSIOSPEED
	if (!stdSpeed)
	{
		speed_t speed = settings->m_baudRate;
		result = m_serial.ioctl(IOSSIOSPEED, &speed) != -1;
		if (!result)
			return false;
	}
#endif

	if (mask & SerialSettingId_FlowControl) // also, adjust DTR & RTS lines
	{
		result = m_serial.setDtr(settings->m_dtr);
		if (!result)
			return false;

		if (settings->m_flowControl != SerialFlowControl_RtsCts)
		{
			result = m_serial.setRts(settings->m_rts);
			if (!result)
				return false;
		}
	}

	return true;
}

bool
Serial::getSettings(SerialSettings* settings)
{
	termios attr;
	bool result = m_serial.getAttr(&attr);
	if (!result)
		return false;

	settings->setAttr(&attr);
	return true;
}

uint_t
Serial::getStatusLines()
{
	uint_t result = m_serial.getStatusLines();
	if (result == -1)
		return -1;

	uint_t lines = 0;
	if (result & TIOCM_CTS)
		lines |= SerialStatusLine_Cts;

	if (result & TIOCM_DSR)
		lines |= SerialStatusLine_Dsr;

	if (result & TIOCM_RNG)
		lines |= SerialStatusLine_Ring;

	if (result & TIOCM_CAR)
		lines |= SerialStatusLine_Dcd;

	return lines;
}

#endif

//..............................................................................

} // namespace io
} // namespace axl
