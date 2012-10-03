#include "axl_Precomp.h"
#include "axl_log_MessageFile.h"
#include "axl_rtl_Packer.h"
#include "axl_err_Error.h"

//.............................................................................

void
AXL_API
axl_log_TMessageFile_Construct(axl_log_TMessageFile* pFile)
{
	axl_sys_TMappedFile_Construct(&pFile->m_File);
	pFile->m_pHdr = NULL;
	pFile->m_TimestampOverride = 0;
}

bool_t
AXL_API
axl_log_TMessageFile_Open(
	axl_log_TMessageFile* pFile,
	const tchar_t* pFileName,
	int Flags
	)
{
	bool_t Result;

	size_t FileSize;
	size_t ActualSize;

	axl_log_TMessageFile_Close(pFile);

	Result = axl_sys_TMappedFile_Open(&pFile->m_File, pFileName, Flags);
	if (!Result)
		return false;

	pFile->m_pHdr = (axl_log_TMessageFileHdr*) axl_sys_TMappedFile_ViewEx(
		&pFile->m_File, 
		0, sizeof(axl_log_TMessageFileHdr), 
		true
		);
	
	FileSize = sizeof(axl_log_TMessageFileHdr) + pFile->m_pHdr->m_MessageSize;
	ActualSize = axl_sys_TMappedFile_GetSize(&pFile->m_File);

	if (pFile->m_pHdr->m_Signature == axl_log_EMessageFile_FileSignature && 
		pFile->m_pHdr->m_Version == axl_log_EMessageFile_CurrentVersion &&
		FileSize <= ActualSize)
	{
		if (FileSize != ActualSize)
			axl_sys_TMappedFile_SetSize(&pFile->m_File, FileSize);
		return true; // existing file is OK
	}

	if (Flags & axl_sys_EFile_ReadOnly)
	{
		axl_err_SetLastError(axl_err_EStatus_InvalidDeviceRequest);
		return false;
	}

	pFile->m_pHdr->m_Signature = axl_log_EMessageFile_FileSignature;
	pFile->m_pHdr->m_Version = axl_log_EMessageFile_CurrentVersion;
	pFile->m_pHdr->m_MessageCount = 0;
	pFile->m_pHdr->m_MessageSize = 0;
//	pFile->m_pHdr->m_LastMessageOffset = 0;
	pFile->m_pHdr->m_ClearCount = 0;
	pFile->m_pHdr->m_ClassGuid = GUID_NULL;

	axl_sys_TMappedFile_SetSize(&pFile->m_File, sizeof(axl_log_TMessageFileHdr));
	return true;
}

void
AXL_API
axl_log_TMessageFile_Close(axl_log_TMessageFile* pFile)
{
	axl_sys_TMappedFile_Close(&pFile->m_File);
	pFile->m_pHdr = NULL;
}

void
AXL_API
axl_log_TMessageFile_Clear(axl_log_TMessageFile* pFile)
{
	ASSERT(pFile->m_pHdr);

	pFile->m_pHdr->m_MessageCount = 0;
	pFile->m_pHdr->m_MessageSize = 0;
	pFile->m_pHdr->m_ClearCount++;

	axl_sys_TMappedFile_SetSize(&pFile->m_File, sizeof(axl_log_TMessageFileHdr));
}

axl_log_TMessage*
AXL_API
axl_log_TMessageFile_GetMessage(
	axl_log_TMessageFile* pFile, 
	size_t Offset
	)
{
	axl_log_TMessage* pMsg;

	ASSERT(pFile->m_pHdr);

	if (Offset >= pFile->m_pHdr->m_MessageSize)
		return NULL;

	Offset += sizeof(axl_log_TMessageFileHdr);

	pMsg = (axl_log_TMessage*) axl_sys_TMappedFile_View(&pFile->m_File, Offset, sizeof(axl_log_TMessage));

	ASSERT(pMsg); 
	ASSERT(pMsg->m_Signature == axl_log_EMessageFile_MessageSignature); 

	if (pMsg->m_DataSize) // make sure mapping covers data as well
		pMsg = (axl_log_TMessage*) axl_sys_TMappedFile_View(&pFile->m_File, Offset, sizeof(axl_log_TMessage) + pMsg->m_DataSize);
	
	return pMsg;
}

bool_t
AXL_API
axl_log_TMessageFile_Write(
	axl_log_TMessageFile* pFile,
	ulong_t Code,
	const void* p,
	size_t Size
	)
{
	axl_log_TMessage* pMsg;

	size_t Offset;
	size_t FullSize;

	ASSERT(pFile->m_pHdr);

	Offset = sizeof(axl_log_TMessageFileHdr) + pFile->m_pHdr->m_MessageSize;
	FullSize = sizeof(axl_log_TMessage) + Size;
	pMsg = (axl_log_TMessage*) axl_sys_TMappedFile_View(&pFile->m_File, Offset, FullSize); 
	if (!pMsg)
		return false;

	pMsg->m_Signature = axl_log_EMessageFile_MessageSignature;
//	pMsg->m_PrevMessageOffset = pFile->m_pHdr->m_LastMessageOffset;
	pMsg->m_Code = Code;
	pMsg->m_DataSize = (ulong_t) Size;

	if (pFile->m_TimestampOverride)
		pMsg->m_Timestamp = pFile->m_TimestampOverride;
	else
		GetSystemTimeAsFileTime((FILETIME*) &pMsg->m_Timestamp);

	if (Size)
		memcpy(pMsg + 1, p, Size);

//	pFile->m_pHdr->m_LastMessageOffset = pFile->m_pHdr->m_MessageSize;
	pFile->m_pHdr->m_MessageCount++;
	pFile->m_pHdr->m_MessageSize += (ulong_t) FullSize;

	axl_sys_TMappedFile_SetSize(&pFile->m_File, Offset + FullSize);
	return true;
}

//.............................................................................
