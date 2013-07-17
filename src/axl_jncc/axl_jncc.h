#pragma once

#include "axl_jncc_CmdLine.h"
#include "axl_jncc_OutStream.h"

//.............................................................................

enum
{
	EJnc_CompilerVersion = MAKELONG (0, MAKEWORD (0, 1)) 
};

//.............................................................................

enum EJncError
{
	EJncError_Success = 0,
	EJncError_InvalidCmdLine,
	EJncError_IoFailure,
	EJncError_CompileFailure,
	EJncError_RunFailure,
};

//.............................................................................

void 
PrintVersion (IOutStream* pOutStream);

void 
PrintUsage (IOutStream* pOutStream);

//.............................................................................

class CJnc
{
protected:
	CCmdLine* m_pCmdLine;
	IOutStream* m_pOutStream;

	jnc::CModule m_Module;
	jnc::CRuntime m_Runtime;

public:
	CJnc ()
	{
		m_pCmdLine = NULL;
		m_pOutStream = NULL;
	}

	int
	Run (
		CCmdLine* pCmdLine,
		IOutStream* pOutStream
		);

protected:
	bool 
	Compile (
		const char* pFileName,
		const char* pSource,
		size_t Length
		);

	void
	PrintLlvmIr ();

	void 
	PrintDisassembly ();

	bool 
	RunFunction (int* pReturnValue = NULL);

	bool 
	RunFunction (
		jnc::CFunction* pFunction, 
		int* pReturnValue = NULL
		);

	static
	int
	StdLib_Printf (
		const char* pFormat,
		...
		);

protected:
	int 
	Server ();

	int
	Client (
		SOCKET Socket,
		sockaddr_in* pSockAddr
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef mt::CScopeTlsSlotT <CJnc> CScopeThreadJnc;

inline
CJnc*
GetCurrentThreadJnc ()
{
	return mt::GetTlsSlotValue <CJnc> ();
}

//.............................................................................
