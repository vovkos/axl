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
#include "test.h"

namespace {

//..............................................................................

class MyFinalizer:
	public ref::RefCount,
	public g::Finalizer
{
public:
	MyFinalizer ()
	{
		printf ("MyFinalizer::MyFinalizer (this = %p)\n", this);
	}

	~MyFinalizer ()
	{
		printf ("MyFinalizer::~MyFinalizer (this = %p)\n", this);
	}

	virtual
	void
	finalize ()
	{
		printf ("MyFinalizer::finalize (this = %p)\n", this);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run ()
{
	ref::Ptr <MyFinalizer> fin = AXL_REF_NEW (MyFinalizer);
	g::getModule ()->addFinalizer (fin);
}

//..............................................................................

ADD_TEST_CASE ("test_Finalizer", run)

}
