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
run ()
{
	size_t bufSize = sizeof (CPoint) + sizeof (CPointBuf::CHdr);
	CPointBuf point (ref::EBuf_Stack, _alloca (bufSize), bufSize);
	CPointBuf point2 = point;

	printf ("point: %d, %d\n", point->x, point->y);
	printf ("point2: %d, %d\n", point2->x, point2->y);

	point2->x *= 10;
	point2->y *= 10;

	printf ("point: %d, %d\n", point->x, point->y);
	printf ("point2: %d, %d\n", point2->x, point2->y);

	char buf [80];
	rtl::CString string (ref::EBuf_Stack, buf, sizeof (buf));

	string.format ("hui %d %d %d", 1, 2, 3);
	string.append ("govno");
	string.append ("muravei");
}

//.............................................................................

} // namespace test_String
