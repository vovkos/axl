// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_CastOp.h"
#include "axl_jnc_FunctionPtrType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CCast_FunctionPtr_Base: public ICastOperator
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

class CCast_FunctionPtr_FromNormal: public CCast_FunctionPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_FunctionPtr_FromNormal, ICastOperator)

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

class CCast_FunctionPtr_Thin2Normal: public CCast_FunctionPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_FunctionPtr_Thin2Normal, ICastOperator)

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
		CFunctionType* pSrcFunctionType,
		CFunctionPtrType* pDstPtrType,
		CValue* pResultValue
		);

	bool
	LlvmCast_DirectThunkNoClosure (
		CFunction* pFunction,
		CFunctionPtrType* pDstPtrType,
		CValue* pResultValue
		);

	bool
	LlvmCast_DirectThunkSimpleClosure (
		CFunction* pFunction,
		const CValue& SimpleClosureObjValue,
		CFunctionPtrType* pDstPtrType,
		CValue* pResultValue
		);

	bool
	LlvmCast_FullClosure (
		EAlloc AllocKind,
		const CValue& OpValue,
		CFunctionType* pSrcFunctionType,
		CFunctionPtrType* pDstPtrType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_FunctionPtr_Weak2Normal: public CCast_FunctionPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_FunctionPtr_Thin2Normal, ICastOperator)

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

class CCast_FunctionPtr_Thin2Thin: public CCast_FunctionPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_FunctionPtr_Thin2Thin, ICastOperator)

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

class CCast_FunctionPtr_Thin2Weak: public CCast_FunctionPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_FunctionPtr_Thin2Weak, ICastOperator)

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

class CCast_FunctionPtr_Unsafe2Unsafe: public CCast_FunctionPtr_Base
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_FunctionPtr_Unsafe2Unsafe, ICastOperator)

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

class CCast_FunctionPtr: public CCast_Master
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_FunctionPtr, ICastOperator)

protected:
	CCast_FunctionPtr_FromNormal m_FromNormal;
	CCast_FunctionPtr_Weak2Normal m_Weak2Normal;
	CCast_FunctionPtr_Thin2Normal m_Thin2Normal;
	CCast_FunctionPtr_Thin2Weak m_Thin2Weak;
	CCast_FunctionPtr_Thin2Thin m_Thin2Thin;
	CCast_FunctionPtr_Unsafe2Unsafe m_Unsafe2Unsafe;

	ICastOperator* m_OperatorTable [EFunctionPtrType__Count] [EFunctionPtrType__Count];

public:
	CCast_FunctionPtr ();

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
