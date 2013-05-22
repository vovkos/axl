#include "pch.h"
#include "axl_log_Represent.h"

namespace axl {
namespace log {

//.............................................................................

void
IRepresentorTarget::AddText0 (
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
IRepresentorTarget::AddText_va (
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
CRepresentorStack::Represent (
	IRepresentorTarget* pTarget, 
	uint32_t PacketCode, 
	const void* p, 
	size_t Size, 
	uint_t VolatileFlags
	)
{
	size_t Count = GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		IRepresentor* pRepresentor = m_p [i];
		bool Result = pRepresentor->Represent (pTarget, PacketCode, p, Size, VolatileFlags);
		if (Result)
			return true;
	}

	return false;
}

//.............................................................................

} // namespace log 
} // namespace axl
