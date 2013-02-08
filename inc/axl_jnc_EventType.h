// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionPtrType.h"

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

class CEventType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionPtrType* m_pFunctionPtrType;
	CStructType* m_pHandlerStructType;
	CStructType* m_pEventStructType;

public:
	CEventType ();

	CFunctionPtrType* 
	GetFunctionPtrType ()
	{
		return m_pFunctionPtrType;
	}

	CFunctionType* 
	GetFunctionType ()
	{
		return m_pFunctionPtrType->GetTargetType ();
	}

	CStructType* 
	GetEventStructType ();

	CStructType* 
	GetHandlerStructType ();

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
