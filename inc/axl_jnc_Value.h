// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................

class CValue
{
protected:
	CType* m_pType;

	union
	{
		int m_Int;
		__int64 m_Int64;
		double m_Double;
	};

public:
	CType* 
	GetType ()
	{
		return m_pType;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
