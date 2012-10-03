#include "axl_Precomp.h"
#include "axl_log_PostProcessor.h"

void 
AXL_VFT
axl_log_TPostProcessor_Reset_b(axl_log_TPostProcessor* pPostProcessor)
{
}

//.............................................................................

void
AXL_API
axl_log_TPostProcessor_Construct(
	axl_log_TPostProcessor* pPostProcessor, 
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_TPostProcessorVTable _VTable = 
	{
		axl_log_TPostProcessor_Destruct,
		axl_log_TPostProcessor_PostProcess_b,
		axl_log_TPostProcessor_Reset_b
	};

	axl_ref_TObject_Construct((axl_ref_TObject*) pPostProcessor, pStorage);
	pPostProcessor->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pPostProcessor, "axl_log_TPostProcessor");
}

void
AXL_VFT
axl_log_TPostProcessor_Destruct(axl_log_TPostProcessor* pPostProcessor)
{
	axl_ref_TObject_Destruct((axl_ref_TObject*) pPostProcessor);
}

bool_t 
AXL_VFT
axl_log_TPostProcessor_PostProcess_b(
	axl_log_TPostProcessor* pPostProcessor,
	axl_log_TMessageFile* pTargetFile,
	ulong_t Code,
	const void* p,
	size_t Size
	)
{
	return true;
}

bool_t 
AXL_API
axl_log_TPostProcessor_PostProcessFile(
	axl_log_TPostProcessor* pPostProcessor,
	axl_log_TMessageFile* pDstFile,
	axl_log_TMessageFile* pSrcFile,
	axl_log_TMessageFileHdr* pSrcFileHdrSnapshot
	)
{
	size_t Offset;
	size_t MsgEnd = pSrcFile->m_pHdr->m_MessageSize;

	if (pSrcFile->m_pHdr->m_ClearCount != pSrcFileHdrSnapshot->m_ClearCount)
 	{
		Offset = 0;
		axl_log_TMessageFile_Clear(pDstFile);
	}
	else
	{
		Offset = pSrcFileHdrSnapshot->m_MessageSize;
	}

	*pSrcFileHdrSnapshot = *pSrcFile->m_pHdr;

	while (Offset < MsgEnd)
	{
		bool_t Result;
		axl_log_TMessage* pMsg = axl_log_TMessageFile_GetMessage(pSrcFile, Offset);
		
		pDstFile->m_TimestampOverride = pMsg->m_Timestamp;

		Result = axl_log_TPostProcessor_PostProcess(pPostProcessor, pDstFile, pMsg->m_Code, pMsg + 1, pMsg->m_DataSize);
		if (!Result)
		{
			pDstFile->m_TimestampOverride = 0;
			return false;
		}

		Offset += sizeof(axl_log_TMessage) + pMsg->m_DataSize;
	}

	pDstFile->m_TimestampOverride = 0;
	return true;
}

//.............................................................................

typedef struct _axl_log_TPostProcessorEntry 
{ 
	axl_rtl_TListEntry;
	axl_log_TPostProcessor* m_pPostProcessor;
	axl_log_TMessageFile m_File;
	axl_log_TMessageFileHdr m_PreviousFileHdrSnapshot;
} axl_log_TPostProcessorEntry;

//.............................................................................

void
AXL_API
axl_log_TPostProcessorStack_Construct(axl_log_TPostProcessorStack* pStack)
{
	pStack->m_pOriginalFile = NULL;
    axl_rtl_TList_Construct(&pStack->m_Stack);
}

void
AXL_API
axl_log_TPostProcessorStack_Destruct(axl_log_TPostProcessorStack* pStack)
{
	axl_log_TPostProcessorStack_Clear(pStack);
}

bool_t
AXL_API
axl_log_TPostProcessorStack_SetOriginalFile(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TMessageFile* pFile
	)
{
	axl_log_TPostProcessorEntry* pEntry = (axl_log_TPostProcessorEntry*) axl_rtl_TList_GetTail(&pStack->m_Stack);
	if (pEntry)
		pEntry->m_PreviousFileHdrSnapshot.m_ClearCount = -1; // this will effectively force re-processing

	pStack->m_pOriginalFile = pFile;
	return true;
}

axl_log_TMessageFile*
AXL_API
axl_log_TPostProcessorStack_GetTargetFile(axl_log_TPostProcessorStack* pStack)
{
	axl_log_TPostProcessorEntry* pEntry = (axl_log_TPostProcessorEntry*) axl_rtl_TList_GetTail(&pStack->m_Stack);
	return pEntry ? &pEntry->m_File : pStack->m_pOriginalFile;
}

void
AXL_API
axl_log_TPostProcessorStack_Clear(axl_log_TPostProcessorStack* pStack)
{
	while (!axl_rtl_TList_IsEmpty(&pStack->m_Stack))
	{
		axl_log_TPostProcessorEntry* pEntry = (axl_log_TPostProcessorEntry*) axl_rtl_TList_RemoveHead(&pStack->m_Stack);
		axl_log_TMessageFile_Destruct(&pEntry->m_File);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pPostProcessor);
		axl_mem_Free(pEntry);
	}
}

axl_log_TPostProcessorEntry*
AXL_API
axl_log_TPostProcessorStack_FindPostProcessor(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TPostProcessor* pPostProcessor
	)
{
	axl_log_TPostProcessorEntry* pEntry = (axl_log_TPostProcessorEntry*) axl_rtl_TList_GetHead(&pStack->m_Stack);
	while (pEntry)
	{	
		if (pEntry->m_pPostProcessor == pPostProcessor)
			return pEntry;

		pEntry = (axl_log_TPostProcessorEntry*) axl_rtl_TList_GetNext(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	}

	return NULL;
}

axl_log_TMessageFile*
AXL_API
axl_log_TPostProcessorStack_AddPostProcessor(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TPostProcessor* pPostProcessor,
	const tchar_t* pFileName,
	int OpenFlags
	)
{
	bool_t Result;

	axl_log_TPostProcessorEntry* pEntry = (axl_log_TPostProcessorEntry*) axl_mem_Allocate(sizeof(axl_log_TPostProcessorEntry));
	axl_log_TMessageFile_Construct(&pEntry->m_File);

	Result = axl_log_TMessageFile_Open(&pEntry->m_File, pFileName, OpenFlags);
	if (!Result)
	{
		axl_log_TMessageFile_Destruct(&pEntry->m_File);
		axl_mem_Free(pEntry);
		return NULL;
	}

	pEntry->m_pPostProcessor = pPostProcessor;
	axl_ref_TObject_Reference((axl_ref_TObject*) pPostProcessor);
	memset(&pEntry->m_PreviousFileHdrSnapshot, 0, sizeof(axl_log_TMessageFileHdr));
	pEntry->m_PreviousFileHdrSnapshot.m_ClearCount = -1; // this will effectively force re-processing
	axl_rtl_TList_InsertTail(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	
	return &pEntry->m_File;
}

axl_log_TMessageFile*
AXL_API
axl_log_TPostProcessorStack_SetPostProcessor(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TPostProcessor* pPostProcessor,
	const tchar_t* pFileName,
	int OpenFlags
	)
{
	axl_log_TPostProcessorStack_Clear(pStack);
	return axl_log_TPostProcessorStack_AddPostProcessor(pStack, pPostProcessor, pFileName, OpenFlags);
}

bool_t
AXL_API
axl_log_TPostProcessorStack_RemovePostProcessor(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TPostProcessor* pPostProcessor
	)
{
	axl_log_TPostProcessorEntry* pEntry = axl_log_TPostProcessorStack_FindPostProcessor(pStack, pPostProcessor);
	if (pEntry)
		return false;

	axl_rtl_TList_Remove(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	axl_log_TMessageFile_Destruct(&pEntry->m_File);
	axl_ref_TObject_Dereference((axl_ref_TObject*) pPostProcessor);
	axl_mem_Free(pEntry);
	return true;
}

bool_t 
AXL_API
axl_log_TPostProcessorStack_PostProcess(axl_log_TPostProcessorStack* pStack)
{
	axl_log_TMessageFile* pFile;
	axl_log_TPostProcessorEntry* pEntry;

	if (!axl_log_TMessageFile_IsOpen(pStack->m_pOriginalFile))
		return false;

	pFile = pStack->m_pOriginalFile;
	pEntry = (axl_log_TPostProcessorEntry*) axl_rtl_TList_GetHead(&pStack->m_Stack);

	while (pEntry)
	{	
		bool_t Result = axl_log_TPostProcessor_PostProcessFile(pEntry->m_pPostProcessor, &pEntry->m_File, pFile, &pEntry->m_PreviousFileHdrSnapshot);
		if (!Result)
			return false;

		pFile = &pEntry->m_File;
		pEntry = (axl_log_TPostProcessorEntry*) axl_rtl_TList_GetNext(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	}

	return true;
}

//.............................................................................