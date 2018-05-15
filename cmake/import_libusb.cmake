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

if (NOT LIBUSB_INC_DIR)
	axl_find_inc_dir (LIBUSB_INC_DIR libusb-1.0/libusb.h)
endif ()

if (NOT LIBUSB_LIB_DIR)
	axl_find_lib_dir (LIBUSB_LIB_DIR usb-1.0)
endif ()

if (EXISTS ${LIBUSB_INC_DIR}/libusb-1.0/libusb.h)
	axl_message ("LibUSB paths:")
	axl_message ("    Includes:"  "${LIBUSB_INC_DIR}")
	axl_message ("    Libraries:" "${LIBUSB_LIB_DIR}")

	if (LIBUSB_DLL_DIR)
		axl_message ("    DLLs:" "${LIBUSB_DLL_DIR}")
	endif ()

	set (LIBUSB_FOUND TRUE)
else ()
	set (LIBUSB_FOUND FALSE)
endif ()

#...............................................................................
