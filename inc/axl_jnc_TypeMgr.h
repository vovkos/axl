// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_ArrayType.h"
#include "axl_jnc_BitFieldType.h"
#include "axl_jnc_EnumType.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_FunctionType.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_jnc_ClassPtrType.h"
#include "axl_jnc_FunctionPtrType.h"
#include "axl_jnc_PropertyPtrType.h"
#include "axl_jnc_MulticastType.h"
#include "axl_jnc_McSnapshotType.h"
#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

enum EStdType
{
	EStdType_BytePtr,
	EStdType_DataPtrValidator,
	EStdType_ObjectHdr,
	EStdType_ObjectClass,
	EStdType_ObjectPtr,
	EStdType_SimpleFunction,
	EStdType_SimpleEvent,
	EStdType_StrenthenClosureFunction,
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

	rtl::CStdListT <CArrayType> m_ArrayTypeList;
	rtl::CStdListT <CBitFieldType> m_BitFieldTypeList;
	rtl::CStdListT <CEnumType> m_EnumTypeList;
	rtl::CStdListT <CStructType> m_StructTypeList;
	rtl::CStdListT <CUnionType> m_UnionTypeList;
	rtl::CStdListT <CClassType> m_ClassTypeList;
	rtl::CStdListT <CFunctionType> m_FunctionTypeList;
	rtl::CStdListT <CPropertyType> m_PropertyTypeList;
	rtl::CStdListT <CDataPtrType> m_DataPtrTypeList;
	rtl::CStdListT <CClassPtrType> m_ClassPtrTypeList;
	rtl::CStdListT <CFunctionPtrType> m_FunctionPtrTypeList;
	rtl::CStdListT <CPropertyPtrType> m_PropertyPtrTypeList;
	rtl::CStdListT <CMulticastType> m_MulticastTypeList;
	rtl::CStdListT <CMcSnapshotType> m_McSnapshotTypeList;
	rtl::CStdListT <CImportType> m_ImportTypeList;
	rtl::CStdListT <CDataPtrTypeTuple> m_DataPtrTypeTupleList;
	rtl::CStdListT <CClassPtrTypeTuple> m_ClassPtrTypeTupleList;
	rtl::CStdListT <CFunctionPtrTypeTuple> m_FunctionPtrTypeTupleList;
	rtl::CStdListT <CPropertyPtrTypeTuple> m_PropertyPtrTypeTupleList;
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
	ResolveImportTypes ();

	bool
	CalcTypeLayouts ();

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

	CBitFieldType* 
	GetBitFieldType (
		CType* pBaseType,
		size_t BitOffset,
		size_t BitCount
		);

	CArrayType* 
	GetArrayType (
		CType* pElementType,
		size_t ElementCount
		);

	CArrayType* 
	GetArrayType (
		EType ElementTypeKind,
		size_t ElementCount
		)
	{
		return GetArrayType (GetPrimitiveType (ElementTypeKind), ElementCount);
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

	CEnumType* 
	CreateEnumType (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		int Flags = 0
		);

	CEnumType* 
	CreateUnnamedEnumType ()
	{
		return CreateEnumType (rtl::CString (), rtl::CString ());
	}

	CStructType* 
	CreateStructType (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		size_t PackFactor = 8
		);

	CStructType* 
	CreateUnnamedStructType (size_t PackFactor = 8)
	{
		return CreateStructType (rtl::CString (), rtl::CString (), PackFactor);
	}

	CStructType* 
	GetStructType (
		CType** ppMemberTypeArray,
		size_t MemberCount,
		size_t PackFactor = 8
		);

	CUnionType* 
	CreateUnionType (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CUnionType* 
	CreateUnnamedUnionType ()
	{
		return CreateUnionType (rtl::CString (), rtl::CString ());
	}

	CClassType* 
	CreateClassType (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		size_t PackFactor = 8
		);

	CClassType* 
	CreateUnnamedClassType (size_t PackFactor = 8)
	{
		return CreateClassType (rtl::CString (), rtl::CString (), PackFactor);
	}
	
	CFunctionType* 
	GetFunctionType (
		ECallConv CallConv,
		CType* pReturnType,
		const rtl::CArrayT <CType*>& ArgTypeArray,
		int Flags = 0
		);
	
	CFunctionType* 
	GetFunctionType (	
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
	GetFunctionType (	
		CType* pReturnType,
		CType* const* ppArgType,
		size_t ArgCount,
		int Flags = 0
		)
	{
		return GetFunctionType (ECallConv_Default, pReturnType, rtl::CArrayT <CType*> (ppArgType, ArgCount), Flags);
	}

	CFunctionType* 
	GetMethodMemberType (
		CClassType* pClassType,
		CFunctionType* pFunctionType,
		int ThisArgTypeFlags = 0
		);

	CFunctionType* 
	GetAbstractMethodMemberType (CFunctionType* pFunctionType);

	CFunctionType* 
	GetShortFunctionType (CFunctionType* pFunctionType);

	CPropertyType* 
	GetPropertyType (
		CFunctionType* pGetterType,
		const CFunctionTypeOverload& SetterType,
		int Flags = 0
		);

	CPropertyType* 
	GetSimplePropertyType (
		ECallConv CallConv,
		CType* pReturnType,
		int Flags = 0
		);

	CPropertyType* 
	GetSimplePropertyType (
		CType* pReturnType,
		int Flags = 0
		)
	{
		return GetSimplePropertyType (ECallConv_Default, pReturnType, Flags);
	}

	CPropertyType* 
	GetIndexedPropertyType (
		ECallConv CallConv,
		CType* pReturnType,
		const rtl::CArrayT <CType*>& ArgTypeArray,
		int Flags = 0
		);

	CPropertyType* 
	GetIndexedPropertyType (
		ECallConv CallConv,
		CType* pReturnType,
		CType* const* ppIndexArgType,
		size_t IndexArgCount,
		int Flags = 0
		)
	{
		return GetIndexedPropertyType (CallConv, pReturnType, rtl::CArrayT <CType*> (ppIndexArgType, IndexArgCount), Flags);
	}

	CPropertyType* 
	GetIndexedPropertyType (
		CType* pReturnType,
		const rtl::CArrayT <CType*>& ArgTypeArray,
		int Flags = 0
		)
	{
		return GetIndexedPropertyType (ECallConv_Default, pReturnType, ArgTypeArray, Flags);
	}

	CPropertyType* 
	GetIndexedPropertyType (
		CType* pReturnType,
		CType* const* ppIndexArgType,
		size_t IndexArgCount,
		int Flags = 0
		)
	{
		return GetIndexedPropertyType (ECallConv_Default, pReturnType, rtl::CArrayT <CType*> (ppIndexArgType, IndexArgCount), Flags);
	}

	CPropertyType* 
	GetPropertyMemberType (
		CClassType* pClassType,
		CPropertyType* pPropertyType
		);

	CPropertyType* 
	GetAbstractPropertyMemberType (CPropertyType* pPropertyType);

	CPropertyType* 
	GetShortPropertyType (CPropertyType* pPropertyType);

	CPropertyType* 
	GetBindablePropertyType (CPropertyType* pPropertyType);

	CMulticastType* 
	GetMulticastType (
		CFunctionType* pFunctionType,
		EFunctionPtrType PtrTypeKind = EFunctionPtrType_Normal
		)
	{
		return GetMulticastType (GetFunctionPtrType (pFunctionType, PtrTypeKind));
	}

	CMulticastType* 
	GetMulticastType (CFunctionPtrType* pFunctionPtrType);

	CMcSnapshotType* 
	GetMcSnapshotType (
		CFunctionType* pFunctionType,
		EFunctionPtrType PtrTypeKind = EFunctionPtrType_Normal
		)
	{
		return GetMcSnapshotType (GetFunctionPtrType (pFunctionType, PtrTypeKind));
	}

	CMcSnapshotType* 
	GetMcSnapshotType (CFunctionPtrType* pFunctionPtrType);

	CDataPtrType* 
	GetDataPtrType (
		CType* pDataType,
		EType TypeKind,
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		int Flags = 0
		);

	CDataPtrType* 
	GetDataPtrType (
		CType* pDataType,
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		int Flags = 0
		)
	{
		return GetDataPtrType (pDataType, EType_DataPtr, PtrTypeKind, Flags);
	}

	CStructType*
	GetDataPtrStructType (CType* pDataType);

	CClassPtrType* 
	GetClassPtrType (
		CClassType* pClassType,
		EClassPtrType PtrTypeKind = EClassPtrType_Normal,
		int Flags = 0
		);

	CFunctionPtrType* 
	GetFunctionPtrType (
		CFunctionType* pFunctionType,
		EType TypeKind,
		EFunctionPtrType PtrTypeKind = EFunctionPtrType_Normal,
		int Flags = 0
		);

	CFunctionPtrType* 
	GetFunctionPtrType (
		CFunctionType* pFunctionType,
		EFunctionPtrType PtrTypeKind = EFunctionPtrType_Normal,
		int Flags = 0
		)
	{
		return GetFunctionPtrType (pFunctionType, EType_FunctionPtr, PtrTypeKind, Flags);
	}

	CStructType* 
	GetClosureFunctionPtrStructType (CFunctionType* pFunctionType);

	CStructType* 
	GetWeakClosureFunctionPtrStructType (CFunctionType* pFunctionType);

	CPropertyPtrType* 
	GetPropertyPtrType (
		CPropertyType* pPropertyType,
		EType TypeKind,
		EPropertyPtrType PtrTypeKind = EPropertyPtrType_Normal,
		int Flags = 0
		);

	CPropertyPtrType* 
	GetPropertyPtrType (
		CPropertyType* pPropertyType,
		EPropertyPtrType PtrTypeKind = EPropertyPtrType_Normal,
		int Flags = 0
		)
	{
		return GetPropertyPtrType (pPropertyType, EType_PropertyPtr, PtrTypeKind, Flags);
	}

	CStructType*
	GetPropertyVTableStructType (CPropertyType* pPropertyType);

	CStructType*
	GetClosurePropertyPtrStructType (CPropertyType* pPropertyType);

	CStructType*
	GetWeakClosurePropertyPtrStructType (CPropertyType* pPropertyType);

	CStructType*
	GetThinAuPropertyPtrStructType (CPropertyType* pPropertyType);

	CStructType*
	GetUnsafeAuPropertyPtrStructType (CPropertyType* pPropertyType);

	CImportType*
	GetImportType (	
		const CQualifiedName& Name,
		CNamespace* pAnchorNamespace
		);

protected:
	CDataPtrTypeTuple*
	GetDataPtrTypeTuple (CType* pType);

	CClassPtrTypeTuple*
	GetClassPtrTypeTuple (CClassType* pClassType);

	CFunctionPtrTypeTuple*
	GetFunctionPtrTypeTuple (CFunctionType* pFunctionType);

	CPropertyPtrTypeTuple*
	GetPropertyPtrTypeTuple (CPropertyType* pPropertyType);

	void
	SetupAllPrimitiveTypes ();

	void
	SetupPrimitiveType (
		EType TypeKind,
		size_t Size,
		const char* pSignature
		);

	CStructType* 
	CreateDataPtrValidatorType ();

	CStructType*
	CreateObjectHdrType ();

	CClassType*
	CreateObjectType ();

	CStructType*
	CreateDataPtrStructType (CType* pDataType);

	CStructType* 
	CreateClosureFunctionPtrStructType (CFunctionType* pFunctionType);

	CStructType* 
	CreateWeakClosureFunctionPtrStructType (CFunctionType* pFunctionType);

	CStructType*
	CreatePropertyVTableStructType (CPropertyType* pPropertyType);

	CStructType*
	CreateClosurePropertyPtrStructType (CPropertyType* pPropertyType);

	CStructType*
	CreateWeakClosurePropertyPtrStructType (CPropertyType* pPropertyType);

	CStructType*
	CreateBindablePropertyThinPtrStructType (CPropertyType* pPropertyType);

	CStructType*
	CreateBindablePropertyUnsafePtrStructType (CPropertyType* pPropertyType);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
