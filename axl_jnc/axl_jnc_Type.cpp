#include "stdafx.h"
#include "axl_jnc_Type.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

EType
GetInt32TypeKind (int32_t Integer)
{
	return
		Integer >= INT8_MIN && Integer <= INT8_MAX ? EType_Int8 :
		(uint32_t) Integer <= UINT8_MAX ? EType_Int8_u :
		Integer >= INT16_MIN && Integer <= INT16_MAX ? EType_Int16 : 
		(uint32_t) Integer <= UINT16_MAX ? EType_Int16_u : 
		Integer >= INT32_MIN && Integer <= INT32_MAX ? EType_Int32 : EType_Int32_u;
}

EType
GetUInt32TypeKind (
	uint32_t Integer,
	bool ForceUnsigned
	)
{
	if (ForceUnsigned)
		return
			Integer <= UINT8_MAX ? EType_Int8_u :
			Integer <= UINT16_MAX ? EType_Int16_u : EType_Int32_u;
	else
		return 
			Integer <= INT8_MAX ? EType_Int8 :
			Integer <= UINT8_MAX ? EType_Int8_u :
			Integer <= INT16_MAX ? EType_Int16 : 
			Integer <= UINT16_MAX ? EType_Int16_u : 
			Integer <= INT32_MAX ? EType_Int32 : EType_Int32_u;
}

EType
GetInt64TypeKind (int64_t Integer)
{
	return
		Integer >= INT8_MIN && Integer <= INT8_MAX ? EType_Int8 :
		(uint64_t) Integer <= UINT8_MAX ? EType_Int8_u :
		Integer >= INT16_MIN && Integer <= INT16_MAX ? EType_Int16 : 
		(uint64_t) Integer <= UINT16_MAX ? EType_Int16_u : 
		Integer >= INT32_MIN && Integer <= INT32_MAX ? EType_Int32 : 
		(uint64_t) Integer <= UINT32_MAX ? EType_Int32_u : 
		Integer >= INT64_MIN && Integer <= INT64_MAX ? EType_Int64 : EType_Int64_u;
}

EType
GetUInt64TypeKind (
	uint64_t Integer,
	bool ForceUnsigned
	)
{
	if (ForceUnsigned)
		return
			Integer <= UINT8_MAX ? EType_Int8_u :
			Integer <= UINT16_MAX ? EType_Int16_u : 
			Integer <= UINT32_MAX ? EType_Int32_u : EType_Int64_u;
	else
		return 
			Integer <= INT8_MAX ? EType_Int8 :
			Integer <= UINT8_MAX ? EType_Int8_u :
			Integer <= INT16_MAX ? EType_Int16 : 
			Integer <= UINT16_MAX ? EType_Int16_u : 
			Integer <= INT32_MAX ? EType_Int32 : 
			Integer <= UINT32_MAX ? EType_Int32_u : 
			Integer <= INT64_MAX ? EType_Int64 : EType_Int64_u;
}

//.............................................................................

rtl::CString 
GetLlvmTypeString (llvm::Type* pLlvmType)
{
	std::string s;
	llvm::raw_string_ostream Stream (s);
	pLlvmType->print (Stream);
	return Stream.str ().c_str ();
}

//.............................................................................

const tchar_t*
GetTypeModifierString (ETypeModifier Modifier)
{
	static const tchar_t* StringTable [] = 
	{
		_T("signed"),        // ETypeModifier_Signed        = 0x00000001,
		_T("unsigned"),      // ETypeModifier_Unsigned      = 0x00000002,
		_T("littleendian"),  // ETypeModifier_LittleEndian  = 0x00000004,
		_T("bigendian"),     // ETypeModifier_BigEndian     = 0x00000008,
		_T("const"),         // ETypeModifier_Const         = 0x00000010,
		_T("readonly"),      // ETypeModifier_ReadOnly      = 0x00000020,
		_T("volatile"),      // ETypeModifier_Volatile      = 0x00000040,
		_T("safe"),          // ETypeModifier_Safe          = 0x00000080,
		_T("unsafe"),        // ETypeModifier_Unsafe        = 0x00000100,
		_T("nonull"),        // ETypeModifier_NoNull        = 0x00000200,
		_T("strong"),        // ETypeModifier_Strong        = 0x00000400,
		_T("weak"),          // ETypeModifier_Weak          = 0x00000800,
		_T("cdecl"),         // ETypeModifier_Cdecl         = 0x00001000,
		_T("stdcall"),       // ETypeModifier_Stdcall       = 0x00002000,
		_T("function"),      // ETypeModifier_Function      = 0x00004000,
		_T("property"),      // ETypeModifier_Property      = 0x00008000,
		_T("multicast"),     // ETypeModifier_Multicast     = 0x00010000,
		_T("event"),         // ETypeModifier_Event         = 0x00020000,
		_T("bindable"),      // ETypeModifier_Bindable      = 0x00040000,
		_T("autoget"),       // ETypeModifier_AutoGet       = 0x00080000,
		_T("indexed"),       // ETypeModifier_Indexed       = 0x00100000,
		_T("closure"),       // ETypeModifier_Closure       = 0x00200000,
		_T("thin"),          // ETypeModifier_Thin          = 0x00400000,
	};

	size_t i = rtl::GetLoBitIdx32 (Modifier);
	return i < countof (StringTable) ? 
		StringTable [i] : 
		_T("undefined-type-modifier");
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t* 
GetPtrTypeFlagString (EPtrTypeFlag Flag)
{
	static const tchar_t* StringTable [] = 
	{
		_T("nonull"),   // EPtrTypeFlag_NoNull    = 0x0100,
		_T("const"),    // EPtrTypeFlag_Const     = 0x0200,
		_T("readonly"), // EPtrTypeFlag_ReadOnly  = 0x0400,
		_T("volatile"), // EPtrTypeFlag_Volatile  = 0x0800,
		_T("event"),    // EPtrTypeFlag_Event     = 0x1000,
	};

	size_t i = rtl::GetLoBitIdx32 (Flag);
	return i < countof (StringTable) ? 
		StringTable [i] : 
		_T("undefined-ptr-type-flag");
}

//.............................................................................

CType::CType ()
{
	m_ItemKind = EModuleItem_Type;
	m_TypeKind = EType_Void;
	m_Flags = 0;
	m_Size = 0;
	m_pLlvmType = NULL;
	m_pDataPtrTypeTuple = NULL;
}

rtl::CString 
CType::GetTypeString ()
{
	if (!m_TypeString.IsEmpty ())
		return m_TypeString;

	PrepareTypeString ();
	
	ASSERT (!m_TypeString.IsEmpty ());
	return m_TypeString;
}

llvm::Type* 
CType::GetLlvmType ()
{
	if (m_pLlvmType)
		return m_pLlvmType;

	PrepareLlvmType ();
	
	ASSERT (m_pLlvmType);
	return m_pLlvmType;
}

CValue 
CType::GetUndefValue ()
{
	llvm::Value* pLlvmValue = llvm::UndefValue::get (GetLlvmType ());
	return CValue (pLlvmValue, this);
}

CValue 
CType::GetZeroValue ()
{
	llvm::Value* pLlvmValue = llvm::Constant::getNullValue (GetLlvmType ());
	return CValue (pLlvmValue, this);
}

bool
CType::PreCalcLayout ()
{
	ASSERT (!(m_Flags & ETypeFlag_LayoutReady));

	if (m_Flags & ETypeFlag_LayoutCalc)
	{
		err::SetFormatStringError (_T("can't calculate layout of '%s' due to type recursion"), GetTypeString ());
		return false;
	}

	m_Flags |= ETypeFlag_LayoutCalc;
	return true;
}

void
CType::PostCalcLayout ()
{
	m_Flags &= ~ETypeFlag_LayoutCalc;
	m_Flags |= ETypeFlag_LayoutReady;
}

bool 
CType::IsAutoSizeArrayType ()
{
	return 
		m_TypeKind == EType_Array &&
		((CArrayType*) this)->GetElementCount () == 0;
}

bool 
CType::IsCharArrayType ()
{
	return 
		m_TypeKind == EType_Array &&
		((CArrayType*) this)->GetElementType ()->GetTypeKind () == EType_Char;
}

bool
CType::IsBindablePropertyType ()
{
	return 
		m_TypeKind == EType_PropertyRef &&
		(((CPropertyPtrType*) this)->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Bindable) != 0;
}

CArrayType* 
CType::GetArrayType (size_t ElementCount)
{
	return m_pModule->m_TypeMgr.GetArrayType (this, ElementCount);
}

CDataPtrType* 
CType::GetDataPtrType (
	EType TypeKind,
	EDataPtrType PtrTypeKind,
	int Flags
	)
{
	return m_pModule->m_TypeMgr.GetDataPtrType (this, TypeKind, PtrTypeKind, Flags);
}

void
CType::PrepareTypeString ()
{
	static const tchar_t* StringTable [EType__PrimitiveTypeCount] = 
	{
		_T("void"),
		_T("variant"),
		_T("bool"),
		_T("int8"),
		_T("unsigned int8"),
		_T("int16"),
		_T("unsigned int16"),
		_T("int32"),
		_T("unsigned int32"),
		_T("int64"),
		_T("unsigned int64"),
		_T("bigendian int16"),
		_T("unsigned bigendian int16"),
		_T("bigendian int32"),
		_T("unsigned bigendian int32"),
		_T("bigendian int64"),
		_T("unsigned bigendian int64"),
		_T("float"),
		_T("double"),
	};

	ASSERT (m_TypeKind < EType__PrimitiveTypeCount);
	m_TypeString = StringTable [m_TypeKind];
}

void
CType::PrepareLlvmType ()
{
	ASSERT (m_TypeKind < EType__PrimitiveTypeCount);

	switch (m_TypeKind)
	{
	case EType_Void:
		m_pLlvmType = llvm::Type::getVoidTy (llvm::getGlobalContext ());
		break;
	
	case EType_Variant:
		ASSERT (false); // variants are not supported yet
		break;

	case EType_Bool:
		m_pLlvmType = llvm::Type::getInt1Ty (llvm::getGlobalContext ());
		break;

	case EType_Int8:
	case EType_Int8_u:
		m_pLlvmType = llvm::Type::getInt8Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int16:
	case EType_Int16_u:
	case EType_Int16_be:
	case EType_Int16_beu:
		m_pLlvmType = llvm::Type::getInt16Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int32:
	case EType_Int32_u:
	case EType_Int32_be:
	case EType_Int32_beu:
		m_pLlvmType = llvm::Type::getInt32Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int64:
	case EType_Int64_u:
	case EType_Int64_be:
	case EType_Int64_beu:
		m_pLlvmType = llvm::Type::getInt64Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Float:
		m_pLlvmType = llvm::Type::getFloatTy (llvm::getGlobalContext ());
		break;
	
	case EType_Double:
		m_pLlvmType = llvm::Type::getDoubleTy (llvm::getGlobalContext ());
		break;

	default:
		ASSERT (false);
	}
}

//.............................................................................

CModuleItem*
CNamedType::FindItemTraverseImpl (
	const tchar_t* pName,
	CBaseTypeCoord* pCoord,
	int Flags
	)
{
	CModuleItem* pItem;

	if (!(Flags & ETraverse_NoThis))
	{
		pItem = FindItem (pName);
		if (pItem)
			return pItem;
	}

	if (!(Flags & ETraverse_NoExtensionNamespace) && m_pExtensionNamespace)
	{
		pItem = m_pExtensionNamespace->FindItem (pName);
		if (pItem)
			return pItem;
	}

	if (!(Flags & ETraverse_NoParentNamespace) && m_pParentNamespace)
	{
		pItem = m_pParentNamespace->FindItemTraverse (pName, pCoord, Flags & ~ETraverse_NoThis);
		if (pItem)
			return pItem;
	}

	return NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
