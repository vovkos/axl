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

if(NOT BOOST_INC_DIR)
	axl_find_inc_dir(BOOST_INC_DIR boost/version.hpp)
endif()

if(EXISTS ${BOOST_INC_DIR}/boost/version.hpp)
	axl_message("Boost paths:")
	axl_message("    Includes:"  "${BOOST_INC_DIR}")

	set(BOOST_FOUND TRUE)
else()
	set(BOOST_FOUND FALSE)
endif()

#...............................................................................
