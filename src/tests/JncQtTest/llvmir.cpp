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
	clear();
	
	rtl::CIteratorT<jnc::CFunction> function = module->m_FunctionMgr.GetFunctionList().GetHead ();
	for (; function; function++) {
		QString text;
		text.sprintf("@%s ()", function->m_Tag.cc ());
		appendPlainText(text);

		llvm::Function *llvmFunction = function->GetLlvmFunction();
		llvm::Function::BasicBlockListType &blockList = llvmFunction->getBasicBlockList();
		llvm::Function::BasicBlockListType::iterator block = blockList.begin();

		for (; block != blockList.end(); block++) {
			text.sprintf("%s:", block->getName());
			appendPlainText(text);

			llvm::BasicBlock::InstListType &instList = block->getInstList();
			llvm::BasicBlock::InstListType::iterator instIt = instList.begin();
			for (; instIt != instList.end(); instIt++) {
				std::string string;
				llvm::raw_string_ostream stream(string);

				llvm::Instruction *inst = instIt;
				inst->print(stream);

				appendPlainText(string.c_str());
			}
		}
	}

	return true;
}

void LlvmIr::setupHighlighter()
{
	highlighter = new LlvmIrHighlighter(document());
}