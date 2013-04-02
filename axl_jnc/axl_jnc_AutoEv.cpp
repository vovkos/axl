#include "stdafx.h"
#include "axl_jnc_AutoEv.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CAutoEv::CAutoEv ()
{
	m_ItemKind = EModuleItem_AutoEv;
	m_NamespaceKind = ENamespace_AutoEv;
	m_pItemDecl = this;
	m_pType = NULL;
	m_pStarter = NULL;
	m_pStopper = NULL;
	m_pParentClassType = NULL;

	m_pParentClassField = NULL;
	m_pDataStructType = NULL;
	m_pStaticDataVariable = NULL;

	m_BindSiteCount = 0;

	memset (m_FieldArray, 0, sizeof (m_FieldArray));
}

bool
CAutoEv::Create (CAutoEvType* pType)
{
	ASSERT (m_StorageKind);

	m_pType = pType;
	m_pStarter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_AutoEvStarter, pType->GetStarterType ());
	m_pStarter->m_StorageKind = m_StorageKind;
	m_pStarter->m_pParentNamespace = this;
	m_pStarter->m_pAutoEv = this;
	m_pStarter->m_Tag = m_Tag + _T(".Start");
	
	m_pStopper = m_pModule->m_FunctionMgr.CreateFunction (EFunction_AutoEvStopper, pType->GetStopperType ());
	m_pStopper->m_StorageKind = m_StorageKind;
	m_pStopper->m_pParentNamespace = this;
	m_pStopper->m_pAutoEv = this;
	m_pStopper->m_Tag = m_Tag + _T(".Stop");

	m_pDataStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pDataStructType->m_StorageKind = m_StorageKind;
	m_pDataStructType->m_pParentNamespace = this;
	m_pDataStructType->m_Tag = m_Tag + _T(".Data");

	m_FieldArray [EAutoEvField_Lock] = m_pDataStructType->CreateField (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));
	m_FieldArray [EAutoEvField_State] = m_pDataStructType->CreateField (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));

	if (m_pParentClassType)
	{
		m_pParentClassField = m_pParentClassType->GetIfaceStructType ()->CreateField (m_pDataStructType);
		m_pType = m_pType->GetMemberAutoEvType (m_pParentClassType);
		m_pStarter->ConvertToMemberMethod (m_pParentClassType);
		m_pStopper->ConvertToMemberMethod (m_pParentClassType);
	}

	rtl::CArrayT <CFunctionArg*> ArgArray = m_pStarter->GetType ()->GetArgArray ();
	if (ArgArray.IsEmpty ())
		return true;

	size_t ArgCount = ArgArray.GetCount ();
	for (size_t i = 0; i < ArgCount; i++)
	{
		CFunctionArg* pArg = ArgArray [i];
		CStructField* pField = m_pDataStructType->CreateField (pArg->GetName (), pArg->GetType ());
		if (!pField)
			return false;

		AddItem (pField);
	}

	return true;
}

CFunction* 
CAutoEv::CreateHandler ()
{
	CFunctionType* pHandlerType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunction* pHandler = m_pModule->m_FunctionMgr.CreateFunction (EFunction_AutoEvHandler, pHandlerType);
	pHandler->m_StorageKind = m_StorageKind;
	pHandler->m_pParentNamespace = this;
	pHandler->m_pAutoEv = this;
	pHandler->m_Tag = m_Tag + _T(".Handler");

	if (m_pParentClassType)
		pHandler->ConvertToMemberMethod (m_pParentClassType);

	return pHandler;
}

bool
CAutoEv::CreateStopper ()
{
	bool Result;

	CStructType* pBindSiteType = (CStructType*) m_pModule->m_TypeMgr.GetStdType (EStdType_AutoEvBindSite);
	CStructField* pEventPtrField = *pBindSiteType->GetFieldList ().GetHead ();
	CStructField* pCookieField = *pBindSiteType->GetFieldList ().GetTail ();

	m_pModule->m_FunctionMgr.InternalPrologue (m_pStopper);

	CBasicBlock* pUnadviseBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("unadvise_block"));
	CBasicBlock* pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("follow_block"));

	CValue StateValue; 
	CValue StateCmpValue; 
	CValue BindSiteArrayValue;

	Result = 
		m_pModule->m_OperatorMgr.GetAutoEvField (this, EAutoEvField_State, &StateValue) &&	
		m_pModule->m_ControlFlowMgr.ConditionalJump (StateValue, pUnadviseBlock, pFollowBlock);

	if (!Result)
		return false;

	Result = m_pModule->m_OperatorMgr.GetAutoEvField (this, EAutoEvField_BindSiteArray, &BindSiteArrayValue);
	if (!Result)
		return false;

	for (size_t i = 0; i < m_BindSiteCount; i++)
	{
		CValue IdxValue (i, EType_SizeT);
		CValue BindSiteValue;
		CValue OnChangeValue;
		CValue CookieValue;

		Result = 
			m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, BindSiteArrayValue, IdxValue, &BindSiteValue) &&
			m_pModule->m_OperatorMgr.GetStructField (BindSiteValue, pEventPtrField, NULL, &OnChangeValue) &&
			m_pModule->m_OperatorMgr.GetStructField (BindSiteValue, pCookieField, NULL, &CookieValue) &&
			m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, &OnChangeValue) &&
			m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_SubAssign, OnChangeValue, CookieValue);

		if (!Result)
			return false;
	}

	Result = m_pModule->m_OperatorMgr.StoreDataRef (StateValue, CValue ((int64_t) 0, EType_Int_p));
	ASSERT (Result);
	
	m_pModule->m_ControlFlowMgr.Follow (pFollowBlock);

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool
CAutoEv::CalcLayout ()
{
	ASSERT (m_BindSiteCount);

	if (!m_VTable.IsEmpty ()) // done already
		return true;

	CType* pBindSiteType = m_pModule->m_TypeMgr.GetStdType (EStdType_AutoEvBindSite);
	CArrayType* pBindSiteArrayType = pBindSiteType->GetArrayType (m_BindSiteCount);
	m_FieldArray [EAutoEvField_BindSiteArray] = m_pDataStructType->CreateField (pBindSiteArrayType);
	m_pDataStructType->CalcLayout ();

	ASSERT (IsMember () || m_StorageKind == EStorage_Static);
	if (m_StorageKind == EStorage_Static)
	{
		m_pStaticDataVariable = m_pModule->m_VariableMgr.CreateVariable (
			EStorage_Static, 
			_T("static_data"),
			m_Tag + _T(".static_data"), 
			m_pDataStructType
			);
	}
	
	m_VTable.SetCount (2);
	m_VTable [0] = m_pStarter;
	m_VTable [1] = m_pStopper;

	CreateVTablePtr ();
	return true;
}

void
CAutoEv::CreateVTablePtr ()
{
	char Buffer [256];
	rtl::CArrayT <llvm::Constant*> LlvmVTable (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	size_t Count = m_VTable.GetCount ();
	LlvmVTable.SetCount (Count);

	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_VTable [i];

		if (pFunction->GetStorageKind () == EStorage_Abstract)
			pFunction = pFunction->GetType ()->GetAbstractFunction ();

		LlvmVTable [i] = pFunction->GetLlvmFunction ();
	}
	
	CStructType* pVTableStructType = m_pType->GetVTableStructType ();

	llvm::Constant* pLlvmVTableConstant = llvm::ConstantStruct::get (
		(llvm::StructType*) pVTableStructType->GetLlvmType (),
		llvm::ArrayRef <llvm::Constant*> (LlvmVTable, Count)
		);

	rtl::CString VariableTag;
	VariableTag.Format (_T("%s.vtbl"), GetQualifiedName ());
	llvm::GlobalVariable* pLlvmVTableVariable = new llvm::GlobalVariable (
			*m_pModule->m_pLlvmModule,
			pVTableStructType->GetLlvmType (),
			false,
			llvm::GlobalVariable::ExternalLinkage,
			pLlvmVTableConstant,
			(const tchar_t*) VariableTag
			);

	m_VTablePtrValue.SetLlvmValue (
		pLlvmVTableVariable, 
		pVTableStructType->GetDataPtrType (EDataPtrType_Unsafe),
		EValue_Const
		);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
