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

set(
	_LUA_LIB_NAME_LIST
	lua
	lua5
	lua53
	lua5.3
	lua52
	lua5.2
	lua51
	lua5.1
	)

if(NOT LUA_INC_DIR)
	axl_find_inc_dir(
		LUA_INC_DIR
		lua.h
		PATH_SUFFIXES . ${_LUA_LIB_NAME_LIST}
		)
endif()

if(NOT LUA_LIB_DIR)
	if(NOT LUA_LIB_NAME)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR LUA_LIB_DIR
			RESULT_LIB_NAME LUA_LIB_NAME
			LIB_NAME ${_LUA_LIB_NAME_LIST}
			)
	else()
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR LUA_LIB_DIR
			LIB_NAME ${LUA_LIB_NAME}
			)
	endif()
elseif(NOT LUA_LIB_NAME)
	axl_find_lib_dir_ex(
		RESULT_LIB_NAME LUA_LIB_NAME
		LIB_DIR ${LUA_LIB_DIR}
		LIB_NAME ${_LUA_LIB_NAME_LIST}
		)
endif()

if(EXISTS ${LUA_INC_DIR}/lua.h AND LUA_LIB_DIR AND LUA_LIB_NAME)
	axl_message("Lua paths:")
	axl_message("    Includes:"  "${LUA_INC_DIR}")
	axl_message("    Libraries:" "${LUA_LIB_DIR}")

	if(LUA_DLL_DIR)
		axl_message("    DLLs:" "${LUA_DLL_DIR}")
	endif()

	axl_message("    Library name:" "${LUA_LIB_NAME}")

	set(LUA_FOUND TRUE)
else()
	set(LUA_FOUND FALSE)
endif()

#...............................................................................
