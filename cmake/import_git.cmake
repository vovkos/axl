#..............................................................................

if ("${GIT_EXE}" STREQUAL "")
	set (GIT_FOUND FALSE)
	message (STATUS "Git:                        <not-found>")
else ()
	set (GIT_FOUND TRUE)
	message (STATUS "Path to Git executable:     ${GIT_EXE}")
endif ()

#..............................................................................
