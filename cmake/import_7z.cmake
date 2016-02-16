#..............................................................................

if ("${7Z_EXE}" STREQUAL "")
	set (7Z_FOUND FALSE)
	message (STATUS "7Zip:                       <not-found>")
else ()
	set (7Z_FOUND TRUE)
	message (STATUS "Path to 7Zip executable:    ${7Z_EXE}")
endif ()

#..............................................................................
