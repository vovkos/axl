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

// AstDoc.h : interface of the CAstDoc class
//


#pragma once

#include "..\axl_jnc\axl_jnc_Parser.h"
#include "axl_jnc_Module.h"

class CAstDoc : public CDocument
{
protected:
	CString m_SourceText;
	jnc::CModule m_Module;
	jnc::CParser m_Parser;

protected: // create from serialization only
	CAstDoc();
	DECLARE_DYNCREATE(CAstDoc)

// Attributes
public:
	CEditView* GetView ()
	{
		POSITION Pos = GetFirstViewPosition ();
		return (CEditView*) GetNextView (Pos); 
	}

// Operations
public:
	bool
	Parse ();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CAstDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnFileParse();

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
