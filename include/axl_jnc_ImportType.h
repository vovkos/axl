// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"
#include "axl_rtl_List.h"

namespace axl {
namespace jnc {
	
//.............................................................................

// primary usage:
// typeof (TImport)
// sizeof (TImport)

// data usage (for classes, spawns class ptr)
// TImport a;
// TImport a [];
// TImport foo (TImport x);

// pointer usage (spawns one of: data ptr, class ptr, function ptr, property ptr, autoev ptr)
// TImport* p;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EImportType
{
	EImportType_Undefined = 0,
	EImportType_Primary,
	EImportType_Data,
	EImportType_Pointer,
};

//.............................................................................

class CImportType: public CType
{
	friend class CTypeMgr;

protected:
	EImportType m_ImportTypeKind;
	CType* m_pActualType;

public:
	CImportType ();

	EImportType 
	GetImportTypeKind ()
	{
		return m_ImportTypeKind;
	}

	bool
	IsResolved ()
	{
		return m_pActualType != NULL;
	}

	CType* 
	GetActualType ()
	{
		ASSERT (m_pActualType); // by this time actual type should be known or compilation should be halted
		return m_pActualType;
	}

	virtual 
	bool
	CalcLayout ();

protected:
	virtual 
	void
	PrepareLlvmType ();
};

//.............................................................................

class CPrimaryImportType: public CImportType
{
	friend class CTypeMgr;

protected:
	CQualifiedName m_Name;
	rtl::CString m_QualifiedName;
	CNamespace* m_pAnchorNamespace;

public:
	CPrimaryImportType ()
	{
		m_ImportTypeKind = EImportType_Primary;
		m_pAnchorNamespace = NULL;	
	}

	const CQualifiedName&
	GetName ()
	{
		return m_Name;
	}

	CNamespace* 
	GetAnchorNamespace ()
	{
		return m_pAnchorNamespace;
	}

	rtl::CString
	GetQualifiedName ();

	static
	rtl::CString
	CreateSignature (
		const CQualifiedName& Name,
		CNamespace* pAnchorNamespace
		)
	{
		return rtl::CString::Format_s ("ZN%s", pAnchorNamespace->CreateQualifiedName (Name).cc ());
	}

protected:
	virtual 
	void
	PrepareTypeString ();
};

//.............................................................................

class CSecondaryImportType: public CImportType
{
	friend class CTypeMgr;

protected:
	CPrimaryImportType* m_pPrimaryImportType;
	uint_t m_TypeModifiers;

public:
	CSecondaryImportType ()
	{
		m_pPrimaryImportType = NULL;
		m_TypeModifiers = 0;
	}

	CPrimaryImportType* 
	GetPrimaryImportType ()
	{
		return m_pPrimaryImportType;
	}

	uint_t
	GetTypeModifiers ()
	{
		return m_TypeModifiers;
	}

	static
	rtl::CString
	CreateSignature (
		EImportType ImportTypeKind,
		CPrimaryImportType* pPrimaryImportType,
		uint_t TypeModifiers
		);

protected:
	virtual 
	void
	PrepareTypeString ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
