#include "pch.h"

using namespace axl;

namespace test_List {

//.............................................................................

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

//.............................................................................

void
Run_BoxList ()
{
	rtl::CBoxListT <int> List;
	List.InsertTail (10);
	List.InsertHead (-10);
	List.InsertTail (20);
	List.InsertHead (-20);

	List.RemoveHead ();

	rtl::CBoxIteratorT <int> It = List.GetHead ();
	for (; It; It++)
	{
		printf ("%d\n", *It);
	}
}

//.............................................................................

} // namespace test_List
