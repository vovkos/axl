#include "pch.h"
#include "axl_jncc.h"

//.............................................................................

void 
PrintVersion (IOutStream* pOutStream)
{
	pOutStream->Printf (
		"JANCY compiler (%s) v%d.%d.%d\n",
		_AXL_CPU_STRING,
		HIBYTE (HIWORD (EJnc_CompilerVersion)),
		LOBYTE (HIWORD (EJnc_CompilerVersion)),
		LOWORD (EJnc_CompilerVersion)
		);
}

void 
PrintUsage (IOutStream* pOutStream)
{
	PrintVersion (pOutStream);

	pOutStream->Printf (
		"Usage: jnc [<options>...] <source_file>\n"
		"    -?, -h, -H     print this usage and exit\n"
		"    -v             print version\n"
		"    -l             print LLVM IR\n"
		"    -d             print disassembly\n"
		"    -r <function>  run function <function>\n"
		"    -s <port>      start compiler server on TCP <port>\n"
		);
}

//.............................................................................

CJnc* CJnc::s_pCurrentJnc = NULL;

int
CJnc::Run (
	CCmdLine* pCmdLine,
	IOutStream* pOutStream
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

	if (pCmdLine->m_SrcFileName.IsEmpty ())
	{
		PrintUsage (pOutStream);
		return EJncError_InvalidCmdLine;
	}
	
	io::CMappedFile SrcFile;
	Result = SrcFile.Open (pCmdLine->m_SrcFileName, io::EFileFlag_ReadOnly);
	const void* p = Result ? SrcFile.View () : NULL;
	if (!p)
	{
		pOutStream->Printf (
			"cannot open '%s': %s\n", 
			pCmdLine->m_SrcFileName.cc (), // thanks a lot gcc
			err::GetError ()->GetDescription ().cc () 
			);
		return EJncError_IoFailure;
	}

	rtl::CString SrcFilePath = io::GetFullFilePath (pCmdLine->m_SrcFileName);

	jnc::CModule Module;
	Result = Compile (SrcFilePath, (const char*) p, (size_t) SrcFile.GetSize ());
	if (!Result)
	{
		pOutStream->Printf ("%s\n", err::GetError ()->GetDescription ().cc ());
		return EJncError_CompileFailure;
	}

	if (pCmdLine->m_Flags & EJncFlag_LlvmIr)
		PrintLlvmIr ();

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

	CFileOutStream StdOut;
	CCmdLine CmdLine;

	Result = CmdLine.Parse (argc, argv);
	if (!Result)
	{
		printf ("error parsing command line: %s\n", err::GetError ()->GetDescription ().cc ());
		PrintUsage (&StdOut);
		return EJncError_InvalidCmdLine;
	}

	CJnc Jnc;
	return Jnc.Run (&CmdLine, &StdOut);
}

//.............................................................................
