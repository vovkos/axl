// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_PointerType.h"
#include "axl_jnc_ArrayType.h"
#include "axl_jnc_BitFieldType.h"
#include "axl_jnc_FunctionType.h"
#include "axl_jnc_FunctionPointerType.h"
#include "axl_jnc_EventType.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_PropertyPointerType.h"
#include "axl_jnc_EnumType.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

enum EStdType
{
	EStdType_BytePtr,
	EStdType_SafePtrValidator,
	EStdType_ObjectHdr,
	EStdType_AbstractInterfacePtr,
	EStdType_AbstractFunctionPtr,
	EStdType_AbstractEvent,
	EStdType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CTypeMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	CType m_PrimitiveTypeArray [EType__PrimitiveTypeCount];
	CType* m_StdTypeArray [EStdType__Count];

	rtl::CStdListT <CQualifierType> m_QualifierTypeList;
	rtl::CStdListT <CPointerType> m_PointerTypeList;
	rtl::CStdListT <CBitFieldType> m_BitFieldTypeList;
	rtl::CStdListT <CEnumType> m_EnumTypeList;
	rtl::CStdListT <CArrayType> m_ArrayTypeList;
	rtl::CStdListT <CStructType> m_StructTypeList;
	rtl::CStdListT <CUnionType> m_UnionTypeList;
	rtl::CStdListT <CClassType> m_ClassTypeList;
	rtl::CStdListT <CFunctionType> m_FunctionTypeList;
	rtl::CStdListT <CFunctionPointerType> m_FunctionPointerTypeList;
	rtl::CStdListT <CEventType> m_EventTypeList;
	rtl::CStdListT <CPropertyType> m_PropertyTypeList;
	rtl::CStdListT <CPropertyPointerType> m_PropertyPointerTypeList;
	rtl::CStdListT <CImportType> m_ImportTypeList;

	rtl::CStringHashTableMapAT <CType*> m_TypeMap;

	size_t m_UnnamedTypeCounter;

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

	bool
	BuildMethodTables ();

	CType* 
	GetPrimitiveType (EType TypeKind)
	{
		ASSERT (TypeKind < EType__PrimitiveTypeCount);
		return &m_PrimitiveTypeArray [TypeKind];
	}

	CType*
	GetStdType (EStdType StdType);

	CType*
	GetInt32Type (int32_t Integer)
	{
		return GetPrimitiveType (GetInt32TypeKind (Integer));
	}

	CType*
	GetUInt32Type (
		uint32_t Integer,
		bool ForceUnsigned
		)
	{
		return GetPrimitiveType (GetUInt32TypeKind (Integer, ForceUnsigned));
	}

	CType*
	GetInt64Type (int64_t Integer)
	{
		return GetPrimitiveType (GetInt64TypeKind (Integer));
	}

	CType*
	GetUInt64Type (
		uint64_t Integer,
		bool ForceUnsigned
		)
	{
		return GetPrimitiveType (GetUInt64TypeKind (Integer, ForceUnsigned));
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
		return GetQualifiedType (GetPrimitiveType (TypeKind), Flags);
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
		return GetPointerType (TypeKind, GetPrimitiveType (BaseTypeKind));
	}

	CFunctionPointerType* 
	GetFunctionPointerType (CFunctionType* pFunctionType);

	CEventType* 
	GetEventType (CFunctionType* pFunctionType);

	CPropertyPointerType* 
	GetPropertyPointerType (CPropertyType* pPropertyType);

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
		return GetArrayType (GetPrimitiveType (BaseTypeKind), ElementCount);
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
		ECallConv CallConv,
		CType* pReturnType,
		const rtl::CArrayT <CType*>& ArgTypeArray,
		int Flags = 0
		);
	
	CFunctionType* 
	CTypeMgr::GetFunctionType (	
		ECallConv CallConv,
		CType* pReturnType,
		CType* const* ppArgType,
		size_t ArgCount,
		int Flags = 0
		)
	{
		return GetFunctionType (CallConv, pReturnType, rtl::CArrayT <CType*> (ppArgType, ArgCount), Flags);
	}

	CFunctionType* 
	GetFunctionType (
		CType* pReturnType,
		const rtl::CArrayT <CType*>& ArgTypeArray,
		int Flags = 0
		)
	{
		return GetFunctionType (ECallConv_Default, pReturnType, ArgTypeArray, Flags);
	}
	
	CFunctionType* 
	CTypeMgr::GetFunctionType (	
		CType* pReturnType,
		CType* const* ppArgType,
		size_t ArgCount,
		int Flags = 0
		)
	{
		return GetFunctionType (ECallConv_Default, pReturnType, rtl::CArrayT <CType*> (ppArgType, ArgCount), Flags);
	}


	CPropertyType* 
	CreatePropertyType ();

	CPropertyType* 
	CreatePropertyType (
		CType* pReturnType,
		bool IsReadOnly
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
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		size_t PackFactor = 8
		);

	CStructType* 
	CreateUnnamedStructType (size_t PackFactor = 8)
	{
		return GetStructType (rtl::CString (), rtl::CString (), PackFactor);
	}

	CUnionType* 
	GetUnionType (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CUnionType* 
	CreateUnnamedUnionType ()
	{
		return GetUnionType (rtl::CString (), rtl::CString ());
	}

	CClassType* 
	GetClassType (
		EType TypeKind,
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		size_t PackFactor
		);

	CClassType* 
	CreateUnnamedClassType (
		EType TypeKind,
		size_t PackFactor = 8
		)
	{
		return GetClassType (TypeKind, rtl::CString (), rtl::CString (), PackFactor);
	}

	CImportType*
	GetImportType (	
		const CQualifiedName& Name,
		CNamespace* pAnchorNamespace
		);

protected:
	void
	SetupAllPrimitiveTypes ();

	void
	SetupPrimitiveType (
		EType TypeKind,
		size_t Size,
		const char* pSignature
		);

	CStructType* 
	CreateSafePtrValidatorType ();

	CStructType*
	CreateObjectHdrType ();

	CPointerType*
	CreateAbstractInterfaceType ();

	CFunctionPointerType*
	CreateAbstractFunctionPointerType ();

	CEventType*
	CreateAbstractEventType ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
