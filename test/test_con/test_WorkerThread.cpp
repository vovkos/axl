#include "pch.h"

namespace test_WorkerThread {

/*

//..............................................................................

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
	ref::Ptr <exe::Workerhread> workerThread = exe::getWorkerThread (2);

	printf ("Main thread TID: %x\n", getCurrentThreadId ());

	workerThread->schedule <exe::ArgSeq_3 <
		char,
		char,
		const char*
		> > (foo, 100, 101, sl::String ("hui"));

	workerThread->schedule <exe::ArgSeq_3 <
		short,
		short,
		const char*
		> > (foo, 200, 201, sl::String ("govno"));

	workerThread->schedule <exe::ArgSeq_3 <
		int,
		int,
		const char*
		> > (foo, 300, 301, sl::String ("muravei"));

	mt::Event event1;

	workerThread->addEvent (
		&event1,
		&exe::Function <
			exe::Arg <int>,
			exe::Arg <void>
			> (onEvent, 1)
		);

	mt::Event event2;

	workerThread->addEvent (
		&event2,
		&exe::Function <
			exe::Arg <int>,
			exe::Arg <void>
			> (onEvent, 2)
		);


	event1.signal ();

	sleep (1000);

	event2.signal ();

	sleep (1000);
}

*/

//..............................................................................

} // namespace test_WorkerThread
