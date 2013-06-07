#include "pch.h"
#include "llvmir.h"
#include "llvmirhighlighter.h"
#include "moc_llvmir.cpp"

LlvmIr::LlvmIr(QWidget *parent)
	: LlvmIrBase(parent)
{
	setReadOnly(true);
	setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
	setLineWrapMode (QPlainTextEdit::NoWrap);
	setupHighlighter();	
}

bool LlvmIr::build(jnc::CModule *module)
{
	clear ();

	uint_t CommentMdKind = module->m_LlvmBuilder.GetCommentMdKind ();

	rtl::CIteratorT <jnc::CFunction> Function = module->m_FunctionMgr.GetFunctionList ().GetHead ();
	for (; Function; Function++)
	{
		jnc::CFunctionType* pFunctionType = Function->GetType (); 

		appendFormat ("%s %s %s %s\n", 
			pFunctionType->GetReturnType ()->GetTypeString ().cc (),
			jnc::GetCallConvString (pFunctionType->GetCallConv ()),
			Function->m_Tag.cc (), 
			pFunctionType->GetArgString ().cc ()
			);

		jnc::CFunction* pExternFunction = Function->GetExternFunction ();
		if (pExternFunction)
		{
			appendFormat ("  ->%s\n", pExternFunction->m_Tag.cc ());
			appendFormat ("\n");
			continue;
		}

		llvm::Function* pLlvmFunction = Function->GetLlvmFunction ();
		llvm::Function::BasicBlockListType& BlockList = pLlvmFunction->getBasicBlockList ();
		llvm::Function::BasicBlockListType::iterator Block = BlockList.begin ();

		for (; Block != BlockList.end (); Block++)
		{
			std::string Name = Block->getName ();
			appendFormat ("%s\n", Name.c_str ());

			llvm::BasicBlock::InstListType& InstList = Block->getInstList ();
			llvm::BasicBlock::InstListType::iterator Inst = InstList.begin ();
			for (; Inst != InstList.end (); Inst++)
			{
				std::string String;
				llvm::raw_string_ostream Stream (String);

				llvm::Instruction* pInst = Inst;

				llvm::MDNode* pMdComment = pInst->getMetadata (CommentMdKind);
				if (pMdComment)
					pInst->setMetadata (CommentMdKind, NULL); // remove before print

				pInst->print (Stream);

				appendFormat ("%s\n", String.c_str ());

				if (pMdComment)
				{
					pInst->setMetadata (CommentMdKind, pMdComment); // restore
					llvm::MDString* pMdString = (llvm::MDString*) pMdComment->getOperand (0);
					appendFormat ("\n; %s\n", pMdString->getString ().data ());
				}
			}
		}

		appendFormat ("\n........................................\n\n");
	}

	return true;
}


void LlvmIr::setupHighlighter()
{
	highlighter = new LlvmIrHighlighter(document());
}