#include "stdafx.h"
#include <axl_llk_Parser.h>
 
#include "TestLexer.h"
#include "TestParser.h"

//.............................................................................

namespace test0 {

struct IMyInterface
{
	virtual axl::ref::CPtrT <IMyInterface> GetChild (int i) = 0;
	virtual void DoSomething () = 0;
	virtual void DoSomethingElse () = 0;
};

class CMyClass: public IMyInterface
{
public:
	CMyClass ()
		{ printf ("CMyClass::CMyClass ()\n"); }

	~CMyClass ()
		{ printf ("CMyClass::~CMyClass ()\n"); }

	axl::ref::CPtrT <IMyInterface> GetChild (int i)
		{ return NULL; }

	virtual void DoSomething ()
		{ printf ("CMyClass::DoSomething ()\n"); }

	virtual void DoSomethingElse ()
		{ printf ("CMyClass::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMyContainer: 
	public axl::ref::IRefCount,
	public IMyInterface
{
public:
	axl::ref::CChildT <axl::ref::CBoxT <CMyClass> > m_Child1;
	axl::ref::CChildT <axl::ref::CBoxT <CMyClass> > m_Child2;
	axl::ref::CChildT <axl::ref::CBoxT <CMyClass> > m_Child3;

public:
	CMyContainer () :
		m_Child1 (this),
		m_Child2 (this),
		m_Child3 (this)
		{ printf ("CMyContainer::CMyContainer ()\n"); }

	~CMyContainer ()
		{ printf ("CMyContainer::~CMyContainer ()\n"); }

	axl::ref::CPtrT <IMyInterface> GetChild (int i)
	{ 
		switch (i)
		{
		case 0:
			return &m_Child1;

		case 1:
			return &m_Child2;

		case 2:
			return &m_Child3;

		default:
			return NULL;
		}
	}

	virtual void DoSomething ()
		{ printf ("CMyContainer::DoSomething ()\n"); }

	virtual void DoSomethingElse ()
		{ printf ("CMyContainer::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMySuperContainer: 
	public axl::ref::IRefCount,
	public IMyInterface
{
public:
	axl::ref::CChildT <CMyContainer> m_Child1;
	axl::ref::CChildT <CMyContainer> m_Child2;
	axl::ref::CChildT <axl::ref::CBoxT <CMyClass> > m_Child3;

public:
	CMySuperContainer () :
		m_Child1 (this),
		m_Child2 (this),
		m_Child3 (this)
		{ printf ("CMySuperContainer::CMySuperContainer ()\n"); }

	~CMySuperContainer ()
		{ printf ("CMySuperContainer::~CMySuperContainer ()\n"); }

	axl::ref::CPtrT <IMyInterface> GetChild (int i)
	{ 
		switch (i)
		{
		case 0:
			return &m_Child1;

		case 1:
			return &m_Child2;

		case 2:
			return &m_Child3;

		default:
			return NULL;
		}
	}

	virtual void DoSomething ()
		{ printf ("CMySuperContainer::DoSomething ()\n"); }

	virtual void DoSomethingElse ()
		{ printf ("CMySuperContainer::DoSomethingElse ()\n"); }
};

//.............................................................................

void Run () 
{
	axl::ref::CPtrT <IMyInterface> p;
	axl::ref::CPtrT <IMyInterface> p2;
	axl::ref::CPtrT <IMyInterface> p3;

	p = AXL_REF_NEW (CMySuperContainer);
	p->DoSomething ();
	p->DoSomethingElse ();

	p->GetChild (1)->DoSomething ();
	p->GetChild (1)->DoSomethingElse ();

	p2 = p->GetChild (2);

	p->GetChild (2)->DoSomething ();
	p->GetChild (2)->DoSomethingElse (); 

	p = NULL;

	p2->DoSomething ();
	p2 = NULL;

	p3 = AXL_REF_NEW (axl::ref::CBoxT <CMyClass>);

	axl::ref::CWeakPtrT <IMyInterface> w = p3;
	p3->DoSomethingElse ();	

	p3 = NULL;

	p = w;

//	w.Detach ();
}

} // namespace test0 {

////.............................................................................
//
//int MyFunction (const tchar_t* p1, const tchar_t* p2, const tchar_t* p3)
//{
//	return printf ("%s %s %s\n", p1, p2, p3);
//}
//
//int __stdcall MyFunction2 (int x1, int x2, int x3)
//{
//	return printf ("%d %d %d\n", x1, x2, x3);
//}
//
////.............................................................................
//
//void FunctionTest ()
//{
//	CFunctionT <
//		MyFunction, 
//		axl::exe::ECallingConvention_Cdecl,
//		axl::rtl::IPackImplT_3 (
//			CArgT <axl::rtl::CString>,
//			CArgString,
//			CArgString
//			> > Function;
//
//	CPtrT <axl::mem::TBlock> Stack = Function.GetArgPacker ()->BuildFromArgs (
//		0, 
//		axl::rtl::CString ("hui"), 
//		axl::rtl::CString ("govno"), 
//		axl::rtl::CString ("muravei")
//		);
//
//	Function.Invoke (Stack->m_p, Stack->m_Size);
//
//	CFunctionT <
//		MyFunction2, 
//		axl::exe::ECallingConvention_Stdcall,
//		axl::rtl::IPackImplT_3 (
//			CArgT <int>,
//			CArgT <int>,
//			CArgT <int>
//			> > Function2;
//
//	Stack = Function2.GetArgPacker ()->BuildFromArgs (
//		0, 
//		10, 
//		20, 
//		30
//		);
//
//	Function2.Invoke (Stack->m_p, Stack->m_Size);
//}
//
////.............................................................................
//
//struct TMyStruct 
//{
//	TListLink m_Link1;
//	TListLink m_Link2;
//
//	int m_a;
//	int m_b;
//	int m_c;
//};
//
//struct TMyStruct2: TListLink
//{
//	int m_x;
//	int m_y;
//	int m_z;
//};
//
////. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//
///*
//
//template <typename T>
//const char* GetTypeTag ();
//
//template <>
//inline const char* GetTypeTag (int>()
//	{ return "int"; }
//
//template <>
//inline const char* GetTypeTag (double>()
//	{ return "double"; }
//
//void TypeTagTest ()
//{
//	printf ("type (int) = %s\n",    GetTypeTag (int>());
//	printf ("type (double) = %s\n", GetTypeTag (double>());
//	printf ("type (TMyStruct) = %s\n", GetTypeTag (TMyStruct>());
//
//	TMyStruct* p = new TMyStruct;
//}
//
//*/
//
////.............................................................................
//
//void ListTest ()
//{
//	CListT <
//		TMyStruct,
//		CDeleteT <TMyStruct>,
//		COffsetT <TMyStruct, TListLink, offsetof (TMyStruct, m_Link1) >
//		> List1;
//
//	CAuxListT <
//		TMyStruct,
//		COffsetT <TMyStruct, TListLink, offsetof (TMyStruct, m_Link2) >
//		> List2;
//
//	TMyStruct* p = new TMyStruct;
//	List1.InsertHead (p);
//	List2.InsertHead (p);
//
//	p = new TMyStruct;
//	List1.InsertHead (p);
//	List2.InsertHead (p);
//
//	CListT <
//		TMyStruct2,
//		CCastT <TMyStruct2*, TListLink*>,
//		CDeleteT <TMyStruct2>
//		> List3;
//
//	TMyStruct2* p2 = new TMyStruct2;
//	List3.InsertTail (p2);
//};
//
////.............................................................................
//
///*
//
//struct axl::rtl::IPack
//{
//};
//
////.............................................................................
//
//template <typename T>
//struct CArgValueAdapterT
//{
//	typedef T TArgType;
//
//	enum
//	{
//		StackSize = sizeof (T),
//		HeapSize  = 0,
//	};
//};
//
//template <typename A1>
//class CStackFramePackT_1: public axl::rtl::IPack
//{
//public:
//	CStackFramePackT_1 ()
//		{ m_Block.m_p = NULL, m_Block.m_Size = 0; }
//};
//
//template <typename A1, typename A2>
//class CStackFramePackT_2: public axl::rtl::IPack
//{
//public:
//};
//
//*/
//
////.............................................................................
//
//struct TBase: TListLink
//{
//	int a, b;
//};
//
//struct TDerived: TBase
//{
//	int c, d;
//};
//
//void TreeTest ()
//{
//	CIteratorT <TDerived> ItDerived;
//	CIteratorT <TBase> ItBase;
//
//	ItBase = ItDerived;
//
//	CRbTreeMapT <int, int> Tree;
//	CMapIteratorT <int, int>::T It;
//
//	srand (GetTickCount ());
//
//	for (size_t i = 0; i < 50; i++)
//		Tree.Goto (rand () % 50);
//
//	It = Tree.GetHead ();
//	for (; It; It++)
//		printf ("%d\n", It->m_Key);
//
//	printf (".........\n");
//
//	for (size_t i = 0; i < 50; i++)
//		Tree.DeleteByKey (rand () % 50);
//
//	It = Tree.GetHead ();
//	for (; It; It++)
//		printf ("%d\n", It->m_Key);
//
//	printf (".........\n");
//}
//
////.............................................................................
//
void HashTableTest ()
{
	axl::rtl::CHashTableMapT <int, int, axl::rtl::CHashIdT <int> > Map;
	axl::rtl::CIteratorT <axl::rtl::CHashTableMapEntryT <int, int> > It;

	srand (GetTickCount ());

	for (size_t i = 0; i < 50; i++)
	{
		int Key = rand () % 50;
		It = Map.Goto (Key);
		It->m_Value = Key * 10;
	}

	It = Map.GetHead ();
	for (; It; It++)
		printf ("%d = %d\n", It->m_Key, It->m_Value);

	printf (".........\n");

	for (size_t i = 0; i < 50; i++)
	{
		int Key = rand () % 50;
		It = Map.Find (Key);
		if (It)
			printf ("%d = %d\n", Key, It->m_Value);
		else
			printf ("%d not found\n", Key);
	}

	printf (".........\n");
}
//
////.............................................................................
//
//class CAddress
//{
//public:
//	ulong_t m_Size;
//	ulong_t m_Protocol;
//
//	// followed by actual data...
//
//	CAddress (ulong_t Size = sizeof (CAddress), ulong_t Protocol = 0)
//		{ m_Size = Size, m_Protocol = Protocol; }
//
//	// VSO
//
//	size_t vso_GetSize () const
//		{ return m_Size; }
//
//	void vso_SetSize (size_t Size)
//		{ m_Size = Size; }
//
//	bool vso_Copy (const CAddress& Src)
//	{ 
//		ASSERT (m_Size >= Src.m_Size);
//		memcpy (this, &Src, Src.m_Size); 
//		return true;
//	}
//};
//
//class CAddressIp4: public CAddress
//{
//public:
//	ulong_t m_Ip;
//	ushort_t m_Port;
//
//	CAddressIp4 (ulong_t Protocol, ulong_t Ip, ushort_t Port):
//		CAddress (sizeof (CAddressIp4), Protocol)
//	{
//		m_Ip = Ip;
//		m_Port = Port;
//	}
//};
//
//void VsoTest ()
//{
//	CAddressIp4 Ip4_1 (17, 0x7f000001, 1001);
//	CAddressIp4 Ip4_2 (17, 0xffffffff, 1002);
//	
//	CVsoT <CAddress> Address;
//	Address = Ip4_1;
//	Address = Ip4_2;
//}
//
////.............................................................................
//
//void StringTest ()
//{
//	CString s ("hui", 2);
//	
//	s += "grr";
//
//	CString s2 = s;
//	CString s3 = s + s;
//}
//


//.............................................................................

typedef void (*FFunction) (
	const char* p1,
	const char* p2,
	const char* p3
	);

int Run (int argc, _TCHAR* argv [])
{
/*	axl::exe::CWorkerThread WorkerThread;
	WorkerThread.Start ();
	WorkerThread.Invoke (TestFunction, axl_exe_ECallingConvention_Cdecl, "%s %s %s", "hui", "govno", "muravei");

	printf ("Waiting...\n");
	Sleep (3000);
	printf ("Done!\n"); */

//	ListTest ();
//	LifeTimeControlTest ();
//	FunctionTest ();
//	TreeTest ();
//	HashTableTest ();
//	StringTest ();
//	TypeTagTest ();
//	VsoTest ();

	return 0;
}

//.............................................................................

class CTest
{
public:
	axl::rtl::CString m_s;
	axl::rtl::CString m_s2;
	axl::rtl::CString m_s3;

	CTest ()
	{
		m_s = "hui!!!";
		m_s2 = "hui!!!";
		m_s3 = "hui!!!";
		printf ("CTest::CTest\n");
	}

	~CTest ()
	{
		printf ("CTest::~CTest\n");
	}
};

//.............................................................................

class CTest2
{
public:
	CTest2 ()
	{
		printf ("CTest2::CTest2\n");
	}

	~CTest2 ()
	{
		printf ("CTest2::~CTest2\n");
	}
};

//.............................................................................

void 
Test (
	int x,
	int y,
	int z
	)
{
	printf ("Test: x = %d, y = %d, z = %d\n", x, y, z);
}

#ifdef _CPPRTTI 

dsdjasdklj +
dsdjasdklj +
dsdjasdklj +
dsdjasdklj +
dsdjasdklj +
dsdjasdklj +

#else

#endif

class CMyScheduler: public axl::exe::IScheduler
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMyScheduler, axl::exe::IScheduler)

	virtual
	EScheduleResult
	ScheduleV (
		axl::exe::IFunction* pFunction, 
		va_list va
		)
	{
		pFunction->InvokeV (va);
		return EScheduleResult_Invoke;
	}
};

struct IInterfaceA
{
	int m_x;

	virtual void Foo () {}
};

struct IInterfaceB: IInterfaceA
{
	int m_y;
};

struct IInterfaceC: IInterfaceA
{
	int m_z;
};

void Test1 ()
{
	CTest* p;
	
	p = axl::rtl::GetSingleton <CTest> ();
	p = axl::rtl::GetSingleton <CTest> ();
	
	CTest2* p2;
	
	p2 = axl::rtl::GetSingleton <CTest2> ();
	p2 = axl::rtl::GetSingleton <CTest2> ();

	axl::ref::CPtrT<CTest> x = AXL_REF_NEW (axl::ref::CBoxT <CTest>);
	axl::ref::CWeakPtrT<CTest> y = x;
	y.Detach ();

	CMyScheduler Scheduler;

	axl::exe::CFunctionT <
		axl::exe::CArgSeqT_2 <
			int,
			int 
			>,
		axl::exe::CArgSeqT_3 <
			int,
			int,
			int
			>
		> Function (Test, -1, -2);

	axl::ref::CPtrT<axl::exe::IFunction> Clone = axl::ref::Clone (&Function);
	Clone.Detach ();


	Function.Invoke (0, 100, 200, 300);
}

void Foo (int i, int j, char* p)
{
	printf ("Foo TID: %x, i: %d, j: %d, p: %s\n", GetCurrentThreadId (), i, j, p);
}

void OnEvent (int i)
{
	printf ("OnEvent TID: %x, i: %d\n", GetCurrentThreadId (), i);
}

void Test2 ()
{
	axl::ref::CPtrT <axl::exe::CWorkerThread> WorkerThread = axl::exe::GetWorkerThread (2);

	printf ("Main thread TID: %x\n", GetCurrentThreadId ());

	WorkerThread->Schedule <axl::exe::CArgSeqT_3 < 
		char,
		char,
		const tchar_t*
		> > (Foo, 100, 101, axl::rtl::CString ("hui"));

	WorkerThread->Schedule <axl::exe::CArgSeqT_3 < 
		short,
		short,
		const tchar_t*
		> > (Foo, 200, 201, axl::rtl::CString ("govno"));

	WorkerThread->Schedule <axl::exe::CArgSeqT_3 < 
		int,
		int,
		const tchar_t*
		> > (Foo, 300, 301, axl::rtl::CString ("muravei"));

	axl::mt::CEvent Event1;

	WorkerThread->AddEvent (
		&Event1, 
		&axl::exe::CFunctionT <
			axl::exe::CArgT <int>,  
			axl::exe::CArgT <void>
			> (OnEvent, 1)
		);

	axl::mt::CEvent Event2;

	WorkerThread->AddEvent (
		&Event2, 
		&axl::exe::CFunctionT <
			axl::exe::CArgT <int>,  
			axl::exe::CArgT <void>
			> (OnEvent, 2)
		);


	Event1.Signal ();

	Sleep (1000);  

	Event2.Signal ();

	Sleep (1000);  
}

void Test3 ()
{
	axl::mt::CTimer Timer;

	Timer.SetTimer (
		1000, 
		&axl::exe::CFunctionT <
			axl::exe::CArgT <int>,  
			axl::exe::CArgT <void>
			> (OnEvent, 5),
		axl::mt::CTimer::ETimer_Periodic
		);

	Sleep (10000);
}

class CMyWindow: public axl::win::CWindowImplT <CMyWindow>
{
public:
	enum
	{
		ClassStyle      = CS_DBLCLKS,
		ClassCursor     = (int) IDC_ARROW,
		ClassBackground = COLOR_MENUHILIGHT,
	};

public:
	CMyWindow ()
	{
		printf ("CMyWindow::CMyWindow ()\n");
	}

	~CMyWindow ()
	{
		printf ("CMyWindow::~CMyWindow ()\n");
	}

//	AXL_WINDOW_CLASS ("CMyWindow")

	AXL_WINDOW_MESSAGE_MAP_BEGIN ()
		AXL_WINDOW_MESSAGE_HANDLER (WM_CREATE, OnCreate)
		AXL_WINDOW_MESSAGE_HANDLER (WM_DESTROY, OnDestroy)
		AXL_WINDOW_MESSAGE_HANDLER (WM_MOVE, OnMove)
		AXL_WINDOW_MESSAGE_HANDLER (WM_SIZE, OnSize)
	AXL_WINDOW_MESSAGE_MAP_END ()

	LRESULT OnCreate (UINT Msg, WPARAM wParam, LPARAM lParam, bool* pHandled)
	{
		printf ("WM_CREATE\n");
		*pHandled = false;
		return 0;
	}

	LRESULT OnDestroy (UINT Msg, WPARAM wParam, LPARAM lParam, bool* pHandled)
	{
		printf ("WM_DESTROY\n");
		PostQuitMessage (0);
		*pHandled = false;
		return 0;
	}

	LRESULT OnMove (UINT Msg, WPARAM wParam, LPARAM lParam, bool* pHandled)
	{
		printf ("WM_MOVE\n");
		*pHandled = false;
		return 0;
	}

	LRESULT OnSize (UINT Msg, WPARAM wParam, LPARAM lParam, bool* pHandled)
	{
		printf ("WM_SIZE\n");
		*pHandled = false;
		return 0;
	}
};

void
Test4_mt (int a)
{
	printf ("Test4_mt: TID: %x, i: %d\n", GetCurrentThreadId (), a);
}

void
Test4_wt (axl::exe::IScheduler* pMtScheduler, int a)
{
	printf ("Test4_wt: TID: %x, i: %d\n", GetCurrentThreadId (), a);
	pMtScheduler->Schedule <axl::exe::CArgT <int> > (Test4_mt, 600);
}

//.............................................................................

class CMyAtlWindow: public CWindowImpl <CMyAtlWindow>
{
public:
	DECLARE_WND_SUPERCLASS ("MyEdit", "EDIT")

	BEGIN_MSG_MAP (CMyAtlWindow)
		MESSAGE_HANDLER (WM_CREATE, OnCreate)
		MESSAGE_HANDLER (WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER (WM_MOVE, OnMove)
	END_MSG_MAP ()

	LRESULT OnCreate (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnCreate\n");
		bHandled = false;
		return 0;
	}

	LRESULT OnDestroy (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnDestroy\n");
		PostQuitMessage (0);
		bHandled = false;
		return 0;
	}

	LRESULT OnMove (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnMove\n");
		bHandled = false;
		return 0;
	}
};

//.............................................................................

void Test4 ()
{
	printf ("Test4: TID: %x\n", GetCurrentThreadId ()); 

	CMyWindow::Register ();

	//CMyAtlWindow Window3;
	//Window3.Create (NULL, 0, "ATL:GOVNO", WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	CMyWindow Window;
	Window.Create (NULL, NULL, "HHHUI!", WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	axl::exe::CWorkerWindow WorkerWindow;
	WorkerWindow.Create ();

	axl::exe::CWorkerThread WorkerThread;
	WorkerThread.Start ();

	WorkerThread.Schedule <axl::exe::CArgSeqT_2 < 
		axl::exe::IScheduler*,
		int
		> > (Test4_wt, (axl::exe::IScheduler*) &WorkerWindow, 300);

	MSG Msg;
	while (GetMessage (&Msg, NULL, 0, 0))
	{
		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}
}

//.............................................................................

class CMyClass
{
public:
	int m_x;
	int m_y;

public:
	CMyClass ()
	{
		printf ("CMyClass::CMyClass ()\n");
		m_x = 0;
		m_y = 0;
	}

	CMyClass (const CMyClass& Src)
	{
		printf ("CMyClass::CMyClass (const CMyClass&)\n");
		m_x = Src.m_x;
		m_y = Src.m_y;
	}

	~CMyClass ()
	{
		printf ("CMyClass::~CMyClass ()\n");
	}

	CMyClass&
	operator = (const CMyClass& Src)
	{
		printf ("CMyClass::operator = (const CMyClass&)\n");
		m_x = Src.m_x;
		m_y = Src.m_y;

		return *this;
	}
};

class CDcbGetSize
{
public:
	size_t 
	operator () (const DCB& Dcb)
	{
		return Dcb.DCBlength;
	}
};

void Test5 ()
{
	axl::rtl::CString s;
	s = "sdkjhasdljadljasdklj";
	char x = s [10];

	s.Clear ();

	axl::rtl::CArrayT <uchar_t> arr;
	arr.SetCount (10);

	axl::rtl::CArrayT <uchar_t> arr2 = arr;
	arr2 [0] = 10;

	axl::ref::CBufT <DCB, CDcbGetSize> Dcb;
	Dcb.GetBuffer ();
	Dcb->DCBlength = 240;
	Dcb->BaudRate = 38400;

	axl::ref::CBufT <DCB, CDcbGetSize> Dcb2 = Dcb;
	Dcb2.GetBuffer ();

	CMyClass a;
	a.m_x = 1;
	a.m_y = 2;

	axl::ref::CBufT <CMyClass> b1;
	
	b1 = a;
	b1->m_x = 10;
	b1->m_y = 20;

	axl::ref::CBufT <CMyClass> b2 = b1;

	b1.GetBuffer ();
	b1.Release ();
	b2.Release ();
//	b1.GetBuffer ();

	//b2 = b1;
}

//.............................................................................

void Test6 ()
{
	axl::err::CError Error;
	
	Error.Pack <axl::rtl::CPackSeqT_4 <
		const char*,
		int,
		const char*,
		int
		> > (
		GUID_ALLOW_AWAYMODE, 
		12345, 
		"hui", 0xdead, "govno", 0xbeaf
		);

	axl::err::CError Error2;
	Error2.Format (
		GUID_ALLOW_AWAYMODE, 
		12345, 
		"%s %d %s %d",
		"hui", 0xdead, "govno", 0xbeaf
		);

	printf ("...\n");
}

//.............................................................................

namespace test10
{

class CMyClass: public axl::ref::IRefCount
{
public:
	CMyClass ()
	{
		printf ("CMyClass::CMyClass ()\n");
	}

	~CMyClass ()
	{
		printf ("CMyClass::CMyClass ()\n");
	}
};

void
Run ()
{
	axl::ref::CPtrT <CMyClass> p = AXL_REF_NEW (CMyClass);
	axl::ref::CPtrT <CMyClass> p2 = axl::ref::GetPtrOrClone ((CMyClass*) p);
	axl::ref::CPtrT <CMyClass> p3 = axl::ref::Clone (p);
	axl::ref::CWeakPtrT <CMyClass> w = p;
	p = NULL;
	p = w;
}

}

//.............................................................................

namespace test11
{

class C1
{
	operator int* ()
	{
		return NULL;
	}
};

template <typename T>
void TempFunc (T* p)
{
}

class CMyClass: public axl::ref::IRefCount
{
public:
	int m_x;
};

void
Run ()
{
	axl::ref::CPtrT <CMyClass> a, b;

	a = AXL_REF_NEW (CMyClass);
	a->m_x = 10;

	a->AddRef ();
	a->AddWeakRef ();

	b = axl::ref::Clone (a);
}

}

//.............................................................................

namespace test12
{

class CMyClass
{
public:
	CMyClass ()
	{
		printf ("CMyClass::CMyClass (this = %x)\n", this);
	}

	~CMyClass ()
	{
		printf ("CMyClass::~CMyClass (this = %x)\n", this);
	}
};

class CMyFinalizer: public axl::g::IFinalizer
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMyFinalizer, axl::g::IFinalizer)

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

void
Run ()
{
	CMyFinalizer Fin;
	axl::g::GetModule ()->AddFinalizer (&Fin);
}

}

//.............................................................................

namespace test13
{

void
Run ()
{
	axl::io::CFile f;
	f.Open ("d:\\hui", axl::io::CFile::EFile_ReadOnly);
	axl::rtl::CString Text = axl::err::GetError ()->GetDescription ();
	printf ("error: %s\n", Text);
}

}

//.............................................................................

namespace test14
{

void
Run ()
{
	axl::rtl::CBoxListT <int> List;
	List.InsertTail (10);
	List.InsertHead (-10);
	List.InsertTail (20);
	List.InsertHead (-20);

	List.RemoveHead ();

	axl::rtl::CBoxIteratorT <int> It = List.GetHead ();
	for (; It; It++)
	{
		printf ("%d\n", *It);
	}
}

}

//.............................................................................

namespace test15
{

template <typename T>
struct CDetailsT
{
	static
	void 
	Foo ()
	{
		printf ("general case for %s!\n", typeid (T).name ());
	}
};

template <typename T>
struct CSimpleDetailsT
{
	static
	void 
	Foo ()
	{
		printf ("simple case for %s!\n", typeid (T).name ());
	}
};

template <>
class CDetailsT <char>: public CSimpleDetailsT <char>
{
};

template <typename T>
class CDetailsT <T*>: public CSimpleDetailsT <T*>
{
};

void Run ()
{
	std::vector <char> StlArray;
	StlArray.resize (10);
	
	axl::rtl::CArrayT <char> Array;
	Array.SetCount (10);
}

}

//.............................................................................

namespace test16
{

void Run ()
{
	axl::ref::CPtrT <axl::exe::CArgBlock> ArgBlock = axl::exe::CreateArgBlock <axl::exe::CArgT <const char*> > (0, "hui govno i muravei");

	axl::ref::CPtrT <axl::mem::TBlock> Package = axl::rtl::FormatPackage(
		"%s %d %s %d",
		"hui", 10, "govno", 20
		);

	axl::rtl::CUnpacker Unpacker (Package->m_p, Package->m_Size);
	
	const char* s1;	
	long l1;	
	char* s2;	
	long l2;	
	
	Unpacker.Unpack (&s1);
	Unpacker.Unpack (&l1);
	Unpacker.Unpack (&s2);
	Unpacker.Unpack (&l2);

	printf ("%s %d %s %d\n", s1, l1, s2, l2);
}

}

//.............................................................................

namespace test17
{

void 
OnAccept (axl::ref::CPtrT <axl::io::CConnectionSock> ConnectionSock)
{
	axl::io::TSockAddrU LocalAddr;
	axl::io::TSockAddrU PeerAddr;

	ConnectionSock->GetLocalAddress (&LocalAddr);
	ConnectionSock->GetPeerAddress (&PeerAddr);
	
	printf ("accepted connection from %s\n", PeerAddr.ToString ());
}

void Run ()
{
	axl::io::CListenerSock Sock;	

	axl::io::CSockAddrIp Addr (1001);

	bool Result = 
		Sock.Open (axl::io::ESockProto_Tcp, Addr) &&
		Sock.Listen (32, &axl::exe::CFunctionT <
			axl::exe::CArgT <void>, 
			axl::exe::CArgT <axl::ref::CPtrT <axl::io::CConnectionSock> > > (OnAccept));;
	
	if (!Result)
	{
		printf ("cannot listen on %s: %s\n", Addr.ToString (), axl::err::GetError ()->GetDescription ());
		return;
	}

	printf ("listening on %s\n", Addr.ToString ());
	
	char s [32];
	gets (s);
}

}

//.............................................................................

namespace test18
{

void 
OnConnect (
	axl::io::CConnectionSock* pSock,
	const axl::err::CError& Error
	)
{
	axl::io::TSockAddrU LocalAddr;
	axl::io::TSockAddrU PeerAddr;

	pSock->GetLocalAddress (&LocalAddr);
	pSock->GetPeerAddress (&PeerAddr);

	if (!Error)
		printf ("established connection to %s from %s\n", PeerAddr.ToString (), LocalAddr.ToString ());
	else
		printf ("cannot establish connection: %s\n", Error->GetDescription ());
}

void 
Run ()
{
	axl::exe::CWorkerThread WorkerThread;
	WorkerThread.Start ();

	axl::io::CConnectionSock Sock;	

	axl::io::CSockAddrIp Addr (0x7f000001, 1001);

	axl::exe::CScheduledFunctionT <
		axl::exe::CArgT <axl::io::CConnectionSock*>, 
		axl::io::CConnectionSock::COnConnectCompleteArg> 
		OnComplete (&WorkerThread, OnConnect, &Sock);

	bool Result = 
		Sock.Open (axl::io::ESockProto_Tcp, axl::io::ESockAddr_Ip) &&
		Sock.SyncConnect (&Addr, 1000);
	
	if (!Result)
	{
		printf ("cannot connect to %s: %s\n", Addr.ToString (), axl::err::GetError ()->GetDescription ());
		return;
	}

	char s [32];
	gets (s);
}

}

//.............................................................................

namespace test19
{

char Buffer [1024] = { 0 };

void 
OnRecvFrom (
	axl::io::CDgramSock* pSock,
	const axl::err::CError& Error,
	axl::io::TSockAddr* pAddr,
	size_t ActualSize
	)
{
	if (Error)
	{
		printf ("error during recvfrom: %s\n", Error->GetDescription ());
		return;
	}

	printf ("received %d bytes from %s: %s\n", ActualSize, pAddr->ToString (), Buffer);

	axl::exe::CFunctionT <
		axl::exe::CArgT <axl::io::CDgramSock*>, 
		axl::io::CDgramSock::COnSendRecvCompleteArg> 
		OnComplete (OnRecvFrom, pSock);

	pSock->RecvFrom (Buffer, 1023, &OnComplete);
}

void 
Run ()
{
	axl::io::CDgramSock Sock;	

	axl::io::CSockAddrIp Addr (0, 1001);

	bool Result = Sock.Open (axl::io::ESockProto_Udp, &Addr);
	if (!Result)
	{
		printf ("cannot open %s: %s\n", Addr.ToString (), axl::err::GetError ()->GetDescription ());
		return;
	}

	printf ("listening on UDP %s\n", Addr.ToString ());

//	axl::io::CSockAddrIp AddrTo (0x7f000001, 1002);
//	size_t x = Sock.SyncSendTo ("hui!", 4, &AddrTo);

	axl::exe::CFunctionT <
		axl::exe::CArgT <axl::io::CDgramSock*>, 
		axl::io::CDgramSock::COnSendRecvCompleteArg> 
		OnComplete (OnRecvFrom, &Sock);

	Sock.RecvFrom (Buffer, 1023, &OnComplete);

	char s [32];
	gets (s);
}

}

//.............................................................................

namespace test20
{

class CPoint: public POINT
{
public:
	CPoint ()
	{
		printf ("CPoint::CPoint ()\n");
		x = 10;
		y = 20;
	}

	~CPoint ()
	{
		printf ("CPoint::~CPoint ()\n");
	}
};

typedef axl::ref::CBufT <CPoint> CPointBuf;

void 
Run ()
{
	size_t BufSize = sizeof (CPoint) + sizeof (CPointBuf::CHdr);
	CPointBuf Point (axl::ref::EBuf_Stack, _alloca (BufSize), BufSize);
	CPointBuf Point2 = Point;

	printf ("point: %d, %d\n", Point->x, Point->y);
	printf ("point2: %d, %d\n", Point2->x, Point2->y);

	Point2->x *= 10;
	Point2->y *= 10;

	printf ("point: %d, %d\n", Point->x, Point->y);
	printf ("point2: %d, %d\n", Point2->x, Point2->y);

	char Buf [80];
	axl::rtl::CString String (axl::ref::EBuf_Stack, Buf, sizeof (Buf));

	String.Format ("hui %d %d %d", 1, 2, 3);
	String.Append ("govno");
	String.Append ("muravei");

	axl::rtl::CString String2 = String;

	size_t i = String.Find (
		axl::rtl::EStrCmp_NoCase | axl::rtl::EStrCmp_Partial, 
		"muravEIKO"
		);
}

}

//.............................................................................

namespace parse_test
{

void 
Run (const tchar_t* pFileName)
{
	bool Result;

	axl::io::CMappedFile File;
	Result = File.Open (pFileName, axl::io::CFile::EFile_ReadOnly);
	if (!Result)
	{
		printf ("%s\n", axl::err::GetError ()->GetDescription ());
		return;
	}

	char* p = (char*) File.View ();
	size_t Size = (size_t) File.GetSize ();

	CLexer Lexer;
	CParser Parser;

	Lexer.Create (pFileName, p, Size);
	Parser.Create ();

	for (;;)
	{
		const CToken* pToken = Lexer.GetToken (); 

		Result = Parser.ParseToken (pToken);
		if (!Result)
		{
			printf ("%s\n", axl::err::GetError ()->GetDescription ());
			return;
		}

		if (!pToken->m_Token)
			break;

		Lexer.NextToken ();
	}
}

}

//.............................................................................

namespace st_test
{

void
Run (const tchar_t* pFileName)
{
	bool Result;

	axl::io::CMappedFile File;
	Result = File.Open (pFileName, axl::io::CFile::EFile_ReadOnly);
	if (!Result)
	{
		printf ("%s\n", axl::err::GetError ()->GetDescription ());
		return;
	}

	char* p = (char*) File.View ();
	size_t Size = (size_t) File.GetSize ();
	
	axl::st::CStringTemplate St;

	axl::rtl::CString ResultString;
	Result = St.Process (&ResultString, pFileName, p, Size);
	if (!Result)
	{
		printf ("error processing string template: %s\n", axl::err::GetError ()->GetDescription ());
		return;
	}

	printf ("result string:\n%s\n", ResultString);
}

}

//.............................................................................

int _tmain (int argc, _TCHAR* argv [])
{
	WSADATA WsaData;
	WORD VersionRequested = MAKEWORD (2, 2);

	WSAStartup (VersionRequested, &WsaData);

	axl::err::CParseErrorProvider::Register ();

	//Test10 ();

	//CObjT_2 <
	//	CMyObject, 
	//	ITransport,
	//	IFunction
	//	> Object;

	//ITransport* pTransport = &Object;
	//IFunction* pFunction = &Object;

	//IClass* pClass = pFunction->GetClass ();

	//IFunction* pFunction2 = (IFunction*) pTransport->GetInterface (__uuidof (IFunction));
	//IFunction* pFunction3 = Clone (pFunction);

	//size_t x = offsetofclass (CMyObject, IFunction);

	parse_test::Run ("expr.txt");

	return 0;
}

//.............................................................................

