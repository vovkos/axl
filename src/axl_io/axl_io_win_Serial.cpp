#include "pch.h"
#include "axl_io_win_Serial.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool 
CSerial::Open (const char* pName)
{
	Close ();

	if (strncmp (pName, "\\\\.\\", 4) == 0)
		pName += 4;
	
	wchar_t DeviceName [128] = { 0 }; 
	_snwprintf (DeviceName, countof (DeviceName), L"\\\\.\\%S", pName);

	m_h = ::CreateFileW (
		DeviceName, 
		GENERIC_READ | GENERIC_WRITE, 
		0, NULL, 
		OPEN_EXISTING, 
		FILE_FLAG_OVERLAPPED, 
		NULL
		);

	return err::Complete (m_h != INVALID_HANDLE_VALUE);
}

dword_t
CSerial::GetStatusLines ()
{
	ASSERT (IsOpen ());

	dword_t Lines;
		
	bool_t Result = ::GetCommModemStatus (m_h, &Lines);
	if (!Result)
		return err::FailWithLastSystemError (-1);

	return Lines;
}

dword_t
CSerial::GetWaitMask ()
{
	dword_t Mask;

	bool_t Result = ::GetCommMask (m_h, &Mask);
	if (!Result)
		return err::FailWithLastSystemError (-1);

	return Mask;
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl

