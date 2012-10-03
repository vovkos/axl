// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_io_MappedFile.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

enum
{
	axl_log_EMessageFile_FileSignature    = ':gol', // log: signature for log files
	axl_log_EMessageFile_MessageSignature = ':gsm', // msg: signature for log messages
	axl_log_EMessageFile_CurrentVersion   = MAKELONG(0, MAKEWORD(1, 2)), // 2.1.0
};

enum
{
	axl_log_EMessageCode_Mergeable    = 0x80000000,
};

// log file structure

// log file is just a collection of messages tagged with code, 
// timestamp and optionally carrying extra binary data
//

//.............................................................................

typedef struct _axl_log_TMessageFileHdr
{
	ulong_t m_Signature; // axl_log_EMessageFileConst_LogFileSignature
	ulong_t m_Version;
	ulong_t m_MessageCount;
	ulong_t m_MessageSize;
//	ulong_t m_LastMessageOffset;
	ulong_t m_ClearCount;
	
	GUID m_ClassGuid;

	// followed by log data entries
} axl_log_TMessageFileHdr;

typedef struct _axl_log_TMessage
{
	ulong_t m_Signature;         // axl_log_EMessageFileConst_MessageSignature
	ulong_t m_PrevMessageOffset; // for walking back

	// actual payload

	ulonglong_t m_Timestamp;
	ulong_t m_Code;
	ulong_t m_DataSize; 

	// followed by message data (if any)

} axl_log_TMessage;

//.............................................................................

typedef struct _axl_log_TMessageFile axl_log_TMessageFile;

void
AXL_API
axl_log_TMessageFile_Construct(axl_log_TMessageFile* pFile);

AXL_INLINE
void
axl_log_TMessageFile_Destruct(axl_log_TMessageFile* pFile);

bool_t
AXL_API
axl_log_TMessageFile_Open(
	axl_log_TMessageFile* pFile,
	const tchar_t* pFileName,
	int Flags
	);

void
AXL_API
axl_log_TMessageFile_Close(axl_log_TMessageFile* pFile);

void
AXL_API
axl_log_TMessageFile_Clear(axl_log_TMessageFile* pFile);

AXL_INLINE
bool_t
axl_log_TMessageFile_IsOpen(axl_log_TMessageFile* pFile);

AXL_INLINE
bool_t
axl_log_TMessageFile_IsEmpty(axl_log_TMessageFile* pFile);

axl_log_TMessage*
AXL_API
axl_log_TMessageFile_GetMessage(
	axl_log_TMessageFile* pFile, 
	size_t Offset
	);

bool_t
AXL_API
axl_log_TMessageFile_Write(
	axl_log_TMessageFile* pFile,
	ulong_t Code,
	const void* p,
	size_t Size
	);

typedef struct _axl_log_TMessageFile
{
	axl_sys_TMappedFile m_File;
	axl_log_TMessageFileHdr* m_pHdr;
	ulonglong_t m_TimestampOverride;

#ifdef __cplusplus
	bool Open(const tchar_t* pFileName, int Flags = 0)
		{ return axl_log_TMessageFile_Open(this, pFileName, Flags) != 0; }
	
	void Close()
		{ axl_log_TMessageFile_Close(this); }

	void Clear()
		{ axl_log_TMessageFile_Clear(this); }

	bool IsOpen()
		{ return axl_log_TMessageFile_IsOpen(this) != 0; }

	bool IsEmpty()
		{ return axl_log_TMessageFile_IsEmpty(this) != 0; }

	axl_log_TMessage* GetMessage(size_t Offset)
		{ return axl_log_TMessageFile_GetMessage(this, Offset); }

	bool Write(ulong_t Code, const void* p = NULL, size_t Size = 0)
		{ return axl_log_TMessageFile_Write(this, Code, p, Size) != 0; }

#endif
} axl_log_TMessageFile;

//.............................................................................

AXL_INLINE
void 
axl_log_TMessageFile_Destruct(axl_log_TMessageFile* pFile)
{ 
	axl_sys_TMappedFile_Destruct(&pFile->m_File); 
}

AXL_INLINE
bool_t
axl_log_TMessageFile_IsOpen(axl_log_TMessageFile* pFile)
{
	return axl_sys_TMappedFile_IsOpen(&pFile->m_File);
}

AXL_INLINE
bool_t
axl_log_TMessageFile_IsEmpty(axl_log_TMessageFile* pFile)
{
	ASSERT(pFile->m_pHdr);
	return pFile->m_pHdr->m_MessageCount == 0;
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

struct EMessageFile { enum
{
	FileSignature    = axl_log_EMessageFile_FileSignature,
	MessageSignature = axl_log_EMessageFile_MessageSignature,
	CurrentVersion   = axl_log_EMessageFile_CurrentVersion,
};};

struct EMessageCode { enum
{
	Mergeable    = axl_log_EMessageCode_Mergeable,
};};

typedef axl_log_TMessageFile    TMessageFile;
typedef axl_log_TMessageFileHdr TMessageFileHdr;
typedef axl_log_TMessage        TMessage;

class CMessageFile: public TMessageFile
{
public:
	CMessageFile()
		{ axl_log_TMessageFile_Construct(this); }

	~CMessageFile()
		{ axl_log_TMessageFile_Destruct(this); }
};

} // namespace log 
} // namespace axl

#endif // __cplusplus

//.............................................................................
