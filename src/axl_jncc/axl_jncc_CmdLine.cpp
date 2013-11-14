#include "pch.h"
#include "axl_jncc_CmdLine.h"

//.............................................................................

bool
CCmdLineParser::OnValue (const char* pValue)
{
	m_pCmdLine->m_SrcFileName = pValue;
	return true;
}

bool
CCmdLineParser::OnSwitch (
	ESwitch Switch,
	const char* pValue
	)
{
	switch (Switch)
	{
	case ECmdLineSwitch_Help:
		m_pCmdLine->m_Flags |= EJncFlag_Help;
		break;

	case ECmdLineSwitch_Version:
		m_pCmdLine->m_Flags |= EJncFlag_Version;
		break;

	case ECmdLineSwitch_DebugInfo:
		m_pCmdLine->m_Flags |= EJncFlag_DebugInfo;
		break;

	case ECmdLineSwitch_Jit:
		m_pCmdLine->m_Flags |= EJncFlag_Jit;
		break;

	case ECmdLineSwitch_McJit:
		m_pCmdLine->m_Flags |= EJncFlag_Jit;
		m_pCmdLine->m_Flags |= EJncFlag_Jit_mc;
		break;

	case ECmdLineSwitch_LlvmIr:
		m_pCmdLine->m_Flags |= EJncFlag_LlvmIr;
		break;

	case ECmdLineSwitch_LlvmIrComments:
		m_pCmdLine->m_Flags |= EJncFlag_LlvmIr;
		m_pCmdLine->m_Flags |= EJncFlag_LlvmIr_c;
		break;

	case ECmdLineSwitch_Disassembly:
		m_pCmdLine->m_Flags |= EJncFlag_Disassembly;
		break;

	case ECmdLineSwitch_Run:
		m_pCmdLine->m_Flags |= EJncFlag_Jit;
		m_pCmdLine->m_Flags |= EJncFlag_RunFunction;
		m_pCmdLine->m_FunctionName = "main";
		break;

	case ECmdLineSwitch_RunFunction:
		m_pCmdLine->m_Flags |= EJncFlag_Jit;
		m_pCmdLine->m_Flags |= EJncFlag_RunFunction;
		m_pCmdLine->m_FunctionName = pValue;
		break;

	case ECmdLineSwitch_Server:
		m_pCmdLine->m_Flags |= EJncFlag_Server;
		m_pCmdLine->m_ServerPort = atoi (pValue);
		if (!m_pCmdLine->m_ServerPort)
		{
			err::SetFormatStringError ("invalid server TCP port '%s'", pValue);
			return false;
		}

		break;
	}

	return true;
}

//.............................................................................
