#include "pch.h"
#include "axl_spy_ThreadState.h"
#include "axl_sl_CallOnce.h"
#include "axl_g_Module.h"
#include "axl_ref_New.h"

namespace axl {
namespace spy {

thread_local ThreadState g_threadState;
thread_local bool g_threadStateDestructed = false;
volatile int32_t g_enableCount = 0;

//..............................................................................

void
ThreadState::addFrame(
	size_t frameBase,
	size_t originalRet
	)
{
	sl::RbTreeIterator<size_t, Frame> it = m_frameMap.visit(frameBase);
	if (it->m_value.m_originalRet)
		it->m_value.m_chainedRetStack.append(it->m_value.m_originalRet);

	it->m_value.m_originalRet = originalRet;
	cleanup(it);
}

size_t
ThreadState::removeFrame(size_t frameBase)
{
#if (_AXL_CPU_X86) // allowance for stdcall ret <n>
	sl::RbTreeIterator<size_t, Frame> it = m_frameMap.find(frameBase, sl::BinTreeFindRelOp_Le);
#else
	sl::RbTreeIterator<size_t, Frame> it = m_frameMap.find(frameBase);
#endif
	if (!it)
	{
		ASSERT(false && "protolesshooks: FATAL ERROR: return address not found");
		return 0;
	}

	size_t originalRet = it->m_value.m_originalRet;
	cleanup(it);

	if (!it->m_value.m_chainedRetStack.isEmpty())
		it->m_value.m_originalRet = it->m_value.m_chainedRetStack.getBackAndPop();
	else
		m_frameMap.erase(it);

	return originalRet;
}

size_t
ThreadState::getOriginalRet(size_t frameBase)
{
#if (_AXL_CPU_X86) // allowance for stdcall ret <n>
	sl::RbTreeIterator<size_t, Frame> it = m_frameMap.find(frameBase, sl::BinTreeFindRelOp_Le);
#else
	sl::RbTreeIterator<size_t, Frame> it = m_frameMap.find(frameBase);
#endif
	if (!it)
	{
		ASSERT(false && "protolesshooks: FATAL ERROR: return address not found");
		return 0;
	}

	return it->m_value.m_originalRet;
}

void
ThreadState::cleanup(const sl::RbTreeIterator<size_t, Frame>& it)
{
	// we may end up with abandoned frames (e.g., due to SEH or longjmp-s)
	// this loop cleans up all frames *above* the current one

	for (;;)
	{
		sl::RbTreeIterator<size_t, Frame> prevIt = it.getPrev();
		if (!prevIt)
			break;

		m_frameMap.erase(prevIt);
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HookFinalizer:
	public ref::RefCount,
	public g::Finalizer
{
public:
	virtual
	void
	finalize()
	{
		g_enableCount = INT_MIN / 2; // compensate for possible unbalnaced enable calls
	}
};

void
addHookFinalizer(int)
{
	g::getModule()->addFinalizer(AXL_REF_NEW(HookFinalizer));
}

void
enableHooks()
{
	sl::callOnce(addHookFinalizer, 0);
	sys::atomicInc(&g_enableCount);
}

void
disableHooks()
{
	sys::atomicDec(&g_enableCount);
}

void
disableCurrentThreadHooks()
{
	g_threadState.disable();
}

void
enableCurrentThreadHooks()
{
	g_threadState.enable();
}

//..............................................................................

} // namespace spy
} // namespace axl
