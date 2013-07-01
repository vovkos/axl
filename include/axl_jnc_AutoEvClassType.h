// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EAutoEvField
{
	EAutoEvField_Parent,
	EAutoEvField_Lock,
	EAutoEvField_State,
	EAutoEvField_BindSiteArray,

	EAutoEvField__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EAutoEvMethod
{
	EAutoEvMethod_Start,
	EAutoEvMethod_Stop,
	EAutoEvMethod__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TAutoEvHandler: rtl::TListLink
{
	rtl::CBoxListT <CValue> m_BindSiteList;
	CFunction* m_pFunction;
};

//.............................................................................

class CAutoEvClassType: public CClassType
{
	friend class CTypeMgr;

protected:
	CStructField* m_FieldArray [EAutoEvField__Count];
	CFunction* m_MethodArray [EAutoEvMethod__Count];
	rtl::CIteratorT <CStructField> m_FirstArgField;
	size_t m_BindSiteCount;
	rtl::CBoxListT <CToken> m_Body;

public:
	CAutoEvClassType ();

	CStructField* 
	GetField (EAutoEvField Field)
	{
		ASSERT (Field < EAutoEvField__Count);
		return m_FieldArray [Field];
	}

	CFunction* 
	GetMethod (EAutoEvMethod Method)
	{
		ASSERT (Method < EAutoEvMethod__Count);
		return m_MethodArray [Method];
	}

	size_t 
	GetBindSiteCount ()
	{
		return m_BindSiteCount;
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

	bool
	SetBody (rtl::CBoxListT <CToken>* pTokenList);

	CFunction*
	CreateHandler ();

	bool
	BindHandlers (const rtl::CConstListT <TAutoEvHandler>& HandlerList);

	virtual
	bool
	Compile ()
	{
		// do not call CClass::Compile (it compiles default-constructor and default-destructor)

		return 
			CompilePrimer () &&
			CompileConstructor () &&
			CompileDestructor () &&
			CompileStartMethod () &&
			CompileStopMethod ();
	}

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format (
			"autoev %s %s", 
			m_QualifiedName.cc (),
			m_MethodArray [EAutoEvMethod_Start]->GetType ()->GetShortType ()->GetArgString ()
			);
	}

	virtual 
	bool
	CalcLayout ();

	bool
	CallStopMethod ();

	bool
	CompileConstructor ();

	bool
	CompileDestructor ();

	bool
	CompileStartMethod ();

	bool
	CompileStopMethod ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
