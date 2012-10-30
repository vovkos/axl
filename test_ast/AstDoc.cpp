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
END_MESSAGE_MAP()

// CAstDoc construction/destruction

CAstDoc::CAstDoc()
{
}

CAstDoc::~CAstDoc()
{
}

void CAstDoc::OnFileCompile()
{
	DoFileSave ();
	Compile ();
}

bool
CAstDoc::Compile ()
{
	bool Result;

	CMainFrame* pMainFrame = GetMainFrame ();

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Parsing...\n"));
	pMainFrame->m_GlobalAstPane.Clear ();
	pMainFrame->m_FunctionAstPane.Clear ();
	pMainFrame->m_ModulePane.Clear ();
	pMainFrame->m_LlvmIrPane.Clear ();
	pMainFrame->m_DasmPane.Clear ();

	m_Module.Create ((const tchar_t*) GetPathName ());
	jnc::CSetCurrentThreadModule ScopeModule (&m_Module);

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
	}

	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Compiling functions...\n"));
	Result = m_Module.m_FunctionMgr.CompileFunctions ();
	if (!Result)
	{
		rtl::CString Text = err::GetError ()->GetDescription ();
		pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("%s\n"), Text);
	}
	
	pMainFrame->m_GlobalAstPane.Build (Parser.GetAst ());
	pMainFrame->m_ModulePane.Build (&m_Module);
	pMainFrame->m_LlvmIrPane.Build (&m_Module);
	pMainFrame->m_DasmPane.Build (&m_Module);
	pMainFrame->m_OutputPane.m_LogCtrl.Trace (_T("Done.\n"));
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
