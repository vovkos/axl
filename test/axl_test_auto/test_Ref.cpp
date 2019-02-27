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

class MyClass
{
public:
	int m_x;
	int m_y;

public:
	MyClass()
	{
		printf("MyClass::MyClass ()\n");
		m_x = 0;
		m_y = 0;
	}

	MyClass(const MyClass& src)
	{
		printf("MyClass::MyClass (const MyClass&)\n");
		m_x = src.m_x;
		m_y = src.m_y;
	}

	~MyClass()
	{
		printf("MyClass::~MyClass ()\n");
	}

	MyClass&
	operator = (const MyClass& src)
	{
		printf("MyClass::operator = (const MyClass&)\n");
		m_x = src.m_x;
		m_y = src.m_y;

		return *this;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_OS_WIN)

class DcbGetSize
{
public:
	size_t
	operator () (const DCB* dcb)
	{
		return dcb->DCBlength;
	}
};

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Point
{
public:
	int x;
	int y;

public:
	Point()
	{
		printf("Point::Point ()\n");
		x = 10;
		y = 20;
	}

	~Point()
	{
		printf("Point::~Point ()\n");
	}
};

typedef ref::Buf<Point> PointBuf;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run_Buf()
{
	sl::String s;
	s = "sdkjhasdljadljasdklj";
	char x = s[10];

	s.clear();

	sl::Array<uchar_t> arr;
	arr.setCount(10);

	sl::Array<uchar_t> arr2 = arr;
	arr2[0] = 10;

#if (_AXL_OS_WIN)
	ref::Buf<DCB, DcbGetSize> dcb;
	dcb.getBuffer();
	dcb->DCBlength = 240;
	dcb->BaudRate = 38400;

	ref::Buf<DCB, DcbGetSize> dcb2 = dcb;
	dcb2.getBuffer();
#endif
	MyClass a;
	a.m_x = 1;
	a.m_y = 2;

	ref::Buf<MyClass> b1;

	b1 = &a;
	b1->m_x = 10;
	b1->m_y = 20;

	ref::Buf<MyClass> b2 = b1;

	b1.getBuffer();
	b1.release();
	b2.release();
//	b1.GetBuffer ();

	//b2 = b1;

	size_t bufSize = sizeof(Point) + sizeof(ref::BufHdr);
	PointBuf point(ref::BufKind_Stack, _alloca(bufSize), bufSize);
	point.createBuffer();

	PointBuf point2 = point;

	printf("point: %p, %d, %d\n", point.cp (), point->x, point->y);
	printf("point2: %p, %d, %d\n", point.cp (), point2->x, point2->y);

	point2->x *= 10;
	point2->y *= 10;

	printf("point: %d, %d\n", point->x, point->y);
	printf("point2: %d, %d\n", point2->x, point2->y);

	char buf[80];
	sl::String string(ref::BufKind_Stack, buf, sizeof(buf));

	string.format("hui %d %d %d", 1, 2, 3);
	string.append("govno");
	string.append("muravei");

}

//..............................................................................

class MyClass2: public ref::RefCount
{
public:
	MyClass2()
	{
		printf("MyClass2::MyClass2 ()\n");
	}

	~MyClass2()
	{
		printf("MyClass2::MyClass2 ()\n");
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run_WeakRef()
{
	ref::Ptr<MyClass2> p = AXL_REF_NEW(MyClass2);
//	ref::CPtrT <MyClass2> p2 = ref::GetPtrOrClone ((MyClass2*) p);
//	ref::CPtrT <MyClass2> p3 = ref::Clone (p);
	ref::WeakPtr<MyClass2> w = p;
	p = ref::g_nullPtr;
	p = w;
}

//..............................................................................

struct IMyInterface
{
	virtual ref::Ptr<IMyInterface> getChild(int i) = 0;
	virtual void doSomething() = 0;
	virtual void doSomethingElse() = 0;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MyClass4: public IMyInterface
{
public:
	MyClass4()
		{ printf("MyClass4::MyClass4 ()\n"); }

	~MyClass4()
		{ printf("MyClass4::~MyClass4 ()\n"); }

	ref::Ptr<IMyInterface> getChild(int i)
		{ return ref::g_nullPtr; }

	virtual void doSomething()
		{ printf("MyClass4::doSomething ()\n"); }

	virtual void doSomethingElse()
		{ printf("MyClass4::doSomethingElse ()\n"); }
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MyContainer:
	public ref::RefCount,
	public IMyInterface
{
public:
	ref::Child<ref::Box<MyClass4> > m_child1;
	ref::Child<ref::Box<MyClass4> > m_child2;
	ref::Child<ref::Box<MyClass4> > m_child3;

public:
	MyContainer() :
		m_child1(this),
		m_child2(this),
		m_child3(this)
		{ printf("MyContainer::MyContainer ()\n"); }

	~MyContainer()
		{ printf("MyContainer::~MyContainer ()\n"); }

	ref::Ptr<IMyInterface> getChild(int i)
	{
		switch(i)
		{
		case 0:
			return &m_child1;

		case 1:
			return &m_child2;

		case 2:
			return &m_child3;

		default:
			return ref::g_nullPtr;
		}
	}

	virtual void doSomething()
		{ printf("MyContainer::doSomething ()\n"); }

	virtual void doSomethingElse()
		{ printf("MyContainer::doSomethingElse ()\n"); }
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MySuperContainer:
	public ref::RefCount,
	public IMyInterface
{
public:
	ref::Child<MyContainer> m_child1;
	ref::Child<MyContainer> m_child2;
	ref::Child<ref::Box<MyClass4> > m_child3;

public:
	MySuperContainer() :
		m_child1(this),
		m_child2(this),
		m_child3(this)
		{ printf("MySuperContainer::MySuperContainer ()\n"); }

	~MySuperContainer()
		{ printf("MySuperContainer::~MySuperContainer ()\n"); }

	ref::Ptr<IMyInterface> getChild(int i)
	{
		switch(i)
		{
		case 0:
			return &m_child1;

		case 1:
			return &m_child2;

		case 2:
			return &m_child3;

		default:
			return ref::g_nullPtr;
		}
	}

	virtual void doSomething()
		{ printf("MySuperContainer::doSomething ()\n"); }

	virtual void doSomethingElse()
		{ printf("MySuperContainer::doSomethingElse ()\n"); }
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run_Container()
{
	ref::Ptr<IMyInterface> p;
	ref::Ptr<IMyInterface> p2;
	ref::Ptr<IMyInterface> p3;

	p = AXL_REF_NEW(MySuperContainer);
	p->doSomething();
	p->doSomethingElse();

	p->getChild(1)->doSomething();
	p->getChild(1)->doSomethingElse();

//	p2 = p->getChild (2);

	p->getChild(2)->doSomething();
	p->getChild(2)->doSomethingElse();

	p = ref::g_nullPtr;

//	p2->doSomething ();
//	p2 = ref::g_nullPtr;

	p3 = AXL_REF_NEW(ref::Box<MyClass4>);

	ref::WeakPtr<IMyInterface> w = p3;
	p3->doSomethingElse();

	p3 = ref::g_nullPtr;

	p = w;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run()
{
	run_Buf();
	run_WeakRef();
	run_Container();
}

//..............................................................................

ADD_TEST_CASE("test_Ref", run)

} // namespace test_Ref
