// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_MERGEFILE_H

#include "axl_io_MappedFile.h"
#include "axl_log_Line.h"

namespace axl {
namespace log {

//.............................................................................

enum
{
	EMergeFile_FileSignature        = ':grm', // mrg: signature for merge files
	EMergeFile_MergePointSignature  = ':tpm', // mpt: signature for merge point
	EMergeFile_CurrentVersion       = MAKELONG (0, MAKEWORD (0, 3)), // 3.0.0
};

//.............................................................................

struct TMergeFileHdr
{
	uint32_t m_Signature; // EMergeFile_LogFileSignature
	uint32_t m_Version;
	uint64_t m_MergePointCount;

	// followed by merge states
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TMergePoint
{
	uint32_t m_Signature; // EMergeFile_MergePointSignature	
	uint32_t m_LineKind;
	uint32_t m_Col;
	uint32_t m_ColorizerStateCalcPacketCount; // here for alignment
	uint64_t m_PartIdx;
	uint64_t m_BinOffset;
	uint64_t m_ColorizerStateCalcPacketOffset; // not earlier than prev merge point, -1 = whole page
};

//.............................................................................

class CMergeFile
{
protected:
	io::CMappedFile m_File;
	TMergeFileHdr* m_pHdr;

public:
	CMergeFile ()
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
		return m_pHdr->m_MergePointCount == 0;
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

	const TMergeFileHdr* 
	GetHdr () const
	{
		return m_pHdr;
	}

	const TMergePoint* 
	GetMergePoint (uint64_t Index) const;

	const TMergePoint* 
	AddMergePoint (
		ELine LineKind,
		uint32_t Col,
		uint64_t PartIdx,
		uint64_t BinOffset,
		uint64_t ColorizerStateCalcPacketOffset,
		uint32_t ColorizerStateCalcPacketCount
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
