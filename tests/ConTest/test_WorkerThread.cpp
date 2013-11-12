#include "pch.h"

using namespace axl;

namespace test_WorkerThread { 

/*

//.............................................................................

void Foo (int i, int j, char* p)
{
	printf ("Foo TID: %x, i: %d, j: %d, p: %s\n", GetCurrentThreadId (), i, j, p);
}

void OnEvent (int i)
{
	printf ("OnEvent TID: %x, i: %d\n", GetCurrentThreadId (), i);
}

void 
Run ()
{
	ref::CPtrT <exe::CWorkerThread> WorkerThread = exe::GetWorkerThread (2);

	printf ("Main thread TID: %x\n", GetCurrentThreadId ());

	WorkerThread->Schedule <exe::CArgSeqT_3 < 
		char,
		char,
		const char*
		> > (Foo, 100, 101, rtl::CString ("hui"));

	WorkerThread->Schedule <exe::CArgSeqT_3 < 
		short,
		short,
		const char*
		> > (Foo, 200, 201, rtl::CString ("govno"));

	WorkerThread->Schedule <exe::CArgSeqT_3 < 
		int,
		int,
		const char*
		> > (Foo, 300, 301, rtl::CString ("muravei"));

	mt::CEvent Event1;

	WorkerThread->AddEvent (
		&Event1, 
		&exe::CFunctionT <
			exe::CArgT <int>,  
			exe::CArgT <void>
			> (OnEvent, 1)
		);

	mt::CEvent Event2;

	WorkerThread->AddEvent (
		&Event2, 
		&exe::CFunctionT <
			exe::CArgT <int>,  
			exe::CArgT <void>
			> (OnEvent, 2)
		);


	Event1.Signal ();

	Sleep (1000);  

	Event2.Signal ();

	Sleep (1000);  
}

*/

//.............................................................................

} // namespace test_WorkerThread
