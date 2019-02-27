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

if(NOT DOXYGEN_EXE)
	axl_find_executable(DOXYGEN_EXE doxygen)

	if(DOXYGEN_EXE)
		axl_message("Doxygen found at:" "${DOXYGEN_EXE}")
	endif()
endif()

if(EXISTS ${DOXYGEN_EXE})
	set(DOXYGEN_FOUND TRUE)
else()
	set(DOXYGEN_FOUND FALSE)
endif()

#...............................................................................
