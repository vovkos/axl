#include "pch.h"

using namespace axl;

namespace test_String { 

//.............................................................................


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

typedef ref::CBufT <CPoint> CPointBuf;

void 
Run ()
{
	size_t BufSize = sizeof (CPoint) + sizeof (CPointBuf::CHdr);
	CPointBuf Point (ref::EBuf_Stack, _alloca (BufSize), BufSize);
	CPointBuf Point2 = Point;

	printf ("point: %d, %d\n", Point->x, Point->y);
	printf ("point2: %d, %d\n", Point2->x, Point2->y);

	Point2->x *= 10;
	Point2->y *= 10;

	printf ("point: %d, %d\n", Point->x, Point->y);
	printf ("point2: %d, %d\n", Point2->x, Point2->y);

	char Buf [80];
	rtl::CString String (ref::EBuf_Stack, Buf, sizeof (Buf));

	String.Format ("hui %d %d %d", 1, 2, 3);
	String.Append ("govno");
	String.Append ("muravei");
}

//.............................................................................

} // namespace test_String
