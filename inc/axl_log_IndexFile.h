// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_sys_MappedFile.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

enum
{
	axl_log_EIndexFile_FileSignature  = ':xdi', // idx: signature for index files
	axl_log_EIndexFile_NodeSignature  = ':don', // nod: signature for nodes
	axl_log_EIndexFile_LeafSignature  = ':fel', // leaf: signature for leaves
	axl_log_EIndexFile_CurrentVersion = MAKELONG(0, MAKEWORD(1, 2)), // 2.1.0
};

//.............................................................................

typedef struct _axl_log_TIndexFileHdr
{
	ulong_t m_Signature; // axl_log_EIndexFile_FileSignature
	ulong_t m_Version;

	// log file <-> index file sync portion

	ulong_t m_MessageCount;
	ulong_t m_MessageSize;
	ulong_t m_LastMessageOffset;
	ulong_t m_ClearCount;

	// index file properties

	ulong_t m_NodeCount;
	ulong_t m_NodeEnd;
	ulong_t m_RootNode;
	ulong_t m_HeadLeaf;
	ulong_t m_TailLeaf;
} axl_log_TIndexFileHdr;

//.............................................................................

typedef struct _axl_log_TIndexNode
{
	ulong_t m_Signature; // axl_log_EIndexFile_NodeSignature
	ulong_t m_Offset;    // back translation
	ulong_t m_Parent;
	ulong_t m_Left;      // for a leaf, its previous leaf; otherwise its left child
	ulong_t m_Right;     // for a leaf, its next leaf; otherwise its right child
	ulong_t m_Depth;     // 0 - it's a leaf, otherwise - it's a node
	ulong_t m_LineCount;
	ulong_t m_DataSize;  // leaf could be followed by leaf data

#ifdef _DEBUG
	ulong_t m_RotationCount;
#endif

} axl_log_TIndexNode;

//.............................................................................

typedef struct _axl_log_TIndexFile axl_log_TIndexFile;

AXL_INLINE
void
axl_log_TIndexFile_Construct(axl_log_TIndexFile* pFile);

AXL_INLINE
void
axl_log_TIndexFile_Destruct(axl_log_TIndexFile* pFile);

bool_t
AXL_API
axl_log_TIndexFile_Open(
	axl_log_TIndexFile* pFile,
	const tchar_t* pFileName,
	int Flags
	);

void
AXL_API
axl_log_TIndexFile_Clear(axl_log_TIndexFile* pFile);

AXL_INLINE
void
axl_log_TIndexFile_Close(axl_log_TIndexFile* pFile);

AXL_INLINE
bool_t
axl_log_TIndexFile_IsOpen(axl_log_TIndexFile* pFile);

AXL_INLINE
bool_t
axl_log_TIndexFile_IsEmpty(axl_log_TIndexFile* pFile);

axl_log_TIndexNode*
AXL_API
axl_log_TIndexFile_GetNode(
	axl_log_TIndexFile* pFile, 
	size_t Offset
	);

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetRootNode(axl_log_TIndexFile* pFile);

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetHeadLeaf(axl_log_TIndexFile* pFile);

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetTailLeaf(axl_log_TIndexFile* pFile);

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetParentNode(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode
	);

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetLeftNode(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode
	);

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetRightNode(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode
	);

AXL_INLINE
ulong_t
axl_log_TIndexFile_GetLineCount(axl_log_TIndexFile* pFile);

axl_log_TIndexNode*
AXL_API
axl_log_TIndexFile_AddLeaf(
	axl_log_TIndexFile* pFile,	
	const void* p,
	size_t Size
	);

bool_t
AXL_API
axl_log_TIndexFile_RemoveTailLeaf(axl_log_TIndexFile* pFile);

size_t
AXL_API
axl_log_TIndexFile_AddTailLeafData(
	axl_log_TIndexFile* pFile,	
	const void* p,
	size_t Size
	);

size_t
AXL_API
axl_log_TIndexFile_RemoveTailLeafData(
	axl_log_TIndexFile* pFile,	
	size_t Size
	);

void
AXL_API
axl_log_TIndexFile_AddLeafLines(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode,
	long LineCount
	);

AXL_INLINE
void
axl_log_TIndexFile_SetLeafLineCount(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode,
	ulong_t LineCount
	);

ulong_t
AXL_API
axl_log_TIndexFile_GetLeafStartLine(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode
	);

axl_log_TIndexNode* 
AXL_API
axl_log_TIndexFile_FindLeafByLine(
	axl_log_TIndexFile* pFile,
	ulong_t Line,
	ulong_t* pStartLine
	);

typedef struct _axl_log_TIndexFile
{
	axl_sys_TMappedFile m_File;
	axl_log_TIndexFileHdr* m_pHdr;

#ifdef __cplusplus
	bool Open(const tchar_t* pFileName, int Flags = 0)
		{ return axl_log_TIndexFile_Open(this, pFileName, Flags) != 0; }

	void Close()
		{ axl_log_TIndexFile_Close(this); }

	bool IsOpen()
		{ return axl_log_TIndexFile_IsOpen(this) != 0; }
		
	axl_log_TIndexNode* GetNode(ulong_t Offset)
		{ return axl_log_TIndexFile_GetNode(this, Offset); }

	axl_log_TIndexNode* GetRootNode()
		{ return axl_log_TIndexFile_GetRootNode(this); }

	axl_log_TIndexNode* GetHeadLeaf()
		{ return axl_log_TIndexFile_GetHeadLeaf(this); }

	axl_log_TIndexNode* GetTailLeaf()
		{ return axl_log_TIndexFile_GetTailLeaf(this); }

	axl_log_TIndexNode* GetParentNode(axl_log_TIndexNode* pNode)
		{ return axl_log_TIndexFile_GetParentNode(this, pNode); }

	axl_log_TIndexNode* GetLeftNode(axl_log_TIndexNode* pNode)
		{ return axl_log_TIndexFile_GetLeftNode(this, pNode); }

	axl_log_TIndexNode* GetRightNode(axl_log_TIndexNode* pNode)
		{ return axl_log_TIndexFile_GetRightNode(this, pNode); }

	ulong_t GetLineCount()
		{ return axl_log_TIndexFile_GetLineCount(this); }

	axl_log_TIndexNode* AddLeaf(const void* p = NULL, size_t Size = 0)
		{ return axl_log_TIndexFile_AddLeaf(this, p, Size); }

	bool RemoveTailLeaf()
		{ return axl_log_TIndexFile_RemoveTailLeaf(this) != 0; }

	size_t AddTailLeafData(const void* p, size_t Size)
		{ return axl_log_TIndexFile_AddTailLeafData(this, p, Size); }

	size_t RemoveTailLeafData(size_t Size)
		{ return axl_log_TIndexFile_RemoveTailLeafData(this, Size); }

	void AddLeafLines(axl_log_TIndexNode* pNode, long LineCount)
		{ axl_log_TIndexFile_AddLeafLines(this, pNode, LineCount); }

	void SetLeafLineCount(axl_log_TIndexNode* pNode, ulong_t LineCount)
		{ axl_log_TIndexFile_SetLeafLineCount(this, pNode, LineCount); }

	ulong_t GetLeafStartLine(axl_log_TIndexNode* pNode)
		{ return axl_log_TIndexFile_GetLeafStartLine(this, pNode); }

	axl_log_TIndexNode* FindLeafByLine(ulong_t Line, ulong_t* pStartLine)
		{ return axl_log_TIndexFile_FindLeafByLine(this, Line, pStartLine); }
#endif
} axl_log_TIndexFile;

//.............................................................................

// inlines

AXL_INLINE
void
axl_log_TIndexFile_Construct(axl_log_TIndexFile* pFile)
{
	axl_sys_TMappedFile_Construct(&pFile->m_File);
}

AXL_INLINE
void
axl_log_TIndexFile_Destruct(axl_log_TIndexFile* pFile)
{
	axl_sys_TMappedFile_Destruct(&pFile->m_File);
}

AXL_INLINE
void
axl_log_TIndexFile_Close(axl_log_TIndexFile* pFile)
{
	axl_sys_TMappedFile_Close(&pFile->m_File);
}

AXL_INLINE
bool_t
axl_log_TIndexFile_IsOpen(axl_log_TIndexFile* pFile)
{
	return axl_sys_TMappedFile_IsOpen(&pFile->m_File);
}

AXL_INLINE
bool_t
axl_log_TIndexFile_IsEmpty(axl_log_TIndexFile* pFile)
{
	ASSERT(pFile->m_pHdr);
	return pFile->m_pHdr->m_NodeCount == 0;
}

AXL_INLINE
ulong_t
axl_log_TIndexFile_GetLineCount(axl_log_TIndexFile* pFile)
{
	return !axl_log_TIndexFile_IsEmpty(pFile) ? axl_log_TIndexFile_GetRootNode(pFile)->m_LineCount : 0;
}

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetRootNode(axl_log_TIndexFile* pFile)
{
	axl_log_TIndexNode* pRoot = axl_log_TIndexFile_GetNode(pFile, pFile->m_pHdr->m_RootNode);
	ASSERT(!pRoot || !pRoot->m_Parent);
	return pRoot;
}

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetHeadLeaf(axl_log_TIndexFile* pFile)
{
	axl_log_TIndexNode* pHead = axl_log_TIndexFile_GetNode(pFile, pFile->m_pHdr->m_HeadLeaf);
	ASSERT(!pHead || !pHead->m_Depth && !pHead->m_Left);
	return pHead;
}

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetTailLeaf(axl_log_TIndexFile* pFile)
{
	axl_log_TIndexNode* pTail = axl_log_TIndexFile_GetNode(pFile, pFile->m_pHdr->m_TailLeaf);
	ASSERT(!pTail || !pTail->m_Depth && !pTail->m_Right);
	return pTail;
}

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetParentNode(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode
	)
{
	axl_log_TIndexNode* pParent = axl_log_TIndexFile_GetNode(pFile, pNode->m_Parent);
	ASSERT(!pParent || pParent->m_Left == pNode->m_Offset || pParent->m_Right == pNode->m_Offset);
	return pParent;
}

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetLeftNode(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode
	)
{
	axl_log_TIndexNode* pLeft = axl_log_TIndexFile_GetNode(pFile, pNode->m_Left);
	ASSERT(!pNode->m_Depth || pLeft->m_Parent == pNode->m_Offset);
	return pLeft;
}

AXL_INLINE
axl_log_TIndexNode*
axl_log_TIndexFile_GetRightNode(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode
	)
{
	axl_log_TIndexNode* pRight = axl_log_TIndexFile_GetNode(pFile, pNode->m_Right);
	ASSERT(!pNode->m_Depth || pRight->m_Parent == pNode->m_Offset);
	return pRight;
}

AXL_INLINE
void
axl_log_TIndexFile_SetLeafLineCount(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode,
	ulong_t LineCount
	)
{
	axl_log_TIndexFile_AddLeafLines(pFile, pNode, LineCount - pNode->m_LineCount);
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

//.............................................................................

struct EIndexFile { enum
{
	FileSignature  = axl_log_EIndexFile_FileSignature, 
	NodeSignature  = axl_log_EIndexFile_NodeSignature,
};};

//.............................................................................

typedef axl_log_TIndexFile    TIndexFile;
typedef axl_log_TIndexFileHdr TIndexFileHdr;
typedef axl_log_TIndexNode    TIndexNode;

//.............................................................................

class CIndexFile: public TIndexFile
{
public:
	CIndexFile()
		{ axl_log_TIndexFile_Construct(this); }

	~CIndexFile()
		{ axl_log_TIndexFile_Destruct(this); }
};

//.............................................................................

} // namespace log 
} // namespace axl

#endif // __cplusplus

//.............................................................................

