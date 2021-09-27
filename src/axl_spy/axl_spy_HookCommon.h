#pragma once

#include "axl_spy_Hook.h"

namespace axl {
namespace spy {

//..............................................................................

struct HookCommonContext {
	void* m_targetFunc;
	void* m_callbackParam;
	HookEnterFunc* m_enterFunc;
	HookLeaveFunc* m_leaveFunc;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

HookAction
hookEnterCommon(
	HookCommonContext* context,
	size_t frameBase,
	size_t originalRet
);

size_t
hookLeaveCommon(
	HookCommonContext* context,
	size_t frameBase
);

//..............................................................................

} // namespace spy
} // namespace axl
