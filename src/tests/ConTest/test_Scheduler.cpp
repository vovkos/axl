#include "pch.h"

using namespace axl;

namespace test_Scheduler { 

/*

//.............................................................................


class CTest
{
public:
	rtl::CString m_s;
	rtl::CString m_s2;
	rtl::CString m_s3;

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

class CMyScheduler: public exe::IScheduler
{
public:
	virtual
	EScheduleResult
	Schedule_va (
		const exe::CFunction& Function, 
		axl_va_list va
		)
	{
		pFunction->Invoke_va (va);
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

void 
Run ()
{
	CTest* p;
	
	p = rtl::GetSingleton <CTest> ();
	p = rtl::GetSingleton <CTest> ();
	
	CTest2* p2;
	
	p2 = rtl::GetSingleton <CTest2> ();
	p2 = rtl::GetSingleton <CTest2> ();

	ref::CPtrT<CTest> x = AXL_REF_NEW (ref::CBoxT <CTest>);
	ref::CWeakPtrT<CTest> y = x;
	y.Detach ();

	CMyScheduler Scheduler;

	exe::CFunctionT <
		exe::CArgSeqT_2 <
			int,
			int 
			>,
		exe::CArgSeqT_3 <
			int,
			int,
			int
			>
		> Function (Test, -1, -2);

	Function.Invoke (0, 100, 200, 300);
}

*/

//.............................................................................

} // namespace test_Scheduler
