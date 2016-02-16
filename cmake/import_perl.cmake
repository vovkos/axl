#..............................................................................

if ("${PERL_EXE}" STREQUAL "")
	set (PERL_FOUND FALSE)
	message (STATUS "Perl:                       <not-found>")
else ()
	set (PERL_FOUND TRUE)
	message (STATUS "Path to Perl executable:    ${PERL_EXE}")
endif ()

#..............................................................................

macro (
add_perl_step
	_OUTPUT_FILE
	_INPUT_FILE
	_SCRIPT_FILE
	# ... 
	)

	set (_SWITCH_LIST ${ARGN})
	
	axl_make_path (_OUTPUT_PATH ${_OUTPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR})
	axl_make_path (_INPUT_PATH ${_INPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})
	axl_make_path (_SCRIPT_PATH ${_SCRIPT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})

	add_custom_command (
		OUTPUT ${_OUTPUT_PATH}
		MAIN_DEPENDENCY ${_INPUT_PATH}
		COMMAND ${PERL_EXE} ${_SCRIPT_PATH} ${_SWITCH_LIST} ${_INPUT_PATH} > ${_OUTPUT_PATH}
		DEPENDS ${_SCRIPT_PATH}
		)
endmacro ()

#..............................................................................
