// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CFunctionPointerType:
	public CType,
	public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CFunctionType* m_pFunctionType;
	CFunctionType* m_pShortFunctionType;
	CStructType* m_pPointerStructType;
	CEventType* m_pEventType;

public:
	CFunctionPointerType ();

	llvm::StructType* 
	GetLlvmType ()
	{
		return GetPointerStructType ()->GetLlvmType ();
	}

	CFunctionType* 
	GetFunctionType ()
	{
		return m_pFunctionType;
	}

	CFunctionType* 
	GetShortFunctionType ()
	{
		return m_pShortFunctionType;
	}

	CStructType* 
	GetPointerStructType ();

	CEventType* 
	GetEventType ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
