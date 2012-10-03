// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_ColorizeSink.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TBinLinePart    axl_log_TBinLinePart;

//.............................................................................

typedef struct _axl_log_TColorizeBlock axl_log_TColorizeBlock;

struct _axl_log_TColorizeBlock
{
	ulong_t m_PartCode;
	size_t m_PartIndex;
	ulonglong_t m_Timestamp;
	size_t m_Offset;
	const void* m_p;
	size_t m_Size;
	axl_log_TBinLinePart* m_pPartArray;
	size_t m_PartCount;
};

//.............................................................................

#ifndef __cplusplus

typedef
void 
(AXL_VFT* 
 axl_log_FColorizerColorize)(
	void* pColorizer,
	axl_log_TColorizeSink* pSink,
	axl_log_TColorizeBlock* pBlock,
	bool_t SaveStateHint
	);

typedef struct _axl_log_TColorizerVTable axl_log_TColorizerVTable;

struct _axl_log_TColorizerVTable
{
	axl_ref_TObjectVTable;
	axl_log_FColorizerColorize m_pfnColorize;
};

#endif

typedef struct _axl_log_TColorizer axl_log_TColorizer;

void
AXL_API
axl_log_TColorizer_Construct(
	axl_log_TColorizer* pColorizer, 
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_TColorizer_Destruct(axl_log_TColorizer* pColorizer);

AXL_INLINE
void 
axl_log_TColorizer_Colorize(
	axl_log_TColorizer* pColorizer,
	axl_log_TColorizeSink* pSink,
	axl_log_TColorizeBlock* pBlock,
	bool_t SaveStateHint
	);

#ifdef __cplusplus
typedef struct AXL_NO_VTABLE _axl_log_TColorizer: axl_ref_TObject 
{
	virtual void AXL_VFT Colorize(axl_log_TColorizeSink* pSink, axl_log_TColorizeBlock* pBlock, bool_t SaveStateHint)
		{};

#else
typedef struct _axl_log_TColorizer { axl_ref_TObject;
#endif
} axl_log_TColorizer;

//.............................................................................

typedef struct _axl_log_TColorizerStack axl_log_TColorizerStack;

void
AXL_API
axl_log_TColorizerStack_Construct(
	axl_log_TColorizerStack* pStack,
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_TColorizerStack_Destruct(axl_log_TColorizerStack* pStack);

void
AXL_API
axl_log_TColorizerStack_Clear(axl_log_TColorizerStack* pStack);

bool_t
AXL_API
axl_log_TColorizerStack_ClearButFirst(axl_log_TColorizerStack* pStack);

bool_t
AXL_API
axl_log_TColorizerStack_AddColorizer(
	axl_log_TColorizerStack* pStack,
	axl_log_TColorizer* pColorizer
	);

bool_t
AXL_API
axl_log_TColorizerStack_RemoveColorizer(
	axl_log_TColorizerStack* pStack,
	axl_log_TColorizer* pColorizer
	);

void 
AXL_VFT
axl_log_TColorizerStack_Colorize(
	axl_log_TColorizerStack* pStack,
	axl_log_TColorizeSink* pSink,
	axl_log_TColorizeBlock* pBlock,
	bool_t SaveStateHint
	);

#ifdef __cplusplus
struct _axl_log_TColorizerStack: axl_log_TColorizer {
#else
struct _axl_log_TColorizerStack { axl_log_TColorizer;
#endif
	axl_rtl_TListEx m_Stack;

#ifdef __cplusplus
	void Clear()
		{ axl_log_TColorizerStack_Clear(this); }

	bool ClearButFirst()
		{ return axl_log_TColorizerStack_ClearButFirst(this) != 0; }

	bool AddColorizer(axl_log_TColorizer* pColorizer)
		{ return axl_log_TColorizerStack_AddColorizer(this, pColorizer) != 0; }

	bool RemoveColorizer(axl_log_TColorizer* pColorizer)
		{ return axl_log_TColorizerStack_RemoveColorizer(this, pColorizer) != 0; }
#endif
};

//.............................................................................

// inlines

AXL_INLINE
void 
axl_log_TColorizer_Colorize(
	axl_log_TColorizer* pColorizer,
	axl_log_TColorizeSink* pSink,
	axl_log_TColorizeBlock* pBlock,
	bool_t SaveStateHint
	)
{
#ifdef __cplusplus
	pColorizer->Colorize(pSink, pBlock, SaveStateHint);
#else
	((axl_log_TColorizerVTable*) pColorizer->m_pVTable)->m_pfnColorize(pColorizer, pSink, pBlock, SaveStateHint);
#endif
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TColorizer      TColorizer;
typedef axl_log_TColorizerStack TColorizerStack;
typedef axl_log_TColorizeBlock  TColorizeBlock;

class AXL_NO_VTABLE CColorizer: public TColorizer
{
public:
	CColorizer(axl::ref::TBuffer* pStorage)
		{ axl_log_TColorizer_Construct(this, pStorage); }

	~CColorizer()
		{ axl_log_TColorizer_Destruct(this); }
};

class AXL_NO_VTABLE CColorizerStack: public TColorizerStack
{
public:
	CColorizerStack(axl::ref::TBuffer* pStorage)
		{ axl_log_TColorizerStack_Construct(this, pStorage); }

	~CColorizerStack()
		{ axl_log_TColorizerStack_Destruct(this); }
};

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
