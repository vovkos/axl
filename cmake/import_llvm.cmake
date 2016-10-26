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

if (EXISTS ${LLVM_CMAKE_DIR}/LLVMConfig.cmake)
	include (${LLVM_CMAKE_DIR}/LLVMConfig.cmake)

	if (NOT LLVM_INC_DIR)
		set (LLVM_INC_DIR ${LLVM_INCLUDE_DIRS})
	endif ()

	if (NOT LLVM_LIB_DIR)
		set (LLVM_LIB_DIR ${LLVM_LIBRARY_DIRS})
	endif ()

	axl_message ("LLVM paths:")
	axl_message ("    CMake files:" "${LLVM_CMAKE_DIR}")
	axl_message ("    Includes:"    "${LLVM_INC_DIR}")
	axl_message ("    Libraries:"   "${LLVM_LIB_DIR}")

	set (LLVM_FOUND TRUE)
else ()
	set (LLVM_FOUND FALSE)
endif ()

#...............................................................................
