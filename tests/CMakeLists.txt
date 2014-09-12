# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

option (
	AXL_BUILD_CON_TEST
	"Build ConTest"
	OFF
	)

if (AXL_BUILD_CON_TEST)
	add_subdirectory (ConTest)
endif ()

#..............................................................................

if (${QT_FOUND})
	option (
		AXL_BUILD_QT_TEST
		"Build QtTest"
		OFF
		)

	if (AXL_BUILD_QT_TEST)
		add_subdirectory (QtTest)
	endif ()
endif ()

#..............................................................................
