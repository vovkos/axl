#include "pch.h"

using namespace axl;

namespace test_CmdLineParser {

//.............................................................................

enum ESwitch
{
	ESwitch_Help,
	ESwitch_SessionProvider,
	ESwitch_SessionFile,
	ESwitch_LogFile,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_RTL_BEGIN_CMD_LINE_SWITCH_TABLE (CSwitchTable, ESwitch)
	AXL_RTL_CMD_LINE_SWITCH_2 (ESwitch_Help, "h", "help", NULL, "Display help")
	AXL_RTL_CMD_LINE_SWITCH_2 (ESwitch_SessionProvider, "s", "session-provider", "<provider>", "Specify provider")
	AXL_RTL_CMD_LINE_SWITCH_2 (ESwitch_SessionFile, "f", "session-file", "<file>", "Specify session file")
	AXL_RTL_CMD_LINE_SWITCH_2 (ESwitch_LogFile, "l", "log-file", "<file>", "Specify log file")
AXL_RTL_END_CMD_LINE_SWITCH_TABLE ()

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMyParser: public rtl::CCmdLineParserT <CMyParser, CSwitchTable>
{
public:
	bool
	onValue (const char* value)
	{
		printf ("OnValue '%s'\n", value);
		return true;
	}

	bool 
	onSwitch (
		ESwitch switch,
		const char* value
		)
	{
		printf ("OnSwitch #%d = '%s'\n", switch, value);
		return true;
	}
};

void
run ()
{
	const char* cmdLine [] = 
	{
		"--help",
		"--session-provider=serial",
		"--session-file=my_serial_ssn.njssn",
		"--log-file=my_serial_ssn.njlog",
		"-fserial_ssn.njssn",
		"-l=serial_ssn.njlog",
	};

	CMyParser parser;
	parser.parse ((int) countof (cmdLine), cmdLine);

}

//.............................................................................

} // namespace test_CmdLineParser
