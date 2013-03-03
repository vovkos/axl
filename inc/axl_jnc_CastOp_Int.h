// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_CastOp.h"
#include "axl_rtl_SwapByteOrder.h"

namespace axl {
namespace jnc {

//.............................................................................

// integer truncation

class CCast_IntTrunc: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_IntTrunc, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return ECast_Explicit;
	}

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

// integer extensions

class CCast_IntExt: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_IntExt, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return ECast_Implicit;
	}

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

class CCast_IntExt_u: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_IntExt_u, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return ECast_Implicit;
	}

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

// big endian <-> little endian

class CCast_SwapByteOrder: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_SwapByteOrder, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return ECast_Implicit;
	}

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

class CCast_IntFromBeInt: public CCast_SuperMaster
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_IntFromBeInt, ICastOperator)

public:
	virtual
	bool
	GetCastOperators (
		const CValue& OpValue,
		CType* pType,
		ICastOperator** ppFirstOperator,
		ICastOperator** ppSecondOperator,
		CType** ppIntermediateType
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_BeInt: public CCast_SuperMaster
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_BeInt, ICastOperator)

public:
	virtual
	bool
	GetCastOperators (
		const CValue& OpValue,
		CType* pType,
		ICastOperator** ppFirstOperator,
		ICastOperator** ppSecondOperator,
		CType** ppIntermediateType
		);
};

//.............................................................................

// floating point -> integer

class CCast_IntFromFp: public ICastOperator
{
public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return ECast_Explicit;
	}

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

class CCast_IntFromFp32: public CCast_IntFromFp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_IntFromFp32, ICastOperator)

public:
	virtual
	bool
	ConstCast (
		const CValue& OpValue,
		CType* pType,
		void* pDst
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_IntFromFp64: public CCast_IntFromFp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_IntFromFp64, ICastOperator)

public:
	virtual
	bool
	ConstCast (
		const CValue& OpValue,
		CType* pType,
		void* pDst
		);
};

//.............................................................................

// enum <-> integer

class CCast_IntFromEnum: public CCast_SuperMaster
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_IntFromEnum, ICastOperator)

public:
	virtual
	bool
	GetCastOperators (
		const CValue& OpValue,
		CType* pType,
		ICastOperator** ppFirstOperator,
		ICastOperator** ppSecondOperator,
		CType** ppIntermediateType
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_Enum: public CCast_SuperMaster
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_Enum, ICastOperator)

public:
	CCast_Enum ()
	{
		m_OpFlags = EOpFlag_KeepEnum;
	}

	virtual
	bool
	GetCastOperators (
		const CValue& OpValue,
		CType* pType,
		ICastOperator** ppFirstOperator,
		ICastOperator** ppSecondOperator,
		CType** ppIntermediateType
		);
};

//.............................................................................

// pointer <-> integer

class CCast_IntFromPtr: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_IntFromPtr, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return OpValue.GetType ()->GetSize () == sizeof (intptr_t) ? ECast_Explicit : ECast_None;
	}

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

class CCast_PtrFromInt: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_PtrFromInt, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return ECast_Explicit;
	}

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

// integer master cast

class CCast_Int: public CCast_Master
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_Int, ICastOperator)

protected:
	CCast_IntTrunc m_Trunc;
	CCast_IntExt m_Ext;
	CCast_IntExt_u m_Ext_u;
	CCast_IntFromBeInt m_FromBeInt;
	CCast_IntFromFp32 m_FromFp32;
	CCast_IntFromFp64 m_FromFp64;
	CCast_IntFromEnum m_FromEnum;
	CCast_IntFromPtr m_FromPtr;

public:
	virtual
	ICastOperator*
	GetCastOperator (
		const CValue& OpValue,
		CType* pType
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
