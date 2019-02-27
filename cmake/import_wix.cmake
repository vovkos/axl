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

if(EXISTS ${WIX_BIN_DIR}/candle.exe)
	set(WIX_FOUND TRUE)
else()
	set(WIX_FOUND FALSE)
endif()

#...............................................................................
