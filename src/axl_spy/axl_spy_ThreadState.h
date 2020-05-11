#pragma once

#include "axl_sl_RbTree.h"
#include "axl_sl_Array.h"
#include "axl_sys_Atomic.h"

namespace axl {
namespace spy {

//..............................................................................

extern volatile intptr_t g_enableCount;
extern thread_local bool g_threadStateDestructed;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ThreadState
{
protected:
	struct Frame
	{
		size_t m_originalRet;
		sl::Array<size_t> m_chainedRetStack; // forwarding via jmp

		Frame()
		{
			m_originalRet = 0;
		}
	};

protected:
	sl::RbTree<size_t, Frame> m_frameMap;
	size_t m_disableCount;

public:
	ThreadState()
	{
		m_disableCount = 0;
	}

	~ThreadState()
	{
		g_threadStateDestructed = true;
	}

	bool
	isEnabled()
	{
		return sys::atomicLoad(&g_enableCount) > 0 && !g_threadStateDestructed && !m_disableCount;
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
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern thread_local ThreadState g_threadState;

//..............................................................................

} // namespace spy
} // namespace axl
