// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................

class CStructType;

class CPointerType: public CDerivedType
{
protected:
	friend class CTypeMgr;

	CStructType* m_pPointerStructType;

public:
	CPointerType ()
	{
		m_TypeKind = EType_Pointer;
	}

	CStructType* 
	GetPointerStructType ();

	llvm::Type* 
	GetLlvmType ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
