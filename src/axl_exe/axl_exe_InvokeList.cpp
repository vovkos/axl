#include "pch.h"
#include "axl_exe_InvokeList.h"
#include "axl_exe_Function.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace exe {

//.............................................................................

bool 
InvokeList::addV (
	IFunction* function, 
	axl_va_list va
	)
{
	Entry* entry = AXL_MEM_NEW (Entry);
	entry->m_function = ref::getPtrOrClone (function);
	entry->m_stack = function->getArgPacker ()->createArgBlockV (va);
	m_list.insertTail (entry);
	return true;
}

size_t 
InvokeList::process (size_t count)
{
	size_t i;

	for (i = 0; i < count && !m_list.isEmpty (); i++)
	{
		Entry* entry = m_list.removeHead ();
		axl_va_list va = { (va_list) entry->m_stack->m_p };
		entry->m_function->invokeV (va);
		AXL_MEM_DELETE (entry);
	}

	return i;
}

//.............................................................................

} // namespace exe
} // namespace axl
