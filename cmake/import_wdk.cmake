#..............................................................................

if (EXISTS ${WDK_DDK_INC_DIR}/ntddk.h)
	message (STATUS "Path definitions for WDK:")
	message (STATUS "    WDK root:       ${WDK_ROOT_DIR}")
	message (STATUS "    WDK binaries:   ${WDK_BIN_DIR}")
	message (STATUS "    CRT includes:   ${WDK_CRT_INC_DIR}")
	message (STATUS "    SDK includes:   ${WDK_SDK_INC_DIR}")
	message (STATUS "    DDK includes:   ${WDK_DDK_INC_DIR}")
	message (STATUS "    CRT libraries:  ${WDK_CRT_LIB_DIR}")
	message (STATUS "    DDK libraries:  ${WDK_DDK_LIB_DIR}")
	
	set (WDK_FOUND TRUE)
else ()
	set (WDK_FOUND FALSE)
endif ()

#..............................................................................
