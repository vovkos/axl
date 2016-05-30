#..............................................................................

set (PCAP_FOUND FALSE)

if (NOT "${PCAP_INC_DIR}" STREQUAL "")
	if (EXISTS ${PCAP_INC_DIR}/pcap/pcap.h)
		set (PCAP_FOUND TRUE)
	endif ()
elseif (APPLE)
	find_path (
		_DIR
		NAMES pcap.h
		)

	if (_DIR)
		set (PCAP_FOUND TRUE) 
	endif ()
endif ()

#..............................................................................
