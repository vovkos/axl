#include "stdafx.h"
#include "test_ast.h"
#include "MainFrm.h"
#include "LlvmIrPane.h"
#include "AstDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//.............................................................................

BEGIN_MESSAGE_MAP(CLlvmIrPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CLlvmIrPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_LogCtrl.Create(
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST));
	
	m_LogCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_LogCtrl.SetFont (&theApp.m_Font);

	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

void CLlvmIrPane::OnSize(UINT nType, int cx, int cy) 
{
	m_LogCtrl.MoveWindow(0, 0, cx, cy);
}

bool
CLlvmIrPane::Build (jnc::CModule* pModule)
{
	Clear ();

	rtl::CIteratorT <jnc::CFunction> Function = pModule->m_FunctionMgr.GetFirstFunction ();
	for (; Function; Function++)
	{
		m_LogCtrl.Trace (_T("@%s ()\r\n"), Function->GetTag ());

		jnc::CFunction* pExternFunction = Function->GetExternFunction ();
		if (pExternFunction)
		{
			m_LogCtrl.Trace (_T("  ->%s\r\n"), pExternFunction->GetTag ());
			m_LogCtrl.Trace (_T("\r\n"));
			continue;
		}

		llvm::Function* pLlvmFunction = Function->GetLlvmFunction ();
		llvm::Function::BasicBlockListType& BlockList = pLlvmFunction->getBasicBlockList ();
		llvm::Function::BasicBlockListType::iterator Block = BlockList.begin ();

		for (; Block != BlockList.end (); Block++)
		{
			std::string Name = Block->getName ();
			m_LogCtrl.Trace (_T("%s\r\n"), Name.c_str ());

			llvm::BasicBlock::InstListType& InstList = Block->getInstList ();
			llvm::BasicBlock::InstListType::iterator& Inst = InstList.begin ();
			for (; Inst != InstList.end (); Inst++)
			{
				std::string String;
				llvm::raw_string_ostream Stream (String);

				llvm::Instruction* pInst = Inst;
				pInst->print (Stream);

				m_LogCtrl.Trace (_T("%s\r\n"), String.c_str ());
			}
		}

		m_LogCtrl.Trace (_T("\r\n"));
	}

	return true;
}

//.............................................................................
