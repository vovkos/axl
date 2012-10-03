// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LLK_AST_H

#include "axl_rtl_Array.h"

namespace axl {
namespace llk {
	
//.............................................................................

template <class TToken>
class CAstT: public rtl::TListLink
{
public:
	typedef TToken CToken;

public:
	int m_Kind;

	CToken m_FirstToken;
	CToken m_LastToken;		

	// later create a wrapper for ast tree 
	// we don't really need tree fields and listlink until we plan to keep ast nodes

	CAstT* m_pParent;
	rtl::CArrayT <CAstT*> m_Children;

public:
	CAstT ()
	{
		m_Kind = -1;
		m_pParent = NULL;
	}

	virtual
	~CAstT () // could be subclassed
	{
	}
};

//.............................................................................

} // namespace llk
} // namespace axl
