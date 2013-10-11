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
	Finalize ()
	{
		printf ("CMyFinalizer::Finalize (this = %x)\n", this);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
Run ()
{
	ref::CPtrT <CMyFinalizer> Fin = AXL_REF_NEW (CMyFinalizer);
	g::GetModule ()->AddFinalizer (Fin);
}

//.............................................................................

} // namespace test_Finalize
