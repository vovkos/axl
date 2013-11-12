#include "pch.h"

using namespace axl;

namespace test_File { 

//.............................................................................

void
Run ()
{
	io::CFile f;
	f.Open ("d:\\hui", io::EFileFlag_ReadOnly);
	rtl::CString Text = err::GetError ()->GetDescription ();
	printf ("error: %s\n", Text);
}

//.............................................................................

} // namespace test_File
