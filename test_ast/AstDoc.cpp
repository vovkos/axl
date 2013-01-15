// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// AstDoc.cpp : implementation of the CAstDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "test_ast.h"
#endif

#include "MainFrm.h"

#include "AstDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAstDoc

IMPLEMENT_DYNCREATE(CAstDoc, CDocument)

BEGIN_MESSAGE_MAP(CAstDoc, CDocument)
	ON_COMMAND(ID_FILE_COMPILE, OnFileCompile) 
	ON_COMMAND(ID_FILE_RUN, OnFileRun) 
END_MESSAGE_MAP()

// CAstDoc construction/destruction

CAstDoc::CAstDoc()
{
	m_pLlvmExecutionEngine = NULL;
}

CAstDoc::~CAstDoc()
{
}

void CAstDoc::OnFileCompile()
{
	Compile ();
}

void CAstDoc::OnFileRun ()
{
	Run ();
}

bool
CAstDoc::Compile ()
{
	DoFileSave ();

	bool Result;

	CMainFrame* pMainFrame = GetMainFrame ();

	if (m_pLlvmExecutionEngine)
	{
		delete m_pLlvmExecutionEngine;
		m_pLlvmExecutionEngine = NULL;
	}	

	CString FilePath = GetPathName ();
	m_Module.Create ((const tchar_t*) FilePath);
	
	llvm::Module* pLlvmModule = new llvm::Module ((const tchar_t*) FilePath, llvm::getGlobalContext ());
	m_Module.m_pLlvmModule = pLlvmModule;

	llvm::EngineBuilder EngineBuilder (pLlvmModule);	
	std::string ErrorString;
	EngineBuilder.setErrorStr (&ErrorString);
	EngineBuilder.setUseMCJIT(true);

	m_pLlvmExecutionEngine = EngineBuilder.create ();
	if (!m_pLlvmExecutionEngine)
	{
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Error creating a JITter (%s)\n"), ErrorString.c_str ());
		return false;
	}

	jnc::CSetCurrentThreadModule ScopeModule (&m_Module);

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Parsing...\n"));
	pMainFrame->m_GlobalAstPane.Clear ();
	pMainFrame->m_FunctionAstPane.Clear ();
	pMainFrame->m_ModulePane.Clear ();
	pMainFrame->m_LlvmIrPane.Clear ();
	pMainFrame->m_DasmPane.Clear ();

	GetView ()->GetWindowText (m_SourceText);

	jnc::CLexer Lexer;
	Lexer.Create ((const tchar_t*) m_strPathName, m_SourceText, m_SourceText.GetLength ());

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
			rtl::CString Text = err::GetError ()->GetDescription ();
			pMainFrame->m_OutputPane.m_LogCtrl.Trace (
				_T("%s(%d,%d): %s\n"), 
				m_strPathName, 
				pToken->m_Pos.m_Line + 1, 
				pToken->m_Pos.m_Col + 1, 
				Text
				);
			return false;
		}

		Lexer.NextToken ();
	}

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Resolving imports...\n"));
	Result = m_Module.m_TypeMgr.ResolveImports ();
	if (!Result)
	{
		rtl::CString Text = err::GetError ()->GetDescription ();
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("%s\n"), Text);
		return false;
	}

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Allocating globals...\n"));
	Result = m_Module.m_VariableMgr.AllocateGlobalVariables ();
	if (!Result)
	{
		rtl::CString Text = err::GetError ()->GetDescription ();
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("%s\n"), Text);
		return false;
	}

	pMainFrame->m_GlobalAstPane.Build (Parser.GetAst ());
	pMainFrame->m_ModulePane.Build (&m_Module);

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Compiling functions...\n"));
	Result = m_Module.m_FunctionMgr.CompileFunctions ();
	if (!Result)
	{
		rtl::CString Text = err::GetError ()->GetDescription ();
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("%s\n"), Text);
	}

	// show compiled IM nevetheless

	pMainFrame->m_LlvmIrPane.Build (&m_Module);

	if (!Result)
		return false;

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("JITting functions...\n"));

	ExportStdLib ();

	Result = m_Module.m_FunctionMgr.JitFunctions (m_pLlvmExecutionEngine);
	if (!Result)
	{
		rtl::CString Text = err::GetError ()->GetDescription ();
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("%s\n"), Text);
		return false;
	}

	pMainFrame->m_DasmPane.Build (&m_Module);
	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Done.\n"));
	return true;
}

bool
CAstDoc::Run ()
{
	bool Result;

	CMainFrame* pMainFrame = GetMainFrame ();

	if (IsModified ())
	{
		Result = Compile ();
		if (!Result)
			return false;
	}

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Finding 'main'...\n"));

	jnc::CFunction* pFunction = FindGlobalFunction (_T("main"));
	if (!pFunction)
	{
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("'main' is not found or not a function\n"));
		return false;
	}
		
	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Running...\n"));

	typedef int (*FMain) ();
	FMain pfnMain = (FMain) pFunction->GetFunctionPointer ();
	ASSERT (pfnMain);

	try
	{
		int Result = pfnMain ();
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Done (retval = %d).\n"), Result);
	}
	catch (err::CError Error)
	{
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("ERROR: %s\n"), Error.GetDescription ());
	}

	return true;
}

void
StdLib_OnRuntimeError (
	int Error,
	void* pCodeAddr,
	void* pDataAddr
	)
{
	const tchar_t* pErrorString;

	switch (Error)
	{
	case jnc::ERuntimeError_LoadOutOfRange:
		pErrorString = "READ_OOR";
		break;

	case jnc::ERuntimeError_StoreOutOfRange:
		pErrorString = "WRITE_OOR";
		break;

	case jnc::ERuntimeError_ScopeMismatch:
		pErrorString = "SCOPE_MISMATCH";
		break;

	case jnc::ERuntimeError_NullInterface:
		pErrorString = "NULL_INTERFACE";
		break;

	case jnc::ERuntimeError_NullFunction:
		pErrorString = "NULL_FUNCTION";
		break;

	case jnc::ERuntimeError_NullProperty:
		pErrorString = "NULL_PROPERTY";
		break;

	default:
		ASSERT (false);
		pErrorString = "<UNDEF>";
	}
	
	throw err::CError (
		_T("RUNTIME ERROR: %s (code %x accessing data %x)"), 
		pErrorString,
		pCodeAddr,
		pDataAddr
		);
}

jnc::TInterfaceHdr* 
StdLib_DynamicCastInterface (
	jnc::TInterfaceHdr* p,
	jnc::CClassType* pType
	)
{
	if (!p)
		return NULL;

	if (p->m_pObject->m_pType->Cmp (pType) == 0)
		return p;
	
	jnc::CClassBaseTypeCoord Coord;
	bool Result = p->m_pObject->m_pType->FindBaseType (pType, &Coord);
	if (!Result)
		return NULL;
	
	jnc::TInterfaceHdr* p2 = (jnc::TInterfaceHdr*) ((uchar_t*) (p->m_pObject + 1) + Coord.m_FieldCoord.m_Offset);
	ASSERT (p2->m_pObject == p->m_pObject);

	return p2;
}

void
StdLib_EventOperator (
	jnc::TEvent* pEvent,
	void* pfn,
	jnc::TInterfaceHdr* pIface,
	jnc::EEventOp OpKind
	)
{
	jnc::TEventHandler* pHandler;

	switch (OpKind)
	{
	case jnc::EEventOp_SetHandler:
		pHandler = AXL_MEM_NEW (jnc::TEventHandler);
		pHandler->m_FunctionPtr.m_pfn = pfn;
		pHandler->m_FunctionPtr.m_pIface = pIface;
		pHandler->m_pPrev = NULL;
		pHandler->m_pNext = NULL;

		pEvent->m_pHead = pHandler;
		pEvent->m_pTail = pHandler;
		break;

	case jnc::EEventOp_AddHandler:
		pHandler = AXL_MEM_NEW (jnc::TEventHandler);
		pHandler->m_FunctionPtr.m_pfn = pfn;
		pHandler->m_FunctionPtr.m_pIface = pIface;
		pHandler->m_pPrev = pEvent->m_pTail;
		pHandler->m_pNext = NULL;

		if (!pEvent->m_pHead)
			pEvent->m_pHead = pHandler;

		if (pEvent->m_pTail)
			pEvent->m_pTail->m_pNext = pHandler;

		pEvent->m_pTail = pHandler;
		break;

	case jnc::EEventOp_RemoveHandler:
		pHandler = pEvent->m_pHead;
		for (; pHandler; pHandler = pHandler->m_pNext)
		{
			if (pHandler->m_FunctionPtr.m_pfn == pfn &&
				pHandler->m_FunctionPtr.m_pIface == pIface)
			{
				if (pHandler->m_pPrev)
					pHandler->m_pPrev->m_pNext = pHandler->m_pNext;

				if (pHandler->m_pNext)
					pHandler->m_pNext->m_pPrev = pHandler->m_pPrev;

				if (pEvent->m_pHead == pHandler)
					pEvent->m_pHead = pHandler->m_pNext;

				if (pEvent->m_pTail == pHandler)
					pEvent->m_pTail = pHandler->m_pPrev;
			}
		}

		break;

	default:
		ASSERT (false);
	}
}

void
StdLib_FireSimpleEvent (jnc::TEvent* pEvent)
{
	typedef void (*FEventHandler) (jnc::TInterfaceHdr*);

	jnc::TEventHandler* pHandler = pEvent->m_pHead;

	for (; pHandler; pHandler = pHandler->m_pNext)
	{
		FEventHandler pfn = (FEventHandler) pHandler->m_FunctionPtr.m_pfn;
		pfn (pHandler->m_FunctionPtr.m_pIface);
	}
}

void*
StdLib_HeapAllocate (jnc::CType* pType)
{
	void* p;

	if (pType->GetTypeKind () == jnc::EType_Class)
	{
		jnc::CClassType* pClassType = (jnc::CClassType*) pType;
		p = malloc (pClassType->GetClassStructType ()->GetSize ());
	}
	else
	{
		p = malloc (pType->GetSize ());
		memset (p, 0, pType->GetSize ());
	}

	return p;
}

void
StdLib_printf (
	const char* pFormat,
	...
	)
{
	CMainFrame* pMainFrame = GetMainFrame ();
	pMainFrame->m_OutputPane.m_LogCtrl.TraceV (pFormat, va_start_e (pFormat));
}

int
StdLib_StructTest (
	void* p,
	size_t Size
	)
{
	CMainFrame* pMainFrame = GetMainFrame ();

	pMainFrame->m_OutputPane.m_LogCtrl.Trace ("StructTest (%x, %d)\n", p, Size);
	/*
	struct TBase
	{
		int m_a;
		int m_b;
	};

	struct TStruct: TBase
	{
		int m_c;
		int m_d;
	};
	*/

	struct TStruct
	{
		unsigned __int16 m_a:1;
		unsigned __int16 m_b:2;
		unsigned __int16 m_c:5;
		unsigned __int16 m_d:7;
	};

	union TUnion
	{
		TStruct m_s;
		unsigned __int16 m_i;
	};

	TStruct* pStruct = (TStruct*) p;
	TUnion* pUnion = (TUnion*) p;

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (
		"TStruct = { %x, %x, %x, %x }; Size = %d; sizeof (TStruct) = %d;\n", 
		pStruct->m_a, pStruct->m_b, pStruct->m_c, pStruct->m_d,
		Size,
		sizeof (TStruct)
		);

	return 0;
}

int
StdLib_ReadInteger ()
{
	return 10;
}

int
__cdecl
StdLib_CallConvTestA (
	int a,
	int b, 
	int c
	)
{
	CMainFrame* pMainFrame = GetMainFrame ();
	pMainFrame->m_OutputPane.m_LogCtrl.Trace ("CallConvTestA (%d, %d, %d)\n", a, b, c);
	return 1000;
}

int
__cdecl
StdLib_CallConvTestB (
	int a,
	int b, 
	int c
	)
{
	CMainFrame* pMainFrame = GetMainFrame ();
	pMainFrame->m_OutputPane.m_LogCtrl.Trace ("CallConvTestB (%d, %d, %d)\n", a, b, c);
	return 2000;
}

int
__stdcall
StdLib_CallConvTestC (
	int a,
	int b, 
	int c
	)
{
	CMainFrame* pMainFrame = GetMainFrame ();
	pMainFrame->m_OutputPane.m_LogCtrl.Trace ("CallConvTestC (%d, %d, %d)\n", a, b, c);
	return 3000;
}

int
__stdcall
StdLib_CallConvTestD (
	int a,
	int b, 
	int c
	)
{
	CMainFrame* pMainFrame = GetMainFrame ();
	pMainFrame->m_OutputPane.m_LogCtrl.Trace ("CallConvTestD (%d, %d, %d)\n", a, b, c);
	return 4000;
}

void
StdLib_PointerCheck (jnc::TSafePtr Ptr)
{
	CMainFrame* pMainFrame = GetMainFrame ();
	pMainFrame->m_OutputPane.m_LogCtrl.Trace (
		"PointerCheck (%p; range = %p:%p; scope = %d)\n", 
		Ptr.m_p,
		Ptr.m_Validator.m_pRegionBegin,
		Ptr.m_Validator.m_pRegionEnd,
		Ptr.m_Validator.m_ScopeLevel
		);
}

bool
CAstDoc::ExportStdLib ()
{
	ExportStdLibFunction (jnc::EStdFunc_OnRuntimeError, StdLib_OnRuntimeError);
	ExportStdLibFunction (jnc::EStdFunc_DynamicCastInterface, StdLib_DynamicCastInterface);
	ExportStdLibFunction (jnc::EStdFunc_EventOperator, StdLib_EventOperator);
	ExportStdLibFunction (jnc::EStdFunc_FireSimpleEvent, StdLib_FireSimpleEvent);
	ExportStdLibFunction (jnc::EStdFunc_HeapAllocate, StdLib_HeapAllocate);

	ExportStdLibFunction (_T("printf"), StdLib_printf);
	ExportStdLibFunction (_T("ReadInteger"), StdLib_ReadInteger);
	ExportStdLibFunction (_T("StructTest"), StdLib_StructTest);
	ExportStdLibFunction (_T("CallConvTestA"), StdLib_CallConvTestA);
	ExportStdLibFunction (_T("CallConvTestB"), StdLib_CallConvTestB);
	ExportStdLibFunction (_T("CallConvTestC"), StdLib_CallConvTestC);
	ExportStdLibFunction (_T("CallConvTestD"), StdLib_CallConvTestD);
	ExportStdLibFunction (_T("PointerCheck"), StdLib_PointerCheck);

	return true;
}

jnc::CFunction* 
CAstDoc::FindGlobalFunction (const tchar_t* pName)
{
	jnc::CModuleItem* pItem = m_Module.m_NamespaceMgr.GetGlobalNamespace ()->FindItem (pName);
	if (!pItem)
		return NULL;

	if (pItem->GetItemKind () != jnc::EModuleItem_GlobalFunction)
		return NULL;

	jnc::CGlobalFunction* pFunction = (jnc::CGlobalFunction*) pItem;
	return pFunction->GetFunction ();
}

bool
CAstDoc::ExportStdLibFunction (
	const tchar_t* pName,
	void* pfn
	)
{
	jnc::CFunction* pFunction = FindGlobalFunction (pName);
	if (!pFunction)
		return false;

	llvm::Function* pLlvmFunction = pFunction->GetLlvmFunction ();
	if (!pLlvmFunction)
		return false;

	m_pLlvmExecutionEngine->addGlobalMapping (pLlvmFunction, pfn);
	return true;
}

bool
CAstDoc::ExportStdLibFunction (
	jnc::EStdFunc FuncKind,
	void* pfn
	)
{
	jnc::CFunction* pFunction = m_Module.m_FunctionMgr.GetStdFunction (FuncKind);
	if (!pFunction)
		return false;

	m_pLlvmExecutionEngine->addGlobalMapping (pFunction->GetLlvmFunction (), pfn);
	return true;
}

BOOL CAstDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	GetView ()->SetFont (&theApp.m_Font);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


BOOL CAstDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CFile File;
	BOOL Result = File.Open (lpszPathName, CFile::modeRead | CFile::shareDenyWrite);
	if (!Result)
		return FALSE;

	size_t Size = (size_t) File.GetLength ();
	rtl::CArrayT <char> Buffer;
	Buffer.SetCount (Size + 1);
	Buffer [Size] = 0;

	File.Read (Buffer, Size);
	GetView ()->SetWindowText ((const char*) Buffer);

	m_strPathName = lpszPathName;

	Compile ();

	SetModifiedFlag(FALSE);
	return TRUE;
}

BOOL CAstDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CFile File;
	BOOL Result = File.Open (lpszPathName, CFile::modeWrite | CFile::shareDenyWrite);
	if (!Result)
		return FALSE;

	CString String;
	GetView ()->GetWindowText (String);

	size_t Length = String.GetLength ();

	File.Write (String, Length);
	File.SetLength (Length);

	SetModifiedFlag(FALSE);
	return TRUE;
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAstDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAstDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAstDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAstDoc diagnostics

#ifdef _DEBUG
void CAstDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAstDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAstDoc commands
