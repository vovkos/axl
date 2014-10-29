#include "pch.h"

namespace test_Function {

//.............................................................................

////.............................................................................
//
//int MyFunction (const char* p1, const char* p2, const char* p3)
//{
//	return printf ("%s %s %s\n", p1, p2, p3);
//}
//
//int __stdcall MyFunction2 (int x1, int x2, int x3)
//{
//	return printf ("%d %d %d\n", x1, x2, x3);
//}
//
////.............................................................................
//
//void FunctionTest ()
//{
//	CFunctionT <
//		MyFunction, 
//		exe::ECallingConvention_Cdecl,
//		rtl::IPackImplT_3 (
//			CArgT <rtl::CString>,
//			CArgString,
//			CArgString
//			> > Function;
//
//	CPtrT <mem::TBlock> Stack = Function.GetArgPacker ()->BuildFromArgs (
//		0, 
//		rtl::CString ("hui"), 
//		rtl::CString ("govno"), 
//		rtl::CString ("muravei")
//		);
//
//	Function.Invoke (Stack->m_p, Stack->m_Size);
//
//	CFunctionT <
//		MyFunction2, 
//		exe::ECallingConvention_Stdcall,
//		rtl::IPackImplT_3 (
//			CArgT <int>,
//			CArgT <int>,
//			CArgT <int>
//			> > Function2;
//
//	Stack = Function2.GetArgPacker ()->BuildFromArgs (
//		0, 
//		10, 
//		20, 
//		30
//		);
//
//	Function2.Invoke (Stack->m_p, Stack->m_Size);
//}
//
////.............................................................................
//


//
//template <typename T>
//struct CArgValueAdapterT
//{
//	typedef T TArgType;
//
//	enum
//	{
//		StackSize = sizeof (T),
//		HeapSize  = 0,
//	};
//};
//
//template <typename A1>
//class CStackFramePackT_1: public rtl::IPack
//{
//public:
//	CStackFramePackT_1 ()
//		{ m_Block.m_p = NULL, m_Block.m_Size = 0; }
//};
//
//template <typename A1, typename A2>
//class CStackFramePackT_2: public rtl::IPack
//{
//public:
//};
//

//.............................................................................

} // namespace test_Function 
