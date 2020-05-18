﻿#include "pch.h"
#include "axl_spy_HookCommon.h"
#include "axl_spy_ThreadState.h"

namespace axl {
namespace spy {

//..............................................................................

HookAction
hookEnterCommon(
	HookCommonContext* context,
	size_t frameBase,
	size_t originalRet
	)
{
	if (!areHooksEnabled())
		return HookAction_JumpTarget;

	disableCurrentThreadHooks();

	ThreadState* threadState = getCurrentThreadState();

	HookAction action = context->m_enterFunc ?
		action = context->m_enterFunc(context->m_targetFunc, context->m_callbackParam, frameBase) :
		HookAction_Default;

	if (!context->m_leaveFunc)
		action = HookAction_JumpTarget;

	if (action == HookAction_Default)
		threadState->addFrame(context, frameBase, originalRet);

	enableCurrentThreadHooks();
	return action;
}

size_t
hookLeaveCommon(
	HookCommonContext* context,
	size_t frameBase
	)
{
	disableCurrentThreadHooks();
	ThreadState* threadState = getCurrentThreadState(false);
	ASSERT(threadState && context->m_leaveFunc);
	context->m_leaveFunc(context->m_targetFunc, context->m_callbackParam, frameBase);
	size_t originalRet = threadState->removeFrame(frameBase);
	enableCurrentThreadHooks();
	return originalRet;
}

//..............................................................................

} // namespace spy
} // namespace axl
