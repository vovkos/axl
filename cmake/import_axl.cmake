#..............................................................................

if ("${AXL_CMAKE_DIR_2}" STREQUAL "")
	set (AXL_FOUND FALSE)
	message (STATUS "AXL:                        <not-found>")
else ()
	include (${AXL_CMAKE_DIR_2}/axl_config.cmake)	

	set (AXL_FOUND TRUE)
	message (STATUS "Path to AXL cmake files:    ${AXL_CMAKE_DIR}")
	message (STATUS "Path to AXL includes:       ${AXL_INC_DIR}")
	message (STATUS "Path to AXL libraries:      ${AXL_LIB_DIR}")
endif ()

#..............................................................................
