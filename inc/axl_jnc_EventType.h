// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionPointerType.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EEventOp
{
	EEventOp_SetHandler,
	EEventOp_AddHandler,
	EEventOp_RemoveHandler,
};

const tchar_t*
GetEventOperatorKindString (EEventOp OperaotrKind);

//.............................................................................

class CEventType:
	public CType,
	public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CFunctionPointerType* m_pFunctionPointerType;
	CStructType* m_pHandlerStructType;
	CStructType* m_pEventStructType;

public:
	CEventType ();

	llvm::StructType* 
	GetLlvmType ()
	{
		return GetEventStructType ()->GetLlvmType ();
	}

	CFunctionType* 
	GetFunctionType ()
	{
		return m_pFunctionPointerType->GetFunctionType ();
	}

	CFunctionPointerType* 
	GetFunctionPointerType ()
	{
		return m_pFunctionPointerType;
	}

	CStructType* 
	GetEventStructType ();

	CStructType* 
	GetHandlerStructType ();

	static
	rtl::CString
	CreateTypeString (CFunctionType* pFunctionType);

	rtl::CString
	CreateTypeString ()
	{
		return CreateTypeString (GetFunctionType ());
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
