#include "pch.h"
#include "axl_exe_InvokeList.h"
#include "axl_exe_Function.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace exe {

//.............................................................................

bool 
CInvokeList::AddV (
	IFunction* pFunction, 
	axl_va_list va
	)
{
	CEntry* pEntry = AXL_MEM_NEW (CEntry);
	pEntry->m_Function = ref::GetPtrOrClone (pFunction);
	pEntry->m_Stack = pFunction->GetArgPacker ()->CreateArgBlockV (va);
	m_List.InsertTail (pEntry);
	return true;
}

size_t 
CInvokeList::Process (size_t Count)
{
	size_t i;

	for (i = 0; i < Count && !m_List.IsEmpty (); i++)
	{
		CEntry* pEntry = m_List.RemoveHead ();
		axl_va_list va = { (va_list) pEntry->m_Stack->m_p };
		pEntry->m_Function->InvokeV (va);
		AXL_MEM_DELETE (pEntry);
	}

	return i;
}

//.............................................................................

} // namespace exe
} // namespace axl
