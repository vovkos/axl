// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_CastOp.h"

namespace axl {
namespace jnc {

//.............................................................................

// arr -> arr

class CCast_Array: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_Array, ICastOperator)

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
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
