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

if(NOT PDFLATEX_EXE)
	axl_find_executable(PDFLATEX_EXE pdflatex)

	if(PDFLATEX_EXE)
		axl_message("Pdflatex found at:" "${PDFLATEX_EXE}")
	endif()
endif()

if(EXISTS ${PDFLATEX_EXE})
	set(LATEX_FOUND TRUE)
else()
	set(LATEX_FOUND FALSE)
endif()

#...............................................................................
