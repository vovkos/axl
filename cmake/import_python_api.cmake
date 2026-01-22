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
	if(NOT PYTHON_LIB_NAME)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR PYTHON_LIB_DIR
			RESULT_LIB_NAME PYTHON_LIB_NAME
			LIB_NAME ${_PYTHON_LIB_NAME_LIST}
		)
	else()
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR PYTHON_LIB_DIR
			LIB_NAME ${PYTHON_LIB_NAME}
		)
	endif()
elseif(NOT PYTHON_LIB_NAME)
	axl_find_lib_dir_ex(
		RESULT_LIB_NAME PYTHON_LIB_NAME
		LIB_DIR ${PYTHON_LIB_DIR}
		LIB_NAME ${_PYTHON_LIB_NAME_LIST}
	)
endif()

if(EXISTS ${PYTHON_INC_DIR}/python.h AND PYTHON_LIB_DIR AND PYTHON_LIB_NAME)
	axl_message("Python API paths:")
	axl_message("    Includes:"  "${PYTHON_INC_DIR}")
	axl_message("    Libraries:" "${PYTHON_LIB_DIR}")

	if(PYTHON_DLL_DIR)
		axl_message("    DLLs:" "${PYTHON_DLL_DIR}")
	endif()

	axl_message("    Library name:" "${PYTHON_LIB_NAME}")

	set(PYTHON_API_FOUND TRUE)
else()
	set(PYTHON_API_FOUND FALSE)
endif()

#...............................................................................
