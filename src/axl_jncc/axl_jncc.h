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
	AXL_JNC_API_BEGIN_LIB ()
		AXL_JNC_API_FUNCTION ("printf",  &Printf)
		AXL_JNC_API_LIB (jnc::CStdLib)
	AXL_JNC_API_END_LIB ()

	static
	int
	Printf (
		const char* pFormat,
		...
		);
};

//.............................................................................

enum EApi
{
	EApi_Serial,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CSerial: public jnc::TInterface
{
protected:
	bool m_IsOpen;

	uint_t m_BaudRate;
	uint_t m_Parity;
	uint_t m_DataBits;
	uint_t m_StopBits;
	uint_t m_FlowControl;

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

	bool
	AXL_CDECL
	SetBaudRate (uint_t BaudRate);

	bool
	AXL_CDECL
	SetParity (uint_t Parity);

	bool
	AXL_CDECL
	SetDataBits (uint_t DataBits);

	bool
	AXL_CDECL
	SetStopBits (uint_t StopBits);

	bool
	AXL_CDECL
	SetFlowControl (uint_t FlowControl);

	AXL_JNC_API_BEGIN_CLASS ("Serial", CSerial, EApi_Serial)
		AXL_JNC_API_FUNCTION ("Open",  &CSerial::Open)
		AXL_JNC_API_FUNCTION ("Close", &CSerial::Close)
		AXL_JNC_API_FUNCTION ("Read",  &CSerial::Read)
		AXL_JNC_API_FUNCTION ("Write", &CSerial::Write)
		AXL_JNC_API_AUTOGET_PROPERTY ("BaudRate", &CSerial::SetBaudRate)
		AXL_JNC_API_AUTOGET_PROPERTY ("Parity",   &CSerial::SetParity)
		AXL_JNC_API_AUTOGET_PROPERTY ("DataBits", &CSerial::SetDataBits)
		AXL_JNC_API_AUTOGET_PROPERTY ("StopBits", &CSerial::SetStopBits)
		AXL_JNC_API_AUTOGET_PROPERTY ("FlowControl", &CSerial::SetFlowControl)
	AXL_JNC_API_END_CLASS ()
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
