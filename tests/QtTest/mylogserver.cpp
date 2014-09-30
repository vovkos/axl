#include "pch.h"
#include "mylogserver.h"

//.............................................................................

bool myLogServer::start (
	log::ClientPeer* client,
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

	return create (
		client,
		&m_logRepresenter,
		&m_logColorizer,
		logPacketFilePath,
		logMergeFilePath,
		logColorizerStateFilePath
		);
}

#ifdef _JANCY_REPRESENTER

bool myLogServer::compile (
	const char* representerClassName,
	size_t fileCount,
	...
	)
{
	AXL_VA_DECL (va, fileCount);

	qApp->setCursorFlashTime (0);

	bool result;

	llvm::LLVMContext* llvmContext = new llvm::LLVMContext;
	llvm::module* llvmModule = new llvm::module ("llvmMainModule", *llvmContext);
	m_module.create ("jncMainModule", llvmModule);

	jnc::ScopehreadModule scopeModule (&m_module);

	for (size_t i = 0; i < fileCount; i++)
	{
		const char* filePath = AXL_VA_ARG (va, const char*);

		printf("Parsing %s...\n", filePath);

		io::MappedFile file;
		result = file.open (filePath, io::FileFlagKind_ReadOnly);
		if (!result)
			return false;

		jnc::Lexer lexer;
		lexer.create (
			filePath,
			(const char*) file.view (),
			file.getSize ()
			);

		jnc::Parser parser;
		parser.create (jnc::Parser::startSymbol, true);

		for (;;)
		{
			const jnc::Token* token = lexer.getToken ();
			if (token->m_token == jnc::TokenKind_Eof)
				break;

			result = parser.parseToken (token);
			if (!result)
			{
				rtl::String text = err::getError ()->getDescription ();

				printf(
					"%s(%d,%d): %s\n",
					filePath,
					token->m_pos.m_line + 1,
					token->m_pos.m_col + 1,
					text.cc ()
					);
				return false;
			}

			lexer.nextToken ();
		}
	}

	printf("Compiling...\n");

	result = m_module.compile ();
	if (!result)
	{
		printf("%s\n", err::getError ()->getDescription ().cc ());
		return false;
	}

#if (_AXL_ENV == AXL_ENV_WIN)
	jnc::JitKind jitKind = jnc::JitKind_Normal;
#else
	jnc::JitKind jitKind = jnc::JitKind_McJit;
#endif

	printf("JITting with '%s'...\n", jnc::getJitKindString (jitKind));

	result = m_runtime.create (&m_module, &m_stdlib, jitKind, 16, 1, 8);
	if (!result)
	{
		printf("%s\n", err::getError ()->getDescription ().cc ());
		return false;
	}

	if (jitKind == jnc::JitKind_Normal)
	{
		llvm::executionEngine* llvmExecutionEngine = m_runtime.getLlvmExecutionEngine ();
		jnc::StdLib::export (&m_module, llvmExecutionEngine);
		m_module.setFunctionPointer (llvmExecutionEngine, "printf", (void*) stdLib::printf);
		m_module.setFunctionPointer (llvmExecutionEngine, "rand", (void*) rand);
	}

	result = m_module.m_functionMgr.jitFunctions (m_runtime.getLlvmExecutionEngine ());
	if (!result)
	{
		printf("%s\n", err::getError ()->getDescription ().cc ());
		return false;
	}

	printf ("Creating representer '%s'...\n", representerClassName);

	jnc::ModuleItem* representerTypeItem = m_module.m_namespaceMgr.getGlobalNamespace ()->findItem (representerClassName);
	if (!representerTypeItem)
	{
		printf ("'%s' is not found\n", representerClassName);
		return false;
	}

	if (representerTypeItem->getItemKind () != jnc::ModuleItemKind_Type ||
		((jnc::Type*) representerTypeItem)->getTypeKind () != jnc::TypeKind_Class)
	{
		printf ("'%s' is not a class\n", representerClassName);
		return false;
	}

	result = m_logRepresenter.create ((jnc::Classype*) representerTypeItem, &m_runtime);
	if (!result)
	{
		printf ("%s\n", err::getError ()->getDescription ().cc ());
		return false;
	}

	printf ("Done.\n");
	return true;
}

#endif

//.............................................................................
