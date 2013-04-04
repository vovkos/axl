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
#include "Multicast.h"

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

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Compiling...\n"));
	Result = m_Module.Compile ();

	// show module contents nevetheless

	pMainFrame->m_GlobalAstPane.Build (Parser.GetAst ());
	pMainFrame->m_ModulePane.Build (&m_Module);
	pMainFrame->m_LlvmIrPane.Build (&m_Module);

	if (!Result)
	{
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("%s\n"), err::GetError ()->GetDescription ());
		return false;
	}

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("JITting...\n"));

	ExportStdLib ();

	Result = m_Module.m_FunctionMgr.JitFunctions (m_pLlvmExecutionEngine);
	if (!Result)
	{
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("%s\n"), err::GetError ()->GetDescription ());
		return false;
	}

	pMainFrame->m_DasmPane.Build (&m_Module);
	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Done.\n"));
	return true;
}

bool
CAstDoc::RunFunction (
	jnc::CFunction* pFunction,
	int* pReturnValue
	)
{
	typedef int (*FFunction) ();
	FFunction pfn = (FFunction) pFunction->GetFunctionPointer ();
	ASSERT (pfn);

	bool Result = true;

	try
	{
		int ReturnValue = pfn ();
		if (pReturnValue)
			*pReturnValue = ReturnValue;
	}
	catch (err::CError Error)
	{
		GetMainFrame ()->m_OutputPane.m_LogCtrl.Trace (_T("ERROR: %s\n"), Error.GetDescription ());
		Result = false;
	}
	catch (...)
	{
		GetMainFrame ()->m_OutputPane.m_LogCtrl.Trace (_T("UNKNOWN EXCEPTION\n"));
		Result = false;
	}

	return Result;
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

	jnc::CFunction* pMainFunction = FindGlobalFunction (_T("main"));
	if (!pMainFunction)
	{
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("'main' is not found or not a function\n"));
		return false;
	}

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Running...\n"));

	jnc::CFunction* pConstructor = m_Module.GetConstructor ();
	if (pConstructor)
	{
		Result = RunFunction (pConstructor);
		if (!Result)
			return false;
	}

	Result = RunFunction (pMainFunction);
	if (!Result)
		return false;

	jnc::CFunction* pDestructor = m_Module.GetDestructor ();
	if (pDestructor)
	{
		Result = RunFunction (pDestructor);
		if (!Result)
			return false;
	}
		
	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Done (retval = %d).\n"), Result);
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

	case jnc::ERuntimeError_AbstractFunction:
		pErrorString = "ABSTRACT_FUNCTION";
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

jnc::TInterface* 
StdLib_DynamicCastClassPtr (
	jnc::TInterface* p,
	jnc::CClassType* pType
	)
{
	if (!p)
		return NULL;

	if (p->m_pObject->m_pType->Cmp (pType) == 0)
		return p;
	
	jnc::CBaseTypeCoord Coord;
	bool Result = p->m_pObject->m_pType->FindBaseTypeTraverse (pType, &Coord);
	if (!Result)
		return NULL;
	
	jnc::TInterface* p2 = (jnc::TInterface*) ((uchar_t*) (p->m_pObject + 1) + Coord.m_Offset);
	ASSERT (p2->m_pObject == p->m_pObject);
	return p2;
}

handle_t
StdLib_MulticastSet (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr Ptr
	)
{
	return ((CMulticast*) pMulticast)->SetHandler (Ptr);
}

handle_t
StdLib_MulticastSet_w (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr_w Ptr
	)
{
	return ((CMulticast*) pMulticast)->SetHandler_w (Ptr);
}

handle_t
StdLib_MulticastSet_u (
	jnc::TMulticast* pMulticast,
	void* pfn
	)
{
	return ((CMulticast*) pMulticast)->SetHandler_u (pfn);
}

handle_t
StdLib_MulticastAdd (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr Ptr
	)
{
	return ((CMulticast*) pMulticast)->AddHandler (Ptr);
}

handle_t
StdLib_MulticastAdd_w (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr_w Ptr
	)
{
	return ((CMulticast*) pMulticast)->AddHandler_w (Ptr);
}

handle_t
StdLib_MulticastAdd_u (
	jnc::TMulticast* pMulticast,
	void* pfn
	)
{
	return ((CMulticast*) pMulticast)->AddHandler_u (pfn);
}

jnc::TFunctionPtr
StdLib_MulticastRemove (
	jnc::TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler (Handle);
}

jnc::TFunctionPtr_w
StdLib_MulticastRemove_w (
	jnc::TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler_w (Handle);
}

void*
StdLib_MulticastRemove_u (
	jnc::TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler_u (Handle);
}

jnc::TMcSnapshot
StdLib_MulticastSnapshot (jnc::TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->Snapshot ();
}

jnc::TMcSnapshot
StdLib_MulticastSnapshot_w (jnc::TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->Snapshot_w ();
}

jnc::TMcSnapshot
StdLib_MulticastSnapshot_u (jnc::TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->Snapshot_u ();
}

void*
StdLib_HeapAlloc (jnc::CType* pType)
{
	void* p = malloc (pType->GetSize ());
	memset (p, 0, pType->GetSize ());

	if (pType->GetTypeKind () == jnc::EType_Struct && ((jnc::CStructType*) pType)->IsClassStructType ())
	{
		jnc::CClassType* pClassType = (jnc::CClassType*) ((jnc::CStructType*) pType)->GetParentNamespace ();
		jnc::CFunction* pDestructor = pClassType->GetDestructor ();
		
		if (pDestructor)
		{
			// add to finalizer list
		}
	}

	return p;
}

void*
StdLib_UHeapAlloc (jnc::CType* pType)
{
	void* p = malloc (pType->GetSize ());
	memset (p, 0, pType->GetSize ());
	return p;
}

void
StdLib_UHeapFree (void* p)
{
	free (p);
}

void
StdLib_UHeapFreeClassPtr (jnc::TInterface* p)
{
	free (p->m_pObject);
}

int
StdLib_printf (
	const char* pFormat,
	...
	)
{
	rtl::CString Text;
	Text.FormatV (pFormat, va_start_e (pFormat));

	CMainFrame* pMainFrame = GetMainFrame ();
	pMainFrame->m_OutputPane.m_LogCtrl.Trace0 (Text);

	return Text.GetLength ();
}

ulong_t
StdLib_GetCurrentThreadId ()
{
	return GetCurrentThreadId ();
}

DWORD 
WINAPI
StdLib_ThreadProc (PVOID pContext)
{
	jnc::TFunctionPtr* pPtr = (jnc::TFunctionPtr*) pContext;
	jnc::TFunctionPtr Ptr = *pPtr;
	AXL_MEM_DELETE (pPtr);

	((void (__cdecl*) (jnc::TInterface*)) Ptr.m_pfn) (Ptr.m_pClosure);
	return 0;
}

int
StdLib_CreateThread (jnc::TFunctionPtr Ptr)
{
	jnc::TFunctionPtr* pPtr = AXL_MEM_NEW (jnc::TFunctionPtr);
	*pPtr = Ptr;

	DWORD ThreadId;
	HANDLE h = CreateThread (NULL, 0, StdLib_ThreadProc, pPtr, 0, &ThreadId);
	return h != NULL;
}

void
StdLib_PointerCheck (jnc::TDataPtr Ptr)
{
	CMainFrame* pMainFrame = GetMainFrame ();
	pMainFrame->m_OutputPane.m_LogCtrl.Trace (
		"PointerCheck (%p; range = %p:%p; scope = %d)\n", 
		Ptr.m_p,
		Ptr.m_pRangeBegin,
		Ptr.m_pRangeEnd,
		Ptr.m_ScopeLevel
		);
}

void
StdLib_IfaceTest (jnc::TInterface* p)
{
	_asm int 3;
}

bool
CAstDoc::ExportStdLib ()
{
	ExportStdLibFunction (jnc::EStdFunc_OnRuntimeError, StdLib_OnRuntimeError);
	ExportStdLibFunction (jnc::EStdFunc_DynamicCastClassPtr, StdLib_DynamicCastClassPtr);
	ExportStdLibFunction (jnc::EStdFunc_HeapAlloc, StdLib_HeapAlloc);
	ExportStdLibFunction (jnc::EStdFunc_UHeapAlloc, StdLib_UHeapAlloc);
	ExportStdLibFunction (jnc::EStdFunc_UHeapFree, StdLib_UHeapFree);
	ExportStdLibFunction (jnc::EStdFunc_UHeapFreeClassPtr, StdLib_UHeapFreeClassPtr);

	// implementation for thin and unsafe is the same

	ExportStdLibFunction (jnc::EStdFunc_MulticastSet,   StdLib_MulticastSet);
	ExportStdLibFunction (jnc::EStdFunc_MulticastSet_w, StdLib_MulticastSet_w);
	ExportStdLibFunction (jnc::EStdFunc_MulticastSet_t, StdLib_MulticastSet_u);
	ExportStdLibFunction (jnc::EStdFunc_MulticastSet_u, StdLib_MulticastSet_u);

	ExportStdLibFunction (jnc::EStdFunc_MulticastAdd,   StdLib_MulticastAdd);
	ExportStdLibFunction (jnc::EStdFunc_MulticastAdd_w, StdLib_MulticastAdd_w);
	ExportStdLibFunction (jnc::EStdFunc_MulticastAdd_t, StdLib_MulticastAdd_u);
	ExportStdLibFunction (jnc::EStdFunc_MulticastAdd_u, StdLib_MulticastAdd_u);

	ExportStdLibFunction (jnc::EStdFunc_MulticastRemove,   StdLib_MulticastRemove);
	ExportStdLibFunction (jnc::EStdFunc_MulticastRemove_w, StdLib_MulticastRemove_w);
	ExportStdLibFunction (jnc::EStdFunc_MulticastRemove_t, StdLib_MulticastRemove_u);
	ExportStdLibFunction (jnc::EStdFunc_MulticastRemove_u, StdLib_MulticastRemove_u);

	ExportStdLibFunction (jnc::EStdFunc_MulticastSnapshot,   StdLib_MulticastSnapshot);
	ExportStdLibFunction (jnc::EStdFunc_MulticastSnapshot_w, StdLib_MulticastSnapshot_w);
	ExportStdLibFunction (jnc::EStdFunc_MulticastSnapshot_t, StdLib_MulticastSnapshot_u);
	ExportStdLibFunction (jnc::EStdFunc_MulticastSnapshot_u, StdLib_MulticastSnapshot_u);
	
	ExportStdLibFunction (_T("printf"), StdLib_printf);
	ExportStdLibFunction (_T("GetCurrentThreadId"), StdLib_GetCurrentThreadId);
	ExportStdLibFunction (_T("CreateThread"), StdLib_CreateThread);
	ExportStdLibFunction (_T("IfaceTest"), StdLib_IfaceTest);
	
	return true;
}

jnc::CFunction* 
CAstDoc::FindGlobalFunction (const tchar_t* pName)
{
	jnc::CModuleItem* pItem = m_Module.m_NamespaceMgr.GetGlobalNamespace ()->FindItem (pName);
	if (!pItem)
		return NULL;

	if (pItem->GetItemKind () != jnc::EModuleItem_Function)
		return NULL;

	return (jnc::CFunction*) pItem;
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
