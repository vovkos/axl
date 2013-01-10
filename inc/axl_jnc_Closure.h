// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EClosureArgFlag
{
	EClosureArgFlag_WeakToStrong           = 1,
	EClosureArgFlag_WeakToStrongFailIfNull = 2,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClosureArg: public rtl::TListLink
{
protected:
	friend class CClosure;

	size_t m_ArgIdx;
	CValue m_Value;
	int m_Flags;

public:
	CClosureArg ()
	{
		m_ArgIdx = -1;
		m_Flags = 0;
	}

	size_t
	GetArgIdx ()
	{
		return m_ArgIdx;
	}

	CValue
	GetValue ()
	{
		return m_Value;
	}

	int
	GetFlags ()
	{
		return m_Flags;
	}
};

//.............................................................................

class CClosure: public ref::IRefCount
{
protected:
	rtl::CStdListT <CClosureArg> m_ArgList;

public:
	void
	Clear ()
	{
		m_ArgList.Clear ();
	}

	bool 
	IsEmpty ()
	{
		return m_ArgList.IsEmpty ();
	}

	size_t
	GetArgCount ()
	{
		return m_ArgList.GetCount ();
	}

	rtl::CIteratorT <CClosureArg>
	GetFirstArg ()
	{
		return m_ArgList.GetHead ();
	}

	CClosureArg*
	CreateArg (
		size_t ArgIdx,
		const CValue&  Value,
		int Flags = 0
		);

	bool
	Apply (rtl::CBoxListT <CValue>* pArgList);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
