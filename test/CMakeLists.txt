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

option(
	BUILD_AXL_TESTS
	"Build AXL tests"
	ON
)

if(NOT BUILD_AXL_TESTS)
	return()
endif()

option(
	BUILD_AXL_TEST_AUTO
	"Build axl_test_auto(misc automated tests - smoke, unit, integration etc)"
	ON
)

if(BUILD_AXL_TEST_AUTO)
	add_subdirectory(axl_test_auto)
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

if(${QT_FOUND})
	option(
		BUILD_AXL_TEST_QT
		"Build axl_test_qt(QT GUI general-purpose testig app)"
		ON
	)

	if(BUILD_AXL_TEST_QT)
		add_subdirectory(axl_test_qt)
	endif()
endif()

# console general-purpose testing app is always there

add_subdirectory(axl_test_con)

#...............................................................................
