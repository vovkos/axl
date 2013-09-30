#include "pch.h"
#include "axl_log_Representer.h"
#include "axl_rtl_String.h"

namespace axl {
namespace log {

//.............................................................................

void
IRepresenterTarget::AddText0 (
	EPart PartKind,
	uint_t PartCode,
	const char* pText,
	size_t Length
	)
{	
	if (Length == -1)
		Length = strlen (pText);

	AddPart (
		PartKind,
		PartCode, 
		pText,
		Length * sizeof (char)
		);			
}

void
IRepresenterTarget::AddText_va (
	EPart PartKind,
	uint_t PartCode,
	const char* pFormat,
	axl_va_list va
	)
{
	rtl::CString Text;
	Text.Format_va (pFormat, va);
	AddText0 (PartKind, PartCode, Text, Text.GetLength ());
}

//.............................................................................

} // namespace log 
} // namespace axl
