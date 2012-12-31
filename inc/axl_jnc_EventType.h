// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionPointerType.h"

namespace axl {
namespace jnc {

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
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
