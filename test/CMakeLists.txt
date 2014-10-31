# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

option (
	BUILD_TEST_CON
	"Build test_con"
	OFF
	)

if (BUILD_TEST_CON)
	add_subdirectory (test_con)
endif ()

#..............................................................................

if (${QT_FOUND})
	option (
		BUILD_TEST_QT
		"Build test_qt"
		OFF
		)

	if (BUILD_TEST_QT)
		add_subdirectory (test_qt)
	endif ()
endif ()

#..............................................................................
