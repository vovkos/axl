#include "pch.h"

namespace test_File { 

//.............................................................................

void
run ()
{
	io::File f;
	f.open ("d:\\hui", io::FileFlag_ReadOnly);
	rtl::String text = err::getError ()->getDescription ();
	printf ("error: %s\n", text);
}

//.............................................................................

} // namespace test_File
