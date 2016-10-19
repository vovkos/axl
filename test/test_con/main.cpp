#include "pch.h"
#include "test.h"

//..............................................................................

#if (_AXL_OS_WIN)
int
wmain (
	int argc,
	wchar_t* argv []
	)
#else
int
main (
	int argc,
	char* argv []
	)
#endif
{
	WSADATA wsaData;
	WORD versionRequested = MAKEWORD (2, 2);

	WSAStartup (versionRequested, &wsaData);
	srand ((uint_t) sys::getTimestamp ());

	err::registerParseErrorProvider ();

	//test_Error::Run ();
	//test_File::Run ();
	//test_Finalizer::Run ();
	//test_Function::Run ();
	//test_HandleTable::Run ();
	//test_HashTable::Run ();
	//test_List::Run ();
	//test_Log::Run ();
	//test_Packer::Run ();
	//test_Parse::Run ();
	//test_RbTree::Run ();
	//test_Ref::Run_Buf ();
	//test_Ref::Run_WeakRef ();
	//test_Ref::Run_Clone ();
	//test_Ref::Run_Container ();
	//test_Scheduler::Run ();
	//test_Sock::Run_Listen ();
	//test_Sock::Run_Connect ();
	//test_Sock::Run_Dgram ();
	//test_String::Run ();
	//test_StringTemplate::Run ();
	//test_Timer::Run ();
	//test_Vso::Run ();
	//test_Window::Run ();
	//test_WorkerThread::Run ();
	//test_Packetizer::Run ();
	//test_CmdLineParser::Run ();
	//test_Usb::run ();
	//test_BoyerMooreFind::run ();

	test_Crypto::run ();

	return 0;
}

//..............................................................................
