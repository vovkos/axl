#include "stdafx.h"
#include "axl_win_Serial.h"

namespace axl {
namespace win {

//.............................................................................

bool 
CSerial::Open (const tchar_t* pName)
{
	Close ();

	tchar_t DeviceName [MAX_PATH + 1] = { 0 };
 
	if (_tcsncmp (pName, _T("\\\\.\\"), 4) != 0)
	{
		_snprintf (DeviceName, MAX_PATH, _T("\\\\.\\%s"), pName);
		DeviceName [MAX_PATH] = 0;
		pName = DeviceName;
	}

	m_h = CreateFile (
		DeviceName, 
		GENERIC_READ | GENERIC_WRITE, 
		0, NULL, 
		OPEN_EXISTING, 
		FILE_FLAG_OVERLAPPED, 
		NULL
		);

	return err::Complete (m_h != INVALID_HANDLE_VALUE);
}

ulong_t
CSerial::GetStatusLines ()
{
	ASSERT (IsOpen ());

	ulong_t Lines;
		
	bool_t Result = ::GetCommModemStatus (m_h, &Lines);
	if (!Result)
	{
		err::SetLastSystemError ();
		return -1;
	}

	return Lines;
}

//.............................................................................

} // namespace win
} // namespace axl

