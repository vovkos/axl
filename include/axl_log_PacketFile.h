// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_PACKETFILE_H

#include "axl_io_MappedFile.h"

namespace axl {
namespace log {

// log file structure

// log file is just a collection of Packets tagged with code, 
// timestamp and optionally carrying extra binary data
//

//.............................................................................

enum
{
	EPacketFile_FileSignature   = ':gol', // log: signature for log files
	EPacketFile_PacketSignature = ':kap', // msg: signature for log packets
	EPacketFile_CurrentVersion  = MAKELONG (0, MAKEWORD (0, 3)), // 3.0.0
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum
{
	EPacketCodeFlag_MergeableBackward = 0x80000000,
	EPacketCodeFlag_MergeableForward  = 0x40000000,
	EPacketCodeFlag_Mergeable         = 0xc0000000,
	EPacketCodeFlag_Volatile          = 0x10000000,
};

//.............................................................................

struct TPacketFileHdr
{
	uint32_t m_Signature; // EPacketFile_LogFileSignature
	uint32_t m_Version;
	uint32_t m_PacketCount;
	uint32_t m_TotalPacketSize;
	uint32_t m_LastPacketOffset;
	uint32_t m_ClearCount;
	
	rtl::TGuid m_ClassGuid;

	// followed by log data entries
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TPacket
{
	uint32_t m_Signature;        // EPacketFile_PacketSignature
	uint32_t m_PrevPacketOffset; // for walking back

	// actual payload

	uint64_t m_Timestamp;
	uint32_t m_Code;
	uint32_t m_DataSize; 

	// followed by Packet data (if any)
};

//.............................................................................

class CPacketFile
{
protected:
	io::CMappedFile m_File;
	TPacketFileHdr* m_pHdr;

public:
	uint64_t m_TimestampOverride;

public:
	CPacketFile ()
	{
		m_pHdr = NULL;
		m_TimestampOverride = 0;
	}

	bool 
	IsOpen () const
	{
		return m_File.IsOpen ();
	}

	bool 
	IsEmpty () const
	{
		ASSERT (m_pHdr);
		return m_pHdr->m_PacketCount == 0;
	}

	bool 
	Open (
		const char* pFileName, 
		uint_t Flags = 0
		);
	
	void 
	Close ();
		
	void 
	Clear ();

	TPacketFileHdr* 
	GetHdr () const
	{
		return m_pHdr;
	}

	TPacket* 
	GetPacket (size_t Offset) const;

	bool 
	Write (
		uint_t Code, 
		const void* p = NULL, 
		size_t Size = 0
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
