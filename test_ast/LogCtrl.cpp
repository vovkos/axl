#include "stdafx.h"
#include "test_ast.h"
#include "OutputPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//.............................................................................

void
CLogCtrl::TraceV (
	const tchar_t* pFormat, 
	va_list va
	)
{
	rtl::CString Text;
	Text.FormatV (pFormat, va);
	Trace0 (Text);
}

void
CLogCtrl::Trace0 (const tchar_t* pText)
{
	size_t Length = GetWindowTextLength ();
	SetSel ((int) Length, (int) Length);
	ReplaceSel (pText);
}

//.............................................................................
