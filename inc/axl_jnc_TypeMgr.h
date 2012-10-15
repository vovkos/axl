// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_ArrayType.h"
#include "axl_jnc_BitFieldType.h"
#include "axl_jnc_FunctionType.h"
#include "axl_jnc_EnumType.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

class CTypeMgr
{
protected:
	CType m_BasicTypeArray [EType_Variant + 1];

	rtl::CStdListT <CDerivedType> m_DerivedTypeList;
	rtl::CStdListT <CArrayType> m_ArrayTypeList;
	rtl::CStdListT <CBitFieldType> m_BitFieldTypeList;
	rtl::CStdListT <CFunctionType> m_FunctionTypeList;
	rtl::CStdListT <CPropertyType> m_PropertyTypeList;
	rtl::CStdListT <CEnumType> m_EnumTypeList;
	rtl::CStdListT <CStructType> m_StructTypeList;
	rtl::CStdListT <CClassType> m_ClassTypeList;
	rtl::CStdListT <CImportType> m_ImportTypeList;

	rtl::CHashTableMapT <const char*, CType*, rtl::CHashString, rtl::CCmpString> m_TypeMap;
	
public:
	CTypeMgr ()
	{
		SetupAllBasicTypes ();
	}

	void
	Clear ();

	CType* 
	GetBasicType (EType Type);

	CDerivedType* 
	GetConstType (CType* pBaseType);

	CDerivedType* 
	GetPointerType (CType* pBaseType);

	CDerivedType* 
	GetReferenceType (CType* pBaseType);

	CArrayType* 
	GetArrayType (
		CType* pBaseType,
		size_t ElementCount
		);

	CBitFieldType* 
	GetBitFieldType (
		CType* pBaseType,
		size_t BitOffset,
		size_t BitCount
		);

	CFunctionType* 
	GetFunctionType (
		CType* pReturnType,
		CType** ppArgType,
		size_t ArgCount,
		int Flags
		);

	CPropertyType* 
	GetPropertyType (
		CFunctionType* pGetterType,
		const CFunctionTypeOverload& SetterType
		);

	CEnumType* 
	GetEnumType (
		EType TypeKind,
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CEnumType* 
	CreateUnnamedEnumType (EType TypeKind)
	{
		return GetEnumType (TypeKind, rtl::CString (), rtl::CString ());
	}

	CStructType* 
	GetStructType (
		EType TypeKind,
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CStructType* 
	CreateUnnamedStructType (EType TypeKind)
	{
		return GetStructType (TypeKind, rtl::CString (), rtl::CString ());
	}

	CClassType* 
	GetClassType (
		EType TypeKind,
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CClassType* 
	CreateUnnamedClassType (EType TypeKind)
	{
		return GetClassType (TypeKind, rtl::CString (), rtl::CString ());
	}

	CImportType*
	GetImportType (	
		const CQualifiedName& Name,
		CNamespace* pAnchorNamespace
		);

protected:
	void
	SetupAllBasicTypes ();

	void
	SetupBasicType (
		EType Type,
		size_t Size,
		const char* pSignature
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
