#include "pch.h"
#include "axl_spy_HookMgr.h"

namespace axl {
namespace spy {

//..............................................................................

void
HookMgr::addFrame(
	size_t frameBase,
	size_t originalRet
	)
{
	sl::RbTreeIterator<size_t, size_t> it = m_frameMap.add(frameBase, originalRet);
	cleanup(it);
}

size_t
HookMgr::removeFrame(size_t frameBase)
{
	sl::RbTreeIterator<size_t, size_t> it = m_frameMap.find(frameBase);
	if (!it)
	{
		ASSERT(false && "protolesshooks: FATAL ERROR: return address not found");
		return 0;
	}

	size_t originalRet = it->m_value;
	cleanup(it);
	m_frameMap.erase(it);
	return originalRet;
}

size_t
HookMgr::findOriginalRet(size_t frameBase)
{
	sl::RbTreeIterator<size_t, size_t> it = m_frameMap.find(frameBase);
	if (!it)
	{
		ASSERT(false && "protolesshooks: FATAL ERROR: return address not found");
		return 0;
	}

	return it->m_value;
}


void
HookMgr::cleanup(const sl::RbTreeIterator<size_t, size_t>& it)
{
	// we may end up with abandoned frames (e.g., due to SEH or longjmp-s)
	// this loop cleans up all frames *above* the current one

	for (;;)
	{
		sl::RbTreeIterator<size_t, size_t> prevIt = it.getPrev();
		if (!prevIt)
			break;

		m_frameMap.erase(prevIt);
	}
}

//..............................................................................

} // namespace spy
} // namespace axl
