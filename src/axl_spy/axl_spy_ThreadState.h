#pragma once

#include "axl_spy_HookCommon.h"
#include "axl_sl_RbTree.h"
#include "axl_sl_Array.h"
#include "axl_sys_Atomic.h"

namespace axl {
namespace spy {

//..............................................................................

class ThreadState: public ref::RefCount
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

public:
	~ThreadState();

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

bool
areHooksEnabled();

ThreadState*
getCurrentThreadState(bool createIfNotExists = true);

//..............................................................................

} // namespace spy
} // namespace axl
