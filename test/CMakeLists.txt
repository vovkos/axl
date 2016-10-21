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

option (
	BUILD_TEST_CON
	"Build test_con"
	OFF
	)

if (BUILD_TEST_CON)
	add_subdirectory (test_con)
endif ()

#...............................................................................

if (${QT_FOUND})
	option (
		BUILD_TEST_QT
		"Build test_qt"
		ON
		)

	if (BUILD_TEST_QT)
		add_subdirectory (test_qt)
	endif ()
endif ()

#...............................................................................

add_subdirectory (test_mini)

#...............................................................................
