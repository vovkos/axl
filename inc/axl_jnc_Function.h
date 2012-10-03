// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

class CBasicBlock;
class CValue;

//.............................................................................

class CFunctionFormalArg: public rtl::TListLink
{
protected:
	friend class CFunctionMgr;
	friend class CDeclFormalArgSuffix;

	rtl::CString m_Name;
	CType* m_pType;
	CValue* m_pDefaultValue;

public:
	CFunctionFormalArg ()
	{
		m_pType = NULL;
		m_pDefaultValue = NULL;
	}

	rtl::CString 
	GetName ()
	{
		return m_Name;
	}

	CType* 
	GetType ()
	{
		return m_pType;
	}

	CValue* 
	GetDefaultValue ()
	{
		return m_pDefaultValue;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CFunction: 
	public CModuleItem,
	public rtl::TListLink
{
protected:
	friend class CFunctionMgr;

	CFunctionType* m_pType;
	CBasicBlock* m_pBlock;
	void* m_pfn;

	rtl::CStdListT <CFunctionFormalArg> m_ArgList;

public:
	CFunction ()
	{
		m_ItemKind = EModuleItem_Function;
		m_pType = NULL;
		m_pBlock = NULL;
		m_pfn = NULL;
	}

	CFunctionType* 
	GetType ()
	{
		return m_pType;
	}

	size_t 
	GetArgCount ()
	{
		return m_ArgList.GetCount ();
	}

	rtl::CIteratorT <CFunctionFormalArg> 
	GetFirstArg ()
	{
		return m_ArgList.GetHead ();
	}

	rtl::CString
	CreateArgString ();
};

//.............................................................................

class CFunctionOverload
{
protected:
	friend class CFunctionMgr;

	CFunction* m_pFunction;
	rtl::CArrayT <CFunction*> m_OverloadArray;

public:
	CFunctionOverload ()
	{
		m_pFunction = NULL;
	}

	bool 
	IsEmpty () const
	{
		return m_pFunction == NULL;
	}

	bool
	IsOverloaded () const
	{
		return !m_OverloadArray.IsEmpty ();
	}

	size_t
	GetOverloadCount () const
	{
		return !IsEmpty () ? m_OverloadArray.GetCount () + 1 : 0;
	}

	CFunction*
	GetFunction (size_t Overload = 0) const
	{
		return 
			Overload == 0 ? m_pFunction : 
			Overload <= m_OverloadArray.GetCount () ? m_OverloadArray [Overload - 1] : NULL;
	}

	bool
	AddOverload (CFunction* pFunction);

	CFunction*
	FindOverload (
		CType** ppArgType,
		size_t ArgCount
		) const;
};

//.............................................................................

class CGlobalFunction:
	public CModuleItem,
	public CFunctionOverload,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CFunctionMgr;

public:
	CGlobalFunction ()
	{
		m_ItemKind = EModuleItem_GlobalFunction;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
