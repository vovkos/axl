#include "pch.h"
#include "axl_spy_ThreadState.h"

namespace axl {
namespace spy {

//..............................................................................

static volatile int32_t g_enableCount;
static size_t g_threadDisableCountSlot = -1;
static size_t g_threadStateSlot = -1;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HookFinalizer:
	public rc::RefCount,
	public g::Finalizer
{
public:
	virtual
	void
	finalize()
	{
		g_enableCount = INT_MIN / 2; // compensate for possible unbalanced enable calls
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
initializeHooks(int)
{
	g_threadDisableCountSlot = sys::createSimpleTlsSlot();
	g_threadStateSlot = sys::getTlsMgr()->createSlot();
	g::getModule()->addFinalizer(AXL_RC_NEW(HookFinalizer));
}

size_t
getCurrentThreadDisableCount()
{
	return sys::getSimpleTlsValue(g_threadDisableCountSlot);
}

bool
areHooksEnabled()
{
	return
		sys::atomicLoad(&g_enableCount) > 0 &&
		getCurrentThreadDisableCount() == 0;
}

void
enableHooks()
{
	sl::callOnce(initializeHooks, 0);
	sys::atomicInc(&g_enableCount);
}

void
disableHooks()
{
	sys::atomicDec(&g_enableCount);
}

inline
void
incrementCurrentThreadDisableCount(intptr_t delta)
{
	size_t count = sys::getSimpleTlsValue(g_threadDisableCountSlot);
	sys::setSimpleTlsValue(g_threadDisableCountSlot, count + delta);
}

void
disableCurrentThreadHooks()
{
	incrementCurrentThreadDisableCount(1);
}

void
enableCurrentThreadHooks()
{
	incrementCurrentThreadDisableCount(-1);
}

ThreadState*
getCurrentThreadState(bool createIfNotExists)
{
	ASSERT(getCurrentThreadDisableCount() && "thread hooks are not disabled (should be)");

	sys::TlsMgr* tlsMgr = sys::getTlsMgr();
	sys::TlsValue currentState = tlsMgr->getSlotValue(g_threadStateSlot);
	if (currentState || !createIfNotExists)
		return (ThreadState*)currentState.p();

	rc::Ptr<ThreadState> newState = AXL_RC_NEW(ThreadState);
	tlsMgr->setSlotValue(g_threadStateSlot, newState);
	return newState;
}

//..............................................................................

ThreadState::~ThreadState()
{
	cleanup(sl::RbTreeIterator<size_t, Frame>());
	sys::setSimpleTlsValue(g_threadDisableCountSlot, INT_MAX); // compensate for possibly unbalanced enable calls
}

void
ThreadState::addFrame(
	HookCommonContext* context,
	size_t frameBase,
	size_t originalRet
	)
{
	sl::RbTreeIterator<size_t, Frame> it = m_frameMap.visit(frameBase);
	if (it->m_value.m_ret.m_originalRet)
		it->m_value.m_chainedRetStack.append(it->m_value.m_ret);

	it->m_value.m_ret.m_context = context;
	it->m_value.m_ret.m_originalRet = originalRet;
	cleanup(it);
}

size_t
ThreadState::removeFrame(size_t frameBase)
{
	sl::RbTreeIterator<size_t, Frame> it = findFrame(frameBase);
	if (!it)
	{
		ASSERT(false && "protolesshooks: FATAL ERROR: return address not found");
		return 0;
	}

	size_t originalRet = it->m_value.m_ret.m_originalRet;
	cleanup(it);

	if (!it->m_value.m_chainedRetStack.isEmpty())
		it->m_value.m_ret = it->m_value.m_chainedRetStack.getBackAndPop();
	else
		m_frameMap.erase(it);

	return originalRet;
}

size_t
ThreadState::getOriginalRet(size_t frameBase)
{
	sl::RbTreeIterator<size_t, Frame> it = findFrame(frameBase);
	if (!it)
	{
		ASSERT(false && "protolesshooks: FATAL ERROR: return address not found");
		return 0;
	}

	return it->m_value.m_ret.m_originalRet;
}

inline
void
callHookLeaveFunc(HookCommonContext* context)
{
	if (context->m_leaveFunc)
		context->m_leaveFunc(context->m_targetFunc, context->m_callbackParam, 0);
}

void
ThreadState::cleanup(const sl::RbTreeIterator<size_t, Frame>& it)
{
	// we may end up with abandoned frames (e.g., due to SEH or longjmp-s)
	// this loop cleans up all frames *above* `it` (or all if `it` is NULL)

	while (m_frameMap.getHead() != it)
	{
		Frame* frame = &m_frameMap.getHead()->m_value;

		size_t chainedRetCount = frame->m_chainedRetStack.getCount();
		for (intptr_t i = chainedRetCount - 1; i >= 0; i--)
			callHookLeaveFunc(frame->m_chainedRetStack[i].m_context);

		callHookLeaveFunc(frame->m_ret.m_context);
		m_frameMap.erase(m_frameMap.getHead());
	}
}

//..............................................................................

} // namespace spy
} // namespace axl
