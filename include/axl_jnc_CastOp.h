// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// these are sometimes needed for inlining casts before axl_jnc_Module.h is include

bool
CastOperator (
	CModule* pModule,
	const CValue& OpValue,
	CType* pType,
	CValue* pOpValue
	);

inline
bool
CastOperator (
	CModule* pModule,
	CValue* pOpValue,
	CType* pType
	)
{
	return CastOperator (pModule, *pOpValue, pType, pOpValue);
}

//.............................................................................

struct ICastOperator: obj::IRoot
{	
	// {3B565CAF-8BA0-408F-9108-D28ECDEF518D}
	AXL_OBJ_INTERFACE (
		ICastOperator,
		0x3b565caf, 0x8ba0, 0x408f, 0x91, 0x08, 0xd2, 0x8e, 0xcd, 0xef, 0x51, 0x8d
		)

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
		EStorage StorageKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		) = 0;

	bool
	Cast (
		EStorage StorageKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// fail by default

class CCast_Default: public ICastOperator
{
public:
	AXL_OBJ_CLASS_0 (CCast_Default, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		)
	{
		return ECast_None;
	}

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

// simple copy

class CCast_Copy: public ICastOperator
{
public:
	AXL_OBJ_CLASS_0 (CCast_Copy, ICastOperator)

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
		EStorage StorageKind,
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
		EStorage StorageKind,
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
		EStorage StorageKind,
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

