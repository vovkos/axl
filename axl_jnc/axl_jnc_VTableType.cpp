#include "stdafx.h"
#include "axl_jnc_VTableType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CStructType*
CVTableType::CreateVTableStructType ()
{
	m_pVTableStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pVTableStructType->m_Tag.Format (_T("%s.vtbl"), m_Tag);
	return m_pVTableStructType;
}

bool
CVTableType::GetVTablePtrValue (CValue* pValue)
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
				_T("cannot instantiate abstract '%s': '%s' has no body"), 
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

void
CVTableType::AddFunctionToVTable (CFunction* pFunction)
{
	pFunction->m_VTableIndex = m_VTable.GetCount ();
	pFunction->m_pVTableType = this;
	m_VTable.Append (pFunction);

	CPointerType* pPointerType = m_pModule->m_TypeMgr.GetPointerType (EType_Pointer_u, pFunction->GetType ());
	m_pVTableStructType->CreateMember (pPointerType);
}

void
CVTableType::Append (CVTableType* pType)
{
	m_VTable.Append (pType->m_VTable);
		
	rtl::CIteratorT <CStructMember> VTableMember = pType->m_pVTableStructType->GetFirstMember ();
	for (; VTableMember; VTableMember++)		
		m_pVTableStructType->CreateMember (VTableMember->GetType ());
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
