#include "pch.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const char*
GetDataPtrTypeKindString (EDataPtrType PtrTypeKind)
{
	static const char* StringTable [EDataPtrType__Count] = 
	{
		"fat",    // EDataPtrType_Normal = 0,
		"thin",   // EDataPtrType_Thin,
	};
		
	return (size_t) PtrTypeKind < EDataPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		"undefined-data-ptr-kind";
}

//.............................................................................

CDataPtrType::CDataPtrType ()
{
	m_TypeKind = EType_DataPtr;
	m_PtrTypeKind = EDataPtrType_Normal;
	m_pTargetType = NULL;
	m_Size = sizeof (TDataPtr);
}

CStructType* 
CDataPtrType::GetDataPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetDataPtrStructType (m_pTargetType);
}

rtl::CString
CDataPtrType::CreateSignature (
	CType* pBaseType,
	EType TypeKind,
	EDataPtrType PtrTypeKind,
	uint_t Flags
	)
{
	rtl::CString Signature = TypeKind == EType_DataRef ? "RD" : "PD";

	if (PtrTypeKind == EDataPtrType_Thin)
		Signature += 't';

	Signature += GetPtrTypeFlagSignature (Flags);
	Signature += pBaseType->GetSignature ();
	return Signature;
}

void
CDataPtrType::PrepareTypeString ()
{
	m_TypeString = m_pTargetType->GetTypeString ();

	if (m_Flags & EPtrTypeFlag__AllMask)
	{
		m_TypeString += ' ';
		m_TypeString += GetPtrTypeFlagString (m_Flags);
	}

	if (m_PtrTypeKind != EDataPtrType_Normal)
	{
		m_TypeString += ' ';
		m_TypeString += GetDataPtrTypeKindString (m_PtrTypeKind);
	}

	m_TypeString += m_TypeKind == EType_DataRef ? "&" : "*";
}

void
CDataPtrType::PrepareLlvmType ()
{
	m_pLlvmType = 
		m_PtrTypeKind != EDataPtrType_Thin ? GetDataPtrStructType ()->GetLlvmType () :
		m_pTargetType->GetTypeKind () != EType_Void ? llvm::PointerType::get (m_pTargetType->GetLlvmType (), 0) :
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetLlvmType ();
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

