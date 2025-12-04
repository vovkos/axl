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

# aligned variable list printing helpers

macro(
axl_calc_max_string_length
	_RESULT
	# ...
)

	set(_STRING_LIST ${ARGN})

	set(_MAX_LENGTH 0)
	foreach(_STRING ${_STRING_LIST})
		string(LENGTH ${_STRING} _LENGTH)
		if(${_LENGTH} GREATER ${_MAX_LENGTH})
			set(_MAX_LENGTH ${_LENGTH})
		endif()
	endforeach()

	set(${_RESULT} ${_MAX_LENGTH})
endmacro()

macro(
axl_create_space_padding
	_RESULT
	_STRING
	_MAX_LENGTH
)

	string(LENGTH ${_STRING} _LENGTH)
	math(EXPR _PADDING_LENGTH "${_MAX_LENGTH} - ${_LENGTH}")

	if(${_PADDING_LENGTH} GREATER "0")
		string(RANDOM LENGTH ${_PADDING_LENGTH} ALPHABET " " ${_RESULT})
	else()
		set(${_RESULT} "")
	endif()
endmacro()

macro(
axl_print_variable_list
	_INDENT
	_MIN_ALIGN
	# ...
)

	set(_VARIABLE_LIST ${ARGN})

	axl_calc_max_string_length(_ALIGN ${_VARIABLE_LIST})

	string(LENGTH ${_INDENT} _INDENT_LENGTH)
	math(EXPR _ALIGN "${_ALIGN} + ${_INDENT_LENGTH} + 2") # colon and space

	if(${_ALIGN} LESS ${_MIN_ALIGN})
		set(_ALIGN ${_MIN_ALIGN})
	endif()

	foreach(_VARIABLE ${_VARIABLE_LIST})
		set(_PREFIX "${_INDENT}${_VARIABLE}:")
		axl_create_space_padding(_PADDING ${_PREFIX} ${_ALIGN})
		axl_message("${_PREFIX}${_PADDING}" ${${_VARIABLE}})
	endforeach()
endmacro()

macro(
axl_filter_list
	_RESULT
	_FILTER
	# ...
)

	set(_LIST ${ARGN})
	unset(_FILTERED_LIST)

	foreach(_ITEM ${_LIST})
		string(REGEX MATCH "${_FILTER}" _MATCH "${_ITEM}")
		if(_MATCH)
			list(APPEND _FILTERED_LIST ${_ITEM})
		endif()
	endforeach()

	set(${_RESULT} ${_FILTERED_LIST})
endmacro()

# when we don't know all the items on the list in advance,
# the best we can do is to align it on a constant column offset

set(AXL_G_MESSAGE_ALIGN 16) # adjustable

macro(
axl_message
	_FIRST
	# ...
)

	set(_ARG_LIST ${ARGN})

	if(NOT _ARG_LIST)
		message(STATUS "${_FIRST}")
	else()
		axl_create_space_padding(_PADDING ${_FIRST} ${AXL_G_MESSAGE_ALIGN})
		list(LENGTH _ARG_LIST _LENGTH)
		if(_LENGTH LESS 2)
			message(STATUS "${_FIRST}${_PADDING}${_ARG_LIST}")
		else()
			list(GET _ARG_LIST 0 _ITEM)
			list(REMOVE_AT _ARG_LIST 0)

			message(STATUS "${_FIRST}${_PADDING}${_ITEM}")
			string(LENGTH "${_FIRST}${_PADDING}" _LENGTH)
			axl_create_space_padding(_PADDING " " ${_LENGTH})

			foreach(_ITEM ${_ARG_LIST})
				message(STATUS "${_PADDING} ${_ITEM}")
			endforeach()
		endif()
	endif()
endmacro()

#...............................................................................

# create a template file in form
#
# set (SETTING1 SETTING1-NOTFOUND)
# set (SETTING2 SETTING2-NOTFOUND)
# ...
#

macro(
	axl_create_empty_setting_file
	_FILE_NAME
	# ...
)

	set(_SETTING_LIST ${ARGN})

	axl_calc_max_string_length(_MAX_LENGTH ${_SETTING_LIST})

	set(_CONTENTS "#\n")
	set(_CONTENTS "${_CONTENTS}# This is an auto-generated setting file\n")
	set(_CONTENTS "${_CONTENTS}# Fill it by hand with proper settings\n")
	set(_CONTENTS "${_CONTENTS}#\n\n")

	foreach(_SETTING ${_SETTING_LIST})
		axl_create_space_padding(_PADDING ${_SETTING} ${_MAX_LENGTH})
		set(_CONTENTS "${_CONTENTS}set(${_SETTING}${_PADDING} ${_SETTING}-NOTFOUND)\n")
	endforeach()

	file(WRITE ${_FILE_NAME} ${_CONTENTS})
endmacro()

#...............................................................................

# detect host/target CPU (we support x86 and amd64)

macro(
axl_normalize_cpu
	_RESULT
	_CPU
)

	string(TOLOWER ${_CPU} _CPU_LC)

	if("${_CPU_LC}" MATCHES "^(amd64|x86_64|x64)$")
		set(${_RESULT} "amd64")
	elseif("${_CPU_LC}" MATCHES "^(x86|i386)$")
		set(${_RESULT} "x86")
	elseif("${_CPU_LC}" MATCHES "^(aarch64|arm64)$")
		set(${_RESULT} "arm64")
	elseif("${_CPU_LC}" MATCHES "^(arm)$")
		set(${_RESULT} "arm32")
	else()
		set(${_RESULT} "${_CPU}") # can't normalize -- leave as is
	endif()
endmacro()

macro(
axl_detect_target_cpu
	_CPU
)

	# TODO: replace with try_run and CPU detection C pre-processor code

	get_filename_component(_CXX_COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME)

	set(_CPUID)

	string(
		APPEND _CPUID
		${_CXX_COMPILER_NAME}
		${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}
		${CMAKE_GENERATOR_PLATFORM}
		${CMAKE_SYSTEM_PROCESSOR}
	)

	string(TOLOWER "${_CPUID}" _CPUID)

	if("${_CPUID}" MATCHES "arm64|aarch64")
		set(${_CPU} "arm64")
	elseif("${_CPUID}" MATCHES "arm")
		set(${_CPU} "arm32")
	else()
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(${_CPU} "amd64")
		else()
			set(${_CPU} "x86")
		endif()
	endif()
endmacro()

macro(
axl_detect_host_cpu
	_CPU
)

	axl_normalize_cpu(${_CPU} "${CMAKE_HOST_SYSTEM_PROCESSOR}")
endmacro()

#...............................................................................

# push and pop variables on stack

set(_AXL_G_STACK)

macro(
axl_push
	_VALUE
)

	list(INSERT _AXL_G_STACK 0 ${_VALUE})
endmacro()

macro(
axl_pop
	_RESULT
)

	list(GET _AXL_G_STACK 0 ${_RESULT})
	list(REMOVE_AT _AXL_G_STACK 0)
endmacro()

macro(
axl_push_and_set
	_VARIABLE
	_VALUE
)

	axl_push(${${_VARIABLE}})
	set(${_VARIABLE} ${_VALUE})
endmacro()

#...............................................................................

# cache setting with list of options

macro(
axl_create_setting
	_SETTING
	# ...
)

	set(_ARG_LIST ${ARGN})

	set(_DESCRIPTION "${_SETTING}")
	set(_DEFAULT_VALUE)
	set(_TYPE STRING)
	set(_OPTION_LIST)
	set(_STATE "OPTIONS")

	foreach(_ARG ${_ARG_LIST})
		string(REGEX MATCH "^(TYPE|DESCRIPTION|DEFAULT|OPTIONS)$" _MATCH ${_ARG})

		if(NOT "${_MATCH}" STREQUAL "")
			set(_STATE ${_MATCH})
		elseif("${_STATE}" STREQUAL "TYPE")
			set(_TYPE ${_ARG})
			set(_STATE "OPTIONS")
		elseif("${_STATE}" STREQUAL "DESCRIPTION")
			set(_DESCRIPTION ${_ARG})
			set(_STATE "OPTIONS")
		elseif("${_STATE}" STREQUAL "DEFAULT")
			set(_DEFAULT_VALUE ${_ARG})
			set(_STATE "OPTIONS")
		else()
			set(_OPTION_LIST ${_OPTION_LIST} ${_ARG})
		endif()
	endforeach()

	if("${${_SETTING}}" STREQUAL "")
		set(_FORCE FORCE)
	else()
		set(_FORCE)
	endif()

	set(
		${_SETTING} ${_DEFAULT_VALUE}
		CACHE ${_TYPE} ${_DESCRIPTION} ${_FORCE}
	)

	if(_OPTION_LIST)
		set_property(
			CACHE ${_SETTING}
			PROPERTY STRINGS
			${_OPTION_LIST}
		)
	endif()
endmacro()

macro(
axl_override_setting
	_SETTING
	# ...
)

	set(_ARG_LIST ${ARGN})

	if(_ARG_LIST)
		list(GET _ARG_LIST 0 _VALUE)
	else()
		set(_VALUE)
	endif()

	get_property(
		_TYPE
		CACHE ${_SETTING}
		PROPERTY TYPE
	)

	get_property(
		_DESCRIPTION
		CACHE ${_SETTING}
		PROPERTY HELPSTRING
	)

	if("${_TYPE}" STREQUAL "" OR "${_DESCRIPTION}" STREQUAL "")
		message(FATAL_ERROR "${_SETTING} is not a cached setting")
	endif()

	set(
		${_SETTING} ${_VALUE}
		CACHE ${_TYPE} ${_DESCRIPTION} FORCE
	)
endmacro()

macro(
axl_override_setting_once
	_SETTING
	# ...
)

	if("${${_SETTING}_OVERRIDEN}" STREQUAL "")
		axl_override_setting(${_SETTING} ${ARGN})

		set(
			${_SETTING}_OVERRIDEN TRUE
			CACHE INTERNAL "${_SETTING} is overriden"
		)
	endif()
endmacro()

#...............................................................................

# compiler flag settings (represented by mutually exclusive options in CMAKE_C/CXX_FLAGS)

# global list (users can add their own configurable settings here)

set(_AXL_G_COMPILER_FLAG_SETTING_LIST)

macro(
axl_create_compiler_flag_setting
	_SETTING
	# ...
)

	set(_ARG_LIST ${ARGN})

	set(_DESCRIPTION "${_SETTING}")
	set(_DEFAULT_VALUE)
	set(_OPTION_LIST)
	set(_STATE "OPTIONS")

	foreach(_ARG ${_ARG_LIST})
		string(REGEX MATCH "^(DESCRIPTION|DEFAULT|OPTIONS)$" _MATCH ${_ARG})

		if(NOT "${_MATCH}" STREQUAL "")
			set(_STATE ${_MATCH})
		elseif("${_STATE}" STREQUAL "DESCRIPTION")
			set(_DESCRIPTION ${_ARG})
			set(_STATE "OPTIONS")
		elseif("${_STATE}" STREQUAL "DEFAULT")
			set(_DEFAULT_VALUE ${_ARG})
			set(_STATE "OPTIONS")
		else()
			set(_OPTION_LIST ${_OPTION_LIST} ${_ARG})
		endif()
	endforeach()

	if(NOT "${${_SETTING}}" STREQUAL "") # setting is already defined (probably, via settings.cmake)
		set(_VALUE ${${_SETTING}})
	elseif(NOT "${_DEFAULT_VALUE}" STREQUAL "")
		set(_VALUE ${_DEFAULT_VALUE})
	else()
		axl_get_compiler_flag_setting_current_value(_VALUE ${_SETTING} ${_OPTION_LIST})
	endif()

	set(
		${_SETTING} ${_VALUE}
		CACHE STRING ${_DESCRIPTION}
	)

	set_property(
		CACHE ${_SETTING}
		PROPERTY STRINGS
		" " ${_OPTION_LIST}
	)

	list(APPEND _AXL_G_COMPILER_FLAG_SETTING_LIST ${_SETTING})
endmacro()

macro(
axl_delete_compiler_flag_setting
	_SETTING
)

	list(REMOVE_ITEM _AXL_G_COMPILER_FLAG_SETTING_LIST ${_SETTING})
endmacro()

macro(
axl_get_compiler_flag_setting_current_value
	_RESULT
	_SETTING
	# ...
)

	axl_create_compiler_flag_regex(_REGEX ${ARGN})

	axl_get_compiler_flag_setting_traits(
		${_SETTING}
		_CONFIGURATION_SUFFIX
		_IS_C_ONLY
		_IS_CPP_ONLY
	)

	if(_IS_C_ONLY)
		if("${_CONFIGURATION_SUFFIX}" STREQUAL "")
			set(_FLAGS CMAKE_C_FLAGS)
		else()
			set(_FLAGS CMAKE_C_FLAGS${_CONFIGURATION_SUFFIX})
		endif()
	else()
		if("${_CONFIGURATION_SUFFIX}" STREQUAL "")
			set(_FLAGS CMAKE_CXX_FLAGS)
		else()
			set(_FLAGS CMAKE_CXX_FLAGS${_CONFIGURATION_SUFFIX})
		endif()
	endif()

	string(REGEX MATCH ${_REGEX} _MATCH " ${${_FLAGS}} ")
	string(STRIP "${_MATCH}" ${_RESULT})
endmacro()

macro(
axl_get_compiler_flag_setting_traits
	_SETTING
	_CONFIGURATION_SUFFIX_RESULT
	_IS_C_ONLY_RESULT
	_IS_CPP_ONLY_RESULT
)

	# check whether this setting is per-configuration

	set(_CONFIGURATION_SUFFIX)

	string(TOUPPER "${_SETTING}" _SETTING_UC)

	foreach(_CONFIGURATION ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER "${_CONFIGURATION}" _CONFIGURATION_UC)
		string(REGEX MATCH "_${_CONFIGURATION_UC}$" _MATCH "${_SETTING}")

		if(NOT "${_MATCH}" STREQUAL "")
			set(_CONFIGURATION_SUFFIX "_${_CONFIGURATION_UC}")
			break()
		endif()
	endforeach()

	# check whether this setting is C-only

	string(REGEX MATCH "_C_" _MATCH "${_SETTING}")
	if(NOT "${_MATCH}" STREQUAL "")
		set(_IS_C_ONLY TRUE)
	else()
		set(_IS_C_ONLY FALSE)
	endif()

	# check whether this setting is C++-only

	string(REGEX MATCH "_CPP_|_CXX_" _MATCH "${_SETTING}")
	if(NOT "${_MATCH}" STREQUAL "")
		set(_IS_CPP_ONLY TRUE)
	else()
		set(_IS_CPP_ONLY FALSE)
	endif()

	set(${_CONFIGURATION_SUFFIX_RESULT} ${_CONFIGURATION_SUFFIX})
	set(${_IS_C_ONLY_RESULT} ${_IS_C_ONLY})
	set(${_IS_CPP_ONLY_RESULT} ${_IS_CPP_ONLY})
endmacro()

macro(
axl_apply_compiler_flag_setting
	_SETTING
)

	axl_create_compiler_flag_setting_regex(_REGEX ${_SETTING})

	axl_get_compiler_flag_setting_traits(
		${_SETTING}
		_CONFIGURATION_SUFFIX
		_IS_C_ONLY
		_IS_CPP_ONLY
	)

	# apply current setting value to C/C++ flags

	if(NOT _IS_CPP_ONLY)
		if(NOT "${_CONFIGURATION_SUFFIX}" STREQUAL "")
			axl_apply_compiler_flag(
				CMAKE_C_FLAGS
				${_REGEX}
			)
		endif()

		axl_apply_compiler_flag(
			CMAKE_C_FLAGS${_CONFIGURATION_SUFFIX}
			${_REGEX}
			${${_SETTING}}
		)
	endif()

	if(NOT _IS_C_ONLY)
		if(NOT "${_CONFIGURATION_SUFFIX}" STREQUAL "")
			axl_apply_compiler_flag(
				CMAKE_CXX_FLAGS
				${_REGEX}
			)
		endif()

		axl_apply_compiler_flag(
			CMAKE_CXX_FLAGS${_CONFIGURATION_SUFFIX}
			${_REGEX}
			${${_SETTING}}
		)
	endif()
endmacro()

macro(
axl_apply_compiler_flag_setting_list
	# ...
)

	set(_SETTING_LIST ${ARGN})

	foreach(_SETTING ${_SETTING_LIST})
		axl_apply_compiler_flag_setting(${_SETTING})
	endforeach()
endmacro()

macro(
axl_apply_all_compiler_flag_settings)
	axl_apply_compiler_flag_setting_list(${_AXL_G_COMPILER_FLAG_SETTING_LIST})
endmacro()

macro(
axl_create_compiler_flag_setting_regex
	_RESULT
	_SETTING
)

	# create regex from option list and

	get_property(
		_OPTION_LIST
		CACHE ${_SETTING}
		PROPERTY STRINGS
	)

	if(NOT _OPTION_LIST)
		message(FATAL_ERROR "${_SETTING} does not have an associated option list")
	endif()

	axl_create_compiler_flag_regex(_REGEX ${_OPTION_LIST})
endmacro()

# direct management of individual flags

macro(
axl_create_compiler_flag_regex
	_RESULT
	# ...
)

	set(_OPTION_LIST ${ARGN})

	set(_REGEX "")

	foreach(_OPTION ${_OPTION_LIST})
		string(STRIP ${_OPTION} _OPTION)
		if(NOT "${_OPTION}" STREQUAL "")
			string(REPLACE "+" "%+" _OPTION ${_OPTION}) # e.g. -std=c++0x
			set(_OPTION_REGEX " +${_OPTION} +")

			if("${_REGEX}" STREQUAL "")
				set(_REGEX ${_OPTION_REGEX})
			else()
				set(_REGEX "${_REGEX}|${_OPTION_REGEX}")
			endif()
		endif()
	endforeach()

	set(${_RESULT} ${_REGEX})
endmacro()

macro(
axl_apply_compiler_flag
	_RESULT
	_REGEX
	# ... _VALUE
)

	set(_ARG_LIST ${ARGN})
	if(_ARG_LIST)
		list(GET _ARG_LIST 0 _VALUE)
	else()
		set(_VALUE)
	endif()

	set(_FLAGS ${${_RESULT}})

	# first, remove

	string(
		REGEX REPLACE
		"${_REGEX}"
		" "
		_FLAGS
		" ${_FLAGS} " # surround with spaces
	)

	string(STRIP "${_FLAGS}" _FLAGS)

	# now, if value is not empty, add it

	string(STRIP "${_VALUE}" _VALUE)

	if(NOT "${_VALUE}" STREQUAL "")
		set(_FLAGS "${_FLAGS} ${_VALUE}")
	endif()

	set(${_RESULT} ${_FLAGS})
endmacro()

macro(
axl_apply_compiler_flag_setting_to_list
	_SETTING
	# ... _FLAG_LIST
)

	set(_FLAG_LIST ${ARGN})

	axl_create_compiler_flag_setting_regex(_REGEX ${_SETTING})

	foreach(_FLAG ${_FLAG_LIST})
		axl_apply_compiler_flag(${_FLAG} ${_REGEX} ${${_SETTING}})
	endforeach()
endmacro()

macro(
axl_set_export_version_script_gcc
	_TARGET
	# ... _EXPORT_LIST
)

	if(APPLE)
		message(FATAL_ERROR "axl_set_export_version_script is not supported")
	endif()

	set(_EXPORT_LIST ${ARGN})
	set(_VERSION_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/${_TARGET}-export.version")

	file(WRITE ${_VERSION_SCRIPT} "{ global: ${_EXPORT_LIST}; local: *; };")

	set_property(
		TARGET ${_TARGET}
		APPEND_STRING
		PROPERTY LINK_FLAGS
		" -Wl,--version-script='${_VERSION_SCRIPT}'"
	)
endmacro()

macro(
axl_create_predefined_macro_file_gcc
	_COMPILER
	_SOURCE_FILE
	_OUTPUT_FILE
)

	file(WRITE ${_SOURCE_FILE}) # create an empty file

	execute_process(
		COMMAND
			${_COMPILER}
			-E                 # pre-process only
			-dM                # output as: #define <name>=<value>
			-o ${_OUTPUT_FILE}
			${_SOURCE_FILE}
		)
endmacro()

#...............................................................................

# exclude files from build (but keep it in project tree)

macro(
axl_set_header_file_only
	_IS_HEADER_FILE_ONLY
	# ...
)
	set_source_files_properties(
		${ARGN}
		PROPERTIES
		HEADER_FILE_ONLY ${_IS_HEADER_FILE_ONLY}
		SKIP_PRECOMPILE_HEADERS ${_IS_HEADER_FILE_ONLY}
	)
endmacro()

macro(
axl_is_excluded_from_build
	_RESULT
	_FILE_NAME
)

	get_source_file_property(
		${_RESULT}
		${_FILE_NAME}
		HEADER_FILE_ONLY
	)
endmacro()

macro(
axl_exclude_from_build
	# ...
)

	axl_set_header_file_only(
		TRUE
		${ARGN}
	)
endmacro()

macro(
axl_undo_exclude_from_build
	# ...
)

	axl_set_header_file_only(
		FALSE
		${ARGN}
	)
endmacro()

#...............................................................................

# file helpers

macro(
axl_find_executable
	_RESULT
	_FILE_NAME
)

	if(WIN32)
		execute_process(
			COMMAND where ${_FILE_NAME}
			ERROR_FILE "NUL"
			OUTPUT_VARIABLE _OUTPUT
		)

		if(NOT "${_OUTPUT}" STREQUAL "")
			string(REGEX REPLACE "\n.*" "" _OUTPUT ${_OUTPUT})
			string(REPLACE "\\" "/" _OUTPUT "${_OUTPUT}")
		endif()
	else()
		execute_process(
			COMMAND which ${_FILE_NAME}
			ERROR_FILE /dev/null
			OUTPUT_VARIABLE _OUTPUT
		)
	endif()

	string(STRIP "${_OUTPUT}" _OUTPUT)

	if("${_OUTPUT}" STREQUAL "")
		set(${_RESULT} ${_RESULT}-NOTFOUND)
	else()
		set(${_RESULT} ${_OUTPUT})
	endif()
endmacro()

macro(
axl_find_inc_dir
	_RESULT
	# ...
)

	unset(_DIR)
	find_path(_DIR ${ARGN})

	if(NOT _DIR)
		set(${_RESULT} ${_RESULT}-NOTFOUND)
	elseif(WIN32)
		string(REPLACE "\\" "/" ${_RESULT} "${_DIR}")
	else()
		set(${_RESULT} ${_DIR})
	endif()

	unset(_DIR CACHE)
endmacro()

macro(
axl_find_lib_dir_ex
	# ...
)
	set(_ARG_LIST ${ARGN})

	set(_RESULT_LIB_DIR)
	set(_RESULT_LIB_NAME)
	set(_LIB_DIR_LIST)
	set(_LIB_NAME_LIST)
	set(_STATE)

	foreach(_ARG ${_ARG_LIST})
		string(REGEX MATCH "^(RESULT_LIB_DIR|RESULT_LIB_NAME|LIB_DIR|LIB_NAME)$" _MATCH ${_ARG})

		if(NOT "${_MATCH}" STREQUAL "")
			set(_STATE ${_MATCH})
		elseif("${_STATE}" STREQUAL "RESULT_LIB_DIR")
			set(_RESULT_LIB_DIR ${_ARG})
			set(_STATE)
		elseif("${_STATE}" STREQUAL "RESULT_LIB_NAME")
			set(_RESULT_LIB_NAME ${_ARG})
			set(_STATE)
		elseif("${_STATE}" STREQUAL "LIB_DIR")
			list(APPEND _LIB_DIR_LIST ${_ARG})
		elseif("${_STATE}" STREQUAL "LIB_NAME")
			list(APPEND _LIB_NAME_LIST ${_ARG})
		endif()
	endforeach()

	if(NOT _LIB_DIR_LIST)
		set(_OPTIONS)
	else()
		set(_OPTIONS PATHS ${_LIB_DIR_LIST} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
	endif()

	foreach(_LIB_NAME ${_LIB_NAME_LIST})
		axl_find_lib(_LIB_DIR _ACTUAL_LIB_NAME ${_LIB_NAME} ${_OPTIONS})

		if(_LIB_DIR)
			if(_RESULT_LIB_DIR)
				set(${_RESULT_LIB_DIR} ${_LIB_DIR})
			endif()

			if(_RESULT_LIB_NAME)
				set(${_RESULT_LIB_NAME} ${_ACTUAL_LIB_NAME})
			endif()

			break()
		endif()
	endforeach()
endmacro()

macro(
axl_find_lib_dir
	_RESULT
	_LIB_NAME
	# ... _OPTIONS
)	# backward-compatible signature

	axl_find_lib(${_RESULT} _UNUSED ${_LIB_NAME} ${ARGN})
endmacro()

macro(
axl_find_lib
	_RESULT_LIB_DIR
	_RESULT_LIB_NAME
	_LIB_NAME
	# ... _OPTIONS
)

	unset(_PATH)
	find_library(_PATH ${_LIB_NAME} ${ARGN})

	if(NOT _PATH)
		set(${_RESULT_LIB_DIR} ${_RESULT_LIB_DIR}-NOTFOUND)
		set(${_RESULT_LIB_NAME} ${_RESULT_LIB_NAME}-NOTFOUND)
	else()
		if(WIN32)
			string(REPLACE "\\" "/" _PATH "${_PATH}")
		endif()

		get_filename_component(${_RESULT_LIB_DIR} ${_PATH} DIRECTORY)
		get_filename_component(${_RESULT_LIB_NAME} ${_PATH} NAME)
	endif()

	unset(_PATH CACHE)
endmacro()

macro(
axl_find_file
	_RESULT
	_FILE_NAME
	# ...
)

	set(_DIR_LIST ${ARGN})

	set(_FILE_PATH ${_FILE_NAME}-NOTFOUND)

	foreach(_DIR ${_DIR_LIST})
		if(EXISTS ${_DIR}/${_FILE_NAME})
			set(_FILE_PATH ${_DIR}/${_FILE_NAME})
			break()
		endif()
	endforeach()

	set(${_RESULT} ${_FILE_PATH})
endmacro()

macro(
axl_find_file_recurse_parent_dirs
	_RESULT
	_FILE_NAME
	_START_DIR
)

	set(_DIR ${_START_DIR})

	while(TRUE)
		if(EXISTS ${_DIR}/${_FILE_NAME})
			set(_FILE_PATH ${_DIR}/${_FILE_NAME})
			break()
		endif()

		get_filename_component(_PARENT_DIR "${_DIR}/.." ABSOLUTE)

		if(${_DIR} STREQUAL ${_PARENT_DIR})
			set(_FILE_PATH ${_FILE_NAME}-NOTFOUND)
			break()
		endif()

		set(_DIR ${_PARENT_DIR})
	endwhile()

	set(${_RESULT} ${_FILE_PATH})
endmacro()

macro(
axl_chain_include
	_FILE_NAME
)

	axl_find_file_recurse_parent_dirs(_FILE_PATH ${_FILE_NAME} ${CMAKE_CURRENT_LIST_DIR}/..)

	if(_FILE_PATH)
		include(${_FILE_PATH})
	endif()
endmacro()

macro(
axl_make_path
	_FILE_PATH
	_FILE_NAME
	_BASE_DIR
)

	if(IS_ABSOLUTE ${_FILE_NAME})
		set(${_FILE_PATH} ${_FILE_NAME})
	else()
		set(${_FILE_PATH} "${_BASE_DIR}/${_FILE_NAME}")
	endif()

endmacro()

macro(
axl_does_file_exist_w_configuration
	_RESULT
	_FILE_NAME
)

	set(${_RESULT} FALSE)
	set(_REGEX "\\$\\((Configuration|CONFIGURATION)\\)")

	if(EXISTS ${_FILE_NAME})
		set(${_RESULT} TRUE)
	elseif(${_FILE_NAME} MATCHES ${_REGEX})
		string(REGEX REPLACE ${_REGEX} "Debug" _FILE_NAME_DEBUG ${_FILE_NAME})
		string(REGEX REPLACE ${_REGEX} "Release" _FILE_NAME_RELEASE ${_FILE_NAME})

		if(EXISTS ${_FILE_NAME_DEBUG} OR EXISTS ${_FILE_NAME_RELEASE})
			set(${_RESULT} TRUE)
		endif()
	endif()
endmacro()

macro(
axl_ensure_file_exists
	_FILE_PATH
	_TEMPLATE_FILE_PATH
)
	if(NOT EXISTS ${_FILE_PATH})
		file(READ ${_TEMPLATE_FILE_PATH} _BUFFER)
		file(WRITE ${_FILE_PATH} ${_BUFFER})
	endif()
endmacro()

macro(
axl_configure_file_w_permissions
	_SRC_FILE
	_DST_FILE
	# ...
)

	set(_PERMISSION_LIST ${ARGN})
	set(_TMP_FILE ${CMAKE_CURRENT_BINARY_DIR}/tmp/${_DST_FILE})

	configure_file(${_SRC_FILE} ${_TMP_FILE})

	file(
		COPY ${_TMP_FILE}
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
		FILE_PERMISSIONS ${_PERMISSION_LIST}
	)

	file(REMOVE ${_TMP_FILE})
endmacro()

macro(
axl_enum_directories
	_RESULT
	_DIR
)

	file(GLOB _FILE_LIST RELATIVE ${_DIR} ${_DIR}/*)
	set(_DIR_LIST)

	foreach(_FILE ${_FILE_LIST})
		if(IS_DIRECTORY ${_DIR}/${_FILE})
			LIST(APPEND _DIR_LIST ${_FILE})
		endif()
	endforeach()

	set(${_RESULT} ${_DIR_LIST})
endmacro()

macro(
axl_touch_if_changed
	_FILE
	# ...
)
	set(_DEPENDENCY_LIST ${ARGN})

	add_custom_command(
		OUTPUT ${_FILE}
		COMMAND ${CMAKE_COMMAND} -E touch ${_FILE}
		DEPENDS ${_DEPENDENCY_LIST}
	)
endmacro()

#...............................................................................

# imports -- CMake' find_package replacement with support for manual override

macro(
axl_include_import_file
	_IMPORT
	_IS_OPTIONAL
)

	set(${_IMPORT}_FOUND FALSE)
	unset(_IMPORT_FILE_PATH)

	axl_find_file(
		_IMPORT_FILE_PATH
		import_${_IMPORT}.cmake
		${AXL_IMPORT_DIR_LIST} # AXL_IMPORT_DIR_LIST first (to allow overriding)
		${CMAKE_CURRENT_LIST_DIR}
	)

	if(_IMPORT_FILE_PATH)
		include(${_IMPORT_FILE_PATH})

		string(TOUPPER ${_IMPORT} _IMPORT_UC)
		if(NOT ${_IMPORT_UC}_FOUND)
			string(TOUPPER ${_IMPORT} _FILTER)
			axl_filter_list(_FILTERED_PATH_LIST ${_FILTER} ${AXL_PATH_LIST})

			if(_FILTERED_PATH_LIST)
				list(LENGTH _FILTERED_PATH_LIST _LENGTH)
				if(${_LENGTH} LESS 2)
					set(_FILTERED_PATH_LIST " ${_FILTERED_PATH_LIST} in")
				else()
					set(_FILTERED_PATH_LIST " the following variables:;${_FILTERED_PATH_LIST} in")
				endif()
			endif()

			if(_IS_OPTIONAL)
				axl_message("${_IMPORT}:" "NOT FOUND, adjust${_FILTERED_PATH_LIST} paths.cmake (optional)")
			else()
				message(FATAL_ERROR "${_IMPORT} is REQUIRED but NOT FOUND, adjust${_FILTERED_PATH_LIST} paths.cmake")
			endif()
		endif()
	elseif(_IS_OPTIONAL)
		axl_message("${_IMPORT}:" "NOT FOUND, missing import_${_IMPORT}.cmake, adjust AXL_IMPORT_DIR_LIST (optional)")
	else()
		message(FATAL_ERROR "${_IMPORT} is REQUIRED but NOT FOUND, missing import_${_IMPORT}.cmake, adjust AXL_IMPORT_DIR_LIST")
	endif()
endmacro()

macro(
axl_import
	#...
)

	set(_IMPORT_LIST ${ARGN})
	set(_IS_OPTIONAL FALSE)

	foreach(_IMPORT ${_IMPORT_LIST})
		if("${_IMPORT}" STREQUAL "REQUIRED")
			set(_IS_OPTIONAL FALSE)
		elseif("${_IMPORT}" STREQUAL "OPTIONAL")
			set(_IS_OPTIONAL TRUE)
		else()
			axl_include_import_file(
				${_IMPORT}
				_IS_OPTIONAL
			)
		endif()
	endforeach()
endmacro()

#...............................................................................

macro(
axl_install_redirected_includes
	_TARGET_DIR
	# ...
)

	set(_H_LIST ${ARGN})
	set(_CUR_SUB_DIR)
	set(_MAIN_H_LIST)
	set(_REDIR_H_LIST)

	foreach(_FILE ${_H_LIST})
		if("${_FILE}" MATCHES ".+/[^/]+/[^/]+\\.h")
			string(REGEX REPLACE ".+/([^/]+)/[^/]+\\.h" "\\1" _SUB_DIR ${_FILE})
			string(REGEX REPLACE "(.+)/([^/]+)/([^/]+\\.h)" "\\1/\\3" _REDIR_H ${_FILE})

			if(NOT "${_SUB_DIR}" STREQUAL "${_CUR_SUB_DIR}")
				if(_MAIN_H_LIST)
					install(
						FILES ${_MAIN_H_LIST}
						DESTINATION ${_TARGET_DIR}/${_CUR_SUB_DIR}
					)

					install(
						FILES ${_REDIR_H_LIST}
						DESTINATION ${_TARGET_DIR}
					)
				endif()

				set(_CUR_SUB_DIR ${_SUB_DIR})
				set(_MAIN_H_LIST)
				set(_REDIR_H_LIST)
			endif()

			list(APPEND _MAIN_H_LIST ${_FILE})
			list(APPEND _REDIR_H_LIST ${_REDIR_H})
		endif()
	endforeach()

	install(
		FILES ${_MAIN_H_LIST}
		DESTINATION ${_TARGET_DIR}/${_CUR_SUB_DIR}
	)

	install(
		FILES ${_REDIR_H_LIST}
		DESTINATION ${_TARGET_DIR}
	)
endmacro()

macro(
axl_replace_configuration_for_install
	_DST_LIST
	# ...
)

	set(_SRC_LIST ${ARGN})
	set(${_DST_LIST})

	foreach(_SRC ${_SRC_LIST})
		string(REPLACE "$(Configuration)" "$<CONFIGURATION>" _DST ${_SRC})
		list(APPEND ${_DST_LIST} ${_DST})
	endforeach()
endmacro()

#...............................................................................

macro(
axl_add_twin_library
	_NEW_TARGET
	_OLD_TARGET
	_EXCLUDE_PATTERN
)

	if("${_EXCLUDE_PATTERN}" STREQUAL "")
		set(_EXCLUDE_PATTERN "^$")
	endif()

	# create a new twin library target

	get_target_property(_SOURCE_DIR ${_OLD_TARGET} SOURCE_DIR)
	get_target_property(_SOURCES ${_OLD_TARGET} SOURCES)

	set(_SOURCE_PATH_LIST)

	foreach(_SOURCE ${_SOURCES})
		if(NOT IS_ABSOLUTE ${_SOURCE})
			set(_SOURCE ${_SOURCE_DIR}/${_SOURCE})
		endif()

		if(${_SOURCE} MATCHES ${_EXCLUDE_PATTERN})
			axl_exclude_from_build(${_SOURCE})
		endif()

		list(
			APPEND
			_SOURCE_PATH_LIST
			${_SOURCE}
		)
	endforeach()

	add_library(
		${_NEW_TARGET}
		${_SOURCE_PATH_LIST}
	)

	get_target_property(
		_PCH_LIST
		${_OLD_TARGET}
		PRECOMPILE_HEADERS
	)

	target_precompile_headers(
		${_NEW_TARGET}
		PRIVATE
		${_PCH_LIST}
	)
endmacro()

#...............................................................................

macro(
axl_copy_dlls_for_target
	_TARGET
	# ...
)

	set(_DLL_LIST ${ARGN})
	if (_DLL_LIST)
		add_custom_command(
			TARGET ${_TARGET}
			POST_BUILD
			COMMAND
				${CMAKE_COMMAND} -E copy
				${_DLL_LIST}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
			COMMENT "Copying DLLs for ${_TARGET}..."
			VERBATIM
		)
	endif()
endmacro()

macro(
axl_copy_msvc_asan_dll)

	if (MSVC_ASAN_DLL)
		get_filename_component(_NAME "${MSVC_ASAN_DLL}" NAME)

		add_custom_target(
			axl_msvc_asan
			ALL
			COMMAND
				${CMAKE_COMMAND} -E copy_if_different
				${MSVC_ASAN_DLL}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${_NAME}
			COMMENT "Copying ASAN runtime DLL..."
			VERBATIM
		)
	endif()
endmacro()

#...............................................................................
