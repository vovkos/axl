#include "pch.h"
#include "axl_io_Serial.h"
#include "axl_mt_Event.h"

namespace axl {
namespace io {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

bool
CSerial::setSettings (
	const TSerialSettings* settings,
	uint_t mask
	)
{
	DCB dcb;
	dcb.DCBlength = sizeof (dcb);

	bool result = m_serial.getSettings (&dcb);
	if (!result)
		return false;

	dcb.fBinary = TRUE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;

	if (mask & ESerialSetting_BaudRate)
		dcb.BaudRate = settings->m_baudRate;

	if (mask & ESerialSetting_DataBits)
		dcb.ByteSize = settings->m_dataBits;

	if (mask & ESerialSetting_StopBits)
		dcb.StopBits = settings->m_stopBits;

	if (mask & ESerialSetting_Parity)
		dcb.fParity = settings->m_parity;

	if (mask & ESerialSetting_FlowControl)
		switch (settings->m_flowControl)
		{
		case ESerialFlowControl_None:
			dcb.fOutxCtsFlow = FALSE;
			dcb.fRtsControl = RTS_CONTROL_DISABLE;
			break;

		case ESerialFlowControl_RtsCts:
			dcb.fOutxCtsFlow = TRUE;
			dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
			break;

		case ESerialFlowControl_XonXoff:
			dcb.fOutX = TRUE;
			dcb.fInX = TRUE;
			break;
		}

	if (!(mask & ESerialSetting_ReadInterval))
		return m_serial.setSettings (&dcb);

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout =
		settings->m_readInterval == 0 ? -1 :
		settings->m_readInterval == -1 ? 0 :
		settings->m_readInterval;

	return
		m_serial.setSettings (&dcb) &&
		m_serial.setTimeouts (&timeouts);
}

bool
CSerial::getSettings (TSerialSettings* settings)
{
	DCB dcb;
	dcb.DCBlength = sizeof (dcb);

	bool result = m_serial.getSettings (&dcb);
	if (!result)
		return false;

	settings->m_baudRate = dcb.BaudRate;
	settings->m_dataBits = dcb.ByteSize;
	settings->m_stopBits = (ESerialStopBits) dcb.StopBits;
	settings->m_parity   = (ESerialParity) dcb.Parity;

	settings->m_flowControl =
		dcb.fOutxCtsFlow && dcb.fRtsControl == RTS_CONTROL_HANDSHAKE ? ESerialFlowControl_RtsCts :
		dcb.fOutX && dcb.fInX ? ESerialFlowControl_XonXoff : ESerialFlowControl_None;

	return true;
}

uint_t
CSerial::getStatusLines ()
{
	uint_t lines = m_serial.getStatusLines ();
	return lines != -1 ? (lines & 0xf0) >> 4 : -1;
}

size_t
CSerial::read (
	void* p,
	size_t size
	)
{
	mt::CEvent completionEvent;

	OVERLAPPED overlapped = { 0 };
	overlapped.hEvent = completionEvent.m_event;

	bool_t result = m_serial.read (p, (dword_t) size, NULL, &overlapped);
	if (!result)
		return -1;

	dword_t actualSize;
	result = ::GetOverlappedResult (m_serial, &overlapped, &actualSize, true);
	if (!result)
	{
		err::setLastSystemError ();
		return -1;
	}

	return actualSize;
}

size_t
CSerial::write (
	const void* p,
	size_t size
	)
{
	mt::CEvent completionEvent;

	OVERLAPPED overlapped = { 0 };
	overlapped.hEvent = completionEvent.m_event;

	bool_t result = m_serial.write (p, (dword_t) size, NULL, &overlapped);
	if (!result)
		return -1;

	dword_t actualSize;
	result = ::GetOverlappedResult (m_serial, &overlapped, &actualSize, true);
	if (!result)
	{
		err::setLastSystemError ();
		return -1;
	}

	return actualSize;
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

bool
CSerial::setSettings (
	const TSerialSettings* settings,
	uint_t mask
	)
{
	termios attr;
	bool result = m_serial.getAttr (&attr);
	if (!result)
		return false;

	if (mask & ESerialSetting_BaudRate)
	{
		speed_t speed;

		switch (settings->m_baudRate)
		{
		case 110:
			speed = B110;
			break;

		case 300:
			speed = B300;
			break;

		case 600:
			speed = B600;
			break;

		case 1200:
			speed = B1200;
			break;

		case 2400:
			speed = B2400;
			break;

		case 4800:
			speed = B4800;
			break;

		case 9600:
			speed = B9600;
			break;

		case 19200:
			speed = B19200;
			break;

		case 38400:
			speed = B38400;
			break;

		case 57600:
			speed = B57600;
			break;

		case 115200:
			speed = B115200;
			break;

		default:
			// TODO: custom baud rate (currently fall back to 38400)
			speed = B38400;
		}

		cfsetispeed (&attr, speed);
		cfsetospeed (&attr, speed);
	}

	if (mask & ESerialSetting_DataBits)
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

	if (mask & ESerialSetting_StopBits)
	{
		if (settings->m_stopBits == ESerialStopBits_2)
			attr.c_cflag |= CSTOPB;
		else
			attr.c_cflag &= ~CSTOPB;
	}

	if (mask & ESerialSetting_Parity)
	{
		attr.c_iflag &= ~(PARMRK | INPCK);
		attr.c_iflag |= IGNPAR;

		switch (settings->m_parity)
		{
		case ESerialParity_None:
			attr.c_cflag &= ~PARENB;
			break;

		case ESerialParity_Odd:
			attr.c_cflag |= PARENB | PARODD;
			break;

		case ESerialParity_Even:
			attr.c_cflag &= ~PARODD;
			attr.c_cflag |= PARENB;
			break;

		case ESerialParity_Mark:
			attr.c_cflag |= PARENB | CMSPAR | PARODD;
			break;

		case ESerialParity_Space:
			attr.c_cflag &= ~PARODD;
			attr.c_cflag |= PARENB | CMSPAR;
			break;

		default:
			attr.c_cflag |= PARENB;
			attr.c_iflag |= PARMRK | INPCK;
			attr.c_iflag &= ~IGNPAR;
		}
	}

	if (mask & ESerialSetting_FlowControl)
		switch (settings->m_flowControl)
		{
		case ESerialFlowControl_RtsCts:
			attr.c_cflag |= CRTSCTS;
			attr.c_iflag &= ~(IXON | IXOFF | IXANY);
			break;

		case ESerialFlowControl_XonXoff:
			attr.c_cflag &= ~CRTSCTS;
			attr.c_iflag |= IXON | IXOFF | IXANY;
			break;

		case ESerialFlowControl_None:
		default:
			attr.c_cflag &= ~CRTSCTS;
			attr.c_iflag &= ~(IXON | IXOFF | IXANY);
		}

	// ensure some extra default flags

	attr.c_cflag |= CREAD | CLOCAL;
	attr.c_lflag = 0;
	attr.c_oflag = 0;
	attr.c_cc [VTIME] = 0;
	attr.c_cc [VMIN]  = 1;

	return m_serial.setAttr (&attr);
}

bool
CSerial::getSettings (TSerialSettings* settings)
{
	termios attr;
	bool result = m_serial.getAttr (&attr);
	if (!result)
		return false;

	speed_t speed = cfgetispeed (&attr);
	switch (speed)
	{
	case B110:
		settings->m_baudRate = 110;
		break;

	case B300:
		settings->m_baudRate = 300;
		break;

	case B600:
		settings->m_baudRate = 600;
		break;

	case B1200:
		settings->m_baudRate = 1200;
		break;

	case B2400:
		settings->m_baudRate = 2400;
		break;

	case B4800:
		settings->m_baudRate = 4800;
		break;

	case B9600:
		settings->m_baudRate = 9600;
		break;

	case B19200:
		settings->m_baudRate = 19200;
		break;

	case B38400:
		settings->m_baudRate = 38400;
		break;

	case B57600:
		settings->m_baudRate = 57600;
		break;

	case B115200:
		settings->m_baudRate = 115200;
		break;

	default:
		// TODO: custom baud rate (currently fall back to 38400)
		settings->m_baudRate = 38400;
	}

	uint_t byteSize = attr.c_cflag & CSIZE;
	switch (byteSize)
	{
	case CS5:
		settings->m_dataBits = 5;
		break;

	case CS6:
		settings->m_dataBits = 6;
		break;

	case CS7:
		settings->m_dataBits = 7;
		break;

	case CS8:
	default:
		settings->m_dataBits = 8;
		break;
	}

	settings->m_stopBits = (attr.c_cflag & CSTOPB) ?
		ESerialStopBits_2 :
		ESerialStopBits_1;

	settings->m_parity =
		(attr.c_cflag & PARENB) ?
			(attr.c_cflag & CMSPAR)  ?
				(attr.c_cflag & PARODD) ? ESerialParity_Odd : ESerialParity_Even :
				(attr.c_cflag & PARODD) ? ESerialParity_Mark : ESerialParity_Space :
		ESerialParity_None;

	settings->m_flowControl =
		(attr.c_cflag & CRTSCTS) ? ESerialFlowControl_RtsCts :
		(attr.c_iflag & (IXON | IXOFF)) ? ESerialFlowControl_XonXoff : ESerialFlowControl_None;

	return true;
}

uint_t
CSerial::getStatusLines ()
{
	uint_t result = m_serial.getStatusLines ();
	if (result == -1)
		return -1;

	uint_t lines = 0;
	if (result & TIOCM_CTS)
		lines |= ESerialStatusLine_Cts;

	if (result & TIOCM_DSR)
		lines |= ESerialStatusLine_Dsr;

	if (result & TIOCM_RNG)
		lines |= ESerialStatusLine_Ring;

	if (result & TIOCM_CAR)
		lines |= ESerialStatusLine_Dcd;

	return lines;
}

#endif

//.............................................................................

} // namespace io
} // namespace axl
