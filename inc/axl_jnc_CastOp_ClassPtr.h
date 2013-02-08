// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_CastOp.h"

namespace axl {
namespace jnc {

//.............................................................................

class CCast_ClassPtr: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_ClassPtr, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
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

} // namespace jnc {
} // namespace axl {
