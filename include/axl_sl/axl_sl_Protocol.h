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

#pragma once

#define _AXL_SL_PROTOCOL_H

#include "axl_sl_String.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class ProtoPeer
{
public:
	void
	sendMsg(
		const void* p,
		size_t size
		)
	{
		static_cast<T*> (this)->sendMsgEx(&p, &size, 1);
	}

	void
	sendMsg(
		const void* p,
		size_t size,
		const void* extra,
		size_t extraSize
		)
	{
		const void* blockArray[] = { p, extra };
		size_t sizeArray[] = { size, extraSize };

		static_cast<T*> (this)->sendMsgEx(blockArray, sizeArray, 2);
	}

	// T must implement:

	//	void
	//	SendMsgEx (
	//		const void* const* pBlockArray,
	// 		size_t const* pSizeArray,
	// 		size_t Count
	// 		);

};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class AbstractProtoPeer: public ProtoPeer<AbstractProtoPeer>
{
public:
	virtual
	void
	sendMsgEx(
		const void* const* blockArray,
		size_t const* sizeArray,
		size_t count
		) = 0;
};

//..............................................................................

struct ProtoMsg
{
	uint32_t m_code;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
struct ProtoMsg_1: ProtoMsg
{
	T m_arg1;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2
	>
struct ProtoMsg_2: ProtoMsg
{
	T1 m_arg1;
	T2 m_arg2;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2,
	typename T3
	>
struct ProtoMsg_3: ProtoMsg
{
	T1 m_arg1;
	T2 m_arg2;
	T3 m_arg3;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2,
	typename T3,
	typename T4
	>
struct ProtoMsg_4: ProtoMsg
{
	T1 m_arg1;
	T2 m_arg2;
	T3 m_arg3;
	T4 m_arg4;
};

//..............................................................................

// convenient macros for sending messages (up to 4 args)

#define AXL_SL_PROTO_SEND_FUNCTION_0(code, sender) \
	void \
	sender() \
	{ \
		axl::sl::ProtoMsg msg; \
		msg.m_code = code; \
		sendMsg(&msg, sizeof(msg)); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_0_STR(code, sender, strArg) \
	void \
	sender(const axl::sl::StringRef& strArg) \
	{ \
		axl::sl::ProtoMsg msg; \
		msg.m_code = code; \
		sendMsg(&msg, sizeof(msg), strArg.cp(), strArg.getLength()); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_0_PTR(code, sender, ptrArg, sizeArg) \
	void \
	sender ( \
		const void* ptrArg, \
		size_t sizeArg \
		) \
	{ \
		axl::sl::ProtoMsg msg; \
		msg.m_code = code; \
		sendMsg(&msg, sizeof(msg), ptrArg, sizeArg); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_0_ERR(code, sender, errArg) \
	void \
	sender(const axl::err::ErrorHdr* errArg) \
	{ \
		axl::sl::ProtoMsg msg; \
		msg.m_code = code; \
		sendMsg(&msg, sizeof(msg), errArg, errArg->m_size); \
	}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_SEND_FUNCTION_1(code, sender, T1, arg1) \
	void \
	sender(T1 arg1) \
	{ \
		axl::sl::ProtoMsg_1<T1> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		sendMsg(&msg, sizeof(msg)); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_1_STR(code, sender, T1, arg1, strArg) \
	void \
	sender ( \
		T1 arg1, \
		const sl::StringRef& strArg \
		) \
	{ \
		axl::sl::ProtoMsg_1<T1> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		sendMsg(&msg, sizeof(msg), strArg.cp(), strArg.getLength()); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_1_PTR(code, sender, T1, arg1, ptrArg, sizeArg) \
	void \
	sender ( \
		T1 arg1, \
		const void* ptrArg, \
		size_t sizeArg \
		) \
	{ \
		axl::sl::ProtoMsg_1<T1> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		sendMsg(&msg, sizeof(msg), ptrArg, sizeArg); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_1_ERR(code, sender, T1, arg1, errArg) \
	void \
	sender ( \
		T1 arg1, \
		const axl::err::ErrorHdr* errArg \
		) \
	{ \
		axl::sl::ProtoMsg_1<T1> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		sendMsg(&msg, sizeof(msg), errArg, errArg->m_size); \
	}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_SEND_FUNCTION_2(code, sender, T1, arg1, T2, arg2) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2 \
		) \
	{ \
		axl::sl::ProtoMsg_2<T1, T2> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		sendMsg(&msg, sizeof(msg)); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_2_STR(code, sender, T1, arg1, T2, arg2, strArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		const sl::StringRef& strArg \
		) \
	{ \
		axl::sl::ProtoMsg_2<T1, T2> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		sendMsg(&msg, sizeof(msg), strArg.cp(), strArg.getLength()); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_2_PTR(code, sender, T1, arg1, T2, arg2, ptrArg, sizeArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		const void* ptrArg, \
		size_t sizeArg \
		) \
	{ \
		axl::sl::ProtoMsg_2<T1, T2> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		sendMsg(&msg, sizeof(msg), ptrArg, sizeArg); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_2_ERR(code, sender, T1, arg1, T2, arg2, errArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		const axl::err::ErrorHdr* errArg \
		) \
	{ \
		axl::sl::ProtoMsg_2<T1, T2> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		sendMsg(&msg, sizeof(msg), errArg, errArg->m_size); \
	}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_SEND_FUNCTION_3(code, sender, T1, arg1, T2, arg2, T3, arg3) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		T2 arg3 \
		) \
	{ \
		axl::sl::ProtoMsg_3<T1, T2, T3> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		msg.m_arg3 = arg3; \
		sendMsg(&msg, sizeof(msg)); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_3_STR(code, sender, T1, arg1, T2, arg2, T3, arg3, strArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		T3 arg3, \
		const sl::StringRef& strArg \
		) \
	{ \
		axl::sl::ProtoMsg_3<T1, T2, T3> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		msg.m_arg3 = arg3; \
		sendMsg(&msg, sizeof(msg), strArg.cp(), strArg.getLength()); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_3_PTR(code, sender, T1, arg1, T2, arg2, T3, arg3, ptrArg, sizeArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		T3 arg3, \
		const void* ptrArg, \
		size_t sizeArg \
		) \
	{ \
		axl::sl::ProtoMsg_3<T1, T2, T3> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		msg.m_arg3 = arg3; \
		sendMsg(&msg, sizeof(msg), ptrArg, sizeArg); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_3_ERR(code, sender, T1, arg1, T2, arg2, T3, arg3, errArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		T3 arg3, \
		const axl::err::ErrorHdr* errArg \
		) \
	{ \
		axl::sl::ProtoMsg_3<T1, T2, T3> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		msg.m_arg3 = arg3; \
		sendMsg(&msg, sizeof(msg), errArg, errArg->m_size); \
	}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_SEND_FUNCTION_4(code, sender, T1, arg1, T2, arg2, T3, arg3, T4, arg4) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		T3 arg3, \
		T4 arg4 \
		) \
	{ \
		axl::sl::ProtoMsg_4<T1, T2, T3, T4> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		msg.m_arg3 = arg3; \
		msg.m_arg4 = arg4; \
		sendMsg(&msg, sizeof(msg)); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_4_STR(code, sender, T1, arg1, T2, arg2, T3, arg3, T4, arg4, strArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		T3 arg3, \
		T4 arg4, \
		const sl::StringRef& strArg \
		) \
	{ \
		axl::sl::ProtoMsg_4<T1, T2, T3, T4> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		msg.m_arg3 = arg3; \
		msg.m_arg4 = arg4; \
		sendMsg(&msg, sizeof(msg), strArg.cp(), strArg.getLength()); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_4_PTR(code, sender, T1, arg1, T2, arg2, T3, arg3, T4, arg4, ptrArg, sizeArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		T3 arg3, \
		T4 arg4, \
		const void* ptrArg, \
		size_t sizeArg \
		) \
	{ \
		axl::sl::ProtoMsg_4<T1, T2, T3, T4> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		msg.m_arg3 = arg3; \
		msg.m_arg4 = arg4; \
		sendMsg(&msg, sizeof(msg), ptrArg, sizeArg); \
	}

#define AXL_SL_PROTO_SEND_FUNCTION_4_ERR(code, sender, T1, arg1, T2, arg2, T3, arg3, T4, arg4, errArg) \
	void \
	sender ( \
		T1 arg1, \
		T2 arg2, \
		T3 arg3, \
		T4 arg4, \
		const axl::err::ErrorHdr* errArg \
		) \
	{ \
		axl::sl::ProtoMsg_4<T1, T2, T3, T4> msg; \
		msg.m_code = code; \
		msg.m_arg1 = arg1; \
		msg.m_arg2 = arg2; \
		msg.m_arg3 = arg3; \
		msg.m_arg4 = arg4; \
		sendMsg(&msg, sizeof(msg), errArg, errArg->m_size); \
	}

//..............................................................................

// convenient macros for receiving & processing messages (up to 4 args)

#define AXL_SL_BEGIN_PROTO_RECV_MAP(processMsg) \
	void processMsg ( \
		const void* p, \
		size_t size \
		) \
	{ \
		if (size < sizeof(axl::sl::ProtoMsg)) \
			return; \
		axl::sl::ProtoMsg* hdr = (axl::sl::ProtoMsg*)p; \
		switch (hdr->m_code) \
		{ \

#define AXL_SL_END_PROTO_RECV_MAP() \
		} \
	}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_RECV_FUNCTION_0(code, handler) \
	case code: \
		handler(); \
		break;

#define AXL_SL_PROTO_RECV_FUNCTION_0_STR(code, handler) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t length = size - sizeof(Msg); \
		axl::sl::StringRef string((char*)(msg + 1), length); \
		handler(string); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_0_PTR(code, handler) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t extraSize = size - sizeof(Msg); \
		handler(msg + 1, extraSize); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_0_ERR(code, handler) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		handler((const axl::err::ErrorHdr*) (msg + 1)); \
		break; \
		}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_RECV_FUNCTION_1(code, handler, T1) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_1<T1> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		handler(msg->m_arg1); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_1_STR(code, handler, T1) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_1<T1> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t length = size - sizeof(Msg); \
		axl::sl::StringRef string((char*)(msg + 1), length); \
		handler(msg->m_arg1, string); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_1_PTR(code, handler, T1) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_1<T1> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t extraSize = size - sizeof(Msg); \
		handler(msg->m_arg1, msg + 1, extraSize); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_1_ERR(code, handler, T1) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_1<T1> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		handler(msg->m_arg1, (const axl::err::ErrorHdr*) (msg + 1)); \
		break; \
		}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_RECV_FUNCTION_2(code, handler, T1, T2) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_2<T1, T2> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		handler(msg->m_arg1, msg->m_arg2); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_2_STR(code, handler, T1, T2) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_2<T1, T2> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t length = size - sizeof(Msg); \
		axl::sl::StringRef string((char*)(msg + 1), length); \
		handler(msg->m_arg1, msg->m_arg2, string); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_2_PTR(code, handler, T1, T2) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_2<T1, T2> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t extraSize = size - sizeof(Msg); \
		handler(msg->m_arg1, msg->m_arg2, msg + 1, extraSize); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_2_ERR(code, handler, T1, T2) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_2<T1, T2> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		handler(msg->m_arg1, msg->m_arg2, (const axl::err::ErrorHdr*) (msg + 1)); \
		break; \
		}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_RECV_FUNCTION_3(code, handler, T1, T2, T3) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_3<T1, T2, T3> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		handler(msg->m_arg1, msg->m_arg2, msg->m_arg3); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_3_STR(code, handler, T1, T2, T3) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_3<T1, T2, T3> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t length = size - sizeof(Msg); \
		axl::sl::StringRef string((char*)(msg + 1), length); \
		handler(msg->m_arg1, msg->m_arg2, msg->m_arg3, string); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_3_PTR(code, handler, T1, T2, T3) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_3<T1, T2, T3> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t extraSize = size - sizeof(Msg); \
		handler(msg->m_arg1, msg->m_arg2, msg->m_arg3, msg + 1, extraSize); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_3_ERR(code, handler, T1, T2, T3) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_3<T1, T2, T3> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t extraSize = size - sizeof(Msg); \
		handler(msg->m_arg1, msg->m_arg2, msg->m_arg3, (const axl::err::ErrorHdr*) (msg + 1)); \
		break; \
		}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SL_PROTO_RECV_FUNCTION_4(code, handler, T1, T2, T3, T4) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_4<T1, T2, T3, T4> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		handler(msg->m_arg1, msg->m_arg2, msg->m_arg3, msg->m_arg4); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_4_STR(code, handler, T1, T2, T3, T4) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_4<T1, T2, T3, T4> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t length = size - sizeof(Msg); \
		axl::sl::StringRef string((char*)(msg + 1), length); \
		handler(msg->m_arg1, msg->m_arg2, msg->m_arg3, msg->m_arg4, string); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_4_PTR(code, handler, T1, T2, T3, T4) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_4<T1, T2, T3, T4> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		size_t extraSize = size - sizeof(Msg); \
		handler(msg->m_arg1, msg->m_arg2, msg->m_arg3, msg->m_arg4, msg + 1, extraSize); \
		break; \
		}

#define AXL_SL_PROTO_RECV_FUNCTION_4_ERR(code, handler, T1, T2, T3, T4) \
	case code: \
		{ \
		typedef axl::sl::ProtoMsg_4<T1, T2, T3, T4> Msg; \
		Msg* msg = (Msg*)p; \
		if (size < sizeof(Msg)) \
			break; \
		handler(msg->m_arg1, msg->m_arg2, msg->m_arg3, msg->m_arg4, (const axl::err::ErrorHdr*) (msg + 1)); \
		break; \
		}

//..............................................................................

} // namespace sl
} // namespace axl
