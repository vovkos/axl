// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionPtrType.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EMcSnapshotField
{
	EMcSnapshotField_Count,
	EMcSnapshotField_PtrArray,

	EMcSnapshotField__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EMcSnapshotMethod
{
	EMcSnapshotMethod_Call,

	EMcSnapshotMethod__Count,
};
//.............................................................................

class CMcSnapshotType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionPtrType* m_pTargetType;
	CStructType* m_pMcSnapshotStructType;
	CStructField* m_FieldArray [EMcSnapshotField__Count];
	CFunction* m_MethodArray [EMcSnapshotMethod__Count];

public:
	CMcSnapshotType ();

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
	GetMcSnapshotStructType ();

	CStructField* 
	GetField (EMcSnapshotField Field)
	{
		ASSERT (Field < EMcSnapshotField__Count);
		return m_FieldArray [Field];
	}

	CFunction* 
	GetMethod (EMcSnapshotMethod Method);

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ();

	CFunction* 
	CreateCallMethod ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
