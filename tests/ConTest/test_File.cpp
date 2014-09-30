#include "pch.h"

using namespace axl;

namespace test_File { 

//.............................................................................

void
run ()
{
	io::File f;
	f.open ("d:\\hui", io::FileFlagKind_ReadOnly);
	rtl::String text = err::getError ()->getDescription ();
	printf ("error: %s\n", text);
}

//.............................................................................

} // namespace test_File
