#..............................................................................

if ("${RAGEL_EXE}" STREQUAL "")
	set (RAGEL_FOUND FALSE)
	message (STATUS "Ragel:                      <not-found>")
else ()
	set (RAGEL_FOUND TRUE)
	message (STATUS "Path to Ragel executable:   ${RAGEL_EXE}")
endif ()

#..............................................................................

macro (
add_ragel_step
	_OUTPUT_FILE
	_INPUT_FILE
	)

	axl_make_path (_OUTPUT_PATH ${_OUTPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR})
	axl_make_path (_INPUT_PATH ${_INPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})

	add_custom_command (
		OUTPUT ${_OUTPUT_PATH}
		MAIN_DEPENDENCY ${_INPUT_PATH}
		COMMAND ${RAGEL_EXE} ${_INPUT_PATH} -o${_OUTPUT_PATH} -L
		)
endmacro ()

#..............................................................................
