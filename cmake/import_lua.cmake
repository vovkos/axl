#..............................................................................

if ("${LUA_INC_DIR}" STREQUAL "")
	set (LUA_FOUND FALSE)
	message (STATUS "Lua:                        <not-found>")
else ()
	set (LUA_FOUND TRUE)
	message (STATUS "Path to Lua includes:       ${LUA_INC_DIR}")
	message (STATUS "Path to Lua libraries:      ${LUA_LIB_DIR}")
endif ()

#..............................................................................
