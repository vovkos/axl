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

unset(LLVM_DIR CACHE)
set(LLVM_FOUND FALSE)

if(NOT LLVM_CMAKE_DIR)
	find_package(LLVM QUIET)
endif()

if(EXISTS ${LLVM_CMAKE_DIR}/LLVMConfig.cmake)
	include(${LLVM_CMAKE_DIR}/LLVMConfig.cmake)

	if(NOT LLVM_INC_DIR)
		set(LLVM_INC_DIR ${LLVM_INCLUDE_DIRS})
	endif()

	if(NOT LLVM_LIB_DIR)
		set(LLVM_LIB_DIR ${LLVM_LIBRARY_DIRS})
	endif()

	set(LLVM_VERSION ${LLVM_PACKAGE_VERSION})

	axl_message("LLVM ${LLVM_VERSION} paths:")
	axl_message("    CMake files:" "${LLVM_CMAKE_DIR}")
	axl_message("    Includes:"    "${LLVM_INC_DIR}")
	axl_message("    Libraries:"   "${LLVM_LIB_DIR}")

	set(LLVM_FOUND TRUE)
endif()

#...............................................................................

macro(
target_link_llvm_libraries
	_TARGET
	# ...
	)

	set(_COMPONENT_LIST ${ARGN})

	if(${LLVM_VERSION} VERSION_LESS 3.5)
		llvm_map_components_to_libraries(_LIB_LIST ${_COMPONENT_LIST})
	else()
		llvm_map_components_to_libnames(_LIB_LIST ${_COMPONENT_LIST})
	endif()

	target_link_libraries(${_TARGET} ${_LIB_LIST})
endmacro()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

macro(
target_link_llvm_jit_libraries
	_TARGET
	# ...
	)

	set(_EXTRA_COMPONENT_LIST ${ARGN})

	set(
		_COMPONENT_LIST
		native
		MCJIT
		IPO
		)

	if(${LLVM_VERSION} VERSION_LESS 3.6)
		set(_COMPONENT_LIST ${_COMPONENT_LIST} jit)
	endif()

	target_link_llvm_libraries(
		${_TARGET}
		${_COMPONENT_LIST}
		${_EXTRA_COMPONENT_LIST}
		)
endmacro()

#...............................................................................
