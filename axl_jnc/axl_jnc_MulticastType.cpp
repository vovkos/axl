#include "stdafx.h"
#include "axl_jnc_MulticastType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetMulticastTypeKindString (EMulticastType MulticastTypeKind)
{
	static const tchar_t* StringTable [EMulticastType__Count] = 
	{
		_T("multicast"),   // EMulticastType_Normal = 0,
		_T("mcsnapshot"),  // EMulticastType_Snapshot,
	};

	return MulticastTypeKind >= 0 && MulticastTypeKind < EMulticastType__Count ? 
		StringTable [MulticastTypeKind] : 
		_T("undefined-multicast-kind");
}

//.............................................................................

CMulticastType::CMulticastType ()
{
	m_TypeKind = EType_Multicast;
	m_Size = sizeof (TMulticast);
	m_pFunctionPtrType = NULL;
	m_pMulticastStructType = NULL;
}

CFunctionType* 
CMulticastType::GetFunctionType ()
{
	return m_pFunctionPtrType->GetTargetType ();
}

CStructType* 
CMulticastType::GetMulticastStructType ()
{
	if (m_pMulticastStructType)
		return m_pMulticastStructType;

	m_pMulticastStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pMulticastStructType->m_Tag.Format (_T("multicast"));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));
	m_pMulticastStructType->CreateFieldMember (m_pFunctionPtrType->GetDataPtrType (EDataPtrType_Unsafe));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));
	m_pMulticastStructType->CalcLayout ();

	return m_pMulticastStructType;
}

rtl::CStringA
CMulticastType::CreateSignature (
	CFunctionPtrType* pFunctionPtrType,
	EMulticastType MulticastTypeKind
	)
{
	rtl::CStringA Signature = 'M';

	switch (MulticastTypeKind)
	{
	case EMulticastType_Snapshot:
		Signature += 's';
		break;
	}

	Signature += pFunctionPtrType->GetSignature ();
	return Signature;
}

void
CMulticastType::PrepareTypeString ()
{
	m_TypeString = GetMulticastTypeKindString (m_MulticastTypeKind);
	m_TypeString += _T(' ');
	m_TypeString += m_pFunctionPtrType->GetTypeModifierString ();
	m_TypeString += m_pFunctionPtrType->GetTargetType ()->GetArgTypeString ();
}

void
CMulticastType::PrepareLlvmType ()
{
	m_pLlvmType = GetMulticastStructType ()->GetLlvmType ();
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
