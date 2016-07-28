#..............................................................................

if (NOT PCAP_INC_DIR)
	axl_find_inc_dir (PCAP_INC_DIR pcap/pcap.h)
endif ()

if (NOT PCAP_LIB_DIR)
	axl_find_lib_dir (PCAP_LIB_DIR pcap)
endif ()

if (EXISTS ${PCAP_INC_DIR}/pcap/pcap.h)
	message (STATUS "Pcap paths:")
	message (STATUS "    Pcap includes:  ${PCAP_INC_DIR}")
	message (STATUS "    Pcap libraries: ${PCAP_LIB_DIR}")

	set (PCAP_FOUND TRUE)
else ()
	set (PCAP_FOUND FALSE)
endif ()

#..............................................................................
