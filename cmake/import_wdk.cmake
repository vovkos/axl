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

if(EXISTS ${WDK_DDK_INC_DIR}/ntddk.h)
	axl_message("WDK paths:")
	axl_message("    WDK root:"      "${WDK_ROOT_DIR}")
	axl_message("    WDK binaries:"  "${WDK_BIN_DIR}")
	axl_message("    CRT includes:"  "${WDK_CRT_INC_DIR}")
	axl_message("    SDK includes:"  "${WDK_SDK_INC_DIR}")
	axl_message("    DDK includes:"  "${WDK_DDK_INC_DIR}")
	axl_message("    CRT libraries:" "${WDK_CRT_LIB_DIR}")
	axl_message("    DDK libraries:" "${WDK_DDK_LIB_DIR}")

	set(WDK_FOUND TRUE)
else()
	set(WDK_FOUND FALSE)
endif()

#...............................................................................
