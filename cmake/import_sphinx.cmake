#..............................................................................

if (NOT SPHINX_BUILD_EXE)
	axl_find_executable (SPHINX_BUILD_EXE sphinx-build)

	if (SPHINX_BUILD_EXE)
		message (STATUS "Sphinx found at: ${SPHINX_BUILD_EXE}")
	endif ()	
endif ()

if (EXISTS ${SPHINX_BUILD_EXE})
	set (SPHINX_FOUND TRUE)
else ()
	set (SPHINX_FOUND FALSE)
endif ()

#..............................................................................
