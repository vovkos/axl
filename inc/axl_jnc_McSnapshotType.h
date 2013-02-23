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

class CMcSnapshotType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionPtrType* m_pTargetType;
	CStructType* m_pMcSnapshotStructType;
	CFunction* m_pCallMethod;

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

	CFunction* 
	GetCallMethod ();

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
