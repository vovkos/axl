#..............................................................................

if ("${LIBUSB_INC_DIR}" STREQUAL "")
	set (LIBUSB_FOUND FALSE)
	message (STATUS "LibUSB:                     <not-found>")
else ()
	set (LIBUSB_FOUND TRUE)
	message (STATUS "Path to LibUSB includes:    ${LIBUSB_INC_DIR}")
	message (STATUS "Path to LibUSB libraries:   ${LIBUSB_LIB_DIR}")
endif ()

#..............................................................................
