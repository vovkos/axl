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
#include "axl_io_SerialSettings.h"
#include "axl_sys_Event.h"
namespace axl {
namespace io {

//..............................................................................

void
SerialSettings::setup(
	uint_t baudRate,
	uint_t dataBits,
	SerialStopBits stopBits,
	SerialParity parity,
	SerialFlowControl flowControl,
	uint_t readInterval,
	bool dtr,
	bool rts
	)
{
	m_baudRate = baudRate;
	m_dataBits = dataBits;
	m_stopBits = stopBits;
	m_parity = parity;
	m_flowControl = flowControl;
	m_readInterval = readInterval;
	m_dtr = dtr;
	m_rts = rts;
}

#if (_AXL_OS_WIN)

void
SerialSettings::setDcb(const DCB* dcb)
{
	m_baudRate = dcb->BaudRate;
	m_dataBits = dcb->ByteSize;
	m_stopBits = (SerialStopBits)dcb->StopBits;
	m_parity   = (SerialParity)dcb->Parity;

	m_flowControl =
		dcb->fOutxCtsFlow && dcb->fRtsControl == RTS_CONTROL_HANDSHAKE ? SerialFlowControl_RtsCts :
		dcb->fOutX && dcb->fInX ? SerialFlowControl_XonXoff : SerialFlowControl_None;

	if (dcb->fDtrControl == DTR_CONTROL_ENABLE)
		m_dtr = true;
	else if (dcb->fDtrControl == DTR_CONTROL_DISABLE)
		m_dtr = false;

	if (dcb->fRtsControl == RTS_CONTROL_ENABLE)
		m_rts = true;
	else if (dcb->fRtsControl == RTS_CONTROL_DISABLE)
		m_rts = false;
}

#elif (_AXL_OS_POSIX)

void
SerialSettings::setAttr(const termios* attr)
{
	speed_t speed = cfgetispeed(attr);
	switch(speed)
	{
	case B110:
		m_baudRate = 110;
		break;

	case B300:
		m_baudRate = 300;
		break;

	case B600:
		m_baudRate = 600;
		break;

	case B1200:
		m_baudRate = 1200;
		break;

	case B2400:
		m_baudRate = 2400;
		break;

	case B4800:
		m_baudRate = 4800;
		break;

	case B9600:
		m_baudRate = 9600;
		break;

	case B19200:
		m_baudRate = 19200;
		break;

	case B38400:
		m_baudRate = 38400;
		break;

	case B57600:
		m_baudRate = 57600;
		break;

	case B115200:
		m_baudRate = 115200;
		break;

	default:
		// TODO: custom baud rate (currently fall back to 38400)
		m_baudRate = 38400;
	}

	uint_t byteSize = attr->c_cflag & CSIZE;
	switch(byteSize)
	{
	case CS5:
		m_dataBits = 5;
		break;

	case CS6:
		m_dataBits = 6;
		break;

	case CS7:
		m_dataBits = 7;
		break;

	case CS8:
	default:
		m_dataBits = 8;
		break;
	}

	m_stopBits = (attr->c_cflag & CSTOPB) ?
		SerialStopBits_2 :
		SerialStopBits_1;

#ifdef CMSPAR
	m_parity =
		(attr->c_cflag & PARENB) ?
			(attr->c_cflag & CMSPAR)  ?
				(attr->c_cflag & PARODD) ? SerialParity_Mark : SerialParity_Space :
				(attr->c_cflag & PARODD) ? SerialParity_Odd : SerialParity_Even :
		SerialParity_None;
#else
	m_parity = (attr->c_cflag & PARENB) ? (attr->c_cflag & PARODD) ?
		SerialParity_Odd :
		SerialParity_Even :
		SerialParity_None;
#endif

	m_flowControl =
		(attr->c_cflag & CRTSCTS) ? SerialFlowControl_RtsCts :
		(attr->c_iflag & (IXON | IXOFF)) ? SerialFlowControl_XonXoff : SerialFlowControl_None;
}

#endif

//..............................................................................

} // namespace io
} // namespace axl
