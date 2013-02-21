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

class CMulticastType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionPtrType* m_pTargetType;
	CStructType* m_pMulticastStructType;

	CFunction* m_pSetMethod_s;
	CFunction* m_pSetMethod_u;
	CFunction* m_pAddMethod_s;
	CFunction* m_pAddMethod_u;
	CFunction* m_pRemoveMethod_s;
	CFunction* m_pRemoveMethod_u;
	CFunction* m_pSnapshotMethod_s;
	CFunction* m_pSnapshotMethod_u;

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
		return PtrTypeKind == EDataPtrType_Unsafe ? GetSetMethod_u () : GetSetMethod_s ();
	}

	CFunction* 
	GetAddMethod (EDataPtrType PtrTypeKind = EDataPtrType_Normal)
	{
		return PtrTypeKind == EDataPtrType_Unsafe ? GetAddMethod_u () : GetAddMethod_s ();
	}

	CFunction* 
	GetRemoveMethod (EDataPtrType PtrTypeKind = EDataPtrType_Normal)
	{
		return PtrTypeKind == EDataPtrType_Unsafe ? GetRemoveMethod_u () : GetRemoveMethod_s ();
	}

	CFunction* 
	GetSnapshotMethod (EDataPtrType PtrTypeKind = EDataPtrType_Normal)
	{
		return PtrTypeKind == EDataPtrType_Unsafe ? GetSnapshotMethod_u () : GetSnapshotMethod_s ();
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
	GetSetMethod_s ();

	CFunction* 
	GetSetMethod_u ();

	CFunction* 
	GetAddMethod_s ();

	CFunction* 
	GetAddMethod_u ();

	CFunction* 
	GetRemoveMethod_s ();

	CFunction* 
	GetRemoveMethod_u ();

	CFunction* 
	GetSnapshotMethod_s ();

	CFunction* 
	GetSnapshotMethod_u ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
