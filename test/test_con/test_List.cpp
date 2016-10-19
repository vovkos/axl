#include "pch.h"

namespace test_List {

//..............................................................................

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

//..............................................................................

void
run_BoxList ()
{
	sl::BoxList <int> list;
	list.insertTail (10);
	list.insertHead (-10);
	list.insertTail (20);
	list.insertHead (-20);

	list.removeHead ();

	sl::BoxIterator <int> it = list.getHead ();
	for (; it; it++)
	{
		printf ("%d\n", *it);
	}
}

//..............................................................................

} // namespace test_List
