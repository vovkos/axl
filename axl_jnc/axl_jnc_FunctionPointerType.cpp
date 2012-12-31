#include "stdafx.h"
#include "axl_jnc_FunctionPointerType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionPointerType::CFunctionPointerType ()
{
	m_TypeKind = EType_FunctionPointer;
	m_Size = sizeof (TFunctionPtr);
	m_pFunctionType = NULL;
	m_pMemberFunctionType = NULL;
	m_pPointerStructType = NULL;
}

CFunctionType* 
CFunctionPointerType::GetMemberFunctionType ()
{
	if (m_pMemberFunctionType)
		return m_pMemberFunctionType;

	rtl::CArrayT <CType*> ArgTypeArray = m_pFunctionType->GetArgTypeArray ();
	ArgTypeArray.Insert (0, m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr));

	m_pMemberFunctionType = m_pModule->m_TypeMgr.GetFunctionType (
		m_pFunctionType->GetCallingConvention (),
		m_pFunctionType->GetReturnType (),
		ArgTypeArray,
		m_pFunctionType->GetFlags ()
		);

	return m_pMemberFunctionType;
}

CStructType* 
CFunctionPointerType::GetPointerStructType ()
{
	if (m_pPointerStructType)
		return m_pPointerStructType;

	m_pPointerStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pPointerStructType->m_Tag.Format (_T("pfn"));
	m_pPointerStructType->CreateMember (m_pFunctionType->GetPointerType (EType_Pointer_u));
	m_pPointerStructType->CreateMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));
	m_pPointerStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr));
	m_pPointerStructType->CalcLayout ();

	return m_pPointerStructType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
