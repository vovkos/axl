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

if(NOT GIT_EXE)
	axl_find_executable(GIT_EXE git)

	if(GIT_EXE)
		axl_message("Git found at:" "${GIT_EXE}")
	endif()
endif()

if(EXISTS ${GIT_EXE})
	set(GIT_FOUND TRUE)
else()
	set(GIT_FOUND FALSE)
endif()

#...............................................................................
