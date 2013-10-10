#include "pch.h"

using namespace axl;

namespace test_CmdLineParser {

//.............................................................................

class CMyParser: public rtl::CCmdLineParserT <CMyParser>
{
protected:
	enum ESwitch
	{
		ESwitch_Undefined,
		ESwitch_Help,
		ESwitch_SessionProvider,
		ESwitch_SessionFile,
		ESwitch_LogFile,
	};

	AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING (CSwitchNameMap, ESwitch)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("help", ESwitch_Help)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("session-provider", ESwitch_SessionProvider)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("session-file", ESwitch_SessionFile)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("log-file", ESwitch_LogFile)
	AXL_RTL_END_HASH_TABLE_MAP ()

	AXL_RTL_BEGIN_HASH_TABLE_MAP_CHAR (CSwitchCharMap, ESwitch)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ('h', ESwitch_Help)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ('s', ESwitch_SessionProvider)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ('f', ESwitch_SessionFile)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ('l', ESwitch_LogFile)
	AXL_RTL_END_HASH_TABLE_MAP ()

public:
	bool
	OnValue (const char* pValue)
	{
		printf ("OnValue '%s'\n", pValue);
		return true;
	}

	bool 
	OnSwitch (
		const char* pSwitchName,
		const char* pValue
		)
	{
		CSwitchNameMap::CIterator It = CSwitchNameMap::Find (pSwitchName);
		ESwitch Switch = It ? It->m_Value : ESwitch_Undefined;
		printf ("OnSwitch '%s' = '%s'\n", pSwitchName, pValue);
		return true;
	}

	bool
	OnSwitch (
		const char SwitchChar,
		const char* pValue
		)
	{
		CSwitchCharMap::CIterator It = CSwitchCharMap::Find (SwitchChar);
		ESwitch Switch = It ? It->m_Value : ESwitch_Undefined;
		printf ("OnSwitch '%c' = '%s'\n", SwitchChar, pValue);
		return true;
	}
};

void
Run ()
{
	const char* CmdLine [] = 
	{
		"--help",
		"--session-provider=serial",
		"--session-file=my_serial_ssn.njssn",
		"--log-file=my_serial_ssn.njlog",
		"-fserial_ssn.njssn",
		"-l=serial_ssn.njlog",
	};

	CMyParser Parser;
	Parser.Parse ((int) countof (CmdLine), CmdLine);

}

//.............................................................................

} // namespace test_CmdLineParser
