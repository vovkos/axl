#include "pch.h"
#include "axl_jncc_CmdLine.h"

//.............................................................................

CCmdLine::CCmdLine ()
{
	m_ParseState = EParseState_Normal;
	m_Flags = 0;
	m_ServerPort = 0;
}

void
CCmdLine::Clear ()
{
	m_SrcFileName.Clear ();
	m_FunctionName.Clear ();

	m_ParseState = EParseState_Normal;
	m_Flags = 0;
	m_ServerPort = 0;
}

#if (_AXL_ENV == AXL_ENV_WIN)
bool
CCmdLine::Parse (
	int argc,
	wchar_t** argv
	)
#else
bool
CCmdLine::Parse (
	int argc,
	char** argv
	)
#endif
{
	bool Result;

	Clear ();

	for (int i = 1; i < argc; i++)
	{
		rtl::CString Arg = argv [i];
		Result = ParseArg (Arg, Arg.GetLength ());
		if (!Result)
			return false;
	}

	return Verify ();
}

bool
CCmdLine::Parse (
	const char* pCmdLine,
	size_t Length
	)
{
	bool Result;

	if (Length == -1)
		Length = strlen (pCmdLine);

	const char* p = pCmdLine;
	const char* pEnd = p + Length;

	for (;;)
	{
		while (p < pEnd && isspace (*p))
			p++;

		if (p == pEnd)
			break;

		const char* pArg;
		size_t Length;

		if (*p == '"')
		{			
			p++;
			pArg = p;

			while (p < pEnd && *p != '"')
				p++;

			if (p == pArg)
			{
				err::SetFormatStringError ("unterminated quoted argument");
				return false;
			}

			Length = p - pArg;
			p++;
		}
		else
		{
			pArg = p;

			while (p < pEnd && !isspace (*p))
				p++;

			Length = p - pArg;
		}

		Result = ParseArg (pArg, Length);
		if (!Result)
			return false;
	}

	return Verify ();
}

bool
CCmdLine::Verify ()
{
	if ((m_Flags & EJncFlag_RunFunction) && m_FunctionName.IsEmpty ())
	{
		err::SetFormatStringError ("'-r' requires function name");
		return false;
	}

	if ((m_Flags & EJncFlag_Server) && !m_ServerPort)
	{
		err::SetFormatStringError ("'-s' requires server port number");
		return false;
	}

	return true;
}

bool
CCmdLine::ParseArg (
	const char* pArg,
	size_t Length
	)
{
	switch (m_ParseState)
	{
	case EParseState_ServerPort:
		m_ServerPort = atoi (pArg);
		m_ParseState = EParseState_Normal;
		break;

	case EParseState_FunctionName:
		m_FunctionName.Copy (pArg, Length);
		m_ParseState = EParseState_Normal;
		break;

	default:
		if (pArg [0] == '-') // switch
		{
			if (Length < 2)
			{
				err::SetFormatStringError ("invalid switch syntax");
				return false;				
			}

			return ParseSwitchArg (pArg [1], pArg + 2, Length - 2);
		}
		
		m_SrcFileName.Copy (pArg, Length);
	}
	
	return true;
}

bool
CCmdLine::ParseSwitchArg (
	char Switch,
	const char* pValue,
	size_t Length
	)
{
	switch (Switch)
	{
	case '?': case 'h': case 'H':
		m_Flags |= EJncFlag_Help;
		break;

	case 'v':
		m_Flags |= EJncFlag_Version;
		break;

	case 'g':
		m_Flags |= EJncFlag_DebugInfo;
		break;

	case 'j':
		m_Flags |= EJncFlag_Jit;
		if (Length && *pValue == 'm')
			m_Flags |= EJncFlag_Jit_mc;
		break;

	case 'l':
		m_Flags |= EJncFlag_LlvmIr;
		if (Length && *pValue == 'c')
			m_Flags |= EJncFlag_LlvmIr_c;
		break;

	case 'd':
		m_Flags |= EJncFlag_Disassembly;
		break;

	case 'r':
		m_Flags |= EJncFlag_RunFunction;

		if (Length)
			m_FunctionName.Copy (pValue, Length);
		else
			m_ParseState = EParseState_FunctionName;

		break;
				
	case 's':
		m_Flags |= EJncFlag_Server;

		if (Length)
			m_ServerPort = atoi (pValue);
		else
			m_ParseState = EParseState_ServerPort;

		break;

	default:
		err::SetFormatStringError ("unknown switch '-%c'", Switch);
		return false;
	}				

	return true;
}


//.............................................................................
