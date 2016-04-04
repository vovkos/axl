#..............................................................................

set (AXL_VERSION_MAJOR     5)
set (AXL_VERSION_MINOR     5)
set (AXL_VERSION_REVISION  5)
set (AXL_VERSION_TAG)

set (AXL_VERSION_FILE_SUFFIX "${AXL_VERSION_MAJOR}.${AXL_VERSION_MINOR}.${AXL_VERSION_REVISION}")

if (NOT "${AXL_VERSION_TAG}" STREQUAL "")
	set (AXL_VERSION_TAG_SUFFIX " ${AXL_VERSION_TAG}")
	set (AXL_VERSION_FILE_SUFFIX "${AXL_VERSION_FILE_SUFFIX}-${AXL_VERSION_TAG}")
else ()
	set (AXL_VERSION_TAG_SUFFIX)
endif ()

set (AXL_VERSION_COMPANY   "Tibbo Technology Inc")
set (AXL_VERSION_YEARS     "2007-2016")

#..............................................................................
