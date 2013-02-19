// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"
#include "axl_jnc_StructType.h"

namespace axl {
namespace jnc {

class CFunctionPtrType;

//.............................................................................

enum EMulticastType
{
	EMulticastType_Normal = 0,
	EMulticastType_Snapshot,
	EMulticastType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetMulticastTypeKindString (EMulticastType MulticastTypeKind);

//.............................................................................

class CMulticastType: public CType
{
protected:
	friend class CTypeMgr;

	EMulticastType m_MulticastTypeKind;
	CFunctionPtrType* m_pFunctionPtrType;
	CStructType* m_pMulticastStructType;

public:
	CMulticastType ();

	EMulticastType 
	GetMulticastTypeKind ()
	{
		return m_MulticastTypeKind;
	}

	CFunctionPtrType* 
	GetFunctionPtrType ()
	{
		return m_pFunctionPtrType;
	}

	CFunctionType* 
	GetFunctionType ();

	CStructType* 
	GetMulticastStructType ();

	static
	rtl::CStringA
	CreateSignature (
		CFunctionPtrType* pFunctionPtrType,
		EMulticastType MulticastTypeKind
		);
	
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
