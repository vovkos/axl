#..............................................................................

if (NOT LUA_INC_DIR)
	axl_find_inc_dir (LUA_INC_DIR lua.h)
endif ()

if (NOT LUA_LIB_DIR)
	axl_find_lib_dir (LUA_LIB_DIR lua)
endif ()

if (EXISTS ${LUA_INC_DIR}/lua.h)
	message (STATUS "Lua paths:")
	message (STATUS "    Lua includes:  ${LUA_INC_DIR}")
	message (STATUS "    Lua libraries: ${LUA_LIB_DIR}")

	set (LUA_FOUND TRUE)
else ()
	set (LUA_FOUND FALSE)
endif ()

#..............................................................................
