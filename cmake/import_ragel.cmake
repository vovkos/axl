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

if(NOT RAGEL_EXE)
	axl_find_executable(RAGEL_EXE ragel)

	if(RAGEL_EXE)
		axl_message("Ragel found at:" "${RAGEL_EXE}")
	endif()
endif()

if(EXISTS ${RAGEL_EXE})
	set(RAGEL_FOUND TRUE)
else()
	set(RAGEL_FOUND FALSE)
endif()

#...............................................................................

macro(
add_ragel_step
	_OUTPUT_FILE
	_INPUT_FILE
	)

	if(NOT RAGEL_FOUND)
		message(FATAL_ERROR "Ragel is required for add_ragel_step() macro")
	endif()

	axl_make_path(_OUTPUT_PATH ${_OUTPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR})
	axl_make_path(_INPUT_PATH ${_INPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})

	add_custom_command(
		OUTPUT ${_OUTPUT_PATH}
		MAIN_DEPENDENCY ${_INPUT_PATH}
		COMMAND ${RAGEL_EXE} ${_INPUT_PATH} -o${_OUTPUT_PATH} -L
		)
endmacro()

#...............................................................................
