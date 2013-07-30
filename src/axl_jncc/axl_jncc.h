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

class CStdLib: public jnc::CStdLib
{
public:
	CStdLib ()
	{
		m_FunctionMap ["printf"] = (void*) Printf;
	}
	
	static
	int
	Printf (
		const char* pFormat,
		...
		);	
};

//.............................................................................

class CJnc
{
protected:
	static CJnc* s_pCurrentJnc;

	CCmdLine* m_pCmdLine;
	IOutStream* m_pOutStream;

	jnc::CModule m_Module;
	jnc::CRuntime m_Runtime;	
	CStdLib m_StdLib;

public:
	CJnc ()
	{
		s_pCurrentJnc = this;
		m_pCmdLine = NULL;
		m_pOutStream = NULL;
	}

	static
	CJnc* 
	GetCurrentJnc ()
	{
		return s_pCurrentJnc;
	}

	IOutStream* 
	GetOutStream ()
	{
		return m_pOutStream;
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

	bool 
	Jit ();

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

protected:
	int 
	Server ();

	int
	Client (
		SOCKET Socket,
		sockaddr_in* pSockAddr
		);
};

//.............................................................................
