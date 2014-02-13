#include "pch.h"
#include "axl_io_Serial.h"
#include "axl_mt_Event.h"

namespace axl {
namespace io {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

bool
CSerial::SetSettings (
	const TSerialSettings* pSettings,
	uint_t Mask
	)
{
	DCB Dcb;
	Dcb.DCBlength = sizeof (Dcb);

	bool Result = m_Serial.GetSettings (&Dcb);
	if (!Result)
		return false;

	if (Mask & ESerialSetting_BaudRate)
		Dcb.BaudRate = pSettings->m_BaudRate;

	if (Mask & ESerialSetting_DataBits)
		Dcb.ByteSize = pSettings->m_DataBits;

	if (Mask & ESerialSetting_StopBits)
		Dcb.StopBits = pSettings->m_StopBits;

	if (Mask & ESerialSetting_Parity)
		Dcb.fParity = pSettings->m_Parity;

	if (Mask & ESerialSetting_FlowControl)
		switch (pSettings->m_FlowControl)
		{
		case ESerialFlowControl_None:
			Dcb.fOutxCtsFlow = FALSE;
			Dcb.fRtsControl = RTS_CONTROL_DISABLE;
			break;

		case ESerialFlowControl_RtsCts:
			Dcb.fOutxCtsFlow = TRUE;
			Dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
			break;

		case ESerialFlowControl_XonXoff:
			Dcb.fOutX = TRUE;
			Dcb.fInX = TRUE;
			break;
		}

	return m_Serial.SetSettings (&Dcb);
}

bool
CSerial::GetSettings (TSerialSettings* pSettings)
{
	DCB Dcb;
	Dcb.DCBlength = sizeof (Dcb);

	bool Result = m_Serial.GetSettings (&Dcb);
	if (!Result)
		return false;

	pSettings->m_BaudRate = Dcb.BaudRate;
	pSettings->m_DataBits = Dcb.ByteSize;
	pSettings->m_StopBits = (ESerialStopBits) Dcb.StopBits;
	pSettings->m_Parity   = (ESerialParity) Dcb.Parity;

	pSettings->m_FlowControl =
		Dcb.fOutxCtsFlow && Dcb.fRtsControl == RTS_CONTROL_HANDSHAKE ? ESerialFlowControl_RtsCts :
		Dcb.fOutX && Dcb.fInX ? ESerialFlowControl_XonXoff : ESerialFlowControl_None;

	return true;
}

uint_t
CSerial::GetStatusLines ()
{
	uint_t Lines = m_Serial.GetStatusLines ();
	return Lines != -1 ? (Lines & 0xf0) >> 4 : -1;
}

size_t
CSerial::Read (
	void* p,
	size_t Size
	)
{
	mt::CEvent CompletionEvent;

	OVERLAPPED Overlapped = { 0 };
	Overlapped.hEvent = CompletionEvent.m_Event;

	bool_t Result = m_Serial.Read (p, (dword_t) Size, NULL, &Overlapped);
	if (!Result)
		return -1;

	dword_t ActualSize;
	Result = GetOverlappedResult (m_Serial, &Overlapped, &ActualSize, true);
	if (!Result)
	{
		err::SetLastSystemError ();
		return -1;
	}

	return ActualSize;
}

size_t
CSerial::Write (
	const void* p,
	size_t Size
	)
{
	mt::CEvent CompletionEvent;

	OVERLAPPED Overlapped = { 0 };
	Overlapped.hEvent = CompletionEvent.m_Event;

	bool_t Result = m_Serial.Write (p, (dword_t) Size, NULL, &Overlapped);
	if (!Result)
		return -1;

	dword_t ActualSize;
	Result = GetOverlappedResult (m_Serial, &Overlapped, &ActualSize, true);
	if (!Result)
	{
		err::SetLastSystemError ();
		return -1;
	}

	return ActualSize;
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

bool
CSerial::SetSettings (
	const TSerialSettings* pSettings,
	uint_t Mask
	)
{
	termios Attr;
	bool Result = m_Serial.GetAttr (&Attr);
	if (!Result)
		return false;

	if (Mask & ESerialSetting_BaudRate)
	{
		speed_t Speed;

		switch (pSettings->m_BaudRate)
		{
		case ESerialBaud_110:
			Speed = B110;
			break;

		case ESerialBaud_300:
			Speed = B300;
			break;

		case ESerialBaud_600:
			Speed = B600;
			break;

		case ESerialBaud_1200:
			Speed = B1200;
			break;

		case ESerialBaud_2400:
			Speed = B2400;
			break;

		case ESerialBaud_4800:
			Speed = B4800;
			break;

		case ESerialBaud_9600:
			Speed = B9600;
			break;

		case ESerialBaud_19200:
			Speed = B19200;
			break;

		case ESerialBaud_38400:
			Speed = B38400;
			break;

		case ESerialBaud_57600:
			Speed = B57600;
			break;

		case ESerialBaud_115200:
			Speed = B115200;
			break;

		default:
			// TODO: custom baud rate (currently fall back to 38400)
			Speed = B38400;
		}

		cfsetispeed (&Attr, Speed);
		cfsetospeed (&Attr, Speed);
	}

	if (Mask & ESerialSetting_DataBits)
	{
		Attr.c_cflag &= ~CSIZE;

		switch (pSettings->m_DataBits)
		{
		case 5:
			Attr.c_cflag |= CS5;
			break;

		case 6:
			Attr.c_cflag |= CS6;
			break;

		case 7:
			Attr.c_cflag |= CS7;
			break;

		case 8:
		default:
			Attr.c_cflag |= CS8;
			break;
		}
	}

	if (Mask & ESerialSetting_StopBits)
	{
		if (pSettings->m_StopBits == ESerialStopBits_2)
			Attr.c_cflag |= CSTOPB;
		else
			Attr.c_cflag &= ~CSTOPB;
	}

	if (Mask & ESerialSetting_Parity)
	{
		Attr.c_iflag &= ~(PARMRK | INPCK);
		Attr.c_iflag |= IGNPAR;

		switch (pSettings->m_Parity)
		{
		case ESerialParity_None:
			Attr.c_cflag &= ~PARENB;
			break;

		case ESerialParity_Odd:
			Attr.c_cflag |= PARENB | PARODD;
			break;

		case ESerialParity_Even:
			Attr.c_cflag &= ~PARODD;
			Attr.c_cflag |= PARENB;
			break;

		case ESerialParity_Mark:
			Attr.c_cflag |= PARENB | CMSPAR | PARODD;
			break;

		case ESerialParity_Space:
			Attr.c_cflag &= ~PARODD;
			Attr.c_cflag |= PARENB | CMSPAR;
			break;

		default:
			Attr.c_cflag |= PARENB;
			Attr.c_iflag |= PARMRK | INPCK;
			Attr.c_iflag &= ~IGNPAR;
		}
	}

	if (Mask & ESerialSetting_FlowControl)
		switch (pSettings->m_FlowControl)
		{
		case ESerialFlowControl_RtsCts:
			Attr.c_cflag |= CRTSCTS;
			Attr.c_iflag &= ~(IXON | IXOFF | IXANY);
			break;

		case ESerialFlowControl_XonXoff:
			Attr.c_cflag &= ~CRTSCTS;
			Attr.c_iflag |= IXON | IXOFF | IXANY;
			break;

		case ESerialFlowControl_None:
		default:
			Attr.c_cflag &= ~CRTSCTS;
			Attr.c_iflag &= ~(IXON | IXOFF | IXANY);
		}

	// ensure some extra default flags

	Attr.c_cflag |= CREAD | CLOCAL;
	Attr.c_lflag = 0;
	Attr.c_oflag = 0;
	Attr.c_cc [VTIME] = 0;
	Attr.c_cc [VMIN]  = 1;

	return m_Serial.SetAttr (&Attr);
}

bool
CSerial::GetSettings (TSerialSettings* pSettings)
{
	termios Attr;
	bool Result = m_Serial.GetAttr (&Attr);
	if (!Result)
		return false;

	speed_t Speed = cfgetispeed (&Attr);
	switch (Speed)
	{
	case B110:
		pSettings->m_BaudRate = ESerialBaud_110;
		break;

	case B300:
		pSettings->m_BaudRate = ESerialBaud_300;
		break;

	case B600:
		pSettings->m_BaudRate = ESerialBaud_600;
		break;

	case B1200:
		pSettings->m_BaudRate = ESerialBaud_1200;
		break;

	case B2400:
		pSettings->m_BaudRate = ESerialBaud_2400;
		break;

	case B4800:
		pSettings->m_BaudRate = ESerialBaud_4800;
		break;

	case B9600:
		pSettings->m_BaudRate = ESerialBaud_9600;
		break;

	case B19200:
		pSettings->m_BaudRate = ESerialBaud_19200;
		break;

	case B38400:
		pSettings->m_BaudRate = ESerialBaud_38400;
		break;

	case B57600:
		pSettings->m_BaudRate = ESerialBaud_57600;
		break;

	case B115200:
		pSettings->m_BaudRate = ESerialBaud_115200;
		break;

	default:
		// TODO: custom baud rate (currently fall back to 38400)
		pSettings->m_BaudRate = ESerialBaud_38400;
	}

	uint_t ByteSize = Attr.c_cflag & CSIZE;
	switch (ByteSize)
	{
	case CS5:
		pSettings->m_DataBits = 5;
		break;

	case CS6:
		pSettings->m_DataBits = 6;
		break;

	case CS7:
		pSettings->m_DataBits = 7;
		break;

	case CS8:
	default:
		pSettings->m_DataBits = 8;
		break;
	}

	pSettings->m_StopBits = (Attr.c_cflag & CSTOPB) ?
		ESerialStopBits_2 :
		ESerialStopBits_1;

	pSettings->m_Parity =
		(Attr.c_cflag & PARENB) ?
			(Attr.c_cflag & CMSPAR)  ?
				(Attr.c_cflag & PARODD) ? ESerialParity_Odd : ESerialParity_Even :
				(Attr.c_cflag & PARODD) ? ESerialParity_Mark : ESerialParity_Space :
		ESerialParity_None;

	pSettings->m_FlowControl =
		(Attr.c_cflag & CRTSCTS) ? ESerialFlowControl_RtsCts :
		(Attr.c_iflag & (IXON | IXOFF)) ? ESerialFlowControl_XonXoff : ESerialFlowControl_None;

	return true;
}

uint_t
CSerial::GetStatusLines ()
{
	uint_t Result = m_Serial.GetStatusLines ();
	if (Result == -1)
		return -1;

	uint_t Lines = 0;
	if (Result & TIOCM_CTS)
		Lines |= ESerialStatusLine_Cts;

	if (Result & TIOCM_DSR)
		Lines |= ESerialStatusLine_Dsr;

	if (Result & TIOCM_RNG)
		Lines |= ESerialStatusLine_Ring;

	if (Result & TIOCM_CAR)
		Lines |= ESerialStatusLine_Dcd;

	return Lines;
}

#endif

//.............................................................................

} // namespace io
} // namespace axl
