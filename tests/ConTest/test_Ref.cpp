#include "pch.h"

using namespace axl;

namespace test_Ref { 

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

	CMyClass (const CMyClass& src)
	{
		printf ("CMyClass::CMyClass (const CMyClass&)\n");
		m_x = src.m_x;
		m_y = src.m_y;
	}

	~CMyClass ()
	{
		printf ("CMyClass::~CMyClass ()\n");
	}

	CMyClass&
	operator = (const CMyClass& src)
	{
		printf ("CMyClass::operator = (const CMyClass&)\n");
		m_x = src.m_x;
		m_y = src.m_y;

		return *this;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CDcbGetSize
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
	rtl::CString s;
	s = "sdkjhasdljadljasdklj";
	char x = s [10];

	s.clear ();

	rtl::CArrayT <uchar_t> arr;
	arr.setCount (10);

	rtl::CArrayT <uchar_t> arr2 = arr;
	arr2 [0] = 10;

	ref::CBufT <DCB, CDcbGetSize> dcb;
	dcb.getBuffer ();
	dcb->DCBlength = 240;
	dcb->BaudRate = 38400;

	ref::CBufT <DCB, CDcbGetSize> dcb2 = dcb;
	dcb2.getBuffer ();

	CMyClass a;
	a.m_x = 1;
	a.m_y = 2;

	ref::CBufT <CMyClass> b1;
	
	b1 = a;
	b1->m_x = 10;
	b1->m_y = 20;

	ref::CBufT <CMyClass> b2 = b1;

	b1.getBuffer ();
	b1.release ();
	b2.release ();
//	b1.GetBuffer ();

	//b2 = b1;
}

//.............................................................................

class CMyClass2: public ref::CRefCount
{
public:
	CMyClass2 ()
	{
		printf ("CMyClass2::CMyClass2 ()\n");
	}

	~CMyClass2 ()
	{
		printf ("CMyClass2::CMyClass2 ()\n");
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run_WeakRef ()
{
	ref::CPtrT <CMyClass2> p = AXL_REF_NEW (CMyClass2);
//	ref::CPtrT <CMyClass2> p2 = ref::GetPtrOrClone ((CMyClass2*) p);
//	ref::CPtrT <CMyClass2> p3 = ref::Clone (p);
	ref::CWeakPtrT <CMyClass2> w = p;
	p = ref::EPtr_Null;
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

class CMyClass3: public ref::CRefCount
{
public:
	int m_x;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
run_Clone ()
{
	ref::CPtrT <CMyClass3> a, b;

	a = AXL_REF_NEW (CMyClass3);
	a->m_x = 10;

	a->addRef ();
	a->addWeakRef ();

//	b = ref::Clone (a);
}

//.............................................................................

struct IMyInterface
{
	virtual ref::CPtrT <IMyInterface> getChild (int i) = 0;
	virtual void doSomething () = 0;
	virtual void doSomethingElse () = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMyClass4: public IMyInterface
{
public:
	CMyClass4 ()
		{ printf ("CMyClass4::CMyClass4 ()\n"); }

	~CMyClass4 ()
		{ printf ("CMyClass4::~CMyClass4 ()\n"); }

	ref::CPtrT <IMyInterface> getChild (int i)
		{ return ref::EPtr_Null; }

	virtual void doSomething ()
		{ printf ("CMyClass4::DoSomething ()\n"); }

	virtual void doSomethingElse ()
		{ printf ("CMyClass4::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMyContainer: 
	public ref::CRefCount,
	public IMyInterface
{
public:
	ref::CChildT <ref::CBoxT <CMyClass4> > m_child1;
	ref::CChildT <ref::CBoxT <CMyClass4> > m_child2;
	ref::CChildT <ref::CBoxT <CMyClass4> > m_child3;

public:
	CMyContainer () :
		m_child1 (this),
		m_child2 (this),
		m_child3 (this)
		{ printf ("CMyContainer::CMyContainer ()\n"); }

	~CMyContainer ()
		{ printf ("CMyContainer::~CMyContainer ()\n"); }

	ref::CPtrT <IMyInterface> getChild (int i)
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
			return ref::EPtr_Null;
		}
	}

	virtual void doSomething ()
		{ printf ("CMyContainer::DoSomething ()\n"); }

	virtual void doSomethingElse ()
		{ printf ("CMyContainer::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMySuperContainer: 
	public ref::CRefCount,
	public IMyInterface
{
public:
	ref::CChildT <CMyContainer> m_child1;
	ref::CChildT <CMyContainer> m_child2;
	ref::CChildT <ref::CBoxT <CMyClass4> > m_child3;

public:
	CMySuperContainer () :
		m_child1 (this),
		m_child2 (this),
		m_child3 (this)
		{ printf ("CMySuperContainer::CMySuperContainer ()\n"); }

	~CMySuperContainer ()
		{ printf ("CMySuperContainer::~CMySuperContainer ()\n"); }

	ref::CPtrT <IMyInterface> getChild (int i)
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
			return ref::EPtr_Null;
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
	ref::CPtrT <IMyInterface> p;
	ref::CPtrT <IMyInterface> p2;
	ref::CPtrT <IMyInterface> p3;

	p = AXL_REF_NEW (CMySuperContainer);
	p->doSomething ();
	p->doSomethingElse ();

	p->getChild (1)->doSomething ();
	p->getChild (1)->doSomethingElse ();

	p2 = p->getChild (2);

	p->getChild (2)->doSomething ();
	p->getChild (2)->doSomethingElse (); 

	p = ref::EPtr_Null;

	p2->doSomething ();
	p2 = ref::EPtr_Null;

	p3 = AXL_REF_NEW (ref::CBoxT <CMyClass4>);

	ref::CWeakPtrT <IMyInterface> w = p3;
	p3->doSomethingElse ();	

	p3 = ref::EPtr_Null;

	p = w;

//	w.Detach ();
}

//.............................................................................

} // namespace test_Ref
