#..............................................................................

if ("${WDK_ROOT_DIR}" STREQUAL "")
	set (WDK_FOUND FALSE)
	message (STATUS "WDK:                        <not-found>")
else ()
	set (WDK_FOUND TRUE)
	message (STATUS "Path to WDK root:           ${WDK_ROOT_DIR}")
	message (STATUS "Path to WDK binaries:       ${WDK_BIN_DIR}")
	message (STATUS "Path to WDK CRT includes:   ${WDK_CRT_INC_DIR}")
	message (STATUS "Path to WDK SDK includes:   ${WDK_SDK_INC_DIR}")
	message (STATUS "Path to WDK DDK includes:   ${WDK_DDK_INC_DIR}")
	message (STATUS "Path to WDK CRT libraries:  ${WDK_CRT_LIB_DIR}")
	message (STATUS "Path to WDK DDK libraries:  ${WDK_DDK_LIB_DIR}")
endif ()

#..............................................................................
