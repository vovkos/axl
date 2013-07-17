#pragma once

//.............................................................................

enum EJncFlag
{
	EJncFlag_Help        = 0x01,
	EJncFlag_Version     = 0x02,
	EJncFlag_LlvmIr      = 0x04,
	EJncFlag_Disassembly = 0x08,
	EJncFlag_RunFunction = 0x10,
	EJncFlag_Server      = 0x20,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCmdLine
{
protected:
	enum EParseState
	{
		EParseState_Normal = 0,
		EParseState_ServerPort,
		EParseState_FunctionName,
	};

protected:
	EParseState m_ParseState;

public:
	uint_t m_Flags;
	uint16_t m_ServerPort;
	rtl::CString m_SrcFileName;
	rtl::CString m_FunctionName;

public:
	CCmdLine ();

	void
	Clear ();

#if (_AXL_ENV == AXL_ENV_WIN)
	bool
	Parse (
		int argc,
		wchar_t** argv
		);
#else
	bool
	Parse (
		int argc,
		char** argv
		);
#endif

	bool
	Parse (
		const char* pCmdLine,
		size_t Length = -1
		);

protected:
	bool
	ParseArg (
		const char* pArg,
		size_t Length
		);

	bool
	ParseSwitchArg (
		char Switch,
		const char* pValue,
		size_t Length
		);
};

//.............................................................................
 