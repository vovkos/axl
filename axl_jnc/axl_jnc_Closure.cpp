#include "stdafx.h"
#include "axl_jnc_Closure.h"

namespace axl {
namespace jnc {

//.............................................................................
	
CClosureArg*
CClosure::CreateArg (
	size_t ArgIdx,
	const CValue&  Value,
	int Flags
	)
{
	if (!m_ArgList.IsEmpty ())
	{
		rtl::CIteratorT <CClosureArg> TailArg = m_ArgList.GetTail ();
		if (TailArg->m_ArgIdx >= ArgIdx)
		{
			err::SetFormatStringError (_T("closure argument #%d is out-of-sequence"), ArgIdx);
			return false;
		}
	}

	CClosureArg* pArg = AXL_MEM_NEW (CClosureArg);
	pArg->m_ArgIdx = ArgIdx;
	pArg->m_Value = Value;
	pArg->m_Flags = Flags;
	m_ArgList.InsertTail (pArg);
	return pArg;
}

bool
CClosure::Apply (rtl::CBoxListT <CValue>* pArgList)
{
	size_t TargetArgIdx = 0;
	rtl::CBoxIteratorT <CValue> TargetArg = pArgList->GetHead ();
	rtl::CIteratorT <CClosureArg> Arg = m_ArgList.GetHead ();
	for (; Arg; Arg++)
	{
		CClosureArg* pArg = *Arg;

		while (TargetArgIdx < pArg->m_ArgIdx)
		{
			if (!TargetArg)
			{
				err::SetFormatStringError (_T("closure call misses argument #%d"), TargetArgIdx + 1);
				return false;
			}

			TargetArg++;
			TargetArgIdx++;
		}

		pArgList->InsertBefore (pArg->m_Value, TargetArg);
		TargetArgIdx++;
	}

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
