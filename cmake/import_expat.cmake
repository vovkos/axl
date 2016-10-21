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

if (NOT EXPAT_INC_DIR)
	axl_find_inc_dir (EXPAT_INC_DIR expat.h)
endif ()

if (NOT EXPAT_LIB_DIR)
	axl_find_lib_dir (EXPAT_LIB_DIR expat)
endif ()

if (EXISTS ${EXPAT_INC_DIR}/expat.h)
	message (STATUS "Expat paths:")
	axl_message ("    Includes:"  "${EXPAT_INC_DIR}")
	axl_message ("    Libraries:" "${EXPAT_LIB_DIR}")

	set (EXPAT_FOUND TRUE)
else ()
	set (EXPAT_FOUND FALSE)
endif ()

#...............................................................................
