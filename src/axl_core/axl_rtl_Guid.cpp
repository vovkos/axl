#include "pch.h"
#include "axl_rtl_Guid.h"
#include "axl_rtl_String.h"
#include "axl_err_Error.h"

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
	const char* p = pString;
	char* pEnd;

	char TerminatorChar = 0;

	if (*p == '{')
	{
		p++;
		TerminatorChar = '}';
	}

	uint32_t a = (uint32_t) strtoul (p, &pEnd, 16);
	if (pEnd != p + 8 || *pEnd != '-')
		return err::Fail (err::EStatus_InvalidParameter);

	p = pEnd + 1;

	uint32_t b = (uint16_t) strtoul (p, &pEnd, 16);
	if (pEnd != p + 4 || *pEnd != '-')
		return err::Fail (err::EStatus_InvalidParameter);

	p = pEnd + 1;

	uint32_t c = (uint16_t) strtoul (p, &pEnd, 16);
	if (pEnd != p + 4 || *pEnd != '-')
		return err::Fail (err::EStatus_InvalidParameter);

	p = pEnd + 1;

	uint16_t d = (uint16_t) strtoul (p, &pEnd, 16);
	if (pEnd != p + 4 || *pEnd != '-')
		return err::Fail (err::EStatus_InvalidParameter);

	p = pEnd + 1;

	uint64_t e = _strtoui64 (p, &pEnd, 16);
	if (pEnd != p + 12 || *pEnd != TerminatorChar)
		return err::Fail (err::EStatus_InvalidParameter);

	memset (this, 0, sizeof (TGuid));
	m_Data1 = a;
	m_Data2 = b;
	m_Data3 = c;
	m_Data4 [0] = (uint8_t) ((d & 0xff00) >> 8);
	m_Data4 [1] = (uint8_t) (d & 0x00ff);
	m_Data4 [2] = (uint8_t) ((e & 0xff0000000000) >> 40);
	m_Data4 [3] = (uint8_t) ((e & 0x00ff00000000) >> 32);
	m_Data4 [4] = (uint8_t) ((e & 0x0000ff000000) >> 24);
	m_Data4 [5] = (uint8_t) ((e & 0x000000ff0000) >> 16);
	m_Data4 [6] = (uint8_t) ((e & 0x00000000ff00) >> 8);
	m_Data4 [7] = (uint8_t) (e & 0x0000000000ff);
	return true;
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
		m_Data4 [0], m_Data4 [1],
		m_Data4 [2], m_Data4 [3],
		m_Data4 [4], m_Data4 [5],
		m_Data4 [6], m_Data4 [7]
		);
}

//.............................................................................

} // namespace err
} // namespace axl
