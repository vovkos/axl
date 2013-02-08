#include "stdafx.h"
#include "axl_jnc_Closure.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t* 
GetClosureKindString (EClosure ClosureKind)
{
	switch (ClosureKind)
	{
	case EClosure_Function:
		return _T("function");

	case EClosure_Property:
		return _T("property");

	default:
		return _T("undefined-closure");
	}
}

//.............................................................................

size_t
CClosure::Append (const rtl::CConstBoxListT <CValue>& ArgList)
{
	ASSERT (!ArgList.IsEmpty ());

	rtl::CBoxIteratorT <CValue> InternalArg = m_ArgList.GetHead ();
	rtl::CBoxIteratorT <CValue> ExternalArg = ArgList.GetHead ();
	
	for (;;)
	{
		while (InternalArg && !InternalArg->IsEmpty ())
			InternalArg++;

		if (!InternalArg)
			break;

		*InternalArg = *ExternalArg;
		InternalArg++;
		ExternalArg++;

		if (!ExternalArg)
			return m_ArgList.GetCount ();
	}

	for (; ExternalArg; ExternalArg++)
		m_ArgList.InsertTail (*ExternalArg);

	return m_ArgList.GetCount ();
}

bool
CClosure::Apply (rtl::CBoxListT <CValue>* pArgList)
{
	rtl::CBoxIteratorT <CValue> ClosureArg = m_ArgList.GetHead ();
	rtl::CBoxIteratorT <CValue> TargetArg = pArgList->GetHead ();
	
	for (size_t i = 0; ClosureArg; ClosureArg++, i++)
	{
		if (!ClosureArg->IsEmpty ())
		{
			pArgList->InsertBefore (*ClosureArg, TargetArg);
		}
		else if (TargetArg)
		{
			TargetArg++;
		}
		else
		{
			err::SetFormatStringError (_T("closure call misses argument #%d"), i + 1);
			return false;
		}
	}

	return true;
}

CType*
CClosure::GetClosureType (CType* pType)
{
	EType TypeKind = pType->GetTypeKind ();

	if (m_ClosureKind == EClosure_Function)
	{
		if (TypeKind == EType_FunctionPtr || TypeKind == EType_FunctionRef)
			return GetFunctionClosureType ((CFunctionPtrType*) pType);
	}
	else if (m_ClosureKind == EClosure_Property)
	{
		if (TypeKind == EType_PropertyPtr || TypeKind == EType_PropertyRef)
			return GetPropertyClosureType ((CPropertyPtrType*) pType);
	}

	err::SetFormatStringError (
		_T("cannot apply %s closure to '%s'"), 
		GetClosureKindString (m_ClosureKind),
		pType->GetTypeString ()
		);

	return NULL;
}

CFunctionPtrType*
CClosure::GetFunctionClosureType (CFunction* pFunction)
{
	ASSERT (m_ClosureKind == EClosure_Function);

	err::SetFormatStringError (_T("function overload closures are not implemented yet"));
	return NULL;
}

CFunctionPtrType*
CClosure::GetFunctionClosureType (CFunctionPtrType* pPtrType)
{
	ASSERT (m_ClosureKind == EClosure_Function);

	bool Result;

	CModule* pModule = pPtrType->GetModule ();
	CFunctionType* pType = pPtrType->GetTargetType ();

	if (pType->GetFlags () & EFunctionTypeFlag_VarArg)
	{
		err::SetFormatStringError (_T("function closures cannot be applied to vararg functions"));
		return NULL;
	}

	rtl::CArrayT <CType*> ArgTypeArray = pType->GetArgTypeArray ();
	size_t ClosureArgCount = m_ArgList.GetCount ();
	size_t ArgCount = ArgTypeArray.GetCount ();

	if (ClosureArgCount > ArgCount)
	{
		err::SetFormatStringError (_T("closure with %d arguments for %s"), ClosureArgCount, pType->GetTypeString ());
		return NULL;
	}

	rtl::CBoxIteratorT <CValue> ClosureArg = m_ArgList.GetHead ();
	for (size_t i = 0; ClosureArg; ClosureArg++)
	{
		if (ClosureArg->IsEmpty ())
		{
			i++;
			continue;
		}

		ASSERT (i < ArgCount);

		Result = pModule->m_OperatorMgr.CheckCastKind (ClosureArg->GetType (), ArgTypeArray [i]);
		if (!Result)
			return NULL;

		ArgTypeArray.Remove (i);
		ArgCount--;
	}

	pType = pModule->m_TypeMgr.GetFunctionType (
		pType->GetCallConv (),
		pType->GetReturnType (), 
		ArgTypeArray
		);
	
	return pType->GetFunctionPtrType (
		pPtrType->GetTypeKind (),
		pPtrType->GetPtrTypeKind (), 
		pPtrType->GetFlags ()
		);
}

CPropertyPtrType*
CClosure::GetPropertyClosureType (CPropertyPtrType* pPtrType)
{
	ASSERT (m_ClosureKind == EClosure_Property);

	err::SetFormatStringError (_T("property closures are not implemented yet"));
	return NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
