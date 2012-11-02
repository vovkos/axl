// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

class CConstMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	rtl::CBoxListT <CValue> m_ConstList;

public:
	CConstMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ()
	{
	}

	const CValue& 
	SaveValue (const CValue& Value)
	{
		rtl::CBoxIteratorT <CValue> It = m_ConstList.InsertTail (Value);
		return *It;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
