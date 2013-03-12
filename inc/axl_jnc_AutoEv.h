// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_AutoEvType.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

class CAutoEv: 
	public CModuleItem,
	public CNamespace
{
protected:
	friend class CFunctionMgr;
	friend class CClassType;
	friend class CProperty;
	friend class CParser;

	CAutoEvType* m_pType;
	CFunction* m_pStarter;
	CFunction* m_pStopper;
	
	CClassType* m_pParentClassType;
	CStructField* m_pParentClassFieldMember;
	
	CStructType* m_pFieldStructType;
	CStructField* m_pBindSiteArrayField;	
	CVariable* m_pStaticDataVariable;

	rtl::CBoxListT <CToken> m_Body;
	ref::CBufT <llk::CAstT <llk::CAstNodeT <CToken> > > m_Ast;

	// vtable

	rtl::CArrayT <CFunction*> m_VTable;
	CValue m_VTablePtrValue;

	size_t m_BindSiteCount;

public:
	CAutoEv ();

	CAutoEvType* 
	GetType ()
	{
		return m_pType;
	}

	CFunction* 
	GetStarter ()
	{
		return m_pStarter;
	}

	CFunction* 
	GetStopper ()
	{
		return m_pStopper;
	}

	CClassType* 
	GetParentClassType ()
	{
		return m_pParentClassType;
	}

	CStructField* 
	GetParentClassFieldMember ()
	{
		return m_pParentClassFieldMember;
	}

	CStructType* 
	GetFieldStructType ()
	{
		return m_pFieldStructType;
	}

	size_t 
	GetBindSiteCount ()
	{
		return m_BindSiteCount;
	}

	CStructField* 
	GetBindSiteArrayField ()
	{
		return m_pBindSiteArrayField;
	}

	CVariable* 
	GetStaticDataVariable ()
	{
		return m_pStaticDataVariable;
	}

	bool
	IsMember ()
	{
		return m_StorageKind == EStorage_Member;
	}

	bool
	HasBody ()
	{
		return !m_Body.IsEmpty ();
	}

	const rtl::CBoxListT <CToken>*
	GetBody ()
	{
		return &m_Body;
	}

	void
	SetBody (rtl::CBoxListT <CToken>* pTokenList)
	{
		m_Body.TakeOver (pTokenList);
	}

	ref::CBufT <llk::CAstT <llk::CAstNodeT <CToken> > > 
	GetAst ()
	{
		return m_Ast;
	}

	bool
	Create (
		CAutoEvType* pType,
		rtl::CStdListT <CFunctionFormalArg>* pArgList
		);

	CFunction* 
	CreateHandler ();

	bool
	CalcLayout ();


protected:
	void
	CreateVTablePtr ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {


