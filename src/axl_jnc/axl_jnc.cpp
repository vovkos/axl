#include "pch.h"
#include "axl_jnc_Parser.llk.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Disassembler.h"
#include "axl_io_FilePathUtils.h"
#include "axl_io_File.h"
#include "axl_io_MappedFile.h"
#include "axl_err_Errno.h"

using namespace axl;
using namespace jnc;

//.............................................................................

enum
{
	EJnc_CompilerVersion = MAKELONG (0, MAKEWORD (0, 1)) 
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EJncFlag
{
	EJncFlag_Run = 0x01,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EJncError
{
	EJncError_Success = 0,
	EJncError_NoSourceFile,	
	EJncError_InvalidSwitch,
	EJncError_IoFailure,
	EJncError_CompileFailure,
	EJncError_RunFailure,
};

//.............................................................................

void 
PrintVersion ()
{
	printf (
		"Jancy compiler (%s) v%d.%d.%d\n",
		_AXL_CPU_STRING,
		HIBYTE (HIWORD (EJnc_CompilerVersion)),
		LOBYTE (HIWORD (EJnc_CompilerVersion)),
		LOWORD (EJnc_CompilerVersion)
		);
}

void 
PrintUsage (bool HasVersion = true)
{
	if (HasVersion)
		PrintVersion ();
	
	printf (
		"Usage: jnc [<options>...] <source_file>\n"
		"    -?, -h, -H       print this usage and exit\n"
		"    -v               print version\n"
		"    -r <function>    run function <function>\n"
		"    -o <output_file> generate LLVM IR in <output_file>\n"
		);
}

//.............................................................................

bool 
Compile (
	CModule* pModule,
	const char* pFileName,
	const char* pSource,
	size_t Length
	)
{
	bool Result;

	llvm::Module* pLlvmModule = new llvm::Module (pFileName, llvm::getGlobalContext ());

	pModule->Create (pFileName, pLlvmModule);

	CScopeThreadModule ScopeModule (pModule);
	
	CLexer Lexer;
	Lexer.Create (pFileName, pSource, Length);

	CParser Parser;
	Parser.Create (CParser::StartSymbol, true);

	for (;;)
	{
		const CToken* pToken = Lexer.GetToken ();
		if (pToken->m_Token == EToken_Eof)
			break;

		Result = Parser.ParseToken (pToken);
		if (!Result)
		{			
			err::PushSrcPosError (pFileName, pToken->m_Pos);
			return false;
		}

		Lexer.NextToken ();
	}

	return pModule->Compile ();
}

bool 
Jit (CModule* pModule)
{
	llvm::EngineBuilder EngineBuilder (pModule->GetLlvmModule ());	
	std::string ErrorString;
	EngineBuilder.setErrorStr (&ErrorString);	
	
#if (_AXL_CPU == AXL_CPU_X86)
	EngineBuilder.setMArch ("x86");
#endif

	llvm::ExecutionEngine* pLlvmExecutionEngine = EngineBuilder.create ();
	if (!pLlvmExecutionEngine)
	{
		err::SetFormatStringError ("Error creating a JITter (%s)\n", ErrorString.c_str ());
		return false;
	}

	CStdLib::Export (pModule, pLlvmExecutionEngine);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, "printf", (void*) printf);
	
	return pModule->m_FunctionMgr.JitFunctions (pLlvmExecutionEngine);
} 

bool 
RunFunction (
	CFunction* pFunction, 
	int* pReturnValue = NULL
	)
{
	typedef int (*FFunction) ();
	FFunction pf = (FFunction) pFunction->GetMachineCode ();
	ASSERT (pf);

	bool Result = true;

	try
	{
		int ReturnValue = pf ();
		if (pReturnValue)
			*pReturnValue = ReturnValue;
	}
	catch (err::CError Error)
	{
		err::SetError (Error);
		Result = false;
	}
	catch (...)
	{
		err::SetStringError ("unknown-exception");
		Result = false;
	}

	return Result;
}	

bool 
Run (
	CModule* pModule,
	const char* pFunctionName,
	int* pReturnValue
	)
{
	bool Result;

	CModuleItem* pFunctionItem = pModule->m_NamespaceMgr.GetGlobalNamespace ()->FindItem (pFunctionName);
	if (!pFunctionItem || pFunctionItem->GetItemKind () != EModuleItem_Function)
	{
		err::SetFormatStringError ("'%s' is not found or not a function\n", pFunctionName);
		return false;
	}
	
	CFunction* pFunction = (CFunction*) pFunctionItem;

	CFunction* pConstructor = pModule->GetConstructor ();
	if (pConstructor)
	{
		Result = RunFunction (pConstructor);
		if (!Result)
			return false;
	}

	Result = RunFunction (pFunction, pReturnValue);
	if (!Result)
		return false;

	CFunction* pDestructor = pModule->GetDestructor ();
	if (pDestructor)
	{
		Result = RunFunction (pDestructor);
		if (!Result)
			return false;
	}
		
	return true;
}

bool
PrintLlvmIr (
	CModule* pModule,
	FILE* pFile
	)
{
	uint_t CommentMdKind = pModule->m_LlvmBuilder.GetCommentMdKind ();

	rtl::CIteratorT <CFunction> Function = pModule->m_FunctionMgr.GetFunctionList ().GetHead ();
	for (; Function; Function++)
	{
		if (Function->GetFlags () & EModuleItemFlag_Orphan)
			continue;

		CFunctionType* pFunctionType = Function->GetType (); 

		fprintf (pFile, "%s %s %s %s\n", 
			pFunctionType->GetReturnType ()->GetTypeString ().cc (),
			GetCallConvString (pFunctionType->GetCallConv ()),
			Function->m_Tag.cc (), 
			pFunctionType->GetArgString ().cc ()
			);

		llvm::Function* pLlvmFunction = Function->GetLlvmFunction ();
		llvm::Function::BasicBlockListType& BlockList = pLlvmFunction->getBasicBlockList ();
		llvm::Function::BasicBlockListType::iterator Block = BlockList.begin ();

		for (; Block != BlockList.end (); Block++)
		{
			std::string Name = Block->getName ();
			fprintf (pFile, "%s:\n", Name.c_str ());

			llvm::BasicBlock::InstListType& InstList = Block->getInstList ();
			llvm::BasicBlock::InstListType::iterator Inst = InstList.begin ();
			for (; Inst != InstList.end (); Inst++)
			{
				std::string String;
				llvm::raw_string_ostream Stream (String);

				llvm::MDNode* pMdComment = Inst->getMetadata (CommentMdKind);
				if (pMdComment)
					Inst->setMetadata (CommentMdKind, NULL); // remove before print

				Inst->print (Stream);

				fprintf (pFile, "%s\n", String.c_str ());

				if (pMdComment)
				{
					Inst->setMetadata (CommentMdKind, pMdComment); // restore
					llvm::MDString* pMdString = (llvm::MDString*) pMdComment->getOperand (0);
					fprintf (pFile, "\n  ; %s\n", pMdString->getString ().data ());
				}
			}
		}

		fprintf (pFile, "\n........................................\n\n");
	}

	return true;
}

bool 
PrintDisassembly (
	CModule *module,
	FILE* pFile
	)
{
	CDisassembler Dasm;

	rtl::CIteratorT <CFunction> Function = module->m_FunctionMgr.GetFunctionList ().GetHead ();
	for (; Function; Function++)
	{
		if (Function->GetFlags () & EModuleItemFlag_Orphan)
			continue;

		CFunctionType* pFunctionType = Function->GetType (); 

		fprintf (
			pFile,
			"%s %s %s %s\n", 
			pFunctionType->GetReturnType ()->GetTypeString ().cc (),
			GetCallConvString (pFunctionType->GetCallConv ()),
			Function->m_Tag.cc (), 
			pFunctionType->GetArgString ().cc ()
			);

		void* pf = Function->GetMachineCode ();
		size_t Size = Function->GetMachineCodeSize ();

		if (pf)
		{
			rtl::CString s = Dasm.Disassemble (pf, Size);
			fprintf (pFile, "\n%s", s.cc ());
		}
		
		fprintf (pFile, "\n........................................\n\n");
	}

	return true;
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

	// analyze command line

	rtl::CString SrcFileName;
	rtl::CString DstFileName;
	rtl::CString RunFunctionName;

	uint_t Flags = 0;

	for (int i = 1; i < argc; i++)
	{
		if (argv [i] [0] == '-') // switch
		{			
			switch (argv [i] [1])
			{
			case '?': case 'h': case 'H':
				PrintUsage ();
				return EJncError_Success;

			case 'v':
				PrintVersion ();
				return EJncError_Success;
				
			case 'o':
				DstFileName = argv [i] [2] ? &argv [i] [2] : ++i < argc ? argv [i] : NULL;
				break;

			case 'r':
				Flags |= EJncFlag_Run;
				RunFunctionName = argv [i] [2] ? &argv [i] [2] : ++i < argc ? argv [i] : NULL;
				break;
				
			default:
				printf ("unknown switch '-%c'\n", argv [i] [1]);
				PrintUsage (false);
				return EJncError_InvalidSwitch;				
			}				
		}
		else
		{
			SrcFileName = argv [i];
		}
	}

	if (SrcFileName.IsEmpty ())
	{
		PrintUsage ();
		return EJncError_NoSourceFile;
	}
	
	io::CMappedFile SrcFile;
	Result = SrcFile.Open (SrcFileName, io::EFileFlag_ReadOnly);
	const void* p = Result ? SrcFile.View () : NULL;
	if (!p)
	{
		printf (
			"Cannot open '%s': %s\n", 
			SrcFileName.cc (), // thanks a lot gcc
			err::GetError ()->GetDescription ().cc () 
			);
		return EJncError_IoFailure;
	}

	rtl::CString SrcFilePath = io::GetFullFilePath (SrcFileName);

	CModule Module;
	Result = 
		Compile (&Module, SrcFilePath, (const char*) p, (size_t) SrcFile.GetSize ()) &&
		Jit (&Module);
	
	if (!Result)
	{
		printf ("%s\n", err::GetError ()->GetDescription ().cc ());
		return EJncError_CompileFailure;
	}
	
	if (Flags & EJncFlag_Run)
	{	
		const char* pFunctionName = !RunFunctionName.IsEmpty () ? RunFunctionName.cc () : "main";
		
		int ReturnValue;
		Result = Run (&Module, pFunctionName, &ReturnValue);
		if (!Result)
		{
			printf ("%s\n", err::GetError ()->GetDescription ().cc ());
			return EJncError_RunFailure;
		}

		printf ("'%s' returned (%d).\n", pFunctionName, ReturnValue);
	}
	else if (DstFileName.IsEmpty ())
	{
		PrintLlvmIr (&Module, stdout);
		PrintDisassembly (&Module, stdout);
	}
	else
	{
		FILE* pFile = fopen (DstFileName, "wt");
		if (!pFile)
		{
			printf (
				"Cannot open '%s': %s\n", 
				DstFileName.cc (), 
				err::CError (err::GUID_Errno, errno).GetDescription ().cc ()
				);

			return EJncError_IoFailure;
		}

		PrintLlvmIr (&Module, pFile);
		PrintDisassembly (&Module, pFile);
		fclose (pFile);
	}
	
	return EJncError_Success;
}

//.............................................................................
