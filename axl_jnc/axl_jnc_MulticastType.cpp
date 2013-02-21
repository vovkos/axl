#include "stdafx.h"
#include "axl_jnc_MulticastType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CMulticastType::CMulticastType ()
{
	m_TypeKind = EType_Multicast;
	m_Size = sizeof (TMulticast);
	m_pTargetType = NULL;
	m_pMulticastStructType = NULL;
	m_pSetMethod_s = NULL;
	m_pSetMethod_u = NULL;
	m_pAddMethod_s = NULL;
	m_pAddMethod_u = NULL;
	m_pRemoveMethod_s = NULL;
	m_pRemoveMethod_u = NULL;
	m_pSnapshotMethod_s = NULL;
	m_pSnapshotMethod_u = NULL;
}

CStructType* 
CMulticastType::GetMulticastStructType ()
{
	if (m_pMulticastStructType)
		return m_pMulticastStructType;

	m_pMulticastStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pMulticastStructType->m_Tag.Format (_T("multicast"));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));
	m_pMulticastStructType->CreateFieldMember (m_pTargetType->GetDataPtrType (EDataPtrType_Unsafe));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));
	m_pMulticastStructType->CalcLayout ();

	return m_pMulticastStructType;
}

void
CMulticastType::PrepareTypeString ()
{
	m_TypeString += m_pTargetType->GetTypeModifierString ();
	m_TypeString += _T("multicast ");
	m_TypeString += m_pTargetType->GetTargetType ()->GetArgTypeString ();
}

CFunction* 
CMulticastType::GetSetMethod_s ()
{
	if (m_pSetMethod_s)
		return m_pSetMethod_s;

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSet_s"), pType);

	CValue ArgValueArray [2];

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pSetMethod_s = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetSetMethod_u ()
{
	if (m_pSetMethod_u)
		return m_pSetMethod_u;

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSet_u"), pType);

	CValue ArgValueArray [2];

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pSetMethod_u = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetAddMethod_s ()
{
	if (m_pAddMethod_s)
		return m_pAddMethod_s;

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastAdd_s"), pType);

	CValue ArgValueArray [2];

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pAddMethod_s = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetAddMethod_u ()
{
	if (m_pAddMethod_u)
		return m_pAddMethod_u;

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastAdd_u"), pType);

	CValue ArgValueArray [2];

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pAddMethod_u = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetRemoveMethod_s ()
{
	if (m_pRemoveMethod_s)
		return m_pRemoveMethod_s;

	CType* pReturnType = m_pTargetType;
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastRemove_s"), pType);

	CValue ArgValueArray [2];

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pRemoveMethod_s = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetRemoveMethod_u ()
{
	if (m_pRemoveMethod_u)
		return m_pRemoveMethod_u;

	CType* pReturnType = m_pTargetType;
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastRemove_u"), pType);

	CValue ArgValueArray [2];

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pRemoveMethod_u = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetSnapshotMethod_s ()
{
	if (m_pSnapshotMethod_s)
		return m_pSnapshotMethod_s;

	CType* pReturnType = m_pTargetType->GetMcSnapshotType ();
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSnapshot_s"), pType);

	CValue ArgValue;

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pSnapshotMethod_s = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetSnapshotMethod_u ()
{
	if (m_pSnapshotMethod_u)
		return m_pSnapshotMethod_u;

	CType* pReturnType = m_pTargetType->GetMcSnapshotType ();
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSnapshot_u"), pType);

	CValue ArgValue;

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pSnapshotMethod_u = pFunction;
	return pFunction;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
