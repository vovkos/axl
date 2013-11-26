#include "pch.h"
#include "axl_jncc.h"

#define _AXL_JNCC_PRINT_USAGE_IF_NO_ARGUMENTS

//.............................................................................

#if 0

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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
AXL_CDECL
CSerial::Open (jnc::TDataPtr Name)
{
	printf ("CSerial::Open (%s)\n", Name.m_p);
	return true;
}

void
AXL_CDECL
CSerial::Close ()
{
	printf ("CSerial::Close ()\n");
}

size_t
AXL_CDECL
CSerial::Read (
	jnc::TDataPtr Ptr,
	size_t Size
	)
{
	printf ("CSerial::Read (%d)\n", Size);
	return true;
}

size_t
AXL_CDECL
CSerial::Write (
	jnc::TDataPtr Ptr,
	size_t Size
	)
{
	printf ("CSerial::Write (%d)\n", Size);
	return true;
}

bool
AXL_CDECL
CSerial::SetBaudRate (uint_t BaudRate)
{
	printf ("CSerial::SetBaudRate (%d)\n", BaudRate);
	return true;
}

bool
AXL_CDECL
CSerial::SetParity (uint_t Parity)
{
	printf ("CSerial::SetParity (%d)\n", Parity);
	return true;
}

bool
AXL_CDECL
CSerial::SetDataBits (uint_t DataBits)
{
	printf ("CSerial::SetDataBits (%d)\n", DataBits);
	return true;
}

bool
AXL_CDECL
CSerial::SetStopBits (uint_t StopBits)
{
	printf ("CSerial::SetStopBits (%d)\n", StopBits);
	return true;
}

bool
AXL_CDECL
CSerial::SetFlowControl (uint_t FlowControl)
{
	printf ("CSerial::SetFlowControl (%d)\n", FlowControl);
	return true;
}

#endif

//.............................................................................

void
PrintVersion (COutStream* pOutStream)
{
	pOutStream->Printf (
		"Jancy compiler (%s) v%d.%d.%d\n",
		_AXL_CPU_STRING,
		HIBYTE (HIWORD (EJnc_CompilerVersion)),
		LOBYTE (HIWORD (EJnc_CompilerVersion)),
		LOWORD (EJnc_CompilerVersion)
		);
}

void
PrintUsage (COutStream* pOutStream)
{
	PrintVersion (pOutStream);

	rtl::CString HelpString = CCmdLineSwitchTable::GetHelpString ();
	pOutStream->Printf ("Usage: jnc [<options>...] <source_file>\n%s", HelpString.cc ());
}

//.............................................................................

CJnc* CJnc::m_pCurrentJnc = NULL;

int
CJnc::Run (
	TCmdLine* pCmdLine,
	COutStream* pOutStream
	)
{
	bool Result;

	m_pCmdLine = pCmdLine;
	m_pOutStream = pOutStream;

	if (pCmdLine->m_Flags & EJncFlag_Help)
	{
		PrintUsage (pOutStream);
		return EJncError_Success;
	}

	if (pCmdLine->m_Flags & EJncFlag_Version)
	{
		PrintVersion (pOutStream);
		return EJncError_Success;
	}

	if (pCmdLine->m_Flags & EJncFlag_Server)
		return Server ();

	rtl::CArrayT <char> StdInBuffer;
	io::CMappedFile SrcFile;

	rtl::CString SrcName;
	const char* pSrc;
	size_t SrcSize;

	if (pCmdLine->m_Flags & EJncFlag_StdInSrc)
	{
#if (_AXL_ENV == AXL_ENV_WIN)
		int StdInFile = _fileno (stdin);
#endif
		for (;;)
		{
			char Buffer [1024];
#if (_AXL_ENV == AXL_ENV_WIN)
			int Result = _read (StdInFile, Buffer, sizeof (Buffer));
#else
			int Result = read (STDIN_FILENO, Buffer, sizeof (Buffer));
#endif
			if (Result <= 0)
				break;

			StdInBuffer.Append (Buffer, Result);
		}

		pSrc = StdInBuffer;
		SrcSize = StdInBuffer.GetCount ();
		SrcName = !m_pCmdLine->m_SrcNameOverride.IsEmpty () ?
			m_pCmdLine->m_SrcNameOverride :
			"stdin";
	}
	else if (!pCmdLine->m_SrcFileName.IsEmpty ())
	{
		Result = SrcFile.Open (pCmdLine->m_SrcFileName, io::EFileFlag_ReadOnly);
		if (!Result)
		{
			pOutStream->Printf (
				"cannot open '%s': %s\n",
				pCmdLine->m_SrcFileName.cc (), // thanks a lot gcc
				err::GetError ()->GetDescription ().cc ()
				);
			return EJncError_IoFailure;
		}

		pSrc = (const char*) SrcFile.View ();
		SrcSize = (size_t) SrcFile.GetSize ();
		SrcName = !m_pCmdLine->m_SrcNameOverride.IsEmpty () ?
			m_pCmdLine->m_SrcNameOverride :
			io::GetFullFilePath (pCmdLine->m_SrcFileName);
	}
	else
	{
		pOutStream->Printf ("missing input (required source-file-name or --stdin)\n");
		return EJncError_InvalidCmdLine;
	}

	if (pCmdLine->m_Flags & (EJncFlag_RunFunction | EJncFlag_Disassembly))
		pCmdLine->m_Flags |= EJncFlag_Jit;

	Result = Compile (SrcName, pSrc, SrcSize);
	if (!Result)
	{
		pOutStream->Printf ("%s\n", err::GetError ()->GetDescription ().cc ());
		return EJncError_CompileFailure;
	}

	if (pCmdLine->m_Flags & EJncFlag_LlvmIr)
		PrintLlvmIr ();

	if (m_pCmdLine->m_Flags & EJncFlag_Jit)
	{
		Result = Jit ();
		if (!Result)
		{
			pOutStream->Printf ("%s\n", err::GetError ()->GetDescription ().cc ());
			return EJncError_CompileFailure;
		}
	}

	if (pCmdLine->m_Flags & EJncFlag_Disassembly)
		PrintDisassembly ();

	if (pCmdLine->m_Flags & EJncFlag_RunFunction)
	{
		int ReturnValue;
		Result = RunFunction (&ReturnValue);
		if (!Result)
		{
			pOutStream->Printf ("%s\n", err::GetError ()->GetDescription ().cc ());
			return EJncError_RunFailure;
		}

		pOutStream->Printf ("'%s' returned (%d).\n", pCmdLine->m_FunctionName.cc (), ReturnValue);
	}

	return EJncError_Success;
}

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)
int
wmain (
	int argc,
	wchar_t* argv []
	)
#else
int
main (
	int argc,
	char* argv []
	)
#endif
{
	bool Result;

	llvm::InitializeNativeTarget ();
	llvm::InitializeNativeTargetAsmParser ();
	llvm::InitializeNativeTargetAsmPrinter ();
	llvm::InitializeNativeTargetDisassembler ();

	err::CParseErrorProvider::Register ();
	srand ((int) axl::g::GetTimestamp ());

	CFileOutStream StdOut;
	TCmdLine CmdLine;
	CCmdLineParser Parser (&CmdLine);

#ifdef _AXL_JNCC_PRINT_USAGE_IF_NO_ARGUMENTS
	if (argc < 2)
	{
		PrintUsage (&StdOut);
		return 0;
	}
#endif

	Result = Parser.Parse (argc, argv);
	if (!Result)
	{
		printf ("error parsing command line: %s\n", err::GetError ()->GetDescription ().cc ());
		return EJncError_InvalidCmdLine;
	}

	CJnc Jnc;
	return Jnc.Run (&CmdLine, &StdOut);
}

//.............................................................................
