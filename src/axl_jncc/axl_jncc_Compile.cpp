#include "pch.h"
#include "axl_jncc.h"

//.............................................................................

bool 
CJnc::Compile (
	const char* pFileName,
	const char* pSource,
	size_t Length
	)
{
	bool Result;

	llvm::Module* pLlvmModule = new llvm::Module (pFileName, llvm::getGlobalContext ());

	uint_t ModuleFlags = 0;
	if (m_pCmdLine->m_Flags & EJncFlag_DebugInfo)
		ModuleFlags |= jnc::EModuleFlag_DebugInfo;

	m_Module.Create (pFileName, pLlvmModule, ModuleFlags);

	jnc::CScopeThreadModule ScopeModule (&m_Module);
	
	jnc::CLexer Lexer;
	Lexer.Create (pFileName, pSource, Length);

	jnc::CParser Parser;
	Parser.Create (jnc::CParser::StartSymbol, true);

	for (;;)
	{
		const jnc::CToken* pToken = Lexer.GetToken ();
		if (pToken->m_Token == jnc::EToken_Eof)
			break;

		Result = Parser.ParseToken (pToken);
		if (!Result)
		{			
			err::PushSrcPosError (pFileName, pToken->m_Pos);
			return false;
		}

		Lexer.NextToken ();
	}

	Result = m_Module.Compile ();
	if (!Result)
		return false;

	return true;
}

bool 
CJnc::Jit ()
{
	jnc::EJit JitKind = (m_pCmdLine->m_Flags & EJncFlag_Jit_mc) ? jnc::EJit_McJit : jnc::EJit_Normal;

	bool Result = m_Runtime.Create (&m_Module, &m_StdLib, JitKind, 16, 1, 4);
	if (!Result)
		return false;
		
	if (JitKind == jnc::EJit_Normal)
	{
		llvm::ExecutionEngine* pLlvmExecutionEngine = m_Runtime.GetLlvmExecutionEngine ();
		jnc::CStdLib::Export (&m_Module, pLlvmExecutionEngine);
		m_Module.SetFunctionPointer (pLlvmExecutionEngine, "printf", (void*) CStdLib::Printf);
	}

	return m_Module.m_FunctionMgr.JitFunctions (m_Runtime.GetLlvmExecutionEngine ());	
}

void
CJnc::PrintLlvmIr ()
{
	if (!(m_pCmdLine->m_Flags & EJncFlag_LlvmIr_c))
	{
		m_pOutStream->Printf ("%s", m_Module.GetLlvmIrString ().cc ());
		return;
	}

	uint_t CommentMdKind = m_Module.m_LlvmIrBuilder.GetCommentMdKind ();

	rtl::CIteratorT <jnc::CFunction> Function = m_Module.m_FunctionMgr.GetFunctionList ().GetHead ();
	for (; Function; Function++)
	{
		if (Function->GetFlags () & jnc::EModuleItemFlag_Orphan)
			continue;

		jnc::CFunctionType* pFunctionType = Function->GetType (); 

		m_pOutStream->Printf ("%s %s %s %s\n", 
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
			m_pOutStream->Printf ("%s:\n", Name.c_str ());

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

				m_pOutStream->Printf ("%s\n", String.c_str ());

				if (pMdComment)
				{
					Inst->setMetadata (CommentMdKind, pMdComment); // restore
					llvm::MDString* pMdString = (llvm::MDString*) pMdComment->getOperand (0);
					m_pOutStream->Printf ("\n  ; %s\n", pMdString->getString ().data ());
				}
			}
		}

		m_pOutStream->Printf ("\n........................................\n\n");
	}
}

void 
CJnc::PrintDisassembly ()
{
	jnc::CDisassembler Dasm;

	rtl::CIteratorT <jnc::CFunction> Function = m_Module.m_FunctionMgr.GetFunctionList ().GetHead ();
	for (; Function; Function++)
	{
		if (Function->GetFlags () & jnc::EModuleItemFlag_Orphan)
			continue;

		jnc::CFunctionType* pFunctionType = Function->GetType (); 

		m_pOutStream->Printf (
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
			m_pOutStream->Printf ("\n%s", s.cc ());
		}
		
		m_pOutStream->Printf ("\n........................................\n\n");
	}
}

//.............................................................................

