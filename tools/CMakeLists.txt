# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

if (${OPENSSL_FOUND})
	option (
		BUILD_TOOL_ECC_KEY
		"Build ecc-key tool"
		ON
		)

	if (BUILD_TOOL_ECC_KEY)
		add_subdirectory (ecc-key)
	endif ()
endif ()

#..............................................................................
