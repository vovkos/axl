#include "pch.h"
#include "axl_sl_Guid.h"
#include "axl_err_Error.h"

namespace axl {
namespace sl {

//.............................................................................

bool
Guid::generate ()
{
	// not yet
	memset (this, 0, sizeof (Guid));
	return false;
}

bool
Guid::parse (const sl::StringRef& string)
{
	const char* p = string.sz ();
	char* end;

	char terminatorChar = 0;

	if (*p == '{')
	{
		p++;
		terminatorChar = '}';
	}

	uint32_t a = (uint32_t) strtoul (p, &end, 16);
	if (end != p + 8 || *end != '-')
		return err::fail (err::SystemErrorCode_InvalidParameter);

	p = end + 1;

	uint32_t b = (uint16_t) strtoul (p, &end, 16);
	if (end != p + 4 || *end != '-')
		return err::fail (err::SystemErrorCode_InvalidParameter);

	p = end + 1;

	uint32_t c = (uint16_t) strtoul (p, &end, 16);
	if (end != p + 4 || *end != '-')
		return err::fail (err::SystemErrorCode_InvalidParameter);

	p = end + 1;

	uint16_t d = (uint16_t) strtoul (p, &end, 16);
	if (end != p + 4 || *end != '-')
		return err::fail (err::SystemErrorCode_InvalidParameter);

	p = end + 1;

	uint64_t e = _strtoui64 (p, &end, 16);
	if (end != p + 12 || *end != terminatorChar)
		return err::fail (err::SystemErrorCode_InvalidParameter);

	memset (this, 0, sizeof (Guid));
	m_data1 = a;
	m_data2 = b;
	m_data3 = c;
	m_data4 [0] = (uint8_t) ((d & 0xff00) >> 8);
	m_data4 [1] = (uint8_t) (d & 0x00ff);
	m_data4 [2] = (uint8_t) ((e & 0xff0000000000) >> 40);
	m_data4 [3] = (uint8_t) ((e & 0x00ff00000000) >> 32);
	m_data4 [4] = (uint8_t) ((e & 0x0000ff000000) >> 24);
	m_data4 [5] = (uint8_t) ((e & 0x000000ff0000) >> 16);
	m_data4 [6] = (uint8_t) ((e & 0x00000000ff00) >> 8);
	m_data4 [7] = (uint8_t) (e & 0x0000000000ff);
	return true;
}

sl::String
Guid::getGuidString (uint_t flags) const
{
	static const char* formatTable [2] [2] =
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

	size_t i1 = (flags & GuidStringFlag_CurlyBraces) ? 1 : 0;
	size_t i2 = (flags & GuidStringFlag_UpperCase) ? 1 : 0;

	return sl::formatString (
		formatTable [i1] [i2],
		m_data1,
		m_data2,
		m_data3,
		m_data4 [0], m_data4 [1],
		m_data4 [2], m_data4 [3],
		m_data4 [4], m_data4 [5],
		m_data4 [6], m_data4 [7]
		);
}

//.............................................................................

} // namespace err
} // namespace axl
