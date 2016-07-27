#..............................................................................

if (NOT LIBUSB_INC_DIR)
	axl_find_inc_dir (LIBUSB_INC_DIR libusb.h)
endif ()

if (NOT LIBUSB_LIB_DIR)
	axl_find_lib_dir (LIBUSB_LIB_DIR usb)
endif ()

if (EXISTS ${LIBUSB_INC_DIR}/libusb.h)
	message (STATUS "LibUSB paths:")
	message (STATUS "    LibUSB includes:  ${LIBUSB_INC_DIR}")
	message (STATUS "    LibUSB libraries: ${LIBUSB_LIB_DIR}")

	set (LIBUSB_FOUND TRUE)
else ()
	set (LIBUSB_FOUND FALSE)
endif ()

#..............................................................................
