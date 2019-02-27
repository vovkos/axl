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

if(NOT PCAP_INC_DIR)
	axl_find_inc_dir(PCAP_INC_DIR pcap/pcap.h)
endif()

if(NOT PCAP_LIB_DIR)
	axl_find_lib_dir(PCAP_LIB_DIR pcap)
endif()

if(EXISTS ${PCAP_INC_DIR}/pcap/pcap.h)
	axl_message("Pcap paths:")
	axl_message("    Includes:"  "${PCAP_INC_DIR}")
	axl_message("    Libraries:" "${PCAP_LIB_DIR}")

	if(PCAP_DLL_DIR)
		axl_message("    DLLs:" "${PCAP_DLL_DIR}")
	endif()

	set(PCAP_FOUND TRUE)
else()
	set(PCAP_FOUND FALSE)
endif()

#...............................................................................
