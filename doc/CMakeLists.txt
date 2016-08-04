# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

if (DOXYGEN_FOUND)
	add_subdirectory (doxygen)
endif ()

if (SPHINX_FOUND)
	add_subdirectory (manual)
endif ()

#..............................................................................
