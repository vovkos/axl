// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnOp.h"

namespace axl {
namespace jnc {

class CFunctionType;
class CFunctionPtrType;

//.............................................................................

// ordered from the worst to the best

enum ECast
{
	ECast_None,
	ECast_Explicit,
	ECast_ImplicitCrossFamily,
	ECast_Implicit,
	ECast_Identitiy,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

err::CError
SetCastError (
	const CValue& OpValue,
	CType* pType
	);

//.............................................................................

[uuid ("2f7742e5-396e-4444-bbc6-56eb475403bf")]
struct ICastOperator: obj::IRoot
{	
protected:
	friend class COperatorMgr;

	CModule* m_pModule;
	int m_OpFlags;

public:
	ICastOperator ();

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	int 
	GetOpFlags ()
	{
		return m_OpFlags;
	}

	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		) = 0;

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
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		) = 0;

	bool
	Cast (
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// simple copy

class CCast_Copy: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_Copy, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return OpValue.GetType ()->Cmp (pType) == 0 ? ECast_Identitiy : ECast_Implicit;
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
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// master cast chooses particular implementation

class CCast_Master: public ICastOperator
{
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
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);

	virtual
	ICastOperator*
	GetCastOperator (
		const CValue& OpValue,
		CType* pType
		) = 0;
};

//.............................................................................

// master cast capable of performing superposition of casts

class CCast_SuperMaster: public ICastOperator
{
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
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);

	virtual
	bool
	GetCastOperators (
		const CValue& OpValue,
		CType* pType,
		ICastOperator** ppOperator1,
		ICastOperator** ppOperator2,
		CType** ppIntermediateType
		) = 0;
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

