#include "pch.h"
#include "test.h"

using namespace axl;

//.............................................................................

int 
_tmain (
	int argc, 
	_TCHAR* argv []
	)
{
	WSADATA WsaData;
	WORD VersionRequested = MAKEWORD (2, 2);

	WSAStartup (VersionRequested, &WsaData);

	err::CParseErrorProvider::Register ();
		
	//test_Error::Run ();
	//test_File::Run ();
	//test_Finalizer::Run ();
	//test_Function::Run ();
	//test_HandleTable::Run ();
	//test_HashTable::Run ();
	//test_List::Run ();
	
	test_Log::Run ();
	
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

	return 0;
}

//.............................................................................
