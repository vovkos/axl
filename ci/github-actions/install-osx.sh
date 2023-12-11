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

brew install lua
brew install libusb
brew install ragel

# openssl is already installed, but not linked

echo "set(OPENSSL_INC_DIR /usr/local/opt/openssl/include)" >> paths.cmake
echo "set(OPENSSL_LIB_DIR /usr/local/opt/openssl/lib)" >> paths.cmake

# re2s requires C++11

echo "axl_override_setting(GCC_FLAG_CPP_STANDARD -std=c++11)" >> settings.cmake
