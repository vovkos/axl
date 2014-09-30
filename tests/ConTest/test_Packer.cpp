#include "pch.h"

using namespace axl;

namespace test_Packer { 

//.............................................................................

void 
run ()
{
	ref::CPtrT <exe::CArgBlock> argBlock = exe::createArgBlock <exe::CArgT <const char*> > (0, "hui govno i muravei");

	ref::CPtrT <mem::TBlock> package = rtl::formatPackage(
		"%s %d %s %d",
		"hui", 10, "govno", 20
		);

	rtl::CUnpacker unpacker (package->m_p, package->m_size);
	
	const char* s1;	
	long l1;	
	char* s2;	
	long l2;	
	
	unpacker.unpack (&s1);
	unpacker.unpack (&l1);
	unpacker.unpack (&s2);
	unpacker.unpack (&l2);

	printf ("%s %d %s %d\n", s1, l1, s2, l2);
}

//.............................................................................

} // namespace test_Packer
