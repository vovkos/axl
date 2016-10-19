#...............................................................................

if (NOT PDFLATEX_EXE)
	axl_find_executable (PDFLATEX_EXE pdflatex)

	if (PDFLATEX_EXE)
		axl_message ("Pdflatex found at:" "${PDFLATEX_EXE}")
	endif ()
endif ()

if (EXISTS ${PDFLATEX_EXE})
	set (LATEX_FOUND TRUE)
else ()
	set (LATEX_FOUND FALSE)
endif ()

#...............................................................................
