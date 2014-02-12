#include "pch.h"
#include "axl_io_Serial.h"
#include "axl_mt_Event.h"

namespace axl {
namespace io {

//.............................................................................

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

bool
CSerial::SetControlLines (
	uint_t Lines,
	uint_t Mask
	)
{
	bool Result;

	if (Mask & ESerialLine_Dtr)
	{
		Result = m_Serial.SetDtr ((Lines & ESerialLine_Dtr) != 0);
		if (!Result)
			return false;
	}

	if (Mask & ESerialLine_Rts)
	{
		Result = m_Serial.SetRts ((Lines & ESerialLine_Rts) != 0);
		if (!Result)
			return false;
	}

	return true;
}

uint_t 
CSerial::GetControlLines ()
{
	return 0;
} 

uint_t 
CSerial::GetStatusLines ()
{
	return m_Serial.GetStatusLines ();
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

//.............................................................................

} // namespace io
} // namespace axl

