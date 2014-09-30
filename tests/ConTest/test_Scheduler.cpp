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
test (
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
	schedule_va (
		const exe::CFunction& function, 
		axl_va_list va
		)
	{
		function->invoke_va (va);
		return EScheduleResult_Invoke;
	}
};

struct IInterfaceA
{
	int m_x;

	virtual void foo () {}
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
run ()
{
	CTest* p;
	
	p = rtl::getSingleton <CTest> ();
	p = rtl::getSingleton <CTest> ();
	
	CTest2* p2;
	
	p2 = rtl::getSingleton <CTest2> ();
	p2 = rtl::getSingleton <CTest2> ();

	ref::CPtrT<CTest> x = AXL_REF_NEW (ref::CBoxT <CTest>);
	ref::CWeakPtrT<CTest> y = x;
	y.detach ();

	CMyScheduler scheduler;

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
		> function (test, -1, -2);

	function.invoke (0, 100, 200, 300);
}

*/

//.............................................................................

} // namespace test_Scheduler
