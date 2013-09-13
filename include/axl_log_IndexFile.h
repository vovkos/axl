// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_INDEXFILE_H

#include "axl_log_BinDataConfig.h"
#include "axl_log_MergeCriteria.h"
#include "axl_io_MappedFile.h"

namespace axl {
namespace log {

//.............................................................................

enum
{
	EIndexFile_FileSignature  = ':xdi', // idx: signature for index files
	EIndexFile_NodeSignature  = ':don', // nod: signature for nodes
	EIndexFile_LeafSignature  = ':fel', // lef: signature for leaves
	EIndexFile_CurrentVersion = MAKELONG(0, MAKEWORD(1, 2)), // 2.1.0
};

//.............................................................................

struct TIndexFileHdr
{
	uint32_t m_Signature; // EIndexFile_FileSignature
	uint32_t m_Version;

	uint32_t m_NodeCount;
	uint32_t m_NodeEnd;
	uint32_t m_RootNode;
	uint32_t m_HeadLeaf;
	uint32_t m_TailLeaf;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TIndexNode
{
	uint32_t m_Signature; // EIndexFile_NodeSignature
	uint32_t m_Offset;    // back translation
	uint32_t m_Parent;
	uint32_t m_Left;      // for a leaf, its previous leaf; otherwise its left child
	uint32_t m_Right;     // for a leaf, its next leaf; otherwise its right child
	uint32_t m_Depth;     // 0 - it's a leaf, otherwise - it's a node
	uint32_t m_LineCount;
	uint32_t m_LeafDataSize;  

#ifdef _DEBUG
	uint32_t m_RotationCount;
#endif

	// leaf nodes are followed by TIndexLeaf
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TIndexLeaf
{
	uint32_t m_PacketOffset;
	uint32_t m_PacketCount;
	uint32_t m_VolatilePacketCount;
	uint32_t m_BinOffset;
	uint32_t m_PartIdx;
	uint32_t m_MergeId;
	uint32_t m_Col;
	TBinDataConfig m_DefBinDataConfig;
	TMergeCriteria m_MergeCriteria;	

	// possibly followed by array of TIndexVolatilePacket
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TIndexVolatilePacket
{
	uint32_t m_VolatileFlags; // hidden, expanded, etc.
	uint32_t m_Offset;        // for effective re-filtering all packets (this way we can iterate through volatile msg array)
	uint32_t m_LineCount;     // for effective re-representing of packets that are not in cache
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

	TIndexFileHdr* 
	GetHdr () const
	{
		return m_pHdr;
	}

	TIndexNode* 
	GetNode (size_t Offset) const;

	TIndexNode* 
	GetRootNode () const
	{
		TIndexNode* pRoot = GetNode (m_pHdr->m_RootNode);
		ASSERT (!pRoot || !pRoot->m_Parent);
		return pRoot;
	}

	TIndexNode* 
	GetHeadLeaf () const
	{
		TIndexNode* pHead = GetNode (m_pHdr->m_HeadLeaf);
		ASSERT (!pHead || !pHead->m_Depth && !pHead->m_Left);
		return pHead;
	}

	TIndexNode* 
	GetTailLeaf () const
	{
		TIndexNode* pTail = GetNode (m_pHdr->m_TailLeaf);
		ASSERT (!pTail || !pTail->m_Depth && !pTail->m_Right);
		return pTail;
	}

	TIndexNode* 
	GetParentNode (TIndexNode* pNode) const
	{
		TIndexNode* pParent = GetNode (pNode->m_Parent);
		ASSERT (!pParent || pParent->m_Left == pNode->m_Offset || pParent->m_Right == pNode->m_Offset);
		return pParent;
	}

	TIndexNode* 
	GetLeftNode (TIndexNode* pNode) const
	{
		TIndexNode* pLeft = GetNode (pNode->m_Left);
		ASSERT (!pNode->m_Depth || pLeft->m_Parent == pNode->m_Offset);
		return pLeft;
	}

	TIndexNode* 
	GetRightNode (TIndexNode* pNode) const
	{
		TIndexNode* pRight = GetNode (pNode->m_Right);
		ASSERT (!pNode->m_Depth || pRight->m_Parent == pNode->m_Offset);
		return pRight;
	}

	size_t 
	GetLineCount () const
	{
		return !IsEmpty () ? GetRootNode ()->m_LineCount : 0;
	}

	size_t
	GetLeafStartLine (TIndexNode* pNode) const;

	TIndexNode* 
	FindLeaf (
		size_t LineIdx, 
		size_t* pStartLine
		) const;

	TIndexNode* 
	AddLeaf ();

	bool 
	RemoveTailLeaf ();

	size_t 
	SetTailLeafData (
		const void* p, 
		size_t Size
		);

	size_t 
	AddTailLeafData (
		const void* p, 
		size_t Size
		);

	void 
	AddLeafLines (
		TIndexNode* pNode, 
		intptr_t LineCount
		);

	void 
	SetLeafLineCount (
		TIndexNode* pNode, 
		size_t LineCount
		)
	{
		AddLeafLines (pNode, LineCount - pNode->m_LineCount);
	}

protected:
	void
	CopyNode (
		TIndexNode* pDst,
		TIndexNode* pSrc
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
