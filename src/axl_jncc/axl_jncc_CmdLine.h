#pragma once

//.............................................................................

enum EJncFlag
{
	EJncFlag_Help        = 0x0001,
	EJncFlag_Version     = 0x0002,
	EJncFlag_LlvmIr      = 0x0004,
	EJncFlag_LlvmIr_c    = 0x0008,
	EJncFlag_Jit         = 0x0010,
	EJncFlag_Jit_mc      = 0x0020,
	EJncFlag_Disassembly = 0x0040,
	EJncFlag_RunFunction = 0x0080,
	EJncFlag_Server      = 0x0100,
	EJncFlag_DebugInfo   = 0x0200,
};

struct TCmdLine
{
	uint_t m_Flags;
	uint16_t m_ServerPort;
	rtl::CString m_SrcFileName;
	rtl::CString m_FunctionName;

	TCmdLine ()
	{
		m_Flags = 0;
		m_ServerPort = 0;
	}
};

//.............................................................................

enum ECmdLineSwitch
{
	ECmdLineSwitch_Undefined = 0,
	ECmdLineSwitch_Help,
	ECmdLineSwitch_Version,
	ECmdLineSwitch_LlvmIr,
	ECmdLineSwitch_LlvmIrComments,
	ECmdLineSwitch_DebugInfo,
	ECmdLineSwitch_Jit,
	ECmdLineSwitch_McJit,
	ECmdLineSwitch_Disassembly,
	ECmdLineSwitch_Run,
	ECmdLineSwitch_RunFunction = rtl::ECmdLineSwitchFlag_HasValue,
	ECmdLineSwitch_Server,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_RTL_BEGIN_CMD_LINE_SWITCH_TABLE (CCmdLineSwitchTable, ECmdLineSwitch)
	AXL_RTL_CMD_LINE_SWITCH_GROUP ("General options")
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_Help,
		"h", "help", NULL,
		"Display this help"
		)
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_Version,
		"v", "version", NULL,
		"Display compiler version"
		)
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_Server,
		"s", "server", "<port>",
		"Run compiler server on TCP port <port>"
		)

	AXL_RTL_CMD_LINE_SWITCH_GROUP ("LLVM IR options")
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_LlvmIr,
		"l", "llvm-ir", NULL,
		"Emit LLVM IR (lli-compatible)"
		)
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_LlvmIrComments,
		"c", "llvm-ir-comments", NULL,
		"Emit LLVM IR (manual mode w/ comments)"
		)
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_DebugInfo,
		"g", "debug-info", NULL,
		"Generate debug information (does not work with legacy JIT)"
		)

	AXL_RTL_CMD_LINE_SWITCH_GROUP ("JIT options")
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_Jit,
		"j", "jit", NULL,
		"JIT compiled module"
		)
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_McJit,
		"m", "mcjit", NULL,
		"Use MC-JIT engine (does not work on Windows)"
		)
	AXL_RTL_CMD_LINE_SWITCH_3 (
		ECmdLineSwitch_Disassembly,
		"d", "dasm", "disassembly", NULL,
		"Emit disassembly of JITted code"
		)
	AXL_RTL_CMD_LINE_SWITCH_2 (
		ECmdLineSwitch_Run,
		"r", "run", NULL,
		"Run function 'main' (implies JITting)"
		)
	AXL_RTL_CMD_LINE_SWITCH (
		ECmdLineSwitch_RunFunction,
		"run-function", "<function>",
		"Run function <function> (implies JITting)"
		)
AXL_RTL_END_CMD_LINE_SWITCH_TABLE ()

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCmdLineParser: public rtl::CCmdLineParserT <CCmdLineParser, CCmdLineSwitchTable>
{
	friend class rtl::CCmdLineParserT <CCmdLineParser, CCmdLineSwitchTable>;

protected:
	TCmdLine* m_pCmdLine;

public:
	CCmdLineParser (TCmdLine* pCmdLine)
	{
		m_pCmdLine = pCmdLine;
	}

protected:
	bool
	OnValue (const char* pValue);

	bool
	OnSwitch (
		ESwitch Switch,
		const char* pValue
		);
};

//.............................................................................
