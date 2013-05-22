#include "pch.h"

using namespace axl;

namespace test_Packer { 

//.............................................................................

void 
Run ()
{
	ref::CPtrT <exe::CArgBlock> ArgBlock = exe::CreateArgBlock <exe::CArgT <const char*> > (0, "hui govno i muravei");

	ref::CPtrT <mem::TBlock> Package = rtl::FormatPackage(
		"%s %d %s %d",
		"hui", 10, "govno", 20
		);

	rtl::CUnpacker Unpacker (Package->m_p, Package->m_Size);
	
	const char* s1;	
	long l1;	
	char* s2;	
	long l2;	
	
	Unpacker.Unpack (&s1);
	Unpacker.Unpack (&l1);
	Unpacker.Unpack (&s2);
	Unpacker.Unpack (&l2);

	printf ("%s %d %s %d\n", s1, l1, s2, l2);
}

//.............................................................................

} // namespace test_Packer
