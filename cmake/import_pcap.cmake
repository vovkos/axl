#..............................................................................

if (NOT "${PCAP_INC_DIR}" STREQUAL "")
	set (PCAP_FOUND TRUE)
	message (STATUS "Path to PCap includes:      ${PCAP_INC_DIR}")
	message (STATUS "Path to PCap libraries:     ${PCAP_LIB_DIR}")
elseif (APPLE)
	find_path (
		_DIR
		NAMES pcap.h
		)

	if (_DIR)
		set (PCAP_FOUND TRUE) 
		message (STATUS "Path to PCap includes:      ${_DIR}")
	else ()
		set (PCAP_FOUND FALSE)
		message (STATUS "PCap:                       <not-found>")
	endif ()
else ()
	set (PCAP_FOUND FALSE)
	message (STATUS "PCap:                       <not-found>")
endif ()

#..............................................................................
