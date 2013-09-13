#include "pch.h"
#include "axl_log_Representer.h"

namespace axl {
namespace log {

//.............................................................................

void
IRepresenterTarget::AddText0 (
	EPart PartKind,
	uint_t PartCode,
	uint_t MergeFlags,
	const char* pText,
	size_t Length
	)
{	
	if (Length == -1)
		Length = strlen (pText);

	AddPart (
		PartKind,
		PartCode, 
		MergeFlags,
		pText,
		Length * sizeof (char)
		);			
}

void
IRepresenterTarget::AddText_va (
	EPart PartKind,
	uint_t PartCode,
	uint_t MergeFlags,
	const char* pFormat,
	axl_va_list va
	)
{
	rtl::CString Text;
	Text.Format_va (pFormat, va);
	AddText0 (PartKind, PartCode, MergeFlags, Text, Text.GetLength ());
}

//.............................................................................

bool 
CRepresenterStack::Represent (
	IRepresenterTarget* pTarget, 
	uint32_t PacketCode, 
	const void* p, 
	size_t Size, 
	uint_t VolatileFlags
	)
{
	size_t Count = GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		IRepresenter* pRepresenter = m_p [i];
		bool Result = pRepresenter->Represent (pTarget, PacketCode, p, Size, VolatileFlags);
		if (Result)
			return true;
	}

	return false;
}

//.............................................................................

} // namespace log 
} // namespace axl
