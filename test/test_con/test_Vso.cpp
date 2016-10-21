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

#include "pch.h"

namespace test_Vso {

//..............................................................................

//class CAddress
//{
//public:
//	ulong_t m_Size;
//	ulong_t m_Protocol;
//
//	// followed by actual data...
//
//	CAddress (ulong_t Size = sizeof (CAddress), ulong_t Protocol = 0)
//		{ m_Size = Size, m_Protocol = Protocol; }
//
//	// VSO
//
//	size_t vso_GetSize () const
//		{ return m_Size; }
//
//	void vso_SetSize (size_t Size)
//		{ m_Size = Size; }
//
//	bool vso_Copy (const CAddress& Src)
//	{
//		ASSERT (m_Size >= Src.m_Size);
//		memcpy (this, &Src, Src.m_Size);
//		return true;
//	}
//};
//
//class CAddressIp4: public CAddress
//{
//public:
//	ulong_t m_Ip;
//	uint16_t m_Port;
//
//	CAddressIp4 (ulong_t Protocol, ulong_t Ip, uint16_t Port):
//		CAddress (sizeof (CAddressIp4), Protocol)
//	{
//		m_Ip = Ip;
//		m_Port = Port;
//	}
//};
//
//void VsoTest ()
//{
//	CAddressIp4 Ip4_1 (17, 0x7f000001, 1001);
//	CAddressIp4 Ip4_2 (17, 0xffffffff, 1002);
//
//	CVsoT <CAddress> Address;
//	Address = Ip4_1;
//	Address = Ip4_2;
//}


//..............................................................................

} // namespace test_Vso
