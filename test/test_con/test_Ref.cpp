#include "pch.h"

namespace test_Ref { 

//.............................................................................

class MyClass
{
public:
	int m_x;
	int m_y;

public:
	MyClass ()
	{
		printf ("CMyClass::CMyClass ()\n");
		m_x = 0;
		m_y = 0;
	}

	MyClass (const MyClass& src)
	{
		printf ("CMyClass::CMyClass (const CMyClass&)\n");
		m_x = src.m_x;
		m_y = src.m_y;
	}

	~MyClass ()
	{
		printf ("CMyClass::~CMyClass ()\n");
	}

	MyClass&
	operator = (const MyClass& src)
	{
		printf ("CMyClass::operator = (const CMyClass&)\n");
		m_x = src.m_x;
		m_y = src.m_y;

		return *this;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DcbGetSize
{
public:
	size_t 
	operator () (const DCB& dcb)
	{
		return dcb.DCBlength;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void 
run_Buf ()
{
	sl::String s;
	s = "sdkjhasdljadljasdklj";
	char x = s [10];

	s.clear ();

	sl::Array <uchar_t> arr;
	arr.setCount (10);

	sl::Array <uchar_t> arr2 = arr;
	arr2 [0] = 10;

	ref::Buf <DCB, DcbGetSize> dcb;
	dcb.getBuffer ();
	dcb->DCBlength = 240;
	dcb->BaudRate = 38400;

	ref::Buf <DCB, DcbGetSize> dcb2 = dcb;
	dcb2.getBuffer ();

	MyClass a;
	a.m_x = 1;
	a.m_y = 2;

	ref::Buf <MyClass> b1;
	
	b1 = a;
	b1->m_x = 10;
	b1->m_y = 20;

	ref::Buf <MyClass> b2 = b1;

	b1.getBuffer ();
	b1.release ();
	b2.release ();
//	b1.GetBuffer ();

	//b2 = b1;
}

//.............................................................................

class MyClass2: public ref::RefCount
{
public:
	MyClass2 ()
	{
		printf ("CMyClass2::CMyClass2 ()\n");
	}

	~MyClass2 ()
	{
		printf ("CMyClass2::CMyClass2 ()\n");
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run_WeakRef ()
{
	ref::Ptr <MyClass2> p = AXL_REF_NEW (MyClass2);
//	ref::CPtrT <CMyClass2> p2 = ref::GetPtrOrClone ((CMyClass2*) p);
//	ref::CPtrT <CMyClass2> p3 = ref::Clone (p);
	ref::WeakPtr <MyClass2> w = p;
	p = ref::PtrKind_Null;
	p = w;
}

//.............................................................................

class C1
{
	operator int* ()
	{
		return NULL;
	}
};

template <typename T>
void tempFunc (T* p)
{
}

class MyClass3: public ref::RefCount
{
public:
	int m_x;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run_Clone ()
{
	ref::Ptr <MyClass3> a, b;

	a = AXL_REF_NEW (MyClass3);
	a->m_x = 10;

	a->addRef ();
	a->addWeakRef ();

//	b = ref::Clone (a);
}

//.............................................................................

struct IMyInterface
{
	virtual ref::Ptr <IMyInterface> getChild (int i) = 0;
	virtual void doSomething () = 0;
	virtual void doSomethingElse () = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MyClass4: public IMyInterface
{
public:
	MyClass4 ()
		{ printf ("CMyClass4::CMyClass4 ()\n"); }

	~MyClass4 ()
		{ printf ("CMyClass4::~CMyClass4 ()\n"); }

	ref::Ptr <IMyInterface> getChild (int i)
		{ return ref::PtrKind_Null; }

	virtual void doSomething ()
		{ printf ("CMyClass4::DoSomething ()\n"); }

	virtual void doSomethingElse ()
		{ printf ("CMyClass4::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MyContainer: 
	public ref::RefCount,
	public IMyInterface
{
public:
	ref::Child <ref::Box <MyClass4> > m_child1;
	ref::Child <ref::Box <MyClass4> > m_child2;
	ref::Child <ref::Box <MyClass4> > m_child3;

public:
	MyContainer () :
		m_child1 (this),
		m_child2 (this),
		m_child3 (this)
		{ printf ("CMyContainer::CMyContainer ()\n"); }

	~MyContainer ()
		{ printf ("CMyContainer::~CMyContainer ()\n"); }

	ref::Ptr <IMyInterface> getChild (int i)
	{ 
		switch (i)
		{
		case 0:
			return &m_child1;

		case 1:
			return &m_child2;

		case 2:
			return &m_child3;

		default:
			return ref::PtrKind_Null;
		}
	}

	virtual void doSomething ()
		{ printf ("CMyContainer::DoSomething ()\n"); }

	virtual void doSomethingElse ()
		{ printf ("CMyContainer::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MySuperContainer: 
	public ref::RefCount,
	public IMyInterface
{
public:
	ref::Child <MyContainer> m_child1;
	ref::Child <MyContainer> m_child2;
	ref::Child <ref::Box <MyClass4> > m_child3;

public:
	MySuperContainer () :
		m_child1 (this),
		m_child2 (this),
		m_child3 (this)
		{ printf ("CMySuperContainer::CMySuperContainer ()\n"); }

	~MySuperContainer ()
		{ printf ("CMySuperContainer::~CMySuperContainer ()\n"); }

	ref::Ptr <IMyInterface> getChild (int i)
	{ 
		switch (i)
		{
		case 0:
			return &m_child1;

		case 1:
			return &m_child2;

		case 2:
			return &m_child3;

		default:
			return ref::PtrKind_Null;
		}
	}

	virtual void doSomething ()
		{ printf ("CMySuperContainer::DoSomething ()\n"); }

	virtual void doSomethingElse ()
		{ printf ("CMySuperContainer::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void 
run_Container () 
{
	ref::Ptr <IMyInterface> p;
	ref::Ptr <IMyInterface> p2;
	ref::Ptr <IMyInterface> p3;

	p = AXL_REF_NEW (MySuperContainer);
	p->doSomething ();
	p->doSomethingElse ();

	p->getChild (1)->doSomething ();
	p->getChild (1)->doSomethingElse ();

	p2 = p->getChild (2);

	p->getChild (2)->doSomething ();
	p->getChild (2)->doSomethingElse (); 

	p = ref::PtrKind_Null;

	p2->doSomething ();
	p2 = ref::PtrKind_Null;

	p3 = AXL_REF_NEW (ref::Box <MyClass4>);

	ref::WeakPtr <IMyInterface> w = p3;
	p3->doSomethingElse ();	

	p3 = ref::PtrKind_Null;

	p = w;

//	w.Detach ();
}

//.............................................................................

} // namespace test_Ref
