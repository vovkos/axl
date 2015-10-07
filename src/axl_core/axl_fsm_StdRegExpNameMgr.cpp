#include "pch.h"
#include "axl_fsm_StdRegExpNameMgr.h"

namespace axl {
namespace fsm {
	
//.............................................................................

void
StdRegExpNameMgr::addName (
	const sl::String& name,
	const sl::String& source
	)
{
	sl::StringHashTableMapIterator <Name*> it = m_nameMap.visit (name);
	if (it->m_value)
	{
		it->m_value->m_source = source;
		return;
	}

	Name* nameEntry = AXL_MEM_NEW (Name);
	nameEntry->m_name = name;
	nameEntry->m_source = source;

	m_nameList.insertTail (nameEntry);
	it->m_value = nameEntry;
}

void
StdRegExpNameMgr::removeName (const char* name)
{
	sl::StringHashTableMapIterator <Name*> it = m_nameMap.find (name);
	if (!it->m_value)
		return;
	
	Name* nameEntry = it->m_value;
	m_nameMap.erase (it);
	m_nameList.erase (nameEntry);
}

//.............................................................................

} // namespace fsm
} // namespace axl
