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

set(
	_PYTHON_LIB_NAME_LIST
	python
	python2
	python3
)

if(NOT PYTHON_INC_DIR)
	axl_find_inc_dir(
		PYTHON_INC_DIR
		python.h
	)
endif()

if(NOT PYTHON_LIB_DIR)
	if(PYTHON_LIB_NAME)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR PYTHON_LIB_DIR
			LIB_NAME ${PYTHON_LIB_NAME}
		)
	elseif (NOT APPLE)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR PYTHON_LIB_DIR
			RESULT_LIB_NAME PYTHON_LIB_NAME
			LIB_NAME ${_PYTHON_LIB_NAME_LIST}
		)
	else()
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR PYTHON_LIB_DIR
			RESULT_LIB_NAME PYTHON_LIB_NAME
			RESULT_FRAMEWORK_VERSION PYTHON_FRAMEWORK_VERSION
			LIB_NAME ${_PYTHON_LIB_NAME_LIST}
		)

		if(PYTHON_FRAMEWORK_VERSION)
			set(PYTHON_LIB_NAME "${PYTHON_LIB_DIR}/${PYTHON_LIB_NAME}")
			set(PYTHON_STDLIB "${PYTHON_LIB_NAME}/Versions/Current/lib/python${PYTHON_FRAMEWORK_VERSION}")
			set(PYTHON_EXT_DIR "${PYTHON_STDLIB}/lib-dynload")
			unset(PYTHON_LIB_DIR)
		endif()
	endif()
elseif(NOT PYTHON_LIB_NAME)
	axl_find_lib_dir_ex(
		RESULT_LIB_NAME PYTHON_LIB_NAME
		LIB_DIR ${PYTHON_LIB_DIR}
		LIB_NAME ${_PYTHON_LIB_NAME_LIST}
	)
endif()

if(EXISTS ${PYTHON_INC_DIR}/Python.h AND PYTHON_LIB_NAME)
	if(WIN32)
		set(PYTHON_LIB_NAME_NGE ${PYTHON_LIB_NAME}) # no generator expressions
		set(PYTHON_LIB_NAME ${PYTHON_LIB_NAME}$<$<CONFIG:Debug>:_d>)
	endif()

	axl_message("Python API paths:")
	axl_message("    Includes:"       "${PYTHON_INC_DIR}")

	if(PYTHON_LIB_DIR)
		axl_message("    Libraries:"  "${PYTHON_LIB_DIR}")
	endif()

	if(PYTHON_DLL_DIR)
		axl_message("    DLLs:"       "${PYTHON_DLL_DIR}")
	endif()

	if(PYTHON_EXT_DIR)
		axl_message("    Extensions:" "${PYTHON_EXT_DIR}")
	endif()

	axl_message("    Stdlib:"         "${PYTHON_STDLIB}")
	axl_message("    Library name:"   "${PYTHON_LIB_NAME}")

	set(PYTHON_API_FOUND TRUE)
else()
	set(PYTHON_API_FOUND FALSE)
endif()

#...............................................................................
