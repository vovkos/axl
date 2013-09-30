#include "pch.h"
#include "axl_log_Protocol.h"

namespace axl {
namespace log {

//.............................................................................

void 
IServer::Clear ()
{
	TMsg Msg;
	Msg.m_MsgKind = ESrvMsg_Clear;
	Msg.m_MsgSize = sizeof (Msg);
	
	SendMsg (&Msg);
}

void 
IServer::Rebuild ()
{
	TMsg Msg;
	Msg.m_MsgKind = ESrvMsg_Rebuild;
	Msg.m_MsgSize = sizeof (Msg);
	
	SendMsg (&Msg);
}

void 
IServer::Update ()
{
	TMsg Msg;
	Msg.m_MsgKind = ESrvMsg_Update;
	Msg.m_MsgSize = sizeof (Msg);
	
	SendMsg (&Msg);
}

void 
IServer::SetBinDataConfig (const TBinDataConfig* pConfig)
{
	TSrvMsg_SetBinDataConfig Msg;
	Msg.m_MsgKind = ESrvMsg_SetBinDataConfig;
	Msg.m_MsgSize = sizeof (Msg);
	Msg.m_BinDataConfig = *pConfig;
	
	SendMsg (&Msg);
}

void 
IServer::WritePacket (
	uint64_t Timestamp,
	uint_t Code,
	const void* p,
	size_t Size
	)
{
	if (!Size)
	{
		TSrvMsg_WritePacket Msg;
		Msg.m_MsgKind = ESrvMsg_WritePacket;
		Msg.m_MsgSize = sizeof (Msg);
		Msg.m_Timestamp = Timestamp;
		Msg.m_Code = Code;
		Msg.m_DataSize = Size;
		
		SendMsg (&Msg);
	}
	else
	{
		char Buffer [256];
		ref::CBufT <TSrvMsg_WritePacket> Msg (ref::EBuf_Stack, Buffer, sizeof (Buffer));

		size_t MsgSize = sizeof (TSrvMsg_WritePacket) + Size;
		Msg.GetBuffer (MsgSize);	
		Msg->m_MsgKind = ESrvMsg_WritePacket;
		Msg->m_MsgSize = MsgSize;
		Msg->m_Timestamp = Timestamp;
		Msg->m_Code = Code;
		Msg->m_DataSize = Size;
		memcpy (Msg + 1, p, Size);
		
		SendMsg (Msg);
	}
}

void 
IServer::RepresentPage (
	uint64_t SyncId,
	const TIndexLeaf* pIndexLeaf,
	const uint64_t* pFoldFlagArray
	)
{
	if (!pIndexLeaf->m_FoldablePacketCount)
	{
		TSrvMsg_RepresentPage Msg;
		Msg.m_MsgKind = ESrvMsg_RepresentPage;
		Msg.m_MsgSize = sizeof (Msg);
		Msg.m_SyncId = SyncId;
		Msg.m_IndexLeaf = *pIndexLeaf;
		
		SendMsg (&Msg);
	}
	else
	{
		char Buffer [256];
		ref::CBufT <TSrvMsg_RepresentPage> Msg (ref::EBuf_Stack, Buffer, sizeof (Buffer));

		size_t FoldFlagArraySize = pIndexLeaf->m_FoldablePacketCount * sizeof (uint64_t);
		size_t MsgSize = sizeof (TSrvMsg_RepresentPage) + FoldFlagArraySize;
		Msg.GetBuffer (MsgSize);	
		Msg->m_MsgKind = ESrvMsg_RepresentPage;
		Msg->m_MsgSize = MsgSize;
		Msg->m_SyncId = SyncId;
		Msg->m_IndexLeaf = *pIndexLeaf;
		memcpy (Msg + 1, pFoldFlagArray, FoldFlagArraySize);
		
		SendMsg (Msg);
	}
}

void 
IServer::FoldPacket (
	uint64_t SyncId,
	uint64_t IndexLeafOffset,
	uint64_t PacketOffset,
	size_t FoldablePacketIdx,
	size_t FirstLineIdx,
	size_t OldLineCount,
	uint64_t FoldFlags
	)
{
	TSrvMsg_FoldPacket Msg;
	Msg.m_MsgKind = ESrvMsg_FoldPacket;
	Msg.m_MsgSize = sizeof (Msg);
	Msg.m_SyncId = SyncId;
	Msg.m_IndexLeafOffset = IndexLeafOffset;
	Msg.m_PacketOffset = PacketOffset;
	Msg.m_FoldablePacketIdx = FoldablePacketIdx;
	Msg.m_FirstLineIdx = FirstLineIdx;
	Msg.m_OldLineCount = OldLineCount;
	Msg.m_FoldFlags = FoldFlags;

	SendMsg (&Msg);
}

//.............................................................................

void 
IClient::ClearCache (uint64_t SyncId)
{
	TCliMsg_ClearCache Msg;
	Msg.m_MsgKind = ECliMsg_ClearCache;
	Msg.m_MsgSize = sizeof (Msg);
	Msg.m_SyncId = SyncId;
	
	SendMsg (&Msg);
}

void 
IClient::FilterProgress (uint_t Percentage)
{
	TCliMsg_Progress Msg;
	Msg.m_MsgKind = ECliMsg_FilterProgress;
	Msg.m_MsgSize = sizeof (Msg);
	Msg.m_Percentage = Percentage;
	
	SendMsg (&Msg);
}

void 
IClient::IndexProgress (uint_t Percentage)
{
	TCliMsg_Progress Msg;
	Msg.m_MsgKind = ECliMsg_IndexProgress;
	Msg.m_MsgSize = sizeof (Msg);
	Msg.m_Percentage = Percentage;
	
	SendMsg (&Msg);
}

void 
IClient::ColorizeProgress (uint_t Percentage)
{
	TCliMsg_Progress Msg;
	Msg.m_MsgKind = ECliMsg_ColorizeProgress;
	Msg.m_MsgSize = sizeof (Msg);
	Msg.m_Percentage = Percentage;
	
	SendMsg (&Msg);
}

void 
IClient::CreateIndexLeaf (
	uint64_t LeafOffset,
	const TBinDataConfig* pBinDataConfig,
	uint64_t LineCount,
	uint64_t PacketOffset,
	uint32_t PacketCount,
	uint32_t FoldablePacketCount,
	uint64_t MergePointIdx,
	uint64_t FoldFlags
	)
{
	TCliMsg_CreateIndexLeaf Msg;
	Msg.m_MsgKind = ECliMsg_CreateIndexLeaf;
	Msg.m_MsgSize = sizeof (Msg);
	Msg.m_LeafOffset = LeafOffset;
	Msg.m_BinDataConfig = *pBinDataConfig;
	Msg.m_LineCount = LineCount;
	Msg.m_PacketOffset = PacketOffset;
	Msg.m_PacketCount = PacketCount;
	Msg.m_FoldablePacketCount = FoldablePacketCount;
	Msg.m_MergePointIdx = MergePointIdx;
	Msg.m_FoldFlags = FoldFlags;

	SendMsg (&Msg);
}

void 
IClient::UpdateIndexTailLeaf (
	uint64_t LeafOffset,
	uint64_t LineCount,
	uint32_t PacketCount,
	uint32_t FoldablePacketCount,
	uint64_t FoldFlags
	)
{
	TCliMsg_UpdateIndexTailLeaf Msg;
	Msg.m_MsgKind = ECliMsg_UpdateIndexTailLeaf;
	Msg.m_MsgSize = sizeof (Msg);
	Msg.m_LeafOffset = LeafOffset;
	Msg.m_LineCount = LineCount;
	Msg.m_PacketCount = PacketCount;
	Msg.m_FoldablePacketCount = FoldablePacketCount;
	Msg.m_FoldFlags = FoldFlags;

	SendMsg (&Msg);
}

void 
IClient::RepresentPageComplete (
	uint64_t SyncId,
	uint64_t IndexLeafOffset,
	size_t LineIdx,
	size_t LineCount,
	size_t FoldablePacketIdx,
	const TFoldablePacket* pFoldablePacketArray,
	size_t FoldablePacketCount,
	const void* pLineBuffer,
	size_t LineBufferSize
	)
{
	char Buffer [256];
	ref::CBufT <TCliMsg_RepresentPageComplete> Msg (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	size_t FoldablePacketArraySize = FoldablePacketCount * sizeof (TFoldablePacket);
	size_t MsgSize = sizeof (TCliMsg_RepresentPageComplete) + FoldablePacketArraySize + LineBufferSize;

	Msg.GetBuffer (MsgSize);	
	Msg->m_MsgKind = ECliMsg_RepresentPageComplete;
	Msg->m_MsgSize = MsgSize;
	Msg->m_SyncId = SyncId;
	Msg->m_IndexLeafOffset = IndexLeafOffset;
	Msg->m_LineIdx = LineIdx;
	Msg->m_LineCount = LineCount;
	Msg->m_FoldablePacketIdx = FoldablePacketIdx;
	Msg->m_LineBufferSize = LineBufferSize;
	Msg->m_FoldablePacketCount = FoldablePacketCount;

	memcpy (Msg + 1, pFoldablePacketArray, FoldablePacketArraySize);
	memcpy ((char*) (Msg + 1) + FoldablePacketArraySize, pLineBuffer, LineBufferSize);

	SendMsg (Msg);
}

void 
IClient::FoldPacketComplete (
	uint64_t SyncId,
	uint64_t IndexLeafOffset,
	size_t LineIdx,
	size_t OldLineCount,
	size_t NewLineCount,
	size_t FoldablePacketIdx,
	uint64_t FoldFlags,
	const void* pLineBuffer,
	size_t LineBufferSize
	)
{
	char Buffer [256];
	ref::CBufT <TCliMsg_FoldPacketComplete> Msg (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	size_t MsgSize = sizeof (TCliMsg_FoldPacketComplete) + LineBufferSize;
	Msg.GetBuffer (MsgSize);	
	Msg->m_MsgKind = ECliMsg_FoldPacketComplete;
	Msg->m_MsgSize = MsgSize;
	Msg->m_SyncId = SyncId;
	Msg->m_IndexLeafOffset = IndexLeafOffset;	
	Msg->m_LineIdx = LineIdx;
	Msg->m_OldLineCount = OldLineCount;
	Msg->m_NewLineCount = NewLineCount;
	Msg->m_FoldablePacketIdx = FoldablePacketIdx;
	Msg->m_LineBufferSize = LineBufferSize;
	Msg->m_FoldFlags = FoldFlags;	

	memcpy (Msg + 1, pLineBuffer, LineBufferSize);

	SendMsg (Msg);
}

//.............................................................................

} // namespace log 
} // namespace axl
