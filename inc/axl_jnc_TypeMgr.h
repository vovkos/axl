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
#include "axl_jnc_UnionType.h"
#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

class CTypeMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	CType m_BasicTypeArray [EType__BasicTypeCount];

	rtl::CStdListT <CDerivedType> m_DerivedTypeList;
	rtl::CStdListT <CBitFieldType> m_BitFieldTypeList;
	rtl::CStdListT <CEnumType> m_EnumTypeList;
	rtl::CStdListT <CArrayType> m_ArrayTypeList;
	rtl::CStdListT <CStructType> m_StructTypeList;
	rtl::CStdListT <CUnionType> m_UnionTypeList;
	rtl::CStdListT <CClassType> m_ClassTypeList;
	rtl::CStdListT <CFunctionType> m_FunctionTypeList;
	rtl::CStdListT <CPropertyType> m_PropertyTypeList;
	rtl::CStdListT <CImportType> m_ImportTypeList;

	rtl::CStringHashTableMapAT <CType*> m_TypeMap;

	CPointerType* m_pBytePtrType;
	CStructType* m_pSafePtrStructType;
	CStructType* m_pFunctionPtrStructType;
	
public:
	CTypeMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	bool
	ResolveImports ();

	CType* 
	GetBasicType (EType TypeKind)
	{
		ASSERT (TypeKind < EType__BasicTypeCount);
		return &m_BasicTypeArray [TypeKind];
	}

	CType*
	GetInt32Type (int32_t Integer)
	{
		return GetBasicType (GetInt32TypeKind (Integer));
	}

	CType*
	GetUInt32Type (
		uint32_t Integer,
		bool ForceUnsigned
		)
	{
		return GetBasicType (GetUInt32TypeKind (Integer, ForceUnsigned));
	}

	CType*
	GetInt64Type (int64_t Integer)
	{
		return GetBasicType (GetInt64TypeKind (Integer));
	}

	CType*
	GetUInt64Type (
		uint64_t Integer,
		bool ForceUnsigned
		)
	{
		return GetBasicType (GetUInt64TypeKind (Integer, ForceUnsigned));
	}

	CType* 
	GetQualifiedType (
		CType* pBaseType,
		int Flags
		);

	CType* 
	GetQualifiedType (
		EType TypeKind,
		int Flags
		)
	{
		return GetQualifiedType (GetBasicType (TypeKind), Flags);
	}

	CPointerType* 
	GetPointerType (
		EType TypeKind,
		CType* pBaseType
		);

	CPointerType* 
	GetPointerType (
		EType TypeKind,
		EType BaseTypeKind
		)
	{
		return GetPointerType (TypeKind, GetBasicType (BaseTypeKind));
	}

	CArrayType* 
	GetArrayType (
		CType* pBaseType,
		size_t ElementCount
		);

	CArrayType* 
	GetArrayType (
		EType BaseTypeKind,
		size_t ElementCount
		)
	{
		return GetArrayType (GetBasicType (BaseTypeKind), ElementCount);
	}

	CArrayType* 
	GetLiteralTypeA (size_t Length)
	{
		return GetArrayType (EType_Char, Length + 1);
	}

	CArrayType* 
	GetLiteralTypeW (size_t Length)
	{
		return GetArrayType (EType_WChar, Length + 1);
	}

	CArrayType* 
	GetLiteralType (size_t Length)
	{
#ifdef _UNICODE
		return GetLiteralTypeW (Length);
#else
		return GetLiteralTypeA (Length);
#endif
	}

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
		int Flags = 0
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
		const rtl::CString& QualifiedName,
		size_t PackFactor = 8
		);

	CStructType* 
	CreateUnnamedStructType (
		EType TypeKind,
		size_t PackFactor = 8
		)
	{
		return GetStructType (TypeKind, rtl::CString (), rtl::CString (), PackFactor);
	}

	CPointerType* 
	GetBytePtrType ();

	CStructType* 
	GetSafePtrStructType ();

	CStructType* 
	GetFunctionPtrStructType ();

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
		EType TypeKind,
		size_t Size,
		const char* pSignature
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
