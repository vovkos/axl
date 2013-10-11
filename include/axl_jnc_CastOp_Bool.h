// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_CastOp.h"

namespace axl {
namespace jnc {

//.............................................................................

// comparison to zero -> bool (common for both integer & fp)

class CCast_BoolFromZeroCmp: public ICastOperator
{
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

// extract 1st element, convert to int, compare it to zero

class CCast_BoolFromPtr: public CCast_BoolFromZeroCmp
{
public:
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

// bool <-> int

class CCast_IntFromBool: public ICastOperator
{
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

// bool master cast

class CCast_Bool: public CCast_Master
{
protected:
	CCast_BoolFromZeroCmp m_FromZeroCmp;
	CCast_BoolFromPtr m_FromPtr;

public:
	CCast_Bool ()
	{
		m_OpFlags = EOpFlag_KeepBool;
	}

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
