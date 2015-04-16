#pragma once

//.............................................................................

#include "axl_g_Pch.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include <iphlpapi.h>
#	pragma comment (lib, "ws2_32.lib")
#	pragma comment (lib, "iphlpapi.lib")
#endif

// axl

#include "axl_rtl_String.h"
#include "axl_err_Error.h"
#include "axl_fsm_RegExp.h"
#include "axl_rtl_AutoPtrArray.h"
#include "axl_mt_LongJmpTry.h"
#include "axl_mt_DynamicLibrary.h"
#include "axl_io_Socket.h"
#include "axl_io_NetworkAdapter.h"

using namespace axl;

//.............................................................................
