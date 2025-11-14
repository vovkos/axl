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
) {
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
) {
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

	if (mask & SerialSettingId_Parity) {
		dcb.fParity = settings->m_parity != SerialParity_None;
		dcb.Parity = settings->m_parity;
	}

	if (mask & SerialSettingId_FlowControl) {
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		dcb.fDsrSensitivity = FALSE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fDtrControl = settings->m_dtr ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
		dcb.fRtsControl = settings->m_rts ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;

		switch (settings->m_flowControl) {
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
Serial::getSettings(SerialSettings* settings) {
	DCB dcb;
	dcb.DCBlength = sizeof(dcb);

	bool result = m_serial.getSettings(&dcb);
	if (!result)
		return false;

	settings->setDcb(&dcb);
	return true;
}

uint_t
Serial::getStatusLines() {
	uint_t lines = m_serial.getStatusLines();
	return lines != -1 ? (lines & 0xf0) >> 4 : -1;
}

#elif (_AXL_OS_POSIX)

bool
Serial::open(
	const sl::StringRef& name,
	uint_t flags
) {
	uint_t posixFlags =
		(flags & FileFlag_ReadOnly) ? O_RDONLY :
		(flags & FileFlag_WriteOnly) ? O_WRONLY : O_RDWR;

	if (flags & FileFlag_Asynchronous)
		posixFlags |= O_NONBLOCK;

	posixFlags |= O_NOCTTY;

	return m_serial.open(name, posixFlags);
}

template <typename T>
bool
prepareTermios(
	T* attr,
	const SerialSettings* settings,
	uint_t mask
) {
	if (mask & SerialSettingId_DataBits) {
		attr->c_cflag &= ~CSIZE;

		switch (settings->m_dataBits) {
		case 5:
			attr->c_cflag |= CS5;
			break;

		case 6:
			attr->c_cflag |= CS6;
			break;

		case 7:
			attr->c_cflag |= CS7;
			break;

		case 8:
		default:
			attr->c_cflag |= CS8;
			break;
		}
	}

	if (mask & SerialSettingId_StopBits) {
		if (settings->m_stopBits == SerialStopBits_2)
			attr->c_cflag |= CSTOPB;
		else
			attr->c_cflag &= ~CSTOPB;
	}

	if (mask & SerialSettingId_Parity) {
#ifdef CMSPAR
		attr->c_cflag &= ~(PARENB | PARODD | CMSPAR);
#else
		attr->c_cflag &= ~(PARENB | PARODD);
#endif
		attr->c_iflag &= ~(PARMRK | INPCK);
		attr->c_iflag |= IGNPAR;

		switch (settings->m_parity) {
		case SerialParity_Odd:
			attr->c_cflag |= PARENB | PARODD;
			break;

		case SerialParity_Even:
			attr->c_cflag |= PARENB;
			break;

#ifdef CMSPAR
		case SerialParity_Mark:
			attr->c_cflag |= PARENB | CMSPAR | PARODD;
			break;

		case SerialParity_Space:
			attr->c_cflag |= PARENB | CMSPAR;
			break;
#else
		case SerialParity_Mark:
		case SerialParity_Space:
			err::setError(err::SystemErrorCode_NotImplemented);
			return false;
#endif
		}
	}

	if (mask & SerialSettingId_FlowControl) {
		attr->c_cflag &= ~CRTSCTS;
		attr->c_iflag &= ~(IXON | IXOFF | IXANY);

		switch (settings->m_flowControl) {
		case SerialFlowControl_RtsCts:
			attr->c_cflag |= CRTSCTS;
			break;

		case SerialFlowControl_XonXoff:
			attr->c_iflag |= IXON | IXOFF | IXANY;
			break;
		}
	}

	// ensure some extra default flags

	attr->c_iflag |= IGNBRK;
	attr->c_iflag &= ~(BRKINT | IGNCR | INLCR | ICRNL | ISTRIP);
	attr->c_oflag = 0;
	attr->c_cflag |= CREAD | CLOCAL;
	attr->c_lflag = 0;

	memset(attr->c_cc, _POSIX_VDISABLE, sizeof(attr->c_cc));
	attr->c_cc[VTIME] = settings->m_readInterval / 100; // milliseconds -> deciseconds
	attr->c_cc[VMIN]  = 1;
	return true;
}

bool
setTermiosSpeed(
	termios* attr,
	speed_t speed
) {
	return err::complete(
		cfsetispeed(attr, speed) != -1 &&
		cfsetospeed(attr, speed) != -1
	);
}

#if (_AXL_IO_PSX_TERMIOS2)
bool
setTermiosSpeed(
	termios2* attr,
	uint_t speed
) {
	attr->c_cflag &= ~CBAUD;
#ifdef BOTHER
	attr->c_cflag |= BOTHER;
#else
	attr->c_cflag |= CBAUDEX;
#endif
	attr->c_ispeed = speed;
	attr->c_ospeed = speed;
	return true;
}
#endif

speed_t
getTermiosStdSpeed(uint_t baudRate) {
	switch (baudRate) {
	case 110:
		return B110;

	case 300:
		return B300;

	case 600:
		return B600;

	case 1200:
		return B1200;

	case 2400:
		return B2400;

	case 4800:
		return B4800;

	case 9600:
		return B9600;

	case 19200:
		return B19200;

	case 38400:
		return B38400;

	case 57600:
		return B57600;

	case 115200:
		return B115200;

#ifdef B230400
	case 230400:
		return B230400;
#endif
#ifdef B460800
	case 460800:
		return B460800;
#endif
#ifdef B500000
	case 500000:
		return B500000;
#endif
#ifdef B576000
	case 576000:
		return B576000;
#endif
#ifdef B921600
	case 921600:
		return B921600;
#endif
#ifdef B1000000
	case 1000000:
		return B1000000;
#endif
#ifdef B1152000
	case 1152000:
		return B1152000;
#endif
#ifdef B1500000
	case 1500000:
		return B1500000;
#endif
#ifdef B2000000
	case 2000000:
		return B2000000;
#endif
#ifdef B2500000
	case 2500000:
		return B2500000;
#endif
#ifdef B3000000
	case 3000000:
		return B3000000;
#endif
#ifdef B3500000
	case 3500000:
		return B3500000;
#endif
#ifdef B4000000
	case 4000000:
		return B4000000;
#endif

	default:
		return 0;
	}
}

bool
Serial::setSettings(
	const SerialSettings* settings,
	uint_t mask
) {
	bool result = false;

	if (!(mask & SerialSettingId_BaudRate)) {
		termios attr;
		result =
			m_serial.getAttr(&attr) &&
			prepareTermios(&attr, settings, mask) &&
			m_serial.setAttr(&attr);
	} else if (speed_t stdSpeed = getTermiosStdSpeed(settings->m_baudRate)) {
		termios attr;
		result =
			m_serial.getAttr(&attr) &&
			prepareTermios(&attr, settings, mask) &&
			setTermiosSpeed(&attr, stdSpeed) &&
			m_serial.setAttr(&attr);
	} else {
#if (_AXL_IO_PSX_TERMIOS2)
		termios2 attr;
		result =
			m_serial.getAttr(&attr) &&
			prepareTermios(&attr, settings, mask) &&
			setTermiosSpeed(&attr, settings->m_baudRate) &&
			m_serial.setAttr(&attr);
#elif (defined IOSSIOSPEED)
		termios attr;
		speed_t speed = settings->m_baudRate;
		result =
			m_serial.getAttr(&attr) &&
			prepareTermios(&attr, settings, mask) &&
			m_serial.setAttr(&attr) &&;
			m_serial.ioctl(IOSSIOSPEED, &speed) != -1;
#else
		return err::fail(err::SystemErrorCode_InvalidParameter);
#endif
	}

	if (!result)
		return false;

	if (mask & SerialSettingId_FlowControl) {
		// adjust DTR & RTS lines

		result =
			m_serial.setDtr(settings->m_dtr) &&
			m_serial.setRts(
				// most drivers don't auto-adjust RTS line with CRTSSTS
				// enable it manually when RTS/CTS control is requested
				// otherwise, the other side will never be able to send
				settings->m_flowControl == SerialFlowControl_RtsCts ||
				settings->m_rts
			);

		if (!result)
			return false;
	}

	return true;
}

bool
Serial::getSettings(SerialSettings* settings) {
	termios attr;
	bool result = m_serial.getAttr(&attr);
	if (!result)
		return false;

	settings->setAttr(&attr);
	return true;
}

uint_t
Serial::getStatusLines() {
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
