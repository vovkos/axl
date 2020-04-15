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

#
# glibc folder
#

set(
	LIB_H_LIST
	)

set(
	LIB_C_LIST
	axl_clock.c
	axl_fcntl.c
	axl_getenv.c
	axl_math.c
	axl_memcpy.c
	axl_spawn.c
	)

source_group(
	glibc
	FILES
	${LIB_C_LIST}
	)

if(NOT "${TARGET_CPU}" STREQUAL "x86")
	axl_exclude_from_build(axl_fcntl.c)
endif()

#...............................................................................
#
# axl_glibc library
#

add_library(
	axl_glibc
	${LIB_H_LIST}
	${LIB_C_LIST}
	)

set_target_properties(
	axl_glibc
	PROPERTIES
	LANGUAGE C
	)

#...............................................................................
