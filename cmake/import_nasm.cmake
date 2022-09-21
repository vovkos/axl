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

if(NOT NASM_EXE)
	axl_find_executable(NASM_EXE nasm)

	if(NASM_EXE)
		axl_message("NASM found at:" "${NASM_EXE}")
	endif()
endif()

if(EXISTS ${NASM_EXE})
	set(NASM_FOUND TRUE)
else()
	set(NASM_FOUND FALSE)
endif()

#...............................................................................

macro(
add_nasm_step
	_OUTPUT_FILE
	_INPUT_FILE
)

	if(NOT NASM_FOUND)
		message(FATAL_ERROR "NASM is required for add_nasm_step() macro")
	endif()

	axl_make_path(_OUTPUT_PATH ${_OUTPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR})
	axl_make_path(_INPUT_PATH ${_INPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})

	add_custom_command(
		OUTPUT ${_OUTPUT_PATH}
		MAIN_DEPENDENCY ${_INPUT_PATH}
		COMMAND ${NASM_EXE} -o${_OUTPUT_PATH} ${ARGN} ${_INPUT_PATH}
	)
endmacro()

#...............................................................................
