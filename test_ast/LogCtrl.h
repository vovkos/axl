#pragma once

//.............................................................................

class CLogCtrl: public CEdit
{
public:
	void
	Trace0 (const tchar_t* pText);

	void
	Trace (
		const tchar_t* pFormat, 
		...
		)
	{
		TraceV (pFormat, va_start_e (pFormat));
	}
		
	void
	TraceV (
		const tchar_t* pFormat, 
		va_list va
		);

	void
	Clear ()
	{
		SetWindowText (NULL);
	}
};

//.............................................................................

