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

if(NOT PERL_EXE)
	axl_find_executable(PERL_EXE perl)

	if(PERL_EXE)
		axl_message("Perl found at:" "${PERL_EXE}")
	endif()
endif()

if(EXISTS ${PERL_EXE})
	set(PERL_FOUND TRUE)
else()
	set(PERL_FOUND FALSE)
endif()

#...............................................................................

macro(
add_perl_step
	_OUTPUT_FILE
	_INPUT_FILE
	_SCRIPT_FILE
	# ...
	)

	if(NOT PERL_FOUND)
		message(FATAL_ERROR "Perl is required for add_perl_step() macro")
	endif()

	set(_SWITCH_LIST ${ARGN})

	axl_make_path(_OUTPUT_PATH ${_OUTPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR})
	axl_make_path(_INPUT_PATH ${_INPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})
	axl_make_path(_SCRIPT_PATH ${_SCRIPT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})

	add_custom_command(
		OUTPUT ${_OUTPUT_PATH}
		MAIN_DEPENDENCY ${_INPUT_PATH}
		COMMAND ${PERL_EXE} ${_SCRIPT_PATH} ${_SWITCH_LIST} ${_INPUT_PATH} > ${_OUTPUT_PATH}
		DEPENDS ${_SCRIPT_PATH}
		)
endmacro()

#...............................................................................
