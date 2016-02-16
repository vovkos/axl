# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2016. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

configure_file (
	axl_config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/axl_config.cmake
	@ONLY
	)

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/paths.cmake)
	configure_file (
		paths.cmake.template
		${CMAKE_SOURCE_DIR}/paths.cmake
		COPYONLY
		)
endif ()

#..............................................................................
