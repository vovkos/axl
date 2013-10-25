#pragma once

#include "axl_g_Pch.h"

#ifdef _AXL_LOG_JANCY
#	define _AXL_LLVM_NO_JIT  // llvm headers FORCE linkage if JIT is included
#	include "axl_jnc_Pch.h"
#endif
