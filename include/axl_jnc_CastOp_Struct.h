// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_CastOp.h"

namespace axl {
namespace jnc {

class CBaseTypeCoord;

//.............................................................................

class CCast_Struct: public ICastOperator
{
public:
	AXL_OBJ_CLASS_0 (CCast_Struct, ICastOperator)

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