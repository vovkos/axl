#include "stdafx.h"
#include "axl_jnc_DerivableType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CBaseType::CBaseType ()
{
	m_pType = NULL;
	m_Offset = 0;
	m_LlvmIndex = -1;
	m_VTableIndex = -1;
	m_Flags = 0;
}

//.............................................................................

void
CBaseTypeCoord::Init ()
{
	m_pType = NULL;
	m_Offset = 0;
	m_VTableIndex = 0;
	m_LlvmIndexArray.SetBuffer (ref::EBuf_Field, m_Buffer, sizeof (m_Buffer));
}

//.............................................................................

CDerivableType::CDerivableType ()
{
	m_pPreConstructor = NULL;
	m_pStaticConstructor = NULL;
}

CBaseType*
CDerivableType::AddBaseType (CDerivableType* pType)
{
	rtl::CStringHashTableMapIteratorAT <CBaseType*> It = m_BaseTypeMap.Goto (pType->GetSignature ());
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return NULL;
	}

	CBaseType* pBaseType = AXL_MEM_NEW (CBaseType);
	pBaseType->m_pType = pType;
	m_BaseTypeList.InsertTail (pBaseType);
	It->m_Value = pBaseType;
	return pBaseType;
}

bool
CDerivableType::AddMethod (CFunction* pFunction)
{
	EStorage StorageKind = pFunction->GetStorageKind ();
	EFunction FunctionKind = pFunction->GetFunctionKind ();
	int FunctionKindFlags = GetFunctionKindFlags (FunctionKind);
	int ThisArgTypeFlags = pFunction->m_ThisArgTypeFlags;

	pFunction->m_pParentNamespace = this;

	switch (StorageKind)
	{
	case EStorage_Static:
		if (ThisArgTypeFlags)
		{
			err::SetFormatStringError (_T("static method cannot be '%s'"), GetPtrTypeFlagString (ThisArgTypeFlags));
			return false;
		}

		break;

	case EStorage_Undefined:
		pFunction->m_StorageKind = EStorage_Member;
		// and fall through

	case EStorage_Member:
		pFunction->ConvertToMemberMethod (this);
		break;

	default:
		err::SetFormatStringError (_T("invalid storage specifier '%s' for method member"), GetStorageKindString (StorageKind));
		return false;
	}

	CFunction** ppTarget = NULL;

	switch (FunctionKind)
	{
	case EFunction_PreConstructor:
		ppTarget = &m_pPreConstructor;
		break;

	case EFunction_StaticConstructor:
		ppTarget = &m_pStaticConstructor;
		break;

	case EFunction_Named:
		return AddFunction (pFunction);

	case EFunction_UnaryOperator:
		pFunction->m_Tag.Format (_T("%s.operator %s"), m_Tag, GetUnOpKindString (pFunction->GetUnOpKind ()));

		if (m_UnaryOperatorTable.IsEmpty ())
			m_UnaryOperatorTable.SetCount (EUnOp__Count);

		ppTarget = &m_UnaryOperatorTable [pFunction->GetUnOpKind ()];
		break;

	case EFunction_BinaryOperator:
		pFunction->m_Tag.Format (_T("%s.operator %s"), m_Tag, GetBinOpKindString (pFunction->GetBinOpKind ()));

		if (m_BinaryOperatorTable.IsEmpty ())
			m_BinaryOperatorTable.SetCount (EBinOp__Count);

		ppTarget = &m_BinaryOperatorTable [pFunction->GetBinOpKind ()];
		break;

	default:
		err::SetFormatStringError (_T("invalid %s in '%s'"), GetFunctionKindString (FunctionKind), GetTypeString ());
		return false;
	}

	pFunction->m_Tag.Format (_T("%s.%s"), m_Tag, GetFunctionKindString (FunctionKind));

	if (!*ppTarget)
	{
		*ppTarget = pFunction;
	}
	else if (FunctionKindFlags & EFunctionKindFlag_NoOverloads)
	{
		err::SetFormatStringError (_T("'%s' already has '%s' method"), GetTypeString (), GetFunctionKindString (FunctionKind));
		return false;
	}
	else
	{
		bool Result = (*ppTarget)->AddOverload (pFunction);
		if (!Result)
			return false;
	}

	return true;
}

bool
CDerivableType::AddProperty (CProperty* pProperty)
{
	ASSERT (pProperty->IsNamed ());
	bool Result = AddItem (pProperty);
	if (!Result)
		return false;

	pProperty->m_pParentNamespace = this;

	EStorage StorageKind = pProperty->GetStorageKind ();
	switch (StorageKind)
	{
	case EStorage_Static:
		break;

	case EStorage_Undefined:
		pProperty->m_StorageKind = EStorage_Member;
		//and fall through

	case EStorage_Member:
		pProperty->m_pParentType = this;
		break;

	default:
		err::SetFormatStringError (_T("invalid storage specifier '%s' for method member"), GetStorageKindString (StorageKind));
		return false;
	}

	return true;
}

bool
CDerivableType::CreateDefaultPreConstructor ()
{
	CFunctionType* pType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (EFunction_PreConstructor, pType);
	pFunction->m_StorageKind = EStorage_Member;
	
	bool Result = AddMethod (pFunction);
	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.m_DefaultPreConstructorTypeArray.Append (this);
	return true;
}

void
CDerivableType::ClearAllBaseTypeConstructedFlags ()
{
	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
		BaseType->m_Flags &= ~EBaseTypeFlag_Constructed;
}

bool
CDerivableType::FindBaseTypeTraverseImpl (
	CType* pType,
	CBaseTypeCoord* pCoord,
	size_t Level
	)
{
	rtl::CStringHashTableMapIteratorAT <CBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
	if (It)
	{
		if (!pCoord)
			return true;

		CBaseType* pBaseType = It->m_Value;
		pCoord->m_pType = pBaseType->m_pType;
		pCoord->m_Offset = pBaseType->m_Offset;
		pCoord->m_LlvmIndexArray.SetCount (Level + 1);
		pCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
		return true;
	}

	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CBaseType* pBaseType = *BaseType;

		bool Result = pBaseType->m_pType->FindBaseTypeTraverseImpl (pType, pCoord, Level + 1);
		if (Result)
		{
			if (pCoord)
			{
				pCoord->m_Offset += pBaseType->m_Offset;
				pCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
			}

			return true;
		}
	}

	return false;
}

CModuleItem*
CDerivableType::FindItemTraverseImpl (
	const tchar_t* pName,
	CBaseTypeCoord* pCoord,
	int Flags,
	size_t Level
	)
{
	CModuleItem* pItem;

	if (!(Flags & ETraverse_NoThis))
	{
		pItem = FindItem (pName);
		if (pItem)
		{
			if (pCoord)
			{
				pCoord->m_pType = this;
				pCoord->m_LlvmIndexArray.SetCount (Level);
			}

			return pItem;
		}
	}

	if (!(Flags & ETraverse_NoExtensionNamespace) && m_pExtensionNamespace)
	{
		pItem = m_pExtensionNamespace->FindItem (pName);
		if (pItem)
		{
			if (pCoord)
			{
				pCoord->m_pType = this;
				pCoord->m_LlvmIndexArray.SetCount (Level);
			}

			return pItem;
		}
	}

	Flags &= ~ETraverse_NoThis;

	if (!(Flags & ETraverse_NoBaseType))
	{
		rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
		for (; BaseType; BaseType++)
		{
			CBaseType* pBaseType = *BaseType;

			pItem = pBaseType->m_pType->FindItemTraverseImpl (pName, pCoord, Flags, Level + 1);
			if (pItem)
			{
				if (pCoord)
				{
					pCoord->m_Offset += pBaseType->m_Offset;
					pCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
					pCoord->m_VTableIndex += pBaseType->m_VTableIndex;
				}

				return pItem;
			}
		}
	}

	if (!(Flags & ETraverse_NoParentNamespace) && m_pParentNamespace)
	{
		pItem = m_pParentNamespace->FindItemTraverse (pName, NULL, Flags);
		if (pItem)
		{
			if (pCoord)
				pCoord->m_pType = NULL; // not this type

			return pItem;
		}
	}

	return NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
