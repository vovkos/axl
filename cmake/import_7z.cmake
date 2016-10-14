#..............................................................................

if (NOT 7Z_EXE)
	axl_find_executable (7Z_EXE 7z)

	if (7Z_EXE)
		axl_message ("7-Zip found at:" "${7Z_EXE}")
	endif ()
endif ()

if (EXISTS ${7Z_EXE})
	set (7Z_FOUND TRUE)
else ()
	set (7Z_FOUND FALSE)
endif ()

#..............................................................................
