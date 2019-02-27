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

if(NOT 7Z_EXE)
	axl_find_executable(7Z_EXE 7z)

	if(7Z_EXE)
		axl_message("7-Zip found at:" "${7Z_EXE}")
	endif()
endif()

if(EXISTS ${7Z_EXE})
	set(7Z_FOUND TRUE)
else()
	set(7Z_FOUND FALSE)
endif()

#...............................................................................
