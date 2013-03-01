// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_AttributeMgr.h"
#include "axl_jnc_TypeMgr.h"
#include "axl_jnc_NamespaceMgr.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_VariableMgr.h"
#include "axl_jnc_ConstMgr.h"
#include "axl_jnc_ControlFlowMgr.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_LlvmBuilder.h"

namespace axl {
namespace jnc {

//.............................................................................

class CModule;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY
DWORD g_ModuleTls = TlsAlloc ();

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
CModule*
GetCurrentThreadModule ()
{
	return (CModule*) TlsGetValue (g_ModuleTls);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
CModule*
SetCurrentThreadModule (CModule* pModule)
{
	CModule* pPrevModule = GetCurrentThreadModule ();
	TlsSetValue (g_ModuleTls, pModule);
	return pPrevModule;
}

//.............................................................................

class CSetCurrentThreadModule
{
protected:
	CModule* m_pPrevModule;

public:
	CSetCurrentThreadModule (CModule* pModule)
	{
		m_pPrevModule = SetCurrentThreadModule (pModule);
	}

	~CSetCurrentThreadModule ()
	{
		SetCurrentThreadModule (m_pPrevModule);
	}
};

//.............................................................................

class CPreModule
{
protected:
	CModule* m_pPrevModule;

protected:
	CPreModule ()
	{
		m_pPrevModule = SetCurrentThreadModule ((CModule*) this);
	}

	void
	RestorePrevModule ()
	{
		SetCurrentThreadModule (m_pPrevModule);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CModule: public CPreModule
{
protected:
	rtl::CString m_FilePath;

public:
	llvm::Module* m_pLlvmModule;

	CTypeMgr m_TypeMgr;
	CAttributeMgr m_AttributeMgr;
	CNamespaceMgr m_NamespaceMgr;
	CFunctionMgr m_FunctionMgr;
	CVariableMgr m_VariableMgr;
	CConstMgr m_ConstMgr;
	CControlFlowMgr m_ControlFlowMgr;
	COperatorMgr m_OperatorMgr;	
	CLlvmBuilder m_LlvmBuilder;

public:
	CModule ()
	{
		m_pLlvmModule = NULL;
	}

	~CModule ()
	{
		Clear ();
	}

	rtl::CString
	GetFilePath ()
	{
		return m_FilePath;
	}

	void
	Clear ();

	bool
	Create (const rtl::CString& FilePath);
};

//.............................................................................

inline
void 
LlvmFatalErrorHandler (
	void* pContext,
	const std::string& ErrorString
	)
{
	throw err::CError (ErrorString.c_str ());
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
