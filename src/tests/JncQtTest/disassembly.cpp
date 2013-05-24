#include "pch.h"
#include "disassembly.h"
#include "moc_disassembly.cpp"

Disassembly::Disassembly(QWidget *parent)
	: DisassemblyBase(parent)
{
	setReadOnly(true);
	setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
	setLineWrapMode (QPlainTextEdit::NoWrap);
}

bool Disassembly::build(jnc::CModule *module)
{
	clear();
	
	jnc::CDisassembler Dasm;

	rtl::CIteratorT <jnc::CFunction> Function = module->m_FunctionMgr.GetFunctionList ().GetHead ();
	for (; Function; Function++)
	{
		jnc::CFunctionType* pFunctionType = Function->GetType (); 

		appendFormat (
			"%s %s %s %s\n", 
			pFunctionType->GetReturnType ()->GetTypeString (),
			jnc::GetCallConvString (pFunctionType->GetCallConv ()),
			Function->m_Tag, 
			pFunctionType->GetArgString ()
			);

		jnc::CFunction* pExternFunction = Function->GetExternFunction ();
		if (pExternFunction)
		{
			appendFormat ("  ->%s\n", pExternFunction->m_Tag);
			appendFormat ("\n");
			continue;
		}

		void* pf = Function->GetMachineCode ();
		size_t Size = Function->GetMachineCodeSize ();

		if (pf)
		{
			rtl::CString s = Dasm.Disassemble (pf, Size);
			appendFormat ("\n%s", s);
		}
		
		appendFormat ("\n........................................\n\n");
	}

	return true;
}
