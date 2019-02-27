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

unset(_DIR)

find_path(
	_DIR
	NAMES libudev.h
	)

if(_DIR)
	set(LIBUDEV_FOUND TRUE)
else()
	set(LIBUDEV_FOUND FALSE)
endif()

unset(_DIR CACHE)

#...............................................................................
