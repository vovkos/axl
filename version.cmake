#...............................................................................

set (AXL_VERSION_MAJOR     5)
set (AXL_VERSION_MINOR     5)
set (AXL_VERSION_REVISION  6)
set (AXL_VERSION_TAG)

set (AXL_VERSION_FILE_SUFFIX "${AXL_VERSION_MAJOR}.${AXL_VERSION_MINOR}.${AXL_VERSION_REVISION}")

if (NOT "${AXL_VERSION_TAG}" STREQUAL "")
	set (AXL_VERSION_TAG_SUFFIX " ${AXL_VERSION_TAG}")
	set (AXL_VERSION_FILE_SUFFIX "${AXL_VERSION_FILE_SUFFIX}-${AXL_VERSION_TAG}")
else ()
	set (AXL_VERSION_TAG_SUFFIX)
endif ()

string (TIMESTAMP AXL_VERSION_YEAR  "%Y")
string (TIMESTAMP AXL_VERSION_MONTH "%m")
string (TIMESTAMP AXL_VERSION_DAY   "%d")

set (AXL_VERSION_COMPANY   "Tibbo Technology Inc")
set (AXL_VERSION_YEARS     "2007-${AXL_VERSION_YEAR}")

#...............................................................................
