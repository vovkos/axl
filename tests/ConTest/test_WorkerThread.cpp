#include "pch.h"

using namespace axl;

namespace test_WorkerThread { 

/*

//.............................................................................

void foo (int i, int j, char* p)
{
	printf ("Foo TID: %x, i: %d, j: %d, p: %s\n", getCurrentThreadId (), i, j, p);
}

void onEvent (int i)
{
	printf ("OnEvent TID: %x, i: %d\n", getCurrentThreadId (), i);
}

void 
run ()
{
	ref::CPtrT <exe::CWorkerThread> workerThread = exe::getWorkerThread (2);

	printf ("Main thread TID: %x\n", getCurrentThreadId ());

	workerThread->schedule <exe::CArgSeqT_3 < 
		char,
		char,
		const char*
		> > (foo, 100, 101, rtl::CString ("hui"));

	workerThread->schedule <exe::CArgSeqT_3 < 
		short,
		short,
		const char*
		> > (foo, 200, 201, rtl::CString ("govno"));

	workerThread->schedule <exe::CArgSeqT_3 < 
		int,
		int,
		const char*
		> > (foo, 300, 301, rtl::CString ("muravei"));

	mt::CEvent event1;

	workerThread->addEvent (
		&event1, 
		&exe::CFunctionT <
			exe::CArgT <int>,  
			exe::CArgT <void>
			> (onEvent, 1)
		);

	mt::CEvent event2;

	workerThread->addEvent (
		&event2, 
		&exe::CFunctionT <
			exe::CArgT <int>,  
			exe::CArgT <void>
			> (onEvent, 2)
		);


	event1.signal ();

	sleep (1000);  

	event2.signal ();

	sleep (1000);  
}

*/

//.............................................................................

} // namespace test_WorkerThread
