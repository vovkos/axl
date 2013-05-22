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
create_flag_setting_impl
	_SETTING
	_TARGET_FLAG_STRING
	_DESCRIPTION
	_REGEX
	# ...
	)

	set (_FLAG_LIST ${ARGN})

	string (
		REGEX MATCH 
		"${_REGEX}" 
		_TARGET_SETTING_VALUE
		"${${_TARGET_FLAG_STRING}}"
		)

	string (
		STRIP
		"${_TARGET_SETTING_VALUE}"
		_TARGET_SETTING_VALUE
		)

	if (NOT ${_SETTING}) 
		set (
			${_SETTING} ${_TARGET_SETTING_VALUE}
			CACHE STRING ${_DESCRIPTION}
			)

		set_property	(
			CACHE 
			${_SETTING}
			PROPERTY STRINGS 
			" " ${_FLAG_LIST}
			)
	else ()
		if ("${_TARGET_SETTING_VALUE}" STREQUAL "")
			set (${_TARGET_FLAG_STRING} "${${_TARGET_FLAG_STRING}} ${${_SETTING}}")
		else  ()
			string (
				REGEX REPLACE 
				"${_REGEX}" 
				" ${${_SETTING}} " 
				${_TARGET_FLAG_STRING} 
				"${${_TARGET_FLAG_STRING}}"
				)
		endif ()
	endif ()
endmacro ()

macro (
create_flag_setting
	_IS_PER_CONFIG
	_SETTING
	_TARGET_FLAG_STRING
	_DESCRIPTION
	# ...
	)

	set (_FLAG_LIST ${ARGN})

	create_flag_list_regex (_REGEX ${_FLAG_LIST})

	if (${_IS_PER_CONFIG})
		foreach (_CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
			string (TOUPPER "${_CONFIG_TYPE}" _CONFIG_TYPE)
			create_flag_setting_impl (
				${_SETTING}_${_CONFIG_TYPE} 
				${_TARGET_FLAG_STRING}_${_CONFIG_TYPE} 
				${_DESCRIPTION} 
				${_REGEX}
				${_FLAG_LIST}
				)
		endforeach ()
	else ()
		create_flag_setting_impl (
			${_SETTING} 
			${_TARGET_FLAG_STRING}
			${_DESCRIPTION} 
			${_REGEX}
			${_FLAG_LIST}
			)
	endif ()
endmacro ()

#..............................................................................

macro (
create_setting
	_IS_PER_CONFIG
	_SETTING
	_DEFAULT
	_TYPE
	_DESCRIPTION
	# ...
	)

	if (${ARGC} GREATER 5)
		set (_OPTION_LIST ${ARGN})

		if (${_IS_PER_CONFIG})
			foreach (_CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
				string (TOUPPER "${_CONFIG_TYPE}" _CONFIG_TYPE)
				set  (
					${_SETTING}_${_CONFIG_TYPE} ${${_DEFAULT}_${_CONFIG_TYPE}}
					CACHE ${_TYPE}
					${_DESCRIPTION} 
					)

				set_property	(
					CACHE 
					${_SETTING}_${_CONFIG_TYPE} 
					PROPERTY STRINGS 
					${_OPTION_LIST}
					)
			endforeach ()
		else ()
			set (
				${_SETTING} ${${_DEFAULT}}
				CACHE ${_TYPE}
				${_DESCRIPTION} 
				)

			set_property	(
				CACHE 
				${_SETTING}
				PROPERTY STRINGS 
				${_OPTION_LIST}
				)
		endif ()
	else ()
		if (${_IS_PER_CONFIG})
			foreach (_CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
				string (TOUPPER "${_CONFIG_TYPE}" _CONFIG_TYPE)
				set  (
					${_SETTING}_${_CONFIG_TYPE} ${${_DEFAULT}_${_CONFIG_TYPE}}
					CACHE ${_TYPE}
					${_DESCRIPTION} 
					)
			endforeach ()
		else ()
			set (
				${_SETTING} ${${_DEFAULT}}
				CACHE ${_TYPE}
				${_DESCRIPTION} 
				)
		endif ()
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
