#include "pch.h"

using namespace axl;

namespace test_Finalize { 

//.............................................................................

class CMyFinalizer: 
	public ref::CRefCount,
	public g::CFinalizer
{
public:
	CMyFinalizer ()
	{
		printf ("CMyFinalizer::CMyFinalizer (this = %x)\n", this);
	}

	~CMyFinalizer ()
	{
		printf ("CMyFinalizer::~CMyFinalizer (this = %x)\n", this);
	}

	virtual
	void 
	finalize ()
	{
		printf ("CMyFinalizer::Finalize (this = %x)\n", this);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run ()
{
	ref::CPtrT <CMyFinalizer> fin = AXL_REF_NEW (CMyFinalizer);
	g::getModule ()->addFinalizer (fin);
}

//.............................................................................

} // namespace test_Finalize
