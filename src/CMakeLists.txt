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

if(LINUX)
	if(${LIBUDEV_FOUND})
		option(
			BUILD_AXL_SYS_LNX_LIBUDEV
			"Build LibUdev wrappers for axl_core library"
			ON
			)

		option(
			BUILD_AXL_SYS_LNX_LIBUDEV_NEW_API
			"Build wrappers for new functions in LibUdev(e.g. udev_device_get_sysattr_value)"
			OFF
			)

		if(BUILD_AXL_SYS_LNX_LIBUDEV)
			add_definitions(-D_AXL_SYS_LNX_LIBUDEV=1)
		endif()
	else()
		unset(BUILD_AXL_SYS_LNX_LIBUDEV CACHE)
	endif()
endif()

add_subdirectory(axl_core)

if (GCC_LINK_GLIBC_WRAPPERS)
	add_subdirectory(axl_glibc)
endif()

#...............................................................................

if(WIN32)
	option(
		BUILD_AXL_GUI_GDI
		"Build GDI drivers for axl_gui library"
		OFF
		)
endif()

if(QT_FOUND)
	option(
		BUILD_AXL_GUI_QT
		"Build QT drivers for axl_gui library"
		ON
		)
else()
	unset(BUILD_AXL_GUI_QT CACHE)
endif()

option(
	BUILD_AXL_GUI
	"Build axl_gui library"
	ON
	)

if(BUILD_AXL_GUI)
	add_subdirectory(axl_gui)
endif()

#...............................................................................

if(RAGEL_FOUND)
	option(
		BUILD_AXL_INI
		"Build axl_ini library"
		ON
		)
else()
	unset(BUILD_AXL_INI CACHE)
endif()

if(BUILD_AXL_INI)
	add_subdirectory(axl_ini)
endif()

#...............................................................................

if(RAGEL_FOUND)
	option(
		BUILD_AXL_DOX
		"Build axl_dox library (Doxygen comment processor)"
		ON
		)
else()
	unset(BUILD_AXL_DOX CACHE)
endif()

if(BUILD_AXL_DOX)
	add_subdirectory(axl_dox)
endif()

#...............................................................................

if(EXPAT_FOUND)
	option(
		BUILD_AXL_XML
		"Build axl_xml library (Expat XML Parser wrappers)"
		ON
		)
else()
	unset(BUILD_AXL_XML CACHE)
endif()

if(BUILD_AXL_XML)
	add_subdirectory(axl_xml)
endif()

#...............................................................................

option(
	BUILD_AXL_FSM
	"Build axl_fsm library (finite state machines)"
	ON
	)

if(BUILD_AXL_FSM)
	add_subdirectory(axl_fsm)
endif()

#...............................................................................

option(
	BUILD_AXL_IO
	"Build axl_io library"
	ON
	)

if(BUILD_AXL_IO)
	add_subdirectory(axl_io)
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

if(PCAP_FOUND)
	option(
		BUILD_AXL_IO_PCAP
		"Build axl_io_pcap library (Pcap wrappers)"
		ON
		)
else()
	unset(BUILD_AXL_IO_PCAP CACHE)
endif()

if(BUILD_AXL_IO_PCAP)
	add_subdirectory(axl_io_pcap)
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

if(LIBUSB_FOUND)
	option(
		BUILD_AXL_IO_USB
		"Build axl_io library (LibUSB wrappers)"
		ON
		)
else()
	unset(BUILD_AXL_IO_USB CACHE)
endif()

if(BUILD_AXL_IO_USB)
	add_subdirectory(axl_io_usb)
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

if(OPENSSL_FOUND)
	option(
		BUILD_AXL_IO_SSL
		"Build axl_io_ssl library (OpenSSL libssl wrappers)"
		ON
		)
else()
	unset(BUILD_AXL_IO_SSL CACHE)
endif()

if(BUILD_AXL_IO_SSL)
	add_subdirectory(axl_io_ssl)
endif()

#...............................................................................

option(
	BUILD_AXL_LEX
	"Build axl_lex library"
	ON
	)

if(BUILD_AXL_LEX)
	add_subdirectory(axl_lex)
endif()

#...............................................................................

if(LUA_FOUND)
	option(
		BUILD_AXL_LUA
		"Build axl_lua library"
		ON
		)
else()
	unset(BUILD_AXL_LUA CACHE)
endif()

if(BUILD_AXL_LUA)
	add_subdirectory(axl_lua)
endif()

#...............................................................................

if(RAGEL_FOUND AND LUA_FOUND)
	option(
		BUILD_AXL_ST
		"Build axl_st library (string templates)"
		ON
		)
else()
	unset(BUILD_AXL_ST CACHE)
endif()

if(BUILD_AXL_ST)
	add_subdirectory(axl_st)
endif()

#...............................................................................

option(
	BUILD_AXL_ZIP
	"Build axl_zip library (miniz wrapper)"
	ON
	)

if(BUILD_AXL_ZIP)
	add_subdirectory(axl_zip)
endif()

#...............................................................................

if(OPENSSL_FOUND)
	option(
		BUILD_AXL_CRY
		"Build axl_cry library (OpenSSL libcrypto wrappers)"
		ON
		)
else()
	unset(BUILD_AXL_CRY CACHE)
endif()

if(BUILD_AXL_CRY)
	add_subdirectory(axl_cry)
endif()

#...............................................................................

if(APPLE)
	option(
		BUILD_AXL_CF
		"Build axl_cf library (Core Foundation Framework wrappers)"
		ON
		)

	option(
		BUILD_AXL_IOK
		"Build axl_iok library (IOKit Framework wrappers)"
		ON
		)
endif()

if(BUILD_AXL_CF)
	add_subdirectory(axl_cf)
endif()

if(BUILD_AXL_IOK)
	add_subdirectory(axl_iok)
endif()

#...............................................................................

if("${TARGET_CPU}" MATCHES "x86|amd64")
	option(
		BUILD_AXL_SPY
		"Build axl_spy library (API spying engine)"
		ON
		)
endif()

if(BUILD_AXL_SPY)
	add_subdirectory(axl_spy)
endif()

#...............................................................................

if(MSVC)
	set(
		AXL_LIBCMT_TWIN_LIB_LIST
		"axl_core;axl_io"
		CACHE STRING
		"List of AXL libraries to build LIBCMT twins for"
		)

	add_subdirectory(axl_libcmt)
endif()

#...............................................................................
