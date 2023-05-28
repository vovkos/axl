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
# lib folder
#

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_io_ssl/axl_io_SslCtx.h
	${AXL_INC_DIR}/axl_io_ssl/axl_io_SslError.h
	${AXL_INC_DIR}/axl_io_ssl/axl_io_Ssl.h
)

set(
	LIB_CPP_LIST
	axl_io_SslCtx.cpp
	axl_io_Ssl.cpp
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_io_ssl/axl_io_SslPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
)

#...............................................................................
#
# axl_io_ssl library
#

include_directories(
	${OPENSSL_INC_DIR}
	${AXL_INC_DIR}
)

add_library(
	axl_io_ssl
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_io_ssl
	PRIVATE
	${PCH_H}
)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	${PCH_H_LIST}
	${LIB_H_LIST}
)

install(
	TARGETS axl_io_ssl
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
