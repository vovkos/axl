#include "pch.h"

using namespace axl;

namespace test_File { 

//.............................................................................

void
run ()
{
	io::CFile f;
	f.open ("d:\\hui", io::EFileFlag_ReadOnly);
	rtl::CString text = err::getError ()->getDescription ();
	printf ("error: %s\n", text);
}

//.............................................................................

} // namespace test_File
