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

if(NOT NASM_EXE)
	axl_find_executable(NASM_EXE nasm)

	if(NASM_EXE)
		axl_message("NASM found at:" "${NASM_EXE}")
	endif()
endif()

if(EXISTS ${NASM_EXE})
	set(NASM_FOUND TRUE)
else()
	set(NASM_FOUND FALSE)
endif()

#...............................................................................
