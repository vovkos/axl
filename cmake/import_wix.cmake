#..............................................................................

if ("${WIX_BIN_DIR}" STREQUAL "")
	set (WIX_FOUND FALSE)
	message (STATUS "WIX:                        <not-found>")
else ()
	set (WIX_FOUND TRUE)
	message (STATUS "Path to WIX binaries:       ${WIX_BIN_DIR}")
endif ()

#..............................................................................
