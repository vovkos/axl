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
	switch (Modifier)
	{
	case ETypeModifier_Signed:
		return _T("signed");

	case ETypeModifier_Unsigned:
		return _T("unsigned");

	case ETypeModifier_LittleEndian:
		return _T("littleendian");

	case ETypeModifier_BigEndian:
		return _T("bigendian");

	case ETypeModifier_Const:
		return _T("const");

	case ETypeModifier_Volatile:
		return _T("volatile");

	case ETypeModifier_Safe:
		return _T("safe");

	case ETypeModifier_Unsafe:
		return _T("unsafe");
		
	case ETypeModifier_NoNull:
		return _T("nonull");

	case ETypeModifier_Strong:
		return _T("strong");

	case ETypeModifier_Weak:
		return _T("weak");

	case ETypeModifier_Cdecl:
		return _T("cdecl");

	case ETypeModifier_Stdcall:
		return _T("stdcall");

	case ETypeModifier_Function:
		return _T("function");

	case ETypeModifier_Property:
		return _T("property");

	case ETypeModifier_Event:
		return _T("event");

	case ETypeModifier_Bindable:
		return _T("bindable");

	case ETypeModifier_AutoGet:
		return _T("autoget");

	case ETypeModifier_Indexed:
		return _T("indexed");

	case ETypeModifier_Closure:
		return _T("closure");

	case ETypeModifier_Thin:
		return _T("thin");

	default:
		return _T("undefined-type-modifier");
	};
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t* 
GetPtrTypeFlagString (EPtrTypeFlag Flag)
{
	switch (Flag)
	{
	case EPtrTypeFlag_NoNull:
		return _T("nonull");

	case EPtrTypeFlag_Const:
		return _T("const");

	case EPtrTypeFlag_ReadOnly:
		return _T("readonly");

	case EPtrTypeFlag_Volatile:
		return _T("volatile");

	default:
		return _T("undefined-ptr-type-flag");
	}
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
	static const tchar_t*
	PrimitiveTypeNameTable [EType__PrimitiveTypeCount] = 
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
	m_TypeString = PrimitiveTypeNameTable [m_TypeKind];
}

/*

	}
	else switch (m_TypeKind)
	{	
	case EType_DataPtr:
	case EType_DataRef:
		m_TypeString = ((CDataPtrType*) this)->CreateTypeString ();
		break;

	case EType_ClassPtr:
		m_TypeString = ((CClassPtrType*) this)->CreateTypeString ();
		break;

	case EType_FunctionPtr:
	case EType_FunctionRef:
		m_TypeString = ((CFunctionPtrType*) this)->CreateTypeString ();
		break;

	case EType_PropertyPtr:
	case EType_PropertyRef:
		m_TypeString = ((CPropertyPtrType*) this)->CreateTypeString ();
		break;

	case EType_Import:
		m_TypeString = ((CImportType*) this)->CreateTypeString ();
		break;

	default:
		ASSERT (false);
	}

	return m_TypeString;
}

*/

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

/*

	case EType_DataPtr:
	case EType_DataRef:
		return ((CDataPtrType*) this)->GetLlvmType ();

	case EType_ClassPtr:
		return ((CClassPtrType*) this)->GetLlvmType ();

	case EType_FunctionPtr:
	case EType_FunctionRef:
		pLlvmType = ((CFunctionPtrType*) this)->GetLlvmType ();
		break;

	case EType_PropertyPtr:
	case EType_PropertyRef:
		pLlvmType = ((CPropertyPtrType*) this)->GetLlvmType ();
		break;

	case EType_Import:
		return ((CImportType*) this)->GetExternType ()->GetLlvmType ();

	default:
		ASSERT (false);
	}

	m_pLlvmType = pLlvmType;
	return pLlvmType;
}

*/

//.............................................................................

} // namespace jnc {
} // namespace axl {
