//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"

namespace test_File {

//..............................................................................

void
run ()
{
	io::File f;
	f.open ("d:\\hui", io::FileFlag_ReadOnly);
	sl::String text = err::getError ()->getDescription ();
	printf ("error: %s\n", text);
}

//..............................................................................

} // namespace test_File
