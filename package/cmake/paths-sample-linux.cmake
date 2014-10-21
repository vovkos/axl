# this file is provided for reference only.
# fill variables with paths on your build machine.
# if library/tool is not needed, leave the path empty.

#..............................................................................

set (LUA_VER  5.2.1)
set (PCAP_VER 1.5.3)
set (QT_VER   5.0.2)

#..............................................................................

if ("${TARGET_CPU}" STREQUAL "amd64")
	set (LUA_INC_DIR      "/home/vladimir/Develop/lua/lua-${LUA_VERSION}/include")
	set (LUA_LIB_DIR      "/home/vladimir/Develop/lua/lua-${LUA_VERSION}/lib")
	set (PCAP_INC_DIR     "/home/vladimir/Develop/libpcap/libpcap-${PCAP_VERSION}/include")
	set (PCAP_LIB_DIR     "/home/vladimir/Develop/libpcap/libpcap-${PCAP_VERSION}/lib")
	set (QT_CMAKE_DIR     "/home/vladimir/Develop/qt/qt-${QT_VERSION}/${QT_VERSION}/gcc_64/lib/cmake")
else ()
	set (LUA_INC_DIR      "/home/vladimir/Develop/lua/lua-${LUA_VERSION}/include")
	set (LUA_LIB_DIR      "/home/vladimir/Develop/lua/lua-${LUA_VERSION}/lib.x86")
	set (PCAP_INC_DIR     "/home/vladimir/Develop/libpcap/libpcap-${PCAP_VERSION}/include")
	set (PCAP_LIB_DIR     "/home/vladimir/Develop/libpcap/libpcap-${PCAP_VERSION}/lib.x86")
	set (QT_CMAKE_DIR     "/home/vladimir/Develop/qt/qt-${QT_VERSION}.x86/${QT_VERSION}/gcc/lib/cmake")
endif ()

set (RAGEL_EXE            "ragel")

#..............................................................................