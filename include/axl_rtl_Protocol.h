// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_PROTOCOL_H

#include "axl_rtl_String.h"

namespace axl {
namespace rtl {

//.............................................................................

struct TProtoMsg
{
	uint32_t m_Code;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
struct TProtoMsgT_1: TProtoMsg
{
	T m_Arg1;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2
	>
struct TProtoMsgT_2: TProtoMsg
{
	T1 m_Arg1;
	T2 m_Arg2;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2,
	typename T3
	>
struct TProtoMsgT_3: TProtoMsg
{
	T1 m_Arg1;
	T2 m_Arg2;
	T3 m_Arg3;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2,
	typename T3,
	typename T4
	>
struct TProtoMsgT_4: TProtoMsg
{
	T1 m_Arg1;
	T2 m_Arg2;
	T3 m_Arg3;
	T4 m_Arg4;
};

//.............................................................................

class CProtoPeer
{
public:
	virtual
	void
	SendMsg (
		const TProtoMsg* pMsg,
		size_t MsgSize,
		const void* pExtra,
		size_t ExtraSize
		) = 0;

	void
	SendMsg (
		const TProtoMsg* pMsg,
		size_t MsgSize
		)
	{
		SendMsg (pMsg, MsgSize, NULL, 0);
	}
};

//.............................................................................

// convenient macros for sending messages (up to 4 args)

#define AXL_RTL_PROTO_SEND_FUNCTION_0(Code, Sender) \
	void \
	Sender () \
	{ \
		axl::rtl::TProtoMsg Msg; \
		Msg.m_Code = Code; \
		SendMsg (&Msg, sizeof (Msg)); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_0_STR(Code, Sender, StrArg) \
	void \
	Sender (const char* StrArg) \
	{ \
		size_t Length = StrArg ? strlen (StrArg) : 0; \
		axl::rtl::TProtoMsg Msg; \
		Msg.m_Code = Code; \
		SendMsg (&Msg, sizeof (Msg), StrArg, Length); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_0_PTR(Code, Sender, PtrArg, SizeArg) \
	void \
	Sender ( \
		const void* PtrArg, \
		size_t SizeArg \
		) \
	{ \
		axl::rtl::TProtoMsg Msg; \
		Msg.m_Code = Code; \
		SendMsg (&Msg, sizeof (Msg), PtrArg, SizeArg); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_0_ERR(Code, Sender, ErrArg) \
	void \
	Sender (const axl::err::TError* ErrArg) \
	{ \
		axl::rtl::TProtoMsg Msg; \
		Msg.m_Code = Code; \
		SendMsg (&Msg, sizeof (Msg), ErrArg, ErrArg->m_Size); \
	}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_SEND_FUNCTION_1(Code, Sender, T1, Arg1) \
	void \
	Sender (T1 Arg1) \
	{ \
		axl::rtl::TProtoMsgT_1 <T1> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		SendMsg (&Msg, sizeof (Msg)); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_1_STR(Code, Sender, T1, Arg1, StrArg) \
	void \
	Sender ( \
		T1 Arg1, \
		const char* StrArg \
		) \
	{ \
		size_t Length = StrArg ? strlen (StrArg) : 0; \
		axl::rtl::TProtoMsgT_1 <T1> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		SendMsg (&Msg, sizeof (Msg), StrArg, Length); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_1_PTR(Code, Sender, T1, Arg1, PtrArg, SizeArg) \
	void \
	Sender ( \
		T1 Arg1, \
		const void* PtrArg, \
		size_t SizeArg \
		) \
	{ \
		axl::rtl::TProtoMsgT_1 <T1> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		SendMsg (&Msg, sizeof (Msg), PtrArg, SizeArg); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_1_ERR(Code, Sender, T1, Arg1, ErrArg) \
	void \
	Sender ( \
		T1 Arg1, \
		const axl::err::TError* ErrArg \
		) \
	{ \
		axl::rtl::TProtoMsgT_1 <T1> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		SendMsg (&Msg, sizeof (Msg), ErrArg, ErrArg->m_Size); \
	}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_SEND_FUNCTION_2(Code, Sender, T1, Arg1, T2, Arg2) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2 \
		) \
	{ \
		axl::rtl::TProtoMsgT_2 <T1, T2> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		SendMsg (&Msg, sizeof (Msg)); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_2_STR(Code, Sender, T1, Arg1, T2, Arg2, StrArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		const char* StrArg \
		) \
	{ \
		size_t Length = StrArg ? strlen (StrArg) : 0; \
		axl::rtl::TProtoMsgT_2 <T1, T2> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		SendMsg (&Msg, sizeof (Msg), StrArg, Length); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_2_PTR(Code, Sender, T1, Arg1, T2, Arg2, PtrArg, SizeArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		const void* PtrArg, \
		size_t SizeArg \
		) \
	{ \
		axl::rtl::TProtoMsgT_2 <T1, T2> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		SendMsg (&Msg, sizeof (Msg), PtrArg, SizeArg); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_2_ERR(Code, Sender, T1, Arg1, T2, Arg2, ErrArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		const axl::err::TError* ErrArg \
		) \
	{ \
		axl::rtl::TProtoMsgT_2 <T1, T2> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		SendMsg (&Msg, sizeof (Msg), ErrArg, ErrArg->m_Size); \
	}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_SEND_FUNCTION_3(Code, Sender, T1, Arg1, T2, Arg2, T3, Arg3) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		T2 Arg3 \
		) \
	{ \
		axl::rtl::TProtoMsgT_3 <T1, T2, T3> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		Msg.m_Arg3 = Arg3; \
		SendMsg (&Msg, sizeof (Msg)); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_3_STR(Code, Sender, T1, Arg1, T2, Arg2, T3, Arg3, StrArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		T3 Arg3, \
		const char* StrArg \
		) \
	{ \
		size_t Length = StrArg ? strlen (StrArg) : 0; \
		axl::rtl::TProtoMsgT_3 <T1, T2, T3> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		Msg.m_Arg3 = Arg3; \
		SendMsg (&Msg, sizeof (Msg), StrArg, Length); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_3_PTR(Code, Sender, T1, Arg1, T2, Arg2, T3, Arg3, PtrArg, SizeArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		T3 Arg3, \
		const void* PtrArg, \
		size_t SizeArg \
		) \
	{ \
		axl::rtl::TProtoMsgT_3 <T1, T2, T3> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		Msg.m_Arg3 = Arg3; \
		SendMsg (&Msg, sizeof (Msg), PtrArg, SizeArg); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_3_ERR(Code, Sender, T1, Arg1, T2, Arg2, T3, Arg3, ErrArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		T3 Arg3, \
		const axl::err::TError* ErrArg \
		) \
	{ \
		axl::rtl::TProtoMsgT_3 <T1, T2, T3> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		Msg.m_Arg3 = Arg3; \
		SendMsg (&Msg, sizeof (Msg), ErrArg, ErrArg->m_Size); \
	}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_SEND_FUNCTION_4(Code, Sender, T1, Arg1, T2, Arg2, T3, Arg3, T4, Arg4) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		T3 Arg3, \
		T4 Arg4 \
		) \
	{ \
		axl::rtl::TProtoMsgT_4 <T1, T2, T3, T4> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		Msg.m_Arg3 = Arg3; \
		Msg.m_Arg4 = Arg4; \
		SendMsg (&Msg, sizeof (Msg)); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_4_STR(Code, Sender, T1, Arg1, T2, Arg2, T3, Arg3, T4, Arg4, StrArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		T3 Arg3, \
		T4 Arg4, \
		const char* StrArg \
		) \
	{ \
		size_t Length = StrArg ? strlen (StrArg) : 0; \
		axl::rtl::TProtoMsgT_4 <T1, T2, T3, T4> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		Msg.m_Arg3 = Arg3; \
		Msg.m_Arg4 = Arg4; \
		SendMsg (&Msg, sizeof (Msg), StrArg, Length); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_4_PTR(Code, Sender, T1, Arg1, T2, Arg2, T3, Arg3, T4, Arg4, PtrArg, SizeArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		T3 Arg3, \
		T4 Arg4, \
		const void* PtrArg, \
		size_t SizeArg \
		) \
	{ \
		axl::rtl::TProtoMsgT_4 <T1, T2, T3, T4> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		Msg.m_Arg3 = Arg3; \
		Msg.m_Arg4 = Arg4; \
		SendMsg (&Msg, sizeof (Msg), PtrArg, SizeArg); \
	}

#define AXL_RTL_PROTO_SEND_FUNCTION_4_ERR(Code, Sender, T1, Arg1, T2, Arg2, T3, Arg3, T4, Arg4, ErrArg) \
	void \
	Sender ( \
		T1 Arg1, \
		T2 Arg2, \
		T3 Arg3, \
		T4 Arg4, \
		const axl::err::TError* ErrArg \
		) \
	{ \
		axl::rtl::TProtoMsgT_4 <T1, T2, T3, T4> Msg; \
		Msg.m_Code = Code; \
		Msg.m_Arg1 = Arg1; \
		Msg.m_Arg2 = Arg2; \
		Msg.m_Arg3 = Arg3; \
		Msg.m_Arg4 = Arg4; \
		SendMsg (&Msg, sizeof (Msg), ErrArg, ErrArg->m_Size); \
	}

//.............................................................................

// convenient macros for receiving & processing messages (up to 4 args)

#define AXL_RTL_BEGIN_PROTO_RECV_MAP(ProcessMsg) \
	void ProcessMsg ( \
		const void* p, \
		size_t Size \
		) \
	{ \
		char StringBuffer [256]; \
		if (Size < sizeof (axl::rtl::TProtoMsg)) \
			return; \
		axl::rtl::TProtoMsg* pHdr = (axl::rtl::TProtoMsg*) p; \
		switch (pHdr->m_Code) \
		{ \

#define AXL_RTL_END_PROTO_RECV_MAP() \
		} \
	}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_RECV_FUNCTION_0(Code, Handler) \
	case Code: \
		Handler (); \
		break;

#define AXL_RTL_PROTO_RECV_FUNCTION_0_STR(Code, Handler) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsg TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t Length = Size - sizeof (TMsg); \
		axl::rtl::CString String(ref::EBuf_Stack, StringBuffer, sizeof (StringBuffer)); \
		String.Copy ((char*) (pMsg + 1), Length); \
		Handler (String); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_0_PTR(Code, Handler) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsg TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t ExtraSize = Size - sizeof (TMsg); \
		Handler (pMsg + 1, ExtraSize); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_0_ERR(Code, Handler) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsg TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		Handler ((const axl::err::TError*) (pMsg + 1)); \
		break; \
		}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_RECV_FUNCTION_1(Code, Handler, T1) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_1 <T1> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		Handler (pMsg->m_Arg1); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_1_STR(Code, Handler, T1) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_1 <T1> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t Length = Size - sizeof (TMsg); \
		axl::rtl::CString String(ref::EBuf_Stack, StringBuffer, sizeof (StringBuffer)); \
		String.Copy ((char*) (pMsg + 1), Length); \
		Handler (pMsg->m_Arg1, String); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_1_PTR(Code, Handler, T1) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_1 <T1> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t ExtraSize = Size - sizeof (TMsg); \
		Handler (pMsg->m_Arg1, pMsg + 1, ExtraSize); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_1_ERR(Code, Handler, T1) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_1 <T1> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		Handler (pMsg->m_Arg1, (const axl::err::TError*) (pMsg + 1)); \
		break; \
		}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_RECV_FUNCTION_2(Code, Handler, T1, T2) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_2 <T1, T2> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_2_STR(Code, Handler, T1, T2) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_2 <T1, T2> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t Length = Size - sizeof (TMsg); \
		axl::rtl::CString String(ref::EBuf_Stack, StringBuffer, sizeof (StringBuffer)); \
		String.Copy ((char*) (pMsg + 1), Length); \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, String); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_2_PTR(Code, Handler, T1, T2) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_2 <T1, T2> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t ExtraSize = Size - sizeof (TMsg); \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg + 1, ExtraSize); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_2_ERR(Code, Handler, T1, T2) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_2 <T1, T2> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, (const axl::err::TError*) (pMsg + 1)); \
		break; \
		}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_RECV_FUNCTION_3(Code, Handler, T1, T2, T3) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_3 <T1, T2, T3> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg->m_Arg3); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_3_STR(Code, Handler, T1, T2, T3) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_3 <T1, T2, T3> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t Length = Size - sizeof (TMsg); \
		axl::rtl::CString String(ref::EBuf_Stack, StringBuffer, sizeof (StringBuffer)); \
		String.Copy ((char*) (pMsg + 1), Length); \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg->m_Arg3, String); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_3_PTR(Code, Handler, T1, T2, T3) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_3 <T1, T2, T3> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t ExtraSize = Size - sizeof (TMsg); \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg->m_Arg3, pMsg + 1, ExtraSize); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_3_ERR(Code, Handler, T1, T2, T3) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_3 <T1, T2, T3> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t ExtraSize = Size - sizeof (TMsg); \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg->m_Arg3, (const axl::err::TError*) (pMsg + 1)); \
		break; \
		}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RTL_PROTO_RECV_FUNCTION_4(Code, Handler, T1, T2, T3, T4) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_4 <T1, T2, T3, T4> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg->m_Arg3, pMsg->m_Arg4); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_4_STR(Code, Handler, T1, T2, T3, T4) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_4 <T1, T2, T3, T4> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t Length = Size - sizeof (TMsg); \
		axl::rtl::CString String(ref::EBuf_Stack, StringBuffer, sizeof (StringBuffer)); \
		String.Copy ((char*) (pMsg + 1), Length); \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg->m_Arg3, pMsg->m_Arg4, String); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_4_PTR(Code, Handler, T1, T2, T3, T4) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_4 <T1, T2, T3, T4> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		size_t ExtraSize = Size - sizeof (TMsg); \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg->m_Arg3, pMsg->m_Arg4, pMsg + 1, ExtraSize); \
		break; \
		}

#define AXL_RTL_PROTO_RECV_FUNCTION_4_ERR(Code, Handler, T1, T2, T3, T4) \
	case Code: \
		{ \
		typedef axl::rtl::TProtoMsgT_4 <T1, T2, T3, T4> TMsg; \
		TMsg* pMsg = (TMsg*) p; \
		if (Size < sizeof (TMsg)) \
			break; \
		Handler (pMsg->m_Arg1, pMsg->m_Arg2, pMsg->m_Arg3, pMsg->m_Arg4, (const axl::err::TError*) (pMsg + 1)); \
		break; \
		}

//.............................................................................

} // namespace rtl
} // namespace axl
