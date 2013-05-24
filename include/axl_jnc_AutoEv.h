// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_AutoEvType.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EAutoEvField
{
	EAutoEvField_Lock,
	EAutoEvField_State,
	EAutoEvField_BindSiteArray,
	EAutoEvField__Count,
};

//.............................................................................

class CAutoEv: 
	public CModuleItem,
	public CNamespace
{
	friend class CFunctionMgr;
	friend class CClassType;
	friend class CProperty;
	friend class CParser;

protected:
	CAutoEvType* m_pType;
	CFunction* m_pStarter;
	CFunction* m_pStopper;
	
	CClassType* m_pParentClassType;
	CStructField* m_pParentClassField;
	
	CStructType* m_pDataStructType;
	CStructField* m_FieldArray [EAutoEvField__Count];
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
	GetParentClassField ()
	{
		return m_pParentClassField;
	}

	CStructType* 
	GetDataStructType ()
	{
		return m_pDataStructType;
	}

	rtl::CIteratorT <CStructField> 
	GetFirstArgField ()
	{
		return m_pDataStructType->GetFieldList ().GetHead () + 2;
	}

	CStructField* 
	GetField (EAutoEvField Field)
	{
		ASSERT (Field < EAutoEvField__Count);
		return m_FieldArray [Field];
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
	Create (CAutoEvType* pType);

	CFunction* 
	CreateHandler ();
	
	bool
	CreateStopper ();

	bool
	CalcLayout ();

protected:
	void
	CreateVTablePtr ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {


