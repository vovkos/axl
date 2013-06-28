// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_CastOp.h"
#include "axl_jnc_DataPtrType.h"

namespace axl {
namespace jnc {

class CBaseTypeCoord;

//.............................................................................

// array -> ptr

class CCast_DataPtr_FromArray: public ICastOperator
{
public:
	AXL_OBJ_CLASS_0 (CCast_DataPtr_FromArray, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		);

	virtual
	bool
	ConstCast (
		const CValue& OpValue,
		CType* pType,
		void* pDst
		);

	virtual
	bool
	LlvmCast (
		EStorage StorageKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		SetCastError (OpValue, pType);
		return false;
	}
};

//.............................................................................

// data ptr -> data ptr

class CCast_DataPtr_Base: public ICastOperator
{
public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		);

protected:
	intptr_t
	GetOffset (
		CDataPtrType* pSrcType,
		CDataPtrType* pDstType,
		CBaseTypeCoord* pCoord
		);

	intptr_t
	GetOffsetUnsafePtrValue (
		const CValue& PtrValue, 
		CDataPtrType* pSrcType,
		CDataPtrType* pDstType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_DataPtr_Normal2Normal: public CCast_DataPtr_Base
{
public:
	AXL_OBJ_CLASS_0 (CCast_DataPtr_Normal2Normal, ICastOperator)

public:
	virtual
	bool
	ConstCast (
		const CValue& OpValue,
		CType* pType,
		void* pDst
		);

	virtual
	bool
	LlvmCast (
		EStorage StorageKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_DataPtr_Thin2Normal: public CCast_DataPtr_Base
{
public:
	AXL_OBJ_CLASS_0 (CCast_DataPtr_Thin2Normal, ICastOperator)

public:
	virtual
	bool
	ConstCast (
		const CValue& OpValue,
		CType* pType,
		void* pDst
		);

	virtual
	bool
	LlvmCast (
		EStorage StorageKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_DataPtr_Normal2Thin: public CCast_DataPtr_Base
{
public:
	AXL_OBJ_CLASS_0 (CCast_DataPtr_Normal2Thin, ICastOperator)

public:
	virtual
	bool
	ConstCast (
		const CValue& OpValue,
		CType* pType,
		void* pDst
		);

	virtual
	bool
	LlvmCast (
		EStorage StorageKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_DataPtr_Thin2Thin: public CCast_DataPtr_Base
{
public:
	AXL_OBJ_CLASS_0 (CCast_DataPtr_Thin2Thin, ICastOperator)

public:
	virtual
	bool
	ConstCast (
		const CValue& OpValue,
		CType* pType,
		void* pDst
		);

	virtual
	bool
	LlvmCast (
		EStorage StorageKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// data ptr master cast

class CCast_DataPtr: public CCast_Master
{
public:
	AXL_OBJ_CLASS_0 (CCast_DataPtr, ICastOperator)

protected:
	CCast_DataPtr_FromArray m_FromArray;
	CCast_DataPtr_Normal2Normal m_Normal2Normal;
	CCast_DataPtr_Thin2Normal m_Thin2Normal;
	CCast_DataPtr_Normal2Thin m_Normal2Thin;
	CCast_DataPtr_Thin2Thin m_Thin2Thin;

	ICastOperator* m_OperatorTable [EDataPtrType__Count] [EDataPtrType__Count];

public:
	CCast_DataPtr ();

	virtual
	ICastOperator*
	GetCastOperator (
		const CValue& OpValue,
		CType* pType
		);
};

//.............................................................................

// data ref (EUnOp_Indir => data ptr cast => EUnOp_Addr)

class CCast_DataRef: public ICastOperator
{
public:
	AXL_OBJ_CLASS_0 (CCast_DataRef, ICastOperator)

public:
	CCast_DataRef ()
	{
		m_OpFlags = EOpFlag_KeepRef;
	}

	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		);

	virtual
	bool
	LlvmCast (
		EStorage StorageKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
