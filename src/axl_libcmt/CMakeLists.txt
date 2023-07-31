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

foreach(_LIB ${AXL_LIBCMT_TWIN_LIB_LIST})
	axl_add_twin_library(
		${_LIB}_libcmt
		${_LIB}
		"_psx_|_lnx_|_drw_|.c$"
	)

	set_target_properties(
		${_LIB}_libcmt
		PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
	)
endforeach()

#...............................................................................
