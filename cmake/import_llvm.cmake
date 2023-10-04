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

	if(NOT LLVM_VERSION_PATCH) # LLVM < 3.5
		string(REGEX MATCH "[0-9]+$" LLVM_VERSION_PATCH ${LLVM_VERSION})
	endif()

	if(NOT LLVM_CONFIG_EXE)
		find_program(
			LLVM_CONFIG_EXE
			llvm-config
			PATHS ${LLVM_INSTALL_PREFIX}/bin
			NO_CACHE
			NO_DEFAULT_PATH
		)
	endif()

	axl_message("LLVM ${LLVM_VERSION} paths:")
	axl_message("    Prefix:"      "${LLVM_INSTALL_PREFIX}")
	axl_message("    CMake files:" "${LLVM_CMAKE_DIR}")
	axl_message("    Includes:"    "${LLVM_INC_DIR}")
	axl_message("    Libraries:"   "${LLVM_LIB_DIR}")

	if(LLVM_CONFIG_EXE)
		axl_message("    llvm-config:" "${LLVM_CONFIG_EXE}")
	endif()

	set(LLVM_FOUND TRUE)
endif()

#...............................................................................

macro(
target_link_llvm_libraries
	_TARGET
	# ...
)

	set(_COMPONENT_LIST ${ARGN})
	unset(_LIB_LIST)

	if(LLVM_CONFIG_EXE)
		execute_process(
			COMMAND ${LLVM_CONFIG_EXE} --libs ${_COMPONENT_LIST}
			ERROR_FILE "NUL"
			OUTPUT_VARIABLE _OUTPUT
		)

		separate_arguments(_LIB_LIST UNIX_COMMAND ${_OUTPUT})
	endif()

	if(NOT _LIB_LIST)
		if(${LLVM_VERSION} VERSION_LESS 3.5)
			llvm_map_components_to_libraries(_LIB_LIST ${_COMPONENT_LIST})
		else()
			llvm_map_components_to_libnames(_LIB_LIST ${_COMPONENT_LIST})
		endif()
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
		list(APPEND _COMPONENT_LIST jit)
	elseif(${LLVM_VERSION} VERSION_GREATER_EQUAL 7.0)
		list(APPEND _COMPONENT_LIST OrcJIT)
	endif()

	target_link_llvm_libraries(
		${_TARGET}
		${_COMPONENT_LIST}
		${_EXTRA_COMPONENT_LIST}
	)
endmacro()

#...............................................................................
