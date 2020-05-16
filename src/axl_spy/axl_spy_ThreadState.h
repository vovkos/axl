#pragma once

#include "axl_spy_HookCommon.h"
#include "axl_sl_RbTree.h"
#include "axl_sl_Array.h"
#include "axl_sys_Atomic.h"

namespace axl {
namespace spy {

//..............................................................................

extern thread_local bool g_isThreadStateDestructed;
extern volatile int32_t g_enableCount;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ThreadState
{
protected:
	struct Ret
	{
		HookCommonContext* m_context;
		size_t m_originalRet;

		Ret()
		{
			m_context = NULL;
			m_originalRet = 0;
		}
	};

	struct Frame
	{
		Ret m_ret;
		sl::Array<Ret> m_chainedRetStack; // forwarding via jmp
	};

protected:
	sl::RbTree<size_t, Frame> m_frameMap;
	size_t m_disableCount;

public:
	ThreadState()
	{
		m_disableCount = 0;
	}

	~ThreadState();

	bool
	isEnabled()
	{
		return sys::atomicLoad(&g_enableCount) > 0 && !g_isThreadStateDestructed && !m_disableCount;
	}

	bool
	disable()
	{
		return ++m_disableCount == 1;
	}

	bool
	enable()
	{
		ASSERT(m_disableCount);
		return --m_disableCount == 0;
	}

	void
	addFrame(
		HookCommonContext* context,
		size_t frameBase,
		size_t originalRet
		);

	size_t
	removeFrame(size_t frameBase);

	size_t
	getOriginalRet(size_t frameBase);

protected:
	void
	cleanup(const sl::RbTreeIterator<size_t, Frame>& it);

	void
	restoreOriginalRets();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern thread_local ThreadState g_threadState;

//..............................................................................

} // namespace spy
} // namespace axl
