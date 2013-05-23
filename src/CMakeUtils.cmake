#..............................................................................

macro (
print_all_vars)

	get_cmake_property(_VARIABLE_LIST VARIABLES)
	foreach (_VARIABLE ${_VARIABLE_LIST})
		message(STATUS "${_VARIABLE} = ${${_VARIABLE}}")
endmacro ()

#..............................................................................

set (__STACK)

macro (
push
	_VALUE
	)

	list (INSERT __STACK 0 ${_VALUE})
endmacro ()

macro (
pop
	_RESULT
	)

	list (GET __STACK 0 ${_RESULT})
	list (REMOVE_AT __STACK 0)
endmacro ()

macro (
push_and_set
	_VARIABLE
	_VALUE
	)

	push (${${_VARIABLE}})
	set (${_VARIABLE} ${_VALUE})
endmacro ()

#..............................................................................

macro (
make_path 
	_FILE_PATH
	_FILE_NAME
	_BASE_DIR
	)

	string (REGEX MATCH "/" _SLASH ${_FILE_NAME})
	
	if (_SLASH)
		set (${_FILE_PATH} ${_FILE_NAME})
	else ()
		set (${_FILE_PATH} "${_BASE_DIR}/${_FILE_NAME}")
	endif ()

endmacro ()

#..............................................................................
  
macro (
create_flag_list_regex
	_REGEX
	# ...
	)

	set (_FLAG_LIST ${ARGN})

	set (${_REGEX} "")

	foreach (_FLAG ${_FLAG_LIST})
		set (_FLAG_REGEX "(^| +)${_FLAG}($| +)")

		if (${_REGEX} STREQUAL "")
			set (${_REGEX} ${_FLAG_REGEX})
		else ()
			set (${_REGEX} "${${_REGEX}}|${_FLAG_REGEX}")
		endif ()
	endforeach ()
endmacro ()

macro (
create_flag_setting
	_SETTING
	_DEFAULT_VALUE
	_TARGET_VARIABLE
	_DESCRIPTION
	# ...
	)

	set (_FLAG_LIST ${ARGN})

	create_flag_list_regex (_REGEX ${_FLAG_LIST})

	string (
		REGEX MATCH 
		"${_REGEX}" 
		_CURRENT_VALUE
		"${${_TARGET_VARIABLE}}"
		)

	string (
		STRIP
		"${_CURRENT_VALUE}"
		_CURRENT_VALUE
		)

	if (NOT ${_SETTING})
		set (_FORCE FORCE)
	else ()
		set (_FORCE)
	endif ()

	if ("${_DEFAULT_VALUE}" STREQUAL "")
		set (
			${_SETTING} ${_CURRENT_VALUE}
			CACHE STRING ${_DESCRIPTION} ${_FORCE}
			)
	else ()
		set (
			${_SETTING} ${_DEFAULT_VALUE}
			CACHE STRING ${_DESCRIPTION} ${_FORCE}
			)
	endif ()

	set_property (
		CACHE 
		${_SETTING}
		PROPERTY STRINGS 
		" " ${_FLAG_LIST}
		)

	if ("${_CURRENT_VALUE}" STREQUAL "")
		set (${_TARGET_VARIABLE} "${${_TARGET_VARIABLE}} ${${_SETTING}}")
	else  ()
		string (
			REGEX REPLACE 
			"${_REGEX}" 
			" ${${_SETTING}} " 
			${_TARGET_VARIABLE} 
			"${${_TARGET_VARIABLE}}"
			)
	endif ()
endmacro ()

#..............................................................................

macro (
create_setting
	_SETTING
	_DEFAULT_VALUE
	_TYPE
	_DESCRIPTION
	# ...
	)

	if (NOT ${_SETTING})
		set (_FORCE FORCE)
	else ()
		set (_FORCE)
	endif ()

	set (
		${_SETTING} ${_DEFAULT_VALUE}
		CACHE ${_TYPE} ${_DESCRIPTION} ${_FORCE}
		)

	if (${ARGC} GREATER 4)
		set (_OPTION_LIST ${ARGN})

		set_property	(
			CACHE 
			${_SETTING}
			PROPERTY STRINGS 
			${_OPTION_LIST}
			)
	endif ()
endmacro ()

#..............................................................................

macro (
set_pch_msvc
	_TARGET
	_PCH_H
	_PCH_CPP
	)

	get_filename_component (_PCH_NAME ${_PCH_H} NAME_WE)
	set (_PCH_BIN "$(IntDir)/${_PCH_NAME}.pch")

	set_target_properties (
		${_TARGET}
		PROPERTIES 
		COMPILE_FLAGS "/Yu\"${_PCH_H}\" /Fp\"${_PCH_BIN}\""
		)  

	set_source_files_properties (
		${_PCH_CPP}
		PROPERTIES 
		COMPILE_FLAGS "/Yc\"${_PCH_H}\" /Fp\"${_PCH_BIN}\""
		OBJECT_OUTPUTS "${_PCH_BIN}"
		)
endmacro ()

macro (
set_pch_gcc
	_TARGET
	_PCH_H
	)

	set (_EXTRA_FLAG_LIST ${ARGN})

	set (_PCH_GCC "${CMAKE_CURRENT_BINARY_DIR}/${_PCH_H}.gcc")	
	set (_PCH_BIN "${CMAKE_CURRENT_BINARY_DIR}/${_PCH_H}.gcc.gch")	

	get_target_property (
		_TARGET_CXX_FLAGS
		${_TARGET}
		COMPILE_FLAGS
		)

	get_target_property (
		_TARGET_CXX_DEFINES
		${_TARGET}
		COMPILE_DEFINITIONS
		)  

	get_target_property (
		_TARGET_CXX_INCLUDES
		${_TARGET}
		INCLUDE_DIRECTORIES
		)  

	string (TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _CXX_FLAGS)
	set (_CXX_FLAGS "${${_CXX_FLAGS}}")

	if (_CXX_FLAGS)
		string (REPLACE " " ";" _CXX_FLAGS ${_CXX_FLAGS})
	endif ()

	if (_TARGET_CXX_FLAGS)
		string (REPLACE " " ";" _TARGET_CXX_FLAGS ${_TARGET_CXX_FLAGS})
		list (APPEND _CXX_FLAGS ${_TARGET_CXX_FLAGS})	
	endif ()

	if (_TARGET_CXX_DEFINES)
		foreach (_DEF ${_TARGET_CXX_DEFINES})
			list (APPEND _CXX_FLAGS "-D${_DEF}")
		endforeach ()		
	endif ()

	if (_TARGET_CXX_INCLUDES)
		foreach (_DIR ${_TARGET_CXX_INCLUDES})
			list (APPEND _CXX_FLAGS "-I${_DIR}")
		endforeach () 
	endif ()

	if (_EXTRA_FLAG_LIST)
		list (APPEND _CXX_FLAGS "${_EXTRA_FLAG_LIST}")
	endif ()
	
	file (
		WRITE ${_PCH_GCC} 
		"\#include \"${CMAKE_CURRENT_SOURCE_DIR}/${_PCH_H}\"\n"
		)

	add_custom_command (
		OUTPUT ${_PCH_BIN}
		MAIN_DEPENDENCY ${_PCH_H}
		COMMAND ${CMAKE_CXX_COMPILER} ${_CXX_FLAGS} -x c++-header -o${_PCH_BIN} ${_PCH_GCC}
		)

	include_directories (${CMAKE_CURRENT_BINARY_DIR})

	set_target_properties (
		${_TARGET}
		PROPERTIES COMPILE_FLAGS
		"-include \"${_PCH_GCC}\""
		)  
endmacro ()

macro (
set_pch
	_TARGET
	_PCH_H
	_PCH_CPP
	)

	if (MSVC)
		set_pch_msvc (${_TARGET} ${_PCH_H} ${_PCH_CPP} ${ARGN})
	elseif (CMAKE_COMPILER_IS_GNUCXX)
		set_pch_gcc (${_TARGET} ${_PCH_H} ${ARGN})
	endif ()
endmacro ()

#..............................................................................

macro (
add_ragel_step
	_OUTPUT_FILE
	_INPUT_FILE
	)

	make_path (_INPUT_PATH ${_INPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})
	make_path (_OUTPUT_PATH ${_OUTPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR})

	add_custom_command (
		OUTPUT ${_OUTPUT_PATH}
		MAIN_DEPENDENCY ${_INPUT_PATH}
		COMMAND ${PATH_RAGEL_EXE} ${_INPUT_PATH} -o${_OUTPUT_PATH} -L		
		)
endmacro () 

#..............................................................................

macro (
add_till_step
	_OUTPUT_FILE
	_INPUT_FILE
	_FRAME_PATH
	)

	make_path (_INPUT_PATH ${_INPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})
	make_path (_OUTPUT_PATH ${_OUTPUT_FILE} ${CMAKE_CURRENT_BINARY_DIR})

	add_custom_command (
		OUTPUT ${_OUTPUT_PATH}
		MAIN_DEPENDENCY ${_INPUT_PATH}
		COMMAND axl_pg ${_INPUT_PATH} -o${_OUTPUT_PATH} -f${_FRAME_PATH} -l
		DEPENDS axl_pg ${ARGN}
		)
endmacro ()

#..............................................................................

macro (
exclude_from_build
	# ...
	)

	set_source_files_properties (
		${ARGN}
		PROPERTIES 
		HEADER_FILE_ONLY true
		)
endmacro ()

#..............................................................................
