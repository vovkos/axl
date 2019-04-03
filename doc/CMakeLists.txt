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

if(SPHINX_FOUND)
	add_subdirectory(index)
	add_subdirectory(build-guide)

	if(DOXYGEN_FOUND AND DOXYREST_FOUND)
		add_subdirectory(manual)
	endif()
endif()

#...............................................................................
