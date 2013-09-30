// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_PACKETFILE_H

#include "axl_io_MappedFile.h"
#include "axl_g_Time.h"

namespace axl {
namespace log {

// log packet file is a collection of packets tagged with code, 
// timestamp and optionally carrying extra binary data

//.............................................................................

enum
{
	EPacketFile_FileSignature   = ':gol', // log: signature for log packet files
	EPacketFile_PacketSignature = ':kap', // msg: signature for log packets
	EPacketFile_CurrentVersion  = MAKELONG (0, MAKEWORD (0, 3)), // 3.0.0
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum
{
	EPacketCodeFlag_Foldable          = 0x80000000,
	EPacketCodeFlag_MergeableBackward = 0x40000000,
	EPacketCodeFlag_MergeableForward  = 0x20000000,
	EPacketCodeFlag_Mergeable         = 0x60000000,
};

//.............................................................................

struct TPacketFileHdr
{
	uint32_t m_Signature; // EPacketFile_LogFileSignature
	uint32_t m_Version;
	uint64_t m_PacketCount;
	uint64_t m_TotalPacketSize;
	uint64_t m_ClearCount; // to synchronize mutliple log servers working with the same packet file	
	rtl::TGuid m_ClassGuid;

	// followed by log packets
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TPacket
{
	uint32_t m_Signature;        // EPacketFile_PacketSignature
	uint32_t m_Code;
	uint64_t m_Timestamp;
	uint64_t m_DataSize; 

	// followed by Packet data (if any)
};

//.............................................................................

class CPacketFile
{
protected:
	io::CMappedFile m_File;
	TPacketFileHdr* m_pHdr;

public:
	CPacketFile ()
	{
		m_pHdr = NULL;
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

	const TPacketFileHdr* 
	GetHdr () const
	{
		return m_pHdr;
	}

	const TPacket* 
	GetPacket (uint64_t Offset) const;

	const TPacket* 
	AddPacket (
		uint64_t Timestamp,	
		uint_t Code, 
		const void* p = NULL, 
		size_t Size = 0
		);

	const TPacket* 
	AddPacket (
		uint_t Code, 
		const void* p = NULL, 
		size_t Size = 0
		)
	{
		return AddPacket (g::GetTimestamp (), Code, p, Size);
	}
};

//.............................................................................

} // namespace log 
} // namespace axl
