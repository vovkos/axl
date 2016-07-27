#..............................................................................

if (NOT GIT_EXE)
	axl_find_executable (GIT_EXE git)

	if (GIT_EXE)
		message (STATUS "Git found at: ${GIT_EXE}")
	endif ()	
endif ()

if (EXISTS ${GIT_EXE})
	set (GIT_FOUND TRUE)
else ()
	set (GIT_FOUND FALSE)
endif ()

#..............................................................................
