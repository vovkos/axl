#...............................................................................

axl_find_file (
	_CONFIG_CMAKE
	axl_config.cmake
	${AXL_CMAKE_DIR}
	)

if (_CONFIG_CMAKE)
	include (${_CONFIG_CMAKE})

	message (STATUS "AXL paths:")
	axl_message ("    CMake files:" "${AXL_CMAKE_DIR}")
	axl_message ("    Includes:"    "${AXL_INC_DIR}")
	axl_message ("    Libraries:"   "${AXL_LIB_DIR}")

	set (AXL_FOUND TRUE)
else ()
	set (AXL_FOUND FALSE)
endif ()

#...............................................................................
