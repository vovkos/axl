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

if (NOT LUA_INC_DIR)
	axl_find_inc_dir (LUA_INC_DIR lua.h)
endif ()

if (NOT LUA_LIB_DIR)
	axl_find_lib_dir_ex (
		RESULT_LIB_DIR LUA_LIB_DIR
		RESULT_LIB_NAME LUA_LIB_NAME
		LIB_NAME
			lua
			lua53
			lua52
			lua51
		)
elseif (NOT LUA_LIB_NAME)
	axl_find_lib_dir_ex (
		RESULT_LIB_NAME LUA_LIB_NAME
		LIB_DIR ${LUA_LIB_DIR}
		LIB_NAME
			lua
			lua53
			lua52
			lua51
		)
endif ()

if (EXISTS ${LUA_INC_DIR}/lua.h AND LUA_LIB_NAME)
	axl_message ("Lua paths:")
	axl_message ("    Includes:"  "${LUA_INC_DIR}")
	axl_message ("    Library dir:" "${LUA_LIB_DIR}")
	axl_message ("    Library name:" "${LUA_LIB_NAME}")

	set (LUA_FOUND TRUE)
else ()
	set (LUA_FOUND FALSE)
endif ()

#...............................................................................
