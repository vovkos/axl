#...............................................................................
#
#  This file is part of the AXL library.
#
#  AXL is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/axl/license.txt
#
#...............................................................................

include_directories(${AXL_INC_DIR})

# override CRT settings to link statically (but don't touch CMake cache)

set(
	MSVC_FLAG_CRT_DEBUG
	"/MTd"
)

set(
	MSVC_FLAG_CRT_RELEASE
	"/MT"
)

axl_apply_compiler_flag_setting_list(
	MSVC_FLAG_CRT_DEBUG
	MSVC_FLAG_CRT_RELEASE
)

foreach(_LIB ${AXL_LIBCMT_TWIN_LIB_LIST})
	axl_add_twin_library(
		${_LIB}_libcmt
		${_LIB}
		"_psx_|_lnx_|_drw_|.c$"
		pch.h
	)
endforeach()

#...............................................................................
