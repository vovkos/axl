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

if(NOT ARMASM_EXE)
	if(${TARGET_CPU} STREQUAL "arm64")
		axl_find_executable(ARMASM_EXE armasm64)
	elseif(${TARGET_CPU} STREQUAL "arm32")
		axl_find_executable(ARMASM_EXE armasm)
	endif()

	if(ARMASM_EXE)
		axl_message("ARMASM found at:" "${ARMASM_EXE}")
	endif()
endif()

if(EXISTS ${ARMASM_EXE})
	set(ARMASM_FOUND TRUE)
else()
	set(ARMASM_FOUND FALSE)
endif()

#...............................................................................

macro(
add_armasm_step
	_OUTPUT_FILE
	_INPUT_FILE
)

	if(NOT ARMASM_FOUND)
		message(FATAL_ERROR "ARMASM is required for add_armasm_step() macro")
	endif()

	axl_make_path(_OUTPUT_PATH ${_OUTPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR})
	axl_make_path(_INPUT_PATH ${_INPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})

	add_custom_command(
		OUTPUT ${_OUTPUT_PATH}
		MAIN_DEPENDENCY ${_INPUT_PATH}
		COMMAND "${ARMASM_EXE}" ${ARGN} -o${_OUTPUT_PATH} ${_INPUT_PATH}
	)
endmacro()

#...............................................................................
