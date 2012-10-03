// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BasicBlock.h"

namespace axl {
namespace jnc {

//.............................................................................

class CControlFlowMgr
{
protected:
	rtl::CStdListT <CBasicBlock> m_BasicBlockList;
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
