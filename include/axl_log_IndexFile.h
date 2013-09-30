// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_INDEXFILE_H

#include "axl_log_Line.h"
#include "axl_log_BinDataConfig.h"
#include "axl_io_MappedFile.h"

namespace axl {
namespace log {

//.............................................................................

enum
{
	EIndexFile_FileSignature  = ':xdi', // idx: signature for index files
	EIndexFile_NodeSignature  = ':don', // nod: signature for nodes
	EIndexFile_LeafSignature  = ':fel', // lef: signature for leaves
	EIndexFile_CurrentVersion = MAKELONG(0, MAKEWORD(0, 3)), // 3.0.0
};

//.............................................................................

struct TIndexFileHdr
{
	uint32_t m_Signature; // EIndexFile_FileSignature
	uint32_t m_Version;
	uint64_t m_NodeCount;
	uint64_t m_LeafCount;
	uint64_t m_TotalNodeSize;
	uint64_t m_RootNode;
	uint64_t m_HeadLeaf;
	uint64_t m_TailLeaf;
	uint64_t m_LineCount;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TIndexNode
{
	uint32_t m_Signature; // EIndexFile_NodeSignature
	uint32_t m_Depth;     // 0 for leaves
	uint64_t m_Offset;    // not essential, but really makes code cleaner 
	uint64_t m_Parent;
	uint64_t m_Left;      // for a leaf, its previous leaf; otherwise its left child
	uint64_t m_Right;     // for a leaf, its next leaf; otherwise its right child
	uint64_t m_LineCount;

#ifdef _DEBUG
	uint64_t m_RotationCount;
#endif

	// leaf nodes are followed by TIndexLeaf
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TIndexLeaf: TIndexNode
{
	TBinDataConfig m_BinDataConfig;

	uint64_t m_PacketOffset;
	uint32_t m_PacketCount;
	uint32_t m_FoldablePacketCount;
	uint64_t m_MergePointIdx;

	// followed by uint64_t m_FoldFlagArray [m_FoldablePacketCount]
};

//.............................................................................

class CIndexFile
{
protected:
	io::CMappedFile m_File;
	TIndexFileHdr* m_pHdr;

public:
	CIndexFile ()
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
		return m_pHdr->m_NodeCount == 0;
	}

	bool 
	Open (
		const char* pFileName, 
		uint_t Flags = 0
		);

	void 
	Close ()
	{ 
		m_File.Close (); 
	}

	void 
	Clear ();

	const TIndexFileHdr* 
	GetHdr () const
	{
		ASSERT (m_pHdr);
		return m_pHdr;
	}

	uint64_t 
	GetLineCount () const
	{
		return m_pHdr->m_LineCount;
	}

	const TIndexLeaf* 
	GetLeaf (uint64_t Offset) const;

	const TIndexLeaf* 
	GetHeadLeaf () const
	{
		ASSERT (m_pHdr);
		return m_pHdr->m_HeadLeaf != -1 ? GetLeaf (m_pHdr->m_HeadLeaf) : NULL;
	}

	const TIndexLeaf* 
	GetTailLeaf () const
	{
		ASSERT (m_pHdr);
		return m_pHdr->m_TailLeaf != -1 ? GetLeaf (m_pHdr->m_TailLeaf) : NULL;
	}

	const TIndexLeaf* 
	GetNextLeaf (const TIndexLeaf* pLeaf) const
	{
		ASSERT (m_pHdr);
		return pLeaf->m_Right != -1 ? GetLeaf (pLeaf->m_Right) : NULL;
	}

	const TIndexLeaf* 
	GetPrevLeaf (const TIndexLeaf* pLeaf) const
	{
		ASSERT (m_pHdr);
		return pLeaf->m_Left != -1 ? GetLeaf (pLeaf->m_Left) : NULL;
	}

	const TIndexLeaf* 
	AddLeaf (
		const TBinDataConfig* pBinDataConfig,
		uint64_t PacketOffset,
		uint64_t MergePointIdx = -1
		);

	uint64_t
	GetLeafStartLine (const TIndexLeaf* pLeaf) const;

	const TIndexLeaf* 
	FindLeaf (
		uint64_t LineIdx, 
		uint64_t* pStartLine
		) const;

	void 
	AddLeafLines (
		const TIndexLeaf* pLeaf, 
		intptr_t LineCount
		);

	void 
	SetLeafLineCount (
		const TIndexLeaf* pLeaf, 
		size_t LineCount
		)
	{
		AddLeafLines (pLeaf, LineCount - (size_t) pLeaf->m_LineCount);
	}

	void
	AddTailLeafPackets (
		size_t PacketCount,
		size_t FoldablePacketCount = 0,
		uint64_t FoldFlags = 0
		);		

protected:
	TIndexNode* 
	GetNode (uint64_t Offset) const;

	TIndexNode* 
	GetRootNode () const;

	TIndexNode* 
	GetParentNode (const TIndexNode* pNode) const;

	TIndexNode* 
	GetLeftNode (const TIndexNode* pNode) const;

	TIndexNode* 
	GetRightNode (const TIndexNode* pNode) const;

	TIndexNode* 
	CreateNode (
		uint32_t Signature,
		size_t Size
		);

	TIndexNode* 
	CreateNode ()
	{
		return CreateNode (EIndexFile_NodeSignature, sizeof (TIndexNode));
	}

	TIndexLeaf* 
	CreateLeaf (
		const TBinDataConfig* pBinDataConfig,
		uint64_t PacketOffset,
		uint64_t MergePointIdx
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
