#include "pch.h"
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
	if (!g_threadState.isEnabled())
		return HookAction_JumpTarget;

	g_threadState.disable();

	HookAction action = context->m_enterFunc ?
		action = context->m_enterFunc(context->m_targetFunc, context->m_callbackParam, frameBase) :
		HookAction_Default;

	if (!context->m_leaveFunc)
		action = HookAction_JumpTarget;

	if (action == HookAction_Default)
		g_threadState.addFrame(frameBase, originalRet);

	g_threadState.enable();
	return action;
}

uint64_t
hookLeaveCommon(
	HookCommonContext* context,
	size_t frameBase
	)
{
	ASSERT(context->m_leaveFunc);

	bool wasEnabled = g_threadState.disable();
	if (wasEnabled)
		context->m_leaveFunc(context->m_targetFunc, context->m_callbackParam, frameBase);

	uint64_t originalRet = g_threadState.removeFrame(frameBase);
	g_threadState.enable();
	return originalRet;
}

//..............................................................................

} // namespace spy
} // namespace axl
