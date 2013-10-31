#include "pch.h"
#include "mylogserver.h"

//.............................................................................

bool MyLogServer::start (
	log::IClient* client,
	const char* logPacketFilePath,
	const char* logMergeFilePath,
	const char* logColorizerStateFilePath
	)
{
#ifdef _JANCY_REPRESENTER
	bool result = compile (
		"CMyRepresenter",
		4,
		"d:/Prj/Ninja/axl/src/tests/QtTest/script/std.jnc",
		"d:/Prj/Ninja/axl/src/tests/QtTest/script/gui.jnc",
		"d:/Prj/Ninja/axl/src/tests/QtTest/script/log.jnc",
		"d:/Prj/Ninja/axl/src/tests/QtTest/script/mylogrepresenter.jnc"
		);
	if (!result)
		return false;
#endif

	return Create (
		client,
		&m_logRepresenter,
		&m_logColorizer,
		logPacketFilePath,
		logMergeFilePath,
		logColorizerStateFilePath
		);
}

#ifdef _JANCY_REPRESENTER

bool MyLogServer::compile (
	const char* representerClassName,
	size_t FileCount,
	...
	)
{
	AXL_VA_DECL (va, FileCount);

	qApp->setCursorFlashTime (0);

	bool result;

	llvm::LLVMContext* llvmContext = new llvm::LLVMContext;
	llvm::Module* llvmModule = new llvm::Module ("llvmMainModule", *llvmContext);
	m_module.Create ("jncMainModule", llvmModule);

	jnc::CScopeThreadModule ScopeModule (&m_module);

	for (size_t i = 0; i < FileCount; i++)
	{
		const char* filePath = AXL_VA_ARG (va, const char*);

		printf("Parsing %s...\n", filePath);

		io::CMappedFile file;
		result = file.Open (filePath, io::EFileFlag_ReadOnly);
		if (!result)
			return false;

		jnc::CLexer Lexer;
		Lexer.Create (
			filePath,
			(const char*) file.View (),
			file.GetSize ()
			);

		jnc::CParser Parser;
		Parser.Create (jnc::CParser::StartSymbol, true);

		for (;;)
		{
			const jnc::CToken* pToken = Lexer.GetToken ();
			if (pToken->m_Token == jnc::EToken_Eof)
				break;

			result = Parser.ParseToken (pToken);
			if (!result)
			{
				rtl::CString Text = err::GetError ()->GetDescription ();

				printf(
					"%s(%d,%d): %s\n",
					filePath,
					pToken->m_Pos.m_Line + 1,
					pToken->m_Pos.m_Col + 1,
					Text.cc ()
					);
				return false;
			}

			Lexer.NextToken ();
		}
	}

	printf("Compiling...\n");

	result = m_module.Compile ();
	if (!result)
	{
		printf("%s\n", err::GetError ()->GetDescription ().cc ());
		return false;
	}

#if (_AXL_ENV == AXL_ENV_WIN)
	jnc::EJit JitKind = jnc::EJit_Normal;
#else
	jnc::EJit JitKind = jnc::EJit_McJit;
#endif

	printf("JITting with '%s'...\n", jnc::GetJitKindString (JitKind));

	result = m_runtime.Create (&m_module, &m_stdlib, JitKind, 16, 1, 8);
	if (!result)
	{
		printf("%s\n", err::GetError ()->GetDescription ().cc ());
		return false;
	}

	if (JitKind == jnc::EJit_Normal)
	{
		llvm::ExecutionEngine* llvmExecutionEngine = m_runtime.GetLlvmExecutionEngine ();
		jnc::CStdLib::Export (&m_module, llvmExecutionEngine);
		m_module.SetFunctionPointer (llvmExecutionEngine, "printf", (void*) StdLib::Printf);
		m_module.SetFunctionPointer (llvmExecutionEngine, "rand", (void*) rand);
	}

	result = m_module.m_FunctionMgr.JitFunctions (m_runtime.GetLlvmExecutionEngine ());
	if (!result)
	{
		printf("%s\n", err::GetError ()->GetDescription ().cc ());
		return false;
	}

	printf ("Creating representer '%s'...\n", representerClassName);

	jnc::CModuleItem* representerTypeItem = m_module.m_NamespaceMgr.GetGlobalNamespace ()->FindItem (representerClassName);
	if (!representerTypeItem)
	{
		printf ("'%s' is not found\n", representerClassName);
		return false;
	}

	if (representerTypeItem->GetItemKind () != jnc::EModuleItem_Type ||
		((jnc::CType*) representerTypeItem)->GetTypeKind () != jnc::EType_Class)
	{
		printf ("'%s' is not a class\n", representerClassName);
		return false;
	}

	result = m_logRepresenter.Create ((jnc::CClassType*) representerTypeItem, &m_runtime);
	if (!result)
	{
		printf ("%s\n", err::GetError ()->GetDescription ().cc ());
		return false;
	}

	printf ("Done.\n");
	return true;
}

#endif

//.............................................................................
