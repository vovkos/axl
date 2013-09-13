#pragma once

#define MYLOGREPRESENTOR_H

//.............................................................................

enum EMyLogPacket
{
	EMyLogPacket_DeviceOpened    = 1 | log::EPacketCodeFlag_Mergeable,
	EMyLogPacket_DeviceClosed    = 2 | log::EPacketCodeFlag_Volatile,
	EMyLogPacket_Connect         = 3,
	EMyLogPacket_ConnectComplete = 4,
	EMyLogPacket_ConnectError    = 5,	
	EMyLogPacket_Bin             = 6 | log::EPacketCodeFlag_Mergeable,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MyLogRepresenter: public log::IRepresenter
{
public:
	AXL_OBJ_CLASS_0 (MyLogRepresenter, log::IRepresenter)

	virtual 
	bool 
	Represent (
		log::IRepresenterTarget* pTarget, 
		uint_t PacketCode, 
		const void* p, 
		size_t Size, 
		uint_t VolatileFlags
		);
};

//.............................................................................

