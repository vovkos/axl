#include "stdafx.h"
#include "axl_io_Serial.h"

namespace axl {
namespace io {

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

	return false;
}

void 
CSerial::Close ()
{
}

bool 
CSerial::SetSettings (
	const TSerialSettings* pSettings,
	int Mask
	)
{
	return false;
}

bool 
CSerial::GetSettings (TSerialSettings* pSettings)
{
	return false;
}

bool
CSerial::SetControlLines (
	int Lines,
	int Mask
	)
{
	return false;
}

int
CSerial::GetControlLines ()
{
	return 0;
}

int
CSerial::GetStatusLines ()
{
	return 0;
}

bool
CSerial::SetEventHandler (
	int Mask,
	exe::IFunction* pOnEvent
	)
{
	return false;
}

//.............................................................................

} // namespace io
} // namespace axl

