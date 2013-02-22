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

	CFunction* m_pSetMethod;
	CFunction* m_pSetMethod_u;
	CFunction* m_pAddMethod;
	CFunction* m_pAddMethod_u;
	CFunction* m_pRemoveMethod;
	CFunction* m_pRemoveMethod_u;
	CFunction* m_pSnapshotMethod;
	CFunction* m_pSnapshotMethod_u;
	CFunction* m_pCallMethod;
	CFunction* m_pCallMethod_u;

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
	GetSetMethod (EDataPtrType PtrTypeKind = EDataPtrType_Normal)
	{
		return PtrTypeKind == EDataPtrType_Unsafe ? GetSetMethodImpl_u () : GetSetMethodImpl ();
	}

	CFunction* 
	GetAddMethod (EDataPtrType PtrTypeKind = EDataPtrType_Normal)
	{
		return PtrTypeKind == EDataPtrType_Unsafe ? GetAddMethodImpl_u () : GetAddMethodImpl ();
	}

	CFunction* 
	GetRemoveMethod (EDataPtrType PtrTypeKind = EDataPtrType_Normal)
	{
		return PtrTypeKind == EDataPtrType_Unsafe ? GetRemoveMethodImpl_u () : GetRemoveMethodImpl ();
	}

	CFunction* 
	GetSnapshotMethod (EDataPtrType PtrTypeKind = EDataPtrType_Normal)
	{
		return PtrTypeKind == EDataPtrType_Unsafe ? GetSnapshotMethodImpl_u () : GetSnapshotMethodImpl ();
	}

	CFunction* 
	GetCallMethod (EDataPtrType PtrTypeKind = EDataPtrType_Normal)
	{
		return PtrTypeKind == EDataPtrType_Unsafe ? GetSnapshotMethodImpl_u () : GetSnapshotMethodImpl ();
//		return PtrTypeKind == EDataPtrType_Unsafe ? GetCallMethodImpl_u () : GetCallMethodImpl ();
	}

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
	GetSetMethodImpl ();

	CFunction* 
	GetSetMethodImpl_u ();

	CFunction* 
	GetAddMethodImpl ();

	CFunction* 
	GetAddMethodImpl_u ();

	CFunction* 
	GetRemoveMethodImpl ();

	CFunction* 
	GetRemoveMethodImpl_u ();

	CFunction* 
	GetSnapshotMethodImpl ();

	CFunction* 
	GetSnapshotMethodImpl_u ();

	void
	ConvertToSimpleMulticastPtr (CValue* pMulticastPtrValue);

	void
	ConvertToSimpleFunctionPtr (CValue* pFunctionPtrValue);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
