#pragma once

#define MYLOGREPRESENTOR_H

//.............................................................................

enum EMyLog
{
	EMyLog_DeviceOpened    = 1,
	EMyLog_DeviceClosed    = 2 | log::EPacketCodeFlag_Foldable,
	EMyLog_Connect         = 4 | log::EPacketCodeFlag_MergeableForward,
	EMyLog_ConnectComplete = 5 | log::EPacketCodeFlag_MergeableBackward,
	EMyLog_ConnectError    = 6 | log::EPacketCodeFlag_MergeableBackward,	
	EMyLog_Connect_m       = 7,
	EMyLog_Bin             = 8 | log::EPacketCodeFlag_Mergeable,
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
		uint64_t FoldFlags
		);
};

//.............................................................................

