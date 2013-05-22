#include "pch.h"
#include "test_ast.h"
#include "OutputPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//.............................................................................

void
CLogCtrl::Trace_va (
	const char* pFormat, 
	axl_va_list va
	)
{
	rtl::CString Text;
	Text.Format_va (pFormat, va);
	Trace_0 (Text);
}

void
CLogCtrl::Trace_0 (const char* pText)
{
	size_t Length = GetWindowTextLength ();
	SetSel ((int) Length, (int) Length);
	ReplaceSel (rtl::CString_w (pText));
}

//.............................................................................
