#include "pch.h"

using namespace axl;

namespace test_Finalize { 

//.............................................................................

class MyFinalizer: 
	public ref::RefCount,
	public g::Finalizer
{
public:
	MyFinalizer ()
	{
		printf ("CMyFinalizer::CMyFinalizer (this = %x)\n", this);
	}

	~MyFinalizer ()
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
	ref::Ptr <MyFinalizer> fin = AXL_REF_NEW (MyFinalizer);
	g::getModule ()->addFinalizer (fin);
}

//.............................................................................

} // namespace test_Finalize
