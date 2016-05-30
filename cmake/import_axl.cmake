#..............................................................................

axl_find_file (
	_CONFIG_CMAKE
	axl_config.cmake
	${AXL_CMAKE_DIR}
	)

if (_CONFIG_CMAKE)
	include (${_CONFIG_CMAKE})

	message (STATUS "Path defintions for AXL")
	message (STATUS "    AXL cmake files: ${AXL_CMAKE_DIR}")
	message (STATUS "    AXL includes:    ${AXL_INC_DIR}")
	message (STATUS "    AXL libraries:   ${AXL_LIB_DIR}")

	set (AXL_FOUND TRUE)	
else ()
	message (STATUS "AXL not found")

	set (AXL_FOUND FALSE)
endif ()

#..............................................................................
