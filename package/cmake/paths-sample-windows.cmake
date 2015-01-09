# this file is provided for reference only.
# fill variables with paths on your build machine.
# if library/tool is not needed, leave the path empty.

#..............................................................................

set (LUA_VERSION     5.2.1)
set (WINPCAP_VERSION 4.1.2)
set (LIBUSB_VERSION  1.0.9)
set (OPENSSL_VERSION 1.0.1h)
set (QT_VERSION      5.0.2)
set (RAGEL_VERSION   6.7)

#..............................................................................

if ("${TARGET_CPU}" STREQUAL "amd64")
	set (LUA_INC_DIR      "c:/Develop/lua/lua-${LUA_VERSION}/include")
	set (LUA_LIB_DIR      "c:/Develop/lua/lua-${LUA_VERSION}/lib/amd64/${CONFIGURATION_SUFFIX}")
	set (PCAP_INC_DIR     "c:/Develop/winpcap/winpcap-${WINPCAP_VERSION}/include")
	set (PCAP_LIB_DIR     "c:/Develop/winpcap/winpcap-${WINPCAP_VERSION}/lib/x64")
	set (LIBUSB_INC_DIR   "c:/Develop/libusb/libusb-${LIBUSB_VERSION}/libusb")
	set (LIBUSB_LIB_DIR   "c:/Develop/libusb/libusb-${LIBUSB_VERSION}/Win32/Debug/lib")
	set (OPENSSL_INC_DIR  "c:/Develop/openssl/openssl-win64-${OPENSSL_VERSION}/include")
	set (OPENSSL_LIB_DIR  "c:/Develop/openssl/openssl-win64-${OPENSSL_VERSION}/lib/vc/static")
	set (QT_CMAKE_DIR     "c:/Develop/qt/qt-${QT_VERSION}/build/amd64/qtbase/lib/cmake")
	set (QT_DLL_DIR       "c:/Develop/qt/qt-${QT_VERSION}/build/amd64/qtbase/lib")
else ()
	set (LUA_INC_DIR      "c:/Develop/lua/lua-${LUA_VERSION}/include")
	set (LUA_LIB_DIR      "c:/Develop/lua/lua-${LUA_VERSION}/lib/x86/${CONFIGURATION_SUFFIX}")
	set (PCAP_INC_DIR     "c:/Develop/winpcap/winpcap-${WINPCAP_VERSION}/include")
	set (PCAP_LIB_DIR     "c:/Develop/winpcap/winpcap-${WINPCAP_VERSION}/lib")
	set (LIBUSB_INC_DIR   "c:/Develop/libusb/libusb-${LIBUSB_VERSION}/libusb")
	set (LIBUSB_LIB_DIR   "c:/Develop/libusb/libusb-${LIBUSB_VERSION}/Win32/Debug/lib")
	set (OPENSSL_INC_DIR  "c:/Develop/openssl/openssl-win32-${OPENSSL_VERSION}/include")
	set (OPENSSL_LIB_DIR  "c:/Develop/openssl/openssl-win32-${OPENSSL_VERSION}/lib/vc/static")
	set (QT_CMAKE_DIR     "c:/Develop/qt/qt-${QT_VERSION}/build/x86/qtbase/lib/cmake")
	set (QT_DLL_DIR       "c:/Develop/qt/qt-${QT_VERSION}/build/x86/qtbase/lib")
endif()

set (RAGEL_EXE            "c:/Develop/ragel/ragel-${RAGEL_VERSION}/ragel.exe")

#..............................................................................
