#include "stdafx.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CClassType::CClassType ()
{
	m_TypeKind = EType_Class;
	m_PackFactor = 8;
	m_pInitializer = NULL;
	m_pFinalizer = NULL;
	m_pPointerStructType = NULL;
	m_pInterfaceStructType = NULL;
	m_pClassStructType = NULL;
	m_pVTableStructType = NULL;
}

bool
CClassType::FindBaseType (
	CClassType* pType,
	size_t* pOffset,
	rtl::CArrayT <size_t>* pLlvmIndexArray,
	size_t* pVTableIndex
	)
{
	rtl::CStringHashTableMapIteratorAT <CClassBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
	if (It)
	{
		CClassBaseType* pBaseType = It->m_Value;

		if (pOffset)
			*pOffset = pBaseType->GetOffset ();

		if (pLlvmIndexArray)
			pLlvmIndexArray->Copy (pBaseType->GetLlvmIndex ());

		if (pVTableIndex)
			*pVTableIndex = pBaseType->m_VTableIndex;

		return true;
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t Offset;
		size_t VTableIndex;

		bool Result = BaseType->m_pType->FindBaseType (pType, &Offset, &LlvmIndexArray, &VTableIndex);
		if (Result)
		{
			if (pOffset)
				*pOffset = BaseType->GetOffset () + Offset;

			if (pVTableIndex)
				*pVTableIndex = BaseType->m_VTableIndex + VTableIndex;

			if (pLlvmIndexArray)
			{
				pLlvmIndexArray->Clear ();
				pLlvmIndexArray->Append (BaseType->GetLlvmIndex ());
				pLlvmIndexArray->Append (LlvmIndexArray);
			}

			return true;
		}
	}

	return false;
}

CClassBaseType*
CClassType::AddBaseType (CClassType* pType)
{
	rtl::CStringHashTableMapIteratorAT <CClassBaseType*> It = m_BaseTypeMap.Goto (pType->GetSignature ());
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return NULL;
	}

	CClassBaseType* pBaseType = AXL_MEM_NEW (CClassBaseType);
	pBaseType->m_pType = pType;
	m_BaseTypeList.InsertTail (pBaseType);
	It->m_Value = pBaseType;
	return pBaseType;
}

CClassMember*
CClassType::FindMember (
	const tchar_t* pName,
	size_t* pBaseTypeOffset,
	rtl::CArrayT <size_t>* pLlvmBaseTypeIndexArray,
	size_t* pBaseTypeVTableIndex
	)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
	if (It)
	{
		CModuleItem* pItem = It->m_Value;
		if (pItem->GetItemKind () != EModuleItem_ClassMember)
			return NULL;

		if (pBaseTypeOffset)
			*pBaseTypeOffset = 0;

		if (pLlvmBaseTypeIndexArray)
			pLlvmBaseTypeIndexArray->Clear ();

		if (pBaseTypeVTableIndex)
			*pBaseTypeVTableIndex = 0;

		return (CClassMember*) pItem;
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmBaseTypeIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t BaseTypeOffset;
		size_t BaseTypeVTableIndex;

		CClassMember* pMember = BaseType->m_pType->FindMember (
			pName, 
			&BaseTypeOffset, 
			&LlvmBaseTypeIndexArray,
			&BaseTypeVTableIndex
			);

		if (pMember)
		{
			if (pBaseTypeOffset)
				*pBaseTypeOffset = BaseType->GetOffset () + BaseTypeOffset;

			if (pBaseTypeVTableIndex)
				*pBaseTypeVTableIndex = BaseType->m_VTableIndex + BaseTypeVTableIndex;

			if (pLlvmBaseTypeIndexArray)
			{
				pLlvmBaseTypeIndexArray->Clear ();
				pLlvmBaseTypeIndexArray->Append (BaseType->GetLlvmIndex ());
				pLlvmBaseTypeIndexArray->Append (LlvmBaseTypeIndexArray);	
			}

			return pMember;
		}
	}

	return NULL;
}

CClassFieldMember*
CClassType::CreateFieldMember (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount
	)
{
	CClassFieldMember* pMember = AXL_MEM_NEW (CClassFieldMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_pBitFieldBaseType = BitCount ? pType : NULL;
	pMember->m_BitCount = BitCount;
	m_FieldMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

CFunction*
CClassType::CreateMethodMember (
	const rtl::CString& Name,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CClassMethodMember* pMethodMember = NULL;

	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Goto (Name);
	if (!It->m_Value)
	{
		pMethodMember = AXL_MEM_NEW (CClassMethodMember);
		pMethodMember->m_Name = Name;
		pMethodMember->m_pParentNamespace = this;
		m_MethodMemberList.InsertTail (pMethodMember);
		It->m_Value = pMethodMember;
	}
	else
	{
		CModuleItem* pItem = It->m_Value;
		if (pItem->GetItemKind () == EModuleItem_ClassMember)
		{
			CClassMember* pMember = (CClassMember*) pItem;
			if (pMember->GetMemberKind () == EClassMember_Method)
				pMethodMember = (CClassMethodMember*) pMember;
		}

		if (!pMethodMember)
		{
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return NULL;
		}
	}

	ASSERT (pMethodMember);

	// adjust type

	rtl::CArrayT <CType*> ArgTypeArray = pType->GetArgTypeArray ();
	ArgTypeArray.Insert (0, this);
		
	CFunctionType* pFullType = m_pModule->m_TypeMgr.GetFunctionType (
		pType->GetReturnType (),
		ArgTypeArray,
		pType->GetFlags ()
		);

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (
		EFunction_Method,
		pMethodMember->GetQualifiedName (),
		pFullType,
		pArgList
		);

	pFunction->m_pClosureType = pType;

	bool Result = pMethodMember->AddOverload (pFunction);
	if (!Result)
		return NULL;

	return pFunction;
}

CClassPropertyMember*
CClassType::CreatePropertyMember (
	const rtl::CString& Name,
	CProperty* pProperty
	)
{
	size_t SetterCount = pProperty->GetSetter ()->GetOverloadCount ();
	size_t VTableIndex = m_VTable.GetCount ();
	m_VTable.SetCount (VTableIndex + 1 + SetterCount);

	CClassPropertyMember* pMember = AXL_MEM_NEW (CClassPropertyMember);
	pMember->m_Name = Name;
	pMember->m_pProperty = pProperty;
	pMember->m_VTableIndex = VTableIndex;
	m_PropertyMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

bool
CClassType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_IsLayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	m_pVTableStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pVTableStructType->m_Tag.Format (_T("%s.vtbl"), m_Tag);

	m_pInterfaceHdrStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	m_pInterfaceHdrStructType->m_Tag.Format (_T("%s.iface.hdr"), m_Tag);
	m_pInterfaceHdrStructType->CreateMember (m_pVTableStructType->GetPointerType (EType_Pointer_u));
	m_pInterfaceHdrStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectHdr)->GetPointerType (EType_Pointer_u));

	m_pInterfaceStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	m_pInterfaceStructType->m_Tag.Format (_T("%s.iface"), m_Tag);
	m_pInterfaceStructType->AddBaseType (m_pInterfaceHdrStructType);

	m_pPointerStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pPointerStructType->m_Tag.Format (_T("%s.ptr"), m_Tag);
	m_pPointerStructType->CreateMember (GetInterfaceStructType ()->GetPointerType (EType_Pointer_u));
	m_pPointerStructType->CreateMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pPointerStructType->CalcLayout ();

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassType* pBaseClassType = BaseType->m_pType;

		Result = pBaseClassType->CalcLayout ();
		if (!Result)
			return false;
				
		BaseType->m_pStructBaseType = m_pInterfaceStructType->AddBaseType (pBaseClassType->GetInterfaceStructType ());
		BaseType->m_VTableIndex = m_VTable.GetCount ();

		m_VTable.Append (pBaseClassType->m_VTable);

		rtl::CIteratorT <CStructMember> VTableMember = pBaseClassType->GetVTableStructType ()->GetFirstMember ();
		for (; VTableMember; VTableMember++)		
			m_pVTableStructType->CreateMember (VTableMember->GetType ());
	}

	rtl::CIteratorT <CClassFieldMember> FieldMember = m_FieldMemberList.GetHead ();
	for (; FieldMember; FieldMember++)
		FieldMember->m_pStructMember = m_pInterfaceStructType->CreateMember (FieldMember->m_pType, FieldMember->m_BitCount);

	rtl::CIteratorT <CClassMethodMember> MethodMember = m_MethodMemberList.GetHead ();
	for (; MethodMember; MethodMember++)
	{
		size_t Count = MethodMember->GetOverloadCount ();
		
		for (size_t i = 0; i < Count; i++)
		{
			CFunction* pFunction = MethodMember->GetFunction (i);
			
			size_t BaseTypeVTableIndex;
			CFunction* pOverridenFunction = FindOverridenMethodMember (
				MethodMember->m_Name,
				pFunction->GetClosureType (),
				&BaseTypeVTableIndex
				);

			if (pOverridenFunction)
			{
				size_t VTableIndex = BaseTypeVTableIndex + pOverridenFunction->GetVTableIndex ();
				pFunction->m_pType = pOverridenFunction->m_pType;
				pFunction->m_VTableIndex = VTableIndex;
				pFunction->m_pOriginClassType = pOverridenFunction->m_pOriginClassType;
				m_VTable [pFunction->m_VTableIndex] = pFunction;
			}
			else
			{
				pFunction->m_VTableIndex = m_VTable.GetCount ();
				pFunction->m_pOriginClassType = this;
				m_VTable.Append (pFunction);
				m_pVTableStructType->CreateMember (pFunction->GetType ()->GetPointerType (EType_Pointer_u));
			}
		}
	}

	m_pVTableStructType->CalcLayout ();
	m_pInterfaceStructType->CalcLayout ();

	if (m_TypeKind == EType_Class)
	{
		m_pClassStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
		m_pClassStructType->m_Tag.Format (_T("%s.class"), m_Tag);
		m_pClassStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectHdr));
		m_pClassStructType->CreateMember (m_pInterfaceStructType);
		m_pClassStructType->CalcLayout ();
	}

	PostCalcLayout ();

	return true;
}

bool
CClassType::GetVTablePtrValue (CValue* pValue)
{
	if (!m_VTablePtrValue.IsEmpty ())
	{
		*pValue = m_VTablePtrValue;
		return true;
	}

	char Buffer [256];
	rtl::CArrayT <llvm::Constant*> LlvmVTable (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	size_t Count = m_VTable.GetCount ();
	LlvmVTable.SetCount (Count);

	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_VTable [i];
		if (!pFunction->HasBody ())
		{
			err::SetFormatStringError (
				_T("cannot instantiate abstact '%s': '%s' has no body"), 
				GetTypeString (),
				pFunction->GetTag ()
				);
			return false;
		}

		LlvmVTable [i] = pFunction->GetLlvmFunction ();
	}

	llvm::Constant* pLlvmVTableConstant = llvm::ConstantStruct::get (
		(llvm::StructType*) m_pVTableStructType->GetLlvmType (),
		llvm::ArrayRef <llvm::Constant*> (LlvmVTable, Count)
		);

	rtl::CString VariableTag;
	VariableTag.Format (_T("%s.vtbl"), GetQualifiedName ());
	llvm::GlobalVariable* pLlvmVTableVariable = new llvm::GlobalVariable (
			*m_pModule->m_pLlvmModule,
			m_pVTableStructType->GetLlvmType (),
			false,
			llvm::GlobalVariable::ExternalLinkage,
			pLlvmVTableConstant,
			(const tchar_t*) VariableTag
			);

	m_VTablePtrValue.SetLlvmValue (
		pLlvmVTableVariable, 
		m_pVTableStructType->GetPointerType (EType_Pointer_u),
		EValue_Const
		);

	*pValue = m_VTablePtrValue;
	return true;
}

CFunction* 
CClassType::GetInitializer ()
{
	if (m_pInitializer)
		return m_pInitializer;

	m_pInitializer = m_pModule->m_FunctionMgr.CreateClassInitializer (this);
	return m_pInitializer;
}

CFunction*
CClassType::FindOverridenMethodMember (
	const rtl::CString& Name,
	CFunctionType* pClosureType,
	size_t* pBaseTypeVTableIndex
	)
{
	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t BaseTypeVTableIndex;
		CClassMember* pMember = BaseType->m_pType->FindMember (Name, NULL, NULL, &BaseTypeVTableIndex);
		if (!pMember || pMember->m_MemberKind != EClassMember_Method)
			continue;

		CClassMethodMember* pMethodMember = (CClassMethodMember*) pMember;
		size_t Count = pMethodMember->GetOverloadCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CFunction* pFunction = pMethodMember->GetFunction (i);
			if (pFunction->GetClosureType ()->Cmp (pClosureType) == 0)
			{
				*pBaseTypeVTableIndex = BaseType->m_VTableIndex + BaseTypeVTableIndex;
				return pFunction;
			}
		}
	}

	return NULL;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
