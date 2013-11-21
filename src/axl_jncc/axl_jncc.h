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
PrintVersion (COutStream* pOutStream);

void
PrintUsage (COutStream* pOutStream);

//.............................................................................

class CStdLib: public jnc::CStdLib
{
public:
	CStdLib ();

	void
	Export (
		jnc::CModule* pModule,
		jnc::CRuntime* pRuntime
		);

	static
	int
	Printf (
		const char* pFormat,
		...
		);
};

//.............................................................................

// test object for playing with jancy export macros

class CSerial: public jnc::TInterface
{
protected:

public:
	bool
	AXL_CDECL
	Open (jnc::TDataPtr Name);

	void
	AXL_CDECL
	Close ();

	size_t
	AXL_CDECL
	Read (
		jnc::TDataPtr Ptr,
		size_t Size
		);

	size_t
	AXL_CDECL
	Write (
		jnc::TDataPtr Ptr,
		size_t Size
		);
};

//.............................................................................

class CJnc
{
protected:
	static CJnc* m_pCurrentJnc;

	TCmdLine* m_pCmdLine;
	COutStream* m_pOutStream;

	jnc::CModule m_Module;
	jnc::CRuntime m_Runtime;
	CStdLib m_StdLib;

public:
	CJnc ()
	{
		m_pCurrentJnc = this;
		m_pCmdLine = NULL;
		m_pOutStream = NULL;
	}

	static
	CJnc*
	GetCurrentJnc ()
	{
		return m_pCurrentJnc;
	}

	COutStream*
	GetOutStream ()
	{
		return m_pOutStream;
	}

	int
	Run (
		TCmdLine* pCmdLine,
		COutStream* pOutStream
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
