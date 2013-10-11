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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CDcbGetSize
{
public:
	size_t 
	operator () (const DCB& Dcb)
	{
		return Dcb.DCBlength;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void 
Run_Buf ()
{
	rtl::CString s;
	s = "sdkjhasdljadljasdklj";
	char x = s [10];

	s.Clear ();

	rtl::CArrayT <uchar_t> arr;
	arr.SetCount (10);

	rtl::CArrayT <uchar_t> arr2 = arr;
	arr2 [0] = 10;

	ref::CBufT <DCB, CDcbGetSize> Dcb;
	Dcb.GetBuffer ();
	Dcb->DCBlength = 240;
	Dcb->BaudRate = 38400;

	ref::CBufT <DCB, CDcbGetSize> Dcb2 = Dcb;
	Dcb2.GetBuffer ();

	CMyClass a;
	a.m_x = 1;
	a.m_y = 2;

	ref::CBufT <CMyClass> b1;
	
	b1 = a;
	b1->m_x = 10;
	b1->m_y = 20;

	ref::CBufT <CMyClass> b2 = b1;

	b1.GetBuffer ();
	b1.Release ();
	b2.Release ();
//	b1.GetBuffer ();

	//b2 = b1;
}

//.............................................................................

class CMyClass2: public ref::IRefCount
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
Run_WeakRef ()
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
void TempFunc (T* p)
{
}

class CMyClass3: public ref::IRefCount
{
public:
	int m_x;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
Run_Clone ()
{
	ref::CPtrT <CMyClass3> a, b;

	a = AXL_REF_NEW (CMyClass3);
	a->m_x = 10;

	a->AddRef ();
	a->AddWeakRef ();

//	b = ref::Clone (a);
}

//.............................................................................

struct IMyInterface
{
	virtual ref::CPtrT <IMyInterface> GetChild (int i) = 0;
	virtual void DoSomething () = 0;
	virtual void DoSomethingElse () = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMyClass4: public IMyInterface
{
public:
	CMyClass4 ()
		{ printf ("CMyClass4::CMyClass4 ()\n"); }

	~CMyClass4 ()
		{ printf ("CMyClass4::~CMyClass4 ()\n"); }

	ref::CPtrT <IMyInterface> GetChild (int i)
		{ return ref::EPtr_Null; }

	virtual void DoSomething ()
		{ printf ("CMyClass4::DoSomething ()\n"); }

	virtual void DoSomethingElse ()
		{ printf ("CMyClass4::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMyContainer: 
	public ref::IRefCount,
	public IMyInterface
{
public:
	ref::CChildT <ref::CBoxT <CMyClass4> > m_Child1;
	ref::CChildT <ref::CBoxT <CMyClass4> > m_Child2;
	ref::CChildT <ref::CBoxT <CMyClass4> > m_Child3;

public:
	CMyContainer () :
		m_Child1 (this),
		m_Child2 (this),
		m_Child3 (this)
		{ printf ("CMyContainer::CMyContainer ()\n"); }

	~CMyContainer ()
		{ printf ("CMyContainer::~CMyContainer ()\n"); }

	ref::CPtrT <IMyInterface> GetChild (int i)
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
			return ref::EPtr_Null;
		}
	}

	virtual void DoSomething ()
		{ printf ("CMyContainer::DoSomething ()\n"); }

	virtual void DoSomethingElse ()
		{ printf ("CMyContainer::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMySuperContainer: 
	public ref::IRefCount,
	public IMyInterface
{
public:
	ref::CChildT <CMyContainer> m_Child1;
	ref::CChildT <CMyContainer> m_Child2;
	ref::CChildT <ref::CBoxT <CMyClass4> > m_Child3;

public:
	CMySuperContainer () :
		m_Child1 (this),
		m_Child2 (this),
		m_Child3 (this)
		{ printf ("CMySuperContainer::CMySuperContainer ()\n"); }

	~CMySuperContainer ()
		{ printf ("CMySuperContainer::~CMySuperContainer ()\n"); }

	ref::CPtrT <IMyInterface> GetChild (int i)
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
			return ref::EPtr_Null;
		}
	}

	virtual void DoSomething ()
		{ printf ("CMySuperContainer::DoSomething ()\n"); }

	virtual void DoSomethingElse ()
		{ printf ("CMySuperContainer::DoSomethingElse ()\n"); }
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void 
Run_Container () 
{
	ref::CPtrT <IMyInterface> p;
	ref::CPtrT <IMyInterface> p2;
	ref::CPtrT <IMyInterface> p3;

	p = AXL_REF_NEW (CMySuperContainer);
	p->DoSomething ();
	p->DoSomethingElse ();

	p->GetChild (1)->DoSomething ();
	p->GetChild (1)->DoSomethingElse ();

	p2 = p->GetChild (2);

	p->GetChild (2)->DoSomething ();
	p->GetChild (2)->DoSomethingElse (); 

	p = ref::EPtr_Null;

	p2->DoSomething ();
	p2 = ref::EPtr_Null;

	p3 = AXL_REF_NEW (ref::CBoxT <CMyClass4>);

	ref::CWeakPtrT <IMyInterface> w = p3;
	p3->DoSomethingElse ();	

	p3 = ref::EPtr_Null;

	p = w;

//	w.Detach ();
}

//.............................................................................

} // namespace test_Ref
