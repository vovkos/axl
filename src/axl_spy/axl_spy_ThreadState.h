#pragma once

#include "axl_spy_HookCommon.h"

namespace axl {
namespace spy {

//..............................................................................

class ThreadState: public rc::RefCount
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
	sl::RbTreeIterator<size_t, Frame>
	findFrame(size_t frameBase)
	{
	#if (!_AXL_CPU_X86)
		return m_frameMap.find(frameBase);
	#else // allowance for stdcall ret <n>
		return m_frameMap.find(frameBase, sl::BinTreeFindRelOp_Le);
	#endif
	}

	void
	cleanup(const sl::RbTreeIterator<size_t, Frame>& it);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
areHooksEnabled();

ThreadState*
getCurrentThreadState(bool createIfNotExists = true);

//..............................................................................

} // namespace spy
} // namespace axl
