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
	LIBC_C_LIST
	axl_glibc_libc.c
)

set(
	LIBM_C_LIST
	axl_glibc_libm.c
)

source_group(
	glibc
	FILES
	${LIBC_C_LIST}
	${LIBM_C_LIST}
)

#...............................................................................
#
# axl_glibc libraries
#

add_library(
	axl_glibc_libc
	${LIBC_C_LIST}
)

set_target_properties(
	axl_glibc_libc
	PROPERTIES
	LANGUAGE C
)

add_library(
	axl_glibc_libm
	${LIBM_C_LIST}
)

set_target_properties(
	axl_glibc_libm
	PROPERTIES
	LANGUAGE C
)

#...............................................................................
