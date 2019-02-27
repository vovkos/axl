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

set(
	_LIBUSB_LIB_NAME_LIST
	usb-1.0
	libusb-1.0
	)

if(NOT LIBUSB_INC_DIR)
	axl_find_inc_dir(LIBUSB_INC_DIR libusb-1.0/libusb.h)
endif()

if(NOT LIBUSB_LIB_DIR)
	if(NOT LIBUSB_LIB_NAME)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR LIBUSB_LIB_DIR
			RESULT_LIB_NAME LIBUSB_LIB_NAME
			LIB_NAME ${_LIBUSB_LIB_NAME_LIST}
			)
	else()
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR LIBUSB_LIB_DIR
			LIB_NAME ${LIBUSB_LIB_NAME}
			)
	endif()
elseif(NOT LIBUSB_LIB_NAME)
	axl_find_lib_dir_ex(
		RESULT_LIB_NAME LIBUSB_LIB_NAME
		LIB_DIR ${LIBUSB_LIB_DIR}
		LIB_NAME ${_LIBUSB_LIB_NAME_LIST}
		)
endif()

if(EXISTS ${LIBUSB_INC_DIR}/libusb-1.0/libusb.h)
	axl_message("LibUSB paths:")
	axl_message("    Includes:"  "${LIBUSB_INC_DIR}")
	axl_message("    Libraries:" "${LIBUSB_LIB_DIR}")

	if(LIBUSB_DLL_DIR)
		axl_message("    DLLs:" "${LIBUSB_DLL_DIR}")
	endif()

	axl_message("    Library name:" "${LIBUSB_LIB_NAME}")

	set(LIBUSB_FOUND TRUE)
else()
	set(LIBUSB_FOUND FALSE)
endif()

#...............................................................................
