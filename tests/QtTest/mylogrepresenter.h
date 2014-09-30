#pragma once

#define MYLOGREPRESENTOR_H

//.............................................................................

enum MyLogKind
{
	MyLogKind_DeviceOpened    = 1,
	MyLogKind_DeviceClosed    = 2 | log::PacketCodeFlagKind_Foldable,
	MyLogKind_Connect         = 4 | log::PacketCodeFlagKind_MergeableForward,
	MyLogKind_ConnectComplete = 5 | log::PacketCodeFlagKind_MergeableBackward,
	MyLogKind_ConnectError    = 6 | log::PacketCodeFlagKind_MergeableBackward,	
	MyLogKind_Connect_m       = 7,
	MyLogKind_Bin             = 8 | log::PacketCodeFlagKind_Mergeable,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class myLogRepresenter: public log::Representer
{
public:
	virtual 
	bool 
	represent (
		log::Representerarget* target, 
		uint_t packetCode, 
		const void* p, 
		size_t size, 
		uint64_t foldFlags
		);
};

//.............................................................................

