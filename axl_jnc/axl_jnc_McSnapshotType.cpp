#include "stdafx.h"
#include "axl_jnc_McSnapshotType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CMcSnapshotType::CMcSnapshotType ()
{
	m_TypeKind = EType_McSnapshot;
	m_pTargetType = NULL;
	m_pMcSnapshotStructType = NULL;
	memset (m_FieldArray, 0, sizeof (m_FieldArray));
	memset (m_MethodArray, 0, sizeof (m_MethodArray));
}

void
CMcSnapshotType::PrepareTypeString ()
{
	m_TypeString += _T("mcsnapshot ");
	m_TypeString += m_pTargetType->GetTypeModifierString ();
	m_TypeString += m_pTargetType->GetTargetType ()->GetArgTypeString ();
}

void
CMcSnapshotType::PrepareLlvmType ()
{
	m_pLlvmType = GetMcSnapshotStructType ()->GetLlvmType ();
}

CStructType* 
CMcSnapshotType::GetMcSnapshotStructType ()
{
	if (m_pMcSnapshotStructType)
		return m_pMcSnapshotStructType;

	m_pMcSnapshotStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pMcSnapshotStructType->m_Tag.Format (_T("mcsnapshot"));

	m_FieldArray [EMcSnapshotField_Count] = m_pMcSnapshotStructType->CreateField (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_FieldArray [EMcSnapshotField_PtrArray] = m_pMcSnapshotStructType->CreateField (m_pTargetType->GetDataPtrType (EDataPtrType_Unsafe));
	
	m_pMcSnapshotStructType->CalcLayout ();

	return m_pMcSnapshotStructType;
}

CFunction* 
CMcSnapshotType::GetMethod (EMcSnapshotMethod Method)
{
	if (m_MethodArray [Method])
		return m_MethodArray [Method];

	CFunction* pFunction;

	switch (Method)
	{
	case EMcSnapshotMethod_Call:
		pFunction = CreateCallMethod ();
		break;

	default:
		ASSERT (false);
		pFunction = NULL;
	}

	m_MethodArray [Method] = pFunction;
	return pFunction;
}

CFunction* 
CMcSnapshotType::CreateCallMethod ()
{
	rtl::CArrayT <CType*> ArgTypeArray = m_pTargetType->GetTargetType ()->GetArgTypeArray ();
	ArgTypeArray.Insert (0, this);
	size_t ArgCount = ArgTypeArray.GetCount ();

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray);
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("McSnapshotCall"), pType);

	char Buffer [256];
	rtl::CArrayT <CValue> ArgValueArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgValueArray.SetCount (ArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, ArgCount);

	rtl::CBoxListT <CValue> ArgValueList;
	for (size_t i = 1; i < ArgCount; i++)
		ArgValueList.InsertTail (ArgValueArray [i]);

	CValue CountValue;
	CValue PtrPfnValue;
	CValue PtrPfnEndValue;
	CValue PtrPfnVariable;

	CType* pPtrPfnType = m_pTargetType->GetDataPtrType (EDataPtrType_Unsafe);
	CType* pSizeType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValueArray [0], 0, pSizeType, &CountValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValueArray [0], 1, pPtrPfnType, &PtrPfnValue);
	m_pModule->m_LlvmBuilder.CreateAlloca (pPtrPfnType, "ppfn", NULL, &PtrPfnVariable);
	m_pModule->m_LlvmBuilder.CreateStore (PtrPfnValue, PtrPfnVariable);
	m_pModule->m_LlvmBuilder.CreateGep (PtrPfnValue, CountValue, pPtrPfnType, &PtrPfnEndValue);

	CBasicBlock* pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("mccall_cond"));
	CBasicBlock* pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("mccall_loop"));
	CBasicBlock* pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("mccall_follow"));

	m_pModule->m_ControlFlowMgr.Follow (pConditionBlock);

	CValue IdxValue;
	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateLoad (PtrPfnVariable, NULL, &PtrPfnValue);
	m_pModule->m_LlvmBuilder.CreateGe_u (PtrPfnValue, PtrPfnEndValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFollowBlock, pBodyBlock, pBodyBlock);

	CValue PfnValue;
	m_pModule->m_LlvmBuilder.CreateLoad (PtrPfnValue, m_pTargetType, &PfnValue);
	m_pModule->m_OperatorMgr.CallOperator (PfnValue, &ArgValueList);

	m_pModule->m_LlvmBuilder.CreateGep (PtrPfnValue, 1, pPtrPfnType, &PtrPfnValue);
	m_pModule->m_LlvmBuilder.CreateStore (PtrPfnValue, PtrPfnVariable);
	m_pModule->m_ControlFlowMgr.Jump (pConditionBlock, pFollowBlock);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
