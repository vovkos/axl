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

if(NOT SPHINX_BUILD_EXE)
	axl_find_executable(SPHINX_BUILD_EXE sphinx-build)

	if(SPHINX_BUILD_EXE)
		axl_message("Sphinx found at:" "${SPHINX_BUILD_EXE}")
	endif()
endif()

if(EXISTS ${SPHINX_BUILD_EXE})
	set(SPHINX_FOUND TRUE)
else()
	set(SPHINX_FOUND FALSE)
endif()

#...............................................................................
