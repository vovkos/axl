#include "pch.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {
	
//.............................................................................

const char* 
GetStdFieldString (EStdField Field)
{
	static const char* StringTable [] = 
	{
		"onchange",   // EStdField_OnChange,
		"value",      // EStdField_Value,
	};

	return (size_t) Field < countof (StringTable) ? 
		StringTable [Field] : 
		"undefined-std-field";
}

//.............................................................................
	
CStructField::CStructField ()
{
	m_ItemKind = EModuleItem_StructField;
	m_pType = NULL;
	m_PtrTypeFlags = 0;
	m_pBitFieldBaseType = NULL;
	m_BitCount = 0;
	m_Offset = 0;
	m_LlvmIndex = -1;
}

//.............................................................................

CStructType::CStructType ()
{
	m_TypeKind = EType_Struct;
	m_Flags = ETypeFlag_Pod | ETypeFlag_Moveable;
	m_AlignFactor = 1;
	m_PackFactor = 8;
	m_FieldActualSize = 0;
	m_FieldAlignedSize = 0;
	m_pLastBitFieldType = NULL;
	m_LastBitFieldOffset = 0;
}

bool
CStructType::IsClassStructType ()
{
	return 
		m_pParentNamespace->GetNamespaceKind () == ENamespace_Type &&
		((CNamedType*) m_pParentNamespace)->GetTypeKind () == EType_Class &&
		((CClassType*) m_pParentNamespace)->GetClassStructType () == this;
}

CStructField*
CStructType::CreateField (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	int PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	pType = m_pModule->m_TypeMgr.PrepareDataType (pType);

	CStructField* pField = AXL_MEM_NEW (CStructField);
	pField->m_StorageKind = m_StorageKind;
	pField->m_pParentType = this;
	pField->m_Name = Name;
	pField->m_pType = pType;
	pField->m_PtrTypeFlags = PtrTypeFlags;
	pField->m_pBitFieldBaseType = BitCount ? pType : NULL;
	pField->m_BitCount = BitCount;

	if (pInitializer)
	{
		pField->m_Initializer.TakeOver (pInitializer);		
		m_InitializedFieldArray.Append (pField);
	}

	m_FieldList.InsertTail (pField);

	if (!(pType->GetFlags () & ETypeFlag_Pod))
		m_Flags &= ~ETypeFlag_Pod;

	if (!(pType->GetFlags () & ETypeFlag_Moveable))
		m_Flags &= ~ETypeFlag_Moveable;

	if (!Name.IsEmpty ())
	{
		bool Result = AddItem (pField);
		if (!Result)
			return NULL;
	}

	return pField;
}

CStructField*
CStructType::GetFieldByIndex (size_t Index)
{
	size_t Count = m_FieldList.GetCount ();
	if (Index >= Count)
	{
		err::SetFormatStringError ("index '%d' is out of bounds", Index);
		return NULL;
	}

	if (m_FieldArray.GetCount () != Count)
	{
		m_FieldArray.SetCount (Count);
		rtl::CIteratorT <CStructField> Field = m_FieldList.GetHead ();
		for (size_t i = 0; i < Count; i++, Field++)
			m_FieldArray [i] = *Field;	
	}

	return m_FieldArray [Index];
}

bool
CStructType::Append (CStructType* pType)
{
	bool Result;

	rtl::CIteratorT <CBaseType> BaseType = pType->m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		Result = AddBaseType (BaseType->m_pType) != NULL;
		if (!Result)
			return false;
	}

	rtl::CIteratorT <CStructField> Field = pType->m_FieldList.GetHead ();
	for (; Field; Field++)
	{
		Result = Field->m_BitCount ? 
			CreateField (Field->m_Name, Field->m_pBitFieldBaseType, Field->m_BitCount, Field->m_PtrTypeFlags) != NULL:
			CreateField (Field->m_Name, Field->m_pType, 0, Field->m_PtrTypeFlags) != NULL;

		if (!Result)
			return false;
	}

	return true;
}

bool
CStructType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_LayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	if (m_pExtensionNamespace)
		ApplyExtensionNamespace ();

	bool HasBaseConstructor = false;

	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CBaseType* pBaseType = *BaseType;

		Result = pBaseType->m_pType->CalcLayout ();
		if (!Result)
			return false;

		if (pBaseType->m_pType->GetConstructor ())
			HasBaseConstructor = true;

		Result = LayoutField (
				pBaseType->m_pType,
				&pBaseType->m_Offset,
				&pBaseType->m_LlvmIndex
				);

		if (!Result)
			return false;
	}

	rtl::CIteratorT <CStructField> Field = m_FieldList.GetHead ();
	for (; Field; Field++)
	{
		CStructField* pField = *Field;

		Result = pField->m_pType->CalcLayout ();
		if (!Result)
			return false;

		Result = pField->m_BitCount ? 
			LayoutBitField (
				pField->m_pBitFieldBaseType,
				pField->m_BitCount,
				&pField->m_pType,
				&pField->m_Offset,
				&pField->m_LlvmIndex
				) :
			LayoutField (
				pField->m_pType,
				&pField->m_Offset,
				&pField->m_LlvmIndex
				);

		if (!Result)
			return false;
	}

	if (m_FieldAlignedSize > m_FieldActualSize)
		InsertPadding (m_FieldAlignedSize - m_FieldActualSize);

	llvm::StructType* pLlvmStructType = (llvm::StructType*) GetLlvmType ();
	pLlvmStructType->setBody (
		llvm::ArrayRef<llvm::Type*> (m_LlvmFieldTypeArray, m_LlvmFieldTypeArray.GetCount ()),
		true
		);

	m_Size = m_FieldAlignedSize;

	if (!m_pConstructor && 
		(HasBaseConstructor || !m_InitializedFieldArray.IsEmpty ()))
	{
		Result = CreateDefaultConstructor ();
		if (!Result)
			return false;
	}

	PostCalcLayout ();
	return true;
}

bool
CStructType::CallBaseTypeConstructors ()
{
	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	ASSERT (ThisValue);

	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CFunction* pConstructor = BaseType->m_pType->GetConstructor ();
		if (!pConstructor)
			return false;

		bool Result = m_pModule->m_OperatorMgr.CallOperator (pConstructor, ThisValue);
		if (!Result)
			return false;
	}

	return true;
}

bool
CStructType::InitializeFields ()
{
	bool Result;

	size_t Count = m_InitializedFieldArray.GetCount ();	
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = m_InitializedFieldArray [i];

		CValue FieldValue;
		Result = 
			m_pModule->m_OperatorMgr.GetField (pField, NULL, &FieldValue) &&
			m_pModule->m_OperatorMgr.ParseInitializer (FieldValue, pField->GetInitializer ());

		if (!Result)
			return false;
	}

	return true;
}

bool
CStructType::LayoutField (
	llvm::Type* pLlvmType,
	size_t Size,
	size_t AlignFactor,
	size_t* pOffset,
	uint_t* pLlvmIndex
	)
{
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	size_t Offset = GetFieldOffset (AlignFactor);
	if (Offset > m_FieldActualSize)
		InsertPadding (Offset - m_FieldActualSize);

	*pOffset = Offset;
	*pLlvmIndex = (uint_t) m_LlvmFieldTypeArray.GetCount ();

	m_pLastBitFieldType = NULL;
	m_LastBitFieldOffset = 0;

	m_LlvmFieldTypeArray.Append (pLlvmType);
	SetFieldActualSize (Offset + Size);
	return true;
}

bool
CStructType::LayoutBitField (
	CType* pBaseType,
	size_t BitCount,
	CType** ppType,
	size_t* pOffset,
	uint_t* pLlvmIndex
	)
{
	size_t BitOffset = GetBitFieldBitOffset (pBaseType, BitCount);
	CBitFieldType* pType = m_pModule->m_TypeMgr.GetBitFieldType (pBaseType, BitOffset, BitCount);
	if (!pType)
		return false;

	size_t AlignFactor = pType->GetAlignFactor ();
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	*ppType = pType;

	if (BitOffset)
	{
		*pOffset = m_LastBitFieldOffset;
		*pLlvmIndex = (uint_t) m_LlvmFieldTypeArray.GetCount () - 1;
		return true;
	}

	size_t Offset = GetFieldOffset (AlignFactor);
	m_pLastBitFieldType = pType;
	m_LastBitFieldOffset = Offset;
	
	if (Offset > m_FieldActualSize)
		InsertPadding (Offset - m_FieldActualSize);

	*pOffset = Offset;
	*pLlvmIndex = (uint_t) m_LlvmFieldTypeArray.GetCount ();

	m_LlvmFieldTypeArray.Append (pType->GetLlvmType ());
	SetFieldActualSize (Offset + pType->GetSize ());
	return true;
}

size_t
CStructType::GetFieldOffset (size_t AlignFactor)
{
	size_t Offset = m_FieldActualSize;

	if (AlignFactor > m_PackFactor)
		AlignFactor = m_PackFactor;

	size_t Mod = Offset % AlignFactor;
	if (Mod)
		Offset += AlignFactor - Mod;

	return Offset;
}

size_t 
CStructType::GetBitFieldBitOffset (
	CType* pType,
	size_t BitCount
	)
{
	if (!m_pLastBitFieldType || m_pLastBitFieldType->GetBaseType ()->Cmp (pType) != 0)
		return 0;

	size_t LastBitOffset = 
		m_pLastBitFieldType->GetBitOffset () + 
		m_pLastBitFieldType->GetBitCount ();

	return LastBitOffset + BitCount <= pType->GetSize () * 8 ? LastBitOffset : 0;
}

size_t
CStructType::SetFieldActualSize (size_t Size)
{
	if (m_FieldActualSize >= Size)
		return m_FieldAlignedSize;

	m_FieldActualSize = Size;
	m_FieldAlignedSize = Size;

	size_t Mod = m_Size % m_AlignFactor;
	if (Mod)
		m_FieldAlignedSize += m_AlignFactor - Mod;

	return m_FieldAlignedSize;
}

CArrayType*
CStructType::InsertPadding (size_t Size)
{
	CArrayType* pType = m_pModule->m_TypeMgr.GetArrayType (EType_Int8_u, Size);
	m_LlvmFieldTypeArray.Append (pType->GetLlvmType ());
	return pType;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
