// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_CastOp.h"
#include "axl_jnc_PropertyPtrType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CCast_PropertyPtr_Base: public ICastOperator
{
public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_PropertyPtr_FromNormal: public CCast_PropertyPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_PropertyPtr_FromNormal, ICastOperator)

public:
	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_PropertyPtr_Thin2Normal: public CCast_PropertyPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_PropertyPtr_Thin2Normal, ICastOperator)

public:
	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);

protected:
	bool
	LlvmCast_NoThunkSimpleClosure (
		const CValue& OpValue,
		const CValue& SimpleClosureObjValue,
		CPropertyType* pSrcPropertyType,
		CPropertyPtrType* pDstPtrType,
		CValue* pResultValue
		);

	bool
	LlvmCast_DirectThunkNoClosure (
		CProperty* pProperty,
		CPropertyPtrType* pDstPtrType,
		CValue* pResultValue
		);

	bool
	LlvmCast_DirectThunkSimpleClosure (
		CProperty* pProperty,
		const CValue& SimpleClosureObjValue,
		CPropertyPtrType* pDstPtrType,
		CValue* pResultValue
		);

	bool
	LlvmCast_FullClosure (
		EAlloc AllocKind,
		const CValue& OpValue,
		CPropertyType* pSrcPropertyType,
		CPropertyPtrType* pDstPtrType,
		CValue* pResultValue
		);

	bool
	CreateClosurePropertyPtr (
		CProperty* pProperty,
		const CValue& ClosureValue,
		CPropertyPtrType* pPtrType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_PropertyPtr_Weak2Normal: public CCast_PropertyPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_PropertyPtr_Thin2Normal, ICastOperator)

public:
	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_PropertyPtr_Thin2Thin: public CCast_PropertyPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_PropertyPtr_Thin2Thin, ICastOperator)

public:
	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_PropertyPtr_Thin2Weak: public CCast_PropertyPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_PropertyPtr_Thin2Weak, ICastOperator)

public:
	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_PropertyPtr_Unsafe2Unsafe: public CCast_PropertyPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_PropertyPtr_Unsafe2Unsafe, ICastOperator)

public:
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

// safe / unsafe fn pointer -> safe fn pointer

class CCast_PropertyPtr: public CCast_Master
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_PropertyPtr, ICastOperator)

protected:
	CCast_PropertyPtr_FromNormal m_FromNormal;
	CCast_PropertyPtr_Weak2Normal m_Weak2Normal;
	CCast_PropertyPtr_Thin2Normal m_Thin2Normal;
	CCast_PropertyPtr_Thin2Weak m_Thin2Weak;
	CCast_PropertyPtr_Thin2Thin m_Thin2Thin;
	CCast_PropertyPtr_Unsafe2Unsafe m_Unsafe2Unsafe;

	ICastOperator* m_OperatorTable [EPropertyPtrType__Count] [EPropertyPtrType__Count];

public:
	CCast_PropertyPtr ();

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
