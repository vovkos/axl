// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_OBJ_PTR_H

#include "axl_obj_Root.h"

namespace axl {
namespace obj {

//.............................................................................

// pointer to interface, queries interface when necessary

template <typename I>
class CPtrT
{
protected:
	I* m_p;

public:
	CPtrT ()
	{
		m_p = NULL;
	}

	CPtrT (I* p)
	{
		m_p = p;
	}

	CPtrT (IRoot* p)
	{
		m_p = (I*) p->GetInterface (__uuidof (I));
	}

	operator I* ()
	{
		return m_p;
	}

	I* 
	operator -> ()
	{
		return m_p;
	}
};

//.............................................................................

} // namespace obj
} // namespace axl
