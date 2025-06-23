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

if(FIND_WDK_CMAKE_DIR)
	list(
		APPEND
		CMAKE_MODULE_PATH
		${FIND_WDK_CMAKE_DIR}
	)

	if(WDK_ROOT_DIR)
		set(ENV{WDKContentRoot} "${WDK_ROOT_DIR}")
	endif()

	find_package(WDK QUIET)

	if(WDK_FOUND)
		set(WDK_SDK_INC_DIR "${WDK_ROOT}/Include/${WDK_INC_VERSION}/shared")
		set(WDK_CRT_INC_DIR "${WDK_ROOT}/Include/${WDK_INC_VERSION}/km/crt")
		set(WDK_DDK_INC_DIR "${WDK_ROOT}/Include/${WDK_INC_VERSION}/km")
		set(WDK_DDK_LIB_DIR "${WDK_ROOT}/Lib/${WDK_LIB_VERSION}/km/${WDK_PLATFORM}")

		axl_message("WDK paths:")
		axl_message("    SDK includes:"  "${WDK_SDK_INC_DIR}")
		axl_message("    CRT includes:"  "${WDK_CRT_INC_DIR}")
		axl_message("    DDK includes:"  "${WDK_DDK_INC_DIR}")
		axl_message("    DDK libraries:" "${WDK_DDK_LIB_DIR}")
	endif()
elseif(EXISTS ${WDK_DDK_INC_DIR}/ntddk.h)
	axl_message("WDK paths:")
	axl_message("    WDK root:"      "${WDK_ROOT_DIR}")
	axl_message("    WDK binaries:"  "${WDK_BIN_DIR}")
	axl_message("    SDK includes:"  "${WDK_SDK_INC_DIR}")
	axl_message("    CRT includes:"  "${WDK_CRT_INC_DIR}")
	axl_message("    DDK includes:"  "${WDK_DDK_INC_DIR}")
	axl_message("    CRT libraries:" "${WDK_CRT_LIB_DIR}")
	axl_message("    DDK libraries:" "${WDK_DDK_LIB_DIR}")

	set(WDK_FOUND TRUE)
else()
	set(WDK_FOUND FALSE)
endif()

#...............................................................................
