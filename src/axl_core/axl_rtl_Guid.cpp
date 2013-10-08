#include "pch.h"
#include "axl_rtl_Guid.h"
#include "axl_rtl_String.h"

namespace axl {
namespace rtl {

//.............................................................................

bool
TGuid::Generate ()
{
	// not yet
	memset (this, 0, sizeof (TGuid));
	return false;
}

bool
TGuid::Parse (const char* pString)
{
	// not yet
	memset (this, 0, sizeof (TGuid));
	return false;
}

rtl::CString
TGuid::GetGuidString (uint_t Flags) const
{
	static const char* FormatTable [2] [2] = 
	{
		{ 
			"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		},
		{
			"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
			"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		}
	};

	size_t i1 = (Flags & EGuidStringFlag_CurlyBraces) ? 1 : 0;
	size_t i2 = (Flags & EGuidStringFlag_UpperCase) ? 1 : 0;

	return rtl::CString::Format_s (
		FormatTable [i1] [i2],
		m_Data1, 
		m_Data2, 
		m_Data3, 
		m_Data4[0], m_Data4[1],
		m_Data4[2], m_Data4[3], 
		m_Data4[4], m_Data4[5], 
		m_Data4[6], m_Data4[7]
		);
}

//.............................................................................

} // namespace err
} // namespace axl
