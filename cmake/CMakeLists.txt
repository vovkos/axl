# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

configure_file (
	axl.cmake.in
	axl.cmake
	@ONLY
	)

configure_file (
	axl.cmake.in
	${CMAKE_CURRENT_LIST_DIR}/latest/axl-${TARGET_CPU}.cmake
	@ONLY
	)

configure_file (
	axl-latest.cmake.in
	${CMAKE_CURRENT_LIST_DIR}/latest/axl.cmake
	@ONLY
	)

#..............................................................................
