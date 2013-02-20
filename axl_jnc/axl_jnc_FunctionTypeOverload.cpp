#include "stdafx.h"
#include "axl_jnc_FunctionTypeOverload.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

size_t
CFunctionTypeOverload::FindOverload (CFunctionType* pType) const
{
	if (!m_pType)
		return -1;

	if (pType->Cmp (m_pType) == 0)
		return 0;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pOverloadType = m_OverloadArray [i];
		if (pType->Cmp (pOverloadType) == 0)
			return i + 1;
	}

	return -1;
}

size_t
CFunctionTypeOverload::FindShortOverload (CFunctionType* pType) const
{
	if (!m_pType)
		return -1;

	if (pType->Cmp (m_pType->GetShortType ()) == 0)
		return 0;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pOverloadType = m_OverloadArray [i];
		if (pType->Cmp (pOverloadType->GetShortType ()) == 0)
			return i + 1;
	}

	return -1;
}

size_t
CFunctionTypeOverload::ChooseOverload (
	CType* const* ppArgTypeArray,
	size_t ArgCount,
	ECast* pCastKind
	) const
{
	ASSERT (m_pType);

	CModule* pModule = m_pType->GetModule ();

	ECast BestCastKind = pModule->m_OperatorMgr.GetArgCastKind (m_pType, ppArgTypeArray, ArgCount);
	size_t BestOverload = BestCastKind ? 0 : -1;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pOverloadType = m_OverloadArray [i];
		ECast CastKind = pModule->m_OperatorMgr.GetArgCastKind (pOverloadType, ppArgTypeArray, ArgCount);
		if (!CastKind)
			continue;

		if (CastKind == BestCastKind)
		{
			err::SetFormatStringError (_T("ambiguous call to overloaded function"));
			return -1;
		}

		if (CastKind > BestCastKind)
		{
			BestOverload = i + 1;
			BestCastKind = CastKind;
		}
	}

	if (BestOverload == -1)
	{
		err::SetFormatStringError (_T("none of the %d overloads accept the specified argument list"), Count + 1);
		return -1;
	}

	if (pCastKind)
		*pCastKind = BestCastKind;

	return BestOverload; 
}

size_t
CFunctionTypeOverload::ChooseOverload (
	const rtl::CConstBoxListT <CValue>& ArgList,
	ECast* pCastKind
	) const
{
	ASSERT (m_pType);

	CModule* pModule = m_pType->GetModule ();

	ECast BestCastKind = pModule->m_OperatorMgr.GetArgCastKind (m_pType, ArgList);
	size_t BestOverload = BestCastKind ? 0 : -1;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pOverloadType = m_OverloadArray [i];
		ECast CastKind = pModule->m_OperatorMgr.GetArgCastKind (pOverloadType, ArgList);
		if (!CastKind)
			continue;

		if (CastKind == BestCastKind)
		{
			err::SetFormatStringError (_T("ambiguous call to overloaded function"));
			return -1;
		}

		if (CastKind > BestCastKind)
		{
			BestOverload = i + 1;
			BestCastKind = CastKind;
		}
	}

	if (BestOverload == -1)
	{
		err::SetFormatStringError (_T("none of the %d overloads accept the specified argument list"), Count + 1);
		return -1;
	}

	if (pCastKind)
		*pCastKind = BestCastKind;

	return BestOverload; 
}

size_t
CFunctionTypeOverload::ChooseSetterOverload (
	CType* pType,
	ECast* pCastKind
	) const
{
	ASSERT (m_pType);

	CModule* pModule = m_pType->GetModule ();

	rtl::CArrayT <CType*> ArgTypeArray = m_pType->GetArgTypeArray ();	
	ECast BestCastKind = pModule->m_OperatorMgr.GetCastKind (pType, ArgTypeArray.GetBack ());
	size_t BestOverload = BestCastKind ? 0 : -1;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pOverloadType = m_OverloadArray [i];

		ArgTypeArray = pOverloadType->GetArgTypeArray ();	
		ECast CastKind = pModule->m_OperatorMgr.GetCastKind (pType, ArgTypeArray.GetBack ());
		if (!CastKind)
			continue;

		if (CastKind == BestCastKind)
		{
			err::SetFormatStringError (_T("ambiguous call to overloaded function"));
			return -1;
		}

		if (CastKind > BestCastKind)
		{
			BestOverload = i + 1;
			BestCastKind = CastKind;
		}
	}

	if (BestOverload == -1)
	{
		err::SetFormatStringError (_T("none of the %d overloads accept the specified argument list"), Count + 1);
		return -1;
	}

	if (pCastKind)
		*pCastKind = BestCastKind;

	return BestOverload; 
}

size_t
CFunctionTypeOverload::ChooseSetterOverload (
	const CValue& Value,
	ECast* pCastKind
	) const
{
	ASSERT (m_pType);

	CModule* pModule = m_pType->GetModule ();

	rtl::CArrayT <CType*> ArgTypeArray = m_pType->GetArgTypeArray ();
	ECast BestCastKind = pModule->m_OperatorMgr.GetCastKind (Value, ArgTypeArray.GetBack ());
	size_t BestOverload = BestCastKind ? 0 : -1;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pOverloadType = m_OverloadArray [i];
		
		ArgTypeArray = m_pType->GetArgTypeArray ();	
		ECast CastKind = pModule->m_OperatorMgr.GetCastKind (Value, ArgTypeArray.GetBack ());
		if (!CastKind)
			continue;

		if (CastKind == BestCastKind)
		{
			err::SetFormatStringError (_T("ambiguous call to overloaded function"));
			return -1;
		}

		if (CastKind > BestCastKind)
		{
			BestOverload = i + 1;
			BestCastKind = CastKind;
		}
	}

	if (BestOverload == -1)
	{
		err::SetFormatStringError (_T("none of the %d overloads accept the specified argument list"), Count + 1);
		return -1;
	}

	if (pCastKind)
		*pCastKind = BestCastKind;

	return BestOverload; 
}

bool
CFunctionTypeOverload::AddOverload (CFunctionType* pType)
{
	if (!m_pType)
	{
		m_pType = pType;
		return true;
	}

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pOverloadType = m_OverloadArray [i];
		
		if (pType->GetArgSignature ().Cmp (pOverloadType->GetArgSignature ()) == 0)
		{
			err::SetFormatStringError (_T("illegal function overload: duplicate argument signature"));
			return false;
		}
	}

	m_OverloadArray.Append (pType);
	return true;
}

void
CFunctionTypeOverload::Copy (
	CFunctionType* const* ppTypeArray,
	size_t Count
	)
{
	ASSERT (Count);
	m_pType = ppTypeArray [0];
	m_OverloadArray.Copy (ppTypeArray + 1, Count - 1);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
