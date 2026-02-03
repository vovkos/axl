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
brew install hidapi
brew install ragel

BREW_PREFIX=$(brew --prefix)

# openssl is already installed, but not linked

echo "set(OPENSSL_INC_DIR $BREW_PREFIX/opt/openssl/include)" >> paths.cmake
echo "set(OPENSSL_LIB_DIR $BREW_PREFIX/opt/openssl/lib)" >> paths.cmake
