//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "test.h"

namespace  {

//..............................................................................

enum SwitchKind
{
	SwitchKind_Help,
	SwitchKind_SessionProvider,
	SwitchKind_SessionFile,
	SwitchKind_LogFile,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SL_BEGIN_CMD_LINE_SWITCH_TABLE (Switchable, SwitchKind)
	AXL_SL_CMD_LINE_SWITCH_2 (SwitchKind_Help, "h", "help", NULL, "Display help")
	AXL_SL_CMD_LINE_SWITCH_2 (SwitchKind_SessionProvider, "s", "session-provider", "<provider>", "Specify provider")
	AXL_SL_CMD_LINE_SWITCH_2 (SwitchKind_SessionFile, "f", "session-file", "<file>", "Specify session file")
	AXL_SL_CMD_LINE_SWITCH_2 (SwitchKind_LogFile, "l", "log-file", "<file>", "Specify log file")
AXL_SL_END_CMD_LINE_SWITCH_TABLE ()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MyParser: public sl::CmdLineParser <MyParser, Switchable>
{
public:
	bool
	onValue (const sl::StringRef& value)
	{
		printf ("OnValue '%s'\n", value.sz ());
		return true;
	}

	bool
	onSwitch (
		SwitchKind switchKind,
		const sl::StringRef& value
		)
	{
		printf ("OnSwitch #%d = '%s'\n", switchKind, value.sz ());
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

	MyParser parser;
	parser.parse ((int) countof (cmdLine), cmdLine);

}

//..............................................................................

ADD_TEST_CASE ("test_CmdLineParser", run)

}
