// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_PROTOCOL_H

#include "axl_log_IndexFile.h"
#include "axl_obj_Root.h"
#include "axl_g_Time.h"

namespace axl {
namespace log {

//.............................................................................

// client-to-server messages

enum ESrvMsg
{
	ESrvMsg_Clear,          // no args
	ESrvMsg_Rebuild,        // no args
	ESrvMsg_Update,         // no args
	ESrvMsg_WritePacket,    // TSrvMsg_WritePacket
	ESrvMsg_Represent,      // TSrvMsg_Represent
	ESrvMsg_Colorize,       // TSrvMsg_Colorize
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSrvMsg_WritePacket
{
	uint64_t m_Timestamp;
	uint32_t m_Code;
	uint32_t m_DataSize;

	// followed by packet data
};

struct TSrvMsg_Represent
{
	uint32_t m_SyncId;
	uint32_t m_IndexNodeOffset;
	uint32_t m_IndexNodeLineCount;
	uint32_t m_LineIdx;
	TIndexLeaf m_IndexLeaf;
	TIndexLeaf m_PrevIndexLeaf;

	// followed by TIndexVolatilePacket []
};

struct TSrvMsg_Colorize
{
	uint32_t m_SyncId;
	uint32_t m_IndexNodeOffset;
	uint32_t m_LineIdx;
	uint32_t m_LineOffset;
	uint32_t m_DataSize;

	// followed by bin data
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct IServer
{
	virtual 
	void 
	Clear () = 0;

	virtual 
	void 
	Rebuild () = 0;

	virtual 
	void 
	SetDefBinDataConfig (const TBinDataConfig* pConfig) = 0;

	virtual 
	void 
	Update () = 0;

	virtual 
	void 
	WritePacket (
		uint64_t Timestamp,
		uint_t Code,
		const void* p,
		size_t Size
		) = 0;

	void 
	WritePacket (
		uint_t Code,
		const void* p = NULL,
		size_t Size = 0
		)
	{
		WritePacket (g::GetTimestamp (), Code, p, Size);
	}
	
	virtual 
	void 
	Represent (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t IndexNodeLineCount,
		size_t LineIdx,
		const TIndexLeaf* pIndexLeaf,
		const TIndexLeaf* pPrevIndexLeaf,
		const TIndexVolatilePacket* pVolatilePacketArray
		) = 0;

	virtual 
	void 
	Colorize (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t LineIdx,
		size_t LineOffset,
		const void* p,
		size_t Size
		) = 0;
};

//.............................................................................

// server-to-client messages

enum ECliMsg
{
	ECliMsg_ClearCache,         // TCliMsg_ClearCache
	ECliMsg_FilterProgress,     // TCliMsg_Progress
	ECliMsg_IndexProgress,      // TCliMsg_Progress
	ECliMsg_SetIndexTailLeaf,   // TCliMsg_SetIndexTailLeaf
	ECliMsg_RepresentComplete,  // TCliMsg_RepresentComplete
	ECliMsg_ColorizeComplete,   // TCliMsg_ColorizeComplete
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TCliMsg_ClearCache
{
	uint32_t m_SyncId;
};

struct TCliMsg_Progress
{
	uint32_t m_Percentage; // -1 = complete
};

struct TCliMsg_SetIndexTailLeaf
{
	bool_t m_IsNewLeaf;
	uint32_t m_LineCount;
	TIndexLeaf m_Leaf;

	// followed by TIndexVolatilePacket []
};

struct TCliMsg_RepresentComplete
{
	uint32_t m_SyncId;
	uint32_t m_IndexNodeOffset;
	uint32_t m_IndexNodeLineCount;
	uint32_t m_LineIdx;
	uint32_t m_LineDataSize;

	// followed by line data
};

struct TCliMsg_ColorizeComplete
{
	uint32_t m_SyncId;
	uint32_t m_IndexNodeOffset;
	uint32_t m_LineIdx;
	uint32_t m_LineOffset;
	uint32_t m_ColorizeDataSize;

	// followed by colorize data
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct IClient
{
	virtual 
	void 
	ClearCache (size_t SyncId) = 0;

	virtual 
	void 
	FilterProgress (uint_t Percentage) = 0; // -1 = complete

	virtual 
	void 
	IndexProgress (uint_t Percentage) = 0; // -1 = complete

	virtual 
	void 
	SetIndexTailLeaf (
		bool IsNewLeaf,
		size_t LineCount,
		const TIndexLeaf* pLeaf,
		const TIndexVolatilePacket* pVolatilePacketArray
		) = 0;

	virtual 
	void 
	RepresentComplete (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t IndexNodeLineCount,
		size_t LineIdx,
		const void* p,
		size_t Size
		) = 0;

	virtual 
	void 
	ColorizeComplete (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t LineIdx,
		size_t LineOffset,
		const void* p,
		size_t Size
		) = 0;
};

//.............................................................................

} // namespace log {
} // namespace axl {
