// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
#include "axl_jnc_AutoEvType.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_jnc_ClassPtrType.h"
#include "axl_jnc_FunctionPtrType.h"
#include "axl_jnc_PropertyPtrType.h"
#include "axl_jnc_AutoEvPtrType.h"
#include "axl_jnc_MulticastType.h"
#include "axl_jnc_McSnapshotType.h"
#include "axl_jnc_AutoEvType.h"
#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

class CTypeMgr
{
	friend class CModule;

protected:
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
	rtl::CStdListT <CAutoEvType> m_AutoEvTypeList;
	rtl::CStdListT <CDataPtrType> m_DataPtrTypeList;
	rtl::CStdListT <CClassPtrType> m_ClassPtrTypeList;
	rtl::CStdListT <CFunctionPtrType> m_FunctionPtrTypeList;
	rtl::CStdListT <CPropertyPtrType> m_PropertyPtrTypeList;
	rtl::CStdListT <CAutoEvPtrType> m_AutoEvPtrTypeList;
	rtl::CStdListT <CMulticastType> m_MulticastTypeList;
	rtl::CStdListT <CMcSnapshotType> m_McSnapshotTypeList;
	rtl::CStdListT <CPrimaryImportType> m_PrimaryImportTypeList;
	rtl::CStdListT <CSecondaryImportType> m_SecondaryImportTypeList;
	
	rtl::CStdListT <CPropertyTypeTuple> m_PropertyTypeTupleList;
	rtl::CStdListT <CDataPtrTypeTuple> m_DataPtrTypeTupleList;
	rtl::CStdListT <CClassPtrTypeTuple> m_ClassPtrTypeTupleList;
	rtl::CStdListT <CFunctionPtrTypeTuple> m_FunctionPtrTypeTupleList;
	rtl::CStdListT <CPropertyPtrTypeTuple> m_PropertyPtrTypeTupleList;
	rtl::CStdListT <CAutoEvPtrTypeTuple> m_AutoEvPtrTypeTupleList;
	rtl::CStdListT <CFunctionArgTuple> m_FunctionArgTupleList;

	rtl::CStdListT <CFunctionArg> m_FunctionArgList;
	rtl::CStdListT <CTypedef> m_TypedefList;

	rtl::CStringHashTableMapAT <CType*> m_TypeMap;
	rtl::CStringHashTableMapAT <CClassType*> m_BoxClassTypeMap;

	rtl::CArrayT <CClassType*> m_StaticDestructArray; // classes with static destructors 

	size_t m_UnnamedEnumTypeCounter;
	size_t m_UnnamedStructTypeCounter;
	size_t m_UnnamedUnionTypeCounter;
	size_t m_UnnamedClassTypeCounter;

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

	rtl::CArrayT <CClassType*> 
	GetStaticDestructArray ()
	{
		return m_StaticDestructArray;
	}

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
	GetUInt32Type (uint32_t Integer)
	{
		return GetPrimitiveType (GetInt32TypeKind_u (Integer));
	}

	CType*
	GetInt64Type (int64_t Integer)
	{
		return GetPrimitiveType (GetInt64TypeKind (Integer));
	}

	CType*
	GetInt64Type_u (uint64_t Integer)
	{
		return GetPrimitiveType (GetInt64TypeKind_u (Integer));
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
	GetLiteralType (size_t Length)
	{
		return GetArrayType (EType_Char, Length + 1);
	}

	CArrayType* 
	GetLiteralType_w (size_t Length)
	{
		return GetArrayType (EType_WChar, Length + 1);
	}

	CTypedef*
	CreateTypedef (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		CType* pType
		);

	CEnumType* 
	CreateEnumType (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		CType* pBaseType = NULL,
		uint_t Flags = 0
		);

	CEnumType* 
	CreateUnnamedEnumType (CType* pBaseType = NULL)
	{
		return CreateEnumType (rtl::CString (), rtl::CString (), pBaseType = NULL);
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

	CClassType* 
	GetBoxClassType (CType* pBaseType);

	CFunctionArg*
	CreateFunctionArg (
		const rtl::CString& Name,
		CType* pType,
		int PtrTypeFlags = 0,
		rtl::CBoxListT <CToken>* pInitializer = NULL
		);

	CFunctionArg*
	GetSimpleFunctionArg (
		CType* pType,
		int PtrTypeFlags = 0
		);

	CFunctionType* 
	GetFunctionType (
		ECallConv CallConv,
		CType* pReturnType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray,
		uint_t Flags = 0
		);

	CFunctionType* 
	GetFunctionType (
		CType* pReturnType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray,
		uint_t Flags = 0
		)
	{
		return GetFunctionType (ECallConv_Default, pReturnType, ArgArray, Flags);
	}

	CFunctionType* 
	GetFunctionType (	
		ECallConv CallConv,
		CType* pReturnType,
		CType* const* ppArgType,
		size_t ArgCount,
		uint_t Flags = 0
		);

	CFunctionType* 
	GetFunctionType (	
		CType* pReturnType,
		CType* const* ppArgType,
		size_t ArgCount,
		uint_t Flags = 0
		)
	{
		return GetFunctionType (ECallConv_Default, pReturnType, ppArgType, ArgCount, Flags);
	}

	CFunctionType* 
	CreateUserFunctionType (
		ECallConv CallConv,
		CType* pReturnType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray,
		uint_t Flags = 0
		);

	CFunctionType* 
	CreateUserFunctionType (
		CType* pReturnType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray,
		uint_t Flags = 0
		)
	{
		return CreateUserFunctionType (ECallConv_Default, pReturnType, ArgArray, Flags);
	}

	CFunctionType* 
	GetMemberMethodType (
		CNamedType* pParentType,
		CFunctionType* pFunctionType,
		int ThisArgTypeFlags = 0
		);

	CFunctionType* 
	GetStdObjectMemberMethodType (CFunctionType* pFunctionType);

	CPropertyType* 
	GetPropertyType (
		CFunctionType* pGetterType,
		const CFunctionTypeOverload& SetterType,
		uint_t Flags = 0
		);

	CPropertyType* 
	GetSimplePropertyType (
		ECallConv CallConv,
		CType* pReturnType,
		uint_t Flags = 0
		);

	CPropertyType* 
	GetSimplePropertyType (
		CType* pReturnType,
		uint_t Flags = 0
		)
	{
		return GetSimplePropertyType (ECallConv_Default, pReturnType, Flags);
	}

	CPropertyType* 
	GetIndexedPropertyType (
		ECallConv CallConv,
		CType* pReturnType,
		CType* const* ppIndexArgType,
		size_t IndexArgCount,
		uint_t Flags = 0
		);

	CPropertyType* 
	GetIndexedPropertyType (
		CType* pReturnType,
		CType* const* ppIndexArgType,
		size_t IndexArgCount,
		uint_t Flags = 0
		)
	{
		return GetIndexedPropertyType (ECallConv_Default, pReturnType, ppIndexArgType, IndexArgCount, Flags);
	}

	CPropertyType* 
	GetIndexedPropertyType (
		ECallConv CallConv,
		CType* pReturnType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray,
		uint_t Flags = 0
		);

	CPropertyType* 
	GetIndexedPropertyType (
		CType* pReturnType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray,
		uint_t Flags = 0
		)
	{
		return GetIndexedPropertyType (ECallConv_Default, pReturnType, ArgArray, Flags);
	}

	CPropertyType* 
	CreateIndexedPropertyType (
		ECallConv CallConv,
		CType* pReturnType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray,
		uint_t Flags = 0
		);

	CPropertyType* 
	CreateIndexedPropertyType (
		CType* pReturnType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray,
		uint_t Flags = 0
		)
	{
		return CreateIndexedPropertyType (ECallConv_Default, pReturnType, ArgArray, Flags);
	}

	CPropertyType* 
	GetMemberPropertyType (
		CNamedType* pParentType,
		CPropertyType* pPropertyType
		);

	CPropertyType* 
	GetStdObjectMemberPropertyType (CPropertyType* pPropertyType);

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

	CAutoEvType* 
	GetAutoEvType (
		CFunctionType* pStarterType,
		CFunctionType* pStopperType = NULL
		);

	CAutoEvType* 
	GetMemberAutoEvType (
		CClassType* pClassType,
		CAutoEvType* pAutoEvType
		);

	CAutoEvType* 
	GetStdObjectMemberAutoEvType (CAutoEvType* pAutoEvType);

	CAutoEvType* 
	GetShortAutoEvType (CAutoEvType* pAutoEvType);

	CDataPtrType* 
	GetDataPtrType (
		CType* pDataType,
		EType TypeKind,
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		uint_t Flags = 0
		);

	CDataPtrType* 
	GetDataPtrType (
		CType* pDataType,
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		uint_t Flags = 0
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
		uint_t Flags = 0
		);
	
	CFunctionPtrType* 
	GetFunctionPtrType (
		CFunctionType* pFunctionType,
		EType TypeKind,
		EFunctionPtrType PtrTypeKind = EFunctionPtrType_Normal,
		uint_t Flags = 0
		);

	CFunctionPtrType* 
	GetFunctionPtrType (
		CFunctionType* pFunctionType,
		EFunctionPtrType PtrTypeKind = EFunctionPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetFunctionPtrType (pFunctionType, EType_FunctionPtr, PtrTypeKind, Flags);
	}

	CStructType* 
	GetFunctionPtrStructType (CFunctionType* pFunctionType);

	CStructType* 
	GetFunctionPtrStructType_w (CFunctionType* pFunctionType);

	CPropertyPtrType* 
	GetPropertyPtrType (
		CPropertyType* pPropertyType,
		EType TypeKind,
		EPropertyPtrType PtrTypeKind = EPropertyPtrType_Normal,
		uint_t Flags = 0
		);

	CPropertyPtrType* 
	GetPropertyPtrType (
		CPropertyType* pPropertyType,
		EPropertyPtrType PtrTypeKind = EPropertyPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetPropertyPtrType (pPropertyType, EType_PropertyPtr, PtrTypeKind, Flags);
	}

	CStructType*
	GetPropertyVTableStructType (CPropertyType* pPropertyType);

	CStructType*
	GetPropertyPtrStructType (CPropertyType* pPropertyType);

	CStructType*
	GetPropertyPtrStructType_w (CPropertyType* pPropertyType);

	CStructType*
	GetAuPropertyPtrStructType_t (CPropertyType* pPropertyType);

	CStructType*
	GetAuPropertyPtrStructType_u (CPropertyType* pPropertyType);
	
	CAutoEvPtrType* 
	GetAutoEvPtrType (
		CAutoEvType* pAutoEvType,
		EType TypeKind,
		EAutoEvPtrType PtrTypeKind = EAutoEvPtrType_Normal,
		uint_t Flags = 0
		);

	CAutoEvPtrType* 
	GetAutoEvPtrType (
		CAutoEvType* pAutoEvType,
		EAutoEvPtrType PtrTypeKind = EAutoEvPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetAutoEvPtrType (pAutoEvType, EType_AutoEvPtr, PtrTypeKind, Flags);
	}

	CStructType*
	GetAutoEvVTableStructType (CAutoEvType* pAutoEvType);

	CStructType*
	GetAutoEvPtrStructType (CAutoEvType* pAutoEvType);

	CStructType*
	GetAutoEvPtrStructType_w (CAutoEvType* pAutoEvType);
	
	CPrimaryImportType*
	GetPrimaryImportType (	
		const CQualifiedName& Name,
		CNamespace* pAnchorNamespace
		);

	CSecondaryImportType*
	GetSecondaryImportType (	
		EImportType ImportTypeKind,
		CPrimaryImportType* pPrimaryImportType,
		uint_t TypeModifiers = 0
		);

	CType*
	PrepareDataType (CType* pType);

protected:
	CDataPtrTypeTuple*
	GetDataPtrTypeTuple (CType* pType);

	CPropertyTypeTuple*
	GetPropertyTypeTuple (CType* pType);

	CFunctionArgTuple*
	GetFunctionArgTuple (CType* pType);

	CClassPtrTypeTuple*
	GetClassPtrTypeTuple (CClassType* pClassType);

	CFunctionPtrTypeTuple*
	GetFunctionPtrTypeTuple (CFunctionType* pFunctionType);

	CPropertyPtrTypeTuple*
	GetPropertyPtrTypeTuple (CPropertyType* pPropertyType);

	CAutoEvPtrTypeTuple*
	GetAutoEvPtrTypeTuple (CAutoEvType* pAutoEvType);

	void
	SetupAllPrimitiveTypes ();

	void
	SetupPrimitiveType (
		EType TypeKind,
		size_t Size,
		const char* pSignature
		);

	CStructType*
	CreateObjectHdrType ();

	CClassType*
	CreateObjectType ();

	CStructType* 
	CreateAutoEvBindSiteType ();

	CClassType* 
	CreateISchedulerType ();

	CStructType*
	CreateDataPtrStructType (CType* pDataType);

	CStructType* 
	CreateFunctionPtrStructType (CFunctionType* pFunctionType);

	CStructType* 
	CreateFunctionPtrStructType_w (CFunctionType* pFunctionType);

	CStructType*
	CreatePropertyVTableStructType (CPropertyType* pPropertyType);

	CStructType*
	CreatePropertyPtrStructType (CPropertyType* pPropertyType);

	CStructType*
	CreatePropertyPtrStructType_w (CPropertyType* pPropertyType);

	CStructType*
	CreateAuPropertyPtrStructType_t (CPropertyType* pPropertyType);

	CStructType*
	CreateAuPropertyPtrStructType_u (CPropertyType* pPropertyType);

	CStructType*
	CreateAutoEvVTableStructType (CAutoEvType* pAutoEvType);

	CStructType*
	CreateAutoEvPtrStructType (CAutoEvType* pAutoEvType);

	CStructType*
	CreateAutoEvPtrStructType_w (CAutoEvType* pAutoEvType);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {