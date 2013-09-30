// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_PROTOCOL_H

#include "axl_log_IndexFile.h"
#include "axl_log_CachePage.h"
#include "axl_obj_Root.h"
#include "axl_g_Time.h"

namespace axl {
namespace log {
		
//.............................................................................
	
struct TMsg
{
	uint_t m_MsgKind;
	size_t m_MsgSize;

	// followed by msg data
};	

//.............................................................................

struct IPeer: obj::IRoot
{
	// {0D19A0D6-A26E-43BC-8CAC-815382420613}
	AXL_OBJ_INTERFACE (
		IPeer,
		0x0d19a0d6, 0xa26e, 0x43bc, 0x8c, 0xac, 0x81, 0x53, 0x82, 0x42, 0x06, 0x13
		)

	virtual
	void
	SendMsg (const TMsg* pMsg) = 0;
};	
	
//.............................................................................

// client-to-server messages

enum ESrvMsg
{
	ESrvMsg_Clear,              // TMsg
	ESrvMsg_Rebuild,            // TMsg
	ESrvMsg_Update,             // TMsg
	ESrvMsg_SetBinDataConfig,   // TSrvMsg_SetBinDataConfig
	ESrvMsg_WritePacket,        // TSrvMsg_WritePacket
	ESrvMsg_RepresentPage,      // TSrvMsg_RepresentPage
	ESrvMsg_FoldPacket,         // TSrvMsg_FoldPacket
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSrvMsg_SetBinDataConfig: TMsg
{
	TBinDataConfig m_BinDataConfig;
};

struct TSrvMsg_WritePacket: TMsg
{
	uint64_t m_Timestamp;
	uint_t m_Code;
	size_t m_DataSize;

	// followed by packet data
};

struct TSrvMsg_RepresentPage: TMsg
{
	uint64_t m_SyncId;
	TIndexLeaf m_IndexLeaf;

	// followed by uint64_t FoldFlagArray []
};

struct TSrvMsg_FoldPacket: TMsg
{
	uint64_t m_SyncId;
	uint64_t m_IndexLeafOffset;
	uint64_t m_PacketOffset;
	size_t m_FoldablePacketIdx;
	size_t m_FirstLineIdx;
	size_t m_OldLineCount;
	uint64_t m_FoldFlags;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct IServer: IPeer
{
	// {6902690F-93B0-4597-875F-3C99D5FED984}
	AXL_OBJ_INTERFACE (
		IServer,
		0x6902690f, 0x93b0, 0x4597, 0x87, 0x5f, 0x3c, 0x99, 0xd5, 0xfe, 0xd9, 0x84
		)

	void 
	Clear ();

	void 
	Rebuild ();

	void 
	Update ();

	void 
	SetBinDataConfig (const TBinDataConfig* pConfig);

	void 
	WritePacket (
		uint64_t Timestamp,
		uint_t Code,
		const void* p,
		size_t Size
		);

	void 
	WritePacket (
		uint_t Code,
		const void* p,
		size_t Size
		)
	{
		WritePacket (g::GetTimestamp (), Code, p, Size);
	}

	void 
	WritePacket (uint_t Code)
	{
		WritePacket (g::GetTimestamp (), Code, NULL, 0);
	}

	void 
	RepresentPage (
		uint64_t SyncId,
		const TIndexLeaf* pIndexLeaf,
		const uint64_t* pFoldFlagArray
		);

	void 
	FoldPacket (
		uint64_t SyncId,
		uint64_t IndexLeafOffset,
		uint64_t PacketOffset,
		size_t FoldablePacketIdx,
		size_t FirstLineIdx,
		size_t OldLineCount,
		uint64_t FoldFlags
		);
};

//.............................................................................

// server-to-client messages

enum ECliMsg
{
	ECliMsg_ClearCache,                // TCliMsg_ClearCache
	ECliMsg_FilterProgress,            // TCliMsg_Progress
	ECliMsg_ColorizeProgress,          // TCliMsg_Progress
	ECliMsg_IndexProgress,             // TCliMsg_Progress
	ECliMsg_CreateIndexLeaf,           // TCliMsg_CreateIndexLeaf
	ECliMsg_UpdateIndexTailLeaf,       // TCliMsg_UpdateIndexreateIndexLeaf
	ECliMsg_RepresentPageComplete,     // TCliMsg_RepresentPageComplete
	ECliMsg_FoldPacketComplete,        // TCliMsg_FoldPacketComplete
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TCliMsg_ClearCache: TMsg
{
	uint64_t m_SyncId;
};

struct TCliMsg_Progress: TMsg
{
	uint32_t m_Percentage; // -1 = complete
};

struct TCliMsg_CreateIndexLeaf: TMsg
{
	uint64_t m_LeafOffset;
	TBinDataConfig m_BinDataConfig;
	uint64_t m_LineCount;
	uint64_t m_PacketOffset;
	uint32_t m_PacketCount;
	uint32_t m_FoldablePacketCount;
	uint64_t m_MergePointIdx;
	uint64_t m_FoldFlags;
};

struct TCliMsg_UpdateIndexTailLeaf: TMsg
{
	uint64_t m_LeafOffset;
	uint64_t m_LineCount;
	uint32_t m_PacketCount;
	uint32_t m_FoldablePacketCount;
	uint64_t m_FoldFlags;
};

struct TCliMsg_RepresentPageComplete: TMsg
{
	uint64_t m_SyncId;
	uint64_t m_IndexLeafOffset;	
	size_t m_LineIdx;
	size_t m_LineCount;
	size_t m_FoldablePacketIdx;
	size_t m_FoldablePacketCount;
	size_t m_LineBufferSize;

	// followed by foldable packet array
	// followed by line buffer
};

struct TCliMsg_FoldPacketComplete: TMsg
{
	uint64_t m_SyncId;
	uint64_t m_IndexLeafOffset;	
	size_t m_LineIdx;
	size_t m_OldLineCount;
	size_t m_NewLineCount;
	size_t m_FoldablePacketIdx;
	size_t m_LineBufferSize;
	uint64_t m_FoldFlags;	

	// followed by line buffer
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct IClient: IPeer
{
	// {9BA0444A-1DEA-4E55-B401-A496C8109C6F}
	AXL_OBJ_INTERFACE (
		IClient,
		0x9ba0444a, 0x1dea, 0x4e55, 0xb4, 0x01, 0xa4, 0x96, 0xc8, 0x10, 0x9c, 0x6f
		)

	void 
	ClearCache (uint64_t SyncId);

	void 
	FilterProgress (uint_t Percentage); // -1 = complete

	void 
	IndexProgress (uint_t Percentage); // -1 = complete

	void 
	ColorizeProgress (uint_t Percentage); // -1 = complete

	void 
	CreateIndexLeaf (
		uint64_t LeafOffset,
		const TBinDataConfig* pBinDataConfig,
		uint64_t LineCount,
		uint64_t PacketOffset,
		uint32_t PacketCount,
		uint32_t FoldablePacketCount,
		uint64_t MergePointIdx,
		uint64_t FoldFlags
		);

	void 
	UpdateIndexTailLeaf (
		uint64_t LeafOffset,
		uint64_t LineCount,
		uint32_t PacketCount,
		uint32_t FoldablePacketCount,
		uint64_t FoldFlags
		);

	void 
	RepresentPageComplete (
		uint64_t SyncId,
		uint64_t IndexLeafOffset,
		size_t LineIdx,
		size_t LineCount,
		size_t FoldablePacketIdx,
		const TFoldablePacket* pFoldablePacketArray,
		size_t FoldablePacketCount,
		const void* pLineBuffer,
		size_t LineBufferSize
		);

	void 
	FoldPacketComplete (
		uint64_t SyncId,
		uint64_t IndexLeafOffset,
		size_t LineIdx,
		size_t OldLineCount,
		size_t NewLineCount,
		size_t FoldablePacketIdx,
		uint64_t FoldFlags,
		const void* pLineBuffer,
		size_t LineBufferSize
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
