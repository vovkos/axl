// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_MessageFile.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

#ifndef __cplusplus

typedef
bool_t 
(AXL_VFT* 
 axl_log_FPostProcessorPostProcess)(
	void* pPostProcessor,
	axl_log_TMessageFile* pTargetFile,
	ulong_t Code,
	const void* p,
	size_t Size
	);

typedef
void 
(AXL_VFT* 
 axl_log_FPostProcessorReset)(void* pPostProcessor);

typedef struct _axl_log_TPostProcessorVTable axl_log_TPostProcessorVTable;

struct _axl_log_TPostProcessorVTable
{
	axl_ref_TObjectVTable;
	axl_log_FPostProcessorPostProcess m_pfnPostProcess;
	axl_log_FPostProcessorReset m_pfnReset;
};

#endif

typedef struct _axl_log_TPostProcessor   axl_log_TPostProcessor;

void
AXL_API
axl_log_TPostProcessor_Construct(
	axl_log_TPostProcessor* pPostProcessor, 
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_TPostProcessor_Destruct(axl_log_TPostProcessor* pPostProcessor);

AXL_INLINE
bool_t 
axl_log_TPostProcessor_PostProcess(
	axl_log_TPostProcessor* pPostProcessor,
	axl_log_TMessageFile* pTargetFile,
	ulong_t Code,
	const void* p,
	size_t Size
	);

bool_t 
AXL_VFT
axl_log_TPostProcessor_PostProcess_b(
	axl_log_TPostProcessor* pPostProcessor,
	axl_log_TMessageFile* pTargetFile,
	ulong_t Code,
	const void* p,
	size_t Size
	);

#ifdef __cplusplus
typedef struct AXL_NO_VTABLE _axl_log_TPostProcessor: axl_ref_TObject 
{
	virtual bool_t AXL_VFT PostProcess(axl_log_TMessageFile* pTargetFile, ulong_t Code, const void* p, size_t Size)
		{ return axl_log_TPostProcessor_PostProcess_b(this, pTargetFile, Code, p, Size); }

	virtual void AXL_VFT Reset()
		{ }
#else
typedef struct _axl_log_TPostProcessor { axl_ref_TObject;
#endif
} axl_log_TPostProcessor;

//.............................................................................

typedef struct _axl_log_TPostProcessorStack axl_log_TPostProcessorStack;

void
AXL_API
axl_log_TPostProcessorStack_Construct(axl_log_TPostProcessorStack* pStack);

void
AXL_API
axl_log_TPostProcessorStack_Destruct(axl_log_TPostProcessorStack* pStack);

void
AXL_API
axl_log_TPostProcessorStack_Clear(axl_log_TPostProcessorStack* pStack);

bool_t
AXL_API
axl_log_TPostProcessorStack_SetOriginalFile(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TMessageFile* pFile
	);

axl_log_TMessageFile*
AXL_API
axl_log_TPostProcessorStack_GetTargetFile(axl_log_TPostProcessorStack* pStack);

axl_log_TMessageFile*
AXL_API
axl_log_TPostProcessorStack_AddPostProcessor(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TPostProcessor* pPostProcessor,
	const tchar_t* pFileName,
	int OpenFlags
	);

axl_log_TMessageFile*
AXL_API
axl_log_TPostProcessorStack_SetPostProcessor(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TPostProcessor* pPostProcessor,
	const tchar_t* pFileName,
	int OpenFlags
	);

bool_t
AXL_API
axl_log_TPostProcessorStack_RemovePostProcessor(
	axl_log_TPostProcessorStack* pStack,
	axl_log_TPostProcessor* pPostProcessor
	);

bool_t 
AXL_API
axl_log_TPostProcessorStack_PostProcess(axl_log_TPostProcessorStack* pStack);

typedef struct _axl_log_TPostProcessorStack
{
	axl_log_TMessageFile* m_pOriginalFile;
	axl_rtl_TList m_Stack;

#ifdef __cplusplus
	void Clear()
		{ axl_log_TPostProcessorStack_Clear(this); }

	bool SetOriginalFile(axl_log_TMessageFile* pFile)
		{ return axl_log_TPostProcessorStack_SetOriginalFile(this, pFile) != 0; }

	axl_log_TMessageFile* GetTargetFile()
		{ return axl_log_TPostProcessorStack_GetTargetFile(this); }

	axl_log_TMessageFile* SetPostProcessor(axl_log_TPostProcessor* pPostProcessor, const tchar_t* pFileName, int OpenFlags = 0)
		{ return axl_log_TPostProcessorStack_SetPostProcessor(this, pPostProcessor, pFileName, OpenFlags); }

	axl_log_TMessageFile* AddPostProcessor(axl_log_TPostProcessor* pPostProcessor, const tchar_t* pFileName, int OpenFlags = 0)
		{ return axl_log_TPostProcessorStack_AddPostProcessor(this, pPostProcessor, pFileName, OpenFlags); }

	bool RemovePostProcessor(axl_log_TPostProcessor* pPostProcessor)
		{ return axl_log_TPostProcessorStack_RemovePostProcessor(this, pPostProcessor) != 0; }

	bool PostProcess()
		{ return axl_log_TPostProcessorStack_PostProcess(this) != 0; }
#endif
} axl_log_TPostProcessorStack;

//.............................................................................

AXL_INLINE
bool_t 
axl_log_TPostProcessor_PostProcess(
	axl_log_TPostProcessor* pPostProcessor,
	axl_log_TMessageFile* pTargetFile,
	ulong_t Code,
	const void* p,
	size_t Size
	)
{
#ifdef __cplusplus
	return pPostProcessor->PostProcess(pTargetFile, Code, p, Size);
#else
	return ((axl_log_TPostProcessorVTable*) pPostProcessor->m_pVTable)->m_pfnPostProcess(pPostProcessor, pTargetFile, Code, p, Size);
#endif
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TPostProcessor      TPostProcessor;
typedef axl_log_TPostProcessorStack TPostProcessorStack;

class AXL_NO_VTABLE CPostProcessor: public TPostProcessor
{
public:
	CPostProcessor(axl::ref::TBuffer* pStorage)
		{ axl_log_TPostProcessor_Construct(this, pStorage); }

	~CPostProcessor()
		{ axl_log_TPostProcessor_Destruct(this); }
};

class AXL_NO_VTABLE CPostProcessorStack: public TPostProcessorStack
{
public:
	CPostProcessorStack()
		{ axl_log_TPostProcessorStack_Construct(this); }

	~CPostProcessorStack()
		{ axl_log_TPostProcessorStack_Destruct(this); }
};

} // namespace log 
} // namespace axl

#endif // __cplusplus

//.............................................................................
