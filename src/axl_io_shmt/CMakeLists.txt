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
	${AXL_INC_DIR}/axl_io_shmt/axl_io_ShmtFile.h
	${AXL_INC_DIR}/axl_io_shmt/axl_io_ShmtBase.h
	${AXL_INC_DIR}/axl_io_shmt/axl_io_ShmtReader.h
	${AXL_INC_DIR}/axl_io_shmt/axl_io_ShmtWriter.h
)

set(
	LIB_CPP_LIST
	axl_io_ShmtBase.cpp
	axl_io_ShmtReader.cpp
	axl_io_ShmtWriter.cpp
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
set(PCH_CPP pch.cpp)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_io_shmt/axl_io_ShmtPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
)

#...............................................................................
#
# axl_io_shmt library
#

include_directories(${AXL_INC_DIR})

add_library(
	axl_io_shmt
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

axl_set_pch(
	axl_io_shmt
	${PCH_H}
	${PCH_CPP}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_io_shmt
	${LIB_H_LIST}
)

install(
	TARGETS axl_io_shmt
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
