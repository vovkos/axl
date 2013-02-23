// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionPtrType.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EMulticastMethod
{
	EMulticastMethod_Clear,
	EMulticastMethod_Set,
	EMulticastMethod_Add,
	EMulticastMethod_Remove,
	EMulticastMethod_Snapshot,
	EMulticastMethod_Call,
	EMulticastMethod__Count,
};

//.............................................................................

class CMulticastType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionPtrType* m_pTargetType;
	CStructType* m_pMulticastStructType;
	CFunction* m_MethodArray [EMulticastMethod__Count] [2];

public:
	CMulticastType ();

	CFunctionPtrType* 
	GetTargetType ()
	{
		return m_pTargetType;
	}

	CFunctionType* 
	GetFunctionType ()
	{
		return m_pTargetType->GetTargetType ();
	}

	CStructType* 
	GetMulticastStructType ();

	CFunction* 
	GetMethod (
		EMulticastMethod Method,
		EDataPtrType PtrTypeKind = EDataPtrType_Normal
		);

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ()
	{
		m_pLlvmType = GetMulticastStructType ()->GetLlvmType ();
	}

	CFunction* 
	CreateClearMethod ();

	CFunction* 
	CreateClearMethod_u ();

	CFunction* 
	CreateSetMethod ();

	CFunction* 
	CreateSetMethod_u ();

	CFunction* 
	CreateAddMethod ();

	CFunction* 
	CreateAddMethod_u ();

	CFunction* 
	CreateRemoveMethod ();

	CFunction* 
	CreateRemoveMethod_u ();

	CFunction* 
	CreateSnapshotMethod ();

	CFunction* 
	CreateSnapshotMethod_u ();
	
	CFunction* 
	CreateCallMethod ();

	CFunction* 
	CreateCallMethod_u ();

	void
	ConvertToSimpleMulticastPtr (CValue* pMulticastPtrValue);

	void
	ConvertToSimpleFunctionPtr (CValue* pFunctionPtrValue);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
